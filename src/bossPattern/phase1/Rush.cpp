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
    m_weakPointVisual.setFillColor(sf::Color(255, 255, 0, 150)); // 약점은 노란색으로 표시
    m_weakPointVisual.setOutlineColor(sf::Color::White);
    m_weakPointVisual.setOutlineThickness(2.f);
}

bool Rush::canExecute(const Boss& boss, const Player& player) const {
    float distance = std::abs(boss.getCenter().x - player.getCenter().x);
    return currentCooldown <= 0 || distance > RUSH_TRIGGER_DISTANCE;
}

void Rush::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    m_phase = Phase::Preparing;
    m_timer = 0.f;
    m_hasHitPlayer = false;
    m_isWeakPointActive = true;

    // 돌진 방향 결정
    m_rushDirection.x = (player.getCenter().x > boss.getCenter().x) ? 1.f : -1.f;
    m_rushDirection.y = 0;

    // 1. 워닝 사인 설정 (보스부터 벽 끝까지)
    float bossX = boss.getPosition().x;
    float bossWidth = boss.getSize();
    if (m_rushDirection.x > 0) {
        m_warningSign.setPosition({bossX + bossWidth, 720.f - 100.f - boss.getSize()});
        m_warningSign.setSize({WORLD_MAX_X - (bossX + bossWidth), boss.getSize()});
    } else {
        m_warningSign.setPosition({0, 720.f - 100.f - boss.getSize()});
        m_warningSign.setSize({bossX, boss.getSize()});
    }

    // 2. 약점 히트박스 설정 (보스 중앙)
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
                m_isWeakPointActive = false; // 돌진 시작 시 약점 사라짐
                boss.setVelocity({m_rushDirection.x * RUSH_SPEED, 0});
            }
            break;
        }
        case Phase::Dashing: {
            // 플레이어와 충돌 판정
            if (!m_hasHitPlayer && boss.getHitbox().findIntersection(player.getHitbox())) {
                player.takeDamage(RUSH_DAMAGE, boss.getPosition());
                m_hasHitPlayer = true;
                // 플레이어와 부딪혀도 돌진은 계속됨
            }
            // 벽과 충돌 판정
            float bossX = boss.getPosition().x;
            if (bossX <= 0 || bossX + boss.getSize() >= WORLD_MAX_X) {
                boss.setVelocity({0, 0}); // 벽에 부딪히면 멈춤
                m_phase = Phase::Done;    // 패턴 종료
            }
            break;
        }
        case Phase::Done:
            break;
    }
}

void Rush::draw(sf::RenderTarget& target) {
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