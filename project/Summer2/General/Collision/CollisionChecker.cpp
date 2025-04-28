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


bool CollisionChecker::CheckCollSS(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//Aのコライダーの情報
	Vector3 posA = actorA->GetRb()->GetNextPos();
	float radiusA = std::dynamic_pointer_cast<SphereCollider>(actorA->GetColl())->GetRadius();
	//Bのコライダーの情報
	Vector3 posB = actorB->GetRb()->GetNextPos();
	float radiusB = std::dynamic_pointer_cast<SphereCollider>(actorB->GetColl())->GetRadius();

	Vector3 aToB = posB - posA;

	//２物体間の長さが半径の合計より大きいなら当たっていない
	if (aToB.Magnitude() > radiusA + radiusB)
	{
		return false;
	}

	//ここまで来たら当たっている
	return true;
}

bool CollisionChecker::CheckCollCS(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//球からカプセルに垂線を引いて球とカプセルの最短距離を求める

	//カプセルのそれぞれの座標
	Vector3 cPosA = actorA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetEndPos() + actorA->GetRb()->GetVec();
	//球の座標
	Vector3 sPos = actorB->GetRb()->GetNextPos();
	//最短距離
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider>(actorB->GetColl())->GetRadius();

	//カプセルの座標Aから球へのベクトル
	Vector3 AtoS = sPos - cPosA;
	//カプセルの座標AからBへの単位ベクトル
	Vector3 AtoB = cPosB - cPosA;
	Vector3 nomAtoB = AtoB.Normalize();

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
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(cPosB);

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
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(cPosA);

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
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(verPos);

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

bool CollisionChecker::CheckCollCC(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	// どちらかのカプセルの一つの座標から片方のカプセルに垂線を引いて
	//　お互いのカプセルに最も近い座標をそれぞれ出す

	//カプセルのそれぞれの座標
	//カプセルA
	Vector3 cPosA = actorA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetNextEndPos(actorA->GetRb()->GetVec());
	//カプセルB
	Vector3 cPosC = actorB->GetRb()->GetNextPos();
	Vector3 cPosD = std::dynamic_pointer_cast<CapsuleCollider>(actorB->GetColl())->GetNextEndPos(actorB->GetRb()->GetVec());
	//最短距離
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider>(actorB->GetColl())->GetRadius();
	//今の最短距離
	Vector3 nowShortDis = {};

	for (int i = 0; i < 2; ++i)
	{
		//線分のそれぞれの座標
		Vector3 lineA;
		Vector3 lineB;
		//最初にカプセルBに対してカプセルAのそれぞれの点から最短の座標を出す
		if (i <= 0)
		{
			//線分CD
			lineA = cPosC;
			lineB = cPosD;
		}
		//次ににカプセルAに対してカプセルBのそれぞれの点から最短の座標を出す
		else
		{
			//線分AB
			lineA = cPosA;
			lineB = cPosB;
		}

		for (int j = 0; j < 2; ++j)
		{
			//確認する座標
			Vector3 checkPoint;
			//Aから線分に下ろす
			if (i <= 0 && j <= 0)
			{
				checkPoint = cPosA;
			}
			//Bから線分に下ろす
			else if (i <= 0 && j >= 1)
			{
				checkPoint = cPosB;
			}
			//Cから線分に下ろす
			else if (i >= 1 && j <= 0)
			{
				checkPoint = cPosC;
			}
			//Dから線分に下ろす
			else
			{
				checkPoint = cPosD;
			}

			//あとは球とカプセルの時と同じ
			//Aから確認する座標へのベクトル
			Vector3 lineAtoP = checkPoint - lineA;
			//線分ABの単位ベクトル
			Vector3 lineAB = lineB - lineA;
			Vector3 nomLineAB = lineAB.Normalize();

			//それぞれのベクトルから内積をだして球から垂線を下した位置を求める
			float dotVer = nomLineAB.Dot(lineAtoP);
			Vector3 verPos = lineA + (nomLineAB * dotVer);//垂線を下した座標

			//垂線を下した座標が線分AB間の間にあるか外にあるかをチェックする
			float dotAB = nomLineAB.Dot(lineAB);//全体
			float rate = dotVer / dotAB;//割合

			//最短距離候補を入れていく
			Vector3 shortCandidate;
			//衝突判定に使うので最短座標を入れる
			Vector3 shortPos;
			if (rate >= 1.0f)
			{
				//座標Bに近い
				shortCandidate = lineB - checkPoint;
				shortPos = lineB;
			}
			else if (rate <= 0.0f)
			{
				//座標Aに近い
				shortCandidate = lineA - checkPoint;
				shortPos = lineA;
			}
			else
			{
				//垂線を下した座標に近い
				shortCandidate = verPos - checkPoint;
				shortPos = verPos;
			}

			//初回または前回の最短距離より小さいなら現在の最短距離とする
			if (nowShortDis.Magnitude() <= 0 || nowShortDis.Magnitude() > shortCandidate.Magnitude())
			{
				nowShortDis = shortCandidate;

				//最短座標を記録
				if (i == 0)
				{
					//衝突判定で使うので一番近い座標を覚えておく
					std::dynamic_pointer_cast<CapsuleCollider>(actorB->GetColl())->SetNearPos(shortPos);
				}
				else
				{
					//衝突判定で使うので一番近い座標を覚えておく
					std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(shortPos);
				}
			}
		}
	}

	//現在の最短距離が変形の合計より大きいなら当たっていない
	if (nowShortDis.Magnitude() > shortDis)
	{
		return false;
	}

	//ここまでくれば当たっている
	return true;
}

bool CollisionChecker::CheckCollSP(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//当たってるポリゴンの数
	auto hitDim = MV1CollCheck_Sphere(
		std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->GetModelHandle(),
		-1,
		actorA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(actorA->GetColl())->GetRadius());
	//一つも当たっていないならfalse
	if (hitDim.HitNum <= 0)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//当たり判定に使うので保存
	std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::CheckCollCP(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//当たってるポリゴンの数
	auto hitDim = MV1CollCheck_Capsule(
		std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->GetModelHandle(),
		-1,
		actorA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetNextEndPos(actorA->GetRb()->GetVec()).ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetRadius(),
		-1);
	//当たっていないならfalse
	if (!hitDim.Dim->HitFlag)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//当たり判定に使うので保存
	std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->SetHitDim(hitDim);

	return true;
}
