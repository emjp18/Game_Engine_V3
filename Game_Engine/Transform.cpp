#include "stdafx.h"
#include "Transform.h"
Transform::Transform()
{
    m_scale.resize(m_instanceCount);
    m_rotationQ.resize(m_instanceCount);
    m_rotationEuler.resize(m_instanceCount);
    m_translation.resize(m_instanceCount);
    m_parent.resize(m_instanceCount);

    auto one = Vector3::One;
    auto id = Quaternion::Identity;
    auto zero = Vector3::Zero;
    for (int i = 0; i < m_parent.size(); i++)
    {
        m_parent[i] = nullptr;
    }
    for (int i = 0; i < m_scale.size(); i++)
    {

        m_scale[i] = one;
    }
    for (int i = 0; i < m_rotationQ.size(); i++)
    {

        m_rotationQ[i] = id;
    }
    for (int i = 0; i < m_rotationEuler.size(); i++)
    {

        m_rotationEuler[i] = zero;
    }
    for (int i = 0; i < m_translation.size(); i++)
    {

        m_translation[i] = zero;
    }
    m_parentOffset = XMMatrixIdentity();
}

Transform::~Transform()
{
}

Matrix& Transform::getWorld(unsigned int instance, bool isParent)
{
    Matrix S = XMMatrixIdentity();
    Matrix R = XMMatrixIdentity();
    Matrix T = XMMatrixIdentity();
    if (isParent)
    {
        instance = 0;
    }
    if (m_useS)
    {
        S = XMMatrixScalingFromVector(m_scale[instance]);
    }
    if (m_useR)
    {
        R = XMMatrixRotationQuaternion(m_rotationQ[instance]);
    }
    if (m_useT)
    {
        T = XMMatrixTranslationFromVector(m_translation[instance]);
    }
    if (m_parent[instance])
    {
        if (!m_byPassWorld)
        {
            m_world = S * R * T;
        }
        
        
        m_world = m_world * m_parent[instance]->getWorld(instance, true);

        return m_world;
    }
    else
    {
        if (!m_byPassWorld)
        {
            m_world = S * R * T;
        }
        if (m_useOffset&&isParent) //st the end of the chain.
        {
            m_world = m_world * m_parentOffset;
        }

        return m_world;
    }
}
Matrix& Transform::getInverseWorld(unsigned int instance, bool isParent)
{
    Matrix S = XMMatrixIdentity();
    Matrix R = XMMatrixIdentity();
    Matrix T = XMMatrixIdentity();
    if (isParent)
    {
        instance = 0;
    }
    if (m_useS)
    {
        S = XMMatrixScalingFromVector(m_scale[instance]);
    }
    if (m_useR)
    {
        R = XMMatrixRotationQuaternion(m_rotationQ[instance]);
    }
    if (m_useT)
    {
        T = XMMatrixTranslationFromVector(m_translation[instance]);
    }
    if (m_parent[instance])
    {
     
        if (!m_byPassWorld)
        {
            m_world = S * R * T;
        }
       
            ;
        m_world = m_world * m_parent[instance]->getWorld(instance, true);

        m_world = m_world.Invert();
        return m_world;
    }
    else
    {
        if (!m_byPassWorld)
        {
            m_world = S * R * T;
        }
        

        m_world = m_world.Invert();
        return m_world;
    }
}
void Transform::setScale(const Vector3& scale, unsigned int instance)
{
    m_scale[instance] = scale;
}

void Transform::setTranslation(const Vector3& position, unsigned int instance)
{
    m_translation[instance] = position;
}

void Transform::setRotationQuat(const Quaternion& rotation, unsigned int instance)
{
    m_rotationQ[instance] = rotation;
}

void Transform::setRotationEuler(const Vector3& rotation, unsigned int instance)
{
    m_rotationQ[instance] = Quaternion::CreateFromYawPitchRoll(rotation.x, rotation.y, rotation.z);
    m_rotationEuler[instance] = rotation;
}





const TRANSFORM_VALUES_LOCAL& Transform::getLocalTranformations(unsigned int instance)
{
    m_localTransformations.rotE = m_rotationEuler[instance];
    m_localTransformations.rotQ = m_rotationQ[instance];
    m_localTransformations.scale = m_scale[instance];
    m_localTransformations.trans = m_translation[instance];

    return m_localTransformations;

}

const TRANSFORM_VALUES_GLOBAL& Transform::getGlobalTransformations(unsigned int instance)
{
    Vector3 s, t;
    Quaternion r;
    getWorld(instance).Decompose(s, r, t);
    m_globalTransformations.rotQ = r;
    m_globalTransformations.scale = s;
    m_globalTransformations.trans = t;

    return m_globalTransformations;
}



void Transform::setInstanceCount(unsigned int count)
{
    m_instanceCount = count;
    m_scale.resize(m_instanceCount);
    m_rotationQ.resize(m_instanceCount);
    m_translation.resize(m_instanceCount);
    m_parent.resize(m_instanceCount);
    m_rotationEuler.resize(m_instanceCount);
    
    auto one = Vector3::One;
    auto id = Quaternion::Identity;
    auto zero = Vector3::Zero;
    for (int i = 0; i < m_parent.size(); i++)
    {
        m_parent[i] = nullptr;
    }
    for (int i = 0; i < m_scale.size(); i++)
    {

        m_scale[i] = one;
    }
    for (int i = 0; i < m_rotationQ.size(); i++)
    {

        m_rotationQ[i] = id;
    }
    for (int i = 0; i < m_rotationEuler.size(); i++)
    {

        m_rotationEuler[i] = zero;
    }
    for (int i = 0; i < m_translation.size(); i++)
    {

        m_translation[i] = zero;
    }
}