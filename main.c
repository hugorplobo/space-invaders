#include "include/raylib.h"

#define PLAYER_HOR_SPD 200
#define SCREEN_WIDTH 650
#define SCREEN_HEIGHT 650
#define PLAYER_WIDTH 44
#define PLAYER_HEIGHT 20
#define ENEMY_WIDTH 44
#define ENEMY_HEIGHT 32

typedef struct {
    char name[4];
    int value;
} Score;

Vector2 player = { SCREEN_WIDTH / 2 - 22, SCREEN_HEIGHT - 50 };
Vector2 enemies[5][8];
Vector2 bullets[50];
Vector2 enemiesBullets[50];
int score = 0;
int marginL = 10, marginR = 600;
int direction = 1;
int enemyCount = 40;
int playerHP = 3;
int framesCounter = 0;
int playerCanShoot = 1;
int isInGame = 0;
int isInScores = 0;
int menuTextSelected = 0;

void updatePlayer(Sound*, float);
void updateBullets(Sound*, Sound*, float);
void updateEnemies(float);
void updateEnemiesBullets(Sound*, float);
void initGame();
void drawGame(Texture2D*, Texture2D*);
void drawMainMenu();
void handleMainMenu();

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);
    HideCursor();

    Texture2D playerTexture = LoadTexture("assets/sprites/player.png");
    Texture2D enemyTexture = LoadTexture("assets/sprites/enemy.png");

    InitAudioDevice();

    Sound playerShootFx = LoadSound("assets/sounds/player_shoot.wav");
    Sound enemyExplosionFx = LoadSound("assets/sounds/enemy_explosion.wav");
    Sound scoreFx = LoadSound("assets/sounds/score.wav");

    SetSoundVolume(playerShootFx, 0.2);
    SetSoundVolume(enemyExplosionFx, 0.1);

    initGame();

    while (!WindowShouldClose()) {

        if (playerHP > 0 && enemyCount > 0 && isInGame) {
            if (!playerCanShoot) framesCounter++;

            if (framesCounter == 60) {
                framesCounter = 0;
                playerCanShoot = 1;
            }

            updatePlayer(&playerShootFx, GetFrameTime());
            updateBullets(&enemyExplosionFx, &scoreFx, GetFrameTime());
            updateEnemies(GetFrameTime());
            updateEnemiesBullets(&enemyExplosionFx, GetFrameTime());
        } else if (!isInGame) {
            handleMainMenu();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (isInGame) {
            drawGame(&playerTexture, &enemyTexture);
        } else {
            drawMainMenu();
        }

        EndDrawing();

    }

    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);

    UnloadSound(playerShootFx);
    UnloadSound(enemyExplosionFx);
    UnloadSound(scoreFx);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}

void updatePlayer(Sound* fx, float delta) {
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player.x -= PLAYER_HOR_SPD * delta;
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player.x += PLAYER_HOR_SPD * delta;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)) {

        if (playerCanShoot) {
            playerCanShoot = 0;
            for (int i = 0; i < 100; i++) {
                if (bullets[i].x == -1 && bullets[i].y == -1) {
                    PlaySound(*fx);
                    bullets[i] = (Vector2) { player.x + 22 - 2, player.y - 2 };
                    break;
                }
            }
        }
    }

    if (player.x + PLAYER_WIDTH > SCREEN_WIDTH) {
        player.x = SCREEN_WIDTH - PLAYER_WIDTH;
    } else if (player.x < 0) {
        player.x = 0;
    }
}

void updateBullets(Sound* explosionFx, Sound* scoreFx, float delta) {
    for (int i = 0; i < 50; ++i) {
        if (bullets[i].x != -1) {
            for (int col = 0; col < 5; ++col) {
                for (int row = 0; row < 8; ++row) {
                    if (enemies[col][row].x != -1 && enemies[col][row].y != 1) {
                        if (CheckCollisionRecs((Rectangle) { bullets[i].x, bullets[i].y, 6, 6 }, (Rectangle) { enemies[col][row].x, enemies[col][row].y, ENEMY_WIDTH, ENEMY_HEIGHT })) {
                            PlaySound(*explosionFx);
                            enemies[col][row] = (Vector2) { -1, -1 };
                            bullets[i] = (Vector2) { -1, -1 };
                            enemyCount--;
                            score++;
                            if (score % 10 == 0) PlaySound(*scoreFx);
                        }
                    }
                }
            }

            if (bullets[i].y < 0) {
                bullets[i] = (Vector2) { -1, -1 };
            }

            if (bullets[i].x != -1) {
                bullets[i].y -= 400 * delta;
            }

        }
    }
}

void updateEnemies(float delta) {
    Vector2 aux = { 0, 0 };
    Vector2* first = &aux;
    Vector2* last = &enemies[0][0];

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 8; ++j){
            if (enemies[i][j].x != -1) first = &enemies[i][j];
        }
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 8; ++j) {
            Vector2* enemy = &enemies[i][j];
            if (enemy->x == -1) continue;

            if (enemy->x < first->x) first = enemy;
            else if (enemy->x > last->x) last = enemy;
        }
    }

    if (first->x <= marginL) direction = 1;
    else if (last->x >= marginR) direction = -1;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 8; ++j) {
            Vector2* enemy = &enemies[i][j];
            if (enemy->x == -1) continue;

            enemy->x += direction * 20 * (40 / enemyCount) * delta;

            if (i == 4) {
                if (GetRandomValue(1, 500) == 1) {
                    for (int k = 0; k < 50; ++k) {
                        if (enemiesBullets[k].x == -1 && enemiesBullets[k].y == -1) {
                            enemiesBullets[k] = (Vector2) { enemy->x + 22 - 2, enemy->y + 2};
                            break;
                        }
                    }
                }
            } else if (enemies[i + 1][j].x == -1) {
                if (GetRandomValue(1, 500) == 1) {
                    for (int k = 0; k < 50; ++k) {
                        if (enemiesBullets[k].x == -1 && enemiesBullets[k].y == -1) {
                            enemiesBullets[k] = (Vector2) { enemy->x + 22 - 2, enemy->y + 2};
                            break;
                        }
                    }
                }
            }
        }
    }
}

void updateEnemiesBullets(Sound* fx, float delta) {
    for (int i = 0; i < 50; ++i) {
        if (enemiesBullets[i].x != -1 && enemiesBullets[i].y != -1) {
            Vector2* bullet = &enemiesBullets[i];
            if (CheckCollisionRecs((Rectangle) { enemiesBullets[i].x, enemiesBullets[i].y, 6, 6 }, (Rectangle) { player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT })) {
                PlaySound(*fx);
                playerHP -= 1;
                enemiesBullets[i] = (Vector2) { -1, -1 };
            }

            if (bullet->y > SCREEN_HEIGHT) {
                *bullet = (Vector2) { -1, -1 };
            }

            if (bullet->x != -1) {
                bullet->y += 400 * delta;
            }
        } 
    }
}

void drawGame(Texture2D* playerTexture, Texture2D* enemyTexture) {
    if (playerHP > 0 && enemyCount > 0) {
        DrawTextureV(*playerTexture, player, WHITE);

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (enemies[i][j].x != -1) {
                    DrawTextureV(*enemyTexture, enemies[i][j], WHITE);
                }
            }
        }

        for (int i = 0; i < 50; ++i) {
            if (bullets[i].x != -1) {
                DrawRectangle(bullets[i].x, bullets[i].y, 6, 6, WHITE);
            }
        }

        for (int i = 0; i < 50; ++i) {
            if (enemiesBullets[i].x != -1) {
                DrawRectangle(enemiesBullets[i].x, enemiesBullets[i].y, 6, 6, WHITE);
            }
        }
        DrawText(TextFormat("SCORE: %05i", score * 100), 10, SCREEN_HEIGHT - 25, 20, RAYWHITE);
    } else if (playerHP <= 0) {
        DrawText("Game Over", SCREEN_WIDTH / 2 - MeasureText("Game Over", 50) / 2, SCREEN_HEIGHT / 2 - 25, 50, RAYWHITE);
    } else {
        DrawText("You Won", SCREEN_WIDTH / 2 - MeasureText("You Won", 50) / 2, SCREEN_HEIGHT / 2 - 25, 50, RAYWHITE);
    }
}

void drawMainMenu() {

    if (!isInScores) {
        DrawText("Space Invaders", SCREEN_WIDTH / 2 - MeasureText("Space Invaders", 70) / 2, 30, 70, WHITE);

        if (menuTextSelected == 0) {
            DrawText("- Play -", SCREEN_WIDTH / 2 - MeasureText("- Play -", 55) / 2, 300, 55, WHITE);
            DrawText("Scores", SCREEN_WIDTH / 2 - MeasureText("Scores", 50) / 2, 400, 50, WHITE);
        } else {
            DrawText("Play", SCREEN_WIDTH / 2 - MeasureText("Play", 50) / 2, 300, 50, WHITE);
            DrawText("- Scores -", SCREEN_WIDTH / 2 - MeasureText("- Scores -", 55) / 2, 400, 55, WHITE);   
        }
    } else {
        return;
    }
}

void handleMainMenu() {
    if (IsKeyPressed(KEY_DOWN)) {
        menuTextSelected = 1;
    } else if (IsKeyPressed(KEY_UP)) {
        menuTextSelected = 0;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (menuTextSelected == 0) {
            isInGame = 1;
        } else {
            isInScores = 1;
        }
    }
}

void initGame() {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 8; ++j) {
            enemies[i][j] = (Vector2) { j * 80 + 20, i * 50 + 50 };
        }
    }

    for (int i = 0; i < 50; ++i) {
        bullets[i] = (Vector2) { -1, -1 };
    }

    for (int i = 0; i < 50; ++i) {
        enemiesBullets[i] = (Vector2) { -1, -1 };
    }
}