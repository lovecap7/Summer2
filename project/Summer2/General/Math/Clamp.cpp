#include "Clamp.h"

Clamp::Clamp()
{
}

//�N�����v�֐�
int Clamp::ClampInt(int value, int min, int max)
{
    //�͈͓��Ɏ��܂��Ă���Ȃ炻�̂܂ܕԂ�
    int clampValue = value;
    //�ŏ���菬�����Ȃ�ŏ��ɍ��킹��
    if (clampValue < min)
    {
        clampValue = min;
    }
    //�ő���傫���Ȃ�ő�ɍ��킹��
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float Clamp::ClampFloat(float value, float min, float max)
{
    //�͈͓��Ɏ��܂��Ă���Ȃ炻�̂܂ܕԂ�
    float clampValue = value;
    //�ŏ���菬�����Ȃ�ŏ��ɍ��킹��
    if (clampValue < min)
    {
        clampValue = min;
    }
    //�ő���傫���Ȃ�ő�ɍ��킹��
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}
