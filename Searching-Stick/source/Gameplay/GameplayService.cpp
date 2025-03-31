#include "../../include/Gameplay/GameplayService.h"
#include "../../include/Gameplay/GameplayController.h"

Gameplay::GameplayService::GameplayService()
{
	gameplay_controller = new GameplayController();
}

Gameplay::GameplayService::~GameplayService()
{
	delete(gameplay_controller);
}

void Gameplay::GameplayService::initialize()
{
	gameplay_controller->initialize();
}

void Gameplay::GameplayService::update()
{
	gameplay_controller->update();
}

void Gameplay::GameplayService::render()
{
	gameplay_controller->render();
}

void Gameplay::GameplayService::reset()
{
	gameplay_controller->reset();
}

void Gameplay::GameplayService::searchElement(Collection::SearchType search_type)
{
	collection_controller->searchElement(search_type);
}
