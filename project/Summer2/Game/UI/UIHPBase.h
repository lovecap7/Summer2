#pragma once
#include "UIBase.h"
#include <memory>
class HurtPoint;
class UIHPBase abstract:
    public UIBase
{
public:
    UIHPBase(std::shared_ptr<HurtPoint> hurtPoint);
    ~UIHPBase();
protected:
    float m_maxHp;//最大体力
    float m_nowHp;//現在の体力
    std::shared_ptr<HurtPoint> m_ownerHP;
};

