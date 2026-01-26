module;
#include <ole2.h>

export module PGUI.UI.DragDrop.DataObject;

import PGUI.ComPtr;
import PGUI.UI.DragDrop.DragDropTypes;

export namespace PGUI::UI::DragDrop
{
	

	class DataObject : public Implements<DataObject, IDataObject>
	{
		public:
		DataObject() = default;
		~DataObject() override = default;

		private:

	};
}
