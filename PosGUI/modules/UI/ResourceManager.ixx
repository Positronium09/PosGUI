export module PGUI.UI.ResourceManager;

import std;
import PGUI.ErrorHandling;
import PGUI.Utils;

export namespace PGUI::UI
{
	class ResourceManager final
	{
		public:
		static auto GetInstance() -> ResourceManager&;

		private:
		ResourceManager() noexcept = default;
		~ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		auto operator=(const ResourceManager&) -> ResourceManager& = delete;
		auto operator=(ResourceManager&&) -> ResourceManager& = delete;
	};
}
