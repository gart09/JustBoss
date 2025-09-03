#pragma once

#include <SFML/Graphics.hpp>

// 싱글턴(Singleton) 디자인 패턴을 사용한 입력 관리자
class InputManager
{
public:
    static InputManager& getInstance();

    // 특정 키가 현재 눌려있는지 확인합니다.
    bool isKeyPressed(sf::Keyboard::Key key);
    // 특정 마우스 버튼이 현재 눌려있는지 확인합니다.
    bool isMouseButtonPressed(sf::Mouse::Button button);
    // 현재 마우스의 위치를 창 기준으로 반환합니다.
    sf::Vector2i getMousePosition(sf::RenderWindow& window);

    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

private:
    InputManager() {}
};
