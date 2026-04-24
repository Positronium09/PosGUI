module;
#include <objidl.h>

export module PGUI.UI.DragDrop.DataObject;

import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.DragDrop.DragDropEnums;
import PGUI.UI.DragDrop.DragDropStructs;
import PGUI.UI.DragDrop.EnumFormatData;

import std;

export namespace PGUI::UI::DragDrop
{
	template <typename Derived, typename ObjBase>
	concept DataObjectDerived = requires(
		const Derived& obj, 
		const typename ObjBase::StoredType& t, 
		const FormatDataView& formatDataView,
		const FormatDataView& supportedFormatView,
		const StorageMedium& storageMedium)
	{
		requires std::derived_from<Derived, ObjBase>;
		{ Derived::CanStore(formatDataView) } -> std::same_as<bool>;
		{ Derived::SupportedFormats() } -> std::convertible_to<std::span<const FormatData>>;
		{ Derived::MatchesFormat(formatDataView, supportedFormatView) } -> std::same_as<bool>;
		{ obj.ToMedium(t, formatDataView) } -> std::same_as<Result<StorageMedium>>;
		{ obj.FromMedium(formatDataView, storageMedium) } -> std::same_as<Result<typename ObjBase::StoredType>>;
	};

	template <typename Derived>
	concept IsReadOnlyFromCOM = requires
	{
		requires Derived::ReadOnlyFromCOM == true;
	};

	template <typename Derived, typename T, bool AllowExtraFormat = false>
	class DataObjectBase : public Implements<DataObjectBase<Derived, T, AllowExtraFormat>, IDataObject>
	{
		static_assert(DataObjectDerived<Derived, DataObjectBase>);
		public:
		using StoredType = T;
		static constexpr auto ExtraFormatsAllowed = AllowExtraFormat;
		using FormatStoragePair = std::pair<FormatData, StorageMedium>;
		using StorageVector = std::vector<FormatStoragePair>;

		auto AddData(const T& data, const FormatData& formatData) noexcept -> Error
		{
			const FormatDataView formatDataView{ formatData };

			if (!Derived::CanStore(formatDataView))
			{
				return Error{ ErrorCode::InvalidFormat };
			}

			for (const auto& storedFormat : formatStoragePairs | std::views::keys)
			{
				if (Derived::MatchesFormat(formatDataView, FormatDataView{ storedFormat }))
				{
					return Error{
						ErrorCode::InvalidFormat
					}.SetCustomMessage(L"Duplicate format");
				}
			}

			auto storageResult = static_cast<const Derived*>(this)->ToMedium(data, formatDataView);
			if (!storageResult.has_value())
			{
				return storageResult.error();
			}
			try
			{
				auto copyResult = formatData.Copy();
				if (!copyResult.has_value())
				{
					throw Exception{ copyResult.error() };
				}
				formatStoragePairs.emplace_back(std::move(copyResult).value(), std::move(storageResult).value());
			}
			catch (const std::exception& e)
			{
				return Error{
					SystemErrorCode::STLFailure
				}.SetCustomMessage(StringToWString(e.what()));
			}

			return Error{ ErrorCode::Success };
		}

		auto AddData(const T& data) noexcept -> Error
		{
			const auto initialSize = formatStoragePairs.size();
			for (const auto& formatData : Derived::SupportedFormats())
			{
				if (const auto result = AddData(data, formatData);
					result.IsFailure() && result.Code() != ErrorCode::InvalidFormat)
				{
					formatStoragePairs.erase(
						formatStoragePairs.begin() + initialSize,
						formatStoragePairs.end()
					);
					return result;
				}
			}

			return Error{ ErrorCode::Success };
		}

		auto ClearData() noexcept -> void
		{
			formatStoragePairs.clear();
		}

		[[nodiscard]] auto GetData(const FormatData& formatData) const noexcept -> Result<T>
		{
			const FormatDataView formatDataView{ formatData };

			for (const auto& [storedFormat, storage] : formatStoragePairs)
			{
				if (const FormatDataView storedFormatView{ storedFormat };
					Derived::MatchesFormat(formatDataView, storedFormatView))
				{
					return static_cast<const Derived*>(this)->FromMedium(storedFormatView, storage);
				}
			}

			return Error{ ErrorCode::NotFound };
		}

		#pragma region COM Methods

		// ReSharper disable CppParameterNeverUsed
		// ReSharper disable once IdentifierTypos
		
		auto DAdvise(
			FORMATETC* formatEtc, DWORD advf,
			IAdviseSink* pAdvSink, DWORD* pdwConnection) -> HRESULT override
		{
			return OLE_E_ADVISENOTSUPPORTED;
		}
		auto DUnadvise(DWORD connection) -> HRESULT override
		{
			return OLE_E_ADVISENOTSUPPORTED;
		}
		auto EnumDAdvise(IEnumSTATDATA** enumAdvise) -> HRESULT override
		{
			return OLE_E_ADVISENOTSUPPORTED;
		}
		
		// ReSharper restore CppParameterNeverUsed

		auto QueryGetData(const FORMATETC* formatEtc) -> HRESULT override
		{
			if (formatEtc == nullptr)
			{
				return E_POINTER;
			}

			const FormatDataView formatData{ *formatEtc };
			for (const auto& supportedFormat : Derived::SupportedFormats())
			{
				if (Derived::MatchesFormat(formatData, FormatDataView{ supportedFormat }))
				{
					return S_OK;
				}
			}

			if constexpr (ExtraFormatsAllowed)
			{
				for (const auto& storedFormat : formatStoragePairs | std::views::keys)
				{
					if (Derived::MatchesFormat(formatData, FormatDataView{ storedFormat }))
					{
						return S_OK;
					}
				}
			}

			return DV_E_FORMATETC;
		}

		auto GetData(const FORMATETC* formatEtc, STGMEDIUM* medium) -> HRESULT override
		{
			if (formatEtc == nullptr || medium == nullptr)
			{
				return E_POINTER;
			}

			const FormatDataView formatData{ *formatEtc };
			auto& mediumRef = *medium;

			for (const auto& [storedFormat, storage] : formatStoragePairs)
			{
				if (!Derived::MatchesFormat(formatData, FormatDataView{ storedFormat }))
				{
					continue;
				}

				const auto dupResult = storage.CopyToSTGMEDIUM();
				if (!dupResult.has_value())
				{
					return dupResult.error().HResult();
				}

				mediumRef = std::move(dupResult).value();

				return S_OK;
			}

			return DV_E_FORMATETC;
		}

		auto GetDataHere(FORMATETC* formatEtc, STGMEDIUM* medium) -> HRESULT override
		{
			if (formatEtc == nullptr || medium == nullptr)
			{
				return E_POINTER;
			}

			const FormatDataView formatData{ *formatEtc };
			const auto& mediumRef = *medium;

			for (const auto& [storedFormat, storage] : formatStoragePairs)
			{
				if (!Derived::MatchesFormat(formatData, FormatDataView{ storedFormat }))
				{
					continue;
				}

				if (const auto error = storage.WriteToSTGMEDIUM(mediumRef);
					error.IsFailure())
				{
					return error.HResult();
				}

				return S_OK;
			}

			return DV_E_FORMATETC;
		}

		auto SetData(const FORMATETC* formatEtc, STGMEDIUM* medium, BOOL release) -> HRESULT override
		{
			if (formatEtc == nullptr || medium == nullptr)
			{
				return E_POINTER;
			}

			if constexpr (IsReadOnlyFromCOM<Derived>)
			{
				return E_NOTIMPL;
			}

			const FormatDataView formatData{ *formatEtc };
			if (!Derived::CanStore(formatData))
			{
				if constexpr (!ExtraFormatsAllowed)
				{
					return DV_E_FORMATETC;
				}
			}

			auto mediumResult = release ? 
				Result<StorageMedium>{ StorageMedium::MoveFrom(std::move(*medium)) } :
				StorageMedium::CopyFrom(*medium);

			if (!mediumResult.has_value())
			{
				return mediumResult.error().HResult();
			}

			const auto it = std::ranges::find_if(formatStoragePairs, [&](const auto& pair)
			{
				return Derived::MatchesFormat(formatData, FormatDataView{ pair.first });
			});
			if (it != formatStoragePairs.end())
			{
				it->second = std::move(mediumResult).value();
				return S_OK;
			}

			try
			{
				auto targetCopy = FormatData::CopyTargetDevice(formatData.targetDevice);
				if (!targetCopy.has_value())
				{
					return targetCopy.error().HResult();
				}
				
				formatStoragePairs.emplace_back(FormatData{
					formatData.format,
					formatData.aspect,
					formatData.storageMediumType,
					std::move(targetCopy.value()),
					formatData.index
				}, std::move(mediumResult).value());
			}
			catch (const std::exception& e)
			{
				return Error{
					SystemErrorCode::STLFailure
				}.SetCustomMessage(StringToWString(e.what())).HResult();
			}

			return S_OK;
		}

		 //ReSharper disable once IdentifierTypo

		auto EnumFormatEtc(const DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) -> HRESULT override
		{
			if (ppenumFormatEtc == nullptr)
			{
				return E_POINTER;
			}
			if (const auto direction = FromUnderlying<DataDirection>(dwDirection);
				direction != DataDirection::Get)
			{
				return E_INVALIDARG;
			}

			try
			{
				*ppenumFormatEtc = new (std::nothrow) EnumFormatData{ Derived::SupportedFormats() };

				if (*ppenumFormatEtc == nullptr)
				{
					return E_OUTOFMEMORY;
				}

				return S_OK;
			}
			catch (const Exception& e)
			{
				*ppenumFormatEtc = nullptr;
				return e.GetError().HResult();
			}
			catch (const std::exception& e)
			{
				*ppenumFormatEtc = nullptr;
				return Error{
					SystemErrorCode::STLFailure
				}.SetCustomMessage(StringToWString(e.what())).HResult();
			}
			
		}

		// ReSharper disable IdentifierTypo

		auto GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut) -> HRESULT override
		{
			if (pformatectIn == nullptr || pformatetcOut == nullptr)
			{
				return E_POINTER;
			}

			const FormatDataView input{ *pformatectIn };

			if constexpr (HasCanonicalFormat)
			{
				auto canonicalFormatResult = Derived::GetCanonicalFormatFor(input);
				if (!canonicalFormatResult.has_value())
				{
					return canonicalFormatResult.error().HResult();
				}

				auto toFormatResult = std::move(canonicalFormatResult).value().ToFORMATETC();
				if (!toFormatResult.has_value())
				{
					return toFormatResult.error().HResult();
				}
				*pformatetcOut = std::move(toFormatResult).value();

				if (input == *pformatetcOut)
				{
					return DATA_S_SAMEFORMATETC;
				}

				return S_OK;
			}
			else
			{
				pformatetcOut->tymed = ToUnderlying(input.storageMediumType);
				pformatetcOut->dwAspect = ToUnderlying(input.aspect);
				pformatetcOut->cfFormat = ToUnderlying(input.format);
				pformatetcOut->lindex = input.index;
				pformatetcOut->ptd = nullptr;

				return DATA_S_SAMEFORMATETC;
			}
		}

		// ReSharper restore IdentifierTypo

		#pragma endregion

		private:
		static constexpr auto HasCanonicalFormat = requires(const FormatDataView& formatDataView)
		{
			{ Derived::GetCanonicalFormatFor(formatDataView) } -> std::same_as<Result<FormatData>>;
		};
		StorageVector formatStoragePairs;
	};


}
