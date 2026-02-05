module PGUI.UI.ResourceManager;

import std;
import PGUI.ErrorHandling;
import PGUI.Utils;

namespace PGUI::UI
{
	auto ResourceManager::GetInstance() -> ResourceManager&
	{
		static ResourceManager instance;
		return instance;
	}
}
