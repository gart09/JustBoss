#include "TripleSlam.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace {
    constexpr float SAFE_ZONE_WIDTH = 100.f;
    constexpr float MIN_SAFE_ZONE_GAP = 100.f;
    constexpr int DAMAGE[3] = {20, 25, 35};

    const AttackTimings SLAM1_TIMINGS = {1.5f, 0.2f, 0.5f};
    const AttackTimings SLAM2_TIMINGS = {1.5f, 0.2f, 0.5f};
    const AttackTimings SLAM3_TIMINGS = {2.0f, 0.4f, 1.0f};

    constexpr float WORLD_MAX_X = 1280.f;
    constexpr float WORLD_HEIGHT = 620.f;
}

TripleSlam::TripleSlam() 
    : m_slamIndex(0), m_phaseTimer(0.f), m_isWeakPointActive(false), m_hasDealtDamage(false) 
{
    cooldown = 20.f;
    currentCooldown = 20.f;
    m_weakPointVisual.setFillColor(sf::Color(255, 255, 0, 150));
    finished = false;

    m_slamTimings[0] = SLAM1_TIMINGS;
    m_slamTimings[1] = SLAM2_TIMINGS;
    m_slamTimings[2] = SLAM3_TIMINGS;
}

void TripleSlam::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    m_slamIndex = 0;
    m_phaseTimer = 0.f;
    m_isWeakPointActive = false;
    
    for(int i = 0; i < 3; ++i) m_slamHitboxes[i].clear();
    for(int i = 0; i < 2; ++i) m_safeZones[i].clear();

    // 1번째 공격: 안전지대 2개를 서로 겹치지 않게 무작위로 생성합니다.
    float z1, z2;
    do {
        z1 = boss.randomFloat(0, WORLD_MAX_X - SAFE_ZONE_WIDTH);
        z2 = boss.randomFloat(0, WORLD_MAX_X - SAFE_ZONE_WIDTH);
    } while (std::abs(z1 - z2) < SAFE_ZONE_WIDTH + MIN_SAFE_ZONE_GAP);
    if (z1 > z2) std::swap(z1, z2);

    m_safeZones[0].push_back({ {z1, 0}, {SAFE_ZONE_WIDTH, WORLD_HEIGHT} });
    m_safeZones[0].push_back({{z2, 0}, {SAFE_ZONE_WIDTH, WORLD_HEIGHT}});

    // 생성된 안전지대를 제외한 나머지 모든 영역을 공격 히트박스로 설정합니다.
    m_slamHitboxes[0].push_back({{0, 0}, {z1, WORLD_HEIGHT}});
    m_slamHitboxes[0].push_back({{z1 + SAFE_ZONE_WIDTH, 0}, {z2 - (z1 + SAFE_ZONE_WIDTH), WORLD_HEIGHT}});
    m_slamHitboxes[0].push_back({{z2 + SAFE_ZONE_WIDTH, 0}, {WORLD_MAX_X - (z2 + SAFE_ZONE_WIDTH), WORLD_HEIGHT}});

    // 2번째 공격: 안전지대 1개를 무작위로 생성합니다.
    float z3 = boss.randomFloat(0, WORLD_MAX_X - SAFE_ZONE_WIDTH);
    m_safeZones[1].push_back({{z3, 0}, {SAFE_ZONE_WIDTH, WORLD_HEIGHT}});
    
    m_slamHitboxes[1].push_back({{0, 0}, {z3, WORLD_HEIGHT}});
    m_slamHitboxes[1].push_back({{z3 + SAFE_ZONE_WIDTH, 0}, {WORLD_MAX_X - (z3 + SAFE_ZONE_WIDTH), WORLD_HEIGHT}});

    // 3번째 공격: 안전지대 없이 전체 영역을 공격하며, 보스에게 약점이 노출됩니다.
    m_slamHitboxes[2].push_back({{0, 0}, {WORLD_MAX_X, WORLD_HEIGHT}});
    sf::Vector2f weakPointSize = {60.f, 60.f};
    m_weakPointHitbox.position.x = boss.getCenter().x - weakPointSize.x / 2;
    m_weakPointHitbox.position.y = boss.getCenter().y - weakPointSize.y / 2;
    m_weakPointHitbox.size.x = weakPointSize.x;
    m_weakPointHitbox.size.y = weakPointSize.y;
}

bool TripleSlam::canExecute(const Boss& boss, const Player& player) const {
    std::cout << "TripleSlam canExecute check: cooldown = " << currentCooldown << ", isFinished: " << isFinished() << std::endl;
    return currentCooldown <= 0;
}

void TripleSlam::update(float dt, Boss& boss, Player& player) {
    if (isFinished()) return;
    m_phaseTimer += dt;
    
    const AttackTimings& currentTimings = m_slamTimings[m_slamIndex];
    float totalSlamDuration = currentTimings.preDelay + currentTimings.activeDuration + currentTimings.recoveryDuration;

    // 현재 시간이 공격 활성 구간일 때, 아직 피해를 주지 않았다면 충돌을 검사합니다.
    if (m_phaseTimer >= currentTimings.preDelay && m_phaseTimer < currentTimings.preDelay + currentTimings.activeDuration) {
        if (!m_hasDealtDamage) {
            for (const auto& hitbox : m_slamHitboxes[m_slamIndex]) {
                if (hitbox.findIntersection(player.getHitbox())) {
                    int damage = 0;
                    switch(m_slamIndex) {
                        case 0: damage = DAMAGE[0]; break;
                        case 1: damage = DAMAGE[1]; break;
                        case 2: damage = DAMAGE[2]; break;
                    }
                    player.takeDamage(damage, boss.getPosition());
                    m_hasDealtDamage = true;
                    break;
                }
            }
        }
    }

    // 3번째 공격의 선딜레이 중에만 약점을 활성화하고, 보스의 위치를 따라다니도록 갱신합니다.
    if (m_slamIndex == 2 && m_phaseTimer < currentTimings.preDelay) {
        m_isWeakPointActive = true;
        m_weakPointHitbox.position.x = boss.getCenter().x - m_weakPointHitbox.size.x / 2;
        m_weakPointHitbox.position.y = boss.getCenter().y - m_weakPointHitbox.size.y / 2;
    } else {
        m_isWeakPointActive = false;
    }
    
    // 한 번의 타격(선딜/공격/후딜)이 모두 끝났다면 다음 타격으로 넘어갑니다.
    if (m_phaseTimer >= totalSlamDuration) {
        m_slamIndex++;
        m_phaseTimer = 0.f;
        m_hasDealtDamage = false;
        
        if (m_slamIndex >= 3) {
            finished = true;
        }
    }
}

void TripleSlam::draw(sf::RenderTarget& target) {
    if (isFinished()) return;

    const AttackTimings& currentTimings = m_slamTimings[m_slamIndex];

    // 선딜레이 구간에는 주황색 경고 범위(워닝 사인)를 표시합니다.
    if (m_phaseTimer < currentTimings.preDelay) {
        m_visualRect.setFillColor(sf::Color(255, 165, 0, 100));
        
        for (const auto& hitbox : m_slamHitboxes[m_slamIndex]) {
            m_visualRect.setPosition({hitbox.position.x, hitbox.position.y});
            m_visualRect.setSize({hitbox.size.x, hitbox.size.y});
            target.draw(m_visualRect);
        }

        if (m_isWeakPointActive) {
            m_weakPointVisual.setPosition({m_weakPointHitbox.position.x, m_weakPointHitbox.position.y});
            m_weakPointVisual.setSize({m_weakPointHitbox.size.x, m_weakPointHitbox.size.y});
            target.draw(m_weakPointVisual);
        }
    }
    // 공격 활성 구간에는 붉은색 공격 범위를 표시합니다.
    else if (m_phaseTimer < currentTimings.preDelay + currentTimings.activeDuration) {
        m_visualRect.setFillColor(sf::Color(255, 0, 0, 100));

        for (const auto& hitbox : m_slamHitboxes[m_slamIndex]) {
            m_visualRect.setPosition({hitbox.position.x, hitbox.position.y});
            m_visualRect.setSize({hitbox.size.x, hitbox.size.y});
            target.draw(m_visualRect);
        }
    }
}

bool TripleSlam::isFinished() const {
    return finished;
}

std::optional<sf::FloatRect> TripleSlam::getWeakPointHitbox() const {
    if (m_isWeakPointActive) {
        return m_weakPointHitbox;
    }
    return std::nullopt;
}