#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "settings.h"

// Initialize semaphore
SemaphoreHandle_t paddle_position_variable_semaphore = xSemaphoreCreateBinary();

// Handle for screentask:
TaskHandle_t screenTaskHandle; // Task 2 handle

void screenTask(void *pvParameters);
void serverTask(void *pvParameters);

// Start paddle in middle of screen
uint16_t sharedVariable_paddle_position = SCREEN_HEIGHT / 2 - PADDLE_WIDTH/2;

void setup()
{
	delay(10);
	Serial.begin(115200);
	Serial.println("Booting...\n");

	// Initialize semaphore
	vSemaphoreCreateBinary(paddle_position_variable_semaphore);

	xTaskCreate(
		screenTask, // Task function
		"Task1",	// Task name
		2048,		// Stack size (words not bytes)
		NULL,		// Task input parameter
		1,			// Priority
		&screenTaskHandle);		// Task handle

	xTaskCreate(
		serverTask, // Task function
		"Task2",	// Task name
		4096,		// Stack size (words not bytes)
		NULL,		// Task input parameter
		1,			// Priority
		NULL);		// Task handle
}

void loop()
{
	// Empty, FreeRTOS task scheduler takes over
}
