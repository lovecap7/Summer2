#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>
class Actor;
class Player;
class Camera;
class SceneController;
class Input;
class CollisionManager;
class TestCollScene :
    public SceneBase
{
public:
    TestCollScene(SceneController& controller);
    ~TestCollScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    std::vector<std::shared_ptr<Actor>> m_actors;
    //プレイヤー
    std::shared_ptr<Player> m_player;
    //カメラ
    std::unique_ptr<Camera> m_camera;
    //当たり判定と衝突処理
    std::unique_ptr<CollisionManager> m_collManager;
private:
    //ハンドル
    int m_playerHandle;
};

