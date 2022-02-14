#pragma once
#include <graphics/sprite.h>
#include <maths/vector4.h>
#include <vector>
#include <string>
#include <input/touch_input_manager.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class InputManager;
	class SpriteRenderer;
	class Application;
}


class GameObject :
    public gef::Sprite
{
public:
	GameObject();

	void Init(gef::InputManager* in, gef::SpriteRenderer* spriteRen, gef::Platform& window, gef::Vector4 pos, gef::Vector2 size);
    void HandleInput(float frame_time);
    bool Update(float frame_time);
	void Render();

	std::string getInput_inText();
	void setSpritePos(gef::Vector4 newPos);
	gef::Vector4 getSpritePos();
	void setSpriteRGBA(int r, int g, int b, int a);

	gef::Vector2 getMousePos();
	float getAngle();

	gef::Vector2 getStick_L_Axis();
	gef::Vector2 getStick_R_Axis();
	float get_RightStick_Angle();

	void ProcessTouchInput(float frame_time);
	Int32 getTouchID();
	gef::Vector2 getTouch_Pos();


	bool IsInside(const gef::Sprite& sprite, const gef::Vector2& point);



private:
	gef::InputManager* input;
	gef::SpriteRenderer* renderer;

	std::string buttonText;
	gef::Vector2 mousePos;
	gef::Sprite my_sprite_;						//Main sprite


	float speed;
	gef::Vector4 oldPos;

	float angle;

	bool reset = false;
	bool programState = true;

	//Controller variables
	gef::Vector2 L_controller;
	gef::Vector2 R_controller;
	float rightStick_angle;


	//Touch Input
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;
	gef::TouchType current_touch;



	gef::Vector4 newPos, destination;
	gef::Vector4 direction, velocity;
	gef::Vector4 check;
};

