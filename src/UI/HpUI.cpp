#include "HpUI.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include <sstream>
#include <iomanip>

void HpUI::draw(Player& player, Boss& boss, sf::RenderWindow& window) {
    drawPlayerUI(player, window);
    drawBossUI(boss, window);
}

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