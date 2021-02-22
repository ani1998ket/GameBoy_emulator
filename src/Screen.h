#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "MMU.h"

class Screen{
public:
    Screen();
    void connect(MMU* p_mmu);
    void loop();
    int get_width();
    int get_height();
    bool is_open();
private:
    MMU* p_mmu;
    sf::RenderWindow window;

    const int pWidth = 160;
    const int pHeight = 144;
    const int scale = 10;

    sf::Color background = sf::Color(0xcadc9fff);
    sf::Color palette[4]{sf::Color(0x0f380fff), sf::Color(0x306230ff), sf::Color(0x8bac0fff), sf::Color(0x9bbc0fff)};

    void render();
};