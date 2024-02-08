#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

    // Start at 1 pixel above border
    uint16_t paddle_x = 1;

    for (;;)
    {
        // Update the paddle position every loop to get latest value (it is a global variable)
        uint16_t paddle_y = sharedVariable_paddle_position;

        //  Clear the buffer (all pixels to off, black)
        display.clearDisplay();

        // Draw the current position
        display.drawRect(paddle_x, paddle_y, PADDLE_HEIGHT, PADDLE_WIDTH, SSD1306_WHITE);
        // Push data currently in RAM to SSDD1306
        display.display();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}