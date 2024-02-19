# ESP32 OLED breakout clone
This project is a [Breakout](https://en.wikipedia.org/wiki/Breakout_(video_game)) clone created using an ESP32 microcontroller and a monochrome OLED screen. In this game, the player controls a paddle to bounce a ball and break bricks. 

<img src="https://github.com/hugovanwettum/ESP32-OLED/assets/2574678/85e89700-0c60-43e1-b2ca-52af35725153" alt="ESP32-OLED-breakout" width="256"/>

Everything is implemented using the real-time operating system [FreeRTOS](https://www.freertos.org/). FreeRTOS manages tasks related to game logic, graphics rendering, and hosting a web server for paddle control. 

<img src="https://github.com/hugovanwettum/ESP32-OLED/assets/2574678/5ac3904b-beee-4e2c-9145-616af15fb270" alt="ESP32-OLED-breakout" width="400"/>

[Jira](https://www.atlassian.com/software/jira) is used for issue tracking and agile development.

# Compiling
This project is being developed in VS Code using PlatformIO. The project depends on the [Adafruit SSD1306 library](https://github.com/adafruit/Adafruit_SSD1306), which is automatically installed via PlatformIO's `platformio.ini` file.

# License
See [LICENCE.md](LICENCE.md)
