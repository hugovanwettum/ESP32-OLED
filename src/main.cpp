#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Display object to write to
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Bitmap array that will be sent to the display
uint8_t bitmap[1024] = {};

// Toggle between white or black screen
bool white_or_black = 0;

void Task1(void *pvParameters) {
  for( ; ; ){
    Serial.print("Task 1 is running: ");
    // Clear the buffer.
    display.clearDisplay();
  
    if(white_or_black == 0){
      Serial.println("white!");
      // Generate white bitmap
      for (int i = 0; i < 1024; i++){
        bitmap[i] = 0xff;
      }
      white_or_black = !white_or_black;
    }else{
      Serial.println("black!");
      // Generate black bitmap
      for (int i = 0; i < 1024; i++){
        bitmap[i] = 0x00;
      }
      white_or_black = !white_or_black;
    }

    // Draw bitmap on the screen
    display.drawBitmap(0, 0, bitmap, 128, 64, 1);
    display.display();

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  delay(10);
  Serial.begin(115200);
  Serial.println("Booting...\n");

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 failed"));
    for(;;);
  }
  delay(2000); //TODO: needed?
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Generate white bitmap
  for (int i = 0; i < 1024; i++){
    bitmap[i] = 0xff;
  }
  
  // Draw bitmap on the screen
  display.drawBitmap(0, 0, bitmap, 128, 64, 1);
  display.display();

  xTaskCreate(
    Task1,                   // Task function
    "Task1",                 // Task name
    2048,                    // Stack size (words not bytes)
    NULL,                    // Task input parameter
    1,                       // Priority
    NULL);                   // Task handle
}

void loop() {
  // Empty, FreeRTOS task scheduler takes over
}
