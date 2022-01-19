#include "stdafx.h"
#include "SoundHandler.h"

SoundHandler::SoundHandler()
{
#ifdef _DEBUG
	m_aflags = m_aflags | AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<AudioEngine>(m_aflags);
	m_winSound = std::make_unique<SoundEffect>(m_audioEngine.get(), L"../Resources/Sounds/Win.wav");
	m_loseSound = std::make_unique<SoundEffect>(m_audioEngine.get(), L"../Resources/Sounds/Lose.wav");
	m_progressSound = std::make_unique<SoundEffect>(m_audioEngine.get(), L"../Resources/Sounds/Progress.wav");
	m_menuSelectionSound = std::make_unique<SoundEffect>(m_audioEngine.get(), L"../Resources/Sounds/Selection.wav");
}

SoundHandler::~SoundHandler()
{
	
}



void SoundHandler::play(SOUND_EFFECT effect)
{
	switch (effect)
	{
	default:
	{
		break;
	}
	case SOUND_EFFECT::GAME_LOSE:
	{
		m_loseSound->Play();
		break;
	}
	case SOUND_EFFECT::GAME_PROGRESS:
	{
		m_progressSound->Play();
		break;
	}
	case SOUND_EFFECT::GAME_WIN:
	{
		m_winSound->Play();
		break;
	}
	case SOUND_EFFECT::MENU_SELECTION:
	{
		m_menuSelectionSound->Play();
		break;
	}
	}
}
