#pragma once
#include <memory>

enum class ActorKind
{
	None,
	Player,		//�v���C���[
	Enemy,		//�G
	Item,		//�A�C�e��
	Obstacle,	//��Q��
	Field,		//�t�B�[���h
};

class Input;
class Camera;
class Collidable;
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
class Actor abstract//new�ł��Ȃ��Ȃ�
{
protected:
	//���̃A�N�^�[�̎��
	ActorKind m_actorKind;
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
public:
	Actor(ActorKind kind);
	virtual ~Actor() {};
	//(�������z�֐�)
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s��
	/// </summary>
	/// <param name="">���̓I�u�W�F�N�g</param>
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera) abstract;
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
	const std::shared_ptr<Collidable>& GetCollidable() const { return m_collidable; }
};

