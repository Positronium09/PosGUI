export module PGUI.Mutex:NullMutex;

export namespace PGUI::Mutex
{
	class NullMutex
	{
		public:
		// ReSharper disable CppMemberFunctionMayBeStatic
		auto lock() const noexcept -> void { }
		auto unlock() const noexcept -> void { }
		// ReSharper disable CppInconsistentNaming

		auto try_lock() const noexcept -> bool { return true; }

		auto lock_shared() noexcept -> void
		{
			
		}
		auto unlock_shared() noexcept -> void
		{
		}
		auto try_lock_shared() noexcept -> bool
		{
			return true;
		}

		// ReSharper restore CppInconsistentNaming
		// ReSharper restore CppMemberFunctionMayBeStatic
	};
}
