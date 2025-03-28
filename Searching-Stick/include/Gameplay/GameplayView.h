#pragma once
#include "UI/UIElement/ImageView.h"

namespace Gameplay
{
	class GameplayController;
	
	class GameplayView
	{
	private:
		const float background_alpha = 55.f;
		UI::UIElement::ImageView* background_image;

		GameplayController* gameplay_controller;

		void initializeBackgroundImage();
	public:
		GameplayView();
		~GameplayView();

		void initialize(GameplayController *gameplay_controller);
		void update();
		void render();
	};
}