#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "Transform.h"
class Camera :
    public Transform
{
private:
    Matrix m_w;
    Matrix m_vp;
    std::vector<Vector4> m_frustumPlanes;
    float m_fov, m_width, m_height;
    Matrix m_world;
    Matrix m_view;
    Matrix m_projection;
    void updateFrustumPlanes();
    CAMERA_TYPE m_type = CAMERA_TYPE::PERSPECTIVE;
    XMFLOAT4 m_pos;
public:
    Camera();
    virtual ~Camera();
    const Matrix& getViewMatrix();
    const Matrix& getProjectionMatrix();
    const Matrix& getVP();
    virtual Matrix& getWorld(unsigned int instance = 0, bool isParent = false) override;
    virtual Matrix& getInverseWorld(unsigned int instance = 0, bool isParent = false) override;
    bool cull(const AABB& aabb);
    bool cull(const BOUNDING_SPHERE& sphere);
    bool cull(const OBB& obb);
    void updateProjectionMatrix(float width, float height, float fov);
    void updateViewMatrix(Vector3& pos, Vector3& forward, Vector3& up);
    void changeFOV(float fov);
    float getFOV() { return m_fov; }
    CAMERA_TYPE getType() { return m_type; }
    void setType(CAMERA_TYPE type) { m_type = type; }
    const XMFLOAT4& getTransAsV4();
    void changeResolution(size_t newResX, size_t newResY);
    const std::vector<Vector4>& getFrustumPlanes() { return m_frustumPlanes; }
};

#endif