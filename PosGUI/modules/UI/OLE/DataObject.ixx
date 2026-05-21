module;
#include <Windows.h>

#include <objidl.h>
#include <wil/resource.h>

export module PGUI.UI.OLE.DataObject;

import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.ComIterator;
import PGUI.ErrorHandling;
import PGUI.UI.OLE.OLEEnums;
import PGUI.UI.OLE.OLEStructs;
import PGUI.UI.OLE.EnumFormatData;

import std;

export namespace PGUI::UI::OLE
{
	template <typename T>
	concept DataObjectHandler = requires(
		const typename T::ValueType& value, 
		const FormatDataView& formatDataView,
		const FormatDataView& supportedFormatView,
		const StorageMedium& storageMedium)
	{
		{ T::CanStore(formatDataView) } -> std::same_as<bool>;
		{ T::SupportedFormats() } -> std::convertible_to<std::span<const FormatData>>;
		{ T::MatchesFormat(formatDataView, supportedFormatView) } -> std::same_as<bool>;
		{ T::ToMedium(value, formatDataView) } -> std::same_as<Result<StorageMedium>>;
		{ T::FromMedium(formatDataView, storageMedium) } -> std::same_as<Result<typename T::ValueType>>;
	};

	template <typename Derived>
	concept IsReadOnlyFromCOM = requires
	{
		requires Derived::ReadOnlyFromCOM == true;
	};

	template <DataObjectHandler Handler, bool AllowExtraFormat = false>
	class DataObject final : public Implements<DataObject<Handler, AllowExtraFormat>, IDataObject>
	{
		public:
		using StoredType = Handler::ValueType;
		static constexpr auto ExtraFormatsAllowed = AllowExtraFormat;
		using FormatStoragePair = std::pair<FormatData, StorageMedium>;
		using StorageVector = std::vector<FormatStoragePair>;

		auto AddData(const StoredType& data, const FormatData& formatData) noexcept -> Result<void>
		{
			const FormatDataView formatDataView{ formatData };

			if (!Handler::CanStore(formatDataView))
			{
				return Unexpected{ Error{ ErrorCode::InvalidFormat } };
			}

			for (const auto& storedFormat : formatStoragePairs | std::views::keys)
			{
				if (Handler::MatchesFormat(formatDataView, FormatDataView{ storedFormat }))
				{
					return Unexpected{ Error{
						ErrorCode::InvalidFormat
					}.SetCustomMessage(L"Duplicate format") };
				}
			}

			auto storageResult = Handler::ToMedium(data, formatDataView);
			if (!storageResult.has_value())
			{
				return Unexpected{ storageResult.error() };
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
				return Unexpected{ Error{
					SystemErrorCode::STLFailure
				}.SetCustomMessage(StringToWString(e.what())) };
			}

			return EmptyResult;
		}

		auto AddData(const StoredType& data) noexcept -> Result<void>
		{
			const auto initialSize = formatStoragePairs.size();
			for (const auto& formatData : Handler::SupportedFormats())
			{
				if (const auto result = AddData(data, formatData);
					!result.has_value() && result.error().Code() != ErrorCode::InvalidFormat)
				{
					formatStoragePairs.erase(
						formatStoragePairs.begin() + initialSize,
						formatStoragePairs.end()
					);
					return result;
				}
			}

			return EmptyResult;
		}

		auto ClearData() noexcept -> void
		{
			formatStoragePairs.clear();
		}

		[[nodiscard]] auto GetData(const FormatData& formatData) const noexcept -> Result<StoredType>
		{
			const FormatDataView formatDataView{ formatData };

			for (const auto& [storedFormat, storage] : formatStoragePairs)
			{
				if (const FormatDataView storedFormatView{ storedFormat };
					Handler::MatchesFormat(formatDataView, storedFormatView))
				{
					return Handler::FromMedium(storedFormatView, storage);
				}
			}

			return Unexpected{ Error{ ErrorCode::NotFound } };
		}

		template <DataObjectHandler H>
		[[nodiscard]] auto GetData(const FormatData& formatData) const noexcept -> Result<typename H::ValueType>
		{
			const FormatDataView formatDataView{ formatData };
			for (const auto& [storedFormat, storage] : formatStoragePairs)
			{
				if (const FormatDataView storedFormatView{ storedFormat };
					H::MatchesFormat(formatDataView, storedFormatView))
				{
					return H::FromMedium(storedFormatView, storage);
				}
			}
			return Unexpected{ Error{ ErrorCode::NotFound } };
		}
		

		#pragma region COM Methods

		// ReSharper disable CppParameterNeverUsed
		// ReSharper disable once IdentifierTypos
		
		auto DAdvise(
			FORMATETC* formatEtc, DWORD advf,
			IAdviseSink* pAdvSink, DWORD* pdwConnection) -> HRESULT override
		{
			Unused(formatEtc, advf, pAdvSink, pdwConnection);
			return OLE_E_ADVISENOTSUPPORTED;
		}
		auto DUnadvise(DWORD connection) -> HRESULT override
		{
			Unused(connection);
			return OLE_E_ADVISENOTSUPPORTED;
		}
		auto EnumDAdvise(IEnumSTATDATA** enumAdvise) -> HRESULT override
		{
			Unused(enumAdvise);
			return OLE_E_ADVISENOTSUPPORTED;
		}
		
		// ReSharper restore CppParameterNeverUsed
		// ReSharper disable CppParameterMayBeConstPtrOrRef

		auto QueryGetData(FORMATETC* formatEtc) -> HRESULT override
		{
			if (formatEtc == nullptr)
			{
				return E_POINTER;
			}

			const FormatDataView formatData{ *formatEtc };
			for (const auto& supportedFormat : Handler::SupportedFormats())
			{
				if (Handler::MatchesFormat(formatData, FormatDataView{ supportedFormat }))
				{
					return S_OK;
				}
			}

			if constexpr (ExtraFormatsAllowed)
			{
				for (const auto& storedFormat : formatStoragePairs | std::views::keys)
				{
					if (Handler::MatchesFormat(formatData, FormatDataView{ storedFormat }))
					{
						return S_OK;
					}
				}
			}

			return DV_E_FORMATETC;
		}

		auto GetData(FORMATETC* formatEtc, STGMEDIUM* medium) -> HRESULT override
		{
			if (formatEtc == nullptr || medium == nullptr)
			{
				return E_POINTER;
			}

			const FormatDataView formatData{ *formatEtc };
			auto& mediumRef = *medium;

			for (const auto& [storedFormat, storage] : formatStoragePairs)
			{
				if (!Handler::MatchesFormat(formatData, FormatDataView{ storedFormat }))
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
				if (!Handler::MatchesFormat(formatData, FormatDataView{ storedFormat }))
				{
					continue;
				}

				if (const auto result = storage.WriteToSTGMEDIUM(mediumRef);
					!result.has_value())
				{
					return result.error().HResult();
				}

				return S_OK;
			}

			return DV_E_FORMATETC;
		}

		auto SetData(FORMATETC* formatEtc, STGMEDIUM* medium, BOOL release) -> HRESULT override
		{
			if (formatEtc == nullptr || medium == nullptr)
			{
				return E_POINTER;
			}

			if constexpr (IsReadOnlyFromCOM<Handler>)
			{
				return E_NOTIMPL;
			}

			const FormatDataView formatData{ *formatEtc };
			if (!Handler::CanStore(formatData))
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
				return Handler::MatchesFormat(formatData, FormatDataView{ pair.first });
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
				*ppenumFormatEtc = new (std::nothrow) EnumFormatData{ Handler::SupportedFormats() };

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
				auto canonicalFormatResult = Handler::GetCanonicalFormatFor(input);
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

		// ReSharper restore CppParameterMayBeConstPtrOrRef
		// ReSharper restore IdentifierTypo

		#pragma endregion

		private:
		static constexpr auto HasCanonicalFormat = requires(const FormatDataView& formatDataView)
		{
			{ Handler::GetCanonicalFormatFor(formatDataView) } -> std::same_as<Result<FormatData>>;
		};
		StorageVector formatStoragePairs;
	};

	struct TextDataHandler final
	{
		using ValueType = std::wstring;

		[[nodiscard]] static auto CanStore(const FormatDataView& formatDataView) noexcept -> bool
		{
			return (
				formatDataView.format == ClipboardFormat::UnicodeText) &&
				formatDataView.aspect == DVAspect::Content &&
				IsFlagSet(formatDataView.storageMediumType, StorageMediumType::HGlobalMemory);
		}

		[[nodiscard]] static auto SupportedFormats() noexcept -> std::span<const FormatData>
		{
			static std::array supportedFormats{
				FormatData{
					ClipboardFormat::UnicodeText, DVAspect::Content,
					StorageMediumType::HGlobalMemory
				}
			};

			return supportedFormats;
		}

		[[nodiscard]] static auto MatchesFormat(
			const FormatDataView& formatDataView, 
			const FormatDataView& supportedFormatView) noexcept -> bool
		{
			return formatDataView.format == supportedFormatView.format &&
				formatDataView.aspect == supportedFormatView.aspect &&
				AreAllFlagsSet(formatDataView.storageMediumType, supportedFormatView.storageMediumType);
		}

		[[nodiscard]] static auto ToMedium(const std::wstring& text, const FormatDataView& formatDataView) noexcept -> Result<StorageMedium>
		{
			if (!CanStore(formatDataView))
			{
				return Unexpected{ Error{ ErrorCode::InvalidFormat } };
			}
			
			const auto bytesRequired = (text.size() + 1) * sizeof(wchar_t);
			wil::unique_hglobal hMem{ GlobalAlloc(GMEM_MOVEABLE, bytesRequired) };
			if (hMem.get() == nullptr)
			{
				return Unexpected{ Error{ E_OUTOFMEMORY } };
			}

			const wil::unique_hglobal_locked lockedMem{ hMem.get() };
			if (lockedMem.get() == nullptr)
			{
				return Unexpected{ Error{ E_POINTER } };
			}

			std::memcpy(lockedMem.get(), text.c_str(), bytesRequired);

			return StorageMedium{ 
				StorageMediumType::HGlobalMemory, 
				StorageHolder{ Handles::HGlobal{ hMem.release() } },
				nullptr,
				true
			};
		}

		[[nodiscard]] static auto FromMedium(const FormatDataView& formatDataView, const StorageMedium& storageMedium) noexcept -> Result<std::wstring>
		{
			if (!CanStore(formatDataView))
			{
				return Unexpected{ Error{ ErrorCode::InvalidFormat } };
			}
			if (const auto result = storageMedium.GetHolder().HasDataOfType(StorageMediumType::HGlobalMemory);
				!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			else if (!result.value())
			{
				return Unexpected{ Error{ ErrorCode::InvalidFormat } };
			}

			const auto [handle] = storageMedium.GetHolder().GetDataOfType<Handles::HGlobal>().value();
			if (handle == nullptr)
			{
				return Unexpected{ Error{ E_POINTER } };
			}
			const auto byteSize = GlobalSize(handle);
			if (byteSize < sizeof(wchar_t))
			{
				return Unexpected{ Error{ ErrorCode::InvalidArgument } };
			}

			const wil::unique_hglobal_locked lockedMem{ handle };
			if (lockedMem.get() == nullptr)
			{
				return Unexpected{ Error{ E_POINTER } };
			}
			const auto charCount = byteSize / sizeof(wchar_t);
			try
			{
				std::wstring str{ static_cast<const wchar_t*>(lockedMem.get()), charCount - 1 };
				return str;
			}
			catch (const std::exception& e)
			{
				return Unexpected{ 
					Error{
						E_OUTOFMEMORY 
					}.SetCustomMessage(StringToWString(e.what())) };
			}
		}
	};
	using TextDataObject = DataObject<TextDataHandler>;

	class DataObjectReadWrite final : public ComPtrHolder<IDataObject>
	{
		public:
		explicit DataObjectReadWrite(const ComPtr<IDataObject>& dataObject) noexcept 
			: ComPtrHolder{ dataObject }
		{ }
		explicit DataObjectReadWrite(ComPtr<IDataObject>&& dataObject) noexcept
			: ComPtrHolder{ std::move(dataObject) }
		{}

		auto SetData(const FormatData& formatData, const StorageMedium& storageMedium) const noexcept -> Result<void>
		{
			auto formatEtcResult = formatData.ToFORMATETC();
			if (!formatEtcResult.has_value())
			{
				return Unexpected{ formatEtcResult.error() };
			}
			FormatData::TargetDevicePtr targetDevice{ formatEtcResult.value().ptd };

			auto storageMediumResult = storageMedium.CopyToSTGMEDIUM();
			if (!storageMediumResult.has_value())
			{
				return Unexpected{ storageMediumResult.error() };
			}

			if (Error error{
					Get()->SetData(
						&formatEtcResult.value(),
						&storageMediumResult.value(),
						true)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto GetData(const FormatData& formatData) const noexcept -> Result<StorageMedium>
		{
			auto formatEtcResult = formatData.ToFORMATETC();
			if (!formatEtcResult.has_value())
			{
				return Unexpected{ formatEtcResult.error() };
			}
			FormatData::TargetDevicePtr targetDevice{ formatEtcResult.value().ptd };

			STGMEDIUM medium{ };
			
			if (const auto error = Error{ Get()->GetData(&formatEtcResult.value(), &medium) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return StorageMedium::MoveFrom(std::move(medium));
		}

		template <DataObjectHandler H>
		[[nodiscard]] auto GetData(const FormatData& formatData) const noexcept -> Result<typename H::ValueType>
		{
			auto formatEtcResult = formatData.ToFORMATETC();
			if (!formatEtcResult.has_value())
			{
				return Unexpected{ formatEtcResult.error() };
			}
			FormatData::TargetDevicePtr targetDevice{ formatEtcResult.value().ptd };
			
			STGMEDIUM medium{ };

			if (const auto error = Error{ Get()->GetData(&formatEtcResult.value(), &medium) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			auto storageMedium = StorageMedium::MoveFrom(std::move(medium));
			
			return H::FromMedium(FormatDataView{ formatData }, storageMedium);
		}

		[[nodiscard]] auto EnumerateFormats(const DataDirection direction = DataDirection::Get) const noexcept -> Result<EnumFORMATETCIterator>
		{
			ComPtr<IEnumFORMATETC> ptr;
			if (const auto error = Error{ Get()->EnumFormatEtc(ToUnderlying(direction), ptr.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			
			return EnumFORMATETCIterator{ ptr };
		}
	};
}
