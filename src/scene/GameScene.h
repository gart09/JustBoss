#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "../entities/Player.h"
#include "../entities/Boss.h"
#include "../entities/Map.h"
#include "../UI/HpUI.h"
#include "../commands/ICommand.h"

class GameScene
{
public:
    GameScene(sf::RenderWindow& window);
    ~GameScene();

    void handleCommand(ICommand& command);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    
    Player* getPlayer() { return m_player.get(); }
private:
    sf::RenderWindow& m_window; // 창에 대한 참조
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Boss> m_boss;
    std::unique_ptr<HpUI> m_hpUI;
    std::unique_ptr<Map> m_map;
};
