#include "PlayerCommand.h"
#include "../entities/Player.h" // Player의 구체적인 함수를 호출해야 하므로 전체 헤더를 포함
#include "../state/IPlayerState.h"

/**
 * @brief 모든 PlayerCommand가 공유하는 핵심 로직입니다.
 * 커맨드 처리를 Player의 '현재 상태'에게 위임합니다.
 * @param player 커맨드를 실행할 대상 플레이어
 */
void PlayerCommand::execute(Player& player) {
    // 1. 플레이어의 현재 상태(예: IdleState)를 가져와 handleInput 함수를 호출합니다.
    //    이때 this는 이 execute 함수를 호출한 구체적인 커맨드(예: MoveCommand) 자신을 가리킵니다.
    auto newState = player.getCurrentState()->handleInput(player, this);

    // 2. 만약 handleInput이 새로운 상태를 반환했다면 (상태 전환이 필요하다면),
    if (newState) {
        // 플레이어의 상태를 새로운 상태로 변경합니다.
        player.changeState(std::move(newState));
    }
}