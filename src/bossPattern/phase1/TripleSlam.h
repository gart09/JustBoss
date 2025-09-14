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
    int m_slamIndex;        // 현재 몇 번째 공격인지 추적 (0, 1, 2)
    float m_phaseTimer;     // 현재 공격 단계(선딜/공격/후딜)의 경과 시간

    // 시각 효과용 객체
    sf::RectangleShape m_visualRect;      // 공격/경고 범위를 그리는 데 사용되는 공용 사각형
    sf::RectangleShape m_weakPointVisual;   // 약점을 그리는 데 사용되는 사각형

    // 패턴 데이터
    std::vector<sf::FloatRect> m_slamHitboxes[3]; // 3번의 공격에 대한 히트박스 목록
    AttackTimings m_slamTimings[3];               // 3번의 공격에 대한 타이밍 정보
    std::vector<sf::FloatRect> m_safeZones[2];    // 1, 2타의 안전지대 정보
    sf::FloatRect m_weakPointHitbox;              // 3타의 약점 히트박스

    // 패턴 상태 플래그
    bool m_isWeakPointActive; // 약점 활성화 여부
    bool m_hasDealtDamage;    // 현재 타격에서 피해를 입혔는지 여부
};