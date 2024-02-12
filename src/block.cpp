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

    // What edges should be tested?
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
    float distX = abs(ballX - testX);
    float distY = abs(ballY - testY);
    float distance = sqrt((distX * distX) + (distY * distY));

    // If distance is less than radius, they collided!
    if (distance < ballRadius)
    {
        // Determine which edge type was collided with and return the corresponding integer value
        if (testY == _y || testY == _y + _width)
        {
            // Collision with side of block
            return 1;
        }
        else
        {
            // Collision with top or bottom of block
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
