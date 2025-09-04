#include "DebugUI.h"
#include "entities/Player.h" // Player Ŭ������ ���Ǹ� �˾ƾ� getAirControlForcePtr()�� ȣ���� �� �ֽ��ϴ�.

// ImGui ���̺귯�� �� SFML ������ ���� ��� ���ϵ��� �����մϴ�.
#include "../lib/imgui/imgui.h"
#include "../lib/imgui-sfml/imgui-SFML.h"

DebugUI::DebugUI(sf::RenderWindow& window)
{
    // ImGui�� ImGui-SFML�� �ʱ�ȭ�ϰ�, ���� â�� �����մϴ�.
    ImGui::SFML::Init(window);
}

DebugUI::~DebugUI()
{
    // ���α׷� ���� �� ImGui ���ҽ��� �����մϴ�.
    ImGui::SFML::Shutdown();
}

void DebugUI::processEvent(sf::RenderWindow& window, const sf::Event& event)
{
    ImGui::SFML::ProcessEvent(window, event);
}

void DebugUI::update(sf::Time deltaTime)
{
    // Game::update()���� ImGui::SFML::Update()�� ȣ��� �Ŀ� �� �Լ��� ����˴ϴ�.
    // ���⼭�� ���� UI ��ҵ��� �����ϴ� ���Ҹ� �մϴ�.

    // ������� �÷��̾� ��ü�� ��ȿ���� Ȯ���մϴ�.
    if (m_player)
    {
        // "Player Debug"��� �̸��� �� ImGui â�� �����մϴ�.
        ImGui::Begin("Player Debug");

        // Player ��ü�κ��� �����Ϸ��� ������ �޸� �ּ�(������)�� �����ɴϴ�.
        float* playerAirControlPtr = m_player->getAirControlForcePtr();

        // "Air Control Force"��� ���� ���� float �Է� ���ڸ� ����ϴ�.
        // �� �Է� ���ڴ� playerAirControlPtr�� ����Ű�� ������ ���� ���� �����մϴ�.
        ImGui::InputFloat("Air Control Force", playerAirControlPtr, 1.0f, 100.0f, "%.1f");

        // ImGui â ������ ��Ĩ�ϴ�.
        ImGui::End();
    }
}

void DebugUI::render(sf::RenderWindow& window)
{
    // update() �Լ����� ������ ��� UI ��ҵ��� ���� â�� �׸��ϴ�.
    ImGui::SFML::Render(window);
}

void DebugUI::setPlayer(Player* player)
{
    // Game Ŭ������ ���� �÷��̾� ��ü�� �˷��ָ�, ��� ������ �����մϴ�.
    m_player = player;
}

