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

    // IPattern의 가상 함수를 재정의하여 약점 히트박스를 외부에 제공
    std::optional<sf::FloatRect> getWeakPointHitbox() const override;

private:
    enum class Phase { Preparing, Dashing, Done };
    Phase m_phase;

    float m_timer;
    sf::Vector2f m_rushDirection;
    bool m_hasHitPlayer;

    // 시각 효과
    sf::RectangleShape m_warningSign;
    sf::RectangleShape m_weakPointVisual;
    
    // 데이터
    sf::FloatRect m_weakPointHitbox;
    bool m_isWeakPointActive;
};