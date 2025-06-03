#pragma once
#include "../EnemyBase.h"
class ActorManager;
class Input;
class Camera;
class EnemyManager;
class AttackManager;
class MeleeAttack;
class PurpleDinosaur :
    public EnemyBase, public std::enable_shared_from_this<PurpleDinosaur>
{
public:
	PurpleDinosaur(int modelHandle, Vector3 pos);
	~PurpleDinosaur();
	//�o�^����
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//�o�^����
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//����������
	void Init()override;
	//�X�V����
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
	//�d��
	void Gravity(const Vector3& gravity)override;
	//�Փ˃C�x���g
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//�`��
	void Draw()const override;
	//�X�V�����̊m��
	void Complete() override;
	//�q�b�g���A�N�V����
	void HitReaction() override;
	bool IsHit() { return m_isHit; };
	void SetIsHit(bool isHit) { m_isHit = isHit; };
	//�U���̃N�[���^�C�����擾
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//�U���̃N�[���^�C�����Z�b�g
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
private:
	//�U�����󂯂���
	bool m_isHit;
	//�U���ł���܂ł̃N�[���^�C��
	int m_attackCoolTime;
	//���ꔻ��̍X�V
	void UpdateHurtPoint()override;
	//�U���̃N�[���^�C�����X�V
	void UpdateAttackCoolTime();
};

