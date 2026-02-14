module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <objidl.h>
#include <shlwapi.h>
#include <wil/resource.h>

export module PGUI.WinResource;

import std;

import PGUI.Utils;
import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI
{
	using RawResourceID = const wchar_t*;

	[[nodiscard]] auto MakeIntResource(const std::integral auto& id) noexcept
	{
		return static_cast<LPCWSTR>(
			reinterpret_cast<LPWSTR>(
			static_cast<ULONG_PTR>(
			static_cast<WORD>(id))));
	}

	struct ResourceID
	{
		explicit(false) ResourceID(const std::integral auto& id) noexcept : 
			id{ MakeIntResource(id) }
		{
		}

		explicit(false) constexpr ResourceID(const std::wstring_view id) noexcept :
			id{ id.data() }
		{
		}

		explicit(false) constexpr ResourceID(const LPCWSTR id) noexcept :
			id{ id }
		{
		}

		explicit(false) constexpr operator const wchar_t*() const noexcept
		{
			return id;
		}

		private:
		RawResourceID id;
	};
	using ResourceType = ResourceID;

	namespace WinResourceType
	{
		const auto Accelerator = MakeIntResource(9);
		const auto AnimatedCursor = MakeIntResource(21);
		const auto AnimatedIcon = MakeIntResource(22);
		const auto Bitmap = MakeIntResource(2);
		const auto Cursor = MakeIntResource(1);
		const auto Dialog = MakeIntResource(5);
		const auto DialogInclude = MakeIntResource(17);
		const auto Font = MakeIntResource(8);
		const auto FontDirectory = MakeIntResource(7);
		const auto GroupCursor = MakeIntResource(12);
		const auto GroupIcon = MakeIntResource(14);
		const auto Html = MakeIntResource(23);
		const auto Icon = MakeIntResource(3);
		const auto Manifest = MakeIntResource(24);
		const auto Menu = MakeIntResource(4);
		const auto MessageTable = MakeIntResource(11);
		const auto PlugPlay = MakeIntResource(19);
		const auto Data = MakeIntResource(10);
		const auto String = MakeIntResource(6);
		const auto Version = MakeIntResource(16);
		const auto Vxd = MakeIntResource(20);
	}

	template <typename T>
	struct DataModulePair
	{
		explicit DataModulePair(T data, wil::unique_hmodule moduleHandle = { nullptr }) noexcept :
			data{ std::move(data) },
			moduleHandle{ std::move(moduleHandle) }
		{
		}

		template <typename Self>
		[[nodiscard]] auto&& GetData(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.data);
		}

		[[nodiscard]] auto GetSize() const noexcept
			requires std::same_as<T, std::span<const std::byte>>
		{
			return data.size();
		}

		auto AdoptModuleHandle(wil::unique_hmodule mod) noexcept
		{
			moduleHandle = std::move(mod);
		}

		[[nodiscard]] auto CreateMemoryStream() const noexcept -> Result<ComPtr<IStream>>
		requires std::same_as<T, std::span<const std::byte>>
		{
			auto* stream = SHCreateMemStream(reinterpret_cast<const BYTE*>(data.data()), GetSize());

			if (stream == nullptr)
			{
				return Unexpected{
					Error{
						GetLastError(),
					}.AddDetail(L"Module", std::format(L"{:p}", static_cast<void*>(moduleHandle.get())))
				};
			}

			return ComPtr<IStream>{ stream };
		}

		DataModulePair(const DataModulePair&) = delete;
		auto operator=(const DataModulePair&) -> DataModulePair& = delete;
		DataModulePair(DataModulePair&&) noexcept = default;
		auto operator=(DataModulePair&&) noexcept -> DataModulePair& = default;

		private:
		T data;
		wil::unique_hmodule moduleHandle;
	};

	using RawDataModulePair = DataModulePair<std::span<const std::byte>>;
	using AcceleratorModulePair = DataModulePair<wil::unique_haccel>;
	using BitmapModulePair = DataModulePair<wil::unique_hbitmap>;
	using CursorModulePair = DataModulePair<wil::unique_hcursor>;
	using IconModulePair = DataModulePair<wil::unique_hicon>;
	using MenuModulePair = DataModulePair<wil::unique_hmenu>;

	namespace Resource
	{
		[[nodiscard]] auto LoadResourceModule(const std::wstring_view moduleName) noexcept -> Result<wil::unique_hmodule>
		{
			const auto hModule = LoadLibraryExW(moduleName.data(), nullptr,
				LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);

			if (hModule == nullptr)
			{
				return Unexpected{
					Error{
						GetLastError(),
					}.AddDetail(L"Module", moduleName.data())
				};
			}

			return wil::unique_hmodule{ hModule };
		}

		[[nodiscard]] auto LoadFromModule(HMODULE moduleHandle, const ResourceID& id, const ResourceType& type) noexcept -> Result<RawDataModulePair>;
		[[nodiscard]] auto LoadFromModule(const std::wstring_view moduleName, const ResourceID& id, const ResourceType& type) noexcept -> Result<RawDataModulePair>
		{
			auto hModule = LoadResourceModule(moduleName);

			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			auto result = LoadFromModule(hModule.value().get(), id, type);
			if (result.has_value())
			{
				result.value().AdoptModuleHandle(std::move(hModule.value()));
			}

			return result;
		}
		[[nodiscard]] auto Load(const ResourceID& id, const ResourceType& type) noexcept
		{
			return LoadFromModule(nullptr, id, type);
		}
		[[nodiscard]] auto LoadRaw(const HMODULE moduleHandle, const ResourceID& id) noexcept
		{
			return LoadFromModule(moduleHandle, id, WinResourceType::Data);
		}
		[[nodiscard]] auto LoadRaw(const ResourceID& id, const HMODULE moduleHandle = nullptr) noexcept
		{
			return LoadFromModule(moduleHandle, id, WinResourceType::Data);
		}
		[[nodiscard]] auto LoadRaw(const std::wstring_view moduleName, const ResourceID& id) noexcept
		{
			return LoadFromModule(moduleName, id, WinResourceType::Data);
		}

		[[nodiscard]] auto LoadAccelerator(HMODULE moduleHandle, const ResourceID& id) noexcept -> Result<AcceleratorModulePair>;
		[[nodiscard]] auto LoadAccelerator(const std::wstring_view moduleName, const ResourceID& id) noexcept -> Result<AcceleratorModulePair>
		{
			auto hModule = LoadResourceModule(moduleName);

			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			auto result = LoadAccelerator(hModule.value().get(), id);
			if (result.has_value())
			{
				result.value().AdoptModuleHandle(std::move(hModule.value()));
			}

			return result;
		}
		[[nodiscard]] auto LoadAccelerator(const ResourceID& id) noexcept -> Result<AcceleratorModulePair>
		{
			return LoadAccelerator(nullptr, id);
		}

		[[nodiscard]] auto LoadBitmapResource(HMODULE moduleHandle, const ResourceID& id) noexcept -> Result<BitmapModulePair>;
		[[nodiscard]] auto LoadBitmapResource(const std::wstring_view moduleName, const ResourceID& id) noexcept -> Result<BitmapModulePair>
		{
			auto hModule = LoadResourceModule(moduleName);
			
			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			auto result = LoadBitmapResource(hModule.value().get(), id);
			if (result.has_value())
			{
				result.value().AdoptModuleHandle(std::move(hModule.value()));
			}

			return result;
		}
		[[nodiscard]] auto LoadBitmapResource(const ResourceID& id) noexcept -> Result<BitmapModulePair>
		{
			return LoadBitmapResource(nullptr, id);
		}

		[[nodiscard]] auto LoadCursorResource(
			HMODULE moduleHandle, 
			const ResourceID& id, SizeI size = SizeI{ }) noexcept -> Result<CursorModulePair>;
		[[nodiscard]] auto LoadCursorResource(
			const std::wstring_view moduleName, 
			const ResourceID& id, const SizeI size = SizeI{ }) noexcept -> Result<CursorModulePair>
		{
			auto hModule = LoadResourceModule(moduleName);
			
			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			auto result = LoadCursorResource(hModule.value().get(), id, size);
			if (result.has_value())
			{
				result.value().AdoptModuleHandle(std::move(hModule.value()));
			}

			return result;
		}
		[[nodiscard]] auto LoadCursorResource(
			const ResourceID& id, 
			const SizeI size = SizeI{ }) noexcept -> Result<CursorModulePair>
		{
			return LoadCursorResource(nullptr, id, size);
		}

		[[nodiscard]] auto LoadIconResource(
			HMODULE moduleHandle, 
			const ResourceID& id, SizeI size = SizeI{ }) noexcept -> Result<IconModulePair>;
		[[nodiscard]] auto LoadIconResource(
			const std::wstring_view moduleName, 
			const ResourceID& id, const SizeI size = SizeI{ }) noexcept -> Result<IconModulePair>
		{
			auto hModule = LoadResourceModule(moduleName);
			
			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			auto result = LoadIconResource(hModule.value().get(), id, size);
			if (result.has_value())
			{
				result.value().AdoptModuleHandle(std::move(hModule.value()));
			}

			return result;
		}
		[[nodiscard]] auto LoadIconResource(
			const ResourceID& id, 
			const SizeI size = SizeI{ }) noexcept -> Result<IconModulePair>
		{
			return LoadIconResource(nullptr, id, size);
		}

		[[nodiscard]] auto LoadMenuResource(HMODULE moduleHandle, const ResourceID& id) noexcept -> Result<MenuModulePair>;
		[[nodiscard]] auto LoadMenuResource(const std::wstring_view moduleName, const ResourceID& id) noexcept -> Result<MenuModulePair>
		{
			auto hModule = LoadResourceModule(moduleName);
			
			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			auto result = LoadMenuResource(hModule.value().get(), id);
			if (result.has_value())
			{
				result.value().AdoptModuleHandle(std::move(hModule.value()));
			}

			return result;
		}
		[[nodiscard]] auto LoadMenuResource(const ResourceID& id) noexcept -> Result<MenuModulePair>
		{
			return LoadMenuResource(nullptr, id);
		}

		[[nodiscard]] auto LoadStringResource(HMODULE moduleHandle, UINT id) noexcept -> Result<std::wstring>;
		[[nodiscard]] auto LoadStringResource(const std::wstring_view moduleName, const UINT id) noexcept -> Result<std::wstring>
		{
			auto hModule = LoadResourceModule(moduleName);
			
			if (!hModule.has_value())
			{
				return Unexpected{ hModule.error() };
			}

			return LoadStringResource(hModule.value().get(), id);
		}
		[[nodiscard]] auto LoadStringResource(const UINT id) noexcept -> Result<std::wstring>
		{
			return LoadStringResource(nullptr, id);
		}
	}
}
