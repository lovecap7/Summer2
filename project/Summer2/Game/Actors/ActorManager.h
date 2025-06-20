#pragma once
#include <memory>
#include <vector>
class Actor;
class Player;
class Input;
class Camera;
class AttackManager;
class CollisionManager;
class UIManager;
class EnemyManager;
class ItemGenerator;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(std::shared_ptr<Player> player);
	virtual~ActorManager();
	//アクターの登録処理
	void Entry(std::shared_ptr<Actor> actor);
	//登録解除
	void Exit(std::shared_ptr<Actor> actor);
	//初期化処理
	void Init(std::vector<std::shared_ptr<Actor>> actors);
	//終了処理
	void End();
	//更新処理
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<UIManager> uiManager);
	//描画
	void Draw()const;
	//コリジョンマネージャー
	std::shared_ptr<CollisionManager> GetCollisionManager() const{ return m_collManager; };
	//エネミーマネージャー
	std::shared_ptr<EnemyManager> GetEnemyManager() const{ return m_enemyManager; };
private:
	//アクター
	std::vector<std::shared_ptr<Actor>> m_actors;
	//プレイヤー
	std::shared_ptr<Player> m_player;
	//当たり判定と衝突処理
	std::shared_ptr<CollisionManager> m_collManager;
	//敵の補助的な処理をする
	std::shared_ptr<EnemyManager> m_enemyManager;
	//攻撃の処理
	std::shared_ptr<AttackManager> m_attackManager;
	//アイテムジェネレーター
	std::shared_ptr<ItemGenerator> m_itemGenerator;
private:
	int m_id;//割り振る番号
	void SetUpId();
	//消滅フラグが立っているアクターの削除
	void CheckDeleteActor(std::shared_ptr<ItemGenerator> itemGenerator);
};

