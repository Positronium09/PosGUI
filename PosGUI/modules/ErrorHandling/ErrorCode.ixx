export module PGUI.ErrorHandling:ErrorCodes;

import std;

export namespace PGUI
{
	enum class ErrorCode : int
	{
		Success = 0,
		Failure,
		OutOfRange,
		InvalidArgument,
		NotImplemented,
		NullPointer,
		NotFound,
		InvalidCast
	};

	enum class SystemErrorCode : int
	{
		STLFailure = 100'000,
		InvalidHandle,
		AccessDenied,
		NotFound,
		InterfaceNotSupported
	};

	template <typename T>
	concept PGUIErrorCodeEnum = std::is_enum_v<T> &&
		(std::same_as<T, ErrorCode> ||
		 std::same_as<T, SystemErrorCode>);
}

// ReSharper disable CppInconsistentNaming
export namespace std
{
	template <>
	struct is_error_code_enum<PGUI::ErrorCode> : true_type { };
	template <>
	struct is_error_code_enum<PGUI::SystemErrorCode> : true_type { };

	template <>
	struct is_error_condition_enum<PGUI::ErrorCode> : true_type { };
	template <>
	struct is_error_condition_enum<PGUI::SystemErrorCode> : true_type { };
}
// ReSharper restore CppInconsistentNaming

export namespace PGUI
{
	class ErrorCategory : public std::error_category
	{
		[[nodiscard]] auto name() const noexcept -> const char* override
		{
			return "PGUI Error Category";
		}
		[[nodiscard]] auto message(int ev) const -> std::string override
		{
			switch (static_cast<ErrorCode>(ev))
			{
				case ErrorCode::Success:
					return "Success";
				case ErrorCode::Failure:
					return "Failure";
				case ErrorCode::OutOfRange:
					return "Out of range";
				case ErrorCode::InvalidArgument:
					return "Invalid argument";
				case ErrorCode::NotImplemented:
					return "Not implemented";
				case ErrorCode::NullPointer:
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
			switch (static_cast<SystemErrorCode>(ev))
			{
				case SystemErrorCode::STLFailure:
					return "STL operation failure";
				case SystemErrorCode::InvalidHandle:
					return "Invalid handle";
				case SystemErrorCode::AccessDenied:
					return "Access denied";
				case SystemErrorCode::NotFound:
					return "Not found";
				case SystemErrorCode::InterfaceNotSupported:
					return "Interface not supported";
				default:
					return "Unknown system error";
			}
		}
	};

	inline const auto& GetErrorCategory() noexcept
	{
		static ErrorCategory instance;
		return instance;
	}
	inline const auto& GetSystemErrorCategory() noexcept
	{
		static SystemCategory instance;
		return instance;
	}
}

// ReSharper disable CppInconsistentNaming
export namespace std
{
	auto make_error_code(PGUI::ErrorCode code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::GetErrorCategory() };
	}
	auto make_error_code(PGUI::SystemErrorCode code) -> std::error_code
	{
		return { static_cast<int>(code), PGUI::GetSystemErrorCategory() };
	}
	auto make_error_condition(PGUI::ErrorCode code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::GetErrorCategory() };
	}
	auto make_error_condition(PGUI::SystemErrorCode code) -> std::error_condition
	{
		return { static_cast<int>(code), PGUI::GetSystemErrorCategory() };
	}
}
// ReSharper restore CppInconsistentNaming