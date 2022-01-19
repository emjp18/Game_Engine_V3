#include "stdafx.h"
#include "GuiHandler.h"



GuiHandler::GuiHandler()
{

}

GuiHandler::~GuiHandler()
{
	RELEASE(m_quitB);
	RELEASE(m_sceneOneB);
	RELEASE(m_sceneTwoB);
	RELEASE(m_sceneThreeB);
	RELEASE(m_sceneFourB);
	RELEASE(m_lowResB);
	RELEASE(m_mediumResB);
	RELEASE(m_HighResB);

	RELEASE(m_quitS);
	RELEASE(m_sceneOneS);
	RELEASE(m_sceneTwoS);
	RELEASE(m_sceneThreeS);
	RELEASE(m_sceneFourS);
	RELEASE(m_lowResS);
	RELEASE(m_mediumResS);
	RELEASE(m_HighResS);
	
}

void GuiHandler::changeRes(size_t newResX, size_t newResY, ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_h = newResY;
	m_w = newResX;

	m_sceneOneF = { m_w / 4.0f,0 };
	m_sceneTwoF = { m_w / 4.0f,((float)m_h / 5.0f) * 1 };
	m_sceneThreeF = { m_w / 4.0f,((float)m_h / 5.0f) * 2 };
	m_sceneFourF = { m_w / 4.0f,((float)m_h / 5.0f) * 3 };
	m_quitF = { 0,0 };
	m_resLowF = { 0,((float)m_h / 5.0f) * 1 };
	m_resMediumF = { 0,((float)m_h / 5.0f) * 2 };
	m_resHighF = { 0,((float)m_h / 5.0f) * 3 };
	int size = 24;
	if (m_w < 1280)
	{
		size = 8;
	}
	std::string sizeS = std::to_string(size);
	std::string fontPath = ("../Resources/SpriteFonts/comicSans" + sizeS + ".spritefont");
	std::wstring convFontPath(fontPath.begin(), fontPath.end());
	
	RELEASE(m_sceneOneS);
	RELEASE(m_sceneTwoS);
	RELEASE(m_sceneThreeS);
	RELEASE(m_sceneFourS);
	RELEASE(m_quitS);
	RELEASE(m_lowResS);
	RELEASE(m_mediumResS);
	RELEASE(m_HighResS);
	m_sceneOneS = new SpriteFont(device, convFontPath.c_str());


	m_sceneTwoS = new SpriteFont(device, convFontPath.c_str());


	m_sceneThreeS = new SpriteFont(device, convFontPath.c_str());


	m_sceneFourS = new SpriteFont(device, convFontPath.c_str());

	
	m_quitS = new SpriteFont(device, convFontPath.c_str());


	m_lowResS = new SpriteFont(device, convFontPath.c_str());


	m_mediumResS = new SpriteFont(device, convFontPath.c_str());


	m_HighResS = new SpriteFont(device, convFontPath.c_str());
}

void GuiHandler::init(size_t w, size_t h, ID3D11DeviceContext* context, ID3D11Device* device)
{
	int size = 24;
	if (w < 1280)
	{
		size = 8;
	}
	std::string sizeS = std::to_string(size);
	std::string fontPath = ("../Resources/SpriteFonts/comicSans" + sizeS + ".spritefont");
	std::wstring convFontPath(fontPath.begin(), fontPath.end());
	m_h = h;
	m_w = w;
	m_sceneOneF = { m_w / 4.0f,0 };
	m_sceneTwoF = { m_w / 4.0f,((float)m_h / 5.0f) *1 };
	m_sceneThreeF = { m_w / 4.0f,((float)m_h / 5.0f) * 2 };
	m_sceneFourF = { m_w / 4.0f,((float)m_h / 5.0f) * 3 };
	m_quitF = { 0,0 };
	m_resLowF = { 0,((float)m_h / 5.0f) * 1 };
	m_resMediumF = { 0,((float)m_h / 5.0f) * 2 };
	m_resHighF = { 0,((float)m_h / 5.0f) * 3 };

	m_sceneOneB = new SpriteBatch(context);
	m_sceneOneS = new SpriteFont(device, convFontPath.c_str());

	m_sceneTwoB = new SpriteBatch(context);
	m_sceneTwoS = new SpriteFont(device, convFontPath.c_str());

	m_sceneThreeB = new SpriteBatch(context);
	m_sceneThreeS = new SpriteFont(device, convFontPath.c_str());

	m_sceneFourB = new SpriteBatch(context);
	m_sceneFourS = new SpriteFont(device, convFontPath.c_str());

	m_quitB = new SpriteBatch(context);
	m_quitS = new SpriteFont(device, convFontPath.c_str());

	m_lowResB = new SpriteBatch(context);
	m_lowResS = new SpriteFont(device, convFontPath.c_str());

	m_mediumResB = new SpriteBatch(context);
	m_mediumResS = new SpriteFont(device, convFontPath.c_str());

	m_HighResB = new SpriteBatch(context);
	m_HighResS = new SpriteFont(device, convFontPath.c_str());

}

void GuiHandler::drawEverything(const POINT& mouseP)
{
	m_quitB->Begin();
	m_sceneOneB->Begin();
	m_sceneTwoB->Begin();
	m_sceneThreeB->Begin();
	m_sceneFourB->Begin();
	m_lowResB->Begin();
	m_mediumResB->Begin();
	m_HighResB->Begin();
	XMVECTOR c0 = Colors::White;
	XMVECTOR c1 = Colors::White;
	XMVECTOR c2 = Colors::White;
	XMVECTOR c3 = Colors::White;
	XMVECTOR c4 = Colors::White;
	XMVECTOR c5 = Colors::White;
	XMVECTOR c6 = Colors::White;
	XMVECTOR c7 = Colors::White;
	Vector2 vec2 = m_sceneOneS->MeasureString(m_sceneOneT.c_str());
	if (mouseP.x<m_sceneOneF.x + vec2.x && mouseP.x>m_sceneOneF.x &&
		mouseP.y<m_sceneOneF.y + vec2.y && mouseP.y>m_sceneOneF.y)
	{
		c0 *= 0.65f;
	}
	vec2 = m_quitS->MeasureString(m_quitT.c_str());
	if (mouseP.x<m_quitF.x + vec2.x && mouseP.x>m_quitF.x &&
		mouseP.y<m_quitF.y + vec2.y && mouseP.y>m_quitF.y)
	{
		c1 *= 0.65f;
	}

	vec2 = m_lowResS->MeasureString(m_lowResT.c_str());
	if (mouseP.x<m_resLowF.x + vec2.x && mouseP.x>m_resLowF.x &&
		mouseP.y<m_resLowF.y + vec2.y && mouseP.y>m_resLowF.y)
	{
		c2 *= 0.65f;
	}
	vec2 = m_mediumResS->MeasureString(m_mediumResT.c_str());
	if (mouseP.x<m_resMediumF.x + vec2.x && mouseP.x>m_resMediumF.x &&
		mouseP.y<m_resMediumF.y + vec2.y && mouseP.y>m_resMediumF.y)
	{
		c3 *= 0.65f;
	}
	vec2 = m_HighResS->MeasureString(m_highResT.c_str());
	if (mouseP.x<m_resHighF.x + vec2.x && mouseP.x>m_resHighF.x &&
		mouseP.y<m_resHighF.y + vec2.y && mouseP.y>m_resHighF.y)
	{
		c4 *= 0.65f;
	}
	vec2 = m_sceneTwoS->MeasureString(m_sceneTwoT.c_str());
	if (mouseP.x<m_sceneTwoF.x + vec2.x && mouseP.x>m_sceneTwoF.x &&
		mouseP.y<m_sceneTwoF.y + vec2.y && mouseP.y>m_sceneTwoF.y)
	{
		c5 *= 0.65f;
	}
	vec2 = m_sceneThreeS->MeasureString(m_sceneThreeT.c_str());
	if (mouseP.x<m_sceneThreeF.x + vec2.x && mouseP.x>m_sceneThreeF.x &&
		mouseP.y<m_sceneThreeF.y + vec2.y && mouseP.y>m_sceneThreeF.y)
	{
		c6 *= 0.65f;
	}
	vec2 = m_sceneFourS->MeasureString(m_sceneFourT.c_str());
	if (mouseP.x<m_sceneFourF.x + vec2.x && mouseP.x>m_sceneFourF.x &&
		mouseP.y<m_sceneFourF.y + vec2.y && mouseP.y>m_sceneFourF.y)
	{
		c7 *= 0.65f;
	}
	m_quitS->DrawString(m_quitB,m_quitT.c_str(),m_quitF, c1);
	m_sceneOneS->DrawString(m_sceneOneB, m_sceneOneT.c_str(), m_sceneOneF, c0);
	m_sceneTwoS->DrawString(m_sceneTwoB, m_sceneTwoT.c_str(), m_sceneTwoF, c5);
	m_sceneThreeS->DrawString(m_sceneThreeB, m_sceneThreeT.c_str(), m_sceneThreeF, c6);
	m_sceneFourS->DrawString(m_sceneFourB, m_sceneFourT.c_str(), m_sceneFourF, c7);
	m_lowResS->DrawString(m_lowResB, m_lowResT.c_str(), m_resLowF, c2);
	m_mediumResS->DrawString(m_mediumResB, m_mediumResT.c_str(), m_resMediumF, c3);
	m_HighResS->DrawString(m_HighResB, m_highResT.c_str(), m_resHighF, c4);

	
	m_quitB->End();
	m_sceneOneB->End();
	m_sceneTwoB->End();
	m_sceneThreeB->End();
	m_sceneFourB->End();
	m_lowResB->End();
	m_mediumResB->End();
	m_HighResB->End();
}

const MENU_OPTIONS& GuiHandler::selectionEverything(const POINT& mouseP)
{
	m_option = MENU_OPTIONS::NONE;
	Vector2 vec2 = m_sceneOneS->MeasureString(m_sceneOneT.c_str());
	if (mouseP.x<m_sceneOneF.x + vec2.x && mouseP.x>m_sceneOneF.x &&
		mouseP.y<m_sceneOneF.y + vec2.y && mouseP.y>m_sceneOneF.y)
	{
		m_option = MENU_OPTIONS::SCENE_CEL;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option;
	}
	vec2 = m_quitS->MeasureString(m_quitT.c_str());
	if (mouseP.x<m_quitF.x + vec2.x && mouseP.x>m_quitF.x &&
		mouseP.y<m_quitF.y + vec2.y && mouseP.y>m_quitF.y)
	{
		
		m_option = MENU_OPTIONS::QUIT;
	
		return m_option; //No sound here cus the game quits and it messes up.
	}

	vec2 = m_lowResS->MeasureString(m_lowResT.c_str());
	if (mouseP.x<m_resLowF.x + vec2.x && mouseP.x>m_resLowF.x &&
		mouseP.y<m_resLowF.y + vec2.y && mouseP.y>m_resLowF.y)
	{
		m_option = MENU_OPTIONS::LOW;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option;
	}
	vec2 = m_mediumResS->MeasureString(m_mediumResT.c_str());
	if (mouseP.x<m_resMediumF.x + vec2.x && mouseP.x>m_resMediumF.x &&
		mouseP.y<m_resMediumF.y + vec2.y && mouseP.y>m_resMediumF.y)
	{
		m_option = MENU_OPTIONS::MEDIUM;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option;
	}
	vec2 = m_HighResS->MeasureString(m_highResT.c_str());
	if (mouseP.x<m_resHighF.x + vec2.x && mouseP.x>m_resHighF.x &&
		mouseP.y<m_resHighF.y + vec2.y && mouseP.y>m_resHighF.y)
	{
		m_option = MENU_OPTIONS::HIGH;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option;
	}
	vec2 = m_sceneTwoS->MeasureString(m_sceneTwoT.c_str());
	if (mouseP.x<m_sceneTwoF.x + vec2.x && mouseP.x>m_sceneTwoF.x &&
		mouseP.y<m_sceneTwoF.y + vec2.y && mouseP.y>m_sceneTwoF.y)
	{

		m_option = MENU_OPTIONS::SCENE_LOD;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option;
	}
	vec2 = m_sceneThreeS->MeasureString(m_sceneThreeT.c_str());
	if (mouseP.x<m_sceneThreeF.x + vec2.x && mouseP.x>m_sceneThreeF.x &&
		mouseP.y<m_sceneThreeF.y + vec2.y && mouseP.y>m_sceneThreeF.y)
	{

		m_option = MENU_OPTIONS::SCENE_HEAT;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option; 
	}
	vec2 = m_sceneFourS->MeasureString(m_sceneFourT.c_str());
	if (mouseP.x<m_sceneFourF.x + vec2.x && mouseP.x>m_sceneFourF.x &&
		mouseP.y<m_sceneFourF.y + vec2.y && mouseP.y>m_sceneFourF.y)
	{

		m_option = MENU_OPTIONS::SCENE_ANIMATION;
		SoundHandler::getInstance().play(SOUND_EFFECT::MENU_SELECTION);
		return m_option; 
	}
	
	return m_option;
}
