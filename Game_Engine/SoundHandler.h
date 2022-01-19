#pragma once
#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H
#include "common.h"
class SoundHandler
{
	AUDIO_ENGINE_FLAGS m_aflags = AudioEngine_Default;

	std::unique_ptr<AudioEngine> m_audioEngine;
	std::unique_ptr<SoundEffect> m_loseSound;
	std::unique_ptr<SoundEffect> m_winSound;
	std::unique_ptr<SoundEffect> m_progressSound;
	std::unique_ptr<SoundEffect> m_menuSelectionSound;
public:
	SoundHandler();
	~SoundHandler();
	static SoundHandler& getInstance() { static SoundHandler instance; return instance; }
	SoundHandler(const SoundHandler&) = delete;
	SoundHandler& operator=(const SoundHandler&) = delete;
	SoundHandler(SoundHandler&&) = delete;
	SoundHandler& operator=(SoundHandler&&) = delete;
	void play(SOUND_EFFECT);
};

#endif