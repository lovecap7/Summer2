#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class BossDragon;
class BossDragonStateChase :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateChase>
{
public:
    BossDragonStateChase(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateChase();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:

};
