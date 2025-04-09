#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "gba.h"
#include "images/garbage.h"

enum gba_state {
  START,
  PLAY,
  WIN
};

#define GOAL_ROW 100
#define GOAL_COL 180
#define GOAL_WIDTH 30
#define GOAL_HEIGHT 30

#define TITLE_TEXT "MVP!"
#define TITLE_ROW_START 20
#define TITLE_COL_START 80
#define ANIMATION_SPEED 2
#define ANIMATION_RANGE 20

#define SPRITE_CENTER_ROW 120
#define SPRITE_CENTER_COL 40

void drawPlayer(Player *player) {
  drawImageDMA(player->row, player->col, player->width, player->height, garbage);
}

void erasePlayer(Player *player) {
  drawRectDMA(player->row, player->col, player->width, player->height, BLACK);
}

int checkCollision(Player *player, int goalRow, int goalCol, int goalWidth, int goalHeight) {
  return (player->row < goalRow + goalHeight && player->row + player->height > goalRow && player->col < goalCol + goalWidth && player->col + player->width > goalCol);
}

void updateTimer(GameState *gameState) {
  gameState->timer = vBlankCounter / 60;
}

void drawTimer(GameState *gameState) {
  char timeStr[20];
  snprintf(timeStr, 20, "Time: %d s", gameState->timer);
  drawRectDMA(5, 5, 100, 10, BLACK);
  drawString(5, 5, timeStr, WHITE);
}

void animateTitleText(int frameCount) {
  static int prevOffset = 0;
  
  int offset_frame = (frameCount / ANIMATION_SPEED) % (2 * ANIMATION_RANGE);
  if (offset_frame > ANIMATION_RANGE) {
    offset_frame = 2 * ANIMATION_RANGE - offset_frame;
  }
  
  if (offset_frame != prevOffset) {
    drawRectDMA(TITLE_ROW_START - ANIMATION_RANGE, TITLE_COL_START - 10, 140, 2 * ANIMATION_RANGE + 10, BLACK);
    
    drawString(TITLE_ROW_START + offset_frame - ANIMATION_RANGE, TITLE_COL_START, TITLE_TEXT, WHITE);
    
    prevOffset = offset_frame;
  }
}

void animateTitleSprite(int frameCount) {
  static int prevRow = SPRITE_CENTER_ROW;
  static int prevCol = SPRITE_CENTER_COL;
  int cenRow = SPRITE_CENTER_ROW;
  int cenCol = SPRITE_CENTER_COL;
  int angle = (frameCount / ANIMATION_SPEED) % 40;
  int row, col;

  if (angle < 10) {
    row = cenRow - angle;
    col = cenCol + angle;
  } else if (angle < 20) {
    row = cenRow - (20 - angle);
    col = cenCol + (20 - angle);
  } else if (angle < 30) {
    row = cenRow + (angle - 20);
    col = cenCol - (angle - 20);
  } else {
    row = cenRow + (40 - angle);
    col = cenCol - (40 - angle);
  }
  
  if (row != prevRow || col != prevCol) {
    if (frameCount > 0) {
      drawRectDMA(prevRow, prevCol, GARBAGE_WIDTH, GARBAGE_HEIGHT, BLACK);
    }
    
    drawImageDMA(row, col, GARBAGE_WIDTH, GARBAGE_HEIGHT, garbage);
    
    prevRow = row;
    prevCol = col;
  }
}

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  u32 prevButtons = BUTTONS;
  u32 currButtons = BUTTONS;

  enum gba_state state = START;
  
  Player player = {
    .row = 80,
    .col = 40,
    .width = GARBAGE_WIDTH,
    .height = GARBAGE_HEIGHT,
    .color = WHITE,
    .rowDelta = 0,
    .colDelta = 0,
    .prevRow = 80,
    .prevCol = 40
  };
  
  GameState gameState = {
    .timer = 0,
    .score = 0
  };
  
  int frameCount = 0;
  int titleInit = 0;
  int needToRedrawGoal = 0;

  while (1) {
    prevButtons = currButtons;
    
    waitForVBlank();
    
    currButtons = BUTTONS;

    switch (state) {
      case START:
        if (!titleInit) {
          fillScreenDMA(BLACK);
          
          drawString(75, 60, "Reach the goal to win!", WHITE);
          
          titleInit = 1;
          frameCount = 0;
        }
        
        animateTitleText(frameCount);
        animateTitleSprite(frameCount);
        frameCount++;
        
        if ((~currButtons & BUTTON_START) && (prevButtons & BUTTON_START)) {
          state = PLAY;
          
          vBlankCounter = 0;
          gameState.timer = 0;
          
          fillScreenDMA(BLACK);
          
          drawRectDMA(GOAL_ROW, GOAL_COL, GOAL_WIDTH, GOAL_HEIGHT, GREEN);
          
          drawPlayer(&player);
          
          drawTimer(&gameState);
          
          drawString(140, 5, "Use D-Pad to move", WHITE);
          drawString(150, 5, "Backspace to reset", WHITE);
        }
        break;
        
      case PLAY:
        if ((~currButtons & BUTTON_SELECT) && (prevButtons & BUTTON_SELECT)) {
          state = START;
          
          player.row = 80;
          player.col = 40;
          player.prevRow = 80;
          player.prevCol = 40;
          
          titleInit = 0;
          
          break;
        }
        
        updateTimer(&gameState);
        drawTimer(&gameState);
        
        player.rowDelta = 0;
        player.colDelta = 0;
        
        if (KEY_DOWN(BUTTON_UP, currButtons)) {
          player.rowDelta = -1;
        }

        if (KEY_DOWN(BUTTON_DOWN, currButtons)) {
          player.rowDelta = 1;
        }

        if (KEY_DOWN(BUTTON_LEFT, currButtons)) {
          player.colDelta = -1;
        }
        
        if (KEY_DOWN(BUTTON_RIGHT, currButtons)) {
          player.colDelta = 1;
        }
        
        if (player.rowDelta != 0 || player.colDelta != 0) {
          int newRow = player.row + player.rowDelta;
          int newCol = player.col + player.colDelta;
          
          if (newRow < 0) {
            newRow = 0;
          }
          if (newRow > HEIGHT - player.height) {
            newRow = HEIGHT - player.height;
          }
          if (newCol < 0) {
            newCol = 0;
          }
          if (newCol > WIDTH - player.width) {
            newCol = WIDTH - player.width;
          }
          
          player.prevRow = player.row;
          player.prevCol = player.col;
          player.row = newRow;
          player.col = newCol;
          
          needToRedrawGoal = checkCollision(&player, GOAL_ROW, GOAL_COL, GOAL_WIDTH, GOAL_HEIGHT) ||
                            checkCollision(&player, player.prevRow, player.prevCol, player.width, player.height);
          
          if (player.rowDelta != 0 && player.colDelta == 0) {
            int eraseRow = (player.rowDelta < 0) ? player.prevRow + player.height - 1 : player.prevRow;
            drawRectDMA(eraseRow, player.prevCol, player.width, 1, BLACK);
          } else if (player.rowDelta == 0 && player.colDelta != 0) {
            int eraseCol = (player.colDelta < 0) ? player.prevCol + player.width - 1 : player.prevCol;
            drawRectDMA(player.prevRow, eraseCol, 1, player.height, BLACK); 
          } else if (player.rowDelta != 0 && player.colDelta != 0) {
            drawRectDMA(player.prevRow, player.prevCol, player.width, player.height, BLACK);
          }
          
          if (needToRedrawGoal) {
            drawRectDMA(GOAL_ROW, GOAL_COL, GOAL_WIDTH, GOAL_HEIGHT, GREEN);
          }
          
          drawPlayer(&player);
          
          if (checkCollision(&player, GOAL_ROW, GOAL_COL, GOAL_WIDTH, GOAL_HEIGHT)) {
            state = WIN;
            
            gameState.score = gameState.timer;
            
            fillScreenDMA(BLACK);
            
            drawImageDMA(20, 20, GARBAGE_WIDTH, GARBAGE_HEIGHT, garbage);
            
            drawString(60, 60, "YOU WIN!", WHITE);
            
            char scoreStr[30];
            snprintf(scoreStr, 30, "Your time: %d seconds", gameState.score);
            drawString(80, 60, scoreStr, WHITE);
            
            drawString(120, 60, "Backspace to restart", WHITE);
          }
        }
        break;
        
      case WIN:
        if ((~currButtons & BUTTON_SELECT) && (prevButtons & BUTTON_SELECT)) {
          state = START;
          
          player.row = 80;
          player.col = 40;
          player.prevRow = 80;
          player.prevCol = 40;
          
          titleInit = 0;
        }
        break;
    }
  }

  return 0;
}