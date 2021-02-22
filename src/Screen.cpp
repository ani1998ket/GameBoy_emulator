#include "Screen.h"

Screen::Screen(){
    window.create( sf::VideoMode( pWidth*scale, pHeight*scale) , "Gameboy");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
}

void Screen::connect( MMU* p_mmu )
{
    this->p_mmu = p_mmu;
}

void Screen::render() 
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

int Screen::get_width() 
{
    return window.getSize().x;
}

int Screen::get_height() 
{
    return window.getSize().y;
}

bool Screen::is_open()
{
    return window.isOpen();
}