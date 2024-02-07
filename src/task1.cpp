#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "settings.h"

// Display object to write to
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void Task1_setup()
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

void Task1(void *pvParameters)
{

    uint16_t point_radius = 5;
    uint16_t point_x = point_radius;
    uint16_t point_y = SCREEN_HEIGHT / 2;

    int16_t Vx = 1;
    int16_t Vy = 1;

    for (;;)
    {
        // Serial.println("Task 1 is running: ");
        //  Clear the buffer (all pixels to off, black)
        display.clearDisplay();

        // Update the x position based on velocity
        point_x += Vx;
        point_y += Vy;

        // Check for collisions
        // X coordinate right side screen
        if (point_x + point_radius >= SCREEN_WIDTH - 1)
        {
            point_x = SCREEN_WIDTH - 1 - point_radius;
            Vx = -Vx;
        }
        // X coordinate left side screen
        if (point_x - point_radius <= 0)
        {
            point_x = 0 + point_radius;
            Vx = -Vx;
        }
        // Y coordinate bottom of screen
        if (point_y + point_radius >= SCREEN_HEIGHT - 1)
        {
            point_y = SCREEN_HEIGHT - 1 - point_radius;
            Vy = -Vy;
        }
        // Y coordinate top of screen
        if (point_y - point_radius <= 0)
        {
            point_y = 0 + point_radius;
            Vy = -Vy;
        }

        // Draw the current position
        display.fillCircle(point_x, point_y, point_radius, SSD1306_WHITE);

        // Push data currently in RAM to SSDD1306
        display.display();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}