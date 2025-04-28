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
private:
    //�n���h��
    int m_playerHandle;
};

