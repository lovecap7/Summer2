#pragma once
#include "EnemyBase.h"
class Input;
class Camera;
class EnemyManager;
class Common1 :
    public EnemyBase
{
public:
    Common1(std::unique_ptr<EnemyManager>& enemyManager,int modelHandle,Vector3 pos);
    ~Common1();
	void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
	void OnHitSearch(const Vector3& playerPos)override;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Common1::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//���O�̏�Ԃ��o���Ă���
	//�ҋ@���
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//�ړ�
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//�U��
	void AttackUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//��炢
	void HitUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//��Ԃɍ��킹�ď��������ׂ����̂�����������
	void StateInit();
	//����
	void SpeedDown();
private:
	//�퓬���
	bool m_isBattleMode;
	//�T�[�`
	bool m_isHitSearch;
	//�U���̃N�[���^�C��
	int m_attackCoolTime;
};

