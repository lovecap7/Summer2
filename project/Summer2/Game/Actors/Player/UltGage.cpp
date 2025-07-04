#include "UltGage.h"
#include "../../../General/Math/MathSub.h"

UltGage::UltGage(int max):
	m_ultGage(0),
	m_maxUltGage(max),
	m_pendingUltGauge(0)
{
}

UltGage::~UltGage()
{
}

void UltGage::AddUltGage(int add)
{
	//最大なら
	if (IsMaxUlt())return;
	//加算
	m_ultGage += add;
	//クランプ
	m_ultGage = MathSub::ClampInt(m_ultGage, 0, m_maxUltGage);
}
