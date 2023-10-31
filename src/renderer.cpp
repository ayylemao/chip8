#include "../include/renderer.h"

Renderer::Renderer(sf::RenderWindow &in_window, CPU &in_cpu) 
: window(in_window), cpu(in_cpu)
{
    image.create(dispX, dispY);
}


void Renderer::drawBuffer()
{
    for (int x = 0; x < cpu.DISPN_X; x++)
    {
        cpu.display[0][x] = 1;
    }

    for (int x = 0; x < cpu.DISPN_X; x++)
        for (int y = 0; y < cpu.DISPN_Y; y++)
    {
        if (cpu.display[y][x] == true)
        {
            image.setPixel(x, y, sf::Color::Yellow);
        }
        else
        {
            image.setPixel(x, y, sf::Color::Black);
        }
    }
}