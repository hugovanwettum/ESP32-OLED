#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #include "esp_random.h"

#include "settings.h"

// Display object to write to
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

extern SemaphoreHandle_t paddle_position_variable_semaphore;

extern uint16_t sharedVariable_paddle_position;

void screenTask(void *pvParameters)
{

    // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
    Wire.begin(5, 4);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
    {
        Serial.println(F("SSD1306 failed"));
        for (;;)
            ;
    }

    int loss_count = 0;

    // Ball starting position
    uint16_t ball_radius = 4;
    uint16_t ball_x = SCREEN_HEIGHT - ball_radius;
    uint16_t ball_y = random(ball_radius, SCREEN_WIDTH - ball_radius);

    // Ball 'speed'
    uint16_t Dx = 2;
    uint16_t Dy = 1;

    // Paddle distance from bottom side
    uint16_t paddle_x = ball_radius * 2;

    for (;;)
    {
        // Update the paddle position every loop to get latest value (it is a global variable)
        uint16_t paddle_y = sharedVariable_paddle_position;

        // Update ball position
        ball_x += Dx;
        ball_y += Dy;

        // Check for collisions
        // X coordinate right side screen
        if (ball_x + ball_radius >= SCREEN_WIDTH - 1)
        {
            ball_x = SCREEN_WIDTH - 1 - ball_radius;
            Dx = -Dx;
        }

        // I could limit collision check to when ball is in bottom half of screen instead of every frame if needed for performance
        // Paddle collision here!
        float testX = ball_x;
        float testY = ball_y;

        // Compute which edge is closest
        if (ball_y < paddle_y)
        { // test left edge
            testY = paddle_y;
        }
        else if (ball_y > paddle_y + PADDLE_WIDTH)
        { // right edge
            testY = paddle_y + PADDLE_WIDTH;
        }
        if (ball_x > paddle_x + PADDLE_HEIGHT)
        { // test top edge
            testX = paddle_x + PADDLE_HEIGHT;
        }
        else if (ball_x < paddle_x)
        { // bottom edge
            testX = paddle_x;
        }

        // Compute distance from ball to closest edge
        float distX = ball_x - testX;
        float distY = ball_y - testY;
        float distance = sqrt((distX * distX) + (distY * distY));

        // If distance is less than radius, they collided!
        if (distance < ball_radius)
        {

            ball_x = paddle_x + PADDLE_HEIGHT + ball_radius;
            // bounce back
            Dx = -Dx;
        }

        // Lose condition:
        if (ball_x - ball_radius <= 0)
        {
            loss_count++;
            // reset
            Dx = 2;
            ball_x = SCREEN_HEIGHT - ball_radius;
            ball_y = random(ball_radius, SCREEN_WIDTH - ball_radius);
            Serial.println("You lost!");
        }

        // Y coordinate bottom of screen
        if (ball_y + ball_radius >= SCREEN_HEIGHT - 1)
        {
            ball_y = SCREEN_HEIGHT - 1 - ball_radius;
            Dy = -Dy;
        }
        // Y coordinate top of screen
        if (ball_y - ball_radius <= 0)
        {
            ball_y = 0 + ball_radius;
            Dy = -Dy;
        }

        //  Clear the buffer (all pixels to off, black)
        display.clearDisplay();

        // Draw the paddle
        display.drawRect(paddle_x, paddle_y, PADDLE_HEIGHT, PADDLE_WIDTH, SSD1306_WHITE);

        // Draw the ball
        display.fillCircle(ball_x, ball_y, ball_radius, SSD1306_WHITE);

        // Draw scoreboard
        for (int i = 0; i < loss_count; i++)
        {
            display.drawPixel(SCREEN_WIDTH - 1, i * 4, SSD1306_WHITE);
        }

        // Push data currently in RAM to SSDD1306
        display.display();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}