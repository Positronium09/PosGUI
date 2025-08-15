export module PGUI.Mutex:NullMutex;

export namespace PGUI::Mutex
{
	class NullMutex
	{
		public:
		// ReSharper disable CppMemberFunctionMayBeStatic
		auto lock() const noexcept -> void { }
		auto unlock() const noexcept -> void { }
		// ReSharper disable once CppInconsistentNaming
		auto try_lock() const noexcept -> bool { return true; }
		// ReSharper restore CppMemberFunctionMayBeStatic
	};
}
