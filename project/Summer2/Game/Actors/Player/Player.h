#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
class Input;
class CameraBase;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input) override;
	void Draw()const override;
	void Complete() override;
private:
	//���f���n���h��
	int m_modelHandle;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Player::*)(const Input& input);
	UpdateFunc_t m_update;
	//�ҋ@���
	void IdleUpdate(const Input& input);
	//�ړ�
	void MoveUpdate(const Input& input);
	//�W�����v
	void JumpUpdate(const Input& input);
};

