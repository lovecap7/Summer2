#pragma once
namespace Battle
{
	//攻撃の重さ
	enum class AttackWeight : int
	{
		Light		= 0,
		Middle		= 1,
		Heavy		= 2,
		Heaviest	= 3,
	};
	//攻撃でひるまない強さ
	typedef AttackWeight Armor;

	//攻撃とアーマーの比較
	bool CheckFlinchAttackAndArmor(AttackWeight aw, Armor am);
}
