#pragma once
#include "EnemyBase.h"
class Input;
class Camera;
class EnemyManager;
class MeleeAttack;
class Common1 :
	public EnemyBase,std::enable_shared_from_this<Common1>
{
public:
    Common1(std::unique_ptr<EnemyManager>& enemyManager,int modelHandle,Vector3 pos);
    ~Common1();
	//����������
	void Init()override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
	void OnHitSearch(const Vector3& playerPos)override;
	void HitReaction() override;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Common1::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//���O�̏�Ԃ��o���Ă���
	//�ҋ@���
	void UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera);
	//�ړ�
	void UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera);
	//�U��
	void UpdateAttack(const Input& input, const std::unique_ptr<Camera>& camera);
	//��炢
	void UpdateHit(const Input& input, const std::unique_ptr<Camera>& camera);
	//���S
	void UpdateDead(const Input& input, const std::unique_ptr<Camera>& camera);
private:
	//��Ԃɍ��킹�ď��������ׂ����̂�����������
	void InitState()override;
	//����
	void SpeedDown();
	//�퓬�Ɋւ���X�V����
	void BattleUpdate();
	//���r�̓����蔻��쐬
	void CreateLeftArm();
	//���r�̈ʒu�X�V
	void UpdateLeftArm();
	//�U���̃R���|�[�l���g
	void CreateAttack() override;
	//���ꔻ��̍X�V
	void UpdateHurtPoint()override;
private:
	//�퓬���
	bool m_isBattleMode;
	//�T�[�`
	bool m_isHitSearch;
	//�U���̃N�[���^�C��
	int m_attackCoolTime;
	//�U��
	std::shared_ptr<MeleeAttack> m_punch;
	//���r
	std::shared_ptr<Collidable> m_leftArm;
};

