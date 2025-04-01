#pragma once
#include "UI/Interface/IUIController.h"
#include "UI/UIElement/ImageView.h"
#include "UI/UIElement/ButtonView.h"
#include "UI/UIElement/TextView.h"
#include "Gameplay/GameplayService.h"

namespace UI
{
	namespace GameplayUI
	{
		class GameplayUIController : public Interface::IUIController
		{
		private:
			const float font_size = 40.f;

			const float text_y_position = 36.f;
			const float text_y_pos2 = 76.f;


			const float search_type_text_x_position = 60.f;
			const float comparisons_text_x_position = 687.f;
			const float array_access_text_x_position = 1250.f;
			const float num_sticks_text_x_position = 60.f;
			const float delay_text_x_position = 687.f;
			const float time_complexity_text_x_position = 1250.f;

			const float menu_button_x_position = 1770.f;
			const float menu_button_y_position = 33.f;

			const float menu_button_width = 100.f;
			const float menu_button_height = 50.f;


			UI::UIElement::TextView* search_type_text;
			UI::UIElement::TextView* number_of_comparisons_text;
			UI::UIElement::TextView* number_of_array_access_text;
			UI::UIElement::TextView* num_sticks_text;
			UI::UIElement::TextView* delay_text;
			UI::UIElement::TextView* time_complexity_text;

			UI::UIElement::ButtonView* menu_button;

			Gameplay::Collection::SearchType current_search_type;

			void createButton();
			void createTexts();
			void initializeButton();
			void initializeTexts();

			void updateSearchTypeText();
			void updateComparisonsText();
			void updateArrayAccessText();

			void updateNumberOfSticksText();
			void updateDelayText();
			void updateTimeComplexityText();

			void menuButtonCallback();
			void registerButtonCallback();

			void destroy();

		public:
			GameplayUIController();
			~GameplayUIController();

			void initialize()override;
			void update()override;
			void render()override;
			void show()override;
		};
	}
}