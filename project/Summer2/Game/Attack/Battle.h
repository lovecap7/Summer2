#pragma once
namespace Battle
{
	//攻撃の強さ
	enum class AttackPower : int
	{
		Low		= 0,
		Middle	= 1,
		High	= 2,
		Highest = 3,
	};
	//攻撃でひるまない強さ
	typedef AttackPower Armor;

	//攻撃とアーマーの比較
	bool CheckFlinch(AttackPower ap, Armor am);
}
