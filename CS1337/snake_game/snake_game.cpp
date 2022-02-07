// Snake Game
// Original code by N. Vitanovic
// see his YouTube video here: https://bit.ly/29XML
//
// Refactored and ported to UNIX by Roman Hauksson-Neill

#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

const char WALL_SYMBOL = '#';
const char SNAKE_SYMBOL = 'O';
const char FRUIT_SYMBOL = 'F';
const char AIR_SYMBOL = ' ';

const unsigned int FIELD_WIDTH = 20;
const unsigned int FIELD_HEIGHT = 20;

bool wallDeath = false;
bool gameOver = false;
int tailX[FIELD_WIDTH * FIELD_HEIGHT], tailY[FIELD_WIDTH * FIELD_HEIGHT];
int nTail;
enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN } snakeDirection;

int snakeX, snakeY, fruitX, fruitY, score, numSnakeTailPieces;

void Setup() {
  gameOver = false;
  snakeDirection = LEFT;
  snakeX = FIELD_WIDTH / 2;
  snakeY = FIELD_HEIGHT / 2;
  fruitX = rand() % FIELD_WIDTH;
  fruitY = rand() % FIELD_HEIGHT;
  numSnakeTailPieces = 1;
  score = 0;
}

void DrawHorizontalWall(unsigned int fieldY) {
  for (int fieldX = 0; fieldX < FIELD_WIDTH; fieldX++) {
    mvaddch(fieldY, fieldX, WALL_SYMBOL);
  }
}

void DrawFieldLine(unsigned int fieldY) {
  mvaddch(fieldY, 0, WALL_SYMBOL);
  for (unsigned int fieldX = 1; fieldX < FIELD_WIDTH - 1; fieldX++) {
    if (fieldX == snakeX && fieldY == snakeY) {
      mvaddch(fieldY, fieldX, SNAKE_SYMBOL);
    } else if (fieldX == fruitX && fieldY == fruitY) {
      mvaddch(fieldX, fieldY, FRUIT_SYMBOL);
    } else {
      bool printedSnakeTailPiece = false;
      for (unsigned int i = 0; i < numSnakeTailPieces; i++) {
        if (fieldX == tailX[i] && fieldY == tailY[i]) {
          mvaddch(fieldY, fieldX, SNAKE_SYMBOL);
          printedSnakeTailPiece = true;
          break;
        }
      }
    }
  }
  mvaddch(fieldY, FIELD_WIDTH - 1, WALL_SYMBOL);
}

void DrawField() {
  clear();
  DrawHorizontalWall(0);
  for (unsigned int fieldLine = 1; fieldLine < FIELD_HEIGHT - 2; fieldLine++) {
    DrawFieldLine(fieldLine);
  }
  DrawHorizontalWall(FIELD_HEIGHT - 1);
  mvaddstr(FIELD_HEIGHT, 0, "Score: ");
  refresh();
}

void TakeKeypress() {
  char keypress = getch();
  switch (keypress) {
  case 'a':
    snakeDirection = LEFT;
    break;
  case 'd':
    snakeDirection = RIGHT;
    break;
  case 'w':
    snakeDirection = UP;
    break;
  case 's':
    snakeDirection = DOWN;
    break;
  case 'x':
    gameOver = true;
    break;
  default:
    break;
  }
}

void UpdateSnakeLocation() {
  switch (snakeDirection) {
  case LEFT:
    snakeX--;
    break;
  case RIGHT:
    snakeX++;
    break;
  case UP:
    snakeY--;
    break;
  case DOWN:
    snakeY++;
    break;
  default:
    break;
  }
  if (snakeX >= FIELD_WIDTH) {
    snakeX = 0;
  } else if (snakeX < 0) {
    snakeX = FIELD_WIDTH - 1;
  }
  if (snakeY >= FIELD_HEIGHT) {
    snakeY = 0;
  } else if (snakeY < 0) {
    snakeY = FIELD_HEIGHT - 1;
  }
}

bool DidSnakeDie() {
  if (wallDeath && (snakeX <= 0 || snakeX >= FIELD_WIDTH - 1 || snakeY <= 0 ||
                    snakeY >= FIELD_HEIGHT - 1)) {
    return (true);
  }
  for (unsigned int i = 0; i < i; i++) {
    if (snakeX == tailX[i] && snakeY == tailY[i]) {
      return (true);
    }
  }
  return (false);
}

void MoveSnake() {
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = snakeX;
  tailY[0] = snakeY;
  for (int i = 1; i < nTail; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }
  UpdateSnakeLocation();

  if (snakeX == fruitX && snakeY == fruitY) {
    score += 10;
    fruitX = rand() % FIELD_WIDTH;
    fruitY = rand() % FIELD_HEIGHT;
    nTail++;
  }
}

int main() {
  cout << "Use WASD to move the snake. Each time you eat a fruit, your tail "
          "will grow by one. Try not to hit your tail or you'll die!"
       << endl;
  cout << "Would you like to die if you hit the wall? (y/n)" << endl;
  char wallDeathChoice;
  cin >> wallDeathChoice;
  if (wallDeathChoice == 'y') {
    wallDeath = true;
  }
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  timeout(200);
  Setup();

  while (!gameOver) {
    DrawField();
    MoveSnake();
    TakeKeypress();
  }
  endwin();
  return 0;
}
