#pragma once
#include "SceneBase.h"
#include <memory>

class Input;
class SceneController;
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
    
};

