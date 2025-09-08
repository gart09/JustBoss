#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <functional>
#include <map>

class ICommand; // 전방 선언

class InputManager 
{
public:
    static InputManager& getInstance();

    // 복사 및 대입을 금지합니다.
    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

    std::unique_ptr<ICommand> processEvent(const sf::Event& event);
    std::unique_ptr<ICommand> processPolling();

private:
    InputManager(); // private 생성자
    ~InputManager() = default;

    // 키를 '눌렀을 때' 실행될 커맨드를 생성하는 함수를 매핑합니다.
    std::map<sf::Keyboard::Key, std::function<std::unique_ptr<ICommand>()>> m_pressedKeyMappings;
    
    // 키를 '뗐을 때' 실행될 커맨드를 생성하는 함수를 매핑합니다.
    std::map<sf::Keyboard::Key, std::function<std::unique_ptr<ICommand>()>> m_releasedKeyMappings;
};