#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define WIDTH 40
#define HEIGHT 20

typedef struct
{
  int x;
  int y;
  char symbol;
} point;

char gameState[HEIGHT][WIDTH + 1];
point food;
point frame[(WIDTH - 1) * 2 + (HEIGHT - 1) * 2];
point *snake;
int snakeSize = 3;
int direction = RIGHT;
void write(point p)
{
  gameState[p.y][p.x] = p.symbol;
}

void clear(void)
{
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      gameState[i][j] = ' ';
    }
    gameState[i][WIDTH] = '\0';
  }
  system("cls");
}

void show(void)
{
  for (int i = 0; i < HEIGHT; i++)
  {
    printf("%s\n", gameState[i]);
  }
}

void createSnake(void)
{
  snake = (point *)malloc(snakeSize * sizeof(point));
  for (int i = 0; i < snakeSize; i++)
  {
    point p = {i + 10, i + 10, '+'};
    snake[i] = p;
  }
}

void createFrame(void)
{
  int index = 0;
  for (int i = 0; i < WIDTH; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      if (i == 0 || j == 0 || i == WIDTH - 1 || j == HEIGHT - 1)
      {
        point p = {i, j, '*'};
        frame[index++] = p;
      }
    }
  }
}

void drawFrame(void)
{
  int size = sizeof(frame) / sizeof(frame[0]);
  for (int i = 0; i < size; i++)
  {
    write(frame[i]);
  }
}

void drawSnake(void)
{
  for (int i = 0; i < snakeSize; i++)
  {
    write(snake[i]);
  }
}

void init(void)
{
  free(snake);
  snakeSize = 3;
  direction = RIGHT;
  clear();
  point f = {WIDTH / 4, HEIGHT / 4, '$'};
  food = f;
  createFrame();
  createSnake();
}

void draw(void)
{
  write(food);
  drawFrame();
  drawSnake();
}

int isDelayEnded(void)
{
  static long totalTime = 0;
  static long timeDelta = 0;
  long now = clock();
  timeDelta += now - totalTime;
  totalTime = now;
  if (timeDelta < CLOCKS_PER_SEC / 5)
  {
    return 0;
  }
  timeDelta -= CLOCKS_PER_SEC / 5;
  return 1;
}

void move(void)
{
  point p = snake[0];
  int x = p.x;
  int y = p.y;
  switch (direction)
  {
  case RIGHT:
    p.x += 1;
    break;
  case LEFT:
    p.x -= 1;
    break;
  case UP:
    p.y -= 1;
    break;
  case DOWN:
    p.y += 1;
    break;
  default:
    break;
  }
  snake[0] = p;
  for (int i = 1; i < snakeSize; i++)
  {
    point head = snake[i];
    int tempX = head.x;
    int tempY = head.y;
    head.x = x;
    head.y = y;
    x = tempX;
    y = tempY;
    snake[i] = head;
  }
}

int collade(point x, point y)
{
  return x.x == y.x && x.y == y.y;
}

int isSnakeColladeFrame(void)
{
  point head = snake[0];
  int count = sizeof(frame) / sizeof(frame[0]);
  for (int i = 0; i < count; i++)
  {
    if (collade(head, frame[i]))
    {
      return 1;
    }
  }
  return 0;
}

int isSnakeColladeSelf(void)
{
  point head = snake[0];
  for (int i = 1; i < snakeSize; i++)
  {
    if (collade(head, snake[i]))
    {
      return 1;
    }
  }
  return 0;
}

int isSnakeColladeFood(void)
{
  return collade(food, snake[0]);
}

void growSnake(void)
{
  snakeSize += 1;
  snake = realloc(snake, snakeSize * sizeof(point));
  point p = {0, 0, '+'};
  snake[snakeSize - 1] = p;
}

void moveFood(void)
{
  food.x = (rand() % (WIDTH - 2)) + 1;
  food.y = (rand() % (HEIGHT - 2)) + 1;
  ;
}

void turnSnake(void)
{
  if (kbhit())
  {
    int c = getch();
    if (c != RIGHT && c != LEFT && c != UP && c != DOWN)
    {
      return;
    }
    direction = c;
  }
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  fflush(stdin);
  system("cls");
  init();
  while (1)
  {
    turnSnake();
    if (isDelayEnded())
    {
      move();
    }
    if (isSnakeColladeFrame() || isSnakeColladeSelf())
    {
      init();
    }
    if (isSnakeColladeFood())
    {
      growSnake();
      moveFood();
    }
    clear();
    draw();
    show();
    fflush(stdin);
  }
  free(snake);
  return 0;
}
