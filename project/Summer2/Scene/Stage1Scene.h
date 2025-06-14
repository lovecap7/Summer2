#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class Player;
class Camera;
class SceneController;
class Input;
class Actor;
class ActorManager;
class Stage1Scene :
    public SceneBase
{
public:
    Stage1Scene(SceneController& controller);
    ~Stage1Scene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    //�A�N�^�[�}�l�[�W���[
    std::shared_ptr<ActorManager> m_actorManager;
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�J����
    std::unique_ptr<Camera> m_camera;
private:
    //�n���h��
    //�L�����N�^�[
    int m_playerHandle;
    int m_purpleDinosaurHandle;
    int m_smallDragonHandle;
    int m_bossDragonHandle;
    //�X�e�[�W�̃I�u�W�F�N�g
    int m_wallHandle;
    int m_pathHandle;
    //�X�e�[�W�̓����蔻��
    int m_cubeHandle;
    int m_cylinderHandle;
private:
    //�L�����N�^�[�̍쐬�Ɣz�u
    void CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors);
    //�X�e�[�W�̍쐬�Ɣz�u
    void CreateStage(std::vector<std::shared_ptr<Actor>>& actors);
private:
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

