export module PGUI.Wrapper;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Utils;

namespace PGUI::Detail
{
	template <bool, typename Handle>
	struct HolderSelector
	{
		using Type = ProjectionHolder<Handle>;
	};

	template <typename Handle>
	struct HolderSelector<true, Handle>
	{
		using Type = ComPtrHolder<Handle>;
	};
}

export namespace PGUI
{
	template <typename T>
	concept WrapperHandle = ComInterface<T> || WinrtProjection<T>;

	template <typename Handle, typename Base>
	concept HandleDerivedFrom =
		WrapperHandle<Handle> && WrapperHandle<Base> &&
		(std::derived_from<Handle, Base> || DerivedFromProjected<Handle, Base>);

	template <WrapperHandle Handle>
	using HolderFor = Detail::HolderSelector<ComInterface<Handle>, Handle>::Type;

	template <WrapperHandle Handle>
	class Wrapper;

	template <typename T>
	concept WrapperType =
		requires { typename T::HandleType; } &&
		WrapperHandle<typename T::HandleType> &&
		std::derived_from<T, Wrapper<typename T::HandleType>>;

	template <WrapperHandle Handle>
	class Wrapper : public HolderFor<Handle>
	{
		public:
		using HandleType = Handle;
		using Holder = HolderFor<Handle>;

		using Holder::Holder;

		template <typename Other> requires
			HandleDerivedFrom<Other, Handle> && NotSameAs<Other, Handle>
		explicit(false) Wrapper(const Wrapper<Other>& other) noexcept :
			Holder{ other.Get() }
		{ }

		template <WrapperType Target> requires
			HandleDerivedFrom<typename Target::HandleType, Handle>
		[[nodiscard]] auto As() const noexcept -> Result<Target>
		{
			if constexpr (ComInterface<Handle>)
			{
				const auto handle = this->template GetAs<typename Target::HandleType>();
				if (!handle)
				{
					return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
				}

				return Target{ handle };
			}
			else
			{
				const auto handle = this->Get().template try_as<typename Target::HandleType>();
				if (!handle)
				{
					return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
				}

				return Target{ handle };
			}
		}
	};
}
