#pragma once
#include "SceneBase.h"
class Input;
class SceneController;
class DebugScene :
    public SceneBase
{
public:
    DebugScene(SceneController& controller);
    ~DebugScene();
    //”h¶ƒNƒ‰ƒX‚ÅÀ‘•‚ğÀ‘•
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
};

