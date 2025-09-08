#include "Player.h"
#include "Boss.h"
#include "../state/PlayerState.h"
#include "../commands/PlayerCommand.h"
#include "../commands/CommandType.h"
#include <iostream>
#include <cmath>

constexpr float WINDOW_WIDTH = 1280.f;
constexpr float GROUND_Y = 620.f;
constexpr float CHAR_SIZE = 50.f;

Player::Player()
    : m_hp(100), m_facingDirection(FacingDirection::Right),
      m_canJump(true), m_canDoubleJump(false), m_dashCooldown(0.f)
{
    m_shape.setSize({CHAR_SIZE, CHAR_SIZE});
    m_shape.setFillColor(sf::Color::Blue);
    
    m_shape.setPosition({200.f, GROUND_Y - CHAR_SIZE - 100.f});


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

void Player::dash(float direction) {
    if (m_dashCooldown > 0) {
        std::cout << "Dash is on cooldown!" << std::endl;
        return;
    }
    float dashDirection = direction;
    // 방향이 지정되지 않았으면(0.f), 현재 바라보는 방향으로 대시
    if (dashDirection == 0.f) {
        dashDirection = (m_facingDirection == FacingDirection::Right) ? 1.f : -1.f;
    }
    
    m_facingDirection = (dashDirection > 0) ? FacingDirection::Right : FacingDirection::Left;
    sf::Vector2f currentPos = m_shape.getPosition();
    m_shape.setPosition({currentPos.x + dashDirection * DASH_DISTANCE, currentPos.y});
    m_dashCooldown = DASH_COOLDOWN_TIME;
    std::cout << "Dash!" << std::endl;
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

// --- Private ---
void Player::applyPhysics(float dt) {
    m_velocity.y += m_gravity * dt;
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
        
        // 2. 수직 속도를 0으로 만들어 더 이상 떨어지지 않게 함
        m_velocity.y = 0;

        // 3. 점프 가능 상태로 변경
        m_canJump = true;
        
        // 더블 점프도 초기화해야 한다면 아래 코드 추가
        m_canDoubleJump = false; 
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