#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "settings.h"
#include "ball.h"

// Display object to write to
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Semaphore for dealing with the global variable
extern SemaphoreHandle_t paddle_position_variable_semaphore;

// The global variable
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
    display.drawRect(paddle_x, paddle_y, PADDLE_HEIGHT, PADDLE_WIDTH, SSD1306_WHITE);

    // Draw the ball
    display.fillCircle(ball_x, ball_y, ball_radius, SSD1306_WHITE);

    // Push data currently in RAM to SSDD1306
    display.display();
}

void screenTask(void *pvParameters)
{
    // Handle screen Initialization
    screenInit();

    int loss_count = 0;

    // Create Ball object for radius = 4, and dX = dY = 2
    Ball ball(4, SCREEN_HEIGHT - 4, random(4, SCREEN_WIDTH - 4), 2, 2);

    // Paddle distance from bottom side
    uint16_t paddle_x = ball.getRadius() * 2;

    // Update the paddle position every loop to get latest value (it is a global variable)
    uint16_t paddle_y = sharedVariable_paddle_position;

    // Draw splashscreen until a user connects:
    drawSplashScreen(ball.getX(), ball.getY(), ball.getRadius(), paddle_x, sharedVariable_paddle_position);

    // STALL EXECUTION until received confirmation that a user connected
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    for (;;)
    {
        // Update the paddle position every loop to get latest value (it is a global variable)
        uint16_t paddle_y = sharedVariable_paddle_position;

        // Update ball position according to Dx and Dy
        ball.move();

        // Check for wall collisions
        ball.handleWallCollision();

        // Check for paddle collision (could be improved with course and finegrained collisions)
        ball.handlePaddleCollision(paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT);

        // Check if ball went past paddle, if so increase loss counter
        loss_count = ball.checkLossCondition(loss_count);

        //  Clear the buffer (all pixels to off, black)
        display.clearDisplay();

        // Draw the paddle
        display.drawRect(paddle_x, paddle_y, PADDLE_HEIGHT, PADDLE_WIDTH, SSD1306_WHITE);

        // Draw the ball
        display.fillCircle(ball.getX(), ball.getY(), ball.getRadius(), SSD1306_WHITE);

        // Draw scoreboard
        for (int i = 0; i < loss_count; i++)
        {
            display.drawPixel(SCREEN_WIDTH - 1, i * 4, SSD1306_WHITE);
        }

        // Push data to SSDD1306
        display.display();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}