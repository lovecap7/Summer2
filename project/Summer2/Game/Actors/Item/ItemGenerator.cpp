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
	//�����_���Ɍ���
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
	if (m_items.empty())return;//��Ȃ�n�����̂͂Ȃ�
	for (auto& item : m_items)
	{
		//�A�N�^�[�̓o�^
		item->Entry(actorManager);
		//����������
		item->Init();
	}
	m_items.clear();//�폜
}
