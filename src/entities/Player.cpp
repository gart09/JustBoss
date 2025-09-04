#include "Player.h"


constexpr float WINDOW_WIDTH = 1280.f;
constexpr float GROUND_Y = 720.f;
constexpr float CHAR_SIZE = 50.f;

Player::Player()
{
    m_shape.setSize({CHAR_SIZE, CHAR_SIZE});
    m_shape.setFillColor(sf::Color::Blue);
    
    m_shape.setPosition({200.f, GROUND_Y - CHAR_SIZE});

    m_canJump = true;
    m_canDoubleJump = false;
    m_facingDirection = FacingDirection::Right;
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
    // --- 수평 이동 로직 ---
    float horizontalInput = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  horizontalInput -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) horizontalInput += 1.f;

    // 키 입력이 있을 때만 바라보는 방향을 갱신
    if (horizontalInput > 0) m_facingDirection = FacingDirection::Right;
    if (horizontalInput < 0) m_facingDirection = FacingDirection::Left;

    // 1. 지상에 있을 때: 등속 운동
    if (m_canJump)
    {
        m_velocity.x = horizontalInput * m_speed;
    }
    // 2. 공중에 있을 때: 공중 제어 (Air Control)
    else
    {
        // 진행 방향과 반대 키를 누르면 속도 감소
        if (m_velocity.x * horizontalInput < 0)
        {
            m_velocity.x += horizontalInput * m_airControlForce;
        }
        std::cout << "m_velocity.x= " << m_velocity.x << std::endl;
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
    // 1. 지상 점프
    if (m_canJump)
    {
        m_velocity.y = -m_jumpStrength;
        m_canJump = false;
        m_canDoubleJump = true;
        std::cout << "Action: Jump!" << std::endl;
        return; // 첫 점프 후 함수 종료
    }
    // 2. 더블 점프
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

void Player::useSkill(const std::string& skillName)
{
    // TODO: 각 스킬에 맞는 이펙트 생성, 재사용 대기시간 등의 코드를 구현합니다.
    std::cout << "Action: Used Skill [" << skillName << "]!" << std::endl;
}

