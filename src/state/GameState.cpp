#include "GameState.h"

// ❗️❗️❗️ 생성자의 멤버 초기화 리스트를 사용하여 sf::Sprite를 초기화합니다.
// 이렇게 하면 "no default constructor" 오류를 해결할 수 있습니다.
GameState::GameState(sf::RenderWindow& window)
    : m_window(window)
{
    m_player = std::make_unique<Player>();
    m_boss = std::make_unique<Boss>();
}

void GameState::handleInput(const sf::Event& event)
{
    // 플레이어의 입력을 처리합니다.
    m_player->handleInput(event);
}

void GameState::update(sf::Time deltaTime)
{
    // 플레이어와 보스의 상태를 업데이트합니다.
    m_player->update(deltaTime);
    m_boss->update(deltaTime);

    // TODO: 여기에 플레이어와 보스 간의 충돌 처리, 스킬 로직 등을 추가합니다.
}

void GameState::draw(sf::RenderWindow& window)
{
    // 배경을 그리는 코드를 제거합니다.
    // 배경색은 Game::render()의 window.clear()에서 처리됩니다.

    // 플레이어와 보스만 그립니다.
    if (m_player)
    {
        m_player->draw(window);
    }
    if (m_boss)
    {
        m_boss->draw(window);
    }
}
