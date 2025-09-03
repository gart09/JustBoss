#pragma once

#include <SFML/Graphics.hpp>

class InputManager
{
public:
    static InputManager& getInstance();

    bool isKeyPressed(sf::Keyboard::Key key);
    bool isMouseButtonPressed(sf::Mouse::Button button);

    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

private:
    InputManager() {}
};
