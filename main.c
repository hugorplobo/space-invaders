#include "include/raylib.h"
#include <stdio.h>

#define PLAYER_HOR_SPD 200
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

typedef struct {
    Vector2 position;
    Vector2 size;
} Character;

Character player = { { SCREEN_WIDTH / 2 - 10, 450 }, { 20, 20 } };
Character enemies[5][10];
Vector2 bullets[100];

void updatePlayer(float);
void updateBullets(float);

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    //SetTargetFPS(60);

    int enemiesMargin = (SCREEN_WIDTH - SCREEN_WIDTH * 9 / 10 - 20) / 2;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            enemies[i][j] = (Character) { { j * SCREEN_WIDTH / 10 + enemiesMargin, i * 30 + enemiesMargin }, { 20, 20 } };
        }
    }

    for (int i = 0; i < 100; ++i) {
        bullets[i] = (Vector2) { -1, -1 };
    }

    while (!WindowShouldClose()) {

        updatePlayer(GetFrameTime());
        updateBullets(GetFrameTime());

        BeginDrawing();

        ClearBackground(BLACK);
        DrawRectangleV(player.position, player.size, RED);

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 10; ++j) {
                DrawRectangleV(enemies[i][j].position, enemies[i][j].size, BLUE);
            }
        }

        for (int i = 0; i < 100; ++i) {
            if (bullets[i].x != -1 && bullets[i].y != -1) {
                DrawRectangle(bullets[i].x, bullets[i].y, 5, 5, WHITE);
            }
        }

        DrawFPS(10, SCREEN_HEIGHT - 42);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}

void updatePlayer(float delta) {
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player.position.x -= PLAYER_HOR_SPD * delta;
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player.position.x += PLAYER_HOR_SPD * delta;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)) {
        for (int i = 0; i < 100; i++) {
            if (bullets[i].x == -1 && bullets[i].y == -1) {
                bullets[i] = (Vector2) { player.position.x + 10 - 2.5, player.position.y - 10 };
                break;
            }
        }
    }

    if (player.position.x + player.size.x > SCREEN_WIDTH) {
        player.position.x = SCREEN_WIDTH - player.size.x;
    } else if (player.position.x < 0) {
        player.position.x = 0;
    }
}

void updateBullets(float delta) {
    for (int i = 0; i < 100; ++i) {
        if (bullets[i].x != -1 && bullets[i].y != -1) {
            for (int col = 0; col < 5; ++col) {
                for (int row = 0; row < 10; ++row) {
                    if (
                        bullets[i].y <= enemies[col][row].position.y + enemies[col][row].size.y &&
                        bullets[i].x >= enemies[col][row].position.x &&
                        bullets[i].x <= enemies[col][row].position.x + enemies[col][row].size.x
                    ) {
                        enemies[col][row] = (Character) { { -1, -1 }, { 0, 0 } };
                        bullets[i] = (Vector2) { -1, -1 };
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