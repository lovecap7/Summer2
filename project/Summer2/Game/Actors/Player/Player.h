#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Input;
class Camera;
class Model;
class AttackBase;
class MeleeAttack;
class AttackManager;
class UIManager;
class UltGage;
class PlayerStateBase;
class Player :
	public Actor, public std::enable_shared_from_this<Player>
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//�o�^����
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//�o�^����
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//����������
	void Init()override;
	//�X�V����
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, std::shared_ptr<AttackManager> attackManager) override;
	//�d��
	void Gravity(const Vector3& gravity)override;
	//�Փ˃C�x���g�֐�
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override;
	//�n�ʂɕt���Ă��邩
	bool IsGround() { return m_isGround; };
	void NoIsGround() { m_isGround = false; };//�n�ʂɕt���Ă��Ȃ��Ɣ��f�����Ƃ��ɌĂ�
	//���͒��̕����L�[
	Vector2 GetStickVec() { return m_stickVec; };
	//���f���N���X
	std::shared_ptr<Model> GetModel() const{ return m_model; };
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//�G�����G
	void OnHitSearchTarget(const Vector3& playerPos);
private:
	//�v���C���[�̏��
	std::shared_ptr<PlayerStateBase> m_state;
	//���f��
	std::shared_ptr<Model> m_model;
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�n�ʂɕt���Ă��邩�ǂ���
	bool m_isGround;
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> m_ultGage;
	//�O���Ƀ^�[�Q�b�g(�G�Ƃ�)�����邩���`�F�b�N���邽�߂̃g���K�[
	std::shared_ptr<Collidable> m_searchTrigger;
	//�^�[�Q�b�g�̃|�W�V����
	Vector3 m_targetPos;
private:
	//���ꔻ��̍X�V
	void UpdateHurtPoint();
};

