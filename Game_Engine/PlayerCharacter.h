#pragma once
#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H
#include "CharacterHandler.h"
#include "Timer.h"
#include "InputHandler.h"
class PlayerCharacter :
    public CharacterHandler
{
private:
    PLAYER_TYPE m_playerType = PLAYER_TYPE::FIRST_PERSON;
    Camera* m_cam;
    Timer m_timer;
    float m_mouseX, m_mouseY, m_mouseSensitivity, m_walkSpeed, m_turnSpeed, m_runSpeed;
    Quaternion m_rotation;
    Vector3 m_forward;
    Vector3 m_up;
    Vector3 m_camPos;
    Vector3 m_charDirection;
    Vector3 m_right;
    Vector3 m_charPos;
    Vector3 m_oldCharDirection;
    Vector3 m_cameraTarget;
    Vector3 m_eulerRot;
    bool m_freeMouse = false;
public:
    PlayerCharacter();
    void setCam(Camera* camera) { m_cam = camera; }
    Camera* getCam() { return m_cam; }
    void updateCam();
    // Inherited via CharacterHandler
    virtual Shape* getCharacterShape() override;
    virtual Shape* getClothing() override;
    virtual Shape* getWeapon() override;
    virtual void setCharacterShape(Shape* shape) override;
    virtual void setClothShape(Shape* shape) override;
    virtual void setWeaponrShape(Shape* shape) override;
    void setType(PLAYER_TYPE type) { m_playerType = type; }
    void updateIMGUI(float mouseSense, float walkSpeed, float turnSpeed, float runSpeed) { m_mouseSensitivity = mouseSense; m_walkSpeed = walkSpeed; m_turnSpeed = turnSpeed; m_runSpeed = runSpeed; };
};

#endif