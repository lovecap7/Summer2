#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"

class Input;
class CameraBase;
class Camera;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input,const std::unique_ptr<Camera>& camera) override;
	void Draw()const override;
	void Complete() override;
private:
	//���f���n���h��
	int m_modelHandle;
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	//�ҋ@���
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//�ړ�
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//�W�����v
	void JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
private:
	//�i�s������Ԃ��x�N�g��
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
};

