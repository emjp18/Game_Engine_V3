#pragma once
#ifndef CHARACTERHANDLER_H
#define CHARACTERHANDLER_H
#include "Shape.h"

class CharacterHandler
{
private:
	
protected:
	Shape* m_characterShape;
	Shape* m_clothes;
	Shape* m_weapon;
public:
	virtual Shape* getCharacterShape() = 0;
	virtual Shape* getClothing() = 0;
	virtual Shape* getWeapon() = 0;
	virtual void setCharacterShape(Shape* shape) = 0;
	virtual void setClothShape(Shape* shape) = 0;
	virtual void setWeaponrShape(Shape* shape) = 0;
};

#endif