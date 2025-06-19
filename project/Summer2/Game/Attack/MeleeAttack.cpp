#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Rigidbody.h"
#include "../Actors/Player/Player.h"
#include "../Actors/Player/UltGage.h"

MeleeAttack::MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner) :
	AttackBase(coll, damage, keepFrame, knockBackPower, owner)
{
}

MeleeAttack::~MeleeAttack()
{
}

void MeleeAttack::Init()
{
	//初期化処理
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDelete = false;
	//IDの初期化
	if (m_hitId.size() != 0)
	{
		m_hitId.clear();
	}
}

void MeleeAttack::Update()
{
	//持続フレームを減らす
	m_keepFrame--;
	//持続フレームが0になったら消滅
	if (m_keepFrame <= 0)
	{
		m_isDelete = true;
	}
}

void MeleeAttack::Draw()
{
#if _DEBUG
	if (m_collidable->GetColl()->GetShape() == Shape::Capsule)
	{
		DrawCapsule3D(
			m_collidable->GetRb()->GetPos().ToDxLibVector(),
			std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
			std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
			32,
			0xff00ff,
			0xff00ff,
			false
		);
	}
	else if(m_collidable->GetColl()->GetShape() == Shape::Sphere)
	{
		DrawSphere3D(
			m_collidable->GetRb()->GetPos().ToDxLibVector(),
			std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
			32,
			0xff00ff,
			0xff00ff,
			false
		);
	}
	
#endif
}

void MeleeAttack::OnHit(std::shared_ptr<Actor> actor)
{
	auto ownerColl = m_owner->GetCollidable();
	auto otherColl = actor->GetCollidable();

	//自分と同じ種類なら無視
	if (ownerColl->GetGameTag() == otherColl->GetGameTag())return;

	bool isFind = false;
	//IDがすでに記録されているか確認
	for (auto id : m_hitId)
	{
		if (id == actor->GetID())
		{
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		//記録されていなければ記録する
		m_hitId.emplace_back(actor->GetID());
		//ダメージを与える
		actor->GetHurtPoint()->OnHitDamage(m_damage);
		//ノックバック
		Vector3 knockBackVec = actor->GetCollidable()->GetRb()->GetNextPos() - m_owner->GetCollidable()->GetRb()->GetNextPos();//離れるベクトル
		knockBackVec.y = 0.0f;//Y成分はなし
		knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//ノックバック
		actor->GetHurtPoint()->OnHitKnockBack(knockBackVec);

		//プレイヤーの攻撃の場合必殺技ゲージを加算する
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			auto gage = std::dynamic_pointer_cast<Player>(m_owner)->GetUltGage();
			gage->AddPedingUltGage();//予約されていた加算ゲージ量を反映
		}
	}
}
