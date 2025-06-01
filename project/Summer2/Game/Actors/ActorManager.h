#pragma once
#include <memory>
#include <vector>
class Actor;
class Player;
class Input;
class Camera;
class AttackManager;
class CollisionManager;
class EnemyManager;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(std::vector<std::shared_ptr<Actor>> actors, std::shared_ptr<Player> player);
	virtual~ActorManager() {};
	//登録処理
	void Entry();
	//登録解除
	void Exit();
	//初期化処理
	void Init();
	//更新処理
	void Update(const Input& input, const std::unique_ptr<Camera>& camera);
	//描画
	void Draw()const;
	//コリジョンマネージャー
	std::unique_ptr<CollisionManager>& GetCollisionManager() const{ return m_collManager; };
	//エネミーマネージャー
	std::unique_ptr<EnemyManager>& GetEnemyManager() const{ return m_enemyManager; };
private:
	//アクター
	std::vector<std::shared_ptr<Actor>> m_actors;
	//プレイヤー
	std::shared_ptr<Player> m_player;
	//当たり判定と衝突処理
	std::unique_ptr<CollisionManager> m_collManager;
	//敵の補助的な処理をする
	std::unique_ptr<EnemyManager> m_enemyManager;
	//攻撃の処理
	std::unique_ptr<AttackManager> m_attackManger;
};

