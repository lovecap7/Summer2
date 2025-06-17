#pragma once
#include <memory>
#include <vector>
class Actor;
class Player;
class Input;
class Camera;
class AttackManager;
class CollisionManager;
class UIManager;
class EnemyManager;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(std::vector<std::shared_ptr<Actor>> actors, std::shared_ptr<Player> player);
	virtual~ActorManager();
	//�o�^����
	void Entry(std::shared_ptr<UIManager> uiManager);
	//�o�^����
	void Exit(std::shared_ptr<UIManager> uiManager);
	//����������
	void Init();
	//�X�V����
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<UIManager> uiManager);
	//�`��
	void Draw()const;
	//�R���W�����}�l�[�W���[
	std::shared_ptr<CollisionManager> GetCollisionManager() const{ return m_collManager; };
	//�G�l�~�[�}�l�[�W���[
	std::shared_ptr<EnemyManager> GetEnemyManager() const{ return m_enemyManager; };
private:
	//�A�N�^�[
	std::vector<std::shared_ptr<Actor>> m_actors;
	//�v���C���[
	std::shared_ptr<Player> m_player;
	//�����蔻��ƏՓˏ���
	std::shared_ptr<CollisionManager> m_collManager;
	//�G�̕⏕�I�ȏ���������
	std::shared_ptr<EnemyManager> m_enemyManager;
	//�U���̏���
	std::shared_ptr<AttackManager> m_attackManager;
private:
	int m_id;//����U��ԍ�
	void SetUpId();
};

