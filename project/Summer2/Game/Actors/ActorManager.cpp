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
#include "../UI/UIManager.h"
#include "Item/ItemGenerator.h"

ActorManager::ActorManager(std::shared_ptr<Player> player):
	m_actors{},
	m_player(player),
	m_id(0)
{
	//コリジョンマネージャー
	m_collManager = std::make_shared<CollisionManager>();
	//エネミーマネージャー
	m_enemyManager = std::make_shared<EnemyManager>(m_player);
	//攻撃の処理
	m_attackManager = std::make_shared<AttackManager>();
	//アイテムジェネレーター
	m_itemGenerator = std::make_shared<ItemGenerator>();
}

ActorManager::~ActorManager()
{
}

void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//すでに登録されているならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		return;
	}
	//アクターをセット
	m_actors.emplace_back(actor);
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//すでに登録されているならそのアクターを消す
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		//削除
		m_actors.erase(it);
	}
}

void ActorManager::Init(std::vector<std::shared_ptr<Actor>> actors)
{
	//アクターの登録
	for (auto& actor : actors)
	{
		actor->Entry(shared_from_this());
	}
	//アクターの初期化処理
	for (auto& actor : actors)
	{
		actor->Init();
	}
}

void ActorManager::End()
{
	//アクターの終了処理
	for (auto& actor : m_actors)
	{
		actor->Exit(shared_from_this());
	}
	//アイテムの終了処理
	m_itemGenerator->End();
}

void ActorManager::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<UIManager> uiManager)
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
	auto itemGenerator = m_itemGenerator;
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [itemGenerator](std::shared_ptr<Actor> actor) {
		bool isDead = actor->IsDelete();
		if (isDead)
		{
			//アイテムをランダム生成
			itemGenerator->RandGenerateItem(actor->GetCollidable()->GetRb()->GetPos());
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

	//アイテムを実装
	itemGenerator->MoveItems(shared_from_this());
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
