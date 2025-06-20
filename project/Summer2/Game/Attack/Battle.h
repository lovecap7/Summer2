#pragma once
namespace Battle
{
	//UŒ‚‚Ì‹­‚³
	enum class AttackPower : int
	{
		Low		= 0,
		Middle	= 1,
		High	= 2
	};
	//UŒ‚‚Å‚Ğ‚é‚Ü‚È‚¢‹­‚³
	typedef AttackPower Armor;

	//UŒ‚‚ÆƒA[ƒ}[‚Ì”äŠr
	bool CheckFlinch(AttackPower ap, Armor am);
}
