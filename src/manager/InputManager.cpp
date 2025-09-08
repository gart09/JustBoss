#include "InputManager.h"
#include "../commands/PlayerCommand.h"

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager() {
    // --- 키를 '눌렀을 때'의 행동을 매핑합니다 ---
    m_pressedKeyMappings[sf::Keyboard::Key::D] = []() { 
        float direction = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction = -1.f;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction = 1.f;
        return std::make_unique<JumpCommand>(direction); 
    };
    m_pressedKeyMappings[sf::Keyboard::Key::S] = []() { return std::make_unique<AttackCommand>(); };
    m_pressedKeyMappings[sf::Keyboard::Key::Q] = []() { return std::make_unique<StartChargeCommand>(); };
    m_pressedKeyMappings[sf::Keyboard::Key::W] = []() { return std::make_unique<WeakPointAttackCommand>(); };
    m_pressedKeyMappings[sf::Keyboard::Key::A] = []() {
        float direction = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction = -1.f;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction = 1.f;
        return std::make_unique<DashCommand>(direction);
    };

    // --- 키를 '뗐을 때'의 행동을 매핑합니다 ---
    m_releasedKeyMappings[sf::Keyboard::Key::Q] = []() { return std::make_unique<ReleaseChargeCommand>(); };
}

std::unique_ptr<ICommand> InputManager::processEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        auto it = m_pressedKeyMappings.find(keyPressed->code);
        if (it != m_pressedKeyMappings.end())
            return it->second();
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        auto it = m_releasedKeyMappings.find(keyReleased->code);
        if (it != m_releasedKeyMappings.end())
            return it->second();
    }

    // 해당하는 이벤트가 없으면 nullptr 반환
    return nullptr;
}

std::unique_ptr<ICommand> InputManager::processPolling() {
    float direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        direction -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        direction += 1.0f;
    }
    
    return std::make_unique<MoveCommand>(direction);
}