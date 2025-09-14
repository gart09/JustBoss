#include "core/Game.h"
#include "../lib/imgui/imgui.h"
#include "../lib/imgui-sfml/imgui-SFML.h"

Game::Game() : m_window(sf::VideoMode({1280, 720}), "Just Boss")
{
    m_currentScene = std::make_unique<GameScene>(m_window);
}

void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        handleInput();
        update(deltaTime);
        render();
    }
}

void Game::handleInput()
{
    if(m_currentScene->isGameOver())
        return;
    m_command = nullptr;
    while (const auto event = m_window.pollEvent())
    {
        // 1. is<>() 템플릿 함수로 이벤트 타입을 확인합니다.
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
        auto command = InputManager::getInstance().processEvent(*event);
        if (command) {
            m_command = std::move(command);
            break; 
        }
    }
    if (!m_command) {
        m_command = InputManager::getInstance().processPolling();
    }
}

void Game::update(sf::Time deltaTime)
{
    if (m_currentScene)
    {
        if (m_command) {
            m_currentScene->handleCommand(*m_command);
        }
        m_currentScene->update(deltaTime);
    }
}

void Game::render()
{
    m_window.clear(sf::Color::Black);

    if (m_currentScene)
    {
        m_currentScene->draw(m_window);
    }
    m_window.display();
}
