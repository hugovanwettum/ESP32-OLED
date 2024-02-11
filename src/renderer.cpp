#include "renderer.h"

Renderer::Renderer(Adafruit_SSD1306 &display) : _display(display) {}

void Renderer::drawBall(const Ball& ball)
{
    // Draw a ball on the screen
    _display.fillCircle(ball.getX(), ball.getY(), ball.getRadius(), SSD1306_WHITE);
}

void Renderer::drawPaddle(const Paddle& paddle)
{
    // Draw the paddle to the screen
    _display.drawRect(paddle.getX(), paddle.getY(), paddle.getHeight(), paddle.getWidth(), SSD1306_WHITE);
}

void Renderer::drawBlock(const Block& block)
{
    _display.drawRect(block.getX(), block.getY(), block.getHeight(), block.getWidth(), SSD1306_WHITE);
}

void Renderer::drawScore(int loss_count)
{
    // For every loss, draw a pixel on the top row of the screen
    for (int i = 0; i < loss_count; i++)
    {
        _display.drawPixel(SCREEN_WIDTH - 1, i * 4, SSD1306_WHITE);
    }
}