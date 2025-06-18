#pragma once
#include <memory>
#include <vector>
#include "ItemBase.h"
#include "../../../General/Math/Vector3.h"
class Actor;
class ItemGenerator
{
public:
	ItemGenerator();
	~ItemGenerator();
	void End();
	//�A�C�e���������_���ɐ���
	void RandGenerateItem(Vector3 pos);
	//�A�C�e�����܂Ƃ߂��z���n��
	void MoveItems(std::shared_ptr<ActorManager> actorManager);
private:
	//�A�C�e�����܂Ƃ߂��z��
	std::vector<std::shared_ptr<ItemBase>> m_items;
	//�n���h��
	int m_heartHandle;
};

