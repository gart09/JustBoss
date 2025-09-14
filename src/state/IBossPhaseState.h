#pragma once
#include "../entities/Boss.h"
#include "../entities/Player.h"
#include "../enum/BossPhaseEnum.h"

class Boss;
class Player;
class IPattern;

class IBossPhaseState {
public:
    virtual void enter(Boss& boss) = 0; // 상태 진입 시 초기화
    virtual void update(Boss& boss, float dt, Player& player) = 0;
    virtual void exit(Boss& boss) = 0; // 상태 종료 시 정리
    virtual void draw(sf::RenderTarget& target, Boss& boss) = 0; // 상태에 따른 시각적 요소 그리기
    virtual IPattern* getCurrentPattern() const = 0;
    virtual bool isGroggyState() const { return false; }
    virtual PhaseID getPhaseID() const = 0;

};