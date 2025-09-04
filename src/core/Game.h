#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "state/GameState.h"

class Game{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow m_window;
    std::unique_ptr<GameState> m_currentState;  
};