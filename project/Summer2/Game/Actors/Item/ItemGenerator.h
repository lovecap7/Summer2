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
	//アイテムをランダムに生成
	void RandGenerateItem(Vector3 pos);
	//アイテムをまとめた配列を渡す
	void MoveItems(std::shared_ptr<ActorManager> actorManager);
private:
	//アイテムをまとめた配列
	std::vector<std::shared_ptr<ItemBase>> m_items;
	//ハンドル
	int m_heartHandle;
};

