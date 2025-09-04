#pragma once

#include <SFML/Graphics.hpp>

// Player 클래스의 전체 정의를 포함하는 대신, 전방 선언을 사용하여
// 컴파일 시간을 줄이고 헤더 간의 의존성을 낮춥니다.
class Player; 

// ImGui의 초기화, 업데이트, 렌더링을 전담하는 관리 클래스입니다.
class DebugUI
{
public:
    // 생성자: 게임 창에 대한 참조를 받아 ImGui를 초기화합니다.
    DebugUI(sf::RenderWindow& window);
    
    // 소멸자: ImGui를 종료합니다.
    ~DebugUI();

    // SFML 이벤트를 ImGui에 전달하여 마우스 클릭, 키보드 입력 등을 처리합니다.
    void processEvent(sf::RenderWindow& window, const sf::Event& event);
    
    // 매 프레임마다 호출되어 UI 창과 위젯(입력 상자 등)을 구성합니다.
    void update(sf::Time deltaTime);
    
    // 최종적으로 구성된 UI를 화면에 그립니다.
    void render(sf::RenderWindow& window);

    // GameState로부터 Player 객체에 대한 포인터를 받아옵니다.
    void setPlayer(Player* player);

private:
    Player* m_player = nullptr; // 디버깅할 플레이어 객체에 대한 포인터
};

