#include "Battle.h"

bool Battle::CheckFlinch(AttackPower ap, Armor am)
{
	//�U���̑傫�����A�[�}�[���傫���Ȃ�true
	return (ap > am);
}
