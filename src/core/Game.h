#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Scene/GameScene.h"
#include "../manager/InputManager.h"

class Game{
public:
    Game();
    void run();

private:
    void handleInput();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow m_window;
    std::unique_ptr<GameScene> m_currentScene;
    std::unique_ptr<ICommand> m_command;
};