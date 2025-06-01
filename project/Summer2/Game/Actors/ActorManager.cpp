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
	m_player(player),
	m_id(0)
{
	//コリジョンマネージャー
	m_collManager = std::make_shared<CollisionManager>();
	//エネミーマネージャー
	m_enemyManager = std::make_shared<EnemyManager>(m_player);
	//攻撃の処理
	m_attackManager = std::make_shared<AttackManager>();
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
	//登録
	Entry();
}

void ActorManager::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//プレイヤーの索敵
	m_enemyManager->Search();
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(input, camera, m_attackManager);
		actor->Gravity(Gravity::kGravity);
	}
	//攻撃の処理
	m_attackManager->Update(m_actors);

	//消滅フラグチェック
	auto thisPointer = shared_from_this();
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [thisPointer](std::shared_ptr<Actor> actor) {
		bool isDead = actor->IsDead();
		if (isDead)//死んでるなら
		{
			//Exit関数を呼ぶ
			actor->Exit(thisPointer);
		}
		return isDead;
		});
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
	m_attackManager->Draw();
}

void ActorManager::SetUpId()
{
	//IDを登録
	for (auto& actor : m_actors)
	{
		actor->SetID(m_id);
		++m_id;
	}
}
