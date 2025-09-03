#include "GameState.h"

GameState::GameState(sf::RenderWindow& window)
    : m_window(window)
{
    m_player = std::make_unique<Player>();
    m_boss = std::make_unique<Boss>();
}

void GameState::handleInput(const sf::Event& event)
{
    m_player->handleInput(event);
}

void GameState::update(sf::Time deltaTime)
{
    m_player->update(deltaTime);
    m_boss->update(deltaTime);

    // TODO: 여기에 플레이어와 보스 간의 충돌 처리, 스킬 로직 등을 추가합니다.
}

void GameState::draw(sf::RenderWindow& window)
{
    if (m_player)
    {
        m_player->draw(window);
    }
    if (m_boss)
    {
        m_boss->draw(window);
    }
}
