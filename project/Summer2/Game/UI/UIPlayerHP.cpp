#include "UIPlayerHP.h"
#include "../Attack/HurtPoint.h"
#include <DxLib.h>

UIPlayerHP::UIPlayerHP(std::shared_ptr<HurtPoint> hurtPoint):
	UIHPBase(hurtPoint)
{
}

UIPlayerHP::~UIPlayerHP()
{
}

void UIPlayerHP::Update()
{
	m_nowHp = m_ownerHP->GetHp();
	if (m_nowHp < 0.0f)
	{
		m_nowHp = 0.0f;
	}
}

void UIPlayerHP::Draw() const
{
	DrawBox(50, 50, (m_maxHp / m_maxHp) * 50, 150, 0x00ff55, true);
	DrawBox(50, 50, (m_nowHp / m_maxHp) * 50, 150, 0x00ff55, true);
}
