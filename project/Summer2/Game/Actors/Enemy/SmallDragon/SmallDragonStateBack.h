#pragma once
#include "SmallDragonStateBase.h"
class SmallDragonStateBack :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateBack>
{
public:
	SmallDragonStateBack(std::shared_ptr<SmallDragon> owner);
	~SmallDragonStateBack();
	void Init() override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:

};


