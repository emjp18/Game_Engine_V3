#pragma once
#ifndef GUIHANDLER_H
#define GUIHANDLER_H
#include "SoundHandler.h"
#include "Timer.h"
class GuiHandler
{
private:
	SpriteFont* m_quitS;
	SpriteFont* m_sceneOneS;
	SpriteFont* m_sceneTwoS;
	SpriteFont* m_sceneThreeS;
	SpriteFont* m_sceneFourS;
	SpriteFont* m_lowResS;
	SpriteFont* m_mediumResS;
	SpriteFont* m_HighResS;  

	SpriteBatch* m_quitB;
	SpriteBatch* m_sceneOneB;
	SpriteBatch* m_sceneTwoB;
	SpriteBatch* m_sceneThreeB;
	SpriteBatch* m_sceneFourB;
	SpriteBatch* m_lowResB;
	SpriteBatch* m_mediumResB;
	SpriteBatch* m_HighResB;

	XMFLOAT2 m_quitF;
	XMFLOAT2 m_sceneOneF;
	XMFLOAT2 m_sceneTwoF;
	XMFLOAT2 m_sceneThreeF;
	XMFLOAT2 m_sceneFourF;
	XMFLOAT2 m_resHighF;
	XMFLOAT2 m_resMediumF;
	XMFLOAT2 m_resLowF;
	
	std::string m_quitT = "EXIT";
	std::string m_sceneOneT = "SCENE_PARTICLE";
	std::string m_sceneTwoT = "SCENE_LOD";
	std::string m_sceneThreeT = "SCENE_HEAT";
	std::string m_sceneFourT = "SCENE_ANIMATION";
	std::string m_highResT = "1920 x 1080";
	std::string m_mediumResT = "1280 x 720";
	std::string m_lowResT = "640 x 480";

	size_t m_h, m_w;
	MENU_OPTIONS m_option = MENU_OPTIONS::NONE;
public:
	GuiHandler();
	~GuiHandler();
	static GuiHandler& getInstance() { static GuiHandler instance; return instance; }
	GuiHandler(const GuiHandler&) = delete;
	GuiHandler& operator=(const GuiHandler&) = delete;
	GuiHandler(GuiHandler&&) = delete;
	GuiHandler& operator=(GuiHandler&&) = delete;
	void changeRes(size_t newResX, size_t newResY, ID3D11Device* device, ID3D11DeviceContext* context);
	void init(size_t w, size_t h, ID3D11DeviceContext* context, ID3D11Device* device);
	void drawEverything(const POINT& mouseP);
	const MENU_OPTIONS& selectionEverything(const POINT& mouseP);

};

#endif