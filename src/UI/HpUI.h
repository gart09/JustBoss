#pragma once
#include <SFML/Graphics.hpp>
#include "../entities/Player.h" // Player 데이터를 알아야 하므로 포함
#include "../entities/Boss.h"   // Boss 데이터를 알아야 하므로 포함

class HpUI {
public:
    // Player와 Boss 객체를 받아와서 UI를 그립니다.
    void draw(Player& player, Boss& boss, sf::RenderWindow& window);

private:
    // 내부적으로만 사용할 UI 렌더링 함수들
    void drawPlayerUI(Player& player, const sf::RenderWindow& window);
    void drawBossUI(Boss& boss, const sf::RenderWindow& window);
};