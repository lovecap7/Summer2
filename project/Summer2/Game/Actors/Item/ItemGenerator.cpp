#include "ItemGenerator.h"
#include "../ActorManager.h"
#include "../Actor.h"
#include "Heart.h"
#include <DxLib.h>

ItemGenerator::ItemGenerator() :
	m_heartHandle(LoadGraph("Data/Model/Item/Heart.mv1"))
{
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
	//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
	auto rand = GetRand(1);
	switch (rand)
	{
	case static_cast<int>(ItemKind::Heart):
		item = std::make_shared<Heart>(MV1DuplicateModel(m_heartHandle), pos);
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
	if (m_items.empty())return;//‹ó‚È‚ç“n‚·‚à‚Ì‚Í‚È‚¢
	for (auto& item : m_items)
	{
		//ƒAƒNƒ^[‚Ì“o˜^
		item->Entry(actorManager);
		//‰Šú‰»ˆ—
		item->Init();
	}
	m_items.clear();//íœ
}
