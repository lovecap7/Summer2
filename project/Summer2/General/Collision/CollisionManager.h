#pragma once
#include <vector>
#include <memory>
class Actor;
class CollisionChecker;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	void Update(std::vector<std::shared_ptr<Actor>> actors);
private:
	//当たり判定のチェックをするクラス
	std::unique_ptr<CollisionChecker> m_collChecker;
};

