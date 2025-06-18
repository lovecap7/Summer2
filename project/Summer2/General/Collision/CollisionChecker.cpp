#include "CollisionChecker.h"
#include "../Math/MyMath.h"
#include <DxLib.h>
#include "../Collidable.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "../Rigidbody.h"

CollisionChecker::CollisionChecker()
{
}


bool CollisionChecker::CheckCollSS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//Aのコライダーの情報
	Vector3 posA = collA->GetRb()->GetNextPos();
	float radiusA = std::dynamic_pointer_cast<SphereCollider>(collA->GetColl())->GetRadius();
	//Bのコライダーの情報
	Vector3 posB = collB->GetRb()->GetNextPos();
	float radiusB = std::dynamic_pointer_cast<SphereCollider>(collB->GetColl())->GetRadius();

	Vector3 aToB = posB - posA;

	//２物体間の長さが半径の合計より大きいなら当たっていない
	if (aToB.Magnitude() > radiusA + radiusB)
	{
		return false;
	}

	//ここまで来たら当たっている
	return true;
}

bool CollisionChecker::CheckCollCS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//球からカプセルに垂線を引いて球とカプセルの最短距離を求める

	//カプセルのそれぞれの座標
	Vector3 cPosA = collA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetEndPos() + collA->GetRb()->GetVec();
	//球の座標
	Vector3 sPos = collB->GetRb()->GetNextPos();
	//最短距離
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider>(collB->GetColl())->GetRadius();

	//カプセルの座標Aから球へのベクトル
	Vector3 AtoS = sPos - cPosA;
	//カプセルの座標AからBへの単位ベクトル
	Vector3 AtoB = cPosB - cPosA;
	Vector3 nomAtoB;
	if (AtoB.Magnitude() > 0.0f)
	{
		nomAtoB = AtoB.Normalize();
	}

	//それぞれのベクトルから内積をだして球から垂線を下した位置を求める
	float dotVer = nomAtoB.Dot(AtoS);
	Vector3 verPos = cPosA + (nomAtoB * dotVer);//垂線を下した座標

	//垂線を下した座標がカプセルのAB間の間にあるか外にあるかをチェックする
	float dotAB = nomAtoB.Dot(AtoB);//全体
	float rate = dotVer / dotAB;//割合

	//1より大きいならposBが球に一番近い座標
	if (rate >= 1.0f)
	{
		//衝突判定で使うので一番近い座標を覚えておく
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosB);

		//posBと球の間距離から当たっているかをチェック
		Vector3 BtoS = sPos - cPosB;
		//最短距離より大きいなら当たっていない
		if (BtoS.Magnitude() > shortDis)
		{
			return false;
		}
	}
	//0より小さいならposAが球に一番近い座標
	else if (rate <= 0.0f)
	{
		//衝突判定で使うので一番近い座標を覚えておく
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosA);

		//posAと球の間の距離から当たっているかチェック
		//最短距離より大きいなら当たっていない
		if (AtoS.Magnitude() > shortDis)
		{
			return false;
		}
	}
	//0~1の間なら垂線を下した座標が球に一番近い座標
	else
	{
		//衝突判定で使うので一番近い座標を覚えておく
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(verPos);

		//垂線を下した座標と球の間の距離から当たっているかチェック
		Vector3 VtoS = sPos - verPos;
		//最短距離より大きいなら当たっていない
		if (VtoS.Magnitude() > shortDis)
		{
			return false;
		}
	}

	//ここまでくれば当たっている
	return true;
}

//衝突判定のために最近接点の計算もしてる
bool CollisionChecker::CheckCollCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	// どちらかのカプセルの一つの座標から片方のカプセルに垂線を引いて
	//　お互いのカプセルに最も近い座標をそれぞれ出す

	//カプセルのそれぞれの座標
	//カプセルA
	Vector3 capAStartPos = collA->GetRb()->GetNextPos();
	Vector3 capAEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec());
	float capARadius = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius();
	//カプセルB
	Vector3 capBStartPos = collB->GetRb()->GetNextPos();
	Vector3 capBEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetNextEndPos(collB->GetRb()->GetVec());
	float capBRadius = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetRadius();
	
	//平行かどうか確認する
	auto capADir = capAEndPos - capAStartPos;//線分1
	auto capBDir = capBStartPos - capBEndPos;//線分2
	//正規化して内席から平行かチェック
	auto cross = capADir.Cross(capBDir);
	//平行なら
	if (cross.SqMagnitude() <= 0.0f)
	{
		return ParallelCC(collA, collB);
	}

	//最短距離
	float shortDis = capARadius + capBRadius;
	//今の最短距離
	float nowShortDis = 10000.0f;//始点同士を最短にしておく

	for (int i = 0; i < 2; ++i)
	{
		//線分のそれぞれの座標
		Vector3 lineStart;
		Vector3 lineEnd;
		//最初にカプセルBに対してカプセルAのそれぞれの点から最短の座標を出す
		if (i <= 0)
		{
			//線分CD
			lineStart = capBStartPos;
			lineEnd = capBEndPos;
		}
		//次ににカプセルAに対してカプセルBのそれぞれの点から最短の座標を出す
		else
		{
			//線分AB
			lineStart = capAStartPos;
			lineEnd = capAEndPos;
		}

		for (int j = 0; j < 2; ++j)
		{
			//確認する座標
			Vector3 checkPoint;
			//Aから線分に下ろす
			if (i == 0 && j == 0)checkPoint = capAStartPos;
			//Bから線分に下ろす
			if (i == 0 && j == 1)checkPoint = capAEndPos;
			//Cから線分に下ろす
			if (i == 1 && j == 0)checkPoint = capBStartPos;
			//Dから線分に下ろす
			if (i == 1 && j == 1)checkPoint = capBEndPos;

			//あとは球とカプセルの時と同じ
			//Aから確認する座標へのベクトル
			Vector3 lineAtoP = checkPoint - lineStart;
			//線分ABの単位ベクトル
			Vector3 lineAB = lineEnd - lineStart;
			Vector3 nomLineAB = lineAB.Normalize();

			//それぞれのベクトルから内積をだして球から垂線を下した位置を求める
			float dotVer = nomLineAB.Dot(lineAtoP);
			Vector3 verPos = lineStart + (nomLineAB * dotVer);//垂線を下した座標

			//垂線を下した座標が線分AB間の間にあるか外にあるかをチェックする
			float rate = dotVer / lineAB.Magnitude();//割合

			//最短距離候補を入れていく
			float shortCandidate;
			//衝突判定に使うので最短座標を入れる
			Vector3 shortPos;
			if (rate >= 1.0f)
			{
				//座標Bに近い
				shortCandidate = (lineEnd - checkPoint).Magnitude();
				shortPos = lineEnd;
			}
			else if (rate <= 0.0f)
			{
				//座標Aに近い
				shortCandidate = (lineStart - checkPoint).Magnitude();
				shortPos = lineStart;
			}
			else
			{
				//垂線を下した座標に近い
				shortCandidate = (verPos - checkPoint).Magnitude();
				shortPos = verPos;
			}

			//初回または前回の最短距離より小さいなら現在の最短距離とする
			if (nowShortDis > shortCandidate)
			{
				nowShortDis = shortCandidate;

				//最短座標を記録
				if (i == 0)
				{
					//衝突判定で使うので一番近い座標を覚えておく
					std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(shortPos);
				}
				else
				{
					//衝突判定で使うので一番近い座標を覚えておく
					std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(shortPos);
				}
			}
		}
	}

	//現在の最短距離が変形の合計より大きいなら当たっていない
	if (nowShortDis > shortDis)
	{
		return false;
	}


	//ここまでくれば当たっている
	return true;
}

bool CollisionChecker::CheckCollCCVerDxLib(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//カプセルのそれぞれの座標
	//カプセルA
	Vector3 capAStartPos = collA->GetRb()->GetNextPos();
	Vector3 capAEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec());
	float capARadius = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius();
	//カプセルB
	Vector3 capBStartPos = collB->GetRb()->GetNextPos();
	Vector3 capBEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetNextEndPos(collB->GetRb()->GetVec());
	float capBRadius = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetRadius();

	return HitCheck_Capsule_Capsule(capAStartPos.ToDxLibVector(), capAEndPos.ToDxLibVector(), capARadius,
		capBStartPos.ToDxLibVector(), capBEndPos.ToDxLibVector(), capBRadius);
}

bool CollisionChecker::CheckCollSP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//当たってるポリゴンの数
	auto hitDim = MV1CollCheck_Sphere(
		std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->GetModelHandle(),
		-1,
		collA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(collA->GetColl())->GetRadius());
	//一つも当たっていないならfalse
	if (hitDim.HitNum <= 0)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//当たり判定に使うので保存
	std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::CheckCollCP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//当たってるポリゴンの数
	auto hitDim = MV1CollCheck_Capsule(
		std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->GetModelHandle(),
		-1,
		collA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec()).ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius(),
		-1);

	//当たっていないならfalse
	if (hitDim.HitNum <= 0)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//当たり判定に使うので保存
	std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::ParallelCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//カプセルA
	Vector3 cPosA = collA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec());
	//カプセルB
	Vector3 cPosC = collB->GetRb()->GetNextPos();
	Vector3 cPosD = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetNextEndPos(collB->GetRb()->GetVec());
	//最短距離
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetRadius();
	
	//各距離をチェック
	Vector3 ac = cPosC - cPosA;
	Vector3 ad = cPosD - cPosA;
	Vector3 bc = cPosC - cPosB;
	Vector3 bd = cPosD - cPosB;
	//最短距離を出す
	float dis = ac.Magnitude();
	//一度入れておく
	std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosA);
	std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosC);
	//短いなら
	if (dis > ad.Magnitude())
	{
		dis = ad.Magnitude();
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosA);
		std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosD);
	}
	if (dis > bc.Magnitude())
	{
		dis = bc.Magnitude();
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosB);
		std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosC);
	}
	if (dis > bd.Magnitude())
	{
		dis = bd.Magnitude();
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosB);
		std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosD);
	}

	//最短距離より大きいなら当たっていない
	if (dis > shortDis)
	{
		return false;
	}

	//ここまで来たら当たってる
	return true;
}