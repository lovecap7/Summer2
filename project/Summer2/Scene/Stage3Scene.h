#pragma once
#include "SceneBase.h"
class Input;
class SceneController;
class Stage3Scene :
    public SceneBase
{
public:
    Stage3Scene(SceneController& controller);
    ~Stage3Scene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Update(Input& input) override;
    virtual void Draw() override;
};

