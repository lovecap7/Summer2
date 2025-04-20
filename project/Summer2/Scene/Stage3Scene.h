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
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Update(Input& input) override;
    virtual void Draw() override;
};

