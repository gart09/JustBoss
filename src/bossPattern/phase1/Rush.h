#pragma once
#include "../IPattern.h"

class Rush : public IPattern {
public:
    Rush();

    bool canExecute(const Boss& boss, const Player& player) const override;
    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    void draw(sf::RenderTarget& target) override;
    bool isFinished() const override;

    std::optional<sf::FloatRect> getWeakPointHitbox() const override;

private:
    // 돌진 패턴의 내부 단계를 나타내는 열거형
    enum class Phase { Preparing, Dashing, Done };
    Phase m_phase;

    // 패턴 상태 변수
    float m_timer;                // 현재 단계의 경과 시간
    sf::Vector2f m_rushDirection; // 보스가 돌진할 방향
    bool m_hasHitPlayer;          // 이번 돌진에서 플레이어를 타격했는지 여부
    bool m_isWeakPointActive;     // 약점 활성화 여부

    // 시각 효과용 객체
    sf::RectangleShape m_warningSign;
    sf::RectangleShape m_weakPointVisual;
    
    // 데이터
    sf::FloatRect m_weakPointHitbox; // 약점의 판정 범위
};