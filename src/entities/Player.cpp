#include "Boss.h"
#include "../state/PlayerState.h"
#include "../commands/PlayerCommand.h"
#include "../bossPattern/IPattern.h"
#include <iostream>
#include <cmath>

constexpr float WINDOW_WIDTH = 1280.f;
constexpr float GROUND_Y = 620.f;

Player::Player()
    : m_hp(100), m_facingDirection(FacingDirection::Right),
      m_canJump(true), m_canDoubleJump(false), m_dashCooldown(0.f),
      m_invincibilityTimer(0.f), m_flashTimer(0.f), m_speed(300.f), m_jumpStrength(600.f)
{
    m_shape.setSize({m_size, m_size});
    m_shape.setFillColor(sf::Color::Blue);
    
    //m_shape.setPosition({200.f, GROUND_Y - getSize()});
    m_shape.setPosition({200.f, GROUND_Y - 50.f});


    m_debugAttackBox.setFillColor(sf::Color(50, 255, 50, 150));
    
    // AttacData: [공격력, 선딜레이, 시각화 시간, 후딜레이, 히트박스]
    // 1. 일반 찌르기 데이터
    sf::FloatRect stabHitbox({50.f, 0.f}, {80.f, 50.f});
    m_attackDataList.push_back(AttackData{10, 0.2f, 0.15f, 0.2f, stabHitbox});
    // 2. 약점 공격 데이터
    sf::FloatRect weakPointHitbox({50.f, 0.f}, {50.f, 50.f});
    m_attackDataList.push_back(AttackData{5, 0.1f, 0.15f, 0.10f, weakPointHitbox});
    // 3. 차지 1단계 데이터
    sf::FloatRect charge1Hitbox({50.f, -10.f}, {70.f, 60.f});
    m_attackDataList.push_back(AttackData{10, 0.2f, 0.2f, 0.20f, charge1Hitbox});
    // 4. 차지 2단계 데이터
    sf::FloatRect charge2Hitbox({50.f, -30.f}, {150.f, 80.f});
    m_attackDataList.push_back(AttackData{50, 0.1f, 0.2f, 0.20f, charge2Hitbox});

    m_chargeBarBackground.setSize({50.f, 8.f});
    m_chargeBarBackground.setFillColor(sf::Color(0, 0, 0, 150));
    m_chargeBarBackground.setOutlineColor(sf::Color::White);
    m_chargeBarBackground.setOutlineThickness(1.f);
    
    m_chargeBarFill.setSize({0, 8.f}); // 처음 너비는 0
    m_chargeBarFill.setFillColor(sf::Color::White);

    changeState(std::make_unique<IdleState>());
}

void Player::update(sf::Time deltaTime, Boss& boss) {
    if (m_invincibilityTimer > 0.f) {
        m_invincibilityTimer -= deltaTime.asSeconds();
    }

    float dt = deltaTime.asSeconds();

    if (m_invincibilityTimer > 0.f) {
        m_invincibilityTimer -= dt;

        m_flashTimer += dt;
        float blinkInterval = 0.1f;
        bool showWhite = std::fmod(m_flashTimer, blinkInterval * 2) < blinkInterval;

        if (showWhite) {
            setColor(sf::Color::White);
        } else {
            resetColor();
        }

    } else {
        resetColor();
    }

    if (m_currentState) {
        auto newState = m_currentState->update(*this, deltaTime.asSeconds());
        if (newState) {
            changeState(std::move(newState));
            m_hasDealtDamage = false;
        }
    }

    std::optional<AttackInfo> attackInfo = m_currentState->getActiveAttackInfo(*this);
    if (attackInfo.has_value()) {
        sf::FloatRect playerHitbox = attackInfo->hitbox;
        sf::FloatRect bossHitbox = boss.getHitbox();
        if (dynamic_cast<WeakAttackState*>(m_currentState.get())) {
            IPattern* currentBossPattern = boss.getCurrentPattern();
            if(currentBossPattern)
                if (auto bossWeakPoint = boss.getCurrentPattern()->getWeakPointHitbox()) {
                    if (playerHitbox.findIntersection(*bossWeakPoint) && !m_hasDealtDamage) {
                        boss.takeDamage(attackInfo->damage);
                        boss.enterGroggyState(boss.getCurrentPhase());
                        m_hasDealtDamage = true;
                    }
                }
        }
        if (playerHitbox.findIntersection(bossHitbox) && !m_hasDealtDamage) {
            boss.takeDamage(attackInfo->damage);
            m_hasDealtDamage = true;
        }
    }

    applyPhysics(deltaTime.asSeconds());
    handleDashCooldown(deltaTime.asSeconds());
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    
    auto attackInfo = m_currentState->getActiveAttackInfo(*this);
    if (attackInfo.has_value()) {
        m_debugAttackBox.setPosition(attackInfo->hitbox.position);
        m_debugAttackBox.setSize(attackInfo->hitbox.size);
        window.draw(m_debugAttackBox);
    }

    auto chargeProgressOpt = m_currentState->getChargeProgress();
    if (chargeProgressOpt.has_value()) {
        float progress = *chargeProgressOpt;
        const sf::Vector2f barSize = m_chargeBarBackground.getSize();
        
        sf::Vector2f barPosition = getPosition();
        barPosition.y -= 20.f;
        
        m_chargeBarBackground.setPosition(barPosition);

        m_chargeBarFill.setSize({ barSize.x * progress, barSize.y });
        m_chargeBarFill.setPosition(barPosition);

        if (progress >= 1.0f) {
            m_chargeBarFill.setFillColor(sf::Color::Green);
        } else {
            m_chargeBarFill.setFillColor(sf::Color::White);
        }
        window.draw(m_chargeBarBackground);
        window.draw(m_chargeBarFill);
    }
}

void Player::changeState(std::unique_ptr<IPlayerState> newState) {
    if (m_currentState) {
        m_currentState->exit(*this);
    }
    m_currentState = std::move(newState);
    m_currentState->enter(*this);
}

void Player::move(float direction) {
    m_velocity.x = direction * m_speed;
    if (direction > 0) m_facingDirection = FacingDirection::Right;
    if (direction < 0) m_facingDirection = FacingDirection::Left;
}

void Player::turn(float direction)
{
    if (direction > 0) {
        m_facingDirection = FacingDirection::Right;
    }
    else if (direction < 0) {
        m_facingDirection = FacingDirection::Left;
    }
}

void Player::takeJump(float direction)
{
    if (m_canJump)
    {
        m_velocity.x = direction * m_speed;
        m_velocity.y = -m_jumpStrength;
        m_canJump = false;
        m_canDoubleJump = true;
        std::cout << "Action: Jump!" << std::endl;
        return;
    }
}

void Player::takeDoubleJump(float m_direction)
{
    if (m_canDoubleJump)
    {
        m_velocity.y = -m_jumpStrength * 0.8f;
        
        float move_direction = m_direction;
        
        if (move_direction == 0.f) {
            move_direction = (m_facingDirection == FacingDirection::Right) ? 1.f : -1.f;
        }

        // 최종 결정된 방향으로 속도를 설정
        m_velocity.x = move_direction * 500.f;

        // 바라보는 방향도 갱신
        m_facingDirection = (move_direction > 0) ? FacingDirection::Right : FacingDirection::Left;

        m_canDoubleJump = false;
        std::cout << "Action: Double Jump!" << std::endl;
    }
}

void Player::takeDamage(int damage, sf::Vector2f damageSourcePosition) {
    if (isInvincible()) return; 
    if (m_hp <= 0) return;

    if (dynamic_cast<ChargingState*>(m_currentState.get())) {
        damage *= 2;
        std::cout << "while Charging attacked!!  Damage 2 times! " << damage << std::endl;
    }

    m_hp -= damage;
    m_invincibilityTimer = 1.0f; // 무적 시간 부여

    std::cout << "Player hit! HP: " << m_hp << std::endl;

    m_invincibilityTimer = 1.0f; // 무적 시간 부여
    m_flashTimer = 0.f;          // <-- 피격 시 점멸 타이머를 리셋
    
    sf::Vector2f playerCenter = getPosition();
    sf::Vector2f direction = playerCenter - damageSourcePosition;
    if (direction.x >= 0) {
        direction.x = 1.f; // 오른쪽에서 공격받음
    } else {
        direction.x = -1.f; // 왼쪽에서 공격받음
    }

    if (m_currentState && m_currentState->isInterruptible()) {
        std::cout << "Action Interrupted!" << std::endl;
        applyKnockback({direction.x * KNOCKBACK_POWER_X, -KNOCKBACK_POWER_Y});
        changeState(std::make_unique<HitStunState>());
    }
    else {
        std::cout << "Super Armor! Action Continues." << std::endl;
        applyKnockback({direction.x * KNOCKBACK_POWER_X, -KNOCKBACK_POWER_Y});
    }
}

void Player::setActiveHitbox(const sf::FloatRect& hitbox) { m_activeHitbox = hitbox; }
void Player::clearActiveHitbox() { m_activeHitbox.reset(); }

// --- Private ---
void Player::applyPhysics(float dt) {
    if (m_currentState && !m_currentState->ignoresGravity()) {
        m_velocity.y += m_gravity * dt;
    }
    m_shape.move(m_velocity * dt);
    

    const sf::Vector2f pos = m_shape.getPosition();
    const sf::Vector2f size = m_shape.getSize();

    // 왼쪽 벽 충돌 확인
    if (pos.x < 0.f) {
        // 위치를 벽에 강제로 고정
        m_shape.setPosition({0.f, pos.y});
        // 벽에 닿았으니 수평 속도를 0으로 만들어 '달라붙는' 현상 방지
        m_velocity.x = 0; 
    }
    // 오른쪽 벽 충돌 확인
    else if (pos.x + size.x > WINDOW_WIDTH) {
        // 위치를 벽에 강제로 고정 (캐릭터 너비만큼 빼줌)
        m_shape.setPosition({WINDOW_WIDTH - size.x, pos.y});
        // 벽에 닿았으니 수평 속도를 0으로 만듦
        m_velocity.x = 0;
    }

    // 플레이어의 바닥 위치 계산 (도형의 y좌표 + 도형의 세로 길이)
    float playerBottom = m_shape.getPosition().y + m_shape.getSize().y;

    // 바닥 충돌 처리
    // 플레이어의 발(playerBottom)이 바닥 라인(GROUND_Y)보다 아래로 내려갔는지 확인
    if (playerBottom > GROUND_Y) {
        // 1. 플레이어의 위치를 바닥에 정확히 맞춤
        // (플레이어의 y좌표 = 바닥 라인 - 플레이어의 높이)
        m_shape.setPosition({m_shape.getPosition().x, GROUND_Y - m_shape.getSize().y});
        if (m_currentState && m_currentState->stopHorizontalOnLand()) {
            m_velocity.x = 0;
        }
        m_velocity.y = 0;

        // 3. 점프 가능 상태로 변경
        m_canJump = true;
        m_canDoubleJump = false; 
    }
}

void Player::applyKnockback(sf::Vector2f knockbackVelocity) {
    m_velocity = knockbackVelocity;
    m_canJump = false;
    m_canDoubleJump = false;
    std::cout << m_velocity.x << ", " << m_velocity.y << std::endl;
}

void Player::handleDashCooldown(float dt) {
    if (m_dashCooldown > 0) {
        m_dashCooldown -= dt;
    }
}

bool Player::canDash() const {
    return m_dashCooldown <= 0;
}

void Player::startDashCooldown() {
    m_dashCooldown = DASH_COOLDOWN_TIME;
}

float Player::getDashCooldownProgress() const {
    float progress = 1.f - (m_dashCooldown / DASH_COOLDOWN_TIME);
    return std::clamp(progress, 0.f, 1.f);
}

sf::Vector2f Player::getCenter() const {
    return {m_shape.getPosition().x + m_size / 2, m_shape.getPosition().y + m_size / 2};
}