#include "stdafx.h"
#include "Light.h"

void Light::setLight(LIGHT_TYPE type, const XMFLOAT4& pos, const XMFLOAT4& color, float intensity, const XMFLOAT4& direction, float fov, LIGHT_SHADOW_TYPE shadowType, float range)
{
	m_type = type;
	switch (type)
	{
	default:
	{
			break;
	}
	case LIGHT_TYPE::POINT:
	{
		m_data.color = color;
		m_data.direction = direction;
		m_data.position = pos;
		m_data.type.x = (int)LIGHT_TYPE::POINT;
		m_data.otherConstants.x = intensity;
		m_data.otherConstants.y = range;
		m_data.otherConstants.z = fov;
		m_data.otherConstants.w = (float)(int)shadowType;

		if (shadowType == LIGHT_SHADOW_TYPE::HAS_SHADOW)
		{
			setShadow();
		}
		break;
	}
	case LIGHT_TYPE::SPOT:
	{
		m_data.color = color;
		m_data.direction = direction;
		m_data.position = pos;
		m_data.type.x = (int)LIGHT_TYPE::SPOT;
		m_data.otherConstants.x = intensity;
		m_data.otherConstants.y = range;
		m_data.otherConstants.z = fov;
		m_data.otherConstants.w = (float)(int)shadowType;
		break;
	}
	case LIGHT_TYPE::DIRECTIONAL:
	{
		m_data.color = color;
		m_data.direction = direction;
		m_data.position = pos;
		m_data.type.x = (int)LIGHT_TYPE::DIRECTIONAL;
		m_data.otherConstants.x = intensity;
		m_data.otherConstants.y = range;
		m_data.otherConstants.z = fov;
		m_data.otherConstants.w = (float)(int)shadowType;
		break;
	}
	}
	m_projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), SHADOW_MAP_SIZE_X / SHADOW_MAP_SIZE_Y, 1, 25);
}

Light::Light(LIGHT_TYPE type, const XMFLOAT4& pos, const XMFLOAT4& color, float intensity, const XMFLOAT4& direction, float fov, LIGHT_SHADOW_TYPE shadowType, float range)
{
	setLight(type, pos, color, intensity, direction, fov, shadowType, range);
}

void Light::setDirection(const XMFLOAT4& direction)
{
	if (m_type != LIGHT_TYPE::POINT)
	{
		m_data.direction = direction;
		if (m_type == LIGHT_TYPE::DIRECTIONAL)
		{
			if (m_data.otherConstants.w == (int)LIGHT_SHADOW_TYPE::HAS_SHADOW && m_bounds.radius != 0.0f) //checking if it has bounds yet.
			{
				setShadow(m_bounds);
				
			}
		}
		
	}
	
}

bool Light::setShadow(const BOUNDING_SPHERE& bounds)
{
	m_bounds = bounds;
	if (m_type == LIGHT_TYPE::DIRECTIONAL)
	{
		
		float scalar = -2.0f * bounds.radius;

		Vector3 lightPos = DirectX::XMVectorScale(XMLoadFloat4(&m_data.direction), scalar);

		Vector3 up = DirectX::XMVectorSet(0, 1, 0, 0);
		Vector3 target = bounds.center;

		m_view[0] = DirectX::XMMatrixLookAtLH(lightPos, target, up);

		Vector3 lightSpace = XMVector3TransformCoord(bounds.center, m_view[0]);


		float l = lightSpace.x - bounds.radius;
		float b = lightSpace.y - bounds.radius;
		float n = lightSpace.z - bounds.radius;
		float r = lightSpace.x + bounds.radius;
		float t = lightSpace.y + bounds.radius;
		float f = lightSpace.z + bounds.radius;

		m_projection = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		Matrix T(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f);

		m_projectiveTC = m_view[0] * m_projection * T;
		return true;
	}
	else
	{
		return false;
	}
}

bool Light::setShadow()
{
	if (m_type == LIGHT_TYPE::POINT)
	{
		/*ightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)); FLIP Z COORD.
		 
		lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
		 
		lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
		 
		lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0));
		 
		lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0));
		 
		lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)
		m_view[0] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + left, -up);

		m_view[0].Transpose();

		m_view[1] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + -left, -up);

		m_view[1].Transpose();
		
		m_view[2] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + up, behind);

		m_view[2].Transpose();
		m_view[3] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + -up, -behind);

		m_view[3].Transpose();
		
		m_view[4] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + behind, -up);

		m_view[4].Transpose();
		m_view[5] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + -behind, -up);
		m_view[5].Transpose();
		
		*/

		Vector3 camUp[6];
		Vector3 camTarget[6];
		camUp[0] = XMFLOAT3(0, 1, 0);
		camUp[1] = XMFLOAT3(0, 1, 0);
		camUp[2] = XMFLOAT3(0, 0, -1);
		camUp[3] = XMFLOAT3(0, 0, 1);
		camUp[4] = XMFLOAT3(0, 1, 0);
		camUp[5] = XMFLOAT3(0, 1, 0);
		camTarget[0] = XMFLOAT3(1, 0, 0);
		camTarget[1] = XMFLOAT3(-1, 0, 0);
		camTarget[2] = XMFLOAT3(0, 1, 0);
		camTarget[3] = XMFLOAT3(0, -1, 0);
		camTarget[4] = XMFLOAT3(0, 0, 1);
		camTarget[5] = XMFLOAT3(0, 0, -1);
		Vector3 up = XMFLOAT3(0, 1, 0);
		Vector3 left = XMFLOAT3(1, 0, 0);
		Vector3 behind = XMFLOAT3(0, 0, -1);
		
		m_view[0] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + camTarget[0], camUp[0]);

		m_view[0].Transpose();

		m_view[1] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + camTarget[1], camUp[1]);

		m_view[1].Transpose();
		
		m_view[2] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + camTarget[2], camUp[2]);

		m_view[2].Transpose();
		m_view[3] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + camTarget[3], camUp[3]);

		m_view[3].Transpose();
		
		m_view[4] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + camTarget[4], camUp[4]);

		m_view[4].Transpose();
		m_view[5] = DirectX::XMMatrixLookAtLH(m_translation[0], m_translation[0] + camTarget[5], camUp[5]);
		m_view[5].Transpose();
		return true;
	}
	else
	{
		return false;
	}
}

void Light::setTranslation(const Vector3& position, unsigned int instance)
{
	if(m_type != LIGHT_TYPE::DIRECTIONAL)
	{
		m_translation[instance] = position;
		m_data.position.x = position.x;
		m_data.position.y = position.y;
		m_data.position.z = position.z;
		m_data.position.w = 1;
		if (m_type == LIGHT_TYPE::POINT)
		{
			if (m_data.otherConstants.w == (int)LIGHT_SHADOW_TYPE::HAS_SHADOW)
			{
				setShadow();
			}
		}
	}
	
	
}

void Light::updateParentLocal()
{
	if (m_parent[0])
	{
		setTranslation(m_parent[0]->getLocalTranformations().trans);
	}
}

void Light::updateParentGlobal()
{
	if (m_parent[0])
	{
		setTranslation(m_parent[0]->getGlobalTransformations().trans);
	}
}






