#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

typedef struct {
    int row;
    int col;
    int width;
    int height;
    u16 color;
    int rowDelta;
    int colDelta;
} Player;

typedef struct {
    int timer;
    int score;
} GameState;

void drawPlayer(Player *player);
void erasePlayer(Player *player);
int checkCollision(Player *player, int goalRow, int goalCol, int goalWidth, int goalHeight);
void updateTimer(GameState *gameState);
void drawTimer(GameState *gameState);
void animateTitleText(int frameCount);
void animateTitleSprite(int frameCount);

#endif