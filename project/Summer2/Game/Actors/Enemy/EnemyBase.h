#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class EnemyManager;
class AttackBase;
class EnemyBase abstract:
    public Actor
{
public:
    EnemyBase(std::unique_ptr<EnemyManager>& enemyManager);
    virtual ~EnemyBase() {};
    virtual void OnHitSearch(const Vector3& playerPos) abstract;
    std::shared_ptr<Collidable>& GetSearchTrigger() { return m_searchTrigger; };
	//ID���擾
	int GetID() const { return m_id; };
protected:
    //���f��
    std::unique_ptr<Model> m_model;
    //�v���C���[���߂��ɂ��邩���`�F�b�N����g���K�[
    std::shared_ptr<Collidable> m_searchTrigger;
    //�G�l�~�[�̎��ʔԍ�
    const int m_id;
    //�U���̃R���|�[�l���g
    virtual void CreateAttack() abstract;
    //��Ԃɍ��킹�ď��������ׂ����̂�����������
    virtual void InitState() abstract;
    //���ꔻ��̍X�V
    virtual void UpdateHurtPoint()abstract;
    //�U��������o��
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
};

