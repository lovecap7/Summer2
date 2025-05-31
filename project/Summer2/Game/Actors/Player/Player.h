#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class Input;
class CameraBase;
class Camera;
class Model;
class AttackBase;
class MeleeAttack;
class AttackManager;
class PlayerStateBase;
class Player :
	public Actor, std::enable_shared_from_this<Player>
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//�X�V����
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
	//�d��
	void Gravity(const Vector3& gravity)override;
	//�Փ˃C�x���g�֐�
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override;
	//�q�b�g���A�N�V����
	void HitReaction() override;
	//�n�ʂɕt���Ă��邩
	bool IsGround() { return m_isGround; };
	void NoIsGround() { m_isGround = false; };//�n�ʂɕt���Ă��Ȃ��Ɣ��f�����Ƃ��ɌĂ�
	//���͒��̕����L�[
	Vector2 GetStickVec() { return m_stickVec; };
	//���f���N���X
	std::shared_ptr<Model> GetModel() const{ return m_model; };
private:
	//�v���C���[�̏��
	std::shared_ptr<PlayerStateBase> m_state;
	//���f��
	std::shared_ptr<Model> m_model;
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�n�ʂɕt���Ă��邩�ǂ���
	bool m_isGround;
private:
	//���ꔻ��̍X�V
	void UpdateHurtPoint();
};

