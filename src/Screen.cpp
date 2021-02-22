#include "Screen.h"
#include <iostream>

Screen::Screen(){
    window.create( sf::VideoMode( pWidth*scale, pHeight*scale) , "Gameboy");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
}

void Screen::connect( MMU* p_mmu )
{
    this->p_mmu = p_mmu;
}

void Screen::loop() 
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    window.clear( background );
    render();
    window.display();
}

void Screen::render()
{
    int address = SEGMENT_VIDEO_RAM.start;
    int pixel_no = 0;
    for (size_t r = 0; r < pHeight; r++)
    {
        for (size_t c = 0; c < pWidth; c++)
        {
            int mask = 0x3 << ( 6 - 2 * (pixel_no % 4) );

            int index = (p_mmu->read( address ) & mask) >> ( 6 - 2 * (pixel_no % 4) ) ;

            sf::RectangleShape rectangle(sf::Vector2f(scale, scale));
            rectangle.move(c*scale, r*scale);
            rectangle.setFillColor( palette[ index ] );
            window.draw(rectangle);
            pixel_no++;
            if( pixel_no % 4 == 0 ) address++;
        }
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