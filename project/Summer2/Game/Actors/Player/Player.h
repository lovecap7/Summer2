#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class Input;
class CameraBase;
class Camera;
class Model;
class AttackBase;
class MeleeAttack;
class AttackManager;
class PlayerStateBase;
class Player :
	public Actor
{
	//�v���C���[���������悤�ɂ���
	friend class PlayerStateBase;
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
	//����
	std::shared_ptr<Collidable> m_rightSword;
	//����
	std::shared_ptr<Collidable> m_leftLeg;
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�^������
	int m_chargeFrame;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//���O�̏�Ԃ��o���Ă���
	//�ҋ@���
	void UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//�ړ�
	void UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//�W�����v
	void UpdateJump(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//������
	void UpdateFall(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//��U��
	void UpdateAttackNormal1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void UpdateAttackNormal2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void UpdateAttackNormal3(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//���U��
	void UpdateAttackCharge1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void UpdateAttackCharge2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//���
	void UpdateRolling(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
private:
	//��Ԃɍ��킹�ď��������ׂ����̂�����������
	void InitState();
	//�i�s������Ԃ��x�N�g��
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
	//�������Ă���
	void SpeedDown();
	//���͂̑傫���ɍ��킹�đ��x��Ԃ�
	float InputValueSpeed(const Input& input);
	//�퓬�Ɋւ���X�V����
	void BattleUpdate();
	//���̓����蔻��쐬
	void CreateRightSword();
	//���̈ʒu�X�V
	void UpdateRightSword();
	//�����̓����蔻��쐬
	void CreateLeftLeg();
	//�����̈ʒu�X�V
	void UpdateLeftLeg();
	//�U���̓����蔻��쐬
	void CreateAttack();
	//�U��������o��
	void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
	//���ꔻ��̍X�V
	void UpdateHurtPoint();
private:
	//�U��
	std::shared_ptr<MeleeAttack> m_attackN1;//�ʏ�1
	std::shared_ptr<MeleeAttack> m_attackN2;//�ʏ�2
	std::shared_ptr<MeleeAttack> m_attackN3;//�ʏ�3
	std::shared_ptr<MeleeAttack> m_attackC;//���ߍU��
};

