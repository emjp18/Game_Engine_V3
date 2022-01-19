#include "stdafx.h"
#include "Camera.h"

void Camera::updateFrustumPlanes()
{
    Matrix comboMatrix = getVP();

    //ROW_MAJOR DirectX
    m_frustumPlanes[0].x = comboMatrix._14 + comboMatrix._11;
    m_frustumPlanes[0].y = comboMatrix._24 + comboMatrix._21;
    m_frustumPlanes[0].z = comboMatrix._34 + comboMatrix._31;
    m_frustumPlanes[0].w = comboMatrix._44 + comboMatrix._41;
    // Right clipping plane
    m_frustumPlanes[1].x = comboMatrix._14 - comboMatrix._11;
    m_frustumPlanes[1].y = comboMatrix._24 - comboMatrix._21;
    m_frustumPlanes[1].z = comboMatrix._34 - comboMatrix._31;
    m_frustumPlanes[1].w = comboMatrix._44 - comboMatrix._41;
    // Top clipping plane
    m_frustumPlanes[2].x = comboMatrix._14 - comboMatrix._12;
    m_frustumPlanes[2].y = comboMatrix._24 - comboMatrix._22;
    m_frustumPlanes[2].z = comboMatrix._34 - comboMatrix._32;
    m_frustumPlanes[2].w = comboMatrix._44 - comboMatrix._42;
    // Bottom clipping plane
    m_frustumPlanes[3].x = comboMatrix._14 + comboMatrix._12;
    m_frustumPlanes[3].y = comboMatrix._24 + comboMatrix._22;
    m_frustumPlanes[3].z = comboMatrix._34 + comboMatrix._32;
    m_frustumPlanes[3].w = comboMatrix._44 + comboMatrix._42;
    // Near clipping plane
    m_frustumPlanes[4].x = comboMatrix._13;
    m_frustumPlanes[4].y = comboMatrix._23;
    m_frustumPlanes[4].z = comboMatrix._33;
    m_frustumPlanes[4].w = comboMatrix._43;
    // Far clipping plane
    m_frustumPlanes[5].x = comboMatrix._14 - comboMatrix._13;
    m_frustumPlanes[5].y = comboMatrix._24 - comboMatrix._23;
    m_frustumPlanes[5].z = comboMatrix._34 - comboMatrix._33;
    m_frustumPlanes[5].w = comboMatrix._44 - comboMatrix._43;

   


    for (int i = 0; i < 6; i++)
    {
        Vector4 Nd = XMFLOAT4(m_frustumPlanes[i].x, m_frustumPlanes[i].y, m_frustumPlanes[i].z, m_frustumPlanes[i].w);
        Nd.Normalize();
        m_frustumPlanes[i] = Nd;
       

    }
}

Camera::Camera()
{
    m_frustumPlanes.resize(6);
}

Camera::~Camera()
{
}

const Matrix& Camera::getViewMatrix()
{
    return m_view;
}

const Matrix& Camera::getProjectionMatrix()
{
    return m_projection;
}

const Matrix& Camera::getVP()
{
    m_vp = m_view * m_projection;
    return m_vp;
}

Matrix& Camera::getWorld(unsigned int instance, bool isParent)
{
     m_w = getViewMatrix().Invert();

  

    return m_w;
}

Matrix& Camera::getInverseWorld(unsigned int instance, bool isParent)
{
    m_w = getWorld().Invert();


    return m_w;
}



bool Camera::cull(const AABB& aabb)
{
  //  for (int f = 0; f < 6; f++)
  //{
  //    float d = m_frustumPlanes[f].w;
  //    Vector3 N = XMFLOAT3(m_frustumPlanes[f].x, m_frustumPlanes[f].y, m_frustumPlanes[f].z);
  //    //N *= -1;
  //    // These two lines not necessary with a (center, extents) AABB representation
  //    Vector3 c = aabb.center; // Compute AABB center
  //    Vector3 e = aabb.max - c; // Compute positive extents
  //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
  //    float r = e.x * fabsf(N.x) + e.y * fabsf(N.y) + e.z * fabsf(N.z);
  //    // Compute distance of box center from plane
  //    float s = N.Dot(c) - d;
  //    // Intersection occurs when distance s falls within [-r,+r] interval
  //    if (fabsf(s) > r)
  //    {
  //        return true;
  //    }

  //}
  //return false;
    for (int planeID = 0; planeID < 6; planeID++)
    {
        Vector3 axisVert = Vector3::Zero;

        Vector3 planeNormal;
        planeNormal.x = m_frustumPlanes[planeID].x;
        planeNormal.y = m_frustumPlanes[planeID].y;
        planeNormal.z = m_frustumPlanes[planeID].z;
        /* planeNormal *= -1;*/
         // x-axis
        if (m_frustumPlanes[planeID].x < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the x axis
            axisVert.x = aabb.min.x;// min x plus tree positions x
        else
            axisVert.x = aabb.max.x; // max x plus tree positions x

        // y-axis
        if (m_frustumPlanes[planeID].y < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the y axis
            axisVert.y = aabb.min.y; // min y plus tree positions y
        else
            axisVert.y = aabb.max.y; // max y plus tree positions y

        // z-axis
        if (m_frustumPlanes[planeID].z < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the z axis
            axisVert.z = aabb.min.z; // min z plus tree positions z
        else
            axisVert.z = aabb.max.z; // max z plus tree positions z

        // Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
        // and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
        // that it should be culled
        if (planeNormal.Dot(axisVert) + m_frustumPlanes[planeID].w < 0.0f)
            return true;
    }
    return false;
}

bool Camera::cull(const BOUNDING_SPHERE& sphere)
{
    for (int f = 0; f < 6; f++)
    {
        float d = m_frustumPlanes[f].w;
        Vector3 N = XMFLOAT3(m_frustumPlanes[f].x, m_frustumPlanes[f].y, m_frustumPlanes[f].z);
        //N *= -1;
        float dist = sphere.center.Dot(N) - d;

        // If sphere center within +/-radius from plane, plane intersects sphere
        if (fabsf(dist) <= sphere.radius)
        {
            return false;
        }
    }
    return true;
}

bool Camera::cull(const OBB& obb)
{
    for (int planeID = 0; planeID < 6; planeID++)
    {
        Vector3 axisVert;

        Vector3 planeNormal((float)m_frustumPlanes[planeID].x, (float)m_frustumPlanes[planeID].y, (float)m_frustumPlanes[planeID].z);


        float nX = obb.uX.Dot(planeNormal);
        float nY = obb.uY.Dot(planeNormal);
        float nZ = obb.uZ.Dot(planeNormal);
        planeNormal.x = nX;
        planeNormal.y = nY;
        planeNormal.z = nZ;


        /* planeNormal *= -1;*/
         // x-axis
        if (m_frustumPlanes[planeID].x < 0.0)    // Which AABB vertex is furthest down (plane normals direction) the x axis
            axisVert.x = obb.min.x;// min x plus tree positions x
        else
            axisVert.x = obb.max.x; // max x plus tree positions x

        // y-axis
        if (m_frustumPlanes[planeID].y < 0.0)    // Which AABB vertex is furthest down (plane normals direction) the y axis
            axisVert.y = obb.min.y; // min y plus tree positions y
        else
            axisVert.y = obb.max.y; // max y plus tree positions y

        // z-axis
        if (m_frustumPlanes[planeID].z < 0.0)    // Which AABB vertex is furthest down (plane normals direction) the z axis
            axisVert.z = obb.min.z; // min z plus tree positions z
        else
            axisVert.z = obb.max.z; // max z plus tree positions z

        // Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
        // and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
        // that it should be culled
        if (planeNormal.Dot(axisVert) + m_frustumPlanes[planeID].w < 0.0)
            return true;
    }
    return false;
}





void Camera::updateProjectionMatrix(float width, float height, float fov)
{
    m_fov = fov;
    m_height = height;
    m_width = width;
   ;
    if (m_type == CAMERA_TYPE::PERSPECTIVE)
    {
        m_projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / height, CAMERA_NEAR_Z, CAMERA_FAR_Z);

    }
    else
    {
        m_projection = XMMatrixOrthographicLH(width, height, CAMERA_NEAR_Z, CAMERA_FAR_Z);

    }
}


void Camera::updateViewMatrix(Vector3& pos, Vector3& forward, Vector3& up)
{
    m_view = XMMatrixLookToLH(pos, forward, up);

    updateFrustumPlanes();
    setTranslation(pos);
}

void Camera::changeFOV(float fov)
{
    updateProjectionMatrix(m_width, m_height, fov);
}

const XMFLOAT4& Camera::getTransAsV4()
{
    Vector3 t = getLocalTranformations().trans;
    
  
    XMStoreFloat4(&m_pos, t);
    m_pos.w = 1;
    return m_pos;
}

void Camera::changeResolution(size_t newResX, size_t newResY)
{
    updateProjectionMatrix((float)newResX, (float)newResY, m_fov);
}
