#include "Gameplay/GameplayView.h"
#include "Gameplay/GameplayController.h"
#include "Global/ServiceLocator.h"
#include "Global/Config.h"

namespace Gameplay
{
	
	GameplayView::GameplayView()
	{
		background_image = nullptr;
		background_image = new UI::UIElement::ImageView();
	}
	GameplayView::~GameplayView()
	{
		delete(background_image);
	}
	void GameplayView::initializeBackgroundImage()
	{
		sf::RenderWindow* game_window = Global::ServiceLocator::getInstance()->getGraphicService()->getGameWindow();

		background_image->initialize(Global::Config::background_texture_path,
			game_window->getSize().x,
			game_window->getSize().y,
			sf::Vector2f(0, 0));

		background_image->setImageAlpha(background_alpha);
	}
	void GameplayView::initialize(GameplayController *gameplay_controller)
	{
		this->gameplay_controller = gameplay_controller;

		initializeBackgroundImage();
	}
	void GameplayView::update()
	{
		background_image->update();
	}
	void GameplayView::render()
	{
		background_image->render();
	}
}