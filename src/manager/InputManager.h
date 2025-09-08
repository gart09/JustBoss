#pragma once

#include <SFML/Window/Event.hpp>
#include <memory>
#include <map>
#include <SFML/Window/Keyboard.hpp>

class ICommand;

class InputManager {
public:
    static InputManager& getInstance();

    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

    // 이벤트 기반 입력 처리 (키 누름, 뗌)
    std::unique_ptr<ICommand> handleEvent(const sf::Event& event);
    // 실시간 입력 처리 (키 누르고 있는 상태)
    std::unique_ptr<ICommand> handleRealtimeInput();

private:
    InputManager();
    // 키 매핑 (향후 이 부분을 파일에서 읽어오도록 수정하면 키 설정 기능 구현 가능)
    std::map<sf::Keyboard::Key, int> m_keyMappings; // int로 커맨드 타입을 저장
};

