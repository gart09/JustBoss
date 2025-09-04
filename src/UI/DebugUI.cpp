#include "DebugUI.h"
#include "entities/Player.h" // Player 클래스의 정의를 알아야 getAirControlForcePtr()를 호출할 수 있습니다.

// ImGui 라이브러리 및 SFML 연동을 위한 헤더 파일들을 포함합니다.
#include "../lib/imgui/imgui.h"
#include "../lib/imgui-sfml/imgui-SFML.h"

DebugUI::DebugUI(sf::RenderWindow& window)
{
    // ImGui와 ImGui-SFML을 초기화하고, 게임 창과 연결합니다.
    ImGui::SFML::Init(window);
}

DebugUI::~DebugUI()
{
    // 프로그램 종료 시 ImGui 리소스를 정리합니다.
    ImGui::SFML::Shutdown();
}

void DebugUI::processEvent(sf::RenderWindow& window, const sf::Event& event)
{
    ImGui::SFML::ProcessEvent(window, event);
}

void DebugUI::update(sf::Time deltaTime)
{
    // Game::update()에서 ImGui::SFML::Update()가 호출된 후에 이 함수가 실행됩니다.
    // 여기서는 실제 UI 요소들을 구성하는 역할만 합니다.

    // 디버깅할 플레이어 객체가 유효한지 확인합니다.
    if (m_player)
    {
        // "Player Debug"라는 이름의 새 ImGui 창을 시작합니다.
        ImGui::Begin("Player Debug");

        // Player 객체로부터 수정하려는 변수의 메모리 주소(포인터)를 가져옵니다.
        float* playerAirControlPtr = m_player->getAirControlForcePtr();

        // "Air Control Force"라는 라벨을 가진 float 입력 상자를 만듭니다.
        // 이 입력 상자는 playerAirControlPtr가 가리키는 변수의 값을 직접 수정합니다.
        ImGui::InputFloat("Air Control Force", playerAirControlPtr, 1.0f, 100.0f, "%.1f");

        // ImGui 창 구성을 마칩니다.
        ImGui::End();
    }
}

void DebugUI::render(sf::RenderWindow& window)
{
    // update() 함수에서 구성된 모든 UI 요소들을 실제 창에 그립니다.
    ImGui::SFML::Render(window);
}

void DebugUI::setPlayer(Player* player)
{
    // Game 클래스가 현재 플레이어 객체를 알려주면, 멤버 변수에 저장합니다.
    m_player = player;
}

