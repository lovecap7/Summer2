#pragma once

/// <summary>
/// �����蔻��̌`�ɂ���ď������ς��̂�
/// ���݂����ǂ̌`�̓����蔻������Ă���̂���
/// ��Ƀ`�F�b�N����ۂɎg��
/// </summary>
enum class ColliderKind
{
	Sphere,
	Capsule,
	Box,
	Polygon
};

/// <summary>
/// �����蔻��̌`�Ɋւ�����N���X
/// </summary>
class ColliderBase abstract
{
public:
	ColliderBase(ColliderKind kind);
	virtual ~ColliderBase() {};

	/// <summary>
	/// �����蔻��̌`���擾
	/// </summary>
	/// <returns>�����蔻��̌`</returns>
	ColliderKind GetCollKind()const { return m_kind; }
private:
	//�����蔻��̌`
	ColliderKind m_kind;
};

