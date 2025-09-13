#include "WarningAttackHelper.h"
#include "../entities/Player.h"
#include "../entities/Boss.h"
#include <algorithm>

WarningAttackHelper::WarningAttackHelper() : m_timer(0.f), m_isFinished(true), m_hasDealtDamage(false), m_damage(0) {}

void WarningAttackHelper::start(
    const std::vector<sf::FloatRect>& hitboxes,
    int damage,
    const AttackTimings& timings,
    const AttackColors& colors
) {
    m_hitboxes = hitboxes;
    m_damage = damage;
    m_timings = timings;
    m_colors = colors;
    
    m_timer = 0.f;
    m_isFinished = false;
    m_hasDealtDamage = false;

    // 시각 효과용 사각형들도 vector 기반으로 생성
    m_visualRects.clear();
    for (const auto& hitbox : m_hitboxes) {
        sf::RectangleShape rect;
        rect.setPosition(hitbox.position);
        rect.setSize({hitbox.size.x, hitbox.size.y});
        rect.setFillColor(sf::Color::Transparent);
        m_visualRects.push_back(rect);
    }
}

void WarningAttackHelper::update(float dt, Player& player, Boss& boss) {
    if (m_isFinished) return;

    m_timer += dt;
    float totalDuration = m_timings.preDelay + m_timings.activeDuration + m_timings.recoveryDuration;

    if (m_timer >= m_timings.preDelay && m_timer < m_timings.preDelay + m_timings.activeDuration) {
        // 공격 판정은 모든 히트박스에 대해 검사
        if (!m_hasDealtDamage) {
            for (const auto& hitbox : m_hitboxes) {
                if (hitbox.findIntersection(player.getHitbox())) {
                    player.takeDamage(m_damage, boss.getPosition());
                    m_hasDealtDamage = true;
                    break; // 한 번만 피해를 주도록 루프 탈출
                }
            }
        }
    } else if (m_timer >= totalDuration) {
        boss.resetVelocity();
        m_isFinished = true;
    }
}

void WarningAttackHelper::draw(sf::RenderTarget& target) {
    if (m_isFinished) return;
    
    sf::Color currentColor;
    // 타이머에 따라 현재 그려야 할 색상을 결정
    if (m_timer < m_timings.preDelay) {
        currentColor = m_colors.warningColor;
    } else if (m_timer < m_timings.preDelay + m_timings.activeDuration) {
        currentColor = m_colors.activeColor;
    } else {
        float recoveryProgress = (m_timer - (m_timings.preDelay + m_timings.activeDuration)) / m_timings.recoveryDuration;
        int alpha = static_cast<int>(m_colors.activeColor.a * (1.0f - recoveryProgress));
        currentColor = sf::Color(m_colors.activeColor.r, m_colors.activeColor.g, m_colors.activeColor.b, std::max(0, alpha));
    }

    // 모든 시각적 사각형에 대해 색상을 적용하고 그림
    if (currentColor.a > 0) {
        for (auto& rect : m_visualRects) {
            rect.setFillColor(currentColor);
            target.draw(rect);
        }
    }
}

bool WarningAttackHelper::isFinished() const {
    return m_isFinished;
}