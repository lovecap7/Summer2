#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class Model;
class UIManager;
class Sky :
    public Actor
{
public:
	Sky(int handle);
	~Sky();
    //“o˜^ˆ—
    void Entry(std::shared_ptr<ActorManager> actorManager, std::shared_ptr<UIManager> uiManager) override {};
    //“o˜^‰ğœ
    void Exit(std::shared_ptr<ActorManager> actorManager, std::shared_ptr<UIManager> uiManager) override {};
    void Init()override {};
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<AttackManager> attackManager, std::shared_ptr<UIManager> uiManager) override;
    void Gravity(const Vector3& gravity)override {};
    void OnHitColl(const std::shared_ptr<Collidable>& other)override {};
    void Draw()const override;
    void Complete() override {};
private:
	//ƒ‚ƒfƒ‹
	std::shared_ptr<Model> m_model;
};

