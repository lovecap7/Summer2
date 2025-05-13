#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class Input;
class CameraBase;
class Camera;
class Model;
class Slash;
class AttackManager;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
	void HitReaction() override;
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
	//���U���̃^������
	int m_chargeHighAttackFrame;
	//����
	std::shared_ptr<Collidable> m_rightSword;
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//���O�̏�Ԃ��o���Ă���
	//�ҋ@���
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//�ړ�
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//�W�����v
	void JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//������
	void FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//��U��
	void AttackNormal1Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void AttackNormal2Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void AttackNormal3Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//���U��
	void AttackCharge1Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void AttackCharge2Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//���
	void RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//��Ԃɍ��킹�ď��������ׂ����̂�����������
	void StateInit();
private:
	//�i�s������Ԃ��x�N�g��
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
	//�������Ă���
	void SpeedDown();
	//����̈ʒu�X�V
	void WeaponUpdate();
	//���ꔻ��̍X�V
	void HurtPointUpdate();
private:
	//�U��
	std::shared_ptr<Slash> m_attackN1;//�ʏ�1
	std::shared_ptr<Slash> m_attackN2;//�ʏ�2
	std::shared_ptr<Slash> m_attackN3;//�ʏ�3
	std::shared_ptr<Slash> m_attackC;//���ߍU��
};

