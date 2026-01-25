module;
#include <Windows.h>
#include <wil/resource.h>

export module PGUI.PropVariant;

import std;

export namespace PGUI
{
	enum class PropVariantType
	{
		Empty = 0,
		Null = 1,
		I1 = 16,
		Ui1 = 17,
		I2 = 2,
		Ui2 = 18,
		I4 = 3,
		Ui4 = 19,
		Int = 22,
		Uint = 23,
		I8 = 20,
		Ui8 = 21,
		R4 = 4,
		R8 = 5,
		Bool = 11,
		Error = 10,
		Cy = 6,
		Date = 7,
		FileTime = 64,
		Clsid = 72,
		Cf = 71,
		Bstr = 8,
		Blob = 65,
		BlobObject = 70,
		LPSTR = 30,
		LPWSTR = 31,
		Unknown = 13,
		Dispatch = 9,
		Stream = 66,
		StreamedObject = 68,
		Storage = 67,
		StoredObject = 69,
		VersionedStream = 73,
		Decimal = 14,
		Vector = 0x1000,
		Array = 0x2000,
		Byref = 0x4000,
		Variant = 12,
		TypeMask = 0xfff
	};
	DEFINE_ENUM_FLAG_OPERATORS(PropVariantType);

	using PropVariantValue =
	std::variant<
		std::nullptr_t,
		CHAR, UCHAR,
		SHORT, USHORT,
		INT, UINT,
		LARGE_INTEGER, ULARGE_INTEGER,
		FLOAT, DOUBLE, // DATE is DOUBLE
		bool, SCODE,
		CY, FILETIME,
		CLSID*, CLIPDATA*,
		BLOB, std::string, std::wstring, // BSTR is LPWSTR
		IUnknown*, IDispatch*,
		IStream*, IStorage*,
		LPVERSIONEDSTREAM,
		DECIMAL, LPSAFEARRAY,
		UCHAR*,
		SHORT*, USHORT*, INT*, UINT*,
		LARGE_INTEGER*, ULARGE_INTEGER*,
		FLOAT*, DOUBLE*,
		bool*, SCODE*,
		CY*, FILETIME*, BSTR*,
		IUnknown**, IDispatch**,
		DECIMAL*, LPSAFEARRAY*,
		PROPVARIANT*,
		std::vector<CHAR>, std::vector<UCHAR>,
		std::vector<SHORT>, std::vector<USHORT>,
		std::vector<LONG>, std::vector<ULONG>,
		std::vector<FLOAT>, std::vector<DOUBLE>,
		std::vector<CY>, std::vector<CLSID>,
		std::vector<bool>,
		std::vector<PROPVARIANT>,
		std::vector<LARGE_INTEGER>, std::vector<ULARGE_INTEGER>,
		std::vector<LPSTR>, std::vector<LPWSTR>,
		std::vector<FILETIME>, std::vector<CLIPDATA>>;

	struct PropVariant final
	{
		PropVariant() noexcept = default;

		explicit(false) PropVariant(const PROPVARIANT& otherVar) noexcept;
		explicit(false) PropVariant(PROPVARIANT&& otherVar) noexcept;

		PropVariant(const PropVariant& other);
		PropVariant(PropVariant&& other) noexcept = default;
		auto operator=(const PropVariant& other) -> PropVariant&;
		auto operator=(PropVariant&& other) noexcept -> PropVariant& = default;

		~PropVariant() noexcept = default;

		auto operator&() noexcept -> PROPVARIANT*;

		[[nodiscard]] auto GetAddress() const noexcept -> const PROPVARIANT* { return &propVariant; }
		[[nodiscard]] auto GetAddress() noexcept -> PROPVARIANT* { return propVariant.addressof(); }
		
		[[nodiscard]] auto ReleaseAndGetAddress() noexcept -> PROPVARIANT*;

		explicit(false) operator PROPVARIANT() const noexcept;

		explicit(false) operator PropVariantValue() const noexcept;

		[[nodiscard]] auto Type() const noexcept { return static_cast<PropVariantType>(propVariant.vt); }

		[[nodiscard]] auto IsEmpty() const noexcept -> bool { return Type() == PropVariantType::Empty; }

		[[nodiscard]] auto GetValue() const noexcept -> PropVariantValue;

		wil::unique_prop_variant propVariant;
	};
}
