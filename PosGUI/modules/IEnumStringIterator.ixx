// ReSharper disable CppInconsistentNaming
module;
#include <objidl.h>

export module PGUI.IEnumStringIterator;

import std;

import PGUI.ComPtr;

export namespace PGUI
{
	class IEnumStringIterator : ComPtrHolder<IEnumString>
	{
		public:
		using iterator_category = std::input_iterator_tag;
		using value_type = std::wstring;
		using difference_type = std::ptrdiff_t;
		using pointer = std::add_pointer_t<value_type>;
		using reference = std::add_lvalue_reference_t<value_type>;

		explicit IEnumStringIterator(const ComPtr<IEnumString>& enumString, bool isEnd = false) noexcept;

		auto operator*() const -> std::wstring_view;
		auto operator->() -> pointer;
		auto operator++() -> IEnumStringIterator&;
		auto operator++(int) -> void;

		auto operator==(const IEnumStringIterator& other) const -> bool;

		private:
		bool end = false;
		std::wstring current;

		auto MoveNext() -> void;
	};
}
