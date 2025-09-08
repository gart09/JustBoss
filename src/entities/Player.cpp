#include "Player.h"
#include "Boss.h"
#include "../state/PlayerState.h"
#include "../commands/PlayerCommand.h"
#include "../commands/CommandType.h"
#include <iostream>
#include <cmath>

constexpr float WINDOW_WIDTH = 1280.f;
constexpr float GROUND_Y = 720.f;
constexpr float CHAR_SIZE = 50.f;

Player::Player()
    : m_hp(100), m_facingDirection(FacingDirection::Right),
      m_canJump(true), m_canDoubleJump(false), m_dashCooldown(0.f)
{
    m_shape.setSize({CHAR_SIZE, CHAR_SIZE});
    m_shape.setFillColor(sf::Color::Blue);
    
    m_shape.setPosition({200.f, GROUND_Y - CHAR_SIZE - 200.f});


    m_debugAttackBox.setFillColor(sf::Color(255, 0, 0, 100));
    
    changeState(std::make_unique<IdleState>());
}

void Player::update(sf::Time deltaTime) {
    if (m_currentState) {
        auto newState = m_currentState->update(*this, deltaTime.asSeconds());
        if (newState) {
            changeState(std::move(newState));
        }
    }
    applyPhysics(deltaTime.asSeconds());
    handleDashCooldown(deltaTime.asSeconds());

    if (m_activeAttack.has_value()) {
        m_debugAttackBox.setPosition(sf::Vector2f(m_activeAttack->worldHitbox.position));

        m_debugAttackBox.setSize(sf::Vector2f(m_activeAttack->worldHitbox.size));
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    if (m_activeAttack.has_value()) {
        window.draw(m_debugAttackBox);
    }
}

void Player::changeState(std::unique_ptr<IPlayerState> newState) {
    m_currentState = std::move(newState);
    m_currentState->enter(*this);
}

void Player::move(float direction) {
    m_velocity.x = direction * m_speed;
    if (direction > 0) m_facingDirection = FacingDirection::Right;
    if (direction < 0) m_facingDirection = FacingDirection::Left;
}

void Player::attack()
{
    // TODO: 여기에 실제 공격 애니메이션, 충돌 판정 등의 코드를 구현합니다.
    std::cout << "Action: Basic Attack!" << std::endl;
}

void Player::jump()
{
    if (m_currentState) {
        auto newState = m_currentState->handleInput(*this, CommandType::Jump); 
        if (newState) changeState(std::move(newState));
    }
}

void Player::takeJump()
{
    if (m_canJump)
    {
        m_velocity.y = -m_jumpStrength;
        m_canJump = false;
        m_canDoubleJump = true;
        std::cout << "Action: Jump!" << std::endl;
        return; // 첫 점프 후 함수 종료
    }
}

void Player::takeDoubleJump()
{
    if (m_canDoubleJump)
    {
        const float doubleJumpHorizontalForce = 500.f;
        m_velocity.y = -m_jumpStrength * 0.8f;

        // 더블점프 방향 결정 로직
        bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
        bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);

        FacingDirection desiredDirection = m_facingDirection;

        // 진행방향과 반대 키를 눌렀을 경우, 방향 전환
        if ((m_facingDirection == FacingDirection::Right && leftPressed) ||
            (m_facingDirection == FacingDirection::Left && rightPressed))
        {
            desiredDirection = (leftPressed) ? FacingDirection::Left : FacingDirection::Right;
            m_facingDirection = desiredDirection;
        }
        
        // 결정된 방향으로 더블점프 실행
        if (desiredDirection == FacingDirection::Right)
        {
            m_velocity.x = doubleJumpHorizontalForce;
        }
        else
        {
            m_velocity.x = -doubleJumpHorizontalForce;
        }

        m_canDoubleJump = false;
        std::cout << "Action: Double Jump! Direction: " 
                  << (desiredDirection == FacingDirection::Right ? "Right" : "Left") << std::endl;
    }
}

void Player::dash() {
    if (m_dashCooldown > 0) {
        std::cout << "Dash is on cooldown!" << std::endl;
        return;
    }
    
    float direction = 0.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction = -1.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction = 1.f;

    if (direction == 0.f) { return; }
    
    m_facingDirection = (direction > 0) ? FacingDirection::Right : FacingDirection::Left;
    sf::Vector2f currentPos = m_shape.getPosition();
    m_shape.setPosition({currentPos.x + direction * DASH_DISTANCE, currentPos.y});
    m_dashCooldown = DASH_COOLDOWN_TIME;
    std::cout << "Dash!" << std::endl;
}

void Player::parryAction() {} // 실제 로직은 상태에서 처리

void Player::weakPointAttack() {
    // 약점 공격 전용 스킬이 있다면 여기에 구현. 현재는 일반 공격으로 처리.
}

void Player::takeDamage(int damage, sf::Vector2f damageSourcePosition) {
    if (dynamic_cast<HitStunState*>(m_currentState.get())) return; // 경직 중 무적
    if (m_hp <= 0) return;

    m_hp -= damage;
    std::cout << "Player hit! HP: " << m_hp << std::endl;
    
    sf::Vector2f playerCenter = getPosition();
    sf::Vector2f direction = playerCenter - damageSourcePosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) direction /= length; else direction = {1.f, 0.f};

    applyKnockback({direction.x * KNOCKBACK_POWER_X, -KNOCKBACK_POWER_Y});
    changeState(std::make_unique<HitStunState>(0.5f));
}

void Player::parry(const BossAttackData& attackData, sf::Vector2f sourcePosition) {
    std::cout << "Parry success!" << std::endl;

    sf::Vector2f playerCenter = getPosition();
    sf::Vector2f direction = playerCenter - sourcePosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) direction /= length; else direction = {1.f, 0.f};
    
    applyKnockback({direction.x * KNOCKBACK_POWER_X * 0.5f, -KNOCKBACK_POWER_Y * 0.5f});
    changeState(std::make_unique<ParrySuccessState>(0.3f));

    if (attackData.isReflectable) {
        createReflectedProjectile(sourcePosition);
    }
}

bool Player::isParrying() const {
    return dynamic_cast<const ParryingState*>(m_currentState.get()) != nullptr;
}

void Player::createReflectedProjectile(sf::Vector2f targetPosition) {
    std::cout << "Reflected projectile created towards boss!" << std::endl;
}

// --- Private ---
void Player::applyPhysics(float dt) {
    m_velocity.y += m_gravity * dt;
    m_shape.move(m_velocity * dt);
    
    // 바닥 충돌 처리
    if (m_shape.getPosition().y >= 720.f - m_shape.getSize().y / 2.f) {
        m_shape.setPosition({m_shape.getPosition().x, 720.f - m_shape.getSize().y / 2.f});
        m_velocity.y = 0;
        m_canJump = true;
    }
}

void Player::applyKnockback(sf::Vector2f knockbackVelocity) {
    m_velocity = knockbackVelocity;
    m_canJump = false;
    m_canDoubleJump = false;
}

void Player::handleDashCooldown(float dt) {
    if (m_dashCooldown > 0) {
        m_dashCooldown -= dt;
    }
}