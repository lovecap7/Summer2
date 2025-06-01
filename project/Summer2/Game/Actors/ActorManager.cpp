#include "ActorManager.h"
#include "Actor.h"
#include "Player/Player.h"
#include "../../General/Input.h"
#include "../Camera/Camera.h"
#include "../Attack/AttackManager.h"
#include "Enemy/EnemyManager.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/CollisionManager.h"
#include "../../General/game.h"

ActorManager::ActorManager(std::vector<std::shared_ptr<Actor>> actors, std::shared_ptr<Player> player):
	m_actors(actors),
	m_player(player)
{
	//�R���W�����}�l�[�W���[
	m_collManager = std::make_unique<CollisionManager>();
	//�G�l�~�[�}�l�[�W���[
	m_enemyManager = std::make_unique<EnemyManager>(m_player);
	//�U���̏���
	m_attackManger = std::make_unique<AttackManager>();
}

void ActorManager::Entry()
{
	//�A�N�^�[�̓o�^����
	for (auto& actor : m_actors)
	{
		actor->Entry(shared_from_this());
	}
}

void ActorManager::Exit()
{
	//�A�N�^�[�̓o�^����
	for (auto& actor : m_actors)
	{
		actor->Exit(shared_from_this());
	}
}

void ActorManager::Init()
{
	//�A�N�^�[�̏���������
	for (auto& actor : m_actors)
	{
		actor->Init();
	}
}

void ActorManager::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//�v���C���[�̍��G
	m_enemyManager->Search();
	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(input, camera, m_attackManger);
		actor->Gravity(Gravity::kGravity);
	}
	//�U���̏���
	m_attackManger->Update(m_actors);

	//���Ńt���O�`�F�b�N
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> actor) {return actor->IsDead();});
	m_actors.erase(remIt, m_actors.end());//�폜

	//�A�N�^�[�̏Փˏ���
	m_collManager->Update(m_actors);
	//�X�V�m��
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}
}

void ActorManager::Draw() const
{
	//�A�N�^�[�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
	//�U���̕`��
	m_attackManger->Draw();
}
