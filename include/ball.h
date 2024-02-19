#ifndef BALL_H
#define BALL_H

#include <Arduino.h>

class Ball
{
public:
    // Constructor
    Ball(uint16_t radius, uint16_t startX, uint16_t startY, uint16_t speedX, uint16_t speedY);

    // Move the ball
    void move();

    // Handle collisions with the walls
    void handleWallCollision();
    void handlePaddleCollision(uint16_t paddleX, uint16_t paddleY, uint16_t paddleWidth, uint16_t paddleHeight);

    // Check if ball went past paddle
    uint16_t checkLossCondition(uint16_t lossCount);

    // Setters
    void setX(uint16_t x);
    void setY(uint16_t y);
    void setRadius(uint16_t radius);
    void setdX(uint16_t Dx);
    void setdY(uint16_t Dy);

    // Getters
    uint16_t getX() const;
    uint16_t getY() const;
    uint16_t getRadius() const;
    uint16_t getdX() const;
    uint16_t getdY() const;

private:
    uint16_t _radius;
    uint16_t _x, _y;
    float _dX, _dY;
};

#endif // BALL_H
