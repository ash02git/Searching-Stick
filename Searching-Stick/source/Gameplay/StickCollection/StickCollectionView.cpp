#include "../../include/Gameplay/StickCollection/StickCollectionView.h"
#include "../../include/Gameplay/StickCollection/StickCollectionController.h"//maybe not needed

namespace Gameplay
{
	namespace Collection
	{
		StickCollectionView::StickCollectionView()
		{
		}
		StickCollectionView::~StickCollectionView()
		{
		}
		void StickCollectionView::initialize(StickCollectionController* collection_controller)
		{
			this->collection_controller = collection_controller;
		}
		void StickCollectionView::update()
		{
		}
		void StickCollectionView::render()
		{
		}
	}
}