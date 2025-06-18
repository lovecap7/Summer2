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
#include "../UI/UIManager.h"
#include "Item/ItemGenerator.h"

ActorManager::ActorManager(std::shared_ptr<Player> player):
	m_actors{},
	m_player(player),
	m_id(0)
{
	//�R���W�����}�l�[�W���[
	m_collManager = std::make_shared<CollisionManager>();
	//�G�l�~�[�}�l�[�W���[
	m_enemyManager = std::make_shared<EnemyManager>(m_player);
	//�U���̏���
	m_attackManager = std::make_shared<AttackManager>();
	//�A�C�e���W�F�l���[�^�[
	m_itemGenerator = std::make_shared<ItemGenerator>();
}

ActorManager::~ActorManager()
{
}

void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		return;
	}
	//�A�N�^�[���Z�b�g
	m_actors.emplace_back(actor);
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//���łɓo�^����Ă���Ȃ炻�̃A�N�^�[������
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		//�폜
		m_actors.erase(it);
	}
}

void ActorManager::Init(std::vector<std::shared_ptr<Actor>> actors)
{
	//�A�N�^�[�̓o�^
	for (auto& actor : actors)
	{
		actor->Entry(shared_from_this());
	}
	//�A�N�^�[�̏���������
	for (auto& actor : actors)
	{
		actor->Init();
	}
}

void ActorManager::End()
{
	//�A�N�^�[�̏I������
	for (auto& actor : m_actors)
	{
		actor->Exit(shared_from_this());
	}
	//�A�C�e���̏I������
	m_itemGenerator->End();
}

void ActorManager::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<UIManager> uiManager)
{
	//�v���C���[�̍��G
	m_enemyManager->Search();
	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(input, camera, m_attackManager);
		actor->Gravity(Gravity::kGravity);
	}
	//�U���̏���
	m_attackManager->Update(m_actors);

	//���Ńt���O�`�F�b�N
	auto itemGenerator = m_itemGenerator;
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [itemGenerator](std::shared_ptr<Actor> actor) {
		bool isDead = actor->IsDelete();
		if (isDead)
		{
			//�A�C�e���������_������
			itemGenerator->RandGenerateItem(actor->GetCollidable()->GetRb()->GetPos());
		}
		return isDead;
		});
	m_actors.erase(remIt, m_actors.end());//�폜

	//�A�N�^�[�̏Փˏ���
	m_collManager->Update(m_actors);
	//�X�V�m��
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}

	//�A�C�e��������
	itemGenerator->MoveItems(shared_from_this());
}

void ActorManager::Draw() const
{
	//�A�N�^�[�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
	//�U���̕`��
	m_attackManager->Draw();
}

void ActorManager::SetUpId()
{
	//ID��o�^
	for (auto& actor : m_actors)
	{
		actor->SetID(m_id);
		++m_id;
	}
}
