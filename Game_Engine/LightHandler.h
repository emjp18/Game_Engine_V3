#pragma once
#ifndef LIGHTANDLER_H
#define LIGHTANDLER_H
#include "common.h"
class LightHandler
{
private:
protected:
	LIGHT_TYPE m_type;
	LIGHT_DATA m_data;
public:
	virtual void setLight(LIGHT_TYPE type, const XMFLOAT4& pos, const XMFLOAT4& color, float intensity, const XMFLOAT4& direction = XMFLOAT4(0, 0, 0, 0),
		float fov = 0.0f, LIGHT_SHADOW_TYPE shadowType = LIGHT_SHADOW_TYPE::NO_SHADOW, float range = 10) = 0;
	virtual LIGHT_TYPE getType() { return m_type; }
	virtual const LIGHT_DATA& getData() { return m_data; }
};

#endif