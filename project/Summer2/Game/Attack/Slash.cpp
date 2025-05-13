#include "Slash.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include "../Actors/Enemy/EnemyBase.h"
#include <DxLib.h>

#if _DEBUG
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"
#endif

Slash::Slash(std::shared_ptr<Collidable> coll, float damege, int keepFrame) :
	AttackBase(coll, damege, keepFrame),
	m_initDamege(damege),
	m_initKeepFrame(keepFrame)
{
}

Slash::~Slash()
{
	
}

void Slash::Init()
{
	//‰Šú‰»ˆ—
	m_damege = m_initDamege;
	m_keepFrame = m_initKeepFrame;
	m_isDead = false;
	//ID‚Ì‰Šú‰»
	if (m_hitEnemyId.size() != 0)
	{
		m_hitEnemyId.clear();
	}
}

void Slash::Update()
{
	//‘±ƒtƒŒ[ƒ€‚ğŒ¸‚ç‚·
	m_keepFrame--;
	//‘±ƒtƒŒ[ƒ€‚ª0‚É‚È‚Á‚½‚çÁ–Å
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void Slash::Draw()
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		32,
		0xff00ff,
		0xff00ff,
		true//–³“G‚Ì‚Í“h‚è‚Â‚Ô‚³‚ê‚é
	);
#endif
}

void Slash::OnHit(std::shared_ptr<Actor> actor)
{
	//“G‚É“–‚½‚Á‚½ê‡ID‚ğ‹L˜^‚·‚é
	if (actor->GetActorKind() == ActorKind::Enemy)
	{
		bool isFind = false;
		//“G‚ÌID‚ª‚·‚Å‚É‹L˜^‚³‚ê‚Ä‚¢‚é‚©Šm”F
		for (auto id : m_hitEnemyId)
		{
			if (id == std::dynamic_pointer_cast<EnemyBase>(actor)->GetID())
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			//‹L˜^‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î‹L˜^‚·‚é
			m_hitEnemyId.emplace_back(std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
			//ƒ_ƒ[ƒW‚ğ—^‚¦‚é
			actor->GetHurtPoint()->OnHitDamage(m_damege);
#if _DEBUG
			//UŒ‚‚ğ“–‚Ä‚½‚±‚Æ‚ğ•ñ
			printfDx("UŒ‚‚ª“–‚½‚Á‚½ ID = %d\n", std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
#endif
		}
	}

#if _DEBUG
	printf("UŒ‚‚ª“–‚½‚Á‚½");
#endif
}
