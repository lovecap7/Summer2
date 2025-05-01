#include "CollisionManager.h"
#include "../../Game/Actors/Actor.h"
#include "CollisionChecker.h"
#include "CollisionProcess.h"
#include "../Rigidbody.h"
#include "ColliderBase.h"
#include "../Collidable.h"

namespace
{
	//�m�F��
	constexpr int kTryNum = 3;
}

CollisionManager::CollisionManager():
	m_collChecker(std::make_unique<CollisionChecker>()),
	m_collProcessor(std::make_unique<CollisionProcess>())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//�␳�������Ƃŕʂ̃I�u�W�F�N�g�ɓ�����\��������̂ň��񐔃`���b�N����
	for (int i = 0;i < kTryNum;++i)
	{
		//�����蔻����`�F�b�N
		for (auto& actorA : actors)
		{
			//�����蔻����s��Ȃ��Ȃ��΂�
			if (!actorA->GetCollidable()->IsCollide())continue;
			for (auto& actorB : actors)
			{
				//�����蔻����s��Ȃ��Ȃ��΂�
				if (!actorB->GetCollidable()->IsCollide())continue;
				//�����Ƃ͓����蔻������Ȃ�
				if (actorA == actorB)continue;

				//�������Ă邩���`�F�b�N
				bool isHit = false;

				//����
				if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					//��
					if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSS(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessSS(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
					//�J�v�Z��
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCS(actorB->GetCollidable(), actorA->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCS(actorB->GetCollidable(), actorA->GetCollidable());
						}
					}
					//�|���S��
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollSP(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessSP(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
				}
				//�J�v�Z����
				else if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					//��
					if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollCS(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCS(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
					//�J�v�Z��
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCC(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCC(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
					//�|���S��
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollCP(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCP(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
				}
				//�|���S����
				else if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
				{
					//��
					if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSP(actorB->GetCollidable(), actorA->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessSP(actorB->GetCollidable(), actorA->GetCollidable());
						}
					}
					//�J�v�Z��
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCP(actorB->GetCollidable(), actorA->GetCollidable());
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCP(actorB->GetCollidable(), actorA->GetCollidable());
						}
					}
				}

				//�������Ă�Ȃ�
				if (isHit)
				{
					//�����������̏���
					actorA->OnHitColl(actorB->GetCollidable());
					actorB->OnHitColl(actorA->GetCollidable());
				}
			}
		}
	}
}
