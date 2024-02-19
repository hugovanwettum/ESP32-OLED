#include <algorithm>

#include "block.h"
#include "helperFunctions.h" //clamp

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
    // Define a small tolerance value
    const float epsilon = 0.1;

    // Calculate the closest point on the block's boundary to the ball
    float closestX = clamp(ballX, _x, _x + _height);
    float closestY = clamp(ballY, _y, _y + _width);

    // Calculate distance between the ball and the closest point on the block
    float distanceX = ballX - closestX;
    float distanceY = ballY - closestY;
    // Euclidean distance using Pythagorean theorem
    float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

    // If distance is less than radius, they collided!
    if (distance <= ballRadius)
    {
        // Determine which edge type was collided with and return the corresponding integer value
        if (fabs(distanceY) > fabs(distanceX))
        {
            // The ball is closer to the sides than the top or bottom
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
