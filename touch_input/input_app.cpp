#include "input_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <system/debug_log.h>


InputApp::InputApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	input_manager_(NULL)
	//active_touch_id_(-1)
{
}

void InputApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	// make sure if there is a panel to detect touch input, then activate it
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
		input_manager_->touch_manager()->EnablePanel(0);

	InitFont();

	gameObject.Init(input_manager_, sprite_renderer_, platform_, gef::Vector4(0, 0,0), gef::Vector2(75, 75));
}

void InputApp::CleanUp()
{
	CleanUpFont();

	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}

bool InputApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	if (input_manager_)
	{
		input_manager_->Update();

		//ProcessTouchInput(frame_time);
		gameObject.Update(frame_time);
	}

	return true;
}

void InputApp::Render()
{
	sprite_renderer_->Begin();

	gameObject.Render();
	DrawHUD();
	sprite_renderer_->End();
}
void InputApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void InputApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void InputApp::DrawHUD()
{
	if (font_)
	{
		// if a touch is active lets draw some text
		if (gameObject.getTouchID() != -1)
		{
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(gameObject.getTouch_Pos().x, gameObject.getTouch_Pos().y, -0.9f),
				1.0f, 0xffff00ff, gef::TJ_LEFT,
				"(%.1f, %.1f)",
				gameObject.getTouch_Pos().x, gameObject.getTouch_Pos().y);
		}

		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(0.0f, 32.0f, -0.9f),
			1.0f, 0xffffffff, gef::TJ_LEFT,
			"Point: %s",
			gameObject.getInput_inText().c_str()
		);

		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

//void InputApp::ProcessTouchInput(float frame_time)
//{
//	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
//	if (touch_input && (touch_input->max_num_panels() > 0))
//	{
//		// get the active touches for this panel
//		const gef::TouchContainer& panel_touches = touch_input->touches(0);
//
//		// go through the touches
//		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
//		{
//			// if active touch id is -1, then we are not currently processing a touch
//			if (active_touch_id_ == -1)
//			{
//				// check for the start of a new touch
//				if (touch->type == gef::TT_NEW)
//				{
//					active_touch_id_ = touch->id;
//
//					// do any processing for a new touch here
//					// we're just going to record the position of the touch
//					touch_position_ = touch->position;
//
//					//'Teleport' sprite to a new position
//					gef::Vector4 newPos = gef::Vector4(touch_position_.x, touch_position_.y, gameObject.getSpritePos().z());
//					gameObject.setSpritePos(newPos);
//					
//				}
//			}
//			else if (active_touch_id_ == touch->id)
//			{
//				// we are processing touch data with a matching id to the one we are looking for
//				if (touch->type == gef::TT_ACTIVE)
//				{
//					// update an active touch here
//					// we're just going to record the position of the touch
//					touch_position_ = touch->position;
//
//					gameObject.setSpriteRGBA(255, 0, 0, 255);
//
//				/*	gef::Vector4 direction, velocity;
//					direction = gef::Vector4(touch_position_.x - gameObject.getSpritePos().x(), touch_position_.y - gameObject.getSpritePos().y(), gameObject.getSpritePos().z());
//					direction.Normalise();
//					velocity = (direction * 200.0f);
//					gameObject.setSpritePos(gameObject.getSpritePos() + (velocity * frame_time));*/
//
//
//					
//				}
//				else if (touch->type == gef::TT_RELEASED)
//				{
//					// the touch we are tracking has been released
//					// perform any actions that need to happen when a touch is released here
//					// we're not doing anything here apart from resetting the active touch id
//					active_touch_id_ = -1;
//					gameObject.setSpriteRGBA(0, 255, 0, 255);
//					
//					gef::Vector4 destination;
//					destination.Lerp(gameObject.getSpritePos(), gef::Vector4(touch_position_.x, touch_position_.y, gameObject.getSpritePos().z()), frame_time);
//					gameObject.setSpritePos(destination);
//				}
//			}
//		}
//	}
//}
//
//
