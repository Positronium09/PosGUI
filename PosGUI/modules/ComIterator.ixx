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
}

export namespace PGUI
{
	template <typename T, typename IteratorType, typename ConvertedType = IteratorType>
		requires ComIteratorType<T, IteratorType, ConvertedType>
	class ComIterator : ComPtrHolder<T>
	{
		public:
		using iterator_category = std::input_iterator_tag;
		using value_type = ConvertedType;
		using difference_type = std::ptrdiff_t;
		using pointer = std::add_pointer_t<value_type>;
		using reference = std::add_lvalue_reference_t<value_type>;

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

		private:
		bool end = false;
		value_type current;

		auto MoveNext()
		{
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

			if constexpr (std::is_pointer_v<IteratorType> && std::same_as<IteratorType, ConvertedType>)
			{
				CoTaskMemFree(current);
			}
			
			current = static_cast<ConvertedType>(item);
			
			if constexpr (std::is_pointer_v<IteratorType> && !std::same_as<IteratorType, ConvertedType>)
			{
				CoTaskMemFree(item);
			}
		}
	};
}
