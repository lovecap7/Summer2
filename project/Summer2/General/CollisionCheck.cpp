#include "CollisionCheck.h"

CollisionCheck::CollisionCheck()
{
}

CollisionCheck::~CollisionCheck()
{
}


//円同士の当たり判定
bool CollisionCheck::IsCollCicCic(const Circle2D& c1, const Circle2D& c2)
{
	//それぞれの中心座標からの間のベクトルを求める
	Vector2 dist = c2.pos - c1.pos;
	//2物体の間の大きさがそれぞれの半径の足した値よりも大きいなら当たっていない
	if (dist.Magnitude() > c1.radius + c2.radius)
	{
		return false;
	}
	//ここまでくれば当たっている
	return true;
}

bool CollisionCheck::IsCollCicRec(const Circle2D& circle, const Rect2D& rect)
{
	//それぞれの中心座標からの間のベクトルを求める
	Vector2 dist = circle.pos - rect.pos;
	//矩形の中心から各頂点への長さを求める
	float hypot = Vector2{ (rect.w / 2.0f),(rect.h / 2.0f) }.Magnitude();
	//Y成分がそれぞれの半径の足した値よりも大きい、または
	//X成分がそれぞれの半径の足した値よりも大きい、または
	//ベクトルの大きさが矩形の中心から各頂点への長さと球の半径の大きさを
	//足した値よりも大きいなら当たっていない
	if (abs(dist.y) > circle.radius + (rect.h / 2.0f) ||
		abs(dist.x) > circle.radius + (rect.w / 2.0f) ||
		dist.Magnitude() > circle.radius + hypot)
	{
		return false;
	}

	//ここまでくれば当たっている
	return true;
}

bool CollisionCheck::IsCollCicCap(const Circle2D& circle, const Capsule2D& capsule)
{
	//球からカプセルに垂線を引いて球からカプセルへの最短距離を求める
	// カプセルのposA -> A
	// カプセルのposB -> B
	// 球の中心座標(pos) -> C
	// 球からカプセルに垂線を下してでた座標 -> V
	Vector2 A = capsule.posA;
	Vector2 B = capsule.posB;
	Vector2 C = circle.pos;


	//カプセルのAから球の中心座標へのベクトル
	Vector2 AtoC = C - A;
	//posAからposBへの単位ベクトル
	Vector2 AtoB = B - A;
	Vector2 nomAtoB = AtoB.Normalize();
	//内積の計算
	//Aから座標Vへの距離を出す
	float dotAtoV = nomAtoB.Dot(AtoC);
	//posAにposAからposBへの単位ベクトル * dotをして座標Vを出す
	Vector2 V = A + (nomAtoB * dotAtoV);


	//VがABの間にある座標かどうかをチェックする
	float dotAtoB = nomAtoB.Dot(AtoB);//全体
	float rate = (dotAtoV / dotAtoB);//割合

	//0より小さいならposAより外、1より大きいならposBより外、どちらでもないならAB間にある
	if (rate > 1.0f)
	{
		//最短距離はposBから球の間の距離()BtoC
		Vector2 BtoC = C - B;
		//球とカプセルの半径の合計より大きければ当たっていない
		if (BtoC.Magnitude() > circle.radius + capsule.radius)
		{
			return false;
		}
	}
	else if (rate < 0.0f)
	{
		//最短距離はposAから球の間の距離(AtoC)
		//球とカプセルの半径の合計より大きければ当たっていない
		if (AtoC.Magnitude() > circle.radius + capsule.radius)
		{
			return false;
		}
	}
	else
	{
		//最短距離は球からV(垂線を下してでた座標)の間の距離(VtoC)
		Vector2 VtoC = C - V;
		//球とカプセルの半径の合計より大きければ当たっていない
		if (VtoC.Magnitude() > circle.radius + capsule.radius)
		{
			return false;
		}
	}

	//ここまでくれば当たっている
	return true;
}

bool CollisionCheck::IsCollRecRec(const Rect2D& r1, const Rect2D& r2)
{
	//それぞれの中心座標からの間のベクトルを求める
	Vector2 dist = r2.pos - r1.pos;
	//X成分の大きさが矩形の幅の合計より大きいなら当たっていない、または
	//Y成分の大きさが矩形の高さの合計より大きいなら当たっていない
	if (abs(dist.x) > (r1.w / 2.0f) + (r2.w / 2.0f) ||
		abs(dist.y) > (r1.h / 2.0f) + (r2.h / 2.0f))
	{
		return false;
	}
	//ここまでくれば当たっている
	return true;
}

bool CollisionCheck::IsCollRecCap(const Rect2D& rect, const Capsule2D& capsule)
{
	//円とカプセルの当たり判定の最後の部分を円と矩形の当たり判定に変えるだけ

	//矩形からカプセルに垂線を引いて球からカプセルへの最短距離を求める
	// カプセルのposA -> A
	// カプセルのposB -> B
	// 矩形の中心座標(pos) -> R
	// 矩形からカプセルに垂線を下してでた座標 -> V
	Vector2 A = capsule.posA;
	Vector2 B = capsule.posB;
	Vector2 R = rect.pos;


	//カプセルのAから矩形の中心座標へのベクトル
	Vector2 AtoR = R - A;
	//posAからposBへの単位ベクトル
	Vector2 AtoB = B - A;
	Vector2 nomAtoB = AtoB.Normalize();
	//内積の計算
	//Aから座標Vへの距離を出す
	float dotAtoV = nomAtoB.Dot(AtoR);
	//posAにposAからposBへの単位ベクトル * dotをして座標Vを出す
	Vector2 V = A + (nomAtoB * dotAtoV);


	//VがABの間にある座標かどうかをチェックする
	float dotAtoB = nomAtoB.Dot(AtoB);//全体
	float rate = (dotAtoV / dotAtoB);//割合

	//球とカプセルの半径の合計より大きければ当たっていない
	//矩形の中心から各頂点への長さを求める
	float hypot = Vector2{ (rect.w / 2.0f),(rect.h / 2.0f) }.Magnitude();
	//0より小さいならposAより外、1より大きいならposBより外、どちらでもないならAB間にある
	if (rate > 1.0f)
	{
		//最短距離はposBから矩形の間の距離()BtoR
		Vector2 BtoR = R - B;
		//Y成分がそれぞれの半径の足した値よりも大きい、または
		//X成分がそれぞれの半径の足した値よりも大きい、または
		//ベクトルの大きさが矩形の中心から各頂点への長さと球の半径の大きさを
		//足した値よりも大きいなら当たっていない
		if (abs(BtoR.y) > capsule.radius + (rect.h / 2.0f) ||
			abs(BtoR.x) > capsule.radius + (rect.w / 2.0f) ||
			BtoR.Magnitude() > capsule.radius + hypot)
		{
			return false;
		}
	}
	else if (rate < 0.0f)
	{
		if (abs(AtoR.y) > capsule.radius + (rect.h / 2.0f) ||
			abs(AtoR.x) > capsule.radius + (rect.w / 2.0f) ||
			AtoR.Magnitude() > capsule.radius + hypot)
		{
			return false;
		}
	}
	else
	{
		//最短距離は球からV(垂線を下してでた座標)の間の距離(VtoC)
		Vector2 VtoR = R - V;
		if (abs(VtoR.y) > capsule.radius + (rect.h / 2.0f) ||
			abs(VtoR.x) > capsule.radius + (rect.w / 2.0f) ||
			VtoR.Magnitude() > capsule.radius + hypot)
		{
			return false;
		}
	}

	//ここまでくれば当たっている
	return true;
}

bool CollisionCheck::IsCollCapCap(const Capsule2D& cap1, const Capsule2D& cap2)
{
	//カプセルの各点から垂線を下ろし、最短距離を求める
	// 最短距離
	// カプセル1のposA -> A
	// カプセル1のposB -> B
	// カプセル2のposA -> C
	// カプセル2のposB -> D
	// 垂線の座標 -> V
	// 最短距離 -> S 
	Vector2 A = cap1.posA;
	Vector2 B = cap1.posB;
	Vector2 C = cap2.posA;
	Vector2 D = cap2.posB;
	Vector2 V = {};
	Vector2 S = {};

	Vector2 point = {};//垂線を下す点
	Vector2 lineA = {};//線分の片方の座標
	Vector2 lineB = {};//線分の片方の座標
	for (int i = 0; i < 2; ++i)
	{
		switch (i)
		{
		case 0:
			//線分CD
			lineA = C;
			lineB = D;
			break;
		case 1:
			//線分AB
			lineA = A;
			lineB = B;
			break;
		default:
			break;
		}
		for (int j = 0; j < 2; ++j)
		{
			if (i == 0 && j == 0)
			{
				//Aから線分に下ろす
				point = A;
			}
			else if (i == 0 && j == 1)
			{
				//Bから
				point = B;
			}
			else if (i == 1 && j == 0)
			{
				//Cから
				point = C;
			}
			else
			{
				//Dから
				point = D;
			}
			//pointへのベクトル
			Vector2 lineAToPoint = point - lineA;
			//線分方向の単位ベクトル
			Vector2 lineAToB = lineB - lineA;
			Vector2 nomLineAToB = lineAToB.Normalize();
			//内積の計算
			//lineAから座標Vへの距離を出す
			float dotLineAtoV = nomLineAToB.Dot(lineAToPoint);
			V = lineA + (nomLineAToB * dotLineAtoV);
			//Vが線分上にある座標かどうかをチェックする
			float dotLineAtoB = nomLineAToB.Dot(lineAToB);//全体
			float rate = (dotLineAtoV / dotLineAtoB);//割合
			//線分上に垂線があるなら最短距離かどうか比較する
			Vector2 candidate;//最短距離候補を入れていく
			if (rate > 1.0f)
			{
				//LineBが最もpointに近い
				candidate = lineB - point;
			}
			else if (rate < 0.0f)
			{
				//LineAが最もpointに近い
				candidate = lineA - point;
			}
			else
			{
				//Vが最もpointに近い
				candidate = V - point;
			}
			//初回または前回の最短距離よりも小さいなら
			if (S.Magnitude() == 0 || S.Magnitude() > candidate.Magnitude())
			{
				//現在の最短距離にする
				S = candidate;
			}
		}
	}

	//最短距離がそれぞれの半径の合計より大きいなら当たっていない
	if (S.Magnitude() > cap1.radius + cap2.radius)
	{
		return false;
	}

	//ここまでくれば当たっている
	return true;
}

