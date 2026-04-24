module;
#include <Windows.h>

#include <objidl.h>
#include <strsafe.h>
#include <wil/resource.h>

module PGUI.UI.DragDrop.DragDropStructs;

import PGUI.UI.DragDrop.DragDropEnums;
import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.ComPtr;

import std;

namespace PGUI::UI::DragDrop
{
	auto StorageMedium::CopyFrom(const STGMEDIUM& medium) noexcept -> Result<StorageMedium>
	{
		StorageMedium result;
		result.type = FromUnderlying<StorageMediumType>(medium.tymed);

		StorageHolder holder;
		switch (result.type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				holder = StorageHolder{ Handles::HGlobal{ medium.hGlobal } };
				break;
			}
			case StorageMediumType::File:
			{
				holder = StorageHolder{ medium.lpszFileName };
				break;
			}
			case StorageMediumType::GDI:
			{
				holder = StorageHolder{ medium.hBitmap };
				break;
			}
			case StorageMediumType::MetaFilePict:
			{
				holder = StorageHolder{ Handles::HMetaFilePict{ medium.hMetaFilePict } };
				break;
			}
			case StorageMediumType::EnhancedMetaFile:
			{
				holder = StorageHolder{ medium.hEnhMetaFile };
				break;
			}
			case StorageMediumType::IStream:
			{
				holder = StorageHolder{ ComPtr{ medium.pstm } };
				break;
			}
			case StorageMediumType::IStorage:
			{
				holder = StorageHolder{ ComPtr{ medium.pstg } };
				break;
			}
			default:
			{
				return Unexpected{
					Error{
						ErrorCode::InvalidArgument
					}.SetCustomMessage(L"Storage medium does not contain data or contains unsupported type for copying")
				};
			}
		}

		auto holderCopyResult = CopyHolder(result.type, holder);
		if (!holderCopyResult)
		{
			return Unexpected{ holderCopyResult.error() };
		}
		result.storage = std::move(holderCopyResult.value());
		result.unknown = ComPtr{ medium.pUnkForRelease };
		result.dataOwned = true;

		return result;
	}

	auto StorageMedium::MoveFrom(STGMEDIUM&& medium) noexcept -> StorageMedium
	{
		StorageMedium result;
		result.type = FromUnderlying<StorageMediumType>(std::exchange(medium.tymed, 0));

		switch (result.type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				result.storage = StorageHolder{
					Handles::HGlobal{ std::exchange(medium.hGlobal, nullptr) }
				};
				break;
			}
			case StorageMediumType::File:
			{
				result.storage = StorageHolder{ std::exchange(medium.lpszFileName, nullptr) };
				break;
			}
			case StorageMediumType::IStream:
			{
				result.storage = StorageHolder{ ComPtr{ std::exchange(medium.pstm, nullptr) } };
				break;
			}
			case StorageMediumType::IStorage:
			{
				result.storage = StorageHolder{ ComPtr{ std::exchange(medium.pstg, nullptr) } };
				break;
			}
			case StorageMediumType::GDI:
			{
				result.storage = StorageHolder{ std::exchange(medium.hBitmap, nullptr) };
				break;
			}
			case StorageMediumType::MetaFilePict:
			{
				result.storage = StorageHolder{
					Handles::HMetaFilePict{ std::exchange(medium.hMetaFilePict, nullptr) }
				};
				break;
			}
			case StorageMediumType::EnhancedMetaFile:
			{
				result.storage = StorageHolder{ std::exchange(medium.hEnhMetaFile, nullptr) };
				break;
			}
			default: ;
		}

		result.unknown.attach(std::exchange(medium.pUnkForRelease, nullptr));
		result.dataOwned = true;

		return result;
	}

	StorageMedium::~StorageMedium() noexcept
	{
		if (!dataOwned)
		{
			return;
		}
		if (unknown.get() != nullptr)
		{
			if (const auto fileResult = storage.GetDataOfType<StorageHolder::FilePath>();
				fileResult.has_value() && fileResult.value() != nullptr)
			{
				CoTaskMemFree(fileResult.value());
			}
			return;
		}
		switch (type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				const auto& hGlobal = storage.GetDataOfType<Handles::HGlobal>().value().handle;
				GlobalFree(hGlobal);
				break;
			}
			case StorageMediumType::File:
			{
				const auto& filePath = storage.GetDataOfType<StorageHolder::FilePath>().value();
				DeleteFileW(filePath);
				CoTaskMemFree(filePath);
				break;
			}
			case StorageMediumType::GDI:
			{
				const auto& bmp = storage.GetDataOfType<HBITMAP>().value();
				DeleteObject(bmp);
				break;
			}
			case StorageMediumType::EnhancedMetaFile:
			{
				const auto& emf = storage.GetDataOfType<HENHMETAFILE>().value();
				DeleteEnhMetaFile(emf);
				break;
			}
			case StorageMediumType::MetaFilePict:
			{
				const auto& mfp = storage.GetDataOfType<Handles::HMetaFilePict>().value().handle;
				{
					const wil::unique_hglobal_locked mfpLock{ mfp };
					if (mfpLock.is_valid())
					{
						const auto& mfpData = *static_cast<METAFILEPICT*>(mfpLock.get());
						DeleteMetaFile(mfpData.hMF);
					}
				}
				GlobalFree(mfp);
				break;
			}
			default: ;
		}
	}

	StorageMedium::StorageMedium(StorageMedium&& other) noexcept :
		type{ std::exchange(other.type, StorageMediumType::Null) },
		storage{ std::move(other.storage) },
		unknown{ std::move(other.unknown) },
		dataOwned{ std::exchange(other.dataOwned, false) }
	{ }

	auto StorageMedium::operator=(StorageMedium&& other) noexcept -> StorageMedium&
	{
		if (this != &other)
		{
			type = std::exchange(other.type, StorageMediumType::Null);
			storage = std::move(other.storage);
			unknown.swap(other.unknown);
			dataOwned = std::exchange(other.dataOwned, false);
		}

		return *this;
	}

	// ReSharper disable CppInconsistentNaming
	// ReSharper disable IdentifierTypo

	auto StorageMedium::CopyToSTGMEDIUM() const noexcept -> Result<STGMEDIUM>
	{
		auto holderCopyResult = CopyHolder(type, storage);
		if (!holderCopyResult)
		{
			return Unexpected{ holderCopyResult.error() };
		}
		STGMEDIUM medium{ };
		medium.tymed = ToUnderlying(type);
		medium.pUnkForRelease = ComPtr{ unknown }.detach();
		switch (type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				medium.hGlobal = holderCopyResult.value().GetDataOfType<Handles::HGlobal>().value().handle;
				break;
			}
			case StorageMediumType::File:
			{
				medium.lpszFileName = holderCopyResult.value().GetDataOfType<StorageHolder::FilePath>().value();
				break;
			}
			case StorageMediumType::IStream:
			{
				using IStreamPtr = StorageHolder::IStreamPtr;
				medium.pstm = IStreamPtr{
					holderCopyResult.value().GetDataOfType<IStreamPtr>().value()
				}.detach();
				break;
			}
			case StorageMediumType::IStorage:
			{
				using IStoragePtr = StorageHolder::IStoragePtr;
				medium.pstg = IStoragePtr{
					holderCopyResult.value().GetDataOfType<IStoragePtr>().value()
				}.detach();
				break;
			}
			case StorageMediumType::GDI:
			{
				medium.hBitmap = holderCopyResult.value().GetDataOfType<HBITMAP>().value();
				break;
			}
			case StorageMediumType::MetaFilePict:
			{
				medium.hMetaFilePict = holderCopyResult.value().GetDataOfType<Handles::HMetaFilePict>().value().handle;
				break;
			}
			case StorageMediumType::EnhancedMetaFile:
			{
				medium.hEnhMetaFile = holderCopyResult.value().GetDataOfType<HENHMETAFILE>().value();
				break;
			}
			default: ;
		}

		// ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
		return medium;
	}

	auto StorageMedium::MoveToSTGMEDIUM() noexcept -> STGMEDIUM
	{
		STGMEDIUM medium{ };

		switch (type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				medium.hGlobal = storage.GetDataOfType<Handles::HGlobal>().value().handle;
				break;
			}
			case StorageMediumType::File:
			{
				medium.lpszFileName = storage.GetDataOfType<StorageHolder::FilePath>().value();
				break;
			}
			case StorageMediumType::IStream:
			{
				medium.pstm = storage.GetDataOfType<StorageHolder::IStreamPtr>().value().detach();
				break;
			}
			case StorageMediumType::IStorage:
			{
				medium.pstg = storage.GetDataOfType<StorageHolder::IStoragePtr>().value().detach();
				break;
			}
			case StorageMediumType::GDI:
			{
				medium.hBitmap = storage.GetDataOfType<HBITMAP>().value();
				break;
			}
			case StorageMediumType::MetaFilePict:
			{
				medium.hMetaFilePict = storage.GetDataOfType<Handles::HMetaFilePict>().value().handle;
				break;
			}
			case StorageMediumType::EnhancedMetaFile:
			{
				medium.hEnhMetaFile = storage.GetDataOfType<HENHMETAFILE>().value();
				break;
			}
			default: ;
		}

		dataOwned = false;
		storage.Clear();
		medium.tymed = ToUnderlying(std::exchange(type, StorageMediumType::Null));
		medium.pUnkForRelease = unknown.detach();

		// ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
		return medium;
	}

	auto StorageMedium::WriteToSTGMEDIUM(const STGMEDIUM& dest) const noexcept -> Error
	{
		if (dest.tymed != ToUnderlying(type))
		{
			return Error{
				DV_E_TYMED
			}.SetCustomMessage(L"Destination storage medium type does not match");
		}

		switch (type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				const auto& hGlobal = storage.GetDataOfType<Handles::HGlobal>().value().handle;
				const wil::unique_hglobal_locked globalLocked{ hGlobal };
				if (!globalLocked.is_valid())
				{
					return Error{
						GetLastError()
					}.SetCustomMessage(L"Failed to lock global memory");
				}
				const wil::unique_hglobal_locked destGlobalLocked{ dest.hGlobal };
				if (!destGlobalLocked.is_valid())
				{
					return Error{
						GetLastError()
					}.SetCustomMessage(L"Failed to lock destination global memory");
				
				}

				SetLastError(ERROR_SUCCESS);
				const auto srcSize = GlobalSize(hGlobal);
				const auto destSize = GlobalSize(dest.hGlobal);
				if (const auto lastError = GetLastError();
					(srcSize == 0 || destSize == 0) && lastError != ERROR_SUCCESS)
				{
					return Error{
						lastError
					}.SetCustomMessage(L"Failed to get size of global memory block");
				}
				if (srcSize > destSize)
				{
					return Error{
							STG_E_MEDIUMFULL
						}.SetCustomMessage(L"Destination global memory block is too small to hold the data")
						 .AddDetail(L"Required Size", std::to_wstring(srcSize))
						 .AddDetail(L"Destination Size", std::to_wstring(destSize));
				}

				std::memcpy(destGlobalLocked.get(), globalLocked.get(), srcSize);

				break;
			}
			case StorageMediumType::File:
			{
				if (!CopyFileW(storage.GetDataOfType<StorageHolder::FilePath>().value(), dest.lpszFileName, false))
				{
					return Error{
						GetLastError()
					}.SetCustomMessage(L"Failed to copy file");
				}

				break;
			}
			case StorageMediumType::IStream:
			{
				const auto& srcStream = storage.GetDataOfType<StorageHolder::IStreamPtr>().value();
				auto hr = srcStream->Seek({ }, STREAM_SEEK_SET, nullptr);
				if (FAILED(hr))
				{
					return Error{
						hr
					}.SetCustomMessage(L"Failed to seek to the beginning of the source stream");
				}
				const auto& destStream = dest.pstm;
				hr = srcStream->CopyTo(
					destStream,
					{ .QuadPart = std::numeric_limits<ULONGLONG>::max() },
					nullptr, nullptr);

				if (FAILED(hr))
				{
					return Error{
						hr
					}.SetCustomMessage(L"Failed to copy data from source stream to destination stream");
				}

				break;
			}
			case StorageMediumType::IStorage:
			{
				const auto& srcStorage = storage.GetDataOfType<StorageHolder::IStoragePtr>().value();
				if (const auto hr = srcStorage->CopyTo(0, nullptr, nullptr, dest.pstg);
					FAILED(hr))
				{
					return Error{
						hr
					}.SetCustomMessage(L"Failed to copy data from source storage to destination storage");
				}

				break;
			}

			case StorageMediumType::Null:
			case StorageMediumType::GDI:
			case StorageMediumType::MetaFilePict:
			case StorageMediumType::EnhancedMetaFile:
			{
				return Error{ DV_E_TYMED };
			}
			default: ;
		}

		return Error{ ErrorCode::Success };
	}

	// ReSharper restore IdentifierTypo
	// ReSharper restore CppInconsistentNaming

	auto StorageMedium::Copy() const noexcept -> Result<StorageMedium>
	{
		auto holderCopyResult = CopyHolder(type, storage);
		if (!holderCopyResult)
		{
			return Unexpected{ holderCopyResult.error() };
		}
		
		StorageMedium copy;
		copy.type = type;
		copy.storage = std::move(holderCopyResult.value());
		copy.unknown = unknown;
		copy.dataOwned = true;
		return copy;
	}

	auto StorageMedium::CopyHolder(const StorageMediumType type,
	                               const StorageHolder& holder) noexcept -> Result<StorageHolder>
	{
		switch (type)
		{
			case StorageMediumType::HGlobalMemory:
			{
				const auto& hGlobal = holder.GetDataOfType<Handles::HGlobal>().value().handle;
				SetLastError(ERROR_SUCCESS);
				const auto size = GlobalSize(hGlobal);
				if (const auto lastError = GetLastError();
					size == 0 && lastError != ERROR_SUCCESS)
				{
					return Unexpected{
						Error{
							lastError
						}.SetCustomMessage(L"Failed to get size of the global memory block")
					};
				}
				
				wil::unique_hglobal mem{ GlobalAlloc(GlobalFlags(hGlobal) & ~GMEM_LOCKCOUNT, size) };
				if (!mem.is_valid())
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to allocate global memory block for copying")
					};
				}
				const wil::unique_hglobal_locked srcLock{ hGlobal };
				if (!srcLock.is_valid())
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to lock source global memory block for copying")
					};
				}
				const wil::unique_hglobal_locked dstLock{ mem.get() };
				if (!dstLock.is_valid())
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to lock destination global memory block for copying")
					};
				}

				std::memcpy(dstLock.get(), srcLock.get(), size);

				return StorageHolder{ Handles::HGlobal{ mem.release() } };
			}
			case StorageMediumType::File:
			{
				const auto& filePath = holder.GetDataOfType<StorageHolder::FilePath>().value();
				const std::wstring_view view{ filePath };
				const auto size = view.size() + 1;
				const auto dest = CoTaskMemAlloc(size * sizeof(wchar_t));
				
				if (!dest)
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to allocate memory for file path")
					};
				}

				if (Error error{ StringCchCopyW(static_cast<wchar_t*>(dest), size, view.data()) };
					error.IsFailure())
				{
					CoTaskMemFree(dest);
					return Unexpected{ error.SetCustomMessage(L"Failed to copy file path to allocated memory") };
				}

				return StorageHolder{ static_cast<StorageHolder::FilePath>(dest) };
			}
			case StorageMediumType::GDI:
			{
				const auto& bmp = holder.GetDataOfType<HBITMAP>().value();
				const auto copy = static_cast<HBITMAP>(CopyImage(bmp, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG));

				if (!copy)
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to copy bitmap")
					};
				}

				return StorageHolder{ copy };
			}
			case StorageMediumType::EnhancedMetaFile:
			{
				const auto& emf = holder.GetDataOfType<HENHMETAFILE>().value();
				const auto copy = CopyEnhMetaFileW(emf, nullptr);

				if (!copy)
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to copy enhanced metafile")
					};
				}

				return StorageHolder{ copy };
			}
			case StorageMediumType::MetaFilePict:
			{
				const auto& mfp = holder.GetDataOfType<Handles::HMetaFilePict>().value().handle;
				const wil::unique_hglobal_locked mfpLock{ mfp };
				if (!mfpLock.is_valid())
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to lock metafile picture for copying")
					};
				}
				// ReSharper disable once CppUseStructuredBinding

				const auto& mfpData = *static_cast<METAFILEPICT*>(mfpLock.get());
				const auto copied = CopyMetaFileW(mfpData.hMF, nullptr);
				if (!copied)
				{
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to copy metafile")
					};
				}

				wil::unique_hglobal destMfp{ GlobalAlloc(GlobalFlags(mfp) & ~GMEM_LOCKCOUNT, sizeof(METAFILEPICT)) };
				if (!destMfp)
				{
					DeleteMetaFile(copied);
					return Unexpected{
						Error{
							GetLastError()
						}.SetCustomMessage(L"Failed to allocate memory for metafile picture")
					};
				}
				{
					const wil::unique_hglobal_locked destMfpLock{ destMfp.get() };
					if (!destMfpLock.is_valid())
					{
						DeleteMetaFile(copied);
						return Unexpected{
							Error{
								GetLastError()
							}.SetCustomMessage(L"Failed to lock memory for metafile picture")
						};
					}
					// ReSharper disable once CppUseStructuredBinding

					auto& destMfpData = *static_cast<METAFILEPICT*>(destMfpLock.get());
					destMfpData.mm = mfpData.mm;
					destMfpData.xExt = mfpData.xExt;
					destMfpData.yExt = mfpData.yExt;
					destMfpData.hMF = copied;
				}

				return StorageHolder{ Handles::HMetaFilePict{ destMfp.release() } };
			}
			case StorageMediumType::IStorage:
			{
				const auto& storagePtr = holder.GetDataOfType<StorageHolder::IStoragePtr>().value();
				return StorageHolder{ storagePtr };
			}
			case StorageMediumType::IStream:
			{
				const auto& streamPtr = holder.GetDataOfType<StorageHolder::IStreamPtr>().value();
				return StorageHolder{ streamPtr };
			}
			default:
			{
				return Unexpected{
					Error{
						ErrorCode::InvalidArgument
					}.SetCustomMessage(L"Storage medium does not contain data or contains unsupported type for copying")
				};
			}
		}
	}
}
