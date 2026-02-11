#include "raylib.h"
#include <deque>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdint>

struct Cell {
    int x;
    int y;
};

struct Particle {
    Vector2 pos;
    Vector2 vel;
    float life;
    float size;
    Color color;
};

enum class PowerUpType {
    None,
    Slow,
    DoubleScore
};

static bool SameCell(const Cell& a, const Cell& b) {
    return a.x == b.x && a.y == b.y;
}

static bool Contains(const std::deque<Cell>& list, const Cell& c) {
    for (const auto& s : list) {
        if (SameCell(s, c)) return true;
    }
    return false;
}

static Cell RandomEmptyCell(int cols, int rows, const std::deque<Cell>& snake, const std::deque<Cell>& obstacles) {
    while (true) {
        Cell c{ GetRandomValue(0, cols - 1), GetRandomValue(0, rows - 1) };
        if (!Contains(snake, c) && !Contains(obstacles, c)) return c;
    }
}

static int LoadHighScore(const char* path) {
    FILE* f = nullptr;
    if (fopen_s(&f, path, "r") != 0 || !f) return 0;
    int value = 0;
    fscanf_s(f, "%d", &value);
    fclose(f);
    return value;
}

static void SaveHighScore(const char* path, int score) {
    FILE* f = nullptr;
    if (fopen_s(&f, path, "w") != 0 || !f) return;
    fprintf(f, "%d", score);
    fclose(f);
}

static Color LerpColor(Color a, Color b, float t) {
    return Color{
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        (unsigned char)(a.a + (b.a - a.a) * t)
    };
}

static void EnsureToneWav(const char* path, float frequency, float seconds) {
    if (FileExists(path)) return;

    if (!DirectoryExists("Resources")) {
        MakeDirectory("Resources");
    }

    const int sampleRate = 44100;
    const int numChannels = 1;
    const int bitsPerSample = 16;
    const int samples = (int)(sampleRate * seconds);
    const int dataSize = samples * numChannels * (bitsPerSample / 8);
    const int byteRate = sampleRate * numChannels * (bitsPerSample / 8);
    const int blockAlign = numChannels * (bitsPerSample / 8);
    const int chunkSize = 36 + dataSize;

    FILE* f = nullptr;
    if (fopen_s(&f, path, "wb") != 0 || !f) return;

    fwrite("RIFF", 1, 4, f);
    fwrite(&chunkSize, 4, 1, f);
    fwrite("WAVE", 1, 4, f);

    fwrite("fmt ", 1, 4, f);
    int subChunk1Size = 16;
    short audioFormat = 1;
    fwrite(&subChunk1Size, 4, 1, f);
    fwrite(&audioFormat, 2, 1, f);
    fwrite(&numChannels, 2, 1, f);
    fwrite(&sampleRate, 4, 1, f);
    fwrite(&byteRate, 4, 1, f);
    fwrite(&blockAlign, 2, 1, f);
    fwrite(&bitsPerSample, 2, 1, f);

    fwrite("data", 1, 4, f);
    fwrite(&dataSize, 4, 1, f);

    for (int i = 0; i < samples; i++) {
        float t = (float)i / (float)sampleRate;
        float s = sinf(2.0f * PI * frequency * t);
        int16_t sample = (int16_t)(s * 0.35f * 32767);
        fwrite(&sample, sizeof(int16_t), 1, f);
    }

    fclose(f);
}

int main() {
    const int cellSize = 20;
    const int cols = 45;
    const int rows = 28;
    const int uiHeight = 70;
    const int width = cols * cellSize;
    const int height = rows * cellSize + uiHeight;

    InitWindow(width, height, "Snake - Enhanced Graphics");
    InitAudioDevice();
    SetTargetFPS(60);

    const char* eatPath = "Resources/eat.wav";
    const char* powerPath = "Resources/power.wav";
    const char* gameOverPath = "Resources/gameover.wav";
    const char* highScorePath = "highscore.txt";

    EnsureToneWav(eatPath, 660.0f, 0.08f);
    EnsureToneWav(powerPath, 880.0f, 0.12f);
    EnsureToneWav(gameOverPath, 220.0f, 0.22f);

    bool audioReady = IsAudioDeviceReady();
    bool soundsReady = audioReady && FileExists(eatPath) && FileExists(powerPath) && FileExists(gameOverPath);

    Sound eatSound{};
    Sound powerSound{};
    Sound gameOverSound{};

    if (soundsReady) {
        eatSound = LoadSound(eatPath);
        powerSound = LoadSound(powerPath);
        gameOverSound = LoadSound(gameOverPath);
    }

    bool muted = false;
    auto ApplyVolumes = [&]() {
        if (!soundsReady) return;
        float v = muted ? 0.0f : 0.9f;
        SetSoundVolume(eatSound, v);
        SetSoundVolume(powerSound, v);
        SetSoundVolume(gameOverSound, v);
    };
    ApplyVolumes();

    std::deque<Cell> snake;
    std::deque<Cell> obstacles;
    std::vector<Particle> particles;

    Cell dir{ 1, 0 };
    Cell food{ 0, 0 };
    Cell powerUp{ 0, 0 };
    Cell bossFood{ 0, 0 };

    int score = 0;
    int highScore = LoadHighScore(highScorePath);
    bool gameOver = false;
    bool paused = false;

    float baseMoveDelay = 0.12f;
    float timer = 0.0f;
    float foodPulse = 0.0f;
    float animTime = 0.0f;
    float shakeTime = 0.0f;
    float shakeStrength = 0.0f;
    float dayTime = 0.0f;

    bool powerUpOnField = false;
    PowerUpType powerUpType = PowerUpType::None;
    PowerUpType activePower = PowerUpType::None;
    float powerUpTimer = 0.0f;
    float powerUpSpawnTimer = 0.0f;

    bool bossActive = false;
    float bossSpawnTimer = 0.0f;
    float bossTimer = 0.0f;
    float bossMoveTimer = 0.0f;

    int comboCount = 0;
    float comboTimer = 0.0f;

    int lastObstacleScore = 0;

    const int obstacleCount = 35;
    const float powerUpSpawnInterval = 12.0f;
    const float slowDuration = 6.0f;
    const float doubleDuration = 8.0f;

    const float bossSpawnInterval = 18.0f;
    const float bossDuration = 8.0f;
    const float bossMoveInterval = 0.4f;

    const float comboWindow = 2.2f;

    auto PlayIfReady = [&](Sound& s) {
        if (soundsReady && !muted) PlaySound(s);
    };

    auto SpawnParticles = [&](Vector2 origin, Color color, int count, float minSpeed, float maxSpeed, float life, float size) {
        for (int i = 0; i < count; i++) {
            float angle = (float)i / (float)count * 2.0f * PI;
            float speed = (float)GetRandomValue((int)minSpeed, (int)maxSpeed);
            particles.push_back(Particle{
                origin,
                { cosf(angle) * speed, sinf(angle) * speed },
                life,
                size,
                color
            });
        }
    };

    auto SpawnFood = [&]() {
        Cell c;
        do {
            c = RandomEmptyCell(cols, rows, snake, obstacles);
        } while ((powerUpOnField && SameCell(c, powerUp)) || (bossActive && SameCell(c, bossFood)));
        return c;
    };

    auto SpawnPowerUp = [&]() {
        Cell c;
        do {
            c = RandomEmptyCell(cols, rows, snake, obstacles);
        } while (SameCell(c, food) || (bossActive && SameCell(c, bossFood)));
        powerUp = c;
        powerUpType = (GetRandomValue(0, 1) == 0) ? PowerUpType::Slow : PowerUpType::DoubleScore;
        powerUpOnField = true;
    };

    auto SpawnBossFood = [&]() {
        Cell c;
        do {
            c = RandomEmptyCell(cols, rows, snake, obstacles);
        } while (SameCell(c, food) || (powerUpOnField && SameCell(c, powerUp)));
        bossFood = c;
        bossActive = true;
        bossTimer = bossDuration;
        bossMoveTimer = bossMoveInterval;
    };

    auto RegenerateObstacles = [&]() {
        obstacles.clear();
        for (int i = 0; i < obstacleCount; i++) {
            Cell o = RandomEmptyCell(cols, rows, snake, obstacles);
            if (!SameCell(o, snake.front()) && !SameCell(o, food) && !(powerUpOnField && SameCell(o, powerUp)) && !(bossActive && SameCell(o, bossFood))) {
                obstacles.push_back(o);
            }
        }
    };

    auto SetGameOver = [&]() {
        if (!gameOver) {
            gameOver = true;
            PlayIfReady(gameOverSound);
            shakeTime = 0.25f;
            shakeStrength = 7.0f;
            if (score > highScore) {
                highScore = score;
                SaveHighScore(highScorePath, highScore);
            }
        }
    };

    auto ResetGame = [&]() {
        snake.clear();
        obstacles.clear();
        particles.clear();

        snake.push_back({ cols / 2, rows / 2 });
        snake.push_back({ cols / 2 - 1, rows / 2 });
        snake.push_back({ cols / 2 - 2, rows / 2 });

        dir = { 1, 0 };
        score = 0;
        gameOver = false;
        paused = false;
        timer = 0.0f;
        baseMoveDelay = 0.12f;

        powerUpOnField = false;
        powerUpType = PowerUpType::None;
        activePower = PowerUpType::None;
        powerUpTimer = 0.0f;
        powerUpSpawnTimer = 0.0f;

        bossActive = false;
        bossSpawnTimer = 0.0f;
        bossTimer = 0.0f;
        bossMoveTimer = 0.0f;

        comboCount = 0;
        comboTimer = 0.0f;
        lastObstacleScore = 0;

        food = SpawnFood();
        RegenerateObstacles();
    };

    ResetGame();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        foodPulse += dt * 5.0f;
        animTime += dt * 6.0f;
        dayTime += dt * 0.08f;

        if (shakeTime > 0.0f) {
            shakeTime -= dt;
            if (shakeTime <= 0.0f) shakeStrength = 0.0f;
        }

        if (comboTimer > 0.0f) {
            comboTimer -= dt;
            if (comboTimer <= 0.0f) comboCount = 0;
        }

        if (activePower != PowerUpType::None) {
            powerUpTimer -= dt;
            if (powerUpTimer <= 0.0f) {
                activePower = PowerUpType::None;
                powerUpTimer = 0.0f;
            }
        }

        // ===== INPUT =====
        if (IsKeyPressed(KEY_P) && !gameOver) paused = !paused;
        if (IsKeyPressed(KEY_M)) { muted = !muted; ApplyVolumes(); }

        if (!gameOver && !paused) {
            if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && dir.y != 1) dir = { 0, -1 };
            if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && dir.y != -1) dir = { 0, 1 };
            if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && dir.x != 1) dir = { -1, 0 };
            if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && dir.x != -1) dir = { 1, 0 };
        }

        if (gameOver && IsKeyPressed(KEY_R)) ResetGame();

        // ===== UPDATE =====
        if (!gameOver && !paused) {
            powerUpSpawnTimer += dt;
            if (!powerUpOnField && activePower == PowerUpType::None && powerUpSpawnTimer >= powerUpSpawnInterval) {
                powerUpSpawnTimer = 0.0f;
                SpawnPowerUp();
            }

            bossSpawnTimer += dt;
            if (!bossActive && bossSpawnTimer >= bossSpawnInterval) {
                bossSpawnTimer = 0.0f;
                SpawnBossFood();
            }

            if (bossActive) {
                bossTimer -= dt;
                bossMoveTimer -= dt;
                if (bossTimer <= 0.0f) {
                    bossActive = false;
                }
                else if (bossMoveTimer <= 0.0f) {
                    bossMoveTimer = bossMoveInterval;
                    Cell next = bossFood;
                    int dirIndex = GetRandomValue(0, 3);
                    if (dirIndex == 0) next.x++;
                    if (dirIndex == 1) next.x--;
                    if (dirIndex == 2) next.y++;
                    if (dirIndex == 3) next.y--;

                    if (next.x < 0) next.x = cols - 1;
                    if (next.x >= cols) next.x = 0;
                    if (next.y < 0) next.y = rows - 1;
                    if (next.y >= rows) next.y = 0;

                    if (!Contains(obstacles, next) && !Contains(snake, next) && !SameCell(next, food) && !(powerUpOnField && SameCell(next, powerUp))) {
                        bossFood = next;
                    }
                }
            }

            float speedFactor = (activePower == PowerUpType::Slow) ? 1.6f : 1.0f;
            float currentMoveDelay = baseMoveDelay * speedFactor;

            timer += dt;

            if (timer >= currentMoveDelay) {
                timer = 0.0f;
                Cell newHead{ snake.front().x + dir.x, snake.front().y + dir.y };

                bool usedPortal = false;
                if (newHead.x < 0) { newHead.x = cols - 1; usedPortal = true; }
                if (newHead.x >= cols) { newHead.x = 0; usedPortal = true; }
                if (newHead.y < 0) { newHead.y = rows - 1; usedPortal = true; }
                if (newHead.y >= rows) { newHead.y = 0; usedPortal = true; }

                if (Contains(obstacles, newHead)) {
                    SetGameOver();
                }
                else {
                    bool willEat = SameCell(newHead, food);
                    bool willPower = powerUpOnField && SameCell(newHead, powerUp);
                    bool willBoss = bossActive && SameCell(newHead, bossFood);

                    bool hitsSelf = Contains(snake, newHead);
                    if (hitsSelf && !(SameCell(newHead, snake.back()) && !willEat && !willBoss)) {
                        SetGameOver();
                    }
                    else {
                        snake.push_front(newHead);

                        if (usedPortal) {
                            Vector2 portalPos = { newHead.x * (float)cellSize + cellSize * 0.5f, newHead.y * (float)cellSize + cellSize * 0.5f };
                            SpawnParticles(portalPos, Color{ 120, 200, 255, 200 }, 10, 40, 80, 0.35f, 3.0f);
                        }

                        if (willPower) {
                            powerUpOnField = false;
                            activePower = powerUpType;
                            powerUpTimer = (activePower == PowerUpType::Slow) ? slowDuration : doubleDuration;
                            PlayIfReady(powerSound);
                            shakeTime = 0.12f;
                            shakeStrength = 4.0f;
                        }

                        if (willEat || willBoss) {
                            int baseScore = willBoss ? 50 : ((activePower == PowerUpType::DoubleScore) ? 20 : 10);

                            if (comboTimer > 0.0f) comboCount++;
                            else comboCount = 1;
                            comboTimer = comboWindow;

                            float comboMult = 1.0f + (comboCount > 1 ? (comboCount - 1) * 0.25f : 0.0f);
                            int addScore = (int)roundf(baseScore * comboMult);

                            score += addScore;
                            if (baseMoveDelay > 0.06f) baseMoveDelay -= 0.003f;

                            Vector2 foodPos = { newHead.x * (float)cellSize + cellSize * 0.5f, newHead.y * (float)cellSize + cellSize * 0.5f };
                            SpawnParticles(foodPos, willBoss ? Color{ 255, 180, 90, 220 } : Color{ 255, 120, 120, 220 }, willBoss ? 40 : 24, 40, 120, 0.8f, willBoss ? 4.0f : 3.0f);
                            PlayIfReady(eatSound);

                            shakeTime = willBoss ? 0.3f : 0.18f;
                            shakeStrength = willBoss ? 7.0f : 5.0f;

                            if (willBoss) bossActive = false;

                            food = SpawnFood();
                        }
                        else {
                            snake.pop_back();
                        }
                    }
                }
            }

            if (score - lastObstacleScore >= 50) {
                lastObstacleScore = score;
                RegenerateObstacles();
            }

            // Trail particles
            Vector2 tailPos = { snake.back().x * (float)cellSize + cellSize * 0.5f, snake.back().y * (float)cellSize + cellSize * 0.5f };
            particles.push_back(Particle{ tailPos, { 0.0f, 0.0f }, 0.25f, 2.0f, Color{ 80, 220, 120, 120 } });
        }

        // Particle update
        for (int i = (int)particles.size() - 1; i >= 0; i--) {
            particles[i].life -= dt;
            particles[i].pos.x += particles[i].vel.x * dt;
            particles[i].pos.y += particles[i].vel.y * dt;
            particles[i].vel.x *= 0.95f;
            particles[i].vel.y *= 0.95f;
            if (particles[i].life <= 0.0f) particles.erase(particles.begin() + i);
        }

        // Screen shake offset (gameplay only)
        float shakeX = (shakeTime > 0.0f) ? (float)GetRandomValue(-100, 100) / 100.0f * shakeStrength : 0.0f;
        float shakeY = (shakeTime > 0.0f) ? (float)GetRandomValue(-100, 100) / 100.0f * shakeStrength : 0.0f;

        // ===== DRAW =====
        BeginDrawing();

        float dayFactor = (sinf(dayTime) * 0.5f + 0.5f);
        Color bgTop = LerpColor(Color{ 15, 18, 28, 255 }, Color{ 30, 35, 60, 255 }, dayFactor);
        Color bgBottom = LerpColor(Color{ 8, 10, 18, 255 }, Color{ 18, 22, 40, 255 }, dayFactor);

        DrawRectangleGradientV(0, 0, width, height, bgTop, bgBottom);

        // Playfield
        DrawRectangle((int)shakeX, (int)shakeY, cols * cellSize, rows * cellSize, Color{ 40, 40, 60, 255 });

        for (int x = 0; x <= cols; x++) {
            DrawLine((int)shakeX + x * cellSize, (int)shakeY, (int)shakeX + x * cellSize, (int)shakeY + rows * cellSize, Color{ 50, 50, 70, 80 });
        }
        for (int y = 0; y <= rows; y++) {
            DrawLine((int)shakeX, (int)shakeY + y * cellSize, (int)shakeX + cols * cellSize, (int)shakeY + y * cellSize, Color{ 50, 50, 70, 80 });
        }

        // Obstacles
        for (const auto& o : obstacles) {
            int px = o.x * cellSize + (int)shakeX;
            int py = o.y * cellSize + (int)shakeY;
            DrawRectangle(px, py, cellSize, cellSize, Color{ 90, 90, 110, 255 });
            DrawRectangle(px + 2, py + 2, cellSize - 4, cellSize - 4, Color{ 70, 70, 90, 255 });
            DrawRectangleLines(px, py, cellSize, cellSize, Color{ 110, 110, 130, 255 });
        }

        // Food (pulse)
        float pulse = sinf(foodPulse) * 0.5f + 0.5f;
        int foodOffset = (int)(pulse * 3);
        int fx = food.x * cellSize + foodOffset + (int)shakeX;
        int fy = food.y * cellSize + foodOffset + (int)shakeY;
        int fsz = cellSize - foodOffset * 2;

        DrawRectangle(food.x * cellSize - 3 + (int)shakeX, food.y * cellSize - 3 + (int)shakeY, cellSize + 6, cellSize + 6,
            Color{ 255, 80, 80, 70 });
        DrawRectangle(fx, fy, fsz, fsz, Color{ 255, 50, 50, 255 });
        DrawRectangle(fx + 3, fy + 3, fsz - 6, fsz - 6, Color{ 255, 150, 150, 255 });

        // Boss food
        if (bossActive) {
            int bx = bossFood.x * cellSize + (int)shakeX;
            int by = bossFood.y * cellSize + (int)shakeY;
            DrawRectangle(bx - 4, by - 4, cellSize + 8, cellSize + 8, Color{ 255, 180, 90, 80 });
            DrawRectangle(bx, by, cellSize, cellSize, Color{ 255, 150, 50, 255 });
            DrawRectangleLines(bx - 2, by - 2, cellSize + 4, cellSize + 4, Color{ 255, 210, 130, 200 });
        }

        // Power-up
        if (powerUpOnField) {
            Color c = (powerUpType == PowerUpType::Slow) ? Color{ 80, 160, 255, 255 } : Color{ 255, 215, 120, 255 };
            const char* text = (powerUpType == PowerUpType::Slow) ? "S" : "2x";
            int px = powerUp.x * cellSize + cellSize / 2 + (int)shakeX;
            int py = powerUp.y * cellSize + cellSize / 2 + (int)shakeY;
            int r = cellSize / 2 - 2;
            DrawCircle(px, py, (float)r, c);
            DrawCircleLines(px, py, (float)r, Color{ 255, 255, 255, 160 });
            int tw = MeasureText(text, 14);
            DrawText(text, px - tw / 2, py - 7, 14, RAYWHITE);
        }

        // Particles
        for (const auto& p : particles) {
            unsigned char a = (unsigned char)(p.life * 255);
            Color c = p.color;
            c.a = a;
            DrawCircleV({ p.pos.x + shakeX, p.pos.y + shakeY }, p.size, c);
        }

        // Snake with tail wave + glow
        for (int i = 0; i < (int)snake.size(); i++) {
            int sx = snake[i].x * cellSize + (int)shakeX;
            int sy = snake[i].y * cellSize + (int)shakeY;

            float wave = sinf(animTime + i * 0.6f) * 1.4f;
            sx += (int)wave;
            sy += (int)wave;

            if (i == 0) {
                DrawRectangle(sx - 3, sy - 3, cellSize + 6, cellSize + 6, Color{ 80, 255, 140, 70 });
                DrawRectangle(sx, sy, cellSize, cellSize, Color{ 50, 200, 80, 255 });
                DrawRectangle(sx + 3, sy + 3, cellSize - 6, cellSize - 6, Color{ 80, 255, 120, 255 });
                DrawRectangleLines(sx, sy, cellSize, cellSize, Color{ 100, 255, 150, 255 });

                int eyeSize = 3;
                if (dir.x == 1) {
                    DrawRectangle(sx + cellSize - 6, sy + 4, eyeSize, eyeSize, BLACK);
                    DrawRectangle(sx + cellSize - 6, sy + cellSize - 7, eyeSize, eyeSize, BLACK);
                }
                else if (dir.x == -1) {
                    DrawRectangle(sx + 3, sy + 4, eyeSize, eyeSize, BLACK);
                    DrawRectangle(sx + 3, sy + cellSize - 7, eyeSize, eyeSize, BLACK);
                }
                else if (dir.y == -1) {
                    DrawRectangle(sx + 4, sy + 3, eyeSize, eyeSize, BLACK);
                    DrawRectangle(sx + cellSize - 7, sy + 3, eyeSize, eyeSize, BLACK);
                }
                else {
                    DrawRectangle(sx + 4, sy + cellSize - 6, eyeSize, eyeSize, BLACK);
                    DrawRectangle(sx + cellSize - 7, sy + cellSize - 6, eyeSize, eyeSize, BLACK);
                }
            }
            else {
                float fade = 1.0f - (i / (float)snake.size()) * 0.5f;
                DrawRectangle(sx - 2, sy - 2, cellSize + 4, cellSize + 4, Color{ 60, 220, 100, 60 });

                Color bodyColor = Color{
                    (unsigned char)(40 * fade),
                    (unsigned char)(180 * fade),
                    (unsigned char)(70 * fade),
                    255
                };
                DrawRectangle(sx, sy, cellSize, cellSize, bodyColor);
                DrawRectangle(sx + 2, sy + 2, cellSize - 4, cellSize - 4,
                    Color{ (unsigned char)(60 * fade), (unsigned char)(200 * fade), (unsigned char)(90 * fade), 255 });
                DrawRectangleLines(sx, sy, cellSize, cellSize, Color{ 60, 220, 100, 200 });
            }
        }

        // UI bar
        DrawRectangleGradientV(0, rows * cellSize, width, uiHeight, Color{ 20, 20, 35, 255 }, Color{ 10, 10, 20, 255 });
        DrawLine(0, rows * cellSize, width, rows * cellSize, Color{ 100, 200, 150, 255 });

        DrawText(TextFormat("SCORE: %d", score), 20, rows * cellSize + 12, 24, Color{ 100, 255, 150, 255 });
        DrawText(TextFormat("HIGH: %d", highScore), 170, rows * cellSize + 12, 24, Color{ 160, 220, 255, 255 });

        if (comboCount > 1) {
            DrawText(TextFormat("COMBO x%d", comboCount), 330, rows * cellSize + 12, 24, Color{ 255, 200, 120, 255 });
        }

        if (activePower == PowerUpType::Slow) {
            DrawText(TextFormat("POWER: SLOW (%.1fs)", powerUpTimer), 470, rows * cellSize + 12, 18, Color{ 120, 180, 255, 255 });
        }
        else if (activePower == PowerUpType::DoubleScore) {
            DrawText(TextFormat("POWER: 2x (%.1fs)", powerUpTimer), 470, rows * cellSize + 12, 18, Color{ 255, 220, 140, 255 });
        }

        const char* muteText = muted ? "M: MUTED" : "M: SOUND";
        DrawText(muteText, width - 120, rows * cellSize + 12, 18, muted ? RED : GREEN);

        DrawText("WASD/Arrows | P Pause | R Restart", 20, rows * cellSize + 40, 16, Color{ 150, 150, 170, 255 });

        if (!soundsReady) {
            DrawText("Audio disabled: missing .wav files or audio device not ready", 20, rows * cellSize + 54, 14, RED);
        }

        if (paused && !gameOver) {
            DrawRectangle(0, 0, width, rows * cellSize, Color{ 0, 0, 0, 150 });
            DrawText("PAUSED", width / 2 - 80, height / 2 - 50, 48, YELLOW);
            DrawText("P: Resume  |  M: Mute  |  R: Restart", width / 2 - 170, height / 2 + 10, 20, RAYWHITE);
        }

        if (gameOver) {
            DrawRectangle(0, 0, width, rows * cellSize, Color{ 0, 0, 0, 180 });
            DrawText("GAME OVER", width / 2 - 140, height / 2 - 60, 56, RED);
            DrawText(TextFormat("Final Score: %d", score), width / 2 - 120, height / 2, 32, Color{ 255, 200, 200, 255 });
            DrawText("Press R to Restart", width / 2 - 130, height / 2 + 50, 24, Color{ 255, 255, 150, 255 });
        }

        EndDrawing();
    }

    if (soundsReady) {
        UnloadSound(eatSound);
        UnloadSound(powerSound);
        UnloadSound(gameOverSound);
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}