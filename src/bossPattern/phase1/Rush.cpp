#include "Rush.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>

namespace {
    constexpr float RUSH_PREPARE_TIME = 1.5f;
    constexpr float RUSH_SPEED = 800.f;
    constexpr float RUSH_TRIGGER_DISTANCE = 700.f;
    constexpr int RUSH_DAMAGE = 30;
    constexpr float WORLD_MAX_X = 1280.f;
}

Rush::Rush() : m_phase(Phase::Done), m_timer(0.f), m_hasHitPlayer(false), m_isWeakPointActive(false) {
    cooldown = 20.f;
    currentCooldown = 10.f;
    m_warningSign.setFillColor(sf::Color(255, 165, 0, 80));
    m_weakPointVisual.setFillColor(sf::Color(255, 255, 0, 150));
    m_weakPointVisual.setOutlineColor(sf::Color::White);
    m_weakPointVisual.setOutlineThickness(2.f);
}

bool Rush::canExecute(const Boss& boss, const Player& player) const {
    float distance = std::abs(boss.getCenter().x - player.getCenter().x);
    // 쿨타임이 지났거나, 플레이어가 일정 거리 이상 멀어졌을 때 발동
    return currentCooldown <= 0 || distance > RUSH_TRIGGER_DISTANCE;
}

void Rush::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    m_phase = Phase::Preparing;
    m_timer = 0.f;
    m_hasHitPlayer = false;
    m_isWeakPointActive = true;

    m_rushDirection.x = (player.getCenter().x > boss.getCenter().x) ? 1.f : -1.f;
    m_rushDirection.y = 0;

    // 보스의 돌진 경로 전체를 경고 표시로 설정합니다.
    float bossX = boss.getPosition().x;
    float bossWidth = boss.getSize();
    if (m_rushDirection.x > 0) {
        m_warningSign.setPosition({bossX + bossWidth, 720.f - 100.f - boss.getSize()});
        m_warningSign.setSize({WORLD_MAX_X - (bossX + bossWidth), boss.getSize()});
    } else {
        m_warningSign.setPosition({0, 720.f - 100.f - boss.getSize()});
        m_warningSign.setSize({bossX, boss.getSize()});
    }

    // 돌진 준비 자세에서 노출될 약점의 위치를 설정합니다.
    sf::Vector2f weakPointSize = {60.f, 60.f};
    m_weakPointHitbox.position.x = boss.getCenter().x - weakPointSize.x / 2;
    m_weakPointHitbox.position.y = boss.getCenter().y - weakPointSize.y / 2;
    m_weakPointHitbox.size.x = weakPointSize.x;
    m_weakPointHitbox.size.y = weakPointSize.y;
    m_weakPointVisual.setSize(weakPointSize);
    m_weakPointVisual.setPosition({m_weakPointHitbox.position.x, m_weakPointHitbox.position.y});
}

void Rush::update(float dt, Boss& boss, Player& player) {
    if (isFinished()) return;

    switch (m_phase) {
        case Phase::Preparing: {
            m_timer += dt;
            if (m_timer >= RUSH_PREPARE_TIME) {
                m_phase = Phase::Dashing;
                m_isWeakPointActive = false;
                boss.setVelocity({m_rushDirection.x * RUSH_SPEED, 0});
            }
            break;
        }
        case Phase::Dashing: {
            if (!m_hasHitPlayer && boss.getHitbox().findIntersection(player.getHitbox())) {
                player.takeDamage(RUSH_DAMAGE, boss.getPosition());
                m_hasHitPlayer = true;
            }
            // 보스가 벽에 부딪히면 패턴을 종료합니다.
            float bossX = boss.getPosition().x;
            if (bossX <= 0 || bossX + boss.getSize() >= WORLD_MAX_X) {
                boss.setVelocity({0, 0});
                m_phase = Phase::Done;
            }
            break;
        }
        case Phase::Done:
            break;
    }
}

void Rush::draw(sf::RenderTarget& target) {
    // 돌진 준비 단계일 때만 경고 사인과 약점을 그립니다.
    if (m_phase == Phase::Preparing) {
        target.draw(m_warningSign);
        if(m_isWeakPointActive) target.draw(m_weakPointVisual);
    }
}

bool Rush::isFinished() const {
    return m_phase == Phase::Done;
}

std::optional<sf::FloatRect> Rush::getWeakPointHitbox() const {
    if (m_isWeakPointActive) {
        return m_weakPointHitbox;
    }
    return std::nullopt;
}