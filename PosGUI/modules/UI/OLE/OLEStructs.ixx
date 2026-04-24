module;
#include <Windows.h>

#include <objidl.h>
#include <wil/resource.h>

export module PGUI.UI.OLE.OLEStructs;

import PGUI.UI.OLE.OLEEnums;
import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.ComPtr;

import std;

export namespace PGUI::UI::OLE
{
	struct FormatData
	{
		using TargetDevicePtr = std::unique_ptr<DVTARGETDEVICE, CoTaskDeleter>;

		ClipboardFormat format{ };
		TargetDevicePtr targetDevice{ nullptr };
		DVAspect aspect{ };
		long index = -1;
		StorageMediumType storageMediumType{ };

		constexpr FormatData(
			const ClipboardFormat format,
			const DVAspect aspect,
			const StorageMediumType storageMediumType,
			TargetDevicePtr&& targetDevice = nullptr,
			const long index = -1) noexcept :
			format{ format },
			targetDevice{ std::move(targetDevice) },
			aspect{ aspect },
			index{ index },
			storageMediumType{ storageMediumType }
		{}

		FormatData(FormatData&&) noexcept = default;
		auto operator=(FormatData&&) noexcept -> FormatData & = default;
		FormatData(const FormatData&) = delete;
		auto operator=(const FormatData&) -> FormatData & = delete;

		// ReSharper disable IdentifierTypo
		// ReSharper disable CppInconsistentNaming

		[[nodiscard]] static auto FromFORMATETC(const FORMATETC& fmt) noexcept -> Result<FormatData>
		{
			FormatData formatData;
			formatData.format = FromUnderlying<ClipboardFormat>(fmt.cfFormat);
			formatData.aspect = FromUnderlying<DVAspect>(fmt.dwAspect);
			formatData.index = fmt.lindex;
			formatData.storageMediumType = FromUnderlying<StorageMediumType>(fmt.tymed);

			if (fmt.ptd == nullptr)
			{
				return formatData;
			}

			auto newDeviceResult = CopyTargetDevice(fmt.ptd);
			if (!newDeviceResult.has_value())
			{
				return Unexpected{ newDeviceResult.error() };
			}

			formatData.targetDevice = std::move(newDeviceResult).value();
			return formatData;
		}

		[[nodiscard]] static auto FromFORMATETC(FORMATETC&& fmt) noexcept -> Result<FormatData>
		{
			FormatData formatData;
			formatData.format = FromUnderlying<ClipboardFormat>(fmt.cfFormat);
			formatData.aspect = FromUnderlying<DVAspect>(fmt.dwAspect);
			formatData.index = fmt.lindex;
			formatData.storageMediumType = FromUnderlying<StorageMediumType>(fmt.tymed);
			formatData.targetDevice.reset(std::exchange(fmt.ptd, nullptr));

			return formatData;
		}

		[[nodiscard]] static auto CopyTargetDevice(const DVTARGETDEVICE* td) noexcept -> Result<TargetDevicePtr>
		{
			auto* newTargetDevice = static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(td->tdSize));
			if (newTargetDevice == nullptr)
			{
				return Unexpected{
					Error{
						ErrorCode::AllocationFailure
					}.SetCustomMessage(L"Failed to allocate memory for target device")
				};
			}

			std::memcpy(newTargetDevice, td, td->tdSize);

			return TargetDevicePtr{ newTargetDevice };
		}

		[[nodiscard]] auto ToFORMATETC() const& noexcept -> Result<FORMATETC>
		{
			DVTARGETDEVICE* newPtd = nullptr;
			if (targetDevice.get() != nullptr)
			{
				auto newPtdResult = CopyTargetDevice(targetDevice.get());

				if (!newPtdResult.has_value())
				{
					return Unexpected{ newPtdResult.error() };
				}

				newPtd = newPtdResult.value().release();
			}

			return FORMATETC{
				.cfFormat = ToUnderlying<CLIPFORMAT>(format),
				.ptd = newPtd,
				.dwAspect = ToUnderlying<DWORD>(aspect),
				.lindex = index,
				.tymed = ToUnderlying<DWORD>(storageMediumType)
			};
		}

		[[nodiscard]] auto ToFORMATETC() && noexcept -> Result<FORMATETC>
		{
			DVTARGETDEVICE* newPtd = nullptr;

			if (targetDevice.get() != nullptr)
			{
				newPtd = targetDevice.release();
			}

			return FORMATETC{
				.cfFormat = ToUnderlying<CLIPFORMAT>(format),
				.ptd = newPtd,
				.dwAspect = ToUnderlying<DWORD>(aspect),
				.lindex = index,
				.tymed = ToUnderlying<DWORD>(storageMediumType)
			};
		}

		// ReSharper restore CppInconsistentNaming
		// ReSharper restore IdentifierTypo

		[[nodiscard]] auto Copy() const& noexcept -> Result<FormatData>
		{
			FormatData copy{ format, aspect, storageMediumType, nullptr, index };

			if (targetDevice.get() != nullptr)
			{
				auto newPtdResult = CopyTargetDevice(targetDevice.get());
				if (!newPtdResult.has_value())
				{
					return Unexpected{ newPtdResult.error() };
				}
				copy.targetDevice = std::move(newPtdResult).value();
			}

			return copy;
		}

		private:
		FormatData() noexcept = default;
	};

	struct FormatDataView
	{
		ClipboardFormat format{};
		const DVTARGETDEVICE* targetDevice = nullptr;
		DVAspect aspect{};
		long index = -1;
		StorageMediumType storageMediumType{};

		constexpr explicit FormatDataView(const FORMATETC& fmt) noexcept :
			format{ FromUnderlying<ClipboardFormat>(fmt.cfFormat) },
			targetDevice{ fmt.ptd },
			aspect{ FromUnderlying<DVAspect>(fmt.dwAspect) },
			index{ fmt.lindex },
			storageMediumType{ FromUnderlying<StorageMediumType>(fmt.tymed) }
		{}

		constexpr explicit FormatDataView(const FormatData& fd) noexcept :
			format{ fd.format },
			targetDevice{ fd.targetDevice.get() },
			aspect{ fd.aspect },
			index{ fd.index },
			storageMediumType{ fd.storageMediumType }
		{}

		[[nodiscard]] constexpr auto operator==(const FormatDataView&) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator==(const FORMATETC& fmt) const noexcept -> bool
		{
			return format == FromUnderlying<ClipboardFormat>(fmt.cfFormat) &&
				targetDevice == fmt.ptd &&
				aspect == FromUnderlying<DVAspect>(fmt.dwAspect) &&
				index == fmt.lindex &&
				storageMediumType == FromUnderlying<StorageMediumType>(fmt.tymed);
		}
		[[nodiscard]] constexpr auto operator==(const FormatData& fd) const noexcept -> bool
		{
			return format == fd.format &&
				targetDevice == fd.targetDevice.get() &&
				aspect == fd.aspect &&
				index == fd.index &&
				storageMediumType == fd.storageMediumType;
		}
	};

	struct StorageHolder
	{
		using FilePath = LPOLESTR;

		using IStreamPtr = ComPtr<IStream>;
		using IStoragePtr = ComPtr<IStorage>;

		using StorageHolderTypes = TypeList<HBITMAP,
			Handles::HMetaFilePict,
			HENHMETAFILE,
			Handles::HGlobal,
			FilePath,
			IStreamPtr,
			IStoragePtr,
			std::monostate>;

		using StorageVariant = StorageHolderTypes::Rebind<std::variant>;

		StorageHolder() noexcept = default;

		template <typename T> requires StorageHolderTypes::Contains<T>
		explicit StorageHolder(const T& data) noexcept : underlyingData{ data }
		{}

		StorageHolder(const StorageHolder&) = delete;
		auto operator=(const StorageHolder&) -> StorageHolder & = delete;
		StorageHolder(StorageHolder&&) noexcept = default;
		auto operator=(StorageHolder&&) noexcept -> StorageHolder & = default;

		[[nodiscard]] constexpr auto HasDataOfType(const StorageMediumType type) const noexcept -> Result<bool>
		{
			if (HasMultipleFlagsSet(type))
			{
				return Unexpected{ Error{
					ErrorCode::InvalidArgument
				}.SetCustomMessage(L"Multiple flags are not allowed")
				};
			}

			switch (type)
			{
				using enum StorageMediumType;
				case HGlobalMemory:
				{
					return std::holds_alternative<Handles::HGlobal>(underlyingData);
				}
				case File:
				{
					return std::holds_alternative<FilePath>(underlyingData);
				}
				case IStream:
				{
					return std::holds_alternative<ComPtr<::IStream>>(underlyingData);
				}
				case IStorage:
				{
					return std::holds_alternative<ComPtr<::IStorage>>(underlyingData);
				}
				case GDI:
				{
					return std::holds_alternative<HBITMAP>(underlyingData);
				}
				case MetaFilePict:
				{
					return std::holds_alternative<Handles::HMetaFilePict>(underlyingData);
				}
				case EnhancedMetaFile:
				{
					return std::holds_alternative<HENHMETAFILE>(underlyingData);
				}
				default:
				{
					return false;
				}
			}
		}

		template <typename T> requires StorageHolderTypes::Contains<T>
		[[nodiscard]] auto GetDataOfType() const noexcept -> Result<T>
		{
			if (!std::holds_alternative<T>(underlyingData))
			{
				return Unexpected{ Error{
					ErrorCode::InvalidArgument
				}.SetCustomMessage(L"Storage does not contain data of the requested type")
				};
			}

			return std::get<T>(underlyingData);
		}

		auto Clear() noexcept -> void
		{
			underlyingData = std::monostate{ };
		}

		private:
		StorageVariant underlyingData;
	};

	struct StorageMedium
	{
		StorageMediumType type = StorageMediumType::Null;
		StorageHolder storage;
		ComPtr<IUnknown> unknown;

		[[nodiscard]] static auto CopyFrom(const STGMEDIUM& medium) noexcept -> Result<StorageMedium>;
		[[nodiscard]] static auto MoveFrom(STGMEDIUM&& medium) noexcept -> StorageMedium;

		constexpr StorageMedium() = default;
		~StorageMedium() noexcept;

		StorageMedium(const StorageMedium&) = delete;
		auto operator=(const StorageMedium&) -> StorageMedium & = delete;
		StorageMedium(StorageMedium&& other) noexcept;
		auto operator=(StorageMedium&& other) noexcept -> StorageMedium&;

		// ReSharper disable CppInconsistentNaming
		// ReSharper disable IdentifierTypo

		[[nodiscard]] auto CopyToSTGMEDIUM() const noexcept -> Result<STGMEDIUM>;
		[[nodiscard]] auto MoveToSTGMEDIUM() noexcept -> STGMEDIUM;

		auto WriteToSTGMEDIUM(const STGMEDIUM& dest) const noexcept -> Error;

		// ReSharper restore IdentifierTypo
		// ReSharper restore CppInconsistentNaming

		[[nodiscard]] auto Copy() const noexcept -> Result<StorageMedium>;

		private:
		[[nodiscard]] static auto CopyHolder(StorageMediumType type, const StorageHolder& holder) noexcept -> Result<StorageHolder>;

		bool dataOwned = false;
	};
}

