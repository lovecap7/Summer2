#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"

enum class ActorKind
{
	None,
	Player,		//�v���C���[
	Enemy,		//�G
	Item,		//�A�C�e��
	Obstacle,	//��Q��
	Field,		//�t�B�[���h
};

class ActorManager;
class Input;
class Camera;
class Collidable;
class HurtPoint;
class AttackManager;
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
/// //new�ł��Ȃ��Ȃ�
class Actor abstract:public std::enable_shared_from_this<Actor>
{
protected:
	//���̃A�N�^�[�̎��
	ActorKind m_actorKind;
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//���ꔻ��
	std::shared_ptr<HurtPoint> m_hurtPoint;
	//����
	bool m_isDead;
	//�A�N�^�[�̎��ʔԍ�
	int m_id;
public:
	Actor(ActorKind kind);
	virtual ~Actor() {};
	//(�������z�֐�)
	/// <summary>
	/// �o�^����
	/// </summary>
	virtual void Entry(std::shared_ptr<ActorManager> actorManager);
	/// <summary>
	/// �o�^����
	/// </summary>
	/// <param name="actorManager"></param>
	virtual void Exit(std::shared_ptr<ActorManager> actorManager);
	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s��
	/// </summary>
	/// <param name="">���̓I�u�W�F�N�g</param>
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) abstract;
	/// <summary>
	/// �d�͂��󂯂�
	/// </summary>
	/// <param name="gravity"></param>
	virtual void Gravity(const Vector3& gravity)abstract;
	/// <summary>
	/// �Փ˂����Ƃ��̃C�x���g�֐�
	/// </summary>
	/// <param name="other"></param>
	virtual void OnHitColl(const std::shared_ptr<Collidable>& other)abstract;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\��
	/// </summary>
	virtual void Draw()const abstract;
	/// <summary>
	/// Update�̏����ɂ���ċN�������Փˏ����Ȃǂ̏����̊m��
	/// </summary>
	virtual void Complete() abstract;
	/// <summary>
	/// �A�N�^�[�̎��
	/// </summary>
	/// <returns></returns>
	ActorKind GetActorKind() { return m_actorKind; };
	/// <summary>
	/// �����蔻��ƏՓˏ����Ɏg��
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Collidable> GetCollidable() const { return m_collidable; }
	/// <summary>
	/// ���ꔻ��
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<HurtPoint> GetHurtPoint() const { return m_hurtPoint; }
	/// <summary>
	/// ���Ńt���O
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return m_isDead; };
	/// <summary>
	/// ID���擾
	/// </summary>
	/// <returns></returns>
	int GetID() const { return m_id; };
	/// <summary>
	/// �U�����󂯂��Ƃ��̃��A�N�V����
	/// </summary>
	virtual void HitReaction() abstract;
};

