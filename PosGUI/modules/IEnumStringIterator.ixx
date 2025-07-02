module;
#include <wrl.h>
#include <objbase.h>

export module PGUI.IEnumStringIterator;

import std;

import PGUI.ComPtr;

export namespace PGUI
{
	class IEnumStringIterator : ComPtrHolder<IEnumString>
	{
		public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = std::wstring;
		using difference_type = std::ptrdiff_t;
		using pointer = std::wstring*;
		using reference = std::wstring&;

		explicit IEnumStringIterator(ComPtr<IEnumString> enumString, bool isEnd = false) noexcept;

		auto operator*() const -> std::wstring_view;
		auto operator->() const -> std::wstring_view;
		auto operator++() -> IEnumStringIterator&;
		auto operator++(int) -> IEnumStringIterator;

		auto operator==(const IEnumStringIterator& other) const -> bool;

		private:
		bool end = false;
		std::wstring current;

		void MoveNext();
	};
}
