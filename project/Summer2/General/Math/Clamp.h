#pragma once
class Clamp
{
public:
	Clamp();
	~Clamp() {};
	/// <summary>
	/// ある値が一定範囲内に収まるようにクランプする
	/// </summary>
	/// <param name="value">クランプする値</param>
	/// <param name="min">最小</param>
	/// <param name="max">最大</param>
	/// <returns>int型</returns>
	int ClampInt(int value, int min, int max);
	/// <summary>
	/// ある値が一定範囲内に収まるようにクランプする
	/// </summary>
	/// <param name="value">クランプする値</param>
	/// <param name="min">最小</param>
	/// <param name="max">最大</param>
	/// <returns>float型</returns>
	float ClampFloat(float value, float min, float max);
};

