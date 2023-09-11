#pragma once
#include "Component.h"


namespace BJEngine
{
    class Vector3D;
}

class CameraShake :
    public BJEngine::Component
{
private:
    BJEngine::Math::Vector3D startPos;
    bool isShake;
    float shakeTime;
    float totalShakeTime;
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    std::vector<BJEngine::Math::Vector3D> shakeRange;

public:
    void Shake(float shaketime, float xdelta, float ydelta);

public:
    virtual void Awake() override;
    virtual void Update() override;
};
