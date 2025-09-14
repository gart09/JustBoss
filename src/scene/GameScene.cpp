#include "GameScene.h"
#include "../lib/imgui/imgui.h"  
#include "../lib/imgui-sfml/imgui-SFML.h"
#include <sstream>   // 문자열 스트림 사용
#include <iomanip>   // 소수점 자리수 지정을 위해 사용

GameScene::GameScene(sf::RenderWindow& window)
    : m_window(window), m_gameState(GameState::Playing)
{
    // ImGui-SFML 초기화
    ImGui::SFML::Init(m_window, true); // <<< 추가

    m_player = std::make_unique<Player>();
    m_boss = std::make_unique<Boss>();
    m_map = std::make_unique<Map>();
    m_hpUI = std::make_unique<HpUI>();
    m_dashUI = std::make_unique<DashUI>();
}

// 소멸자 구현
GameScene::~GameScene() // <<< 추가된 부분
{
    ImGui::SFML::Shutdown();
}

void GameScene::handleCommand(ICommand& command)
{
    // 전달받은 커맨드를 플레이어에게 넘겨 실행합니다.
    command.execute(*m_player);
}

void GameScene::update(sf::Time deltaTime)
{
    ImGui::SFML::Update(m_window, deltaTime);
    if(!isGameOver()) {
    m_player->update(deltaTime, *m_boss);
    m_boss->update(deltaTime, *m_player);

    ImGui::SetNextWindowPos(ImVec2(10, 100));
    ImGui::SetNextWindowBgAlpha(0.35f); 
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImGui::Begin("Instructions", nullptr, window_flags);

    ImGui::Text("Q: ChargeAttack      W: WeakAttack");
    ImGui::Text("A: Dash(Can Skip Attack Motions. 5s Cooldown)   S: Attack   D: Jump");
    ImGui::Text("D when Jump: Double Jump  /  W when Jump: Air Stop and WeakAttack");
    ImGui::Text("When the boss's body glows, hitting that part with the W attack will put the boss in a groggy state. ");
    ImGui::Text("It takes double damage for 3 seconds.");
    ImGui::End();
    }

    if (m_gameState == GameState::Playing) {
        // ...
        if (m_boss->getHP() <= 0) {
            m_gameState = GameState::Clear;
            m_restartClock.restart();
            m_isGameOver = true;
        }
        else if (m_player->getHP() <= 0) {
            m_gameState = GameState::Defeat;
            m_restartClock.restart();
            m_isGameOver = true;
        }
    } else {
        if (m_restartClock.getElapsedTime().asSeconds() >= RESTART_DELAY) {
            resetGame();
            m_isGameOver = false;
        }
    }
}

void GameScene::draw(sf::RenderWindow& window)
{
    if (m_boss) { m_boss->draw(window); }
    if (m_player) { m_player->draw(window); }
    if (m_map) { m_map->draw(window);}
    if (m_hpUI) { m_hpUI->draw(*m_player, *m_boss, window); }
    if (m_dashUI) { m_dashUI->draw(*m_player, window); }

    if (m_gameState != GameState::Playing) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        ImGui::Begin("Game Result", nullptr, flags);

        const char* resultMessage = (m_gameState == GameState::Clear) ? "Game Clear!" : "Game Defeat.";
        
        float remainingTime = RESTART_DELAY - m_restartClock.getElapsedTime().asSeconds();
        if (remainingTime < 0) remainingTime = 0;
        
        std::stringstream ss;
        ss << "Restarting in " << std::fixed << std::setprecision(1) << remainingTime << "s...";
        std::string countdownText = ss.str();

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::SetWindowFontScale(2.5f);
        ImGui::Text("%s", resultMessage);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Text("%s", countdownText.c_str());
        ImGui::PopFont();

        ImGui::End();
    }
    ImGui::SFML::Render(window);    
}

void GameScene::resetGame() {
    std::cout << "--- Restarting Game ---" << std::endl;
    m_player = std::make_unique<Player>();
    m_boss = std::make_unique<Boss>();
    m_gameState = GameState::Playing;
}