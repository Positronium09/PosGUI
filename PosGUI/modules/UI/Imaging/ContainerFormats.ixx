module;
#include <wincodec.h>

export module PGUI.UI.Imaging.ContainerFormats;

export namespace PGUI::UI::Imaging
{
	using ContainerFormat = GUID;
	namespace ContainerFormats
	{
		const auto& Bmp = GUID_ContainerFormatBmp;
		const auto& Png = GUID_ContainerFormatPng;
		const auto& Jpeg = GUID_ContainerFormatJpeg;
		const auto& Tiff = GUID_ContainerFormatTiff;
		const auto& Gif = GUID_ContainerFormatGif;
		const auto& Wmp = GUID_ContainerFormatWmp;
	};
}
