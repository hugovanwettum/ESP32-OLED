#include <Arduino.h>
#include <vector>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "settings.h"
#include "ball.h"
#include "paddle.h"
#include "renderer.h"
#include "block.h"

// Display object to write to
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Semaphore for dealing with the global variable
extern SemaphoreHandle_t paddle_position_variable_semaphore;

// The global variable of paddle position
extern uint16_t sharedVariable_paddle_position;

void screenInit()
{
    // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
    Wire.begin(5, 4);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
    {
        Serial.println(F("SSD1306 failed"));
        for (;;)
            ;
    }
}

void drawSplashScreen(uint16_t ball_x, uint16_t ball_y, uint16_t ball_radius, uint16_t paddle_x, uint16_t paddle_y)
{
    //  Clear the buffer (all pixels to off, black)
    display.clearDisplay();

    // Draw the paddle
    display.drawRect(paddle_x, paddle_y, PADDLE_HEIGHT, PADDLE_START_WIDTH, SSD1306_WHITE);

    // Draw the ball
    display.fillCircle(ball_x, ball_y, ball_radius, SSD1306_WHITE);

    // Push data currently in RAM to SSDD1306
    display.display();
}

void screenTask(void *pvParameters)
{
    // SETUP
    // Handle screen Initialization
    screenInit();

    // Create Renderer object
    Renderer renderer(display);

    // Count the number of times the ball goes past the paddle
    int loss_count = 0;

    // Create Ball object for radius = 2, and dX = 2, dY = 1
    Ball ball(2, ball.getRadius() * 2, random(4, SCREEN_WIDTH - 4), 2, 1);

    // Create Paddle object
    Paddle paddle(ball.getRadius() * 2 - 1, sharedVariable_paddle_position, PADDLE_HEIGHT, PADDLE_START_WIDTH);

    // Create vector of blocks
    const int NUM_ROWS = 6;
    const int NUM_COLUMNS = 4;
    // Vector to store blocks
    std::vector<Block> blocks;
    // Create block objects
    // For every row:
    for (int j = 0; j < NUM_ROWS; ++j)
    {
        // And for every column:
        for (int i = 0; i < NUM_COLUMNS; i++)
        {
            uint16_t width = 16;
            uint16_t height = 8;
            uint16_t x = SCREEN_WIDTH - (j + 1) * height;
            uint16_t y = i * width;

            blocks.push_back(Block(x, y, width, height));
        }
    }

    // Draw splashscreen until a user connects:
    drawSplashScreen(ball.getX(), ball.getY(), ball.getRadius(), paddle.getX(), paddle.getY());

    // STALL EXECUTION until received confirmation that a user connected
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // MAIN GAME LOOP
    for (;;)
    {
        // Handle input
        // Set current paddle position to value from global variable that is sent from serverTask
        paddle.setY(sharedVariable_paddle_position);

        // Update ball position according to Dx and Dy
        ball.move();

        // Check for wall collisions
        ball.handleWallCollision();

        // Check for paddle collision (could be improved with course and finegrained collisions)
        ball.handlePaddleCollision(paddle.getX(), paddle.getY(), paddle.getWidth(), paddle.getHeight());

        // For all blocks
        for (auto &block : blocks)
        {
            // If the block is not yet destroyed
            if (!block.isDestroyed())
            {
                // Check if it collides with the ball
                int collisionValue = block.checkCollision(ball.getX(), ball.getY(), ball.getRadius());
                // Serial.println(collisionValue);
                if (collisionValue)
                {
                    // If it does collide:
                    // Set the status to destroyed
                    block.destroy();
                    // Change ball direction
                    if (collisionValue == 1)
                    {
                        // Ball hit the side, so y direction should flip
                        ball.setdY(-ball.getdY());
                    }
                    else
                    {
                        // Else ball hit the top or bottom, so x direction should flip
                        ball.setdX(-ball.getdX());
                    }
                }
            }
        }

        // Check if ball went past paddle, if so increase loss counter
        loss_count = ball.checkLossCondition(loss_count);

        //  Clear the buffer (all pixels to off, black)
        display.clearDisplay();

        // Draw the paddle
        renderer.drawPaddle(paddle);

        // Draw the ball
        renderer.drawBall(ball);

        // For all blocks
        for (const auto &block : blocks)
        {
            // If the block is not yet destroyed
            if (!block.isDestroyed())
            {
                renderer.drawBlock(block);
            }
        }

        // Draw scoreboard
        renderer.drawScore(loss_count);

        // Push data to SSDD1306
        display.display();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}