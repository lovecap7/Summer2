#pragma once
namespace Battle
{
	//�U���̋���
	enum class AttackPower : int
	{
		Low		= 0,
		Middle	= 1,
		High	= 2
	};
	//�U���łЂ�܂Ȃ�����
	typedef AttackPower Armor;

	//�U���ƃA�[�}�[�̔�r
	bool CheckFlinch(AttackPower ap, Armor am);
}
