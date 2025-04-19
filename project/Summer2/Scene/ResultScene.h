#pragma once
#include "SceneBase.h"
#include "../General/Geometry.h"
#include <memory>

class Input;
class ResultChara;
class ResultScene :
    public SceneBase
{
public:
    ResultScene(SceneController& controller);
    ~ResultScene();
    //�h���N���X�Ŏ���������
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    //�t���[���𐔂���
    int m_countFrame;
    //�X�R�A�̍��W
    Vector2 m_scorePos;
private:
    //���U���g�ɂ���L����
	std::shared_ptr<ResultChara> m_chara;
};

