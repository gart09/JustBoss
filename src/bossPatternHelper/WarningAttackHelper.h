#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player;
class Boss;

struct AttackTimings {
    float preDelay;
    float activeDuration;
    float recoveryDuration;
};

struct AttackColors {
    sf::Color warningColor;
    sf::Color activeColor;
};

// 패턴 실행의 공통 로직을 담당하는 '부품' 클래스
class WarningAttackHelper {
public:
    WarningAttackHelper();

    void start(
        const std::vector<sf::FloatRect>& hitboxes,
        int damage,
        const AttackTimings& timings,
        const AttackColors& colors
    );
    
    void update(float dt, Player& player, const Boss& boss);
    void draw(sf::RenderTarget& target);
    bool isFinished() const;

private:
    float m_timer;
    bool m_isFinished;
    bool m_hasDealtDamage;
    int m_damage;

    AttackTimings m_timings;
    AttackColors m_colors;

    std::vector<sf::FloatRect> m_hitboxes;
    std::vector<sf::RectangleShape> m_visualRects;
};