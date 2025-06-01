#include "PlayerStateHit.h"

PlayerStateHit::PlayerStateHit(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
}

void PlayerStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
}
