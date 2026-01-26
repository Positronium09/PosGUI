module;
#include <ole2.h>
#include <Windows.h>

module PGUI.UI.DragDrop;

import PGUI.Event;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import PGUI.UI.Input;

namespace PGUI::UI
{
	auto DragDrop::DragEnter(IDataObject* pDataObj, const DWORD grfKeyState,
	                         const POINTL pt,
	                         DWORD* pdwEffect) -> HRESULT
	{
		if (pdwEffect == nullptr)
		{
			return E_INVALIDARG;
		}

		const auto modifierKeys = GetModifierKeysFromWparam(grfKeyState);
		const auto mouseButton = GetMouseButtonsFromWparam(grfKeyState);
		const DragDropModifierInfo modifierInfo{ modifierKeys, mouseButton };
		const PointF point{
			static_cast<float>(pt.x),
			static_cast<float>(pt.y)
		};

		try
		{
			auto effect = FromUnderlying<DragDropEffect>(*pdwEffect);
			OnDragEnter(pDataObj, modifierInfo, point, effect);
			*pdwEffect = ToUnderlying(effect);
		}
		catch (Exception& exception)
		{
			Logger::Error(exception.GetError());
			if (exception.HResult() == E_OUTOFMEMORY)
			{
				return E_OUTOFMEMORY;
			}
			return E_UNEXPECTED;
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}

		return S_OK;
	}

	auto DragDrop::DragOver(const DWORD grfKeyState, const POINTL pt, DWORD* pdwEffect) -> HRESULT
	{
		if (pdwEffect == nullptr)
		{
			return E_INVALIDARG;
		}
		const auto modifierKeys = GetModifierKeysFromWparam(grfKeyState);
		const auto mouseButton = GetMouseButtonsFromWparam(grfKeyState);
		const DragDropModifierInfo modifierInfo{ modifierKeys, mouseButton };
		const PointF point{
			static_cast<float>(pt.x),
			static_cast<float>(pt.y)
		};

		try
		{
			auto effect = FromUnderlying<DragDropEffect>(*pdwEffect);
			OnDragOver(modifierInfo, point, effect);
			*pdwEffect = ToUnderlying(effect);
		}
		catch (Exception& exception)
		{
			Logger::Error(exception.GetError());
			if (exception.HResult() == E_OUTOFMEMORY)
			{
				return E_OUTOFMEMORY;
			}
			return E_UNEXPECTED;
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
		return S_OK;
	}

	auto DragDrop::DragLeave() -> HRESULT
	{
		try
		{
			OnDragLeave();
		}
		catch (Exception& exception)
		{
			Logger::Error(exception.GetError());
			if (exception.HResult() == E_OUTOFMEMORY)
			{
				return E_OUTOFMEMORY;
			}
			return E_UNEXPECTED;
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
		return S_OK;
	}

	auto DragDrop::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) -> HRESULT
	{
		if (pdwEffect == nullptr)
		{
			return E_INVALIDARG;
		}
		
		const auto modifierKeys = GetModifierKeysFromWparam(grfKeyState);
		const auto mouseButton = GetMouseButtonsFromWparam(grfKeyState);
		const DragDropModifierInfo modifierInfo{ modifierKeys, mouseButton };
		const PointF point{
			static_cast<float>(pt.x),
			static_cast<float>(pt.y)
		};

		try
		{
			auto effect = FromUnderlying<DragDropEffect>(*pdwEffect);
			OnDrop(pDataObj, modifierInfo, point, effect);
			*pdwEffect = ToUnderlying(effect);
		}
		catch (Exception& exception)
		{
			Logger::Error(exception.GetError());
			if (exception.HResult() == E_OUTOFMEMORY)
			{
				return E_OUTOFMEMORY;
			}
			return E_UNEXPECTED;
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
		return S_OK;
	}
}
