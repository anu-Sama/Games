#include "raylib.h"

#define BALL_SPEEDX	150
#define BALL_SPEEDY	100
#define BALL_RADIUS 8


struct Ball {
	Vector2 position;
	Vector2 speed;
	float radius;
	Color color;

	void Draw() {
		DrawCircle(position.x, position.y, radius, color);
	}

};

struct Paddle
{
	Vector2 position;
	Vector2 speed;
	int width;
	int height;
	Color color;

	Rectangle getRect() {
		
		return Rectangle{ position.x,position.y, (float)width,(float)height };

	}

	void DrawRect() {
		
		DrawRectangleRec(getRect(), color);
	}

};


static int screenWidth = 800;
static int screenHeight = 450;

static bool gameOver = false;
static bool pause = false;
static float directionX = 1;
static float directionY = 1;

const char* winnerTitle = nullptr;

static Ball ball;
static Paddle leftPaddle, rightPaddle;

static void InitGame(void);
static void UpdateGame(void);

int main(void) {

	InitWindow(screenWidth, screenHeight, "Pong Ping");

	InitGame();
	

	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(RAYWHITE);

			UpdateGame();


			// Drawing the ball
			//DrawCircle(screenWidth / 2, screenHeight / 2, 8.0f, DARKGRAY);
			ball.Draw();

			// Drawing left paddle
			leftPaddle.DrawRect();

			// Drawing right paddle
			rightPaddle.DrawRect();
			if (winnerTitle != nullptr) {
				
				DrawText(winnerTitle, screenWidth / 2 - 150, screenHeight / 2 - 30, 60, DARKGREEN);
			}

			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void InitGame() {

	ball.position.x = screenWidth / 2;
	ball.position.y = screenHeight / 2;

	ball.speed.x = BALL_SPEEDX;
	ball.speed.y = BALL_SPEEDY;

	ball.radius = BALL_RADIUS;

	ball.color = DARKGRAY;

	// Left Paddle
	
	leftPaddle.position = Vector2{ 20  , (float)screenHeight / 2 - 50 };
	leftPaddle.speed	= Vector2{ 100,100 };
	leftPaddle.width	= 10;
	leftPaddle.height	= 100;
	leftPaddle.color	= DARKGRAY;

	// Right paddle

	rightPaddle.position = Vector2{ (float)screenWidth - 20  , (float)screenHeight / 2 - 50 };
	rightPaddle.speed	= Vector2{ 100,100 };
	rightPaddle.width	= 10;
	rightPaddle.height	= 100;
	rightPaddle.color	= DARKGRAY;
}


void UpdateGame() {

	if (!gameOver) {
		if (IsKeyDown(KEY_P)) pause = !pause;
		
		if (!pause) {

			ball.position.x += directionX * ball.speed.x * GetFrameTime();
			ball.position.y -= directionY * ball.speed.y * GetFrameTime();

			if (IsKeyDown(KEY_W)) leftPaddle.position.y -= leftPaddle.speed.y * GetFrameTime();
			if (IsKeyDown(KEY_S)) leftPaddle.position.y += leftPaddle.speed.y * GetFrameTime();
			if (IsKeyDown(KEY_UP)) rightPaddle.position.y -= rightPaddle.speed.y * GetFrameTime();
			if (IsKeyDown(KEY_DOWN)) rightPaddle.position.y += rightPaddle.speed.y * GetFrameTime();
		
			if (CheckCollisionCircleRec(ball.position, ball.radius, leftPaddle.getRect())) {
				directionX *= -1.1;
				ball.speed.y = (ball.position.y - leftPaddle.position.y) / leftPaddle.height * ball.speed.x;

			}
			if (CheckCollisionCircleRec(ball.position, ball.radius, rightPaddle.getRect())) {
				directionX *= -1.1;
				ball.speed.y = (ball.position.y - leftPaddle.position.y) / leftPaddle.height * ball.speed.x;
			}

			if (ball.position.y > screenHeight) {
				directionY *= -1;
				ball.position.y = screenHeight;
			}
			if (ball.position.y < 0) {
				directionY *= -1;
				ball.position.y = 0;
			}

			if (ball.position.x > screenWidth + 10) {
				gameOver = true;
				winnerTitle = "Left Wins!";
			}
			if (ball.position.x < -10) {
				gameOver = true;
				winnerTitle = "Right Wins!";
			}
		}
	}
}
