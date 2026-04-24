module;
#include <Windows.h>

export module PGUI.UI.DragDrop.DragDropEnums;

export namespace PGUI::UI::DragDrop
{
	enum class ClipboardFormat : CLIPFORMAT
	{
		Text = CF_TEXT,
		Bitmap = CF_BITMAP,
		MetaFilePict = CF_METAFILEPICT,
		Sylk = CF_SYLK,
		Dif = CF_DIF,
		Tiff = CF_TIFF,
		OemText = CF_OEMTEXT,
		Dib = CF_DIB,
		Palette = CF_PALETTE,
		PenData = CF_PENDATA,
		Riff = CF_RIFF,
		Wave = CF_WAVE,
		UnicodeText = CF_UNICODETEXT,
		EnhMetaFile = CF_ENHMETAFILE,
		HDrop = CF_HDROP,
		Locale = CF_LOCALE,
		DibV5 = CF_DIBV5,
		OwnerDisplay = CF_OWNERDISPLAY,
		DspText = CF_DSPTEXT,
		DspBitmap = CF_DSPBITMAP,
		DspMetaFilePict = CF_DSPMETAFILEPICT,
		DspEnhMetaFile = CF_DSPENHMETAFILE,
		PrivateFirst = CF_PRIVATEFIRST,
		PrivateLast = CF_PRIVATELAST,
		GdiObjFirst = CF_GDIOBJFIRST,
		GdiObjLast = CF_GDIOBJLAST
	};

	// ReSharper disable once CppInconsistentNaming

	enum class DVAspect : DWORD
	{
		Content = DVASPECT_CONTENT,
		Thumbnail = DVASPECT_THUMBNAIL,
		Icon = DVASPECT_ICON,
		DocPrint = DVASPECT_DOCPRINT
	};

	enum class StorageMediumType : DWORD
	{
		HGlobalMemory = TYMED_HGLOBAL,
		File = TYMED_FILE,
		IStream = TYMED_ISTREAM,
		IStorage = TYMED_ISTORAGE,
		GDI = TYMED_GDI,
		MetaFilePict = TYMED_MFPICT,
		EnhancedMetaFile = TYMED_ENHMF,
		Null = TYMED_NULL
	};
	DEFINE_ENUM_FLAG_OPERATORS(StorageMediumType);

	enum class DataDirection
	{
		Get = DATADIR_GET,
		Set = DATADIR_SET
	};
}
