#pragma once
#include "EnemyBase.h"
class Input;
class Camera;
class Common1 :
    public EnemyBase
{
public:
    Common1(int modelHandle,Vector3 pos);
    ~Common1();
	void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
	void HitSearch(const Vector3& playerPos)override;
private:
	//ó‘Ô‘JˆÚ
	using UpdateFunc_t = void(Common1::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//’¼‘O‚Ìó‘Ô‚ğŠo‚¦‚Ä‚¨‚­
	//‘Ò‹@ó‘Ô
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//ˆÚ“®
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//UŒ‚
	void AttackUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//‹ò‚ç‚¢
	void HitUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//ó‘Ô‚É‡‚í‚¹‚Ä‰Šú‰»‚·‚×‚«‚à‚Ì‚ğ‰Šú‰»‚·‚é
	void StateInit();
	//Œ¸‘¬
	void SpeedDown();
};

