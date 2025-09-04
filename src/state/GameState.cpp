#include "GameState.h"
#include "../lib/imgui/imgui.h"  
#include "../lib/imgui-sfml/imgui-SFML.h"

GameState::GameState(sf::RenderWindow& window)
    : m_window(window)
{
    // ImGui-SFML 초기화
    ImGui::SFML::Init(m_window); // <<< 추가

    m_player = std::make_unique<Player>();
    m_boss = std::make_unique<Boss>();
}

// 소멸자 구현
GameState::~GameState() // <<< 추가된 부분
{
    ImGui::SFML::Shutdown();
}

void GameState::handleInput(const sf::Event& event)
{
    ImGui::SFML::ProcessEvent(m_window, event);
    m_player->handleInput(event);
}

void GameState::update(sf::Time deltaTime)
{
    ImGui::SFML::Update(m_window, deltaTime);
    m_player->update(deltaTime);
    m_boss->update(deltaTime);

    ImGui::Begin("Player Debug");
    // Player 객체에서 m_airControlForce 변수의 포인터를 가져옵니다.
    float* airControlForcePtr = m_player->getAirControlForcePtr();
    float* speed = m_player->getSpeed();

    // SliderFloat 위젯을 사용하여 변수를 조작합니다.
    // "Air Control" 이라는 라벨을 가지며, 0.0f 부터 1.0f 까지 조절 가능합니다.
    ImGui::SliderFloat("Air Control", airControlForcePtr, 0.0f, 1.0f);
    ImGui::SliderFloat("Speed", speed, 0.0f, 1000.0f);

    // 현재 값도 텍스트로 표시해줄 수 있습니다.
    ImGui::Text("Current Value: %f", *airControlForcePtr);
    ImGui::Text("Speed: %f", *speed);

    ImGui::End(); // 창을 닫습니다.

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
    // 모든 게임 오브젝트를 그린 후, 마지막에 ImGui UI를 그립니다.
    ImGui::SFML::Render(window); // <<< 추가        
}
