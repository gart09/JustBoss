#pragma once
#include "../state/BossPhase1State.h"
#include "../bossPattern/phase1/Rush.h"
#include "../bossPattern/phase1/FrontSlam.h"
#include "../bossPattern/phase1/GroundSlam.h"
#include "../bossPattern/phase1/MagneticField.h"
#include "../bossPattern/phase1/TripleSlam.h"
#include <random>

float randomf(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(gen);
}

BossPhase1State::BossPhase1State() {
    m_currentPattern = nullptr;
    m_thinkTimer = 3.f;
    m_decisionTimer = 0.f;
    m_generalPatterns.push_back(std::make_unique<FrontSlam>());
    m_generalPatterns.push_back(std::make_unique<GroundSlam>());
    m_generalPatterns.push_back(std::make_unique<MagneticField>());
    
    m_specialPatterns.push_back(std::make_unique<Rush>());
    m_specialPatterns.push_back(std::make_unique<TripleSlam>());
}

void BossPhase1State::enter(Boss& boss)
{
    std::cout << "Boss enters Phase 1: Wandering peacefully..." << std::endl;
}

void BossPhase1State::draw(sf::RenderTarget& target, Boss& boss) {
    if (m_currentPattern) {
        m_currentPattern->draw(target);
    }
}

void BossPhase1State::update(Boss& boss, float dt, Player& player) {
    for (const auto& pattern : m_generalPatterns) {
        pattern->updateCooldown(dt);
    }
    for (const auto& pattern : m_specialPatterns) {
        pattern->updateCooldown(dt);
    }
    if (m_currentPattern) {
        m_currentPattern->update(dt, boss, player);
        if (m_currentPattern->isFinished()) {
            m_currentPattern = nullptr;
            m_thinkTimer = m_thinkDelay;
        }
        return;
    }
    m_thinkTimer -= dt;
    if (m_thinkTimer > 0) {
        m_decisionTimer -= dt;

        if (m_decisionTimer <= 0.f) {
            m_decisionTimer = DECISION_INTERVAL;
            float distance = boss.getCenter().x - player.getCenter().x;
            float optimalDistance = 100.0f;
            float moveSpeed = 100.0f;

            if (abs(distance) > optimalDistance) {
                boss.setVelocity({(distance > 0 ? -1.f : 1.f) * moveSpeed, 0.f});
            } else {
                boss.wander(dt);
            }
        return;
        }
    }
    else{
        m_currentPattern = choosePattern(boss, player);
        if (m_currentPattern) {
            m_currentPattern->execute(boss, player);
        } else {
            m_thinkTimer = randomf(2.0f, 5.0f); 
        }
    }
}

void BossPhase1State::exit(Boss& boss)
{
    std::cout << "Boss exits Phase 1." << std::endl;
}

IPattern* BossPhase1State::choosePattern(Boss& boss, Player& player) {
    std::vector<IPattern*> availableSpecialsPatterns;
    for(auto& pattern : m_specialPatterns) {
        if (pattern->canExecute(boss, player)) {
            availableSpecialsPatterns.push_back(pattern.get());
        }
    }
    if(!availableSpecialsPatterns.empty()) {
        int randomIndex = rand() % availableSpecialsPatterns.size();
        return availableSpecialsPatterns[randomIndex];
    }

    std::vector<IPattern*> availableGeneralPatterns;
    for (const auto& pattern : m_generalPatterns) {
        if (pattern->canExecute(boss, player)) {
            availableGeneralPatterns.push_back(pattern.get());
        }
    }

    if (!availableGeneralPatterns.empty()) {
        int randomIndex = rand() % availableGeneralPatterns.size();
        std::cout << "Boss chooses a general pattern to execute:" << randomIndex << std::endl;
        return availableGeneralPatterns[randomIndex];
    }
    
    return nullptr;
}
