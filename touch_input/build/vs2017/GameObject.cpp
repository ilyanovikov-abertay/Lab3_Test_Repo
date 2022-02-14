#define PI 3.14159265358979323846 //pi

#include "GameObject.h"

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <input/sony_controller_input_manager.h>


GameObject::GameObject() :
	input(NULL),
	renderer(NULL),
	active_touch_id_(-1)
{
}


void GameObject::Init(gef::InputManager* in, gef::SpriteRenderer* spriteRen, gef::Platform& window, gef::Vector4 pos, gef::Vector2 size)
{
	//input = gef::InputManager::Create(platform_);
	input = in;
	renderer = spriteRen;
	

	//Main sprite
	my_sprite_.set_position(window.width() * 0.5f, window.height() * 0.5f, 0.0f);
	oldPos = my_sprite_.position();
	my_sprite_.set_width(size.x);
	my_sprite_.set_height(size.y);
	
	speed = 100.0f;
}

void GameObject::HandleInput(float frame_time)
{
	std::vector<gef::Keyboard::KeyCode> keyVector;

	for (int i = 0; i < 103; i++)
	{
		keyVector.push_back((gef::Keyboard::KeyCode)i);
	}

	//Check if keyboard is valid
	if (input->keyboard())
	{
		for (auto it : keyVector)
		{
			if (input->keyboard()->IsKeyPressed(it))
			{
				buttonText = std::to_string(it);

				break;
			}
		}

		//Sprite Movement
		if (input->keyboard()->IsKeyDown(gef::Keyboard::KeyCode::KC_D))
		{
			my_sprite_.set_position(my_sprite_.position().x() + speed * frame_time, my_sprite_.position().y(), my_sprite_.position().z());
		}
		if (input->keyboard()->IsKeyDown(gef::Keyboard::KeyCode::KC_A))
		{
			my_sprite_.set_position(my_sprite_.position().x() - speed * frame_time, my_sprite_.position().y(), my_sprite_.position().z());
		}
		if (input->keyboard()->IsKeyDown(gef::Keyboard::KeyCode::KC_W))
		{
			my_sprite_.set_position(my_sprite_.position().x(), my_sprite_.position().y() - speed * frame_time, my_sprite_.position().z());
		}
		if (input->keyboard()->IsKeyDown(gef::Keyboard::KeyCode::KC_S))
		{
			my_sprite_.set_position(my_sprite_.position().x(), my_sprite_.position().y() + speed * frame_time, my_sprite_.position().z());
		}


		//Snap the sprite back to the default position
		if (input->keyboard()->IsKeyPressed(gef::Keyboard::KeyCode::KC_SPACE))
		{
			my_sprite_.set_position(oldPos);
			my_sprite_.set_colourRGBA(255, 255, 255, 255);
			current_touch = gef::TT_NONE;
		}

		if (input->keyboard()->IsKeyPressed(gef::Keyboard::KC_ESCAPE))
		{
			programState = false;
		}
	}

	//Touch Input
	if (input->touch_manager())
	{
		//mousePos.x = input->touch_manager()->mouse_position().x;
		//mousePos.y = input->touch_manager()->mouse_position().y;
		ProcessTouchInput(frame_time);
	}
	
	//Check if controller is valid
	if (input->controller_input())
	{
		const gef::SonyController* controller = input->controller_input()->GetController(0);
		if (controller)
		{
			//Button press
			if (controller->buttons_down() & gef_SONY_CTRL_TRIANGLE)
			{
				buttonText = "button";
			}
			else
			{
				buttonText = "";
			}

			//Analogue joysticks check
			L_controller.x = controller->left_stick_x_axis();
			L_controller.y = controller->left_stick_y_axis();

			R_controller.x = controller->right_stick_x_axis();
			R_controller.y = controller->right_stick_y_axis();

			//Right stick angle calculation
			rightStick_angle = atan2(R_controller.x, -R_controller.y) * 180 / PI;
			if (rightStick_angle < 0)
			{
				rightStick_angle = 180 + (180 + rightStick_angle);
			}
			if (!controller->right_stick_x_axis() && !controller->right_stick_y_axis())
			{
				rightStick_angle = 0;
			}

			//Sprite Movement
			if (controller->buttons_down() & gef_SONY_CTRL_RIGHT)
			{
				my_sprite_.set_position(my_sprite_.position().x() + speed * frame_time, my_sprite_.position().y(), my_sprite_.position().z());
			}
			if (controller->buttons_down() & gef_SONY_CTRL_LEFT)
			{
				my_sprite_.set_position(my_sprite_.position().x() - speed * frame_time, my_sprite_.position().y(), my_sprite_.position().z());
			}
			if (controller->buttons_down() & gef_SONY_CTRL_UP)
			{
				my_sprite_.set_position(my_sprite_.position().x(), my_sprite_.position().y() - speed * frame_time, my_sprite_.position().z());
			}
			if (controller->buttons_down() & gef_SONY_CTRL_DOWN)
			{
				my_sprite_.set_position(my_sprite_.position().x(), my_sprite_.position().y() + speed * frame_time, my_sprite_.position().z());
			}

			//Snap the sprite back to the default position
			if (controller->buttons_pressed() & gef_SONY_CTRL_TRIANGLE)
			{
				my_sprite_.set_position(oldPos);
			}


			//Terminate program
			if (controller->buttons_pressed() & (gef_SONY_CTRL_SELECT | gef_SONY_CTRL_START) == (gef_SONY_CTRL_SELECT | gef_SONY_CTRL_START))
			{
				programState = false;
			}
		}
	}
}


bool GameObject::Update(float frame_time)
{
	HandleInput(frame_time);

	if (programState == false)
	{
		return false;
	}

	newPos = gef::Vector4(touch_position_.x, touch_position_.y, my_sprite_.position().z());

	switch (current_touch)
	{
	case gef::TT_NEW:
		//'Teleport' sprite to a new position
		//my_sprite_.set_position(newPos);
		
		break;
	case gef::TT_ACTIVE:
			//setSpriteRGBA(255, 0, 0, 255);
			///*		destination.Lerp(my_sprite_.position(), newPos, frame_time);
			//		setSpritePos(destination);*/

			//direction = gef::Vector4(newPos.x() - getSpritePos().x(), newPos.y() - getSpritePos().y(), getSpritePos().z());
			//direction.Normalise();
			//velocity = (direction * 200.0f);
			//setSpritePos(getSpritePos() + (velocity * frame_time));

		break;
	case gef::TT_RELEASED:
		////Set colour to green 
		//	setSpriteRGBA(0, 255, 0, 255);
		//	/*		destination.Lerp(my_sprite_.position(), newPos, frame_time);
		//			setSpritePos(destination);*/

		//	direction = gef::Vector4(newPos.x() - getSpritePos().x(), newPos.y() - getSpritePos().y(), getSpritePos().z());
		//	direction.Normalise();
		//	velocity = (direction * 200.0f);
		//	setSpritePos(getSpritePos() + (velocity * frame_time));
		break;
	default:
		break;
	}

	if (gef::Vector4(my_sprite_.position() - newPos).Length() < 0.1f)
	{
		my_sprite_.set_position(newPos.x(), newPos.y(), getSpritePos().z());
		my_sprite_.set_position(my_sprite_.position());
		current_touch = gef::TT_NONE;

	}



	if (IsInside(my_sprite_, touch_position_) == true)
	{
		buttonText = " Inside";
	}
	else
	{
		buttonText = " Outside";
	}






	angle = atan2( my_sprite_.position().x() - mousePos.x, my_sprite_.position().y() - mousePos.y) * -180 / PI;
	if (angle < 0)
	{
		angle = 180 + (180 + angle);
	}

	//Set rotation of a sprite based on the angle between the mouse and the sprite
	//my_sprite_.set_rotation(angle * PI / 180);
	
	//Set rotation based on the angle right stick is at
	//my_sprite_.set_rotation(rightStick_angle * PI / 180);

	return true;
}



void GameObject::Render()
{
	// draw all sprites between the Begin() and End() calls
	renderer->Begin();

	// draw my sprite here
	renderer->DrawSprite(my_sprite_);

	
	renderer->End();
}



std::string GameObject::getInput_inText()
{
	return buttonText;
}

void GameObject::setSpritePos(gef::Vector4 newPos)
{
	my_sprite_.set_position(newPos);
}

gef::Vector4 GameObject::getSpritePos()
{
	return my_sprite_.position();
}


void GameObject::setSpriteRGBA(int r, int g, int b, int a)
{
	my_sprite_.set_colourRGBA(r, g, b, a);
}

gef::Vector2 GameObject::getMousePos()
{
	return mousePos;
}

float GameObject::getAngle()
{
	return angle;
}



gef::Vector2 GameObject::getStick_L_Axis()
{
	return L_controller;
}


gef::Vector2 GameObject::getStick_R_Axis()
{
	return R_controller;
}

float GameObject::get_RightStick_Angle()
{
	return rightStick_angle;
}







void GameObject::ProcessTouchInput(float frame_time)
{
	const gef::TouchInputManager* touch_input = input->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			// if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					// do any processing for a new touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;
					current_touch = touch->type;
				
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;
					current_touch = touch->type;
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything here apart from resetting the active touch id
					current_touch = touch->type;
					active_touch_id_ = -1;
				}
			}
		}
	}
}


Int32 GameObject::getTouchID()
{
	return active_touch_id_;
}
gef::Vector2 GameObject::getTouch_Pos()
{
	return touch_position_;
}


bool GameObject::IsInside(const gef::Sprite& sprite, const gef::Vector2& point)
{
	gef::Vector2 topLeft = gef::Vector2(sprite.position().x() - sprite.width() / 2.0f, sprite.position().y() - sprite.height() / 2.0f);
	gef::Vector2 topRight = gef::Vector2(sprite.position().x() + sprite.width() / 2.0f, sprite.position().y() - sprite.height() / 2.0f);
	gef::Vector2 bottomRight = gef::Vector2(sprite.position().x() + sprite.width() / 2.0f, sprite.position().y() + sprite.height() / 2.0f);
	gef::Vector2 bottomLeft = gef::Vector2(sprite.position().x() - sprite.width() / 2.0f, sprite.position().y() + sprite.height() / 2.0f);

	if ((point.x >= topLeft.x && point.x <= topRight.x) && ((point.x >= bottomLeft.x) && (point.x <= bottomRight.x)))
	{
		if ((point.y >= topLeft.y && point.x >= topRight.y) && ((point.y <= bottomLeft.y) && (point.y <= bottomRight.y)))
		{
			return true;
		}
	}
	
	return false;
}