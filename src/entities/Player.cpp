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
    
    // AttacData: [공격력, 선딜레이, 시각화 시간, 후딜레이, 히트박스]
    // 1. 일반 찌르기 데이터
    sf::FloatRect stabHitbox({50.f, 0.f}, {80.f, 50.f});
    m_attackDataList.push_back(AttackData{10, 0.1f, 0.15f, 0.05f, stabHitbox});
    // 2. 약점 공격 데이터
    sf::FloatRect weakPointHitbox({50.f, 0.f}, {50.f, 50.f});
    m_attackDataList.push_back(AttackData{15, 0.1f, 0.15f, 0.10f, weakPointHitbox});
    // 3. 차지 1단계 데이터
    sf::FloatRect charge1Hitbox({50.f, -10.f}, {70.f, 60.f});
    m_attackDataList.push_back(AttackData{20, 0.1f, 0.2f, 0.10f, charge1Hitbox});
    // 4. 차지 2단계 데이터
    sf::FloatRect charge2Hitbox({50.f, -30.f}, {150.f, 80.f});
    m_attackDataList.push_back(AttackData{35, 0.1f, 0.2f, 0.20f, charge2Hitbox});

    m_chargeBarBackground.setSize({50.f, 8.f});
    m_chargeBarBackground.setFillColor(sf::Color(0, 0, 0, 150));
    m_chargeBarBackground.setOutlineColor(sf::Color::White);
    m_chargeBarBackground.setOutlineThickness(1.f);
    
    m_chargeBarFill.setSize({0, 8.f}); // 처음 너비는 0
    m_chargeBarFill.setFillColor(sf::Color::White);

    changeState(std::make_unique<IdleState>());
}

void Player::update(sf::Time deltaTime, Boss& boss) {
    if (m_currentState) {
        auto newState = m_currentState->update(*this, deltaTime.asSeconds());
        if (newState) {
            changeState(std::move(newState));
            m_hasDealtDamage = false; // 새로운 상태로 전환되면 데미지 입힘 기록 초기화
        }
    }
    std::optional<AttackInfo> attackInfo = m_currentState->getActiveAttackInfo(*this);
    
    
    if (attackInfo.has_value()) {
        // --- intersects 함수를 대체하는 직접 충돌 검사 로직 ---
        sf::FloatRect playerHitbox = attackInfo->hitbox;
        sf::FloatRect bossHitbox = boss.getHitbox();

        // 두 사각형이 충돌했는지 직접 계산
        bool isColliding = 
            playerHitbox.position.x < bossHitbox.position.x + bossHitbox.size.x &&
            playerHitbox.position.x + playerHitbox.size.x > bossHitbox.position.x &&
            playerHitbox.position.y < bossHitbox.position.y + bossHitbox.size.y &&
            playerHitbox.position.y + playerHitbox.size.y > bossHitbox.position.y;

        if (isColliding && !m_hasDealtDamage) {
            // 충돌했다면 보스에게 데미지를 입히고,
            boss.takeDamage(attackInfo->damage);
            m_hasDealtDamage = true; // 현재 공격 상태에서 데미지를 입혔음을 기록
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
        m_debugAttackBox.setPosition(attackInfo->hitbox.position); // .position이 sf::Vector2f이므로 바로 전달
        m_debugAttackBox.setSize(attackInfo->hitbox.size);
        window.draw(m_debugAttackBox);
    }

    auto chargeProgressOpt = m_currentState->getChargeProgress();
    // 2. 차지 진행도 정보가 있을 경우에만 (즉, ChargingState일 때만)
    if (chargeProgressOpt.has_value()) {
        float progress = *chargeProgressOpt;
        const sf::Vector2f barSize = m_chargeBarBackground.getSize();
        
        // 차지 바 위치를 플레이어 머리 위로 설정
        sf::Vector2f barPosition = getPosition();
        //barPosition.x +=  // 플레이어 중앙에 맞춤
        barPosition.y -= 20.f; // 머리 위 20px
        
        m_chargeBarBackground.setPosition(barPosition);

        // 진행도에 따라 채워지는 바의 너비를 계산
        m_chargeBarFill.setSize({ barSize.x * progress, barSize.y });
        m_chargeBarFill.setPosition(barPosition);

        // 100% 충전 시 색을 초록색으로 변경
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
    // 0이 아닌 방향 값이 들어올 때만 방향을 갱신합니다.
    if (direction > 0) {
        m_facingDirection = FacingDirection::Right;
    }
    else if (direction < 0) {
        m_facingDirection = FacingDirection::Left;
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

void Player::setActiveHitbox(const sf::FloatRect& hitbox) { m_activeHitbox = hitbox; }
void Player::clearActiveHitbox() { m_activeHitbox.reset(); }

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