#pragma once
class Clamp
{
public:
	Clamp();
	~Clamp() {};
	/// <summary>
	/// ����l�����͈͓��Ɏ��܂�悤�ɃN�����v����
	/// </summary>
	/// <param name="value">�N�����v����l</param>
	/// <param name="min">�ŏ�</param>
	/// <param name="max">�ő�</param>
	/// <returns>int�^</returns>
	int ClampInt(int value, int min, int max);
	/// <summary>
	/// ����l�����͈͓��Ɏ��܂�悤�ɃN�����v����
	/// </summary>
	/// <param name="value">�N�����v����l</param>
	/// <param name="min">�ŏ�</param>
	/// <param name="max">�ő�</param>
	/// <returns>float�^</returns>
	float ClampFloat(float value, float min, float max);
};

