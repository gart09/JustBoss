#include "InputManager.h"
#include "../commands/PlayerCommand.h"


InputManager& InputManager::getInstance() {
    // 이 함수가 처음 호출될 때 단 한 번만 instance가 생성됩니다.
    // C++11 표준부터는 이 방식이 Thread-safe 하도록 보장됩니다.
    static InputManager instance; 
    return instance;
}

InputManager::InputManager() {
    // 키 누름에 대한 커맨드 매핑. 1:점프, 2:공격, 3:대쉬, 4:패링, 5:약점공격
    m_keyMappings[sf::Keyboard::Key::D] = 1;
    m_keyMappings[sf::Keyboard::Key::S] = 2;
    m_keyMappings[sf::Keyboard::Key::A] = 3;
    m_keyMappings[sf::Keyboard::Key::Q] = 4;
    m_keyMappings[sf::Keyboard::Key::E] = 5;
}

std::unique_ptr<ICommand> InputManager::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        auto it = m_keyMappings.find(event.getIf<sf::Event::KeyPressed>()->code);
        if (it != m_keyMappings.end()) {
            switch (it->second) {
                case 1: return std::make_unique<JumpCommand>();
                case 2: return std::make_unique<AttackCommand>();
                case 3: return std::make_unique<DashCommand>();
                case 4: return std::make_unique<ParryCommand>();
                case 5: return std::make_unique<WeakPointAttackCommand>();
            }
        }
    }
    return nullptr;
}

std::unique_ptr<ICommand> InputManager::handleRealtimeInput() {
    float direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        direction -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        direction += 1.0f;
    }

    if (direction != 0.0f) {
        return std::make_unique<MoveCommand>(direction);
    }
    return nullptr;
}

