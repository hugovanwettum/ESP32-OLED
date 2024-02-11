#include "ball.h"
#include "settings.h"

Ball::Ball(uint16_t radius, uint16_t startX, uint16_t startY, uint16_t speedX, uint16_t speedY) : _radius(radius), _x(startX), _y(startY), _dX(speedX), _dY(speedY) {}

void Ball::move()
{
    _x += _dX;
    _y += _dY;
}

void Ball::handleWallCollision()
{
    // X coordinate (collision with top of screen)
    if (_x + _radius >= SCREEN_WIDTH - 1)
    {
        // Make sure ball doesn't escape screen
        _x = SCREEN_WIDTH - 1 - _radius;
        // Reverse direction
        _dX = -_dX;
    }

    // Y coordinate (right side of screen)
    if (_y + _radius >= SCREEN_HEIGHT - 1)
    {
        // Make sure ball doesn't escape screen
        _y = SCREEN_HEIGHT - 1 - _radius;
        // Reverse direction
        _dY = -_dY;
    }

    // Y coordinate (left side of screen)
    if (_y - _radius <= 0)
    {
        // Make sure ball doesn't escape screen
        _y = _radius;
        // Reverse direction
        _dY = -_dY;
    }
}

void Ball::handlePaddleCollision(uint16_t paddleX, uint16_t paddleY, uint16_t paddleWidth, uint16_t paddleHeight)
{
    float testX = _x;
    float testY = _y;

    // Compute which edge is closest
    if (_y < paddleY)
    { // test left edge
        testY = paddleY;
    }
    else if (_y > paddleY + paddleWidth)
    { // right edge
        testY = paddleY + paddleWidth;
    }
    if (_x > paddleX + paddleHeight)
    { // test top edge
        testX = paddleX + paddleHeight;
    }
    else if (_x < paddleX)
    { // bottom edge
        testX = paddleX;
    }

    // Compute distance from ball to closest edge
    float distX = _x - testX;
    float distY = _y - testY;
    float distance = sqrt((distX * distX) + (distY * distY));

    // If distance is less than radius, they collided!
    if (distance < _radius)
    {
        _x = paddleX + paddleHeight + _radius;
        // bounce back
        _dX = -_dX;
    }
}

uint16_t Ball::checkLossCondition(uint16_t lossCount)
{
    // If the ball reaches the bottom of the screen
    if (_x - _radius <= 0)
    {
        lossCount++;
        // Also reset the ball position
        _x = _radius * 3;
        _y = random(_radius, SCREEN_WIDTH - _radius);
        // And reset the direction
        _dX = 2;
    }

    return lossCount;
}

void Ball::setX(uint16_t x)
{
    _x = x;
}

void Ball::setY(uint16_t y)
{
    _y = y;
}

void Ball::setRadius(uint16_t radius)
{
    _radius = radius;
}

void Ball::setdX(uint16_t dX)
{
    _dX = dX;
}

void Ball::setdY(uint16_t dY)
{
    _dY = dY;
}

uint16_t Ball::getX() const
{
    return _x;
}

uint16_t Ball::getY() const
{
    return _y;
}

uint16_t Ball::getRadius() const
{
    return _radius;
}

uint16_t Ball::getdX() const
{
    return _dX;
}

uint16_t Ball::getdY() const
{
    return _dY;
}