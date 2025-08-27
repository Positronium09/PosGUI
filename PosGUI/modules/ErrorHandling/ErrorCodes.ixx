export module PGUI.ErrorHandling:ErrorCodes;

import std;

export namespace PGUI
{
	enum class GeneralErrorCodes : int
	{
		Success = 0,
		Failure,
		OutOfRange,
		InvalidArgument,
		NotImplemented,
		NullPointer
	};

	enum class SystemErrorCodes : int
	{
		STLFailure = 100'000,
		OutOfMemory,
		InvalidHandle,
		AccessDenied,
		NotFound,
		InterfaceNotSupported
	};

	enum class DirectXErrorCodes : int
	{
		D3D11NotAvailable = 200'000,
		D3D11FeatureLevelNotSupported,
		DXGIFactoryCreationFailed,
		DXGIAdapterNotFound,
		DXGIDeviceCreationFailed,
		DXGISwapChainCreationFailed,
		DXGISwapChainResizeFailed,
		DXGISwapChainSwapFailed,
		DXGIDeviceLost,
		DXGIDeviceRemoved,
	};

	enum class DirectCompositionErrorCodes : int
	{
		DCompositionNotAvailable = 300'000,
		DCompositionDeviceCreationFailed,
		DCompositionVisualCreationFailed,
		DCompositionTargetCreationFailed,
		DCompositionCommitFailed
	};

	enum class Direct2DErrorCodes : int
	{
		D2D1NotAvailable = 400'000,
		D2D1DeviceCreationFailed,
		D2D1DeviceContextCreationFailed,
		D2D1BitmapCreationFailed,
		D2D1BrushCreationFailed
	};

	enum class DirectWriteErrorCodes : int
	{
		DWriteNotAvailable = 500'000,
		DWriteFactoryCreationFailed,
		DWriteFontFileCreationFailed,
		DWriteTextFormatCreationFailed,
		DWriteTextLayoutCreationFailed
	};

	enum class WICErrorCodes : int
	{
		WICNotAvailable = 600'000,
		WICFactoryCreationFailed,
		WICDecoderCreationFailed,
		WICFrameRetrievalFailed,
		WICBitmapCreationFailed
	};

	enum class AnimationErrorCodes : int
	{
		AnimationNotAvailable = 700'000,
		AnimationCreationFailed,
		AnimationInvalidParameters,
		AnimationUpdateFailed
	};

	template <typename T>
	concept PGUIErrorCodeEnum = std::is_enum_v<T> &&
		(std::same_as<T, GeneralErrorCodes> ||
		 std::same_as<T, SystemErrorCodes> ||
		 std::same_as<T, DirectXErrorCodes> ||
		 std::same_as<T, DirectCompositionErrorCodes> ||
		 std::same_as<T, Direct2DErrorCodes> ||
		 std::same_as<T, DirectWriteErrorCodes> ||
		 std::same_as<T, WICErrorCodes> ||
			std::same_as<T, AnimationErrorCodes>);
}

// ReSharper disable CppInconsistentNaming
export namespace std
{
	template <>
	struct is_error_code_enum<PGUI::GeneralErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::SystemErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::DirectXErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::DirectCompositionErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::Direct2DErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::DirectWriteErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::WICErrorCodes> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::AnimationErrorCodes> : true_type { };

	template <>
	struct is_error_condition_enum<PGUI::GeneralErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::SystemErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::DirectXErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::DirectCompositionErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::Direct2DErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::DirectWriteErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::WICErrorCodes> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::AnimationErrorCodes> : true_type { };
}
// ReSharper restore CppInconsistentNaming

export namespace PGUI
{
	class GeneralCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI General Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<GeneralErrorCodes>(ev))
			{
				case GeneralErrorCodes::Success:
					return "Success";
				case GeneralErrorCodes::Failure:
					return "Failure";
				case GeneralErrorCodes::OutOfRange:
					return "Out of range";
				case GeneralErrorCodes::InvalidArgument:
					return "Invalid argument";
				case GeneralErrorCodes::NotImplemented:
					return "Not implemented";
				case GeneralErrorCodes::NullPointer:
					return "Null pointer";
				default:
					return "Unknown general error";
			}
		}
	};

	class SystemCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI System Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<SystemErrorCodes>(ev))
			{
				case SystemErrorCodes::STLFailure:
					return "STL operation failure";
				case SystemErrorCodes::OutOfMemory:
					return "Out of memory";
				case SystemErrorCodes::InvalidHandle:
					return "Invalid handle";
				case SystemErrorCodes::AccessDenied:
					return "Access denied";
				case SystemErrorCodes::NotFound:
					return "Not found";
				case SystemErrorCodes::InterfaceNotSupported:
					return "Interface not supported";
				default:
					return "Unknown system error";
			}
		}
	};

	class DirectXCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI DirectX Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<DirectXErrorCodes>(ev))
			{
				case DirectXErrorCodes::D3D11NotAvailable:
					return "Direct3D 11 not available";
				case DirectXErrorCodes::D3D11FeatureLevelNotSupported:
					return "Direct3D 11 feature level not supported";
				case DirectXErrorCodes::DXGIFactoryCreationFailed:
					return "DXGI Factory creation failed";
				case DirectXErrorCodes::DXGIAdapterNotFound:
					return "DXGI Adapter not found";
				case DirectXErrorCodes::DXGIDeviceCreationFailed:
					return "DXGI Device creation failed";
				case DirectXErrorCodes::DXGISwapChainCreationFailed:
					return "DXGI Swap Chain creation failed";
				case DirectXErrorCodes::DXGISwapChainResizeFailed:
					return "DXGI Swap Chain resize failed";
				case DirectXErrorCodes::DXGISwapChainSwapFailed:
					return "DXGI Swap Chain swap/present failed";
				case DirectXErrorCodes::DXGIDeviceLost:
					return "DXGI Device lost";
				case DirectXErrorCodes::DXGIDeviceRemoved:
					return "DXGI Device removed";
				default:
					return "Unknown DirectX error";
			}
		}
	};

	class DirectCompositionCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI DirectComposition Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<DirectCompositionErrorCodes>(ev))
			{
				case DirectCompositionErrorCodes::DCompositionNotAvailable:
					return "DirectComposition not available";
				case DirectCompositionErrorCodes::DCompositionDeviceCreationFailed:
					return "DirectComposition device creation failed";
				case DirectCompositionErrorCodes::DCompositionVisualCreationFailed:
					return "DirectComposition visual creation failed";
				case DirectCompositionErrorCodes::DCompositionTargetCreationFailed:
					return "DirectComposition target creation failed";
				case DirectCompositionErrorCodes::DCompositionCommitFailed:
					return "DirectComposition commit failed";
				default:
					return "Unknown DirectComposition error";
			}
		}
	};

	class Direct2DCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI Direct2D Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<Direct2DErrorCodes>(ev))
			{
				case Direct2DErrorCodes::D2D1NotAvailable:
					return "Direct2D not available";
				case Direct2DErrorCodes::D2D1DeviceCreationFailed:
					return "Direct2D device creation failed";
				case Direct2DErrorCodes::D2D1DeviceContextCreationFailed:
					return "Direct2D device context creation failed";
				case Direct2DErrorCodes::D2D1BitmapCreationFailed:
					return "Direct2D bitmap creation failed";
				case Direct2DErrorCodes::D2D1BrushCreationFailed:
					return "Direct2D brush creation failed";
				default:
					return "Unknown Direct2D error";
			}
		}
	};

	class DirectWriteCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI DirectWrite Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<DirectWriteErrorCodes>(ev))
			{
				case DirectWriteErrorCodes::DWriteNotAvailable:
					return "DirectWrite not available";
				case DirectWriteErrorCodes::DWriteFactoryCreationFailed:
					return "DirectWrite factory creation failed";
				case DirectWriteErrorCodes::DWriteFontFileCreationFailed:
					return "DirectWrite font file creation failed";
				case DirectWriteErrorCodes::DWriteTextFormatCreationFailed:
					return "DirectWrite text format creation failed";
				case DirectWriteErrorCodes::DWriteTextLayoutCreationFailed:
					return "DirectWrite text layout creation failed";
				default:
					return "Unknown DirectWrite error";
			}
		}
	};

	class WICCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI WIC Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<WICErrorCodes>(ev))
			{
				case WICErrorCodes::WICNotAvailable:
					return "WIC not available";
				case WICErrorCodes::WICFactoryCreationFailed:
					return "WIC factory creation failed";
				case WICErrorCodes::WICDecoderCreationFailed:
					return "WIC decoder creation failed";
				case WICErrorCodes::WICFrameRetrievalFailed:
					return "WIC frame retrieval failed";
				case WICErrorCodes::WICBitmapCreationFailed:
					return "WIC bitmap creation failed";
				default:
					return "Unknown WIC error";
			}
		}
	};
	class AnimationCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI Animation Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<AnimationErrorCodes>(ev))
			{
				case AnimationErrorCodes::AnimationNotAvailable:
					return "Animation not available";
				case AnimationErrorCodes::AnimationCreationFailed:
					return "Animation creation failed";
				case AnimationErrorCodes::AnimationInvalidParameters:
					return "Animation invalid parameters";
				case AnimationErrorCodes::AnimationUpdateFailed:
					return "Animation update failed";
				default:
					return "Unknown Animation error";
			}
		}
	};

	inline const auto& GeneralErrorCategory() noexcept
	{
		static GeneralCategory instance;
		return instance;
	}
	inline const auto& SystemErrorCategory() noexcept
	{
		static SystemCategory instance;
		return instance;
	}
	inline const auto& DirectXErrorCategory() noexcept
	{
		static DirectXCategory instance;
		return instance;
	}
	inline const auto& DirectCompositionErrorCategory() noexcept
	{
		static DirectCompositionCategory instance;
		return instance;
	}
	inline const auto& Direct2DErrorCategory() noexcept
	{
		static Direct2DCategory instance;
		return instance;
	}
	inline const auto& DirectWriteErrorCategory() noexcept
	{
		static DirectWriteCategory instance;
		return instance;
	}
	inline const auto& WICErrorCategory() noexcept
	{
		static WICCategory instance;
		return instance;
	}
	inline const auto& AnimationErrorCategory() noexcept
	{
		static AnimationCategory instance;
		return instance;
	}
}

// ReSharper disable CppInconsistentNaming
export namespace std
{
	auto make_error_code(PGUI::GeneralErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::GeneralErrorCategory() };
	}
	auto make_error_code(PGUI::SystemErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::SystemErrorCategory() };
	}
	auto make_error_code(PGUI::DirectXErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::DirectXErrorCategory() };
	}
	auto make_error_code(PGUI::DirectCompositionErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::DirectCompositionErrorCategory() };
	}
	auto make_error_code(PGUI::Direct2DErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::Direct2DErrorCategory() };
	}
	auto make_error_code(PGUI::DirectWriteErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::DirectWriteErrorCategory() };
	}
	auto make_error_code(PGUI::WICErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::WICErrorCategory() };
	}
	auto make_error_code(PGUI::AnimationErrorCodes code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::AnimationErrorCategory() };
	}

	auto make_error_condition(PGUI::GeneralErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::GeneralErrorCategory() };
	}
	auto make_error_condition(PGUI::SystemErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::SystemErrorCategory() };
	}
	auto make_error_condition(PGUI::DirectXErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::DirectXErrorCategory() };
	}
	auto make_error_condition(PGUI::DirectCompositionErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::DirectCompositionErrorCategory() };
	}
	auto make_error_condition(PGUI::Direct2DErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::Direct2DErrorCategory() };
	}
	auto make_error_condition(PGUI::DirectWriteErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::DirectWriteErrorCategory() };
	}
	auto make_error_condition(PGUI::WICErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::WICErrorCategory() };
	}
	auto make_error_condition(PGUI::AnimationErrorCodes code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::AnimationErrorCategory() };
	}
}
// ReSharper restore CppInconsistentNaming