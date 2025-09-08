#include "../entities/Map.h"

// 게임 전체에서 사용할 상수들을 이곳에서도 정의하거나
// 별도의 전역 설정 파일(e.g., Config.h)을 만들어 포함(include)하는 것이 좋습니다.
constexpr float WINDOW_WIDTH = 1280.f;
constexpr float GROUND_Y = 720.f;

// 생성자 구현
Map::Map()
{
    // 바닥 크기 설정
    m_floor.setSize(sf::Vector2f(WINDOW_WIDTH, 200.f));
    
    // 바닥 색상 설정
    m_floor.setFillColor(sf::Color(139, 69, 19)); // SaddleBrown 색상

    // 바닥 위치 설정
    m_floor.setPosition({0.f, GROUND_Y});
}

// 그리기 함수 구현
void Map::draw(sf::RenderWindow& window)
{
    // 멤버 변수인 m_floor를 창에 그립니다.
    window.draw(m_floor);
}