#pragma once
#include "../IPattern.h"
#include <vector>
#include "../../src/bossPatternHelper/WarningAttackHelper.h"

class TripleSlam : public IPattern {
public:
    TripleSlam();

    bool canExecute(const Boss& boss, const Player& player) const override;
    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    void draw(sf::RenderTarget& target) override;
    bool isFinished() const override;
    std::optional<sf::FloatRect> getWeakPointHitbox() const override;

private:
    // 이 패턴의 내부 상태를 관리
    int m_slamIndex;        // 현재 몇 번째 공격인지 (0, 1, 2)
    float m_phaseTimer;     // 현재 단계(선딜/공격/후딜)의 경과 시간
    sf::RectangleShape m_visualRect;

    // 각 공격 단계별 히트박스와 안전지대 정보를 미리 저장할 변수
    std::vector<sf::FloatRect> m_slamHitboxes[3];
    AttackTimings m_slamTimings[3];
    std::vector<sf::FloatRect> m_safeZones[2];

    // 약점 관련 변수
    sf::FloatRect m_weakPointHitbox;
    bool m_isWeakPointActive;

    // 중복 타격 방지 플래그
    bool m_hasDealtDamage;

    // 시각 효과용 객체
    sf::RectangleShape m_attackVisual;     // 공격 범위 표시
    sf::RectangleShape m_safeZoneVisual;   // 안전지대 표시
    sf::RectangleShape m_weakPointVisual;  // 약점 표시
};