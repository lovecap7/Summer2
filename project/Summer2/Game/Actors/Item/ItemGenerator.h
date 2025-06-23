#pragma once
#include <memory>
#include <vector>
#include "ItemBase.h"
#include "../../../General/Math/Vector3.h"
class ActorManager;
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
	//�񕜂̐���
	void GenerateHeart(Vector3 pos);
	//���e�̐���
	void GenerateBomb(Vector3 pos);
private:
	//�A�C�e�����܂Ƃ߂��z��
	std::vector<std::shared_ptr<ItemBase>> m_items;
	//�n���h��
	int m_heartHandle;//��
	int m_bombHandle;//���e
};

