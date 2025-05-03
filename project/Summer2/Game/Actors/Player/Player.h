#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"

class Input;
class CameraBase;
class Camera;
class Model;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input,const std::unique_ptr<Camera>& camera) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
private:
	//���f��
	std::unique_ptr<Model> m_model;
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�n�ʂɕt���Ă��邩�ǂ���
	bool m_isGround;
	//�W�����v�̉�
	unsigned int m_jumpNum;
	int m_nextJumpFrame;//2��ڂ̃W�����v���s����܂ł̎���
	//���̍U���̓��͂����邩
	bool m_isNextAttackInput;
	//���U���̃^������
	int m_chargeHighAttackFrame;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//���O�̏�Ԃ��o���Ă���
	//�ҋ@���
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//�ړ�
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//�W�����v
	void JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//������
	void FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//��U��
	void AttackLight1Update(const Input& input, const std::unique_ptr<Camera>& camera);
	void AttackLight2Update(const Input& input, const std::unique_ptr<Camera>& camera);
	void AttackLight3Update(const Input& input, const std::unique_ptr<Camera>& camera);
	//���U��
	void AttackHigh1Update(const Input& input, const std::unique_ptr<Camera>& camera);
	void AttackHigh2Update(const Input& input, const std::unique_ptr<Camera>& camera);
	//���
	void RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//��Ԃɍ��킹�ď��������ׂ����̂�����������
	void StateInit();
private:
	//�i�s������Ԃ��x�N�g��
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
	//�������Ă���
	void SpeedDown();
};

