/// Игра "Змейка"

#include "raylib.h"
#include "raymath.h"
#include <vector>

const int SQUARE_SIZE = 30;
const int SCREEN_WIDTH = 810;
const int SCREEN_HEIGHT = 630;

/// <summary>
/// Одно звено тела змейки
/// </summary>
struct Snake {
	Vector2 position;
	Vector2 size;
	Vector2 speed;
	Color color;
};
/// <summary>
/// Еда для змейки
/// </summary>
struct Food {
	Vector2 position;
	Vector2 size;
	Color color;
	bool active;
};

bool gameOver = false;
int counterTail = 1;
int framesCounter = 0;
Food food = {};
std::vector<Snake> snakeBody = {};
Vector2 offset = {};

/// <summary>
/// Инициализация перменных
/// </summary>
void InitGame();
/// <summary>
/// Обновление игры
/// </summary>
void UpdateGame();
/// <summary>
/// Отрисовка игры
/// </summary>
void DrawGame();

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake game");
	//Инициализация
	InitGame();
	SetTargetFPS(10);
	//Обновление кадра
	while (!WindowShouldClose()) {
		//	Обновить позицию объектов игры
		UpdateGame();
		//  Обновить графическое отображение игры
		DrawGame();
	}
	//Очистка памяти
	CloseWindow();
	return 0;
}

void InitGame() {
	//Инициализация общеигровых переменных
	framesCounter = 0;
	gameOver = false;

	offset = { SCREEN_WIDTH % SQUARE_SIZE,
				SCREEN_HEIGHT % SQUARE_SIZE
	};

	//Инициализация змейки
	counterTail = 1;
	Vector2 position = { offset.x / 2,offset.y / 2 };
	Vector2 size = { SQUARE_SIZE,SQUARE_SIZE };
	Vector2 speed = { SQUARE_SIZE,0 };
	Color color = DARKBLUE;
	snakeBody.push_back({ position,size,speed,color });

	//Инициализация еды
	food.size = { SQUARE_SIZE,SQUARE_SIZE };
	food.color = ORANGE;
	food.active = false;
}

void UpdateGame() {
	if (!gameOver) {
		//Реакция на нажатие стрелочек
		if (IsKeyPressed(KEY_LEFT)) {
			snakeBody[0].speed = { -SQUARE_SIZE,0 };
		}
		if (IsKeyPressed(KEY_RIGHT)) {
			snakeBody[0].speed = { SQUARE_SIZE,0 };
		}
		if (IsKeyPressed(KEY_UP)) {
			snakeBody[0].speed = { 0,-SQUARE_SIZE };
		}
		if (IsKeyPressed(KEY_DOWN)) {
			snakeBody[0].speed = { 0,SQUARE_SIZE };
		}
		//Продвижение
		for (int i = counterTail-1; i > 0; i--)
		{
			snakeBody[i].position = snakeBody[i-1].position;
		}
		snakeBody[0].position = Vector2Add(snakeBody[0].position, snakeBody[0].speed);
		
		//Столкновение со стеной

		//Столкновение с самим собой
		for (int i = 1; i < counterTail; i++)
		{
			if (Vector2Equals(snakeBody[0].position,snakeBody[i].position)){
				gameOver = true;
			}
		}
		//Столкновение с едой
		if (!food.active) {
			food.active = true;
			food.position = {
				GetRandomValue(0,(SCREEN_WIDTH / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
				GetRandomValue(0,(SCREEN_HEIGHT / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2
			};
			for (int i = 0; i < counterTail; i++)
			{
				while (Vector2Equals(food.position, snakeBody[i].position)) {
					food.position = {
						GetRandomValue(0,(SCREEN_WIDTH / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
						GetRandomValue(0,(SCREEN_HEIGHT / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2
					};
				}
			}
		}
		if (Vector2Equals(food.position, snakeBody[0].position)) {
			Snake cell = {
				{
					Vector2Add(snakeBody[counterTail-1].position,
								Vector2Negate(snakeBody[0].speed))
				},
				{ SQUARE_SIZE,SQUARE_SIZE },
				{ 0,0 },
				BLUE
			};
			snakeBody.push_back(cell);
			counterTail++;
			food.active = false;
		}
		framesCounter++;
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			InitGame();
			gameOver = false;
		}
	}

	
}

void DrawGame() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	if (!gameOver) {
		for (int i = 0; i < SCREEN_WIDTH/SQUARE_SIZE+1; i++)
		{
			DrawLineV(
				{ (float)SQUARE_SIZE * i+offset.x/2,offset.y/2 },
				{ (float)SQUARE_SIZE * i+offset.x/2,SCREEN_HEIGHT-offset.y/2}, 
				BLACK);
		}

		for (int i = 0; i < SCREEN_HEIGHT / SQUARE_SIZE + 1; i++)
		{
			DrawLineV(
				{ offset.x/2,(float)SQUARE_SIZE * i + offset.y/2 },
				{ SCREEN_WIDTH - offset.x/2,(float)SQUARE_SIZE * i + offset.y/2},
				BLACK);
		}

		for (const auto& cell: snakeBody) {
			DrawRectangleV(cell.position,cell.size,cell.color);
		}
		DrawRectangleV(food.position, food.size, food.color);
	}
	else {
		DrawText("Enter to restart.",
			SCREEN_WIDTH/2 - MeasureText("Enter to restart",20),
			SCREEN_HEIGHT/2 -50,
			20,GRAY);
	}
	EndDrawing();
}