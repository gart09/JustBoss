#include "core/Game.h"
#include "../lib/imgui/imgui.h"
#include "../lib/imgui-sfml/imgui-SFML.h"

// Game 클래스 생성자
Game::Game() : m_window(sf::VideoMode({1280, 720}), "Just Boss")
{
    // 게임 시작 시 첫 화면으로 GameScene을 생성합니다.
    m_currentScene = std::make_unique<GameScene>(m_window);
}

// 게임의 메인 루프를 실행하는 함수
void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Time deltaTime = clock.restart(); // 이전 프레임부터 지금까지 걸린 시간
        
        // 매 프레임마다 입력, 업데이트, 렌더링 순서로 실행
        handleInput();
        update(deltaTime);
        render();
    }
}

// 입력을 처리하는 함수
void Game::handleInput()
{
    // 게임이 끝난 상태(클리어/패배)에서는 더 이상 입력을 받지 않습니다.
    if(m_currentScene->isGameOver())
        return;
    
    // 매 프레임 커맨드를 초기화합니다.
    m_command = nullptr;

    // SFML 이벤트를 순회하며 처리합니다. (키 누름/뗌, 창 닫기 등)
    while (const auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        // 이벤트 기반의 커맨드(점프, 공격 등)가 있는지 확인합니다.
        auto command = InputManager::getInstance().processEvent(*event);
        if (command) {
            m_command = std::move(command);
            break; // 이벤트 커맨드는 한 프레임에 하나만 처리
        }
    }

    // 만약 이벤트 기반 커맨드가 없었다면, 지속적인 입력(이동)을 확인합니다.
    if (!m_command) {
        m_command = InputManager::getInstance().processPolling();
    }
}

// 게임의 논리적인 상태를 업데이트하는 함수
void Game::update(sf::Time deltaTime)
{
    if (m_currentScene)
    {
        // handleInput에서 생성된 커맨드가 있다면, 현재 씬에 전달하여 처리합니다.
        if (m_command) {
            m_currentScene->handleCommand(*m_command);
        }
        // 현재 씬의 업데이트 로직을 실행합니다.
        m_currentScene->update(deltaTime);
    }
}

// 모든 것을 화면에 그리는 함수
void Game::render()
{
    m_window.clear(sf::Color::Black);

    if (m_currentScene)
    {
        m_currentScene->draw(m_window);
    }
    m_window.display();
}