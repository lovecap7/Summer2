#include "GameScene.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>

#include  "../General/Input.h"
#include "SceneController.h"
#include "../Game/Score/ScoreManager.h"
#include "ResultScene.h"
#include "../Game/Actor.h"
#include "../Game/Player.h"
#include "../General/GameCamera.h"
#include "../General/Collision/Collidable.h"
#include "../General/Collision/Trigger.h"
#include "../General/Collision/ColliderBase.h"
#include "../General/game.h"
//�����蔻��
#include "../General/Collision/CollisionChecker.h"
#include "../General/Collision/CollisionProcess.h"
//UI
#include "../General/UI/UIManager.h"
//�G
#include "../Game/Enemy1.h"
#include "../Game/Enemy2.h"
//�X�e�[�W
#include "../Game/Stage/StageGround.h"
//�S�[��
#include "../Game/Stage/Goal.h"

namespace
{
	constexpr float kWorldSize = 10000.0f;
	constexpr float kSpaceInterval = 100.0f;
	// ��ʂɕ\���ł���ő�p�[�e�B�N��
	constexpr int kEffectParticleLimit = 20000;
	//�����x
	constexpr float kMaxAlpha = 255.0f;
	constexpr float kAddAlpha = 5.0f;
	//�X�R�A���W
	constexpr int kScorePosX = 500;
	constexpr int kScorePosY = 0;

	//�G�t�F�N�g�p�X
	const char* kHitEffect = "../SonicHomingGame/Effects/efk/HitBom.efkefc";//�q�b�g�G�t�F�N�g
	const char* kHomigEffect = "../SonicHomingGame/Effects/efk/Homing.efkefc";//�z�[�~���O���̃G�t�F�N�g
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Model/Player/Player.mv1")),
	//�G
	m_beeHandle(MV1LoadModel("Model/Enemy/Bee.mv1")),
	m_crabHandle(MV1LoadModel("Model/Enemy/Crab.mv1")),
	//��
	m_groundOpenHandle(MV1LoadModel("Model/Stage/Plane/Plane_Open.mv1")),
	//�^�������ȓ�
	m_roadStraightHandle(MV1LoadModel("Model/Stage/Road/Road_Straight.mv1")),
	//�R�[�i�[
	m_roadCornerHandle(MV1LoadModel("Model/Stage/Road/Road_Corner.mv1")),
	//��
	m_roadSlopeHandle(MV1LoadModel("Model/Stage/Road/Road_Slider.mv1")),
	//��
	m_skyHandle(MV1LoadModel("Model/Sky/Sky_Night01.pmx")),
	m_skyAngle(0.0f),
	//�S�[��
	m_goalHandle(MV1LoadModel("Model/Stage/Goal/Goal.mv1")),
	//�t�F�[�h�C��
	m_fadeAlpha(kMaxAlpha)
{
	//�v���C���[�̃|�C���^���J�����ƃA�N�^�[�ɓn��
	std::shared_ptr<Player> player = std::make_shared<Player>(m_playerHandle,kHomigEffect);
	m_actors.push_back(player);

	//�J����
	m_camera = std::make_shared<GameCamera>(player);
	//�����蔻����`�F�b�N����N���X
	m_collChecker = std::make_shared<CollisionChecker>();
	m_collProcess = std::make_shared<CollisionProcess>();

	//UI
	m_uiManager = std::make_shared<UIManager>();

	//�X�e�[�W�����
	StageCreate();

	//���C�g�̌���
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));

	//�t�F�[�h�C�����Ă���X�^�[�g
	m_update = &GameScene::FadeInUpdate;
	m_draw = &GameScene::FadeDraw;
}

void GameScene::StageCreate()
{
	//��
	MV1SetPosition(m_skyHandle, VGet(0.0f, -200.0f, 0.0f));
	MV1SetScale(m_skyHandle, VGet(30.0f, 30.0f, 30.0f));

	//�X�^�[�g�n�_
	std::shared_ptr<StageGround> startGround = std::make_shared<StageGround>(MV1DuplicateModel(m_groundOpenHandle));
	startGround->SetScale(VGet(50.0f, 10.0f, 50.0f));
	startGround->SetPos(Vector3{ 0.0f,0.0f,0.0f });
	m_actors.push_back(startGround);

	//��
	std::shared_ptr<StageGround> roadS1 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS1->SetScale(VGet(20.0f, 30.0f, 20.0f));
	roadS1->SetPos(Vector3{ 0.0f,0.0f,-2000.0f });
	m_actors.push_back(roadS1);
	//��2
	std::shared_ptr<StageGround> roadS2 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS2->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS2->SetPos(Vector3{ -23000.0f,2000.0f,-700.0f });
	m_actors.push_back(roadS2);
	//��3
	std::shared_ptr<StageGround> roadS3 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS3->SetScale(VGet(50.0f, 50.0f, 50.0f));
	roadS3->SetPos(Vector3{ 5000.0f,2000.0f,10000.0f });
	roadS3->SetAngle(VGet(0.0f, 90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS3);
	//��4
	std::shared_ptr<StageGround> roadS4 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS4->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS4->SetPos(Vector3{ -12000.0f,2000.0f,5000.0f });
	roadS4->SetAngle(VGet(0.0f, 45.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS4);
	//��5
	std::shared_ptr<StageGround> roadS5 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS5->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS5->SetPos(Vector3{ -7000.0f,2000.0f,7000.0f });
	roadS5->SetAngle(VGet(0.0f, 45.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS5);
	//��6
	std::shared_ptr<StageGround> roadS6 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS6->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS6->SetPos(Vector3{ -15000.0f,2000.0f,7000.0f });
	roadS6->SetAngle(VGet(0.0f, 45.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS6);

	//�J�[�u
	std::shared_ptr<StageGround> roadC1 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadCornerHandle));
	roadC1->SetScale(VGet(20.0f, 30.0f, 20.0f));
	roadC1->SetPos(Vector3{ 0.0f,0.0f,-10000.0f });
	m_actors.push_back(roadC1);
	//�J�[�u2
	std::shared_ptr<StageGround> roadC2 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadCornerHandle));
	roadC2->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadC2->SetPos(Vector3{ -15900.0f,2000.0f,-18000.0f });
	roadC2->SetAngle(VGet(0.0f, 90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadC2);
	//�J�[�u3
	std::shared_ptr<StageGround> roadC3 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadCornerHandle));
	roadC3->SetScale(VGet(50.0f, 30.0f, 30.0f));
	roadC3->SetPos(Vector3{ -14000.0f,5000.0f,8000.0f });
	roadC3->SetAngle(VGet(0.0f / 180.0f * DX_PI_F, 180.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadC3);

	//�⓹
	std::shared_ptr<StageGround> roadSl1 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadSlopeHandle));
	roadSl1->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadSl1->SetPos(Vector3{ -8000.0f,100.0f,-18000.0f });
	roadSl1->SetAngle(VGet(0.0f, 90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadSl1);
	//�⓹2
	std::shared_ptr<StageGround> roadSl2 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadSlopeHandle));
	roadSl2->SetScale(VGet(50.0f, 50.0f, 50.0f));
	roadSl2->SetPos(Vector3{ 4000.0f,-100.0f,10000.0f });
	roadSl2->SetAngle(VGet(20.0f / 180.0f * DX_PI_F, -90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadSl2);

	//�G1bee
	std::shared_ptr<Enemy1> bee1 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle),Vector3{ 0.0f,2000.0f,-12000.0f },kHitEffect);
	m_actors.push_back(bee1);
	//�G2bee
	std::shared_ptr<Enemy1> bee2 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle),Vector3{ -16300.0f,3000.0f,-18000.0f }, kHitEffect);
	m_actors.push_back(bee2);
	//�G3bee
	std::shared_ptr<Enemy1> bee3 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -16300.0f,3000.0f,-19000.0f },kHitEffect);
	m_actors.push_back(bee3);
	//�G4bee
	std::shared_ptr<Enemy1> bee4 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -16300.0f,3000.0f,-17000.0f },kHitEffect);
	m_actors.push_back(bee4);
	//�G5bee
	std::shared_ptr<Enemy1> bee5 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -10000.0f,4000.0f,5000.0f },kHitEffect);
	m_actors.push_back(bee5);
	//�G6bee
	std::shared_ptr<Enemy1> bee6 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -5000.0f,4000.0f,5000.0f },kHitEffect);
	m_actors.push_back(bee6);
	for (int i = 1;i <= 12;++i)
	{
		//�G7~17bee
		std::shared_ptr<Enemy1> bee7to17 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ 2000.0f * i ,2500.0f + 1000 * i,10000.0f },kHitEffect);
		m_actors.push_back(bee7to17);

	}
	for (int i = 1;i <= 5;++i)
	{
		//�G18~22bee
		std::shared_ptr<Enemy1> bee18to22 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -20000.0f + 3000.0f * i,4000.0f,-7000.0f + 3000.0f * i },kHitEffect);
		m_actors.push_back(bee18to22);

	}
	
	for (int i = 1;i <= 3;++i)
	{
		//�G1~3crab
		std::shared_ptr<Enemy2> crab1To3 = std::make_shared<Enemy2>(MV1DuplicateModel(m_beeHandle), Vector3{ 0.0f,200.0f,-5000.0f + -2000.0f * i },kHitEffect);
		m_actors.push_back(crab1To3);
	}
	

	//�S�[���O
	std::shared_ptr<StageGround> endGround = std::make_shared<StageGround>(MV1DuplicateModel(m_groundOpenHandle));
	endGround->SetScale(VGet(50.0f, 50.0f, 50.0f));
	endGround->SetPos(Vector3{	23600.0f,11000.0f,10000.0f });
	m_actors.push_back(endGround);

	//�S�[��
	std::shared_ptr<Goal> goal = std::make_shared<Goal>(MV1DuplicateModel(m_goalHandle));
	goal->SetScale(VGet(10.0f, 10.0f, 10.0f));
	goal->SetPos(Vector3{ 23600.0f,12000.0f,10000.0f });
	m_actors.push_back(goal);
}


GameScene::~GameScene()
{
	//���f���̍폜
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_beeHandle);
	MV1DeleteModel(m_crabHandle);
	MV1DeleteModel(m_groundOpenHandle);
	MV1DeleteModel(m_roadStraightHandle);
	MV1DeleteModel(m_roadCornerHandle);
	MV1DeleteModel(m_roadSlopeHandle);
	MV1DeleteModel(m_skyHandle);
	MV1DeleteModel(m_goalHandle);
	//�G�t�F�N�g�̍폜
	DeleteEffekseerEffect(LoadEffekseerEffect(kHitEffect));
	DeleteEffekseerEffect(LoadEffekseerEffect(kHomigEffect));
}

void GameScene::Update(Input& input)
{
	//��Ԃɉ���������
	(this->*m_update)(input);
}

void GameScene::Draw()
{
	//��Ԃɉ���������
	(this->*m_draw)();
}

void GameScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}

	//��̉�]
	m_skyAngle += 0.01f / 180.0f * DX_PI_F;
	MV1SetRotationXYZ(m_skyHandle, VGet(0.0f, m_skyAngle, 0.0f));

	//�I�u�W�F�N�g�̍X�V����
	for (auto& actor : m_actors)
	{
		actor->Update(input, m_camera);
	}

	//�g���K�[�̔���`�F�b�N
	TriggerCheck();

	//�����蔻��̃`�F�b�N
	CollisionCheck();

	//�J�������烌�C���΂��ăz�[�~���O�̃^�[�Q�b�g��T��
	m_camera->SearchHomingTarget(m_actors, m_collChecker);

	//�I�u�W�F�N�g�̈ړ�
	for (auto& actor : m_actors)
	{
		actor->CompleteMove();
	}

	//���S���̃I�u�W�F�N�g����X�R�A�����Z
	for (auto& actor : m_actors)
	{
		//�G�ȊO�Ȃ�X�L�b�v
		if (actor->GetCollidable()->GetObjectType() != ObjectType::Enemy)continue;
		//���S������Ȃ��Ȃ�X�L�b�v
		if (!std::dynamic_pointer_cast<EnemyBase>(actor)->IsDeading())continue;

		//�����܂ŗ����珈��
		//�X�R�A�����Z
		int score = std::dynamic_pointer_cast<EnemyBase>(actor)->GetScere();
		m_controller.GetScoreManager()->AddTotalScere(score);
		//�n���I������̂�0�ɂ���
		std::dynamic_pointer_cast<EnemyBase>(actor)->ResetScore();
	}

	//���񂾃I�u�W�F�N�g������
	//remove_if�ŏ����ɍ������̂�z��̌��ɓ��������̐擪�A�h���X��Ԃ�
	auto remIt = std::remove_if(
		m_actors.begin(), //�v�f�̓�
		m_actors.end(), //�v�f�̏I���
		//�����_��(�����֐�)
		[](const std::shared_ptr<Actor>& actor)->bool {
			return actor->IsDead();//����ł�I�u�W�F�N�g���폜
		});
	//��ŏ������Č��ɓ��������v�f�̐擪�A�h���X���Q�b�g�����̂ł��̗v�f����I���̗v�f�܂ł��폜����
	m_actors.erase(remIt, m_actors.end());


	//�J�����̍X�V����
	m_camera->Update(input);

	for (auto& actor : m_actors)
	{
		//�S�[�����Ă邩�`�F�b�N
		if (actor->GetTrigger()->GetTriggerType() == TriggerType::Goal)
		{
			if (std::dynamic_pointer_cast<Goal>(actor)->IsGoal())
			{
				m_update = &GameScene::FadeOutUpdate;
				m_draw = &GameScene::FadeDraw;
			}
			break;
		}
	}

	//UI�̍X�V
	m_uiManager->Update(m_actors, m_camera);

	//�X�R�A
	m_controller.GetScoreManager()->Update();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();
}

void GameScene::NormalDraw()
{
	MV1DrawModel(m_skyHandle);
#if _DEBUG

	DrawString(0, 0, "Game Scene", 0xffffff);

	for (float i = -kWorldSize / 2.0f;i <= kWorldSize / 2.0f;i += kSpaceInterval)
	{
		DrawLine3D(VGet(-kWorldSize / 2.0f, 0.0f, i), VGet(kWorldSize / 2.0f, 0.0f, i), 0xffffff);//X��
		DrawLine3D(VGet(i, 0.0f, -kWorldSize / 2.0f), VGet(i, 0.0f, kWorldSize / 2.0f), 0xffffff);//Z��
	}
	DrawLine3D(VGet(-kWorldSize / 2.0f, 0.0f, 0.0f), VGet(kWorldSize / 2.0f, 0.0f, 0.0f), 0x0000ff);//X��
	DrawLine3D(VGet(0.0f, -kWorldSize / 2.0f, 0.0f), VGet(0.0f, kWorldSize / 2.0f, 0.0f), 0xff0000);//Y��
	DrawLine3D(VGet(0.0f, 0.0f, -kWorldSize / 2.0f), VGet(0.0f, 0.0f, kWorldSize / 2.0f), 0x00ff00);//Z��
#endif
	//�I�u�W�F�N�g�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}

	//UI�̕`��
	m_uiManager->Draw();

	//�X�R�A
	m_controller.GetScoreManager()->Draw(kScorePosX, kScorePosY);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}

void GameScene::FadeInUpdate(Input& input)
{
	//��̉�]
	m_skyAngle += 0.01f / 180.0f * DX_PI_F;
	MV1SetRotationXYZ(m_skyHandle, VGet(0.0f, m_skyAngle, 0.0f));

	//�I�u�W�F�N�g�̍X�V����
	for (auto& actor : m_actors)
	{
		actor->Update(input, m_camera);
	}

	//�����蔻��̃`�F�b�N
	CollisionCheck();

	//�I�u�W�F�N�g�̈ړ�
	for (auto& actor : m_actors)
	{
		actor->CompleteMove();
	}

	//�J�����̍X�V����
	m_camera->Update(input);

	if (m_fadeAlpha <= 0)
	{
		m_update = &GameScene::NormalUpdate;
		m_draw = &GameScene::NormalDraw;
		return;
	}
	m_fadeAlpha -= kAddAlpha;

	//UI�̍X�V
	m_uiManager->Update(m_actors, m_camera);

	//�X�R�A
	m_controller.GetScoreManager()->Update();
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (m_fadeAlpha >= kMaxAlpha)
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	m_fadeAlpha += kAddAlpha;
	//UI�̍X�V
	m_uiManager->Update(m_actors, m_camera);

	//�X�R�A
	m_controller.GetScoreManager()->Update();
}

void GameScene::FadeDraw()
{
	MV1DrawModel(m_skyHandle);

	//�I�u�W�F�N�g�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}

	//UI�̕`��
	m_uiManager->Draw();

	//�X�R�A
	m_controller.GetScoreManager()->Draw(kScorePosX, kScorePosY);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

//�g���K�[
void GameScene::TriggerCheck()
{
	//�����������`�F�b�N����I�u�W�F�N�g
	for (auto& actorA : m_actors)
	{
		//���񂾃I�u�W�F�N�g�Ȃ�X�L�b�v
		if (actorA->IsDead())continue;
		//�����Ȃ��Ȃ�X�L�b�v
		if (actorA->GetCollidable()->GetObjectType() == ObjectType::None)continue;

		//�g���K�[
		for (auto& actorB : m_actors)
		{
			//���񂾃I�u�W�F�N�g�Ȃ�X�L�b�v
			if (actorB->IsDead())continue;
			//�����I�u�W�F�N�g�^�C�v�Ȃ画������Ȃ�
			if (actorA->GetCollidable()->GetObjectType() == actorB->GetCollidable()->GetObjectType())continue;
			//�g���K�[�̓����蔻��̏����擾
			auto trigger = actorB->GetTrigger();
			//�Ȃɂ��Ȃ��Ȃ�X�L�b�v
			if (trigger->GetTriggerType() == TriggerType::None)continue;

			//�������Ă���Ȃ�true;
			bool isHit = false;

			//���Ƃ̓����蔻��
			if (actorA->GetCollidable()->GetCollider()->GetCollKind() == ColliderKind::Sphere)
			{
				//�������Ă���Ȃ�true
				isHit = m_collChecker->CheckSCollTrigger(actorA->GetCollidable(), actorB->GetTrigger());
			}
			//�J�v�Z��
			else if (actorA->GetCollidable()->GetCollider()->GetCollKind() == ColliderKind::Capsule)
			{
				//�������Ă���Ȃ�true
				isHit = m_collChecker->CheckCCollTrigger(actorA->GetCollidable(), actorB->GetTrigger());
			}
			//�|���S��
			else if (actorA->GetCollidable()->GetCollider()->GetCollKind() == ColliderKind::Polygon)
			{
				//�������Ă���Ȃ�true
				isHit = m_collChecker->CheckPCollTrigger(actorA->GetCollidable(), actorB->GetTrigger());
			}

			//�������Ă���Ȃ�
			if (isHit)
			{
				actorB->OnHitTrigger(actorA);
				actorA->BeHitTrigger(actorB);
			}
		}
	}
}

//�Փˏ���
void GameScene::CollisionCheck()
{
	for (auto& actorA : m_actors)
	{
		//���񂾃I�u�W�F�N�g�Ȃ�X�L�b�v
		if (actorA->IsDead())continue;
		//�����蔻��̏����擾
		auto& collA = actorA->GetCollidable();

		//�����Ȃ��Ȃ�X�L�b�v
		if (collA->GetObjectType() == ObjectType::None)continue;


		auto kindA = collA->GetCollider()->GetCollKind();

		for (auto& actorB : m_actors)
		{
			//���񂾃I�u�W�F�N�g�Ȃ�X�L�b�v
			if (actorB->IsDead())continue;
			//�����I�u�W�F�N�g��2��ȏ㓖���蔻��̃`�F�b�N���s���̂�h��
			if (actorB <= actorA)continue;
			//�����蔻��̏����擾
			auto& collB = actorB->GetCollidable();
			//�Ȃɂ��Ȃ��Ȃ�X�L�b�v
			if (collB->GetObjectType() == ObjectType::None)continue;

			//�������Ă���Ȃ�true;
			bool isHit = false;
			//�����蔻��̎��
			auto kindB = collB->GetCollider()->GetCollKind();

			//��
			if (kindA == ColliderKind::Sphere)
			{
				//���Ƌ��̓����蔻��
				if (kindB == ColliderKind::Sphere)
				{
					//�������Ă���Ȃ�true
					isHit = m_collChecker->CheckCollSS(collA, collB);
					if (isHit)
					{
						//�Փˏ���
						m_collProcess->CollisionSS(collA, collB);
					}
				}
				//���ƃJ�v�Z��
				else if (kindB == ColliderKind::Capsule)
				{
					//�������Ă���Ȃ�true
					isHit = m_collChecker->CheckCollCS(collB, collA);//�����J�v�Z���ŉE����
					if (isHit)
					{
						//�Փˏ���
						m_collProcess->CollisionSC(collA, collB);
					}
				}
				//���ƃ|���S��
				else if (kindB == ColliderKind::Polygon)
				{
					//�������Ă���Ȃ�true
					isHit = m_collChecker->CheckCollSP(collA, collB);//�������ŉE���|���S��
					if (isHit)
					{
						//�Փˏ���
						m_collProcess->CollisionSP(collA, collB);
					}
				}
			}
			//�J�v�Z��
			else if (kindA == ColliderKind::Capsule)
			{
				//�J�v�Z���Ƌ��̓����蔻��
				if (kindB == ColliderKind::Sphere)
				{
					//�������Ă���Ȃ�true
					isHit = m_collChecker->CheckCollCS(collA, collB);//�����J�v�Z���ŉE����
					if (isHit)
					{
						//�Փˏ���
						m_collProcess->CollisionSC(collB, collA);
					}
				}
				//�J�v�Z�����m
				else if (kindB == ColliderKind::Capsule)
				{
					//�������Ă���Ȃ�true
					isHit = m_collChecker->CheckCollCC(collA, collB);
				}
			}
			//�|���S��
			else if (kindA == ColliderKind::Polygon)
			{
				if (kindB == ColliderKind::Sphere)
				{
					//�������Ă���Ȃ�true
					isHit = m_collChecker->CheckCollSP(collB, collA);//�������ŉE���|���S��
					if (isHit)
					{
						//�Փˏ���
						m_collProcess->CollisionSP(collB, collA);
					}
				}
			}

			//���������Ȃ炻�ꂼ��̃C�x���g���N����
			if (isHit)
			{
				actorA->OnHitColl(actorB);
				actorB->OnHitColl(actorA);
			}
		}
	}
}
