#ifndef PADDLE_H
#define PADDLE_H

#include <Arduino.h>

class Paddle
{
public:
    // Constructor
    Paddle(uint16_t startX, uint16_t startY, uint16_t height, uint16_t width);

    // Setters
    void setX(uint16_t x);
    void setY(uint16_t y);
    void setHeight(uint16_t height);
    void setWidth(uint16_t width);

    // Getters
    uint16_t getX() const;
    uint16_t getY() const;
    uint16_t getHeight() const;
    uint16_t getWidth() const;

private:
    uint16_t _x, _y;
    uint16_t _height, _width;
};

#endif // PADDLE_H
