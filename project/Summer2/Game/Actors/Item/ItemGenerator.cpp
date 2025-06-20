#include "ItemGenerator.h"
#include "../ActorManager.h"
#include "../Actor.h"
#include "Heart.h"
#include <DxLib.h>
#include <cassert>

ItemGenerator::ItemGenerator() :
	m_heartHandle(MV1LoadModel("Data/Model/Item/Heart.mv1"))
{
	assert(m_heartHandle >= 0);
}

ItemGenerator::~ItemGenerator()
{
}

void ItemGenerator::End()
{
	MV1DeleteModel(m_heartHandle);
}

void ItemGenerator::RandGenerateItem(Vector3 pos)
{
	std::shared_ptr<ItemBase> item = nullptr;
	//ランダムに決定
	auto rand = 0;//GetRand(1);
	switch (rand)
	{
	case static_cast<int>(ItemKind::Heart):
		item = std::make_shared<Heart>(MV1DuplicateModel(m_heartHandle), pos);
		break;
	default:
		break;
	}
	if (item != nullptr)
	{
		m_items.emplace_back(item);
	}
}

void ItemGenerator::MoveItems(std::shared_ptr<ActorManager> actorManager)
{
	if (m_items.empty())return;//空なら渡すものはない
	for (auto& item : m_items)
	{
		//アクターの登録
		item->Entry(actorManager);
		//初期化処理
		item->Init();
	}
	m_items.clear();//削除
}
