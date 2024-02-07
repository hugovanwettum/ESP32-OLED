#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Initialize semaphore
SemaphoreHandle_t radius_variable_semaphore = xSemaphoreCreateBinary();

void screenTask(void *pvParameters);
void serverTask(void *pvParameters);

uint16_t sharedVariable_radius = 5;

void setup()
{
	delay(10);
	Serial.begin(115200);
	Serial.println("Booting...\n");

	// Initialize semaphore
	vSemaphoreCreateBinary(radius_variable_semaphore);

	xTaskCreate(
		screenTask, // Task function
		"Task1",	// Task name
		2048,		// Stack size (words not bytes)
		NULL,		// Task input parameter
		1,			// Priority
		NULL);		// Task handle

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
