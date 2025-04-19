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
    //”h¶ƒNƒ‰ƒX‚ÅÀ‘•‚ğÀ‘•
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    
};

