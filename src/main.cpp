#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void Task1_setup();
void Task1(void *pvParameters);

void setup()
{
	delay(10);
	Serial.begin(115200);
	Serial.println("Booting...\n");

	Task1_setup();

	xTaskCreate(
		Task1,	 // Task function
		"Task1", // Task name
		2048,	 // Stack size (words not bytes)
		NULL,	 // Task input parameter
		1,		 // Priority
		NULL);	 // Task handle
}

void loop()
{
	// Empty, FreeRTOS task scheduler takes over
}
