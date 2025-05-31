#include "PlayerStateBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
PlayerStateBase::PlayerStateBase(std::shared_ptr<Player>  player):
	m_player(player)
{
}

PlayerStateBase::~PlayerStateBase()
{
}

void PlayerStateBase::ChangeState(std::shared_ptr<PlayerStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}

Vector3 PlayerStateBase::GetForwardVec(const Input& input, const std::unique_ptr<Camera>& camera)
{
	Vector3 rV = { 0.0f,0.0f,0.0f, };
	//����
	Vector2 stickVec = m_player->GetStickVec();
	if (stickVec.Magnitude() < 0.0f)
	{
		return rV;
	}
	//�J�����̌����ɂ��킹��
	//�J�����̌���
	Vector2 cameraDir{ camera->GetDir().x,camera->GetDir().z };
	cameraDir = cameraDir.Normalize();
	//���[���h���W��Z��������ɃJ�������ǂ̂��炢�����Ă���̂����v�Z
	Vector2 z = Vector2{ 0.0f, -1.0f };
	//�J�����̌���(�p�x)
	float cameraTheata = Theata(z, cameraDir);
	//��ɑ΂��ăX�e�B�b�N���ǂ̂��炢�����Ă���̂����v�Z
	float stickTheata = Theata(z, stickVec.Normalize());
	//�v���C���[�𒆐S�Ɏ��̍��W����]
	Quaternion rotaQ = Quaternion::AngleAxis(cameraTheata + stickTheata, Vector3::Up());
	//�x�N�g���ɂ�����(��])
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaQ * moveVec;
	moveVec.y = 0.0f; //Y���͖���
	rV = moveVec.Normalize();
	return rV;
}
void PlayerStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//�U��������
	attack->Init();//��Ԃ����Z�b�g
	attackManager->Entry(attack);//�o�^
}
