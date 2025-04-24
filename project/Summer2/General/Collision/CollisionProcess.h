#pragma once
#include <memory>
class Collidable;
class CollisionProcess
{
public:
	CollisionProcess();
	~CollisionProcess();
	/// <summary>
	/// ���Ƌ��̏Փˏ���
	/// </summary>
	/// <param name="otherA">��</param>
	/// <param name="otherB">��</param>
	void ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// ���ƃ|���S���̏Փˏ���
	/// </summary>
	/// <param name="otherA">��</param>
	/// <param name="otherB">�|���S��</param>
	void ProcessSP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փˏ���
	/// </summary>
	/// <param name="otherA">�J�v�Z��</param>
	/// <param name="otherB">�J�v�Z��</param>
	void ProcessCC(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// �J�v�Z���Ƌ�
	/// </summary>
	/// <param name="otherA">�J�v�Z��</param>
	/// <param name="otherB">��</param>
	void ProcessCS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// �J�v�Z���ƃ|���S���̏Փˏ���
	/// </summary>
	/// <param name="otherA">�J�v�Z��</param>
	/// <param name="otherB">�|���S��</param>
	void ProcessCP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
};

