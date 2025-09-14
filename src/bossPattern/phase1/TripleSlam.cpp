#include "TripleSlam.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace {
    // --- 스킬 세부 데이터 정의 ---
    constexpr float SAFE_ZONE_WIDTH = 100.f;
    constexpr float MIN_SAFE_ZONE_GAP = 100.f;
    constexpr int DAMAGE[3] = {20, 25, 35};

    // ▼▼▼ 각 타격의 타이밍을 개별적으로 정의 ▼▼▼
    const AttackTimings SLAM1_TIMINGS = {1.5f, 0.2f, 0.5f};
    const AttackTimings SLAM2_TIMINGS = {1.5f, 0.2f, 0.5f};
    const AttackTimings SLAM3_TIMINGS = {2.0f, 0.4f, 1.0f};

    // 월드 크기 및 바닥 위치
    constexpr float WORLD_MAX_X = 1280.f;
    constexpr float WORLD_HEIGHT = 620.f;
}

TripleSlam::TripleSlam() 
    : m_slamIndex(0), m_phaseTimer(0.f), m_isWeakPointActive(false), m_hasDealtDamage(false) 
{
    cooldown = 20.f;
    currentCooldown = 20.f;
    m_weakPointVisual.setFillColor(sf::Color(255, 255, 0, 150));

    m_slamTimings[0] = SLAM1_TIMINGS;
    m_slamTimings[1] = SLAM2_TIMINGS;
    m_slamTimings[2] = SLAM3_TIMINGS;
}

void TripleSlam::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player); // 쿨타임 등 공통 초기화
    boss.setVelocity({0.f, 0.f});
    m_slamIndex = 0;
    m_phaseTimer = 0.f;
    m_isWeakPointActive = false;
    
    // 이전에 계산된 히트박스들 초기화
    for(int i = 0; i < 3; ++i) m_slamHitboxes[i].clear();
    for(int i = 0; i < 2; ++i) m_safeZones[i].clear();

    // --- 1번째 공격 (안전지대 2개) ---
    float z1, z2;
    do {
        z1 = boss.randomFloat(0, WORLD_MAX_X - SAFE_ZONE_WIDTH);
        z2 = boss.randomFloat(0, WORLD_MAX_X - SAFE_ZONE_WIDTH);
    } while (std::abs(z1 - z2) < SAFE_ZONE_WIDTH + MIN_SAFE_ZONE_GAP);
    if (z1 > z2) std::swap(z1, z2);

    m_safeZones[0].push_back({ {z1, 0}, {SAFE_ZONE_WIDTH, WORLD_HEIGHT} });
    m_safeZones[0].push_back({{z2, 0}, {SAFE_ZONE_WIDTH, WORLD_HEIGHT}});

    m_slamHitboxes[0].push_back({{0, 0}, {z1, WORLD_HEIGHT}});
    m_slamHitboxes[0].push_back({{z1 + SAFE_ZONE_WIDTH, 0}, {z2 - (z1 + SAFE_ZONE_WIDTH), WORLD_HEIGHT}});
    m_slamHitboxes[0].push_back({{z2 + SAFE_ZONE_WIDTH, 0}, {WORLD_MAX_X - (z2 + SAFE_ZONE_WIDTH), WORLD_HEIGHT}});

    // --- 2번째 공격 (안전지대 1개) ---
    float z3 = boss.randomFloat(0, WORLD_MAX_X - SAFE_ZONE_WIDTH);
    m_safeZones[1].push_back({{z3, 0}, {SAFE_ZONE_WIDTH, WORLD_HEIGHT}});
    
    m_slamHitboxes[1].push_back({{0, 0}, {z3, WORLD_HEIGHT}});
    m_slamHitboxes[1].push_back({{z3 + SAFE_ZONE_WIDTH, 0}, {WORLD_MAX_X - (z3 + SAFE_ZONE_WIDTH), WORLD_HEIGHT}});

    // --- 3번째 공격 (안전지대 없음, 약점 노출) ---
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
    
    // ▼▼▼ 현재 타격에 맞는 타이밍 정보를 가져와서 사용 ▼▼▼
    const AttackTimings& currentTimings = m_slamTimings[m_slamIndex];
    float totalSlamDuration = currentTimings.preDelay + currentTimings.activeDuration + currentTimings.recoveryDuration;

    // 현재가 공격 활성 구간인지 확인
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

    // 3번째 공격의 '선딜레이' 동안에만 약점 활성화
    if (m_slamIndex == 2 && m_phaseTimer < currentTimings.preDelay) {
        m_isWeakPointActive = true;
        m_weakPointHitbox.position.x = boss.getCenter().x - m_weakPointHitbox.size.x / 2;
        m_weakPointHitbox.position.y = boss.getCenter().y - m_weakPointHitbox.size.y / 2;
    } else {
        m_isWeakPointActive = false;
    }
    
    // 현재 타격의 주기가 끝났는지 확인
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

    // --- ▼▼▼ 그리기 로직 수정 ▼▼▼ ---
    // 현재가 선딜레이 구간이라면, '공격 범위'를 워닝 사인으로 그림
    if (m_phaseTimer < currentTimings.preDelay) {
        m_visualRect.setFillColor(sf::Color(255, 165, 0, 100)); // 주황색 워닝 컬러
        
        // 미리 계산해둔 '공격 히트박스'들을 직접 순회하며 그립니다.
        for (const auto& hitbox : m_slamHitboxes[m_slamIndex]) {
            m_visualRect.setPosition({hitbox.position.x, hitbox.position.y});
            m_visualRect.setSize({hitbox.size.x, hitbox.size.y});
            target.draw(m_visualRect);
        }

        // 3번째 공격 선딜레이에는 약점 위치 표시
        if (m_isWeakPointActive) {
            m_weakPointVisual.setPosition({m_weakPointHitbox.position.x, m_weakPointHitbox.position.y});
            m_weakPointVisual.setSize({m_weakPointHitbox.size.x, m_weakPointHitbox.size.y});
            target.draw(m_weakPointVisual);
        }
    }
    // 공격 활성 구간이라면, '공격 범위'를 공격 이펙트 색으로 그림
    else if (m_phaseTimer < currentTimings.preDelay + currentTimings.activeDuration) {
        m_visualRect.setFillColor(sf::Color(255, 0, 0, 100)); // 빨간색 공격 컬러

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