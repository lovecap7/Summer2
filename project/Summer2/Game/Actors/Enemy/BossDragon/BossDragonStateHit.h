#pragma once
#include "BossDragonStateBase.h"
class BossDragon;
class Input;
class Camera;
class AttackManager;
class BossDragonStateHit :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateHit>
{
public:
    BossDragonStateHit(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    void SpeedDown();
};

