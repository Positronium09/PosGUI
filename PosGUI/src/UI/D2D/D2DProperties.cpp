module;
#include <d2d1_1.h>

module PGUI.UI.D2D.D2DProperties;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::UI::D2D
{
	D2DProperties::D2DProperties(const ComPtr<ID2D1Properties>& properties) noexcept :
		ComPtrHolder{ properties }
	{ }

	auto D2DProperties::GetPropertyName(const UINT32 index) const noexcept -> Result<std::wstring>
	{
		const auto& ptr = Get();
		const auto nameLength = ptr->GetPropertyNameLength(index);

		std::wstring name(nameLength, L'\0');
		if (const auto hr = ptr->GetPropertyName(index, name.data(), nameLength + 1);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get property name");
			return Unexpected{ error };
		}

		return name;
	}

	auto D2DProperties::GetSubProperty(const UINT32 index) const noexcept -> Result<D2DProperties>
	{
		ComPtr<ID2D1Properties> subProperty;
		if (const auto hr = Get()->GetSubProperties(index, subProperty.put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get sub-properties");
			return Unexpected{ error };
		}

		return D2DProperties{ subProperty };
	}
}
