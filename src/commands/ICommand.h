#pragma once

// Player 클래스의 전체 정의를 포함하지 않고 이름만 알려주어 컴파일 의존성을 낮춥니다.
class Player;

/**
 * @brief 모든 커맨드 객체가 상속받는 최상위 인터페이스입니다.
 * execute 함수를 통해 구체적인 행동을 실행하도록 약속합니다.
 */
class ICommand {
public:
    virtual ~ICommand() = default;
    
    // 이 커맨드를 Player에 대해 실행합니다.
    virtual void execute(Player& player) = 0;
};