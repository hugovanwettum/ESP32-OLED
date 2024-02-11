#include "paddle.h"

Paddle::Paddle(uint16_t startX, uint16_t startY, uint16_t height, uint16_t width) : _x(startX), _y(startY), _height(height), _width(width) {}

// Setters
void Paddle::setX(uint16_t x)
{
    _x = x;
}

void Paddle::setY(uint16_t y)
{
    _y = y;
}

void Paddle::setHeight(uint16_t height)
{
    _height = height;
}

void Paddle::setWidth(uint16_t width)
{
    _width = width;
}


// Getters
uint16_t Paddle::getX() const
{
    return _x;
}

uint16_t Paddle::getY() const
{
    return _y;
}

uint16_t Paddle::getHeight() const
{
    return _height;
}

uint16_t Paddle::getWidth() const
{
    return _width;
}