#include "Player.h"


constexpr float WINDOW_WIDTH = 1280.f;
constexpr float GROUND_Y = 720.f;
constexpr float CHAR_SIZE = 50.f;

Player::Player()
{
    m_shape.setSize({CHAR_SIZE, CHAR_SIZE});
    m_shape.setFillColor(sf::Color::Blue);
    
    m_shape.setPosition({200.f, GROUND_Y - CHAR_SIZE});

    m_canJump = true; // 처음에는 땅에 있으므로 점프 가능
}

void Player::handleInput(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        // keyPressed->code 로 키 코드를 확인합니다.
        if (keyPressed->code == sf::Keyboard::Key::D)
        {
            jump();
        }
        else if (keyPressed->code == sf::Keyboard::Key::A)
        {
            attack();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Q)
        {
            useSkill("Q");
        }
        else if (keyPressed->code == sf::Keyboard::Key::W)
        {
            useSkill("W");
        }
        else if (keyPressed->code == sf::Keyboard::Key::E)
        {
            useSkill("E");
        }
        else if (keyPressed->code == sf::Keyboard::Key::R)
        {
            useSkill("R");
        }
    }
}

void Player::update(sf::Time deltaTime)
{
    m_velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_velocity.x -= m_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_velocity.x += m_speed;
    }

    // 중력 적용
    m_velocity.y += m_gravity * deltaTime.asSeconds();
    m_shape.move(m_velocity * deltaTime.asSeconds());

    // 1. 왼쪽 경계 확인
    if (m_shape.getPosition().x < 0.f)
    {
        m_shape.setPosition({0.f, m_shape.getPosition().y});
    }
    // 2. 오른쪽 경계 확인 (캐릭터의 오른쪽 끝 = x위치 + 캐릭터 너비)
    else if (m_shape.getPosition().x + CHAR_SIZE > WINDOW_WIDTH)
    {
        m_shape.setPosition({WINDOW_WIDTH - CHAR_SIZE, m_shape.getPosition().y});
    }

    // 3. 바닥 경계 확인
    if (m_shape.getPosition().y > 720.f - CHAR_SIZE)
    {
        m_shape.setPosition({m_shape.getPosition().x, 720.f - CHAR_SIZE});
        m_velocity.y = 0;
        m_canJump = true;
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}


// --- Private Member Functions ---

void Player::attack()
{
    // TODO: 여기에 실제 공격 애니메이션, 충돌 판정 등의 코드를 구현합니다.
    std::cout << "Action: Basic Attack!" << std::endl;
}

void Player::jump()
{
    // 점프 가능한 상태일 때만 점프합니다.
    if (m_canJump)
    {
        m_velocity.y = -m_jumpStrength; // y속도에 음수 값을 주어 위로 튀어 오르게 함
        m_canJump = false; // 점프를 했으므로 땅에 닿기 전까지 다시 점프 불가능
        std::cout << "Action: Jump!" << std::endl;
    }
}

void Player::useSkill(const std::string& skillName)
{
    // TODO: 각 스킬에 맞는 이펙트 생성, 재사용 대기시간 등의 코드를 구현합니다.
    std::cout << "Action: Used Skill [" << skillName << "]!" << std::endl;
}

