#pragma once
#include <vector>
#include <memory>
class Actor;
class CollisionChecker;
class CollisionProcess;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	void Update(std::vector<std::shared_ptr<Actor>> actors);
private:
	//�����蔻��̃`�F�b�N������N���X
	std::unique_ptr<CollisionChecker> m_collChecker;
	//�Փˏ���
	std::unique_ptr<CollisionProcess> m_collProcessor;
};

