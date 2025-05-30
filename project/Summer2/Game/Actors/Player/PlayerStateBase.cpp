#include "PlayerStateBase.h"
#include "Player.h"

PlayerStateBase::PlayerStateBase(Player* player):
	m_player(player)
{
	
}

PlayerStateBase::~PlayerStateBase()
{
	delete(m_player);
	m_player = nullptr;
}
