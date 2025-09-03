#include "InputManager.h"

InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key)
{
    // SFML의 키보드 함수를 그대로 호출하여 결과를 반환합니다.
    return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button)
{
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i InputManager::getMousePosition(sf::RenderWindow& window)
{
    return sf::Mouse::getPosition(window);
}
