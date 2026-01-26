module;
#include <ole2.h>
#include <Windows.h>

module PGUI.UI.DragDrop.DropSource;

import PGUI.Event;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import PGUI.UI.Input;

namespace PGUI::UI::DragDrop
{
	auto DropSource::QueryContinueDrag(const BOOL fEscapePressed, const DWORD grfKeyState) -> HRESULT
	{
		try
		{
			const auto modifierKeys = GetModifierKeysFromWparam(grfKeyState);
			const auto mouseButton = GetMouseButtonsFromWparam(grfKeyState);
			const DragDropModifierInfo modifierInfo{ modifierKeys, mouseButton };

			OnQueryContinueDrag(fEscapePressed != FALSE, modifierInfo);

			return S_OK;
		}
		catch (Exception& exception)
		{
			Logger::Error(exception.GetError());
			return exception.HResult();
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
	}

	auto DropSource::GiveFeedback(const DWORD dwEffect) -> HRESULT
	{
		try
		{
			const auto effect = FromUnderlying<DragDropEffect>(dwEffect);
			OnGiveFeedback(effect);

			return S_OK;
		}
		catch (Exception& exception)
		{
			Logger::Error(exception.GetError());
			return exception.HResult();
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
	}
}
