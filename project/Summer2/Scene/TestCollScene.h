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
class EnemyManager;
class AttackManager;
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
    //敵の補助的な処理をする
    std::unique_ptr<EnemyManager> m_enemyManager;
    //攻撃の処理
    std::unique_ptr<AttackManager> m_attackManger;
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

