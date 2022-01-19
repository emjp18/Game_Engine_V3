#include "stdafx.h"
#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter()
{
    m_timer.start();
    m_mouseX = 0.0f;
    m_mouseY = 0.0f;
    m_mouseSensitivity = 0.006f;

    m_rotation = Quaternion::Identity;
    m_forward = XMFLOAT3(0,0,1);
    m_up = XMFLOAT3(0,1,0);
    m_camPos = XMFLOAT3(50, 200, 50);
    m_walkSpeed = 5.50f;
    m_charDirection = XMFLOAT3(0, 0, 0);
    m_runSpeed = 10.0f;
    m_turnSpeed = 10.06f;
    m_right = XMFLOAT3(1, 0, 0);
    m_charPos = XMFLOAT3(0, 0, 0);
    m_oldCharDirection = XMFLOAT3(0, 0, 0);
    m_cameraTarget = XMFLOAT3(0, 0, 0);
    m_eulerRot = XMFLOAT3(0, 0, 0);
}

void PlayerCharacter::updateCam()
{
    m_timer.run();
  
    
    if (InputHandler::getInstance().GetKey(Keyboard::Keys::E))
    {
        m_freeMouse = !m_freeMouse;
    }

    if (m_freeMouse)
    {
        if (!InputHandler::getInstance().GetIsVisible())
        {
           
            InputHandler::getInstance().FreeMouse();
            InputHandler::getInstance().SetMouseMode(Mouse::MODE_ABSOLUTE);
        }
       
        
    }

    else
    {
        if (InputHandler::getInstance().GetIsVisible())
        {
            InputHandler::getInstance().ConfineMouse();
            InputHandler::getInstance().SetMouseMode(Mouse::MODE_RELATIVE);
        }
        if (InputHandler::getInstance().isScrollingDown())
        {
            float fov = m_cam->getFOV();
            fov -= 20.0f;
            if (fov < 1.0f)
                fov = 1.0f;
            if (fov > 90.0f)
                fov = 90.0f;
            m_cam->changeFOV(fov);
        }
        else if (InputHandler::getInstance().isScrollingUp())
        {
            float fov = m_cam->getFOV();
            fov += 20.0f;
            if (fov < 1.0f)
                fov = 1.0f;
            if (fov > 90.0f)
                fov = 90.0f;
            m_cam->changeFOV(fov);
        }
        switch (m_playerType)
        {
        default:
        {
            break;
        }
        case PLAYER_TYPE::FIRST_PERSON:
        {
            if (!m_cam)
            {
                OutputDebugStringA("\nLOSTCAM\n");
            }
            /*m_camPos = m_cam->getLocalTranformations().trans;*/
            m_mouseX = (float)InputHandler::getInstance().g_mouseX;
            m_mouseY = (float)InputHandler::getInstance().g_mouseY;
            float dt = (float)m_timer.getLerpedDT();
            m_mouseX *= m_mouseSensitivity * dt;
            m_mouseY *= m_mouseSensitivity * dt;

            m_rotation = m_rotation.CreateFromYawPitchRoll(m_mouseX, m_mouseY, 0.0f);
            m_forward = XMVector3Rotate({ 0,0,1 }, m_rotation);
            m_forward.Normalize();
            m_up = XMVector3Rotate({ 0,1,0 }, m_rotation);
            m_up.Normalize();


            Vector3 oldPos = m_camPos;
            if (KEY_DOWN(Keyboard::Keys::W)&& KEY_DOWN(Keyboard::Keys::Space))
            {

                m_camPos += XMVector3Rotate({ 0,0,1 }, m_rotation) * m_runSpeed * dt;

            }
            else if (KEY_DOWN(Keyboard::Keys::W))
            {
                m_camPos += XMVector3Rotate({ 0,0,1 }, m_rotation) * m_walkSpeed * dt;
            }

            if (KEY_DOWN(Keyboard::Keys::S) && KEY_DOWN(Keyboard::Keys::Space))
            {

                m_camPos -= XMVector3Rotate({ 0,0,1 }, m_rotation) * m_runSpeed * dt;

            }
            else if (KEY_DOWN(Keyboard::Keys::S))
            {
                m_camPos -= XMVector3Rotate({ 0,0,1 }, m_rotation) * m_walkSpeed * dt;
            }

            if (KEY_DOWN(Keyboard::Keys::A) && KEY_DOWN(Keyboard::Keys::Space))
            {

                m_camPos -= XMVector3Rotate({ 1,0,0 }, m_rotation) * m_runSpeed * dt;

            }
            else if (KEY_DOWN(Keyboard::Keys::A))
            {
                m_camPos -= XMVector3Rotate({ 1,0,0 }, m_rotation) * m_walkSpeed * dt;
            }

            if (KEY_DOWN(Keyboard::Keys::D) && KEY_DOWN(Keyboard::Keys::Space))
            {

                m_camPos += XMVector3Rotate({ 1,0,0 }, m_rotation) * m_runSpeed * dt;

            }
            else if (KEY_DOWN(Keyboard::Keys::D))
            {
                m_camPos += XMVector3Rotate({ 1,0,0 }, m_rotation) * m_walkSpeed * dt;
            }


            


            if (isnan(m_charDirection.x) || isnan(m_charDirection.y) || isnan(m_charDirection.z))
            {
                m_camPos = oldPos;
            }

            Vector3 posFor = m_camPos + m_forward;
            m_cam->updateViewMatrix(posFor, m_forward, m_up);
            break;
        }
        case PLAYER_TYPE::THIRD_PERSON:
        {
            if (!m_cam)
            {
                OutputDebugStringA("\nLOSTCAM\n");
            }
            m_mouseX = (float)InputHandler::getInstance().g_mouseX;
            m_mouseY = (float)InputHandler::getInstance().g_mouseY;

            m_charDirection.x = 0.0f;
            m_charDirection.y = 0.0f;
            m_charDirection.z = 0.0f;

            float dt = (float)m_timer.getLerpedDT();
            Vector3 defaultForward = XMFLOAT3(0, 0, 1);
            Vector3 defaultRight = XMFLOAT3(1, 0, 0);
            m_mouseX *= m_mouseSensitivity * dt;
            m_mouseY *= m_mouseSensitivity * dt;
            bool isMoving = false;
            Vector3 oldPos = m_camPos;
            if (KEY_DOWN(Keyboard::Keys::W))
            {
                m_charDirection += m_forward;
                isMoving = true;
            }
            //m_camPosition.z += 1.f;
            if (KEY_DOWN(Keyboard::Keys::S))
            {
                m_charDirection -= m_forward;
                isMoving = true;

            }
            //m_camPosition.z -= 1.f;
            if (KEY_DOWN(Keyboard::Keys::A))
            {
                m_charDirection += m_right;
                isMoving = true;

            }
            //m_camPosition.x -= 1.f;
            if (KEY_DOWN(Keyboard::Keys::D))
            {
                m_charDirection -= m_right;
                isMoving = true;


            }

            if (isMoving && m_characterShape->g_canMove)
            {
                /*DC_SKELETON* plaSkel = dynamic_cast<DC_SKELETON*>(m_characterShape->getDrawCall());
                DC_SKELETON* clothSkel = dynamic_cast<DC_SKELETON*>(m_clothes->getDrawCall());
                if (plaSkel) {
                    plaSkel->g_animations[G_S_RUN]->play();
                    plaSkel->g_currentTrack = G_S_RUN;
                }
                  

                if (clothSkel)
                {
                    clothSkel->g_animations[G_S_RUN]->play();
                    clothSkel->g_currentTrack = G_S_RUN;
                }*/
            }


            //
            // Third Person Camera
            m_charPos = m_characterShape->getLocalTranformations().trans;

            m_charDirection = XMVector3Normalize(m_charDirection);

            if (XMVectorGetX(XMVector3Dot(m_charDirection, m_oldCharDirection)) == -1)
                m_oldCharDirection += XMVectorSet(0.02f, 0.0f, -0.02f, 0.0f);


            Vector3 currCharDirection = m_oldCharDirection + (m_charDirection * m_turnSpeed * dt);

            currCharDirection = XMVector3Normalize(currCharDirection);

            float charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(defaultForward)));
            if (XMVectorGetY(XMVector3Cross(currCharDirection, defaultForward)) > 0.0f)
                charDirAngle = -charDirAngle;
            //
            m_eulerRot.x = (m_charDirection.x * cosf(charDirAngle)) - (m_charDirection.x * sinf(charDirAngle));
            m_eulerRot.y = (m_charDirection.y * sinf(charDirAngle)) + (m_charDirection.y * cosf(charDirAngle));
            m_eulerRot.z = 0;
            if (charDirAngle > 0)
            {
                m_eulerRot *= -1;
            }


            m_characterShape->setRotationQuat(Quaternion::CreateFromYawPitchRoll(m_eulerRot.x, m_eulerRot.y, 0));

            //
            
            if (isMoving && m_characterShape->g_canMove&& KEY_DOWN(Keyboard::Keys::Space))
            {
                Vector3 pos = m_charPos + (m_charDirection * m_runSpeed * dt);
                pos.y = m_characterShape->getLocalTranformations().trans.y;
                m_characterShape->setTranslation(pos);


            }
            else if (isMoving && m_characterShape->g_canMove)
            {
                Vector3 pos = m_charPos + (m_charDirection * m_walkSpeed * dt);
                pos.y = m_characterShape->getLocalTranformations().trans.y;
                m_characterShape->setTranslation(pos);
            }
            m_oldCharDirection = currCharDirection;

            // Set the cameras target to be looking at the character.




            m_cameraTarget = m_charPos;

            m_cameraTarget.z += 150;

            // This line is because this lessons model was set to stand on the point (0,0,0) (my bad), and we
            // don't want to just be looking at the models feet, so we move the camera's target vector up 5 units

            m_cameraTarget = XMVectorSetY(m_cameraTarget, XMVectorGetY(m_cameraTarget) + 0);



            if (m_mouseY < -0.4f)
            {
                m_mouseY = -0.4f;
            }
            if (m_mouseY > 0.4f)
            {
                m_mouseY = 0.4f;
            }

            Matrix camRotationMatrix = XMMatrixRotationRollPitchYaw(-m_mouseY, m_mouseX, 0);
            m_camPos = XMVector3TransformNormal(defaultForward, camRotationMatrix);
            m_camPos = XMVector3Normalize(m_camPos);



            // Set our cameras position to rotate around the character. We need to add 5 to the characters
            // position's y axis because i'm stupid and modeled the character in the 3d modeling program
            // to be "standing" on (0,0,0), instead of centered around it ;) Well target her head here though
            m_camPos = (m_camPos * 10) + m_cameraTarget;

            // We need to set our cameras forward and right vectors to lay
            // in the worlds xz plane, since they are the vectors we will
            // be using to determine the direction our character is running
            m_forward = XMVector3Normalize(m_cameraTarget - m_camPos);    // Get forward vector based on target
            m_forward = XMVectorSetY(m_forward, 0.0f);    // set forwards y component to 0 so it lays only on
            // the xz plane
            m_forward = XMVector3Normalize(m_forward);
            // To get our camera's Right vector, we set it's x component to the negative z component from the
            // camera's forward vector, and the z component to the camera forwards x component
            m_right = XMVectorSet(-XMVectorGetZ(m_forward), 0.0f, XMVectorGetX(m_forward), 0.0f);

            // Our camera does not "roll", so we can safely assume that the cameras right vector is always
            // in the xz plane, so to get the up vector, we just get the normalized vector from the camera
            // position to the cameras target, and cross it with the camera's Right vector
            m_up = XMVector3Cross(XMVector3Normalize(m_camPos - m_cameraTarget), m_right);
            //

            if (isnan(m_camPos.x) || isnan(m_camPos.y) || isnan(m_camPos.z))
            {
                m_camPos = oldPos;
                OutputDebugStringA("\nLOSTCAM\n");
            }

            m_cam->updateViewMatrix(m_camPos, m_forward, m_up);
            break;
        }
        }
    }
    
}

Shape* PlayerCharacter::getCharacterShape()
{
    return m_characterShape;
}

Shape* PlayerCharacter::getClothing()
{
    return m_clothes;
}

Shape* PlayerCharacter::getWeapon()
{
    return m_weapon;
}

void PlayerCharacter::setCharacterShape(Shape* shape)
{
    m_characterShape = shape;
}

void PlayerCharacter::setClothShape(Shape* shape)
{
    m_clothes = shape;
}

void PlayerCharacter::setWeaponrShape(Shape* shape)
{
    m_weapon = shape;
}
