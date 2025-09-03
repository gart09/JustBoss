#include "InputManager.h"

InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button)
{
    return sf::Mouse::isButtonPressed(button);
}
