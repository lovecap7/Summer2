#include "Battle.h"

bool Battle::CheckFlinch(AttackPower ap, Armor am)
{
	//攻撃の大きさがアーマーより大きいならtrue
	return (ap > am);
}
