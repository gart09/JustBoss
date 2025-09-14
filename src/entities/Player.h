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
    void turn(float direction);
    void takeJump(float direction);
    void takeDoubleJump(float horizontal_input);

    // 피격
    void takeDamage(int damage, sf::Vector2f attackerPosition);
    bool isInvincible() const { return m_invincibilityTimer > 0; }
    void resetColor() { m_shape.setFillColor(sf::Color::Blue); }
    void setColor(sf::Color color) { m_shape.setFillColor(color); }

    // 게터 함수들
    const std::vector<AttackData>& getAttackDataList() const { return m_attackDataList; }
    const std::vector<AttackData>& getComboData() const { return m_basicComboData; }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::Vector2f getCenter() const;
    FacingDirection getFacingDirection() const { return m_facingDirection; }
    float getDirection() const { return m_facingDirection == FacingDirection::Right ? 1.f : -1.f; }
    std::unique_ptr<IPlayerState>& getCurrentState() { return m_currentState; }
    float getSpeed() const { return m_speed; }
    float getSize() const { return m_size; }
    int getHP() const { return m_hp; }
    int getMaxHP() const { return m_maxHp; }
    void getDamage(int damage) { m_hp -= damage; }
    std::optional<sf::FloatRect> getActiveHitbox() const { return m_activeHitbox; };
    bool isOnGround() const { return m_canJump ;}
    bool canDoubleJump() const { return m_canDoubleJump; }

    // 히트박스 관련 함수
    void setActiveHitbox(const sf::FloatRect& hitbox);
    sf::FloatRect getHitbox() const { return m_shape.getGlobalBounds(); };
    void clearActiveHitbox();

    // 대쉬 관련 함수
    bool canDash() const;
    void startDashCooldown();  
    float getDashCooldownProgress() const;

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

    float m_speed = 300.f;
    float m_gravity = 1800.f;
    float m_jumpStrength = 600.f;
    int m_hp = 100;
    int m_maxHp = 100;
    float m_size = 50.f;

    // 점프 관련
    bool m_canJump;
    bool m_canDoubleJump;

    // 대쉬 관련
    static constexpr float DASH_DISTANCE = 250.f;
    static constexpr float DASH_COOLDOWN_TIME = 5.0f;
    float m_dashCooldown;

    // 넉백 관련
    static constexpr float KNOCKBACK_POWER_X = 250.f;
    static constexpr float KNOCKBACK_POWER_Y = 300.f;

    // 공격 관련
    std::vector<AttackData> m_basicComboData;
    sf::RectangleShape m_debugAttackBox;
    sf::RectangleShape m_chargeBarBackground;
    sf::RectangleShape m_chargeBarFill;
    bool m_hasDealtDamage = false;

    // 피격 관련
    float m_invincibilityTimer;
    float m_flashTimer = 0.f;
};

