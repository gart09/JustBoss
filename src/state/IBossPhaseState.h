#pragma once
#include "../entities/Boss.h"
#include "../entities/Player.h"

class Boss; // 전방 선언
class Player;

class IBossPhaseState {
public:
    virtual void enter(Boss& boss) = 0; // 상태 진입 시 초기화
    virtual void update(Boss& boss, float dt, Player& player) = 0;
    virtual void exit(Boss& boss) = 0; // 상태 종료 시 정리
    virtual void draw(sf::RenderTarget& target) = 0; // 상태에 따른 시각적 요소 그리기
};