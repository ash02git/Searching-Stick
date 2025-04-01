#pragma once
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>

namespace Gameplay
{
	namespace Collection
	{
		class StickCollectionView;
		class StickCollectionModel;
		struct Stick;
		enum class SearchType;
		
		class StickCollectionController
		{
		private:
			StickCollectionView* collection_view;
			StickCollectionModel* collection_model;
			std::vector<Stick*> sticks;
			Collection::SearchType search_type;

			Stick* stick_to_search;

			std::thread search_thread;

			int number_of_comparisons;
			int number_of_array_access;

			int current_operation_delay;

			sf::String time_complexity;

			void initializeSticks();
			float calculateStickWidth();

			void shuffleSticks();

			void updateSticksPosition();

			void resetSticksColor();
			void resetSearchStick();
			void resetVariables();
			
			void initializeSticksArray();

			float calculateStickHeight(int array_pos);

			void destroy();

			void processLinearSearch();

			void joinThreads();
			void processSearchThreadState();

		public:
			StickCollectionController();
			~StickCollectionController();

			void initialize();
			void update();
			void render();

			void reset();

			SearchType getSearchType();

			int getNumberOfSticks();

			void searchElement(SearchType search_type);

			int getNumberOfComparisons();
			int getNumberOfArrayAccess();

			int getDelayMilliseconds();
			sf::String getTimeComplexity();
		};
	}
}