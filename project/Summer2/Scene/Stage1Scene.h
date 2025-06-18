#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class Player;
class Camera;
class SceneController;
class Input;
class Actor;
class ActorManager;
class Sky;
class UIManager;
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
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //アクター
    std::vector<std::shared_ptr<Actor>> m_actors;
    //アクターマネージャー
    std::shared_ptr<ActorManager> m_actorManager;
    //プレイヤー
    std::shared_ptr<Player> m_player;
    //カメラ
    std::unique_ptr<Camera> m_camera;
    //UI
    std::shared_ptr<UIManager> m_uiManager;
private:
    //ハンドル
    //キャラクター
    int m_playerHandle;
    int m_purpleDinosaurHandle;
    int m_smallDragonHandle;
    int m_bossDragonHandle;
    //ステージのオブジェクト
    int m_wallHandle;
    int m_pathHandle;
    int m_skyHandle;
    //ステージの当たり判定
    int m_cubeHandle;
    int m_cylinderHandle;
private:
    //キャラクターの作成と配置
    void CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors);
    //ステージの作成と配置
    void CreateStage(std::vector<std::shared_ptr<Actor>>& actors);
private:
    //デバッグモード用
    bool m_isUpdateStop = false;
};

