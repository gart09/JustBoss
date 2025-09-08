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
    while (const auto event = m_window.pollEvent())
    {
        // 1. is<>() 템플릿 함수로 이벤트 타입을 확인합니다.
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
        if (auto command = InputManager::getInstance().handleEvent(*event))
        {
            m_commands.push_back(std::move(command));
        }
    }
    if (auto command = InputManager::getInstance().handleRealtimeInput())
    {
        m_commands.push_back(std::move(command));
    }
}

void Game::update(sf::Time deltaTime)
{
    if (m_currentScene)
    {
        for (const auto& command : m_commands)
        {
            m_currentScene->handleCommand(*command);
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
