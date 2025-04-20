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
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
};

