module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wil/resource.h>

module PGUI.WinResource;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

namespace PGUI::Resource
{
	auto LoadFromModule(const HMODULE moduleHandle, const ResourceID& id,
	                    const ResourceType& type) noexcept -> Result<RawDataModulePair>
	{
		const auto sourceHandle = FindResourceW(moduleHandle, id, type);
		if (sourceHandle == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}
		auto* hGlobal = LoadResource(moduleHandle, sourceHandle);
		if (hGlobal == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}
		const auto size = static_cast<std::size_t>(SizeofResource(moduleHandle, sourceHandle));
		if (size == 0)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		auto* dataPtr = static_cast<std::byte*>(LockResource(hGlobal));
		if (dataPtr == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		return RawDataModulePair{
			std::span{
				dataPtr,
				size
			}
		};
	}

	auto LoadAccelerator(const HMODULE moduleHandle, const ResourceID& id) noexcept -> Result<AcceleratorModulePair>
	{
		const auto acceleratorHandle = LoadAcceleratorsW(moduleHandle, id);
		if (acceleratorHandle == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		return AcceleratorModulePair{
			wil::unique_haccel{ acceleratorHandle }
		};
	}

	auto LoadBitmapResource(const HMODULE moduleHandle, const ResourceID& id) noexcept -> Result<BitmapModulePair>
	{
		const auto bitmapHandle = LoadImageW(moduleHandle, id, IMAGE_BITMAP, 0, 0,
		                                     LR_CREATEDIBSECTION | LR_DEFAULTCOLOR);
		if (bitmapHandle == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		return BitmapModulePair{
			wil::unique_hbitmap{ static_cast<HBITMAP>(bitmapHandle) }
		};
	}

	auto LoadCursorResource(const HMODULE moduleHandle, const ResourceID& id,
	                        const SizeI size) noexcept -> Result<CursorModulePair>
	{
		const auto cursorHandle = LoadImageW(moduleHandle, id, IMAGE_CURSOR, size.cx, size.cy, LR_DEFAULTCOLOR);

		if (cursorHandle == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		return CursorModulePair{
			wil::unique_hcursor{ static_cast<HCURSOR>(cursorHandle) }
		};
	}

	auto LoadIconResource(const HMODULE moduleHandle, const ResourceID& id,
	                      const SizeI size) noexcept -> Result<IconModulePair>
	{
		const auto iconHandle = LoadImageW(moduleHandle, id, IMAGE_ICON, size.cx, size.cy, LR_DEFAULTCOLOR);

		if (iconHandle == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		return IconModulePair{
			wil::unique_hicon{ static_cast<HICON>(iconHandle) }
		};
	}

	auto LoadMenuResource(const HMODULE moduleHandle, const ResourceID& id) noexcept -> Result<MenuModulePair>
	{
		const auto menuHandle = LoadMenuW(moduleHandle, id);
		if (menuHandle == nullptr)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", static_cast<const wchar_t*>(id))
			};
		}

		return MenuModulePair{
			wil::unique_hmenu{ menuHandle }
		};
	}

	auto LoadStringResource(const HMODULE moduleHandle, const UINT id) noexcept -> Result<std::wstring>
	{
		const auto count = LoadStringW(moduleHandle, id, nullptr, 0);
		if (count == 0)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", std::to_wstring(id))
			};
		}

		std::wstring buffer(count + 1, L'\0');
		if (const auto copied = LoadStringW(moduleHandle, id, buffer.data(), count);
			copied != count)
		{
			return Unexpected{
				Error{
					GetLastError(),
				}.SetCustomMessage(L"The copied character count doesnt match the count of the string")
				 .AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle)))
				 .AddDetail(L"Resource ID", std::to_wstring(id))
			};
		}

		buffer.resize(count);

		return buffer;
	}
}
