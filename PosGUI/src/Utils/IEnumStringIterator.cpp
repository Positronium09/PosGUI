module;
#include <wrl.h>
#include <objbase.h>

module PGUI.IEnumStringIterator;

import std;

import PGUI.ComPtr;

namespace PGUI
{
	IEnumStringIterator::IEnumStringIterator(ComPtr<IEnumString> enumString, bool isEnd) noexcept
		: ComPtrHolder{ enumString }, end{ isEnd }
	{
		MoveNext();
	}

	auto IEnumStringIterator::operator*() const -> std::wstring_view
	{
		return current;
	}
	auto IEnumStringIterator::operator->() const -> std::wstring_view
	{
		return current;
	}
	auto IEnumStringIterator::operator++() -> IEnumStringIterator&
	{
		MoveNext();
		return *this;
	}
	
	auto IEnumStringIterator::operator++(int) -> IEnumStringIterator
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

	void IEnumStringIterator::MoveNext()
	{
		if (end)
		{
			return;
		}

		ULONG fetched;
		LPOLESTR str;
		
		auto hr = Get()->Next(1, &str, &fetched);
		if (hr != S_OK || fetched != 1)
		{
			end = true;
			return;
		}

		current = str;
		CoTaskMemFree(str);
	}
}
