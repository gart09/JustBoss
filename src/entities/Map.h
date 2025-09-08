#pragma once

#include <SFML/Graphics.hpp>

class Map
{
public:
    // 생성자: 맵 객체가 생성될 때 바닥 초기화를 담당합니다.
    Map();

    // 그리기 함수: 맵의 모든 요소(지금은 바닥)를 창에 그립니다.
    void draw(sf::RenderWindow& window);

private:
    // 바닥을 나타내는 사각형 객체
    sf::RectangleShape m_floor;
};