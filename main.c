#include "include/raylib.h"
#include <stdio.h>

#define PLAYER_HOR_SPD 200
#define SCREEN_WIDTH 650
#define SCREEN_HEIGHT 650
#define PLAYER_WIDTH 44
#define PLAYER_HEIGHT 20
#define ENEMY_WIDTH 44
#define ENEMY_HEIGHT 32

Vector2 player = { SCREEN_WIDTH / 2 - 22, SCREEN_HEIGHT - 50 };
Vector2 enemies[5][10];
Vector2 bullets[100];
char scoreText[20] = "SCORE: 0";
int score = 0;

void updatePlayer(Sound*, float);
void updateBullets(Sound*, float);

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    Texture2D playerTexture = LoadTexture("assets/sprites/player.png");
    Texture2D enemyTexture = LoadTexture("assets/sprites/enemy.png");

    InitAudioDevice();

    Sound playerShootFx = LoadSound("assets/sounds/player_shoot.wav");
    Sound enemyExplosionFx = LoadSound("assets/sounds/enemy_explosion.wav");
    Sound scoreFx = LoadSound("assets/sounds/score.wav");

    SetSoundVolume(playerShootFx, 0.2);
    SetSoundVolume(enemyExplosionFx, 0.1);

    int enemiesMargin = (SCREEN_WIDTH - SCREEN_WIDTH * 9 / 10 - 44) / 2;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            enemies[i][j] = (Vector2) { j * SCREEN_WIDTH / 10 + enemiesMargin, i * 50 + enemiesMargin };
        }
    }

    for (int i = 0; i < 100; ++i) {
        bullets[i] = (Vector2) { -1, -1 };
    }

    while (!WindowShouldClose()) {

        updatePlayer(&playerShootFx, GetFrameTime());
        updateBullets(&enemyExplosionFx, GetFrameTime());

        BeginDrawing();

        ClearBackground(BLACK);
        DrawTextureV(playerTexture, player, WHITE);

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (enemies[i][j].x != -1 && enemies[i][j].y != -1) {
                    DrawTextureV(enemyTexture, enemies[i][j], WHITE);
                }
            }
        }

        for (int i = 0; i < 100; ++i) {
            if (bullets[i].x != -1 && bullets[i].y != -1) {
                DrawRectangle(bullets[i].x, bullets[i].y, 6, 6, WHITE);
            }
        }

        sprintf(scoreText, "SCORE: %d", score * 100);
        DrawText(scoreText, 10, SCREEN_HEIGHT - 25, 20, RAYWHITE);

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
        for (int i = 0; i < 100; i++) {
            if (bullets[i].x == -1 && bullets[i].y == -1) {
                PlaySound(*fx);
                bullets[i] = (Vector2) { player.x + 22 - 2, player.y - 2 };
                break;
            }
        }
    }

    if (player.x + PLAYER_WIDTH > SCREEN_WIDTH) {
        player.x = SCREEN_WIDTH - PLAYER_WIDTH;
    } else if (player.x < 0) {
        player.x = 0;
    }
}

void updateBullets(Sound* fx, float delta) {
    for (int i = 0; i < 100; ++i) {
        if (bullets[i].x != -1 && bullets[i].y != -1) {
            for (int col = 0; col < 5; ++col) {
                for (int row = 0; row < 10; ++row) {
                    if (enemies[col][row].x != -1 && enemies[col][row].y != 1) {
                        if (CheckCollisionRecs((Rectangle) { bullets[i].x, bullets[i].y, 6, 6 }, (Rectangle) { enemies[col][row].x, enemies[col][row].y, PLAYER_WIDTH, PLAYER_HEIGHT })) {
                            PlaySound(*fx);
                            enemies[col][row] = (Vector2) { -1, -1 };
                            bullets[i] = (Vector2) { -1, -1 };
                            score++;
                        }
                    }
                }
            }

            if (bullets[i].y < 0) {
                bullets[i] = (Vector2) { -1, -1 };
            }

            if (bullets[i].x != -1 && bullets[i].y != -1) {
                bullets[i].y -= 400 * delta;
            }

        }
    }
}