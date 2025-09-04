#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "../entities/Player.h"
#include "../entities/Boss.h"

class GameState
{
public:
    GameState(sf::RenderWindow& window);
    ~GameState();

    void handleInput(const sf::Event& event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    
    Player* getPlayer() { return m_player.get(); }
private:
    sf::RenderWindow& m_window; // 창에 대한 참조
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Boss> m_boss;
};
