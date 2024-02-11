#include <algorithm>

#include "block.h"

Block::Block(uint16_t x, uint16_t y, uint16_t width, uint16_t height) : _x(x), _y(y), _width(width), _height(height), _destroyed(false) {}

// Getters
uint16_t Block::getX() const
{
    return _x;
}

uint16_t Block::getY() const
{
    return _y;
}

uint16_t Block::getWidth() const
{
    return _width;
}

uint16_t Block::getHeight() const
{
    return _height;
}

// Check collision with ball
int Block::checkCollision(uint16_t ballX, uint16_t ballY, uint16_t ballRadius) const
{
    float testX = ballX;
    float testY = ballY;

    // Compute which edge is closest
    if (ballY < _y)
    { // test left edge
        testY = _y;
    }
    else if (ballY > _y + _width)
    { // right edge
        testY = _y + _width;
    }
    if (ballX > _x + _height)
    { // test top edge
        testX = _x + _height;
    }
    else if (ballX < _x)
    { // bottom edge
        testX = _x;
    }

    // Compute distance from ball to closest edge
    float distX = ballX - testX;
    float distY = ballY - testY;
    float distance = sqrt((distX * distX) + (distY * distY));

    // If distance is less than radius, they collided!
    if (distance < ballRadius)
    {
        if (distY < distX)
        {
            // return 1 if the collision was the side of the block
            return 1;
        }
        else
        {
            // return 2 if the collision was top or bottom of the block
            return 2;
        }
    }
    else
    {
        return 0;
    }
}

// Destroy the block (e.g., after collision with ball)
void Block::destroy()
{
    _destroyed = true;
}

// Check if the block is destroyed
bool Block::isDestroyed() const
{
    return _destroyed;
}
