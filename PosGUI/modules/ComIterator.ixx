module;
#include <Objbase.h>

export module PGUI.ComIterator;

import std;

import PGUI.ComPtr;

namespace PGUI
{
	template <typename T, typename IteratorType, typename ConvertedType = IteratorType>
	concept ComIteratorType = requires(T* val)
	{
		{ val->Clone(static_cast<T**>(nullptr)) } -> std::same_as<HRESULT>;
		
		{ val->Next(
			static_cast<ULONG>(0), 
			static_cast<IteratorType*>(nullptr), 
			static_cast<ULONG*>(nullptr)) 
		} -> std::same_as<HRESULT>;

		{ val->Skip(static_cast<ULONG>(0)) } -> std::same_as<HRESULT>;
		
		{ val->Reset() } -> std::same_as<HRESULT>;

		requires std::convertible_to<IteratorType, ConvertedType> || std::same_as<IteratorType, ConvertedType>;
	};

	template <typename T, typename IteratorType>
	concept IteratorDerivedType = requires(T* t, IteratorType& val)
	{
		{ t->DeleteCurrent() } noexcept -> std::same_as<void>;

		{ t->ConvertItem(val) } noexcept -> std::same_as<void>;

		{ t->DeleteItem(val) } noexcept -> std::same_as<void>;
	};
}

export namespace PGUI
{
	template <typename Derived, ComInterface T, typename IteratorType, typename ValueType>
	class ComIterator : ComPtrHolder<T>
	{
		public:
		using iterator_category = std::input_iterator_tag;
		using value_type = ValueType;
		using difference_type = std::ptrdiff_t;
		using pointer = std::add_pointer_t<ValueType>;
		using reference = std::add_lvalue_reference_t<ValueType>;

		explicit ComIterator(const ComPtr<T>& iter, const bool isEnd = false) noexcept : 
			ComPtrHolder<T>{ iter }, end{ isEnd }
		{
			this->MoveNext();
		}

		const auto& operator*() const
		{
			return current;
		}
		auto operator->() -> pointer
		{
			return &current;
		}
		auto operator++() -> ComIterator&
		{
			this->MoveNext();
			return *this;
		}
		auto operator++(int) -> void
		{
			this->MoveNext();
		}

		auto operator==(const ComIterator& other) const -> bool
		{
			if (this->end && other.end)
			{
				return true;
			}
			return this->GetRaw() == other.GetRaw() && this->end == other.end;
		}

		protected:
		[[nodiscard]] auto& GetCurrentValue() noexcept
		{
			return current;
		}
		[[nodiscard]] const auto& GetCurrentValue() const noexcept
		{
			return current;
		}

		private:
		bool end = false;
		value_type current;

		auto MoveNext()
		{
			static_assert(IteratorDerivedType<Derived, IteratorType>, 
				"Derived must implement:\n"
				"void DeleteCurrent() noexcept\n"
				"void ConvertItem(<const> IteratorType&) noexcept\n"
				"void DeleteItem(<const> IteratorType&) noexcept");

			if (this->end)
			{
				return;
			}

			ULONG fetched;
			IteratorType item;

			if (const auto hr = this->GetRaw()->Next(1, &item, &fetched);
				hr || fetched == 0)
			{
				this->end = true;
				return;
			}

			static_cast<Derived*>(this)->DeleteCurrent();

			static_cast<Derived*>(this)->ConvertItem(item);

			static_cast<Derived*>(this)->DeleteItem(item);
		}
	};

	struct EnumStringIterator : 
		ComIterator<EnumStringIterator, IEnumString, LPOLESTR, std::wstring>
	{
		using ComIterator::ComIterator;

		static auto DeleteCurrent() noexcept -> void
		{
			
		}

		auto ConvertItem(const LPOLESTR& str) noexcept -> void
		{
			GetCurrentValue() = std::wstring{ str };
		}

		static auto DeleteItem(const LPOLESTR& str) noexcept -> void
		{
			CoTaskMemFree(str);
		}
	};

	struct EnumVariantIterator :
		ComIterator<EnumVariantIterator, IEnumVARIANT, VARIANT, VARIANT>
	{
		using ComIterator::ComIterator;

		auto DeleteCurrent() noexcept -> void
		{
			[[maybe_unused]] auto hr = VariantClear(std::addressof(GetCurrentValue()));
		}
		
		auto ConvertItem(const VARIANT& var) noexcept -> void
		{
			[[maybe_unused]] auto hr = VariantCopy(std::addressof(GetCurrentValue()), &var);
		}

		static auto DeleteItem(VARIANT& var) noexcept -> void
		{
			[[maybe_unused]] auto hr = VariantClear(&var);
		}
	};
	static_assert(IteratorDerivedType<EnumVariantIterator, VARIANT>);

	template <ComInterface Interface, ComInterface IInterfaceEnum>
	struct EnumInterfaceIterator :
		ComIterator<EnumInterfaceIterator<Interface, IInterfaceEnum>, IInterfaceEnum, Interface*, ComPtr<Interface>>
	{
		using ComIterator<EnumInterfaceIterator, IInterfaceEnum, Interface*, ComPtr<Interface>>::ComIterator;

		auto DeleteCurrent() noexcept -> void
		{
			this->GetCurrentValue().reset();
		}
		
		auto ConvertItem(Interface* ptr) noexcept -> void
		{
			this->GetCurrentValue().attach(ptr);
		}

		static auto DeleteItem(Interface*) noexcept -> void
		{
		}
	};

	using EnumIUnknownIterator = EnumInterfaceIterator<IUnknown, IEnumUnknown>;
	static_assert(IteratorDerivedType<EnumIUnknownIterator, IUnknown*>);
}
