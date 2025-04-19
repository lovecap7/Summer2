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
    //派生クラスで実装を実装
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    //フレームを数える
    int m_countFrame;
    //スコアの座標
    Vector2 m_scorePos;
private:
    //リザルトにいるキャラ
	std::shared_ptr<ResultChara> m_chara;
};

