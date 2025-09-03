#include "core/Game.h"

Game::Game() : m_window(sf::VideoMode({1280, 720}), "Just Boss")
{
    m_currentState = std::make_unique<GameState>(m_window);
}

void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    // --- ❗️ SFML 3.0.0 이벤트 루프 (완전히 변경됨) ❗️ ---
    while (const auto event = m_window.pollEvent())
    {
        // 1. is<>() 템플릿 함수로 이벤트 타입을 확인합니다.
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        // 2. 현재 상태에 이벤트 객체 자체를 전달합니다.
        if (m_currentState)
        {
            m_currentState->handleInput(*event);
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    if (m_currentState)
    {
        m_currentState->update(deltaTime);
    }
}

void Game::render()
{
    m_window.clear(sf::Color(50, 50, 50));

    if (m_currentState)
    {
        m_currentState->draw(m_window);
    }

    m_window.display();
}
