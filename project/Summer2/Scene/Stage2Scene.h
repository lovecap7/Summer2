#pragma once
#include "SceneBase.h"
class Input;
class SceneController;
class Stage2Scene :
    public SceneBase
{
public:
    Stage2Scene(SceneController& controller);
    ~Stage2Scene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
};

