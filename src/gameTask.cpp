#include <Arduino.h>
#include <vector>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

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

void displayQRCode(esp_qrcode_handle_t qrcode) {
    // Credits: https://www.hackster.io/mdraber/generate-qr-codes-with-arduino-on-oled-display-53c074
    
    // Clear the display
    display.clearDisplay();

    // Calculate the scale factor
    int scale = min(display.width() / esp_qrcode_get_size(qrcode), display.height() / esp_qrcode_get_size(qrcode));
  
    // Calculate horizontal shift
    int shiftX = (display.width() - esp_qrcode_get_size(qrcode) * scale) / 2;
  
    // Calculate vertical shift
    int shiftY = (display.height() - esp_qrcode_get_size(qrcode) * scale) / 2;

    // Draw the QR code on the display
    for (uint8_t y = 0; y < esp_qrcode_get_size(qrcode); y++) {
        for (uint8_t x = 0; x < esp_qrcode_get_size(qrcode); x++) {
            if (esp_qrcode_get_module(qrcode, x, y)) {
                display.fillRect(shiftX + x * scale, shiftY + y * scale, scale, scale, WHITE);
            }
        }
    }

    // Update the display
    display.display();
}

void generateQRCode(const char* text) {
    // Define the configuration for QR code generation
    esp_qrcode_config_t config = ESP_QRCODE_CONFIG_DEFAULT();

    // Set the display function to the custom function given above
    config.display_func = displayQRCode;

    // Generate the QR code
    esp_err_t err = esp_qrcode_generate(&config, text);
    if (err != ESP_OK) {
        // Handle error
        printf("Failed to generate QR code. Error code: %d\n", err);
    }
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

    // Create a QR code for the webpage to connect to
    // For now considered static, can also get the IP from the server task
    generateQRCode("192.168.1.87");

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