#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>
class Actor;
class Player;
class Camera;
class SceneController;
class Input;
class CollisionManager;
class EnemyManager;
class AttackManager;
class TestCollScene :
    public SceneBase
{
public:
    TestCollScene(SceneController& controller);
    ~TestCollScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    std::vector<std::shared_ptr<Actor>> m_actors;
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�J����
    std::unique_ptr<Camera> m_camera;
    //�����蔻��ƏՓˏ���
    std::unique_ptr<CollisionManager> m_collManager;
    //�G�̕⏕�I�ȏ���������
    std::unique_ptr<EnemyManager> m_enemyManager;
    //�U���̏���
    std::unique_ptr<AttackManager> m_attackManger;
private:
    //�n���h��
    int m_playerHandle;
    int m_polygonHandle;
    int m_wallHandle;
    int m_common1Handle;
private:
#if _DEBUG
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;

#endif
};

