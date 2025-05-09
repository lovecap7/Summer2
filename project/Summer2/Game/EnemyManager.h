#pragma once
#include <memory>
#include <vector>
class Actor;
class Player;
class CollisionChecker;
class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> player);
	virtual ~EnemyManager() {};
	void Update(std::vector<std::shared_ptr<Actor>> actors);
	//IDを与える
	int GetEnemyID();
private:
	//トリガーの当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
	//プレイヤーのポインタ
	std::shared_ptr<Player> m_player;
	//敵に割り振るID(IDはインスタンス化された際に数がIDになる)
	int m_enemyId;
};

