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
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
};

