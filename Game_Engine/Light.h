#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include "LightHandler.h"
#include "Transform.h"
class Light :
    public Transform, public LightHandler
{
private:
    Matrix m_view[6];
    Matrix m_projection;
    Matrix m_projectiveTC;
    BOUNDING_SPHERE m_bounds;
    // Inherited via LightHandler
    virtual void setLight(LIGHT_TYPE type, const XMFLOAT4& pos, const XMFLOAT4& color, float intensity,
        const XMFLOAT4& direction = XMFLOAT4(0, 0, 0, 0), float fov = 0.0f, LIGHT_SHADOW_TYPE shadowType = LIGHT_SHADOW_TYPE::NO_SHADOW, float range = 10) override;
public:
    Light(LIGHT_TYPE type, const XMFLOAT4& pos, const XMFLOAT4& color, float intensity,
        const XMFLOAT4& direction = XMFLOAT4(0, 0, 0, 0), float fov = 0.0f, LIGHT_SHADOW_TYPE shadowType = LIGHT_SHADOW_TYPE::NO_SHADOW, float range = 10);
    void setColor(const XMFLOAT4& color) { m_data.color = color; }
    void setDirection(const XMFLOAT4& direction);
    void setShadow(bool HasShadow) { m_data.otherConstants.w = 1; }
    bool setShadow(const BOUNDING_SPHERE& bounds);
    bool setShadow();
    const Matrix& getView(unsigned int nr = 0) { return m_view[nr]; }
    const Matrix& getProjection() { return m_projection; }
    const Matrix& getProjectiveTexCoord() { return m_projectiveTC; }
    virtual void setTranslation(const Vector3& position, unsigned int instance = 0) override;
    void updateParentGlobal();
    void updateParentLocal();
   
};

#endif