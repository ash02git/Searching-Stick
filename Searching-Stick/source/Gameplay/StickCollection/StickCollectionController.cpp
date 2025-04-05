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
			if (search_thread.joinable()) 
				search_thread.join();
			
			for (int i = 0; i < sticks.size(); i++) delete(sticks[i]);
			sticks.clear();
			
			delete(collection_model);
			delete(collection_view);
		}
		void StickCollectionController::processLinearSearch()
		{
			Sound::SoundService* sound_service = Global::ServiceLocator::getInstance()->getSoundService();
			for (int i = 0; i < sticks.size(); i++)
			{


				number_of_array_access += 1;		// keeps track of the number of sticks array is accessed
				number_of_comparisons++;				// keeps track of the number of comparisons made between target stick and another stick

				sound_service->playSound(Sound::SoundType::COMPARE_SFX);				// play the comparision sound

				if (sticks[i] == stick_to_search)			// condition to check if the current stick is the target stick
				{
					// if the target stick is found, this line of code sets the fill colour of the target's stick view
					stick_to_search->stick_view->setFillColor(collection_model->found_element_color);
					stick_to_search = nullptr;			// sets the pointer to null; meaning the search is completed.
					return;
				}
				else
				{
					// sets the fill color of the current stick's view to the processing_element_color; meaning the stick is still being checked
					sticks[i]->stick_view->setFillColor(collection_model->processing_element_color);

					//pauses the thread for a small duration to show the searching operation
					std::this_thread::sleep_for(std::chrono::milliseconds(current_operation_delay));

					// sets the fill color of the current stick's view back to the default element_color after the pause.
					sticks[i]->stick_view->setFillColor(collection_model->element_color);
				}
			}
		}
		void StickCollectionController::processBinarySearch()
		{
			int left = 0;
			int right = sticks.size();

			Sound::SoundService* sound_service = Global::ServiceLocator::getInstance()->getSoundService();
			while (left < right)
			{
				int mid = left + (right - left) / 2;
				number_of_array_access += 2;
				number_of_comparisons++;

				sound_service->playSound(Sound::SoundType::COMPARE_SFX);

				if (sticks[mid] == stick_to_search)
				{
					sticks[mid]->stick_view->setFillColor(collection_model->found_element_color);
					stick_to_search = nullptr;
					return;
				}

				sticks[mid]->stick_view->setFillColor(collection_model->processing_element_color);
				std::this_thread::sleep_for(std::chrono::milliseconds(current_operation_delay));
				sticks[mid]->stick_view->setFillColor(collection_model->element_color);

				number_of_array_access++;
				if (sticks[mid]->data <= stick_to_search->data) left = mid;
				else right = mid;
			}
		}
		void StickCollectionController::joinThreads()
		{
			search_thread.join();
		}
		void StickCollectionController::processSearchThreadState()
		{
			if (search_thread.joinable() && stick_to_search == nullptr)
			{
				joinThreads();
			}
		}
		void StickCollectionController::sortElements()
		{
			std::sort(sticks.begin(), sticks.end(), [this](const Stick* a, const Stick* b) { return compareElementsByData(a, b); });//error fix here

			updateSticksPosition();
		}
		bool StickCollectionController::compareElementsByData(const Stick* a, const Stick* b)
		{
			return a->data < b->data;
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
			collection_model->initialize();
			
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
			current_operation_delay = 0;

			if (search_thread.joinable()) 
				search_thread.join();
			
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
			resetSearchStick();//new line

			this->search_type = search_type;
			switch (search_type)
			{
			case Gameplay::Collection::SearchType::LINEAR_SEARCH:
				time_complexity = "O(n)";
				current_operation_delay = collection_model->linear_search_delay;

				search_thread = std::thread(&StickCollectionController::processLinearSearch, this);
				break;
			case Gameplay::Collection::SearchType::BINARY_SEARCH:
				sortElements();
				time_complexity = "O(log n)";
				current_operation_delay = collection_model->binary_search_delay;
				search_thread = std::thread(&StickCollectionController::processBinarySearch, this);
				break;
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
		int StickCollectionController::getDelayMilliseconds()
		{
			return current_operation_delay;
		}

		sf::String StickCollectionController::getTimeComplexity()
		{
			return time_complexity;
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