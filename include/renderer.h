#ifndef RENDERER_H
#define RENDERER_H

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "settings.h"
#include "ball.h"
#include "paddle.h"

class Renderer
{
public:
    Renderer(Adafruit_SSD1306 &display);

    void drawBall(const Ball& ball);
    void drawPaddle(const Paddle& paddle);
    void drawScore(int lossCount);

private:
    // Display object to write to

    Adafruit_SSD1306 _display;
};

#endif // RENDERER_H
