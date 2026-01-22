module;
#include <wrl.h>

module PGUI.IEnumStringIterator;

import std;

import PGUI.ComPtr;

namespace PGUI
{
	IEnumStringIterator::IEnumStringIterator(const ComPtr<IEnumString>& enumString, const bool isEnd) noexcept :
		ComPtrHolder{ enumString }, end{ isEnd }
	{
		MoveNext();
	}

	auto IEnumStringIterator::operator*() const -> std::wstring_view
	{
		return current;
	}

	auto IEnumStringIterator::operator->() -> pointer
	{
		return &current;
	}

	auto IEnumStringIterator::operator++(int) -> void
	{
		MoveNext();
	}

	auto IEnumStringIterator::operator++() -> IEnumStringIterator&
	{
		auto temp = *this;
		MoveNext();
		return temp;
	}

	auto IEnumStringIterator::operator==(const IEnumStringIterator& other) const -> bool
	{
		if (end && other.end)
		{
			return true;
		}
		return GetRaw() == other.GetRaw() && end == other.end;
	}

	auto IEnumStringIterator::MoveNext() -> void
	{
		if (end)
		{
			return;
		}

		ULONG fetched;
		LPOLESTR str;

		if (const auto hr = Get()->Next(1, &str, &fetched);
			hr != S_OK || fetched != 1)
		{
			end = true;
			return;
		}

		current = str;
		CoTaskMemFree(str);
	}
}
