#include "DashUI.h"
#include "imgui.h"
#include "imgui-sfml.h"

void DashUI::draw(Player& player, sf::RenderWindow& window) {
    ImGui::SetNextWindowPos(ImVec2(320.f, 620.f));
    ImGui::SetNextWindowSize(ImVec2(200, 50));

    ImGui::Begin("DashStatus", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    // 3. UI 내용 그리기
    ImGui::Text("Dash CoolTime"); 
    
    float progress = player.getDashCooldownProgress();

    if (progress >= 1.0f) {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.8f, 0.0f, 1.0f)); // 진한 녹색
    }

    // ProgressBar를 그립니다.
    ImGui::ProgressBar(progress, ImVec2(-1, 0));

    // 5. 색상을 변경했다면, 반드시 원래대로 되돌립니다.
    if (progress >= 1.0f) {
        ImGui::PopStyleColor();
    }

    ImGui::End(); // 창을 닫습니다.
}