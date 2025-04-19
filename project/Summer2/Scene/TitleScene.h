#pragma once
#include "SceneBase.h"
#include <memory>
#include  "../General/Geometry.h"

class Input;
class TitleScene :
    public SceneBase
{
public:
    TitleScene(SceneController& controller);
    ~TitleScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
};

