#include "../../include/Gameplay/StickCollection/StickCollectionController.h"
#include "../../include/Gameplay/StickCollection/StickCollectionModel.h"
#include "../../include/Gameplay/StickCollection/StickCollectionView.h"
#include "../../include/Gameplay/StickCollection/Stick.h"
#include "../../include/Global/ServiceLocator.h"

#include <random>

namespace Gameplay
{
	namespace Collection
	{
		float StickCollectionController::calculateStickHeight(int array_pos)
		{
			// scaling its position to a range of 0.0 to 1.0 and multiplying by the maximum element height.
			return (static_cast<float>(array_pos + 1) / collection_model->number_of_elements) * collection_model->max_element_height;
		}
		void StickCollectionController::destroy()
		{
			for (int i = 0; i < sticks.size(); i++) delete(sticks[i]);
			sticks.clear();
			
			delete(collection_model);
			delete(collection_view);
		}
		void StickCollectionController::processLinearSearch()
		{
			for (int i = 0; i < sticks.size(); i++)
			{

				number_of_array_access += 1;
				number_of_comparisons++;

				Global::ServiceLocator::getInstance()->getSoundService()->playSound(Sound::SoundType::COMPARE_SFX);

				if (sticks[i] == stick_to_search)
				{
					stick_to_search->stick_view->setFillColor(collection_model->found_element_color);
					stick_to_search = nullptr;
					return;
				}
				else
				{
					sticks[i]->stick_view->setFillColor(collection_model->processing_element_color);
					sticks[i]->stick_view->setFillColor(collection_model->element_color);
				}
			}
		}
		StickCollectionController::StickCollectionController()
		{
			collection_view = new StickCollectionView();
			collection_model = new StickCollectionModel();

			initializeSticksArray();
		}
		StickCollectionController::~StickCollectionController()
		{
			destroy();
		}
		void StickCollectionController::initialize()
		{
			initializeSticks();
			reset();
		}
		void StickCollectionController::update()
		{
			for (int i = 0; i < sticks.size(); i++)
				sticks[i]->stick_view->update();
		}
		void StickCollectionController::render()
		{
			for (int i = 0; i < sticks.size(); i++)
				sticks[i]->stick_view->render();
		}
		void StickCollectionController::reset()
		{
			shuffleSticks();
			resetSearchStick();
			resetVariables();
			
			updateSticksPosition();
			resetSticksColor();

		}
		SearchType StickCollectionController::getSearchType()
		{
			return search_type;
		}
		int StickCollectionController::getNumberOfSticks()
		{
			return collection_model->number_of_elements;
		}
		void StickCollectionController::searchElement(SearchType search_type)
		{
			switch (search_type)
			{
			case Gameplay::Collection::SearchType::LINEAR_SEARCH:
				processLinearSearch();
				break;
			case Gameplay::Collection::SearchType::BINARY_SEARCH:
				break;
			default:
				break;
			}
		}
		int StickCollectionController::getNumberOfComparisons()
		{
			return number_of_comparisons;
		}
		int StickCollectionController::getNumberOfArrayAccess()
		{
			return number_of_array_access;
		}
		void StickCollectionController::initializeSticks()
		{
			float rectangle_width = calculateStickWidth();			// calculate width


			for (int i = 0; i < collection_model->number_of_elements; i++)    // loop over the vector of sticks
			{
				float rectangle_height = calculateStickHeight(i); 	// calculate height ( assigning the returned value from the calculateStickHeight() method to the variable rectangle_height) 

				sf::Vector2f rectangle_size = sf::Vector2f(rectangle_width, rectangle_height); 	// create a 2D vector 'rectangle_size' to store width and height of rectangle

				// Initialize each stick at 0,0 with the default color 
				// You will update the position of sticks later
				// You will change the color of the sticks later to visually show the search algorithm taking place.
				sticks[i]->stick_view->initialize(rectangle_size, sf::Vector2f(0, 0), 0, collection_model->element_color);
			}
		}
		float StickCollectionController::calculateStickWidth()
		{
			// to get the width of the game window
			float total_space = static_cast<float>(Global::ServiceLocator::getInstance()->getGraphicService()->getGameWindow()->getSize().x);

			// Calculate total spacing as 10% of the total space
			float total_spacing = collection_model->space_percentage * total_space;

			// Calculate the space between each stick
			float space_between = total_spacing / (collection_model->number_of_elements - 1);
			collection_model->setElementSpacing(space_between);

			// Calculate the remaining space for the rectangles
			float remaining_space = total_space - total_spacing;

			// Calculate the width of each rectangle
			float rectangle_width = remaining_space / collection_model->number_of_elements;

			return rectangle_width;
		}
		void StickCollectionController::shuffleSticks()
		{
			// declare a variable 'device' of type std::random_device
	// 'std::random_device is a random number generator that produces non-deterministic random numbers.
			std::random_device device;
			std::mt19937 random_engine(device());

			// shuffle the elements in the sticks collection using the random engine
			std::shuffle(sticks.begin(), sticks.end(), random_engine);
		}
		void StickCollectionController::updateSticksPosition()
		{
			for (int i = 0; i < sticks.size(); i++)
			{
				// to calculate x_position of the current stick based on its index, width and spacing between them
				float x_position = (i * sticks[i]->stick_view->getSize().x) + ((i + 1) * collection_model->elements_spacing);

				// to calculate y_position of the current stick based on fixed element_y_position and stick's height
				float y_position = collection_model->element_y_position - sticks[i]->stick_view->getSize().y;

				// to set the position of stick view
				sticks[i]->stick_view->setPosition(sf::Vector2f(x_position, y_position));
			}
		}
		void StickCollectionController::resetSticksColor()
		{
			for (int i = 0; i < sticks.size(); i++)
				sticks[i]->stick_view->setFillColor(collection_model->element_color);
		}
		void StickCollectionController::resetSearchStick()
		{
			stick_to_search = sticks[std::rand() % sticks.size()];
			stick_to_search->stick_view->setFillColor(collection_model->search_element_color);
		}
		void StickCollectionController::resetVariables()
		{
			number_of_comparisons = 0;
			number_of_array_access = 0;
		}
		void StickCollectionController::initializeSticksArray()
		{
			for (int i = 0; i < collection_model->number_of_elements; i++)
				sticks.push_back(new Stick(i));
		}
	}
}