#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "../state/IPlayerState.h"
#include "../data/AttackData.h"


// 전방 선언
class IPlayerState;
class ICommand;
class Boss;
enum class FacingDirection { Left, Right };

class Player
{
public:
    Player();

    void update(sf::Time deltaTime, Boss& boss);
    void draw(sf::RenderWindow& window);
    void changeState(std::unique_ptr<IPlayerState> newState);

    // 상태 제어 함수
    void setVelocityX(float vx) { m_velocity.x = vx; };
    void setVelocityY(float vy) { m_velocity.y = vy; };

    //행동 함수
    void move(float direction);
    void takeJump();
    void takeDoubleJump(float horizontal_input);

    // 피격
    void takeDamage(int damage, sf::Vector2f damageSourcePosition);

    // 게터 함수들
    const std::vector<AttackData>& getAttackDataList() const { return m_attackDataList; }
    const std::vector<AttackData>& getComboData() const { return m_basicComboData; }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    FacingDirection getFacingDirection() const { return m_facingDirection; }
    std::unique_ptr<IPlayerState>& getCurrentState() { return m_currentState; }
    float getAirControlForcePtr() { return m_airControlForce; }
    float getSpeed() { return m_speed; }
    int getHP() const { return m_hp; }
    int getMaxHP() const { return m_hp; }
    void getDamage(int damage) { m_hp -= damage; }
    std::optional<sf::FloatRect> getActiveHitbox() const { return m_activeHitbox; };
    bool isOnGround() const { return m_canJump ;}
    bool canDoubleJump() const { return m_canDoubleJump; }

    // 히트박스 관련 함수
    void setActiveHitbox(const sf::FloatRect& hitbox);
    void clearActiveHitbox();

    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
private:
    void applyPhysics(float dt);
    void applyKnockback(sf::Vector2f knockbackVelocity);
    void handleDashCooldown(float dt);

    std::unique_ptr<IPlayerState> m_currentState;
    FacingDirection m_facingDirection;
    std::vector<AttackData> m_attackDataList;
    std::optional<sf::FloatRect> m_activeHitbox;

    // --- 플레이어 능력치 및 물리 상수 ---
    float m_speed = 300.f;      // 초당 이동 속도 (픽셀)
    float m_gravity = 1800.f;   // 중력 가속도
    float m_jumpStrength = 600.f; // 점프 시의 수직 속도
    float m_airControlForce = 0.1f;
    int m_hp = 100;
    int m_maxHp = 100;

    // 점프 관련
    bool m_canJump;
    bool m_canDoubleJump;

    // 대쉬 관련
    static constexpr float DASH_DISTANCE = 250.f;
    static constexpr float DASH_COOLDOWN_TIME = 3.0f;
    float m_dashCooldown;

    // 넉백 관련
    static constexpr float KNOCKBACK_POWER_X = 250.f;
    static constexpr float KNOCKBACK_POWER_Y = 300.f;

    // 공격 관련
    std::vector<AttackData> m_basicComboData;
    sf::RectangleShape m_debugAttackBox; // 공격 판정 시각화용
    sf::RectangleShape m_chargeBarBackground;
    sf::RectangleShape m_chargeBarFill;
};

