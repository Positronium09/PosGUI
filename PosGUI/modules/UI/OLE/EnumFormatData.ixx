module;
#include <objidl.h>

export module PGUI.UI.OLE.EnumFormatData;

import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.OLE.OLEStructs;

import std;

export namespace PGUI::UI::OLE
{
	class EnumFormatData final : public Implements<EnumFormatData, IEnumFORMATETC>
	{
		public:
		[[nodiscard]] static auto Create(
			const std::span<const FormatData> enumFormats) noexcept -> Result<ComPtr<EnumFormatData>>
		{
			ComPtr<EnumFormatData> enumFormatData;
			enumFormatData.attach(new(std::nothrow) EnumFormatData{ });
			if (!enumFormatData)
			{
				return Unexpected{ Error{ ErrorCode::AllocationFailure } };
			}

			if (const auto result = WrapThrowing([&enumFormatData, enumFormats]
				{
					enumFormatData->formats.reserve(enumFormats.size());
					for (const auto& format : enumFormats)
					{
						auto copyResult = format.Copy();
						if (!copyResult.has_value())
						{
							throw Exception{ copyResult.error() };
						}
						enumFormatData->formats.emplace_back(MoveChecked(copyResult).value());
					}
				});
				!result.has_value())
			{
				return Unexpected{ result.error() };
			}

			return enumFormatData;
		}

		EnumFormatData(const EnumFormatData&) = delete;
		auto operator=(const EnumFormatData&) -> EnumFormatData& = delete;
		EnumFormatData(EnumFormatData&&) noexcept = delete;
		auto operator=(EnumFormatData&&) noexcept -> EnumFormatData& = delete;

		// ReSharper disable IdentifierTypo

		auto Clone(IEnumFORMATETC** ppenum) -> HRESULT override
		{
			if (!ppenum)
			{
				return E_POINTER;
			}

			auto cloneResult = Create(formats);
			if (!cloneResult.has_value())
			{
				return cloneResult.error().HResult();
			}

			auto& clone = cloneResult.value();
			clone->currentIndex = currentIndex;

			*ppenum = clone.detach();

			return S_OK;
		}

		auto Next(const ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched) -> HRESULT override
		{
			if (celt == 0 || rgelt == nullptr)
			{
				return E_INVALIDARG;
			}
			if (celt > 1 && pceltFetched == nullptr)
			{
				return E_INVALIDARG;
			}

			if (currentIndex >= formats.size())
			{
				if (pceltFetched)
				{
					*pceltFetched = 0;
				}
				return S_FALSE;
			}

			if (celt == 1)
			{
				const auto conversionResult = formats[currentIndex].ToFORMATETC();
				if (!conversionResult.has_value())
				{
					return conversionResult.error().HResult();
				}
				rgelt[0] = conversionResult.value();
				currentIndex++;
				
				if (pceltFetched)
				{
					*pceltFetched = 1;
				}

				return S_OK;
			}

			*pceltFetched = 0;
			for (auto index = 0UL; index < celt; index++)
			{
				if (currentIndex >= formats.size())
				{
					break;
				}
				const auto conversionResult = formats[currentIndex].ToFORMATETC();
				if (!conversionResult.has_value())
				{
					for (auto i = 0UL; i < index; i++)
					{
						if (rgelt[i].ptd != nullptr)
						{
							CoTaskMemFree(rgelt[i].ptd);
							rgelt[i].ptd = nullptr;
						}
					}
					*pceltFetched = 0;
					return conversionResult.error().HResult();
				}
				rgelt[index] = conversionResult.value();
				(*pceltFetched)++;
				currentIndex++;
			}

			if (celt == *pceltFetched)
			{
				return S_OK;
			}

			return S_FALSE;
		}

		auto Reset() -> HRESULT override
		{
			currentIndex = 0;
			return S_OK;
		}

		auto Skip(const ULONG celt) -> HRESULT override
		{
			if (celt > formats.size() - currentIndex)
			{
				currentIndex = formats.size();
				return S_FALSE;
			}
			
			currentIndex += celt;

			return S_OK;
		}

		// ReSharper restore IdentifierTypo

		private:
		EnumFormatData() = default;

		std::vector<FormatData> formats;
		std::vector<FormatData>::size_type currentIndex{ 0 };
	};
}
