#include "CollisionProcess.h"
#include "../Collidable.h"
#include "../Rigidbody.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include <vector>

namespace
{
	//押し戻しの値に足して密着するのを防ぐ
	constexpr float kOverlapGap = 0.5f;
}

CollisionProcess::CollisionProcess() :
	m_wallNum(0),
	m_floorNum(0),
	m_wall{ nullptr },
	m_floor{ nullptr }
{
}

CollisionProcess::~CollisionProcess()
{
}

void CollisionProcess::ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//AからBへのベクトル
	Vector3 aToB = otherB->GetRb()->GetNextPos() - otherA->GetRb()->GetNextPos();
	//最短距離
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (otherA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider> (otherB->GetColl())->GetRadius();
	//どのくらい重ねっているか
	float overlap = shortDis - aToB.Magnitude();
	overlap += kOverlapGap;

	//動かす物体とそうじゃない物体とで処理を分ける
	if (otherA->IsStatic() && !otherB->IsStatic())
	{
		otherB->GetRb()->AddVec(aToB.Normalize() * overlap);
	}
	else if (otherB->IsStatic() && !otherA->IsStatic())
	{
		otherA->GetRb()->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		otherA->GetRb()->AddVec(aToB.Normalize() * -overlap / 2.0f);
		otherB->GetRb()->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void CollisionProcess::ProcessSP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//初期化
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsFloor(false);
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsWall(false);

	//壁と床とのフラグリセット
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->ResetHitFlag();
	
	//当たったポリゴンの情報
	auto hitDim = std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->GetHitDim();

	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic())
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		DxLib::MV1CollResultPolyDimTerminate(hitDim);
		return;
	}

	//球の座標
	Position3 nextPos = otherA->GetRb()->GetNextPos();//移動後

	//床ポリゴンと壁ポリゴンに分ける
	AnalyzeWallAndFloor(hitDim, nextPos);
	//床と当たったなら
	if (m_floorNum > 0)
	{
		//補正するベクトルを返す
		Vector3 overlapVec = OverlapVecSphereAndPoly(m_floorNum, nextPos, *m_floor, std::dynamic_pointer_cast<SphereCollider> (otherA->GetColl())->GetRadius());

		//ポリゴンは固定(static)なので球のみ動かす
		otherA->GetRb()->AddVec(overlapVec);

		//修正方向が上向きなら床
		if (overlapVec.y > 0)
		{
			//床に当たっているので
			std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsFloor(true);
		}
	}

	//壁と当たっているなら
	if(m_wallNum > 0)
	{
		//壁に当たっているので
		std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsWall(true);

		//補正するベクトルを返す
		Vector3 overlapVec = OverlapVecSphereAndPoly(m_wallNum, nextPos, *m_wall, std::dynamic_pointer_cast<SphereCollider> (otherA->GetColl())->GetRadius());

		//ポリゴンは固定(static)なので球のみ動かす
		otherA->GetRb()->AddVec(overlapVec);
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	DxLib::MV1CollResultPolyDimTerminate(hitDim);
}

void CollisionProcess::ProcessCC(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//カプセルの押し戻しはそれぞれの当たったポイントから計算します

	//AからBへのベクトル
	Vector3 aToB = std::dynamic_pointer_cast<CapsuleCollider>(otherB->GetColl())->GetNearPos() - std::dynamic_pointer_cast<CapsuleCollider>(otherA->GetColl())->GetNearPos();
	//最短距離
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider> (otherA->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider> (otherB->GetColl())->GetRadius();
	//どのくらい重ねっているか
	float overlap = shortDis - aToB.Magnitude();
	overlap += kOverlapGap;

	//動かす物体とそうじゃない物体とで処理を分ける
	if (otherA->IsStatic() && !otherB->IsStatic())
	{
		otherB->GetRb()->AddVec(aToB.Normalize() * overlap);
	}
	else if (otherB->IsStatic() && !otherA->IsStatic())
	{
		otherA->GetRb()->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		otherA->GetRb()->AddVec(aToB.Normalize() * -overlap / 2.0f);
		otherB->GetRb()->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void CollisionProcess::ProcessCS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//AからBへのベクトル
	Vector3 aToB = otherB->GetRb()->GetNextPos() - std::dynamic_pointer_cast<CapsuleCollider>(otherA->GetColl())->GetNearPos();

	//最短距離
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (otherB->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider> (otherA->GetColl())->GetRadius();
	//どのくらい重ねっているか
	float overlap = shortDis - aToB.Magnitude();
	overlap += kOverlapGap;

	//動かす物体とそうじゃない物体とで処理を分ける
	if (otherA->IsStatic() && !otherB->IsStatic())
	{
		otherB->GetRb()->AddVec(aToB.Normalize() * overlap);
	}
	else if (otherB->IsStatic() && !otherA->IsStatic())
	{
		otherA->GetRb()->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		otherA->GetRb()->AddVec(aToB.Normalize() * -overlap / 2.0f);
		otherB->GetRb()->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void CollisionProcess::ProcessCP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//初期化
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsFloor(false);
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsWall(false);

	//壁と床とのフラグリセット
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->ResetHitFlag();

	//当たったポリゴンの情報
	auto hitDim = std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->GetHitDim();
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic())
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		DxLib::MV1CollResultPolyDimTerminate(hitDim);
		return;
	}

	//カプセルの頭座標と足座標
	Position3 headPos = std::dynamic_pointer_cast<CapsuleCollider>(otherA->GetColl())->GetNextEndPos(otherA->GetRb()->GetVec());//移動後
	Position3 legPos = otherA->GetRb()->GetNextPos();//移動後
	//頭より足のほうが低い位置にあるなら入れ替える
	if (headPos.y < legPos.y)
	{
		Position3 temp = legPos;
		legPos = headPos;
		headPos = temp;
	}

	//床ポリゴンと壁ポリゴンに分ける
	AnalyzeWallAndFloor(hitDim, legPos);
	//床と当たったなら
	if (m_floorNum > 0)
	{
		Vector3 overlapVec = OverlapVecCapsuleAndPoly(m_floorNum, headPos, legPos, *m_floor, std::dynamic_pointer_cast<CapsuleCollider> (otherA->GetColl())->GetRadius());

		//ポリゴンは固定(static)なので球のみ動かす
		otherA->GetRb()->AddVec(overlapVec);

		//修正方向が上向きなら床
		if (overlapVec.y > 0)
		{
			//床に当たっているので
			std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsFloor(true);
		}
	}

	//壁と当たっているなら
	if (m_wallNum > 0)
	{
		//壁に当たっているので
		std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->SetIsWall(true);

		//補正するベクトルを返す
		Vector3 overlapVec = OverlapVecCapsuleAndPoly(m_wallNum, headPos, legPos, *m_wall, std::dynamic_pointer_cast<CapsuleCollider> (otherA->GetColl())->GetRadius());

		//ポリゴンは固定(static)なので球のみ動かす
		otherA->GetRb()->AddVec(overlapVec);
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	DxLib::MV1CollResultPolyDimTerminate(hitDim);

}

void CollisionProcess::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const Vector3& nextPos)
{
	//壁ポリゴンと床ポリゴンの数を初期化する
	m_wallNum = 0;
	m_floorNum = 0;

	//検出されたポリゴンの数だけ繰り返す
	for (int i = 0; i < hitDim.HitNum;++i)
	{
		//XZ平面に垂直かどうかはポリゴンの法線のY成分が0に近いかどうかで判断する
		if (hitDim.Dim[i].Normal.y < 0.0001f && hitDim.Dim[i].Normal.y > -0.0001f)
		{
			//壁ポリゴンと判断された場合でも、プレイヤーのY座標＋1.0fより高いポリゴンのみ当たり判定を行う
			//段さで突っかかるのを防ぐため
			if (hitDim.Dim[i].Position[0].y > nextPos.y + 1.0f ||
				hitDim.Dim[i].Position[1].y > nextPos.y + 1.0f ||
				hitDim.Dim[i].Position[2].y > nextPos.y + 1.0f)
			{
				//ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に保存する
				if (m_wallNum < kMaxHitPolygon)
				{
					//ポリゴンの構造体のアドレスを壁ポリゴン配列に保存
					m_wall[m_wallNum] = &hitDim.Dim[i];
					++m_wallNum;
				}
			}
		}
		//床ポリゴン
		else
		{
			//ポリゴンの数が列挙できる限界数に達していなかったらポリゴン配列に保存
			if (m_floorNum < kMaxHitPolygon)
			{
				//ポリゴンの構造体のアドレスを床ポリゴン配列に保存
				m_floor[m_floorNum] = &hitDim.Dim[i];
				++m_floorNum;
			}
		}
	}
}

Vector3 CollisionProcess::OverlapVecSphereAndPoly(int hitNum ,const Vector3& nextPos,MV1_COLL_RESULT_POLY* dim ,float shortDis)
{
	//垂線を下して近い点を探して最短距離を求める
	float hitShortDis = 0;//最短距離
	//法線
	Vector3 nom = {};
	for (int i = 0; i < hitNum; ++i)
	{
		//内積と法線ベクトルから当たってる座標を求める
		VECTOR bToA = VSub(nextPos.ToDxLibVector(), dim[i].Position[0]);
		float dot = VDot(dim[i].Normal, bToA);
		//ポリゴンと当たったオブジェクトが法線方向にいるなら向きを反転
		if ((bToA.y > 0 && dim[i].Normal.y > 0) || (bToA.y < 0 && dim[i].Normal.y < 0))
		{
			dot *= -1;
		}
		//当たった座標
		VECTOR hitPos = VAdd(VScale(dim[i].Normal, dot), nextPos.ToDxLibVector());
		//距離
		float dis = VSize(VSub(hitPos, nextPos.ToDxLibVector()));
		//初回または前回より距離が短いなら
		if (i <= 0 || hitShortDis > dis)
		{
			//現状の最短
			hitShortDis = dis;
			//法線
			nom = Vector3{ dim[i].Normal.x,dim[i].Normal.y ,dim[i].Normal.z };
		}
	}
	//押し戻し
	//どれくらい押し戻すか
	float overlap = shortDis - hitShortDis;
	overlap += kOverlapGap;

	return nom * overlap;
}

Vector3 CollisionProcess::OverlapVecCapsuleAndPoly(int hitNum, const Vector3& headPos, const Vector3& legPos,MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//垂線を下して近い点を探して最短距離を求める
	float hitShortDis = 0;//最短距離
	//法線
	Vector3 nom = {};
	for (int i = 0; i < hitNum; ++i)
	{
		//最短距離の2乗を返す
		float dis = Segment_Triangle_MinLength_Square(headPos.ToDxLibVector(), legPos.ToDxLibVector(), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);
		//平方根を返す
		dis = sqrtf(dis);
		//初回または前回より距離が短いなら
		if (i <= 0 || hitShortDis > dis)
		{
			//現状の最短
			hitShortDis = dis;
			//法線
			nom = Vector3{ dim[i].Normal.x,dim[i].Normal.y ,dim[i].Normal.z };
		}
	}
	//押し戻し
	//どれくらい押し戻すか
	float overlap = shortDis - hitShortDis;
	overlap += kOverlapGap;

	return nom * overlap;
}
