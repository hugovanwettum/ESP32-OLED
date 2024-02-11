#ifndef BLOCK_H
#define BLOCK_H

#include <Arduino.h>

class Block
{
public:
    Block(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    // Getters
    uint16_t getX() const;
    uint16_t getY() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;

    // Check collision with ball
    int checkCollision(uint16_t ballX, uint16_t ballY, uint16_t ballRadius) const;

    // Destroy the block (e.g., after collision with ball)
    void destroy();

    // Check if the block is destroyed
    bool isDestroyed() const;

private:
    uint16_t _x;
    uint16_t _y;
    uint16_t _width;
    uint16_t _height;
    bool _destroyed;
};

#endif // BLOCK_H
