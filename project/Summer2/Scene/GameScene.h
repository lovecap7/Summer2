#pragma once
#include "SceneBase.h"
#include  "../General/Geometry.h"
#include  <vector>
#include  <memory>

class Input;
class Actor;
class Collidable;
class GameCamera;
class CollisionChecker;
class CollisionProcess;
class UIManager;

class GameScene :
    public SceneBase
{
public:
    GameScene(SceneController& controller);
  
    ~GameScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// </summary>
    /// <param name="input"></param>
    virtual void Update(Input& input) override;

  
    virtual void Draw() override;
private:
    //�I�u�W�F�N�g
    std::vector<std::shared_ptr<Actor>> m_actors;
    //�J����
    std::shared_ptr<GameCamera> m_camera;
    //�����蔻��
    std::shared_ptr<CollisionChecker> m_collChecker;
    std::shared_ptr<CollisionProcess> m_collProcess;

    //UI
    std::shared_ptr<UIManager> m_uiManager;

private:
    //���f���n���h��
    //�v���C���[
    int m_playerHandle;
    //�G
    int m_crabHandle;
    int m_beeHandle;
    //�X�e�[�W
    //��
    int m_groundOpenHandle;
    //�^�������ȓ�
    int m_roadStraightHandle;
    //�R�[�i�[
    int m_roadCornerHandle;
    //��
    int m_roadSlopeHandle;
    //��
    int m_skyHandle;
    //�S�[��
    int m_goalHandle;

    //�t�F�[�h
    float m_fadeAlpha;

    //��̉�]�Ɏg��
    float m_skyAngle;

private:
    //��ԑJ��
    using UpdateFunc_t = void(GameScene::*)(Input& input);
    UpdateFunc_t m_update;
    using DrawFunc_t = void(GameScene::*)();
    DrawFunc_t m_draw;

    //�ҋ@���
    void NormalUpdate(Input& input);
    void NormalDraw();
    //���񂾂񖾂邭�Ȃ�
    void FadeInUpdate(Input& input);
    //���񂾂�Â��Ȃ�
    void FadeOutUpdate(Input& input);
    void FadeDraw();

    //�����蔻��̃`�F�b�N
    void CollisionCheck();
    //�g���K�[����̃`�F�b�N
    void TriggerCheck();
    //�X�e�[�W
    void StageCreate();

};

