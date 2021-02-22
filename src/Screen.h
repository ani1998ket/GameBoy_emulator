#include <SFML/Window.hpp>
#include "MMU.h"

class Screen{
public:
    Screen();
    void connect(MMU* p_mmu);
    void render();
    int get_width();
    int get_height();
    bool is_open();
private:
    MMU* p_mmu;
    sf::Window window;

    const int pWidth = 160;
    const int pHeight = 144;
    const int scale = 5;
};