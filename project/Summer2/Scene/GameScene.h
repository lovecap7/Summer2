#pragma once
#include "SceneBase.h"
#include  <vector>
#include  <memory>

class Input;
class SceneController;
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
   
};

