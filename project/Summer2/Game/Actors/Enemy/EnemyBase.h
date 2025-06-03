#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class AttackBase;
class AttackManager;
class EnemyBase abstract :
    public Actor
{
public:
    EnemyBase();
    virtual ~EnemyBase() {};
    //�͈͓��Ƀv���C���[�������ꍇ�ʒu���擾
    void OnHitSearch(const Vector3& playerPos);
    //���G�ɐ���������
    bool IsHitSearch() const { return m_isHitSearch; };
    //�v���C���[�ւ̃x�N�g����Ԃ��֐�
    Vector3 GetPlayerVec() const;
    std::shared_ptr<Collidable>& GetSearchTrigger() { return m_searchTrigger; };
    //���f���N���X
    std::shared_ptr<Model> GetModel() const { return m_model; };
protected:
    //���f��
    std::shared_ptr<Model> m_model;
    //�v���C���[���߂��ɂ��邩���`�F�b�N����g���K�[
    std::shared_ptr<Collidable> m_searchTrigger;
    //�T�[�`�ɐ���������
    bool m_isHitSearch;
    //�v���C���[�̍��W
    Vector3 m_playerPos;
    //���ꔻ��̍X�V
    virtual void UpdateHurtPoint()abstract;
    //�U��������o��
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
};

