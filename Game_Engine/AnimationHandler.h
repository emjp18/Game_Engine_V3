#pragma once
#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H
#include "Timer.h"
class AnimationHandler
{
private:
	ANIMATION_TYPE m_AnimationType = ANIMATION_TYPE::SKELETON;
	std::string m_TrackType;
protected:

	float m_time = 0.0f;
	float m_fps;
public:
	virtual void play(float dt, bool once = false, float scale = 1.0f) = 0;
	virtual ANIMATION_TYPE getAnimationType() { return m_AnimationType; }
	virtual void setAnimationType(ANIMATION_TYPE type) { m_AnimationType = type; }
	virtual std::string getTrackType() { return m_TrackType; }
	virtual void setTrackType(std::string type) { m_TrackType = type; }
	virtual ~AnimationHandler();
	bool g_done = false;
};

#endif