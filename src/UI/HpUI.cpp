#include "HpUI.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include <sstream>
#include <iomanip>

// HpUI 클래스의 메인 드로우 함수
void HpUI::draw(Player& player, Boss& boss, sf::RenderWindow& window) {
    drawPlayerUI(player, window);
    drawBossUI(boss, window);
}

// 기존의 drawPlayerUI 함수 코드를 그대로 가져와 클래스 멤버 함수로 만듭니다.
void HpUI::drawPlayerUI(Player& player, const sf::RenderWindow& window) {
    ImGui::SetNextWindowPos(ImVec2(10, window.getSize().y - 100.f));
    ImGui::SetNextWindowSize(ImVec2(300, 100));

    ImGui::Begin("PlayerStatus", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Player HP");
    float healthFraction = static_cast<float>(player.getHP()) / static_cast<float>(player.getMaxHP());
    std::string healthText = std::to_string(player.getHP()) + " / " + std::to_string(player.getMaxHP());
    ImGui::ProgressBar(healthFraction, ImVec2(-1, 0), healthText.c_str());

    ImGui::Spacing();
    ImGui::End();
}

// 기존의 drawBossUI 함수 코드를 그대로 가져와 클래스 멤버 함수로 만듭니다.
void HpUI::drawBossUI(Boss& boss, const sf::RenderWindow& window) {
    float windowWidth = static_cast<float>(window.getSize().x);
    ImGui::SetNextWindowPos(ImVec2(windowWidth / 2 - 200, 10));
    ImGui::SetNextWindowSize(ImVec2(400, 60));

    ImGui::Begin("BossStatus", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("BOSS HP");
    float healthFraction = static_cast<float>(boss.getHP()) / static_cast<float>(boss.getMaxHP());
    std::string healthText = std::to_string(boss.getHP()) + " / " + std::to_string(boss.getMaxHP());
    ImGui::ProgressBar(healthFraction, ImVec2(-1, 20), healthText.c_str());

    ImGui::End();
}