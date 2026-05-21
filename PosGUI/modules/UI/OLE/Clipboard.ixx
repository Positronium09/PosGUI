module;
#include <Windows.h>

#include <ole2.h>

export module PGUI.UI.OLE.Clipboard;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.OLE.DataObject;

import std;

export namespace PGUI::UI::OLE
{
	template <DataObjectHandler H, bool B>
	auto SetClipboard(const ComPtr<DataObject<H, B>>& dataObject) noexcept -> Result<void>
	{
		if (Error error{ OleSetClipboard(dataObject.get()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto SetClipboard(const ComPtr<IDataObject>& dataObject) noexcept -> Result<void>
	{
		if (Error error{ OleSetClipboard(dataObject.get()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto ClearClipboard() noexcept -> Result<void>
	{
		if (Error error{ OleSetClipboard(nullptr) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	[[nodiscard]] auto GetClipboard() noexcept -> Result<DataObjectReadWrite>
	{
		ComPtr<IDataObject> dataObject;
		if (const auto error = Error{ OleGetClipboard(dataObject.put()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return DataObjectReadWrite{ std::move(dataObject) };
	}

	[[nodiscard]] auto IsClipboardEmpty() noexcept -> bool
	{
		return CountClipboardFormats() == 0;
	}

	[[nodiscard]] auto FlushClipboard() noexcept -> Result<void>
	{
		if (Error error{ OleFlushClipboard() };
			error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	[[nodiscard]] auto IsCurrentClipboard(const ComPtr<IDataObject>& dataObject) noexcept -> Result<bool>
	{
		const auto error = Error{ OleIsCurrentClipboard(dataObject.get()) };
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}

		return error.HResult() == S_OK;
	}
}
