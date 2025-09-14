#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../enum/BossPhaseEnum.h"

class IBossPhaseState; // 전방 선언
class IPattern;
class Player;

class Boss
{
public:
    Boss();
    ~Boss();
    void update(sf::Time deltaTime, Player& player);
    void draw(sf::RenderWindow& window);
    int getHP() { return m_hp; }
    int getMaxHP() { return m_maxHp; }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    float getSize() const { return m_size; }
    IPattern* getCurrentPattern() const;
    sf::Vector2f getCenter() const;
    PhaseID getCurrentPhase() const;
    sf::FloatRect getHitbox() const;

    void takeDamage(int damage);
    void changeState(std::unique_ptr<IBossPhaseState> newState);
    void wander(float dt);
    void setVelocity(const sf::Vector2f& velocity);
    void resetVelocity() { m_speed = 100.f; }

    void enterGroggyState (PhaseID phaseId);
    void setColor(sf::Color color) { m_shape.setFillColor(color); };
    void resetColor() { m_shape.setFillColor(sf::Color::Red); };

    float Boss::randomFloat(float min, float max);
private:
    void applyPhysics(float dt); 

    sf::RectangleShape m_shape; // 임시로 사용할 사각형 모양
    sf::Vector2f m_velocity;
    float m_size = 200.f;
    float m_speed = 100.f;
    int m_hp = 500;
    int m_maxHp = 500;
    float wanderDirection_ = 1.0f; // 1.0f: 오른쪽, -1.0f: 왼쪽
    float wanderTimer_ = 0.0f; // 방향 전환 타이머
    std::unique_ptr<IBossPhaseState> m_currentState;

    sf::Color m_originalColor;     // 원래 색상
    float m_hitEffectTimer;        // 하얗게 점멸하는 효과가 지속되는 시간

};

