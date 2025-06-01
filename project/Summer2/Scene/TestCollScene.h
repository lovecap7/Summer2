#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>
class ActorManager;
class Player;
class Camera;
class SceneController;
class Input;
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
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    //アクターマネージャー
    std::unique_ptr<ActorManager> m_actorManager;
    //プレイヤー
    std::shared_ptr<Player> m_player;
    //カメラ
    std::unique_ptr<Camera> m_camera;
private:
    //ハンドル
    int m_playerHandle;
    int m_polygonHandle;
    int m_wallHandle;
    int m_common1Handle;
private:
#if _DEBUG
    //デバッグモード用
    bool m_isUpdateStop = false;
#endif
};

