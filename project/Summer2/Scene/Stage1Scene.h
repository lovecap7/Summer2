#pragma once
#include "SceneBase.h"
class SceneController;
class Input;
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
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
};

