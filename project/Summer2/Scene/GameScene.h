#pragma once
#include "SceneBase.h"
#include  "../General/Geometry.h"
#include  <vector>
#include  <memory>

class Input;
class Actor;
class Collidable;
class GameCamera;
class CollisionChecker;
class CollisionProcess;
class UIManager;

class GameScene :
    public SceneBase
{
public:
    GameScene(SceneController& controller);
  
    ~GameScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// </summary>
    /// <param name="input"></param>
    virtual void Update(Input& input) override;

  
    virtual void Draw() override;
private:
    //オブジェクト
    std::vector<std::shared_ptr<Actor>> m_actors;
    //カメラ
    std::shared_ptr<GameCamera> m_camera;
    //当たり判定
    std::shared_ptr<CollisionChecker> m_collChecker;
    std::shared_ptr<CollisionProcess> m_collProcess;

    //UI
    std::shared_ptr<UIManager> m_uiManager;

private:
    //モデルハンドル
    //プレイヤー
    int m_playerHandle;
    //敵
    int m_crabHandle;
    int m_beeHandle;
    //ステージ
    //床
    int m_groundOpenHandle;
    //真っすぐな道
    int m_roadStraightHandle;
    //コーナー
    int m_roadCornerHandle;
    //坂
    int m_roadSlopeHandle;
    //空
    int m_skyHandle;
    //ゴール
    int m_goalHandle;

    //フェード
    float m_fadeAlpha;

    //空の回転に使う
    float m_skyAngle;

private:
    //状態遷移
    using UpdateFunc_t = void(GameScene::*)(Input& input);
    UpdateFunc_t m_update;
    using DrawFunc_t = void(GameScene::*)();
    DrawFunc_t m_draw;

    //待機状態
    void NormalUpdate(Input& input);
    void NormalDraw();
    //だんだん明るくなる
    void FadeInUpdate(Input& input);
    //だんだん暗くなる
    void FadeOutUpdate(Input& input);
    void FadeDraw();

    //当たり判定のチェック
    void CollisionCheck();
    //トリガー判定のチェック
    void TriggerCheck();
    //ステージ
    void StageCreate();

};

