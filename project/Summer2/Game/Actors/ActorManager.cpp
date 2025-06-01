#include "ActorManager.h"
#include "Actor.h"
#include "Player/Player.h"
#include "../../General/Input.h"
#include "../Camera/Camera.h"
#include "../Attack/AttackManager.h"
#include "Enemy/EnemyManager.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/CollisionManager.h"
#include "../../General/game.h"

ActorManager::ActorManager(std::vector<std::shared_ptr<Actor>> actors, std::shared_ptr<Player> player):
	m_actors(actors),
	m_player(player)
{
	//コリジョンマネージャー
	m_collManager = std::make_unique<CollisionManager>();
	//エネミーマネージャー
	m_enemyManager = std::make_unique<EnemyManager>(m_player);
	//攻撃の処理
	m_attackManger = std::make_unique<AttackManager>();
}

void ActorManager::Entry()
{
	//アクターの登録処理
	for (auto& actor : m_actors)
	{
		actor->Entry(shared_from_this());
	}
}

void ActorManager::Exit()
{
	//アクターの登録解除
	for (auto& actor : m_actors)
	{
		actor->Exit(shared_from_this());
	}
}

void ActorManager::Init()
{
	//アクターの初期化処理
	for (auto& actor : m_actors)
	{
		actor->Init();
	}
}

void ActorManager::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//プレイヤーの索敵
	m_enemyManager->Search();
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(input, camera, m_attackManger);
		actor->Gravity(Gravity::kGravity);
	}
	//攻撃の処理
	m_attackManger->Update(m_actors);

	//消滅フラグチェック
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> actor) {return actor->IsDead();});
	m_actors.erase(remIt, m_actors.end());//削除

	//アクターの衝突処理
	m_collManager->Update(m_actors);
	//更新確定
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}
}

void ActorManager::Draw() const
{
	//アクターの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
	//攻撃の描画
	m_attackManger->Draw();
}
