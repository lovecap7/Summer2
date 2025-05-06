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
private:
	//�g���K�[�̓����蔻��̃`�F�b�N������N���X
	std::shared_ptr<CollisionChecker> m_collChecker;
	//�v���C���[�̃|�C���^
	std::shared_ptr<Player> m_player;
};

