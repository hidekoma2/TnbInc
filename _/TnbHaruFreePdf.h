#pragma once
/**
 *@file
 *  Haru Free PDF Library関係のヘッダ
 *
 *		<a class="el" href="http://libharu.sourceforge.net"> Haru Free PDF Library Ⅱ</a> を使用します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



#ifndef _HPDF_H
	#include "other/hpdf.h"
	#pragma comment(lib,"libhpdf.lib")
#endif



//TNB Library
namespace TNB
{

	
#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// PointerHandleBase用 HPDF_Doc型ハンドルDestoryクラス
struct TPhFreeHpdf_Doc
{
	void operator()(HPDF_Doc P) { ::HPDF_Free(P); }
};

/**@ingroup PDF
 * HPDF_Doc型ハンドルテンプレート
 *
 *		この型に HPDF_Free() で破棄するハンドルを渡しておくと、
 *		「領域」を参照する本クラスががなくなり次第、
 *		自動的に Free します。
 */
typedef TNB::CPointerHandleBaseT<HPDF_Doc, TPhFreeHpdf_Doc, NULL> CHpdfDocHandle;

#endif //_TnbDOXYGEN
	



/**@ingroup PDF
 * HPDF_Page管理
 */
class CHpdfPage
{
	HPDF_Page m_page;	///< HPDF_Pageハンドル

public:

	/**@ingroup PDF
	 * HPDF_Pageの値
	 */
	class CValue
	{
		HPDF_REAL m_val;	///< 値
	public:
		/// コンストラクタ
		CValue(double f) : m_val(static_cast<HPDF_REAL>(f)){}
		/// オペレータ
		operator HPDF_REAL(void){return m_val;}
		/// オペレータ
		operator int(void){return m_val;}
	};
	
	/// コンストラクタ
	CHpdfPage(HPDF_Page p) : m_page(p)
	{
	}

	/// オペレータ
	operator HPDF_Page(void)
	{
		return m_page;
	}


	/*
	 *	@retval NULL以外 Pageのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() に通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_PAGE	An invalid page handle was set.
*/

	/**
	 * [設定] changes the width of a page.	
	 *	@param value	Specify the new width of a page. The valid value is between 3 and 14400.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_PAGE	An invalid page handle was set.
	 *	@retval HPDF_PAGE_INVALID_SIZE	An invalid size was set.
	 *	@retval	HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetWidth(CValue value)
	{
		return ::HPDF_Page_SetWidth(m_page, value);
	}

	/**
	 * [設定] changes the height of a page. 
	 *	@param value	Specify the new height of a page. The valid value is between 3 and 14400.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_PAGE	An invalid page handle was set.
	 *	@retval HPDF_PAGE_INVALID_SIZE	An invalid size was set.
	 *	@retval	HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetHeight(CValue value)
	{
		return ::HPDF_Page_SetHeight(m_page, value);
	}

	/**
	 * [設定] changes the size and direction of a page to a predefined size.
	 *	@param size	Specify a predefined page-size value.
	 *	@param direction Specify the direction of the page.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_PAGE		An invalid page handle was set.
	 *	@retval HPDF_PAGE_INVALID_SIZE	An invalid size was set.
	 *	@retval HPDF_PAGE_INVALID_DIRECTION	An invalid direction was set.
	 *	@retval	HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetSize(HPDF_PageSizes size, HPDF_PageDirection direction)
	{
		return ::HPDF_Page_SetSize(m_page, size, direction);
	}

	/**
	 * [設定] sets rotation angle of the page. 
	 *	@param angle Specify the rotation angle of the page. It must be a multiple of 90 Degrees.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_PAGE				An invalid page handle was set.
	 *	@retval PDF_PAGE_INVALID_ROTATE_VALUE	An invalid rotation angle was set.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetRotate(BYTE angle)
	{
		return ::HPDF_Page_SetRotate(m_page, angle);
	}

	/**
	 * [取得] gets the height of a page. 
	 *	@retval 0 エラー。
	 *	@retval 0以外 succeed, it returns the height of a page.
	 */
	CValue GetWidth(void) const
	{
		return ::HPDF_Page_GetWidth(m_page);
	}

	/**
	 * [取得] gets the height of a page. 
	 *	@retval 0 エラー。
	 *	@retval 0以外 succeed, it returns the height of a page.
	 */
	CValue GetHeight(void) const
	{
		return::HPDF_Page_GetHeight(m_page);
	}

	/**
	 * [作成] creates a new destination object for the page. 
	 *	@retval NULL以外 Destinationのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_PAGE	An invalid page handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 */
	HPDF_Destination CreateDestination(void) const
	{
		return ::HPDF_Page_CreateDestination(m_page);
	}

	/**
	 * [作成] creates a new text annotation object for the page. 
	 *	@param rect A Rectangle where the annotation is displayed
	 *	@param text The text to be displayed.
	 *	@param encoder An encoder handle which is used to encode the text. If it is null, PDFDocEncoding is used.
	 *	@retval NULL以外 Annotationのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_PAGE		An invalid page handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_ENCODER	An invalid encoder handle is specified..
	 */
	HPDF_Annotation CreateTextAnnot(const HPDF_Rect& rect, LPCSTR text, HPDF_Encoder encoder) const
	{
		return ::HPDF_Page_CreateTextAnnot(m_page, rect, text, encoder);
	}

	/**
	 * [作成] creates a new link annotation object for the page. 
	 *	@param rect		A rectangle of clickable area.
	 *	@param dst		A handle of destination object to jump to.
	 *	@retval NULL以外 Annotationのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_PAGE		An invalid page handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_DESTINATION	An invalid destination handle is specified..
	 */
	HPDF_Annotation CreateLinkAnnot(const HPDF_Rect& rect, HPDF_Destination dst) const
	{
		return ::HPDF_Page_CreateLinkAnnot(m_page, rect, dst);
	}

	/**
	 * [作成] creates a new web link annotation object for the page. 
	 *	@param rect		A rectangle of clickable area.
	 *	@param uri		URL of destination to jump to.
	 *	@retval NULL以外 Annotationのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_PAGE		An invalid page handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *
	 */
	HPDF_Annotation CreateUriLinkAnnot(const HPDF_Rect& rect, LPCSTR uri) const
	{
		return ::HPDF_Page_CreateURILinkAnnot(m_page, rect, uri);
	}

	/**
	 * [確認] gets the width of the text in current fontsize, character spacing and word spacing.
	 *	@param text		The text to get width. 
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval 0以外 succeed, it returns the width of the text in current fontsize, character spacing and word spacing.
	 */
	CValue TextWidth(LPCSTR text) const
	{
		return ::HPDF_Page_TextWidth(m_page, text);
	}

	/**
	 * [計算] calculates the byte length which can be included within the specified width.
	 *	@param text		The text to get width.
	 *	@param width	The width of the area to put the text.
	 *	@param isWordWrap
	 *		When there are three words of "ABCDE", "FGH", and "IJKL", and the substring until "J" can be included within the width, 
	 *		if isWordWrap parameter is false it returns 12,  
	 *		and if isWordWrap parameter is false isWordWrap parameter is false it returns 10 (the end of the previous word).
	 *	@param realWidth If this parameter is not NULL, the real widths of the text is set. 
	 *						An application can set it to NULL if it is unnecessary
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval 0以外 succeed, it returns the byte length which can be included within the specified width in current fontsize, character spacing and word spacing.
	 */
	size_t MeasureText(LPCSTR text, CValue width, bool isWordWrap, CValue* realWidth = NULL)
	{
		HPDF_REAL r;
		size_t s = ::HPDF_Page_MeasureText(m_page, text, width, isWordWrap, &r);
		if ( realWidth != NULL )
		{
			*realWidth = CValue(r);
		}
		return s;
	}

	/**
	 * [取得] gets the current graphics mode.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval 0以外 succeed, it returns the current graphics mode of the page.  Otherwise it returns ZERO. 
	*/
	DWORD GetGraphicsMode(void) const
	{
		return ::HPDF_Page_GetGMode(m_page);
	}

	/**
	 * [取得] gets the current position for path painting.
	 *	@note An application can invoke GetCurrentPos() only when graphics mode is HPDF_GMODE_PATH_OBJECT.
	 *	@retval {0,0} エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval {0,0}以外 succeed, it returns a HPDF_Point struct indicating the current position for path painting of the page.
	*/
	HPDF_Point GetCurrentPos(void) const
	{
		return ::HPDF_Page_GetCurrentPos(m_page);
	}

	/**
	 * [取得] gets the current position for text showing.
	 *	@note An application can invoke GetCurrentTextPos() only when graphics mode is HPDF_GMODE_TEXT_OBJECT.
	 *	@retval {0,0} エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval {0,0}以外 succeed, it returns a HPDF_Point struct indicating the current position for text showing of the page.
	 */
	HPDF_Point GetCurrentTextPos(void) const
	{
		return ::HPDF_Page_GetCurrentTextPos(m_page);
	}

	/**
	 * [取得] gets the handle of the page's current font.
	 *	@retval NULL以外 Font のハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_Font GetCurrentFont(void) const
	{
		return ::HPDF_Page_GetCurrentFont(m_page);
	}

	/**
	 * [取得] gets the size of the page's current font.
	 *	@retval 0以外 succeed, it returns the size of the page's current font. 
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetCurrentFontSize(void) const
	{
		return ::HPDF_Page_GetCurrentFontSize(m_page);
	}

	/**
	 * [取得] gets the current transformation matrix of the page.
	 *	@retval NULL以外 TransMatrix のハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *
	 */
	HPDF_TransMatrix GetTransMatrix(void) const
	{
		return ::HPDF_Page_GetTransMatrix(m_page);
	}

	/**
	 * [取得] gets the current line width of the page.
	 *	@retval HPDF_DEF_LINEWIDTH以外 succeed, it returns the current line width for path painting of the page.
	 *	@retval HPDF_DEF_LINEWIDTH エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetLineWidth(void) const
	{
		return ::HPDF_Page_GetLineWidth(m_page);
	}

	/**
	 * [取得] gets the current line cap style of the page.
	 *	@retval HPDF_DEF_LINECAP以外 succeed, it returns the current line cap style of the page.
	 *	@retval HPDF_DEF_LINECAP エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_LineCap GetLineCap(void) const
	{
		return ::HPDF_Page_GetLineCap(m_page);
	}

	/**
	 * [取得] gets the current line join style of the page.
	 *	@retval HPDF_DEF_LINEJOIN以外 succeed, it returns the current line join style of the page.
	 *	@retval HPDF_DEF_LINEJOIN エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_LineJoin GetLineJoin(void) const
	{
		return ::HPDF_Page_GetLineJoin(m_page);
	}

	/**
	 * [取得] gets the current value of the page's miter limit.
	 *	@retval HPDF_DEF_MITERLIMIT以外 succeed, it returns the current value of the page's miter limit.
	 *	@retval HPDF_DEF_MITERLIMIT エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetMiterLimit(void) const
	{
		return ::HPDF_Page_GetMiterLimit(m_page);
	}

	/**
	 * [取得] gets the current pattern of the page.
	 *	@return it returns a HPDF_DashMode struct of the current pattern of the page.
	*/
	HPDF_DashMode GetDash(void) const
	{
		return ::HPDF_Page_GetDash(m_page);
	}

	/**
	 * [取得] gets the current value of the page's flatness.
	 *	@retval HPDF_DEF_MITERLIMIT以外 succeed, it returns the current value of the page's miter limit.
	 *	@retval HPDF_DEF_MITERLIMIT エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetFlat(void) const
	{
		return ::HPDF_Page_GetFlat(m_page);
	}

	/**
	 * [取得] gets the current value of the page's character spacing.
	 *	@retval 0以外 succeed, it returns the current value of the page's character spacing.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetCharSpace(void) const
	{
		return ::HPDF_Page_GetCharSpace(m_page);
	}

	/**
	 * [取得] returns the current value of the page's word spacing.
	 *	@retval 0以外 succeed, it returns the current value of the page's word spacing.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetWordSpace(void) const
	{
		return ::HPDF_Page_GetWordSpace(m_page);
	}

	/**
	 * [取得] returns the current value of the page's horizontal scalling for text showing.
	 *	@retval HPDF_DEF_HSCALING以外 succeed, it returns the current value of the page's horizontal scalling.
	 *	@retval HPDF_DEF_HSCALING エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetHorizontalScalling(void) const
	{
		return ::HPDF_Page_GetHorizontalScalling(m_page);
	}

	/**
	 * [取得] returns the current value of the page's line spacing.
	 *	@retval 0以外 succeed, it returns the current value of the line spacing.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	*/
	CValue GetTextLeading(void) const
	{
		return ::HPDF_Page_GetTextLeading(m_page);
	}

	/**
	 * [取得] returns the current value of the page's text rendering mode.
	 *	@retval 0以外 succeed, it returns the current value of the text rendering mode.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetTextRenderingMode(void) const
	{
		return ::HPDF_Page_GetTextRenderingMode(m_page);
	}

	/**
	 * [取得] returns the current value of the page's text rising.
	 *	@retval 0以外 succeed, it returns the current value of the text rising.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CValue GetTextRise(void) const
	{
		return ::HPDF_Page_GetTextRise(m_page);
	}

	/**
	 * [取得] returns the current value of the page's filling color. 
	 *		GetRgbFill() is valid only when the page's filling color space is HPDF_CS_DEVICE_RGB.
	 *	@retval {0,0,0} エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval {0,0,0}以外 succeed, it returns the current value of the page's filling color.
	 */
	HPDF_RGBColor GetRgbFill(void) const
	{
		return ::HPDF_Page_GetRGBFill(m_page);
	}

	/**
	 * [取得] returns the current value of the page's stroking color.
	 *		GetRgbStroke() is valid only when the page's stroking color space is HPDF_CS_DEVICE_RGB.
	 *	@retval {0,0,0} エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval {0,0,0}以外 succeed, it returns the current value of the page's stroking color.
	 */
	HPDF_RGBColor GetRgbStroke(void) const
	{
		return ::HPDF_Page_GetRGBStroke(m_page);
	}

	/**
	 * [取得] returns the current value of the page's filling color.
	 *		GetCmykFill() is valid only when the page's filling color space is HPDF_CS_DEVICE_CMYK.
	 *	@retval {0,0,0,0} エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval {0,0,0,0}以外 succeed, it returns the current value of the page's filling color.
	 */
	HPDF_CMYKColor GetCmykFill(void) const
	{
		return ::HPDF_Page_GetCMYKFill(m_page);
	}

	/**
	 * [取得] returns the current value of the page's stroking color.
	 *		GetCmykStroke() is valid only when the page's stroking color space is HPDF_CS_DEVICE_CMYK.
	 *	@retval {0,0,0,0} エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval {0,0,0,0}以外 succeed, it returns the current value of the page's stroking color. 
	 */
	HPDF_CMYKColor GetCmykStroke(void) const
	{
		return ::HPDF_Page_GetCMYKStroke(m_page);
	}

	/**
	 * [取得] returns the current value of the page's filling color.
	 *		GetGrayFill() is valid only when the page's stroking color space is HPDF_CS_DEVICE_GRAY.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval 0以外 succeed, it returns the current value of the page's filling color.
	 */
	CValue GetGrayFill(void) const
	{
		return ::HPDF_Page_GetGrayFill(m_page);
	}

	/**
	 * [取得] returns the current value of the page's stroking color.
	 *		GetGrayStroke() is valid only when the page's stroking color space is HPDF_CS_DEVICE_GRAY.
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval 0以外 succeed, it returns the current value of the page's stroking color.
	 */
	CValue GetGrayStroke(void) const
	{
		return ::HPDF_Page_GetGrayStroke(m_page);
	}

	/**
	 * [取得] returns the current value of the page's stroking color space. 
	 *	@retval HPDF_CS_EOF エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval HPDF_CS_EOF以外 succeed, it returns the current value of the page's stroking color space.
	 */
	HPDF_ColorSpace GetStrokingColorSpace(void) const
	{
		return ::HPDF_Page_GetStrokingColorSpace(m_page);
	}

	/**
	 * [取得] returns the current value of the page's stroking color space. 
	 *	@retval HPDF_CS_EOF エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval HPDF_CS_EOF以外 succeed, it returns the current value of the page's stroking color space.
	 */
	HPDF_ColorSpace GetFillingColorSpace(void) const
	{
		return ::HPDF_Page_GetFillingColorSpace(m_page);
	}

	/**
	 * [取得] gets the current text transformation matrix of the page.
	 *	@retval NULL以外 TransMatrix のハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_TransMatrix GetTextMatrix(void) const
	{
		return ::HPDF_Page_GetTextMatrix(m_page);
	}

	/**
	 * [取得] returns the number of the page's graphics state stack. 
	 *	@retval 0 エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *	@retval 0以外 succeed, it returns the number of the page's graphics state stack.
	 */
	UINT GetGraphicsStateDepth(void) const
	{
		return ::HPDF_Page_GetGStateDepth(m_page);
	}

	/**
	 * [設定] configures the setting for slide transition of the page.
	 *	@param type	The transition style.
	 *	@param dispTime		The display duration of the page. (in seconds)
	 *	@param transTime	The duration of the transition effect. Default value is 1(second).
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetSlideShow(HPDF_TransitionStyle type, CValue dispTime, CValue transTime)
	{
		return ::HPDF_Page_SetSlideShow(m_page, type, dispTime, transTime);
	}

	/**
	 * [設定] sets the width of the line used to stroke a path.
	 *	@note An application can invoke SetLineWidth() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.
	 *	@param lineWidth The width of line.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetLineWidth(CValue lineWidth)
	{
		return ::HPDF_Page_SetLineWidth(m_page, lineWidth);
	}

	/**
	 * [設定] sets the shape to be used at the ends of line.
	 *	@note An application can invoke SetLineCap() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.
	 *	@param lineCap The style of line-cap.	
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetLineCap(HPDF_LineCap lineCap)
	{
		return ::HPDF_Page_SetLineCap(m_page, lineCap);
	}

	/**
	 * [設定] Sets the line join style in the page.
	 *	@note An application can invoke SetLineJoin() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.
	 *	@param lineJoin The style of line-join.  HPDF_MITER_JOIN / HPDF_ROUND_JOIN / HPDF_BEVEL_JOIN	
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetLineJoin(HPDF_LineJoin lineJoin)
	{
		return ::HPDF_Page_SetLineJoin(m_page, lineJoin);
	}

	/**
	 * [設定] Sets the miter limit in the page.
	 *	@param miterLimit miter limit.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetMiterLimit(CValue miterLimit)
	{
		return ::HPDF_Page_SetMiterLimit(m_page, miterLimit);
	}

	/**
	 * [設定] Sets the line dash pattern in the page.
	 *	@note An application can invoke Page_SetDash() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.
	 *	@note	Sample of the dash pattern.
	 *			\n		dash_ptn=NULL, num_pattern = 0, phase=0
	 *			\n		dash_ptn=[3], num_pattern = 1, phase=1
	 *			\n		dash_ptn=[7, 3], num_pattern = 2, phase=2
	 *			\n		dash_ptn=[8, 7, 2, 7], num_pattern = 4, phase=0
	 *	@param dash_ptn An array of HPDF_UINT16 which specify pattern of dashes and gaps used to stroke paths.
	 *	@param num_param The number of the element of the dash_pattern. Admitted ranges are from 0 to 8. 
	 *	@param phase The phase in which the pattern begins. 
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetDash(const HPDF_UINT16* dash_ptn, HPDF_UINT num_param, HPDF_UINT phase)
	{
		return ::HPDF_Page_SetDash(m_page, dash_ptn, num_param, phase);
	}

	/**
	 * [設定] Sets the flatness in the page.
	 *	@param flatness flatness
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetFlat(CValue flatness)
	{
		return ::HPDF_Page_SetFlat(m_page, flatness);
	}

	/**
	 * [設定] applys the graphics state to the page.
	 *	@note An application can invoke GraphicsSave() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION.
	 *	@param ext_gstate	The handle of a extended graphics state object.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetExtGraphicsState(HPDF_ExtGState ext_gstate)
	{
		return ::HPDF_Page_SetExtGState(m_page, ext_gstate);
	}
	
	/**
	 * [処理] saves the page's current graphics parameter to the stack.
	 *	@note An application can invoke GraphicsSave() up to 28 and can restore the saved parameter by invoking GraphicsRestore().
	 *	@note The parameters that are saved by GraphicsSave() is as follows.
	 *			\n	- Transformation Matrix.
	 *			\n	- Line Width.
	 *			\n	- Line Cap Style.
	 *			\n	- Line Join Style.
	 *			\n	- Miter Limit.
	 *			\n	- Dash Mode.
	 *			\n	- Flatness.
	 *			\n	- Character Spacing.
	 *			\n	- Word Spacing.
	 *			\n	- Horizontal Scalling.
	 *			\n	- Text Leading.
	 *			\n	- Rendering Mode.
	 *			\n	- Text Rise.
	 *			\n	- Filling Color.
	 *			\n	- Stroking Color.
	 *			\n	- Font.
	 *			\n	- Font Size.
	 *	@note	An application can invoke GraphicsSave() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS GraphicsSave(void)
	{
		return ::HPDF_Page_GSave(m_page);
	}

	/**
	 * [処理] restore the graphics state which is saved by GraphicsSave().
	 *	@note An application can invoke GraphicsSave() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS GraphicsRestore(void)
	{
		return ::HPDF_Page_GRestore(m_page);
	}

	/**
	 * [処理] concatenates the page's current transformation matrix and specified matrix.
	 *	@par For example
	 *		if you want to rotate the coordinate system of the page by 45 degrees, use Concat() as follows.
	 *			<table><tr><td><pre>
	 *			  float rad1 = 45 / 180 * 3.141592;
	 *			  page.Concat(cos(rad1), sin(rad1), -sin(rad1), cos(rad1), 220, 350); 
	 *			</pre></td></tr></table>
	 *	@par
	 *		Additionally to change the coordinate system of the page to 300dpi, use Concat() as follows.
	 *			<table><tr><td><pre>
	 *			  page.Concat(72.0f / 300.0f, 0, 0, 72.0f / 300.0f, 0, 0); 
	 *			</pre></td></tr></table>
	 *	@par
	 *		Invoke GraphicsSave() before Concat(). Then the change by Concat() can be restored by invoking GraphicsRestore().
	 *			<table><tr><td><pre>
	 *			  // save the current graphics states
	 *			  page.GraphicsSave();
	 *			  // concatenate the transformation matrix
	 *			  page.Concat(72.0f / 300.0f, 0, 0, 72.0f / 300.0f, 0, 0);
	 *			  // show text on the translated coordinates
	 *			  page.BeginText();
	 *			  page.MoveTextPos(50, 100);
	 *			  page.ShoeText("Text on the translated coordinates");
	 *			  page.EndText();
	 *			  // restore the graphics states
	 *			  page.GraphicsRestore();
	 *			</pre></td></tr></table>
	 *	@note An application can invoke GraphicsSave() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION.
	 *	@param a	The transformation matrix to concatenate.
	 *	@param b	The transformation matrix to concatenate.
	 *	@param c	The transformation matrix to concatenate.
	 *	@param d	The transformation matrix to concatenate.
	 *	@param x	The transformation matrix to concatenate.
	 *	@param y	The transformation matrix to concatenate.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS Concat(CValue a, CValue b, CValue c, CValue d, CValue x, CValue y)
	{
		return ::HPDF_Page_Concat(m_page, a, b, c, d, x, y);
	}

	/**
	 * [処理] starts a new subpath and move the current point for drawing path,  
	 *	@note MoveTo() sets the start point for the path to the point (x, y) and changes the graphics mode to HPDF_GMODE_PATH_OBJECT.
	 *		An application can invoke MoveTo() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_PATH_OBJECT.
	 *	@param x	The start point for drawing path
	 *	@param y	The start point for drawing path
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS MoveTo(CValue x, CValue y)
	{
		return ::HPDF_Page_MoveTo(m_page, x, y);
	}

	/**
	 * [処理] appends a path from the current point to the specified point.
	 *	@note An application can invoke LineTo() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT.
	 *	@param x	The end point of the path
	 *	@param y	The end point of the path
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS LineTo(CValue x, CValue  y)
	{
		return ::HPDF_Page_LineTo(m_page, x, y);
	}

	/*
	Description
	HPDF_Page_CurveTo() appends a Bezier curve to the current path using two spesified points.
	The point (x1, y1) and the point (x2, y2) are used as the control points for a Bezier curve and current point is moved to the point (x3, y3)
	An application can invoke HPDF_Page_CurveTo() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT.


	Parameter
	page 
	The handle of a page object.

	x1, y1, x2, y2, x3, y3
	The control points for a Bezier curve.

	Returns
	When HPDF_Page_CurveTo() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/

	HPDF_STATUS CurveTo(CValue x1, CValue y1, CValue x2, CValue y2, CValue x3, CValue y3)
	{
		return ::HPDF_Page_CurveTo(m_page, x1, y1, x2, y2, x3, y3);
	}

	/*
	Description
	HPDF_Page_CurveTo2() appends a Bezier curve to the current path using two spesified points.
	The current point and the point (x2, y2) are used as the control points for a Bezier curve and current point is moved to the point (x3, y3)
	An application can invoke HPDF_Page_CurveTo3() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT.



	Parameter
	page 
	The handle of a page object.

	x2, y2, x3, y3
	The control points for a Bezier curve.

	Returns
	When HPDF_Page_CurveTo3() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS CurveTo2(CValue x2, CValue y2, CValue x3, CValue y3)
	{
		return ::HPDF_Page_CurveTo2(m_page, x2, y2, x3, y3);
	}

	/*
	Description
	HPDF_Page_CurveTo3() appends a Bezier curve to the current path using two spesified points.
	The point (x1, y1) and the point (x3, y3) are used as the control points for a Bezier curve and current point is moved to the point (x3, y3)
	An application can invoke HPDF_Page_CurveTo2() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT.



	Parameter
	page 
	The handle of a page object.

	x1, y1, x3, y3
	The control points for a Bezier curve.

	Returns
	When HPDF_Page_CurveTo3() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS CurveTo3(CValue x1, CValue y1, CValue x3, CValue y3)
	{
		return ::HPDF_Page_CurveTo3(m_page, x1, y1, x3, y3);
	}

	/*
	Description
	HPDF_Page_ClosePath() appends a strait line from the current point to the start point of sub path.
	The current point is moved to the start point of sub path. 
	An application can invoke HPDF_Page_CurveTo2() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_ClosePath() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ClosePath(void)
	{
		return ::HPDF_Page_ClosePath(m_page);
	}

	/*
	Description
	HPDF_Page_Rectangle() appends a rectangle to the current path.
	An application can invoke HPDF_Page_Rectangle() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_PATH_OBJECT.

	Parameter
	page 
	The handle of a page object.

	x, y
	The lower-left point of the rectangle.

	width
	The width of the rectangle.

	height
	The height of the rectangle.

	Returns
	When HPDF_Page_Rectangle() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS Rectangle(CValue x, CValue y, CValue width, CValue height)
	{
		return ::HPDF_Page_Rectangle(m_page, x, y, width, height);
	}

	/*
	Description
	HPDF_Page_Stroke() paints the current path.
	An application can invoke HPDF_Page_Stroke() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_Stroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS Stroke(void)
	{
		return ::HPDF_Page_Stroke(m_page);
	}

	/*
	Description
	HPDF_Page_ClosePathStroke() closes the current path, then it paints the path.
	An application can invoke HPDF_Page_ClosePathStroke() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_ClosePathStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ClosePathStroke(void)
	{
		return ::HPDF_Page_ClosePathStroke(m_page);
	}

	/*
	Description
	HPDF_Page_Fill() fills the current path using the nonzero winding number rule.
	An application can invoke HPDF_Page_Fill() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_Fill() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS Fill(void)
	{
		return ::HPDF_Page_Fill(m_page);
	}

	/*
	Description
	HPDF_Page_Eofill() fills the current path using the even-odd rule.
	An application can invoke HPDF_Page_Eofill() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_Eofill() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS Eofill(void)
	{
		return ::HPDF_Page_Eofill(m_page);
	}

	/*
	Description
	HPDF_Page_FillStroke() fills the current path using the nonzero winding number rule, then it paints the path. 
	An application can invoke HPDF_Page_FillStroke() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_FillStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS FillStroke(void)
	{
		return ::HPDF_Page_FillStroke(m_page);
	}

	/*
	Description
	HPDF_Page_EofillStroke() fills the current path using the even-odd rule, then it paints the path. 
	An application can invoke HPDF_Page_EofillStroke() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_EofillStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS EofillStroke(void)
	{
		return ::HPDF_Page_EofillStroke(m_page);
	}

	/*
	Description
	HPDF_Page_ClosePathFillStroke() closes the current path, fills the current path using the nonzero winding number rule, then it paints the path. 
	An application can invoke HPDF_Page_ClosePathFillStroke() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_ClosePathFillStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ClosePathFillStroke(void)
	{
		return ::HPDF_Page_ClosePathFillStroke(m_page);
	}

	/*
	Description
	HPDF_Page_ClosePathEofillStroke() closes the current path, fills the current path using the even-odd rule, then it paints the path. 
	An application can invoke HPDF_Page_ClosePathEofillStroke() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_ClosePathEofillStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ClosePathEoillStroke(void)
	{
		return ::HPDF_Page_ClosePathEofillStroke(m_page);
	}

	/*
	Description
	HPDF_Page_EndPath() ends the path object without filling and painting operation.
	An application can invoke HPDF_Page_EndPath() when the graphics mode of the page is in HPDF_GMODE_PATH_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_EndPath() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS EndPath(void)
	{
		return ::HPDF_Page_EndPath(m_page);
	}

	/*
	*/
	HPDF_STATUS Clip(void)
	{
		return ::HPDF_Page_Clip(m_page);
	}

	/*
	*/
	HPDF_STATUS Eoclip(void)
	{
		return ::HPDF_Page_Eoclip(m_page);
	}

	/*
	Description
	HPDF_Page_BeginText() begins a text object and sets the current text position to the point (0, 0).
	An application can invoke HPDF_Page_BeginText() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION. And it changes the graphics mode to HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_BeginText() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS BeginText(void)
	{
		return ::HPDF_Page_BeginText(m_page);
	}

	/*
	Description
	HPDF_Page_EndText() ends a text object.
	An application can invoke HPDF_Page_EndText() when the graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT. And it changes the graphics mode to HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	Returns
	When HPDF_Page_EndText() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS EndText(void)
	{
		return ::HPDF_Page_EndText(m_page);
	}

	/*
	Description
	HPDF_Page_SetCharSpace() sets the character spacing for text showing.
	The initial value of character spacing is 0.
	An application can invoke HPDF_Page_SetCharSpace() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	value
	The value of character spacing.

	Returns
	When HPDF_Page_SetCharSpace() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetCharSpace(CValue value)
	{
		return ::HPDF_Page_SetCharSpace(m_page, value);
	}

	/*
	Description
	HPDF_Page_SetWordSpace() sets the word spacing for text showing.
	The initial value of word spacing is 0.
	An application can invoke HPDF_Page_SetWordSpace() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	value
	The value of word spacing.

	Returns
	When HPDF_Page_SetWordSpace() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetWordSpace(CValue value)
	{
		return ::HPDF_Page_SetWordSpace(m_page, value);
	}

	/*
	Description
	HPDF_Page_SetHorizontalScalling() sets the horizontal scalling for text showing.
	The initial value of horizontal scalling is 100.
	An application can invoke HPDF_Page_SetHorizontalScalling() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	value 
	The value of horizontal scalling.

	Returns
	When HPDF_Page_SetHorizontalScalling() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetHorizontalScalling(CValue  value)
	{
		return ::HPDF_Page_SetHorizontalScalling(m_page, value);
	}

	/*
	Description
	HPDF_Page_SetTextLeading() sets the text leading (line spacing) for text showing.
	The initial value of leading is 0.
	An application can invoke HPDF_Page_SetTextLeading() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	value 
	The value of text leading.

	Returns
	When HPDF_Page_SetTextLeading() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetTextLeading(CValue value)
	{
		return ::HPDF_Page_SetTextLeading(m_page, value);
	}

	/*
	Description
	HPDF_Page_SetFontAndSize() sets the type of font and size leading.
	An application can invoke HPDF_Page_SetFontAndSize() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	font 
	The handle of a font object.

	size
	The size of a font.

	Returns
	When HPDF_Page_SetFontAndSize() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetFontAndSize(HPDF_Font font, CValue size)
	{
		return ::HPDF_Page_SetFontAndSize(m_page, font, size);
	}

	/*
	Description
	HPDF_Page_SetTextRenderingMode() sets the text rendering mode.
	The initial value of text rendering mode is HPDF_FILL.
	An application can invoke HPDF_Page_SetTextRenderingMode() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	mode 
	The text rendering mode (one of the following values)VALUE	SAMPLE
	HPDF_FILL	
	HPDF_STROKE	
	HPDF_FILL_THEN_STROKE	
	HPDF_INVISIBLE	


	HPDF_FILL_CLIPPING	
	HPDF_STROKE_CLIPPING	
	HPDF_FILL_STROKE_CLIPPING	
	HPDF_CLIPPING	





	Returns
	When HPDF_Page_SetTextRenderingMode() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetTextRenderingMode(HPDF_TextRenderingMode  mode)
	{
		return ::HPDF_Page_SetTextRenderingMode(m_page, mode);
	}

	/*
	*/
	HPDF_STATUS SetTextRise(CValue value)
	{
		return ::HPDF_Page_SetTextRise(m_page, value);
	}

	/*
	Description
	HPDF_Page_MoveTextPos() moves the current text position to the start of the next line with using specified offset values. If the start position of the current line is (x1, y1), the start of the next line is (x1 + x, y1 + y). 
	An application can invoke HPDF_MoveTextPos() when the graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	xpos, ypos
	The offset of the start of the next line.

	text
	The text to show.

	Returns
	When HPDF_Page_MoveTextPos() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS MoveTextPos(CValue x, CValue y)
	{
		return ::HPDF_Page_MoveTextPos(m_page, x, y);
	}

	/*
	Description
	HPDF_Page_MoveTextPos2() moves the current text position to the start of the next line with using specified offset values, and sets the text-leading to -y. If the start position of the current line is (x1, y1), the start of the next line is (x1 + x, y1 + y). 
	An application can invoke HPDF_MoveTextPos2() when the graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	xpos, ypos
	The offset of the start of the next line.

	text
	The text to show.

	Returns
	When HPDF_Page_MoveTextPos2() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS MoveTextPos2(CValue x, CValue y)
	{
		return ::HPDF_Page_MoveTextPos2(m_page, x, y);
	}

	/*
	*/
	HPDF_STATUS SetTextMatrix(CValue a, CValue b, CValue c, CValue d, CValue x, CValue y)
	{
		return ::HPDF_Page_SetTextMatrix(m_page, a, b, c, d, x, y);
	}
	
	/*
	Description
	HPDF_Page_MoveTextPos2() moves the current text position to the start of the next line. If the start position of the current line is (x1, y1), the start of the next line is (x1, y1 - text leading).
	An application can invoke HPDF_MoveTextPos2() when the graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT.

	NOTE: 
	Since the default value of Text Leading is 0,  an application have to invoke HPDF_Page_SetTextLeading() before HPDF_Page_MoveTextPos2() to set text leading. 

	Parameter
	page 
	The handle of a page object.

	xpos, ypos
	The offset of the start of the next line.

	text
	The text to show.

	Returns
	When HPDF_Page_MoveTextPos2() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS MoveToNextLine(void)
	{
		return ::HPDF_Page_MoveToNextLine(m_page);
	}

	/*
	Description
	HPDF_Page_ShowText() prints the text at the current position on the page.
	An application can invoke HPDF_Page_ShowText() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	text
	The text to print.

	Returns
	When HPDF_Page_ShowText() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ShowText(LPCSTR text)
	{
		return ::HPDF_Page_ShowText(m_page, text);
	}

	/*
	Description
	HPDF_Page_ShowTextNextLine() moves the current text position to the start of the next line, then prints the text at the current position on the page.
	An application can invoke HPDF_Page_ShowTextNextLine() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	text
	The text to print.

	Returns
	When HPDF_Page_ShowTextNextLine() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ShowTextNextLine(LPCSTR text)
	{
		return ::HPDF_Page_ShowTextNextLine(m_page, text);
	}

	/*
	Description
	HPDF_Page_ShowTextNextLineEx() moves the current text position to the start of the next line, then sets the word spacing, character spacing and prints the text at the current position on the page.
	An application can invoke HPDF_Page_ShowTextNextLine() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	text
	The text to print.

	Returns
	When HPDF_Page_ShowTextNextLine() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS ShowTextNextLineEx(CValue wordSpace, CValue charSpace, LPCSTR text)
	{
		return ::HPDF_Page_ShowTextNextLineEx(m_page, wordSpace, charSpace, text);
	}

	/*
	Description
	HPDF_Page_SetGrayFill() sets the filling color.
	An application can invoke HPDF_Page_SetGrayFill() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	value
	The value of the gray level between 0 and 1.

	Returns
	When HPDF_Page_SetGrayFill() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetGrayFill(CValue gray)
	{
		return ::HPDF_Page_SetGrayFill(m_page, gray);
	}

	/*
	Description
	HPDF_Page_SetGrayStroke() sets the stroking color.
	An application can invoke HPDF_Page_SetGrayStroke() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	value
	The value of the gray level between 0 and 1.

	Returns
	When HPDF_Page_SetGrayStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetGrayStroke(CValue gray)
	{
		return ::HPDF_Page_SetGrayStroke(m_page, gray);
	}

	/*
	Description
	HPDF_Page_SetRGBFill() sets the filling color.
	An application can invoke HPDF_Page_SetRGBFill() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	r, g, b
	The level of each color element. They must be between 0 and 1. (See "Colors") 

	Returns
	When HPDF_Page_SetRGBFill() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetRgbFill(CValue r, CValue g, CValue b)
	{
		return ::HPDF_Page_SetRGBFill(m_page, r, g, b);
	}

	/*
	Description
	HPDF_Page_SetRGBStroke() sets the stroking color.
	An application can invoke HPDF_Page_SetRGBStroke() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	r, g, b
	The level of each color element. They must be between 0 and 1. (See "Colors") 

	Returns
	When HPDF_Page_SetRGBStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetRgbStroke(CValue r, CValue g, CValue b)
	{
		return ::HPDF_Page_SetRGBStroke(m_page, r, g, b);
	}

	/*
	Description
	HPDF_Page_SetCMYKFill() sets the filling color.
	An application can invoke HPDF_Page_SetCMYKFill() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	c, m, y, k
	The level of each color element. They must be between 0 and 1.

	Returns
	When HPDF_Page_SetCMYKFill() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetCmykFill(CValue c, CValue m, CValue y, CValue k)
	{
		return ::HPDF_Page_SetCMYKFill(m_page, c, m, y, k);
	}

	/*
	Description
	HPDF_Page_SetCMYKStroke() sets the stroking color.
	An application can invoke HPDF_Page_SetCMYKStroke() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	c, m, y, k
	The level of each color element. They must be between 0 and 1.

	Returns
	When HPDF_Page_SetCMYKStroke() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS SetCmykStroke(CValue c, CValue m, CValue y, CValue k)
	{
		return ::HPDF_Page_SetCMYKStroke(m_page, c, m, y, k);
	}

	/*
	*/
	HPDF_STATUS ExecuteXObject(HPDF_XObject obj)
	{
		return ::HPDF_Page_ExecuteXObject(m_page, obj);
	}

	/*
	Description
	HPDF_Page_DrawImage() shows an image in one operation.
	An application can invoke HPDF_Page_DrawImage() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION.

	Parameter
	page 
	The handle of a page object.

	image 
	The handle of an image object.
	x, y 
	The lower-left point of the region where image is displayed.

	width 
	The width of the region where image is displayed.

	height 
	The width of the region where image is displayed. 

	Returns
	When HPDF_Page_DrawImage() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS DrawImage(HPDF_Image image, CValue x, CValue y, CValue width, CValue height)
	{
		return ::HPDF_Page_DrawImage(m_page, image, x, y, width, height);
	}

	/*
	Description
	HPDF_Page_Circle() appends a circle to the current path.
	An application can invoke HPDF_Page_Circle() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_PATH_OBJECT.

	Parameter
	page 
	The handle of a page object.

	x, y
	The center point of the circle.

	ray
	The ray of the circle.

	Returns
	When HPDF_Page_Circle() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS Circle(CValue x, CValue y, CValue ray)
	{
		return ::HPDF_Page_Circle(m_page, x, y, ray);
	}

	/*
	Description
	HPDF_Page_Arc() appends a circle to the current path.
	An application can invoke HPDF_PageArc() when the graphics mode of the page is in HPDF_GMODE_PAGE_DESCRIPTION or HPDF_GMODE_PATH_OBJECT.

	Parameter
	page 
	The handle of a page object.

	x, y
	The center point of the circle.

	ray
	The ray of the circle.

	ang1
	The angle of the begining of the arc.

	ang2
	The angle of the end of the arc. It must be greater than ang1.

	Returns
	When HPDF_Page_Arc() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS Arc(CValue x, CValue y, CValue ray, CValue ang1, CValue ang2)
	{
		return ::HPDF_Page_Arc(m_page, x, y, ray, ang1, ang2);
	}

	/*
	Description
	HPDF_Page_TextOut() prints the text on the specified position.
	An application can invoke HPDF_TextOut() when the graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	xpos, ypos
	The point position where the text is displayed.

	text
	The text to show.

	Returns
	When HPDF_Page_TextOut() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS TextOut(CValue xpos, CValue ypos, LPCSTR text)
	{
		return ::HPDF_Page_TextOut(m_page, xpos, ypos, text);
	}

	/*
	Description
	HPDF_Page_TextRext() TextRext() print the text inside the specified region. The characters which is not 
	An application can invoke HPDF_TextRect() when the graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT.

	Parameter
	page 
	The handle of a page object.

	left, top, right, bottom
	Coordinates of corners of the region to output text.

	text
	The text to show.

	align
	The alignment of the text.VALUE	DESCRIPTION
	HPDF_TALIGN_LEFT	The text is aligned to left.
	HPDF_TALIGN_RIGHT	The text is aligned to right.
	HPDF_TALIGN_CENTER	The text is aligned to center.
	HPDF_TALIGN_JUSTIFY	Add spaces between the words to justify both left and right side.


	len
	If this value is not NULL, The number of characters which was printed in the area is returned.


	Returns
	When HPDF_Page_TextRect() succeed, it returns HPDF_OK, Otherwise it returns error code and error-handler is invoked.
	*/
	HPDF_STATUS TextRect(CValue left, CValue top, CValue right, CValue bottom, LPCSTR text, HPDF_TextAlignment align, HPDF_UINT* len)
	{
		return ::HPDF_Page_TextRect(m_page, left, top, right, bottom, text, align, len);
	}

};






/**@ingroup PDF
 * OnError()で通知されます。
 */
class CHpdfDocument
{
	CHpdfDocHandle	m_handle;		///< DOCのハンドル

	/// エラーハンドラ
	static void WINAPI ms_ErrorHandler(HPDF_STATUS errorNo, HPDF_STATUS detailNo, LPVOID P)
	{
		CHpdfDocument* pDoc = static_cast<CHpdfDocument*>(P);
		TRACE2(" CHpdfDocument Error: error_no=%04X, detail_no=%u\n", errorNo, detailNo);
		pDoc->OnError(errorNo, detailNo);
	}

public:

	/**
	 * [通知] エラー通知
	 *	@note このメソッドをオーバーライドしておくと、エラー時に即通知されます。
	 *	@param errorNo エラー番号
	 *	@param detailNo エラー詳細番号
	 */
	virtual void OnError(HPDF_STATUS errorNo, HPDF_STATUS detailNo)
	{
	}

	/**
	 * コンストラクタ
	 */ 
	CHpdfDocument(void)
	{
		m_handle = ::HPDF_New(&ms_ErrorHandler, this);
	}

	/**
	 * [取得] エラー番号取得
	 *	@return エラー番号
	 */
	HPDF_STATUS GetLastError(void) const
	{
		return ::HPDF_GetError(m_handle);
	}

	/**
	 * [取得] エラー詳細番号取得
	 *	@return エラー詳細番号
	 */
	HPDF_STATUS GetLastErrorDetail(void) const
	{
		return ::HPDF_GetErrorDetail(m_handle);
	}

	/**
	 * [確認] 本インスタンス有効確認
	 *	@retval true 有効
	 *	@retval false 無効
	 */
	bool IsValid(void) const
	{
		return ! m_handle.IsNull();
	}

	/**
	 * [作成] creates new document. 
	 *		If document object already has a document, the current document is revoked.
	 *	@retval HPDF_OK succeeds
	 *	@retval HPDF_INVALID_DOCUMENT An invalid document handle is set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS NewDoc(void) const
	{
		return ::HPDF_NewDoc(m_handle);
	}

	/**
	 * [破棄] revokes the current document. 
	 *	@note keeps loaded resource (such as fonts and encodings) and these resources are 
	 *		recycled when new document required these resources. 
	 */
	void FreeDoc(void) const
	{
		::HPDF_FreeDoc(m_handle);
	}

	/**
	 * [破棄] revokes the current document. 
	 *	@note revokes the current document and all resources.
	 */
	void FreeDocAll(void) const
	{
		::HPDF_FreeDocAll(m_handle);
	}

	/**
	 * [処理] saves the current document to a file.
	 *	@param lpszFileName Specify the name of file to save.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT		An invalid document handle is set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *	@retval HPDF_FILE_IO_ERROR			An error occurred while processing file I/O.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SaveToFile(LPCSTR lpszFileName) const
	{
		return ::HPDF_SaveToFile(m_handle, lpszFileName);
	}

	/**
	 * [処理] saves the current document to a temporary stream of a document object.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT		An invalid document handle is set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SaveToStream(void) const
	{
		return ::HPDF_SaveToStream(m_handle);
	}

	/**
	 * [取得] gets the size of the temporary stream of the document.
	 *	@retval ０以外 成功。
	 *	@retval ０ エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	size_t GetStreamSize(void) const
	{
		return ::HPDF_GetStreamSize(m_handle);
	}

	/**
	 * [設定] copies the data from the temporary stream of the document into the buffer.
	 *	@param[out] buf	A pointer of the buffer.
	 *	@param[out] _size Specifiy the size of buffer. After ReadFromStream() is execute, 
	 *				the number of bytes that succeeds in reading is set.
	 *	@note	Even if the result of ReadFromStream() is HPDF_EOF, 
	 *			if the value of size parameter is more than 0, data is read into the buffer. 
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle is set.
	 *	@retval HPDF_INVALID_OPERATION	No data is saved to the temporary stream of the document.
	 *	@retval HPDF_INVALID_PARAMETER	An invalid value was set to 'size' parameter.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS ReadFromStream(LPVOID buf, size_t& _size)
	{
		return ::HPDF_ReadFromStream(m_handle, reinterpret_cast<BYTE*>(buf), &_size);
	}

	/**
	 * [処理] rewinds the temporary stream of the document.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle is set.
	 *	@retval HPDF_INVALID_OPERATION	No data is saved to the temporary stream of the document.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS ResetStream(void)
	{
		return ::HPDF_ResetStream(m_handle);
	}

	/**
	 * [設定] ページ設定
	 *	@note In the default setting, a HPDF_Doc object has one "Pages" object as root of pages. 
	 *			All "Page" objects are created as a kid of the "Pages" object.
	 *			Since a "Pages" object can own only 8191 kids objects, the maximum number of pages are 8191 page.
	 *
	 *	Additionally, the state that there are a lot of  "Page" object under one "Pages" object is not good, 
	 *	because it causes performance degradation of  a viewer application.
	 *
	 *	An application can change the setting of a pages tree by invoking SetPagesConfiguration(). 
	 *	If pagePerPages parameter is set to more than zero, a two-tier pages tree is created. 
	 *	A root "Pages" object can own 8191 "Pages" object, and each lower "Pages" object can own 
	 *	pagePerPages "Page" objects. As a result, the maximum number of pages becomes 8191 × pagePerPages page.
	 *
	 *	An application cannot invoke SetPageConfiguration() after a page is added to document.
	 *
	 *	@param pagePerPages	Specify the numbers of pages that a "Pages" object can own.
	 *
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle is set.
	 *	@retval HPDF_INVALID_DOCUMENT_STATE	A page object already exists in a document.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetPagesConfiguration(size_t pagePerPages)
	{
		return ::HPDF_SetPagesConfiguration(m_handle, pagePerPages);
	}

	/**
	 * [設定] sets how the page should be displayed. 
	 *	If this attribute is not set, the setting of a viewer application is used.
	 *	@param layout The following values are available.
	 *		\n	HPDF_PAGE_LAYOUT_SINGLE	Only one page is displayed.
	 *		\n	HPDF_PAGE_LAYOUT_ONE_COLUMN	Display the pages in one column.
	 *		\n	HPDF_PAGE_LAYOUT_TWO_COLUMN_LEFT	Display the pages in two column. The page of the odd number is displayed left. 
	 *		\n	HPDF_PAGE_LAYOUT_TWO_COLUMN_RIGHT	Display the pages in two column. The page of the odd number is displayed right. 
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle is set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *	@retval HPDF_PAGE_LAYOUT_OUT_OF_RANGE	An invalid page layout is specified.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetPageLayout(HPDF_PageLayout layout)
	{
		return ::HPDF_SetPageLayout(m_handle, layout);
	}

	/**
	 * [取得] returns the current setting for page layout.
	 *	@retval HPDF_PAGE_LAYOUT_EOF 設定されていない。
	 *	@retval それ以外 設定されているページレイアウト。
	 */
	HPDF_PageLayout GetPageLayout(void) const
	{
		return ::HPDF_GetPageLayout(m_handle);
	}

	/**
	 * [設定] sets how the document should be displayed. 
	 *	@param mode	The following values are available.
	 *			\n		HPDF_PAGE_MODE_USE_NONE	Display the document with neither outline nor thumbnail. 
	 *			\n		HPDF_PAGE_MODE_USE_OUTLINE	Display the document with outline pain.
	 *			\n		HPDF_PAGE_MODE_USE_THUMBS	Display the document with thumbnail pain.
	 *			\n		HPDF_PAGE_MODE_FULL_SCREEN	Display the document with full screen mode. 
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle is set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *	@retval HPDF_PAGE_MODE_OUT_OF_RANGE	An invalid page mode is specified.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetPageMode(HPDF_PageMode mode)
	{
		return ::HPDF_SetPageMode(m_handle, mode);
	}

	/**
	 * [取得] returns the current setting for page mode.
	 *	@return the current setting for page mode.
	*/
	HPDF_PageMode GetPageMode(void) const
	{
		return ::HPDF_GetPageMode(m_handle);
	}

	/**
	 * [設定] set the first page appears when a document is opened. 
	 *	@param openAction Set a valid destination object.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *	@retval HPDF_INVALID_DESTINATION	An invalid destination object was set.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetOpenAction(HPDF_Destination openAction)
	{
		return ::HPDF_SetOpenAction(m_handle, openAction);
	}

	/**
	 * [取得] returns the handle of current page object.
	 *	@retval NULL エラー。
	 *	@retval それ以外  handle of a current page object.
	 */
	CHpdfPage GetCurrentPage(void) const
	{
		return ::HPDF_GetCurrentPage(m_handle);
	}

	/**
	 * [作成] creates a new page and adds it after the last page of a document. 
	 *	@retval NULL以外 Pageのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 */
	CHpdfPage AddPage(void)
	{
		return ::HPDF_AddPage(m_handle);
	}

	/**
	 * [作成] creates a new page and inserts it just before the specified page. 
	 *	@param target Specify the handle of a page object which specifies the insertion place.
	 *	@retval NULL以外 Pageのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_PAGE	An invalid page handle was set.
	 */
	CHpdfPage InsertPage(CHpdfPage target)
	{
		return ::HPDF_InsertPage(m_handle, target);
	}

	/**
	 * [取得] gets the handle of a corresponding font object by specified name and encoding.
	 *	@param lpszFontName Specify a valid font name. 
	 *			  (See <a class="el" href="http://libharu.sourceforge.net/fonts.html"> "Fonts and Encodings"</a> )
	 *	@param lpszEncodingName Specify a valid encoding name. 
	 *			  (See <a class="el" href="http://libharu.sourceforge.net/fonts.html"> "Fonts and Encodings"</a> )
	 *	@retval NULL以外 Fontのハンドル。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT		An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *			\n		HPDF_INVALID_FONT_NAME
	 *			\n		HPDF_UNSUPPORTED_FONT_TYPE	An invalid font name was set.
	 *			\n		HPDF_INVALID_ENCODING_NAME	An invalid encoding name was set.
	 */
	HPDF_Font GetFont(LPCSTR lpszFontName, LPCSTR lpszEncodingName) const
	{
		return ::HPDF_GetFont(m_handle, lpszFontName, lpszEncodingName);
	}

	/**
	 * [取得] loads a type1 font from an external file and register it to a document object.
	 *	@param lpszAfmFileName	A path of an AFM file. 
	 *	@param lpszDataFileName	A path of a PFA/PFB file. If it is NULL, 
	 *							the gryph data of font file is not embedded to a PDF file.
	 *	@retval NULL以外 フォントの名前（文字列）。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *			\n		HPDF_FONT_EXISTS			The font of the same name has already been registered.
	 *			\n		HPDF_INVALID_AFM_HEADER
	 *			\n		HPDF_INVALID_CHAR_MATRICS_DATA
	 *			\n		HPDF_INVALID_N_DATA			Cannot recognize AFM file.
	 *			\n		HPDF_UNSUPPORTED_TYPE1_FONT	Cannot recognize PFA/PFB file.
	 */
	LPCSTR LoadType1FontFromFile(LPCSTR lpszAfmFileName, LPCSTR lpszDataFileName)
	{
		return ::HPDF_LoadType1FontFromFile(m_handle, lpszAfmFileName, lpszDataFileName);
	}

	/**
	 * [取得] loads a TrueType font from an external file and register it to a document object. 
	 *	@param lpszFileName A path of a TrueType font file (.ttf). 
	 *	@param boIsEmbedding	true - the glyph data of the font is embedded.
	 *						\n	false - only the matrix data is included in PDF file.
	 *	@retval NULL以外 フォントの名前（文字列）。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *			\n		HPDF_FONT_EXISTS			The font of the same name has already been registered.
	 *			\n		HPDF_INVALID_TTC_INDEX	
	 *			\n		HPDF_INVALID_TTC_FILE	
	 *			\n		HPDF_TTF_INVALID_CMAP	
	 *			\n		HPDF_TTF_INVALID_FOMAT	
	 *			\n		HPDF_TTF_MISSING_TABLE	Failed to load .ttf file.
	 *			\n		HPDF_TTF_CANNOT_EMBEDDING_FONT	The font is not allowed to embed.
	 */
	LPCSTR LoadTTFontFromFile(LPCSTR lpszFileName, bool boIsEmbedding) const
	{
		return ::HPDF_LoadTTFontFromFile(m_handle, lpszFileName,  boIsEmbedding);
	}

	/**
	 * [取得] loads a TrueType font from an TrueType collection file and register it to a document object.
	 *	@param lpszFileName A path of a TrueType font collection file (.ttc). 
	 *	@param index	The index of font that wants to be loaded. 
	 *	@param boIsEmbedding	true - the glyph data of the font is embedded.
	 *						\n	false - only the matrix data is included in PDF file.
	 *	@retval NULL以外 フォントの名前（文字列）。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM		Memory Allocation Failed.
	 *			\n		HPDF_FONT_EXISTS			The font of the same name has already been registered.
	 *			\n		HPDF_INVALID_TTC_INDEX	
	 *			\n		HPDF_INVALID_TTC_FILE	
	 *			\n		HPDF_TTF_INVALID_CMAP	
	 *			\n		HPDF_TTF_INVALID_FOMAT	
	 *			\n		HPDF_TTF_MISSING_TABLE	Failed to load .ttf file.
	 *			\n		HPDF_TTF_CANNOT_EMBEDDING_FONT	The font is not allowed to embed.
	 *			\n		HPDF_INVALID_TTC_INDEX		The value specified at index parameter exceeds the number of fonts. 
	 */
	LPCSTR LoadTTFontFromFile2(LPCSTR lpszFileName, size_t index, bool boIsEmbedding)
	{
		return ::HPDF_LoadTTFontFromFile2(m_handle, lpszFileName, index, boIsEmbedding);
	}

	/**
	 * [追加] adds a page labeling range for the document.
	 *	@param pageNum The first page that applies this labeling range. 
	 *	@param style	The numbering style.
	 *		\n			1	HPDF_PAGE_NUM_STYLE_DECIMAL	Page label is displayed by Arabic numerals.
	 *		\n			2	HPDF_PAGE_NUM_STYLE_UPPER_ROMAN	Page label is displayed by Uppercase roman numerals.
	 *		\n			3	HPDF_PAGE_NUM_STYLE_LOWER_ROMAN	Page label is displayed by Lowercase roman numerals.
	 *		\n			4	HPDF_PAGE_NUM_STYLE_UPPER_LETTERS	Page label is displayed by Uppercase letters (using A to Z).
	 *		\n			5	HPDF_PAGE_NUM_STYLE_LOWER_LETTERS	Page label is displayed by Lowercase letters (using a to z).
	 *	@param firstPage The first page number in this range. 
	 *	@param lpszPrefix	The prefix for the page label. (省略可能.) 
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_PAGE_NUM_STYLE_OUT_OF_RANGE	An invalid page numbering style is specified.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	*/
	HPDF_STATUS AddPageLabel(size_t pageNum, HPDF_PageNumStyle style, size_t firstPage, LPCSTR lpszPrefix = NULL)
	{
		return ::HPDF_AddPageLabel(m_handle, pageNum, style, firstPage, lpszPrefix);
	}

	/**
	 * [設定] enables Japanese fonts.
	 *		After UseJpnFonts() is involed, an application can use the following Japanese fonts.
	 *	@see http://libharu.sourceforge.net/document_handling.html
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The font of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseJpnFonts(void)
	{
		return ::HPDF_UseJPFonts(m_handle);
	}

	/**
	 * [設定] enables Korean fonts. 
	 *		After UseKorFonts() is involed, an application can use the following Korean fonts.
	 *	@see http://libharu.sourceforge.net/document_handling.html
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The font of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	*/
	HPDF_STATUS UseKorFonts(void)
	{
		return ::HPDF_UseKRFonts(m_handle);
	}

	/**
	 * [設定] enables simplified Chinese fonts. 
	 *		After UseChnSimpFonts() is involed, an application can use the following simplified Chinese fonts.
	 *	@see http://libharu.sourceforge.net/document_handling.html
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The font of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseChnSimpFonts(void)
	{
		return ::HPDF_UseCNSFonts(m_handle);
	}

	/**
	 * [設定] enables traditional Chinese fonts. 
	 *		After UseChnTradFonts() is involed, an application can use the following traditional Chinese fonts.
	 *	@see http://libharu.sourceforge.net/document_handling.html
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The font of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseChnTradFonts(void)
	{
		return ::HPDF_UseCNTFonts(m_handle);
	}

	/**
	 * [作成] creates a new outline object.
	 *	@param parent	The handle of an outline object which comes to the parent of the created outline object.
	 *					If this parameter is NULL, The outline is created as a root outline.
	 *	@param lpszTitle	The caption of the outline object.
	 *	@param encoder The handle of an encoding object applied to the title.
	 *					省略すると PDFDocEncoding が使われます。
	 *	@retval NULL以外 アウトラインオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_OUTLINE	An invalid parent outline is specified.
	 */
	HPDF_Outline CreateOutline(HPDF_Outline parent, LPCSTR lpszTitle, HPDF_Encoder encoder = NULL) const
	{
		return ::HPDF_CreateOutline(m_handle, parent, lpszTitle, encoder);
	}

	/**
	 * [取得] gets the handle of a corresponding encoder object by specified encoding name.
	 *	@param lpszEncodingName Specify a valid encoding name. 
	 *			  (See <a class="el" href="http://libharu.sourceforge.net/fonts.html"> "Fonts and Encodings"</a> )
	 *	@retval NULL以外 エンコーダオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_ENCODING_NAME	An invalid encoding name was set. 
	 */
	HPDF_Encoder GetEncoder(LPCSTR lpszEncodingName) const
	{
		return ::HPDF_GetEncoder(m_handle, lpszEncodingName);
	}

	/**
	 * [取得] gets the handle of the current encoder of the document object.
	 *		The current encoder is set by invoking SetCurrentEncoder() and 
	 *		it is used to processing a text  when an application invoks SetInfoAttr().
	 *		The default value of it is NULL.
	 *	@return a handle of an encoder object or NULL.
	*/
	HPDF_Encoder GetCurrentEncoder(void) const
	{
		return ::HPDF_GetCurrentEncoder(m_handle);
	}

	/**
	 * [設定] sets the current encoder for the document.
	 *	@param lpszEncodingName The name of an encoding. 
	 *			  (See <a class="el" href="http://libharu.sourceforge.net/fonts.html"> "Fonts and Encodings"</a> )
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_INVALID_ENCODING_NAME	An invalid encoding name was set.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The font of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	*/
	HPDF_STATUS SetCurrentEncoder(LPCSTR lpszEncodingName)
	{
		return ::HPDF_SetCurrentEncoder(m_handle, lpszEncodingName);
	}

	/**
	 * [設定] enables Japanese encodings. 
	 *		After UseJpnEncodings() is involed, an application can use the following Japanese encodings.
	 *	@see http://libharu.sourceforge.net/document_handling.html
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The encoding of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseJpnEncodings(void)
	{
		return ::HPDF_UseJPEncodings(m_handle);
	}

	/**
	 * [設定] enables Korean encodings. 
	 *		After UseKorEncodings() is involed, an application can use the following Korean encodings.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The encoding of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseKorEncodings(void)
	{
		return ::HPDF_UseKREncodings(m_handle);
	}

	/**
	 * [設定] enables simplified Chinese encodings.
	 *		After UseChnSimpEncodings() is involed, an application can use the following simplified Chinese encodings.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The encoding of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseChnSimpEncodings(void)
	{
		return ::HPDF_UseCNSEncodings(m_handle);
	}

	/**
	 * [設定] enables traditional Chinese encodings. 
	 *		After UseChnTradEncodings() is involed, an application can use the following traditional Chinese encodings.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_DUPLICATE_REGISTRATION	The encoding of the same name has already been registered.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS UseChnTradEncodings(void)
	{
		return ::HPDF_UseCNTEncodings(m_handle);
	}

	/**
	 * [取得] loads an external png image file. 
	 *	@param lpszFileName	A path to a PNG image file. 
	 *	@retval NULL以外 イメージオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_UNSUPPORTED_FUNC	The library is not configured to use PNGLIB.
	 *			\n		HPDF_LIBPNG_ERROR	Failed when invoking PNGLIB's function.
	 *			\n		HPDF_INVALID_PNG_IMAGE	Invalid png format.
	 */
	HPDF_Image LoadPngImageFromFile(LPCSTR lpszFileName)
	{
		return ::HPDF_LoadPngImageFromFile(m_handle, lpszFileName);
	}

	/**
	 * [取得] loads an external png image file.
	 *		Unlike  HPDF_LoadPngImageFromFile(),  HPDF_LoadPngImageFromFile2() does not load whole data immediately. 
	 *		(only size and color properties is loaded). 
	 *		The main data is loaded just before the image object is written to PDF, and the loaded data is deleted immediately. 
	 *	@param lpszFileName A path to a PNG image file. 
	 *	@retval NULL以外 イメージオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_UNSUPPORTED_FUNC	The library is not configured to use PNGLIB.
	 *			\n		HPDF_LIBPNG_ERROR	Failed when invoking PNGLIB's function.
	 *			\n		HPDF_INVALID_PNG_IMAGE	Invalid png format.
	*/
	HPDF_Image LoadPngImageFromFile2(LPCSTR lpszFileName)
	{
		return ::HPDF_LoadPngImageFromFile2(m_handle, lpszFileName);
	}

	/**
	 * [取得] loads an image which has "raw" image format.
	 *		This function loads the data without any conversion. So it is usually faster than the other functions.
	 *		LoadRawImageFromFile() can load 3 types of format described below.
	 *	
	 *	8 bit gray scale image. 
	 *		The gray scale describes one pixel by one byte. And the size of the image data is same as width * height.
	 *		The sequence of the data is as follows.
	 *		\n		01	02	03	04
	 *		\n		06	07	08	09
	 *		\n		11	12	13	14
	 *
	 *	24bit RGB color image. 
	 *		The 24bit RGB color image describes one pixel by 3 byte (each one byte describes a value of either red, green or blue). And the size of the image is same as width * height * 3.
	 *		The sequence of the data is as follows.
	 *		\n		01	01	01	02	02	02	03	03	03	04	04	04
	 *		\n		06	06	06	07	07	07	08	08	08	09	09	09
	 *		\n		11	11	11	12	12	12	13	13	13	14	14	14
	 *
	 *	36bit CMYK color image. 
	 *		The 36bit CMYK color image describes one pixel by 4 byte (each one byte describes a value of either Cyan Magenta Yellow Black). And the size of the image is same as width * height * 4. 
	 *		The sequence of the data is as follows.
	 *		\n		01	01	01	01	02	02	02	02	03	03	03	03	04	04	04	04
	 *		\n		06	06	06	06	07	07	07	07	08	08	08	08	09	09	09	09
	 *		\n		11	11	11	11	12	12	12	12	13	13	13	13	14	14	14	14
	 *
	 *	@param lpszFileName A path to a image file.
	 *	@param width The width of an image file. 
	 *	@param height The height of an image file. 
	 *	@param colorSpace HPDF_CS_DEVICE_GRAY or HPDF_CS_DEVICE_RGB or HPDF_CS_DEVICE_CMYK is allowed.
	 *	@retval NULL以外 イメージオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_UNSUPPORTED_FUNC	The library is not configured to use PNGLIB.
	 *			\n		HPDF_LIBPNG_ERROR	Failed when invoking PNGLIB's function.
	 *			\n		HPDF_INVALID_PNG_IMAGE	Invalid png format.
	 *			\n		HPDF_FILE_IO_ERROR	Cannot read data from the file.
	 */
	HPDF_Image LoadRawImageFromFile(LPCSTR lpszFileName, size_t width, size_t height, HPDF_ColorSpace colorSpace)
	{
		return ::HPDF_LoadRawImageFromFile(m_handle, lpszFileName, width, height, colorSpace);
	}

	/**
	 * [取得] loads an image which has "raw" image format from buffer.
	 *		This function loads the data without any conversion. So it is usually faster than the other functions.
	 *		The formats that LoadRawImageFromMem() can load is the same as LoadRawImageFromFile().
	 *	@param buf The pointer to the image data.
	 *	@param width The width of an image file. 
	 *	@param height The height of an image file. 
	 *	@param colorSpace HPDF_CS_DEVICE_GRAY or HPDF_CS_DEVICE_RGB or HPDF_CS_DEVICE_CMYK is allowed.
	 *	@param bitsPerComponent The bit size of each color component. The valid value is either 1, 2, 4, 8. 
	 *	@retval NULL以外 イメージオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_UNSUPPORTED_FUNC	The library is not configured to use PNGLIB.
	 *			\n		HPDF_LIBPNG_ERROR	Failed when invoking PNGLIB's function.
	 *			\n		HPDF_INVALID_PNG_IMAGE	Invalid png format.
	*/
	HPDF_Image LoadRawImageFromMem(LPCVOID buf, size_t width, size_t height, HPDF_ColorSpace colorSpace, size_t bitsPerComponent)
	{
		return ::HPDF_LoadRawImageFromMem(m_handle, reinterpret_cast<const unsigned char*>(buf), width, height, colorSpace, bitsPerComponent);
	}

	/**
	 * [取得] loads an external Jpeg image file.
	 *	@param lpszFileName A path to a Jpeg image file. 
	 *	@retval NULL以外 イメージオブジェクト。
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_UNSUPPORTED_JPEG_FORMAT	Unsupported () JPEG image format.
	 */
	HPDF_Image LoadJpegImageFromFile(LPCSTR lpszFileName)
	{
		return ::HPDF_LoadJpegImageFromFile(m_handle, lpszFileName);
	}

	/**
	 * [設定] sets the text of the info dictionary. 
	 *		SetInfoAttr() uses the current encoding of the document. If it is not set, the PDFDocEncoding is used.
	 *	@param type The following values are available. 
	 *			HPDF_INFO_AUTHOR / HPDF_INFO_CREATOR / HPDF_INFO_TITLE / HPDF_INFO_SUBJECT / HPDF_INFO_KEYWORDS
	 *	@param lpszValue A text to set the infomation. 
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_INVALID_PARAMETER	An invalid type parameter was set.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetInfoAttr(HPDF_InfoType type, LPCSTR lpszValue)
	{
		return ::HPDF_SetInfoAttr(m_handle, type, lpszValue);
	}

	/**
	 * [取得] gets the string value fron info dictionary.
	 *	@param type The following values are available.
	 *		HPDF_INFO_CREATION_DATE / HPDF_INFO_MOD_DATE /
	 *		HPDF_INFO_AUTHOR / HPDF_INFO_CREATOR /
	 *		HPDF_INFO_TITLE / HPDF_INFO_SUBJECT /
	 *		HPDF_INFO_KEYWORDS.
	 *	@retval NULL以外 the string value of the info dictionary.
	 *	@retval NULL エラー。エラー番号は OnError() で通知されます。また、 GetLastError() でも取得出来ます。
	 *			\n		HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *			\n		HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *			\n		HPDF_INVALID_PARAMETER	An invalid type parameter was set.
	 */
	LPCSTR GetInfoAttr(HPDF_InfoType type) const
	{
		return ::HPDF_GetInfoAttr(m_handle, type);
	}

	/**
	 * [設定] sets the datetime value to the info dictionary. 
	 *	@param type The following values are available. HPDF_INFO_CREATION_DATE / HPDF_INFO_MOD_DATE.
	 *	@param value	A datetime value to set the infomation.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_INVALID_PARAMETER	An invalid type parameter was set.
	 *	@retval HPDF_INVALID_DATE_TIME  An invalid datetime value was set.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
 	 */
	HPDF_STATUS SetInfoDateAttr(HPDF_InfoType type, const HPDF_Date& value)
	{
		return ::HPDF_SetInfoDateAttr(m_handle, type, value);
	}

	/**
	 * [設定] sets the pasword for the document.
	 *		If the password is set, contents in the document are encrypted. 
	 *	@param lpszOwnerPassword 
	 *		The password for the owner of the document. The owner can change the permission of the document.
	 *		NULL, zero length string and the same value as user password are not allowed. 
	 *	@param lpszUserPassword
	 *		The password for the user of the document. The user_password is allowed to be set to NULL or zero length string.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_INVALID_PASSWORD	
	 *				NULL, zero length string and the same value as user password are set to owner-password.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetPassword(LPCSTR lpszOwnerPassword, LPCSTR lpszUserPassword)
	{
		return ::HPDF_SetPassword(m_handle, lpszOwnerPassword, lpszUserPassword);
	}

	/**
	 * [設定] set the flags of the permission for the document.
	 *	@param permission 	The flags specifying which operations are permitted. 
	 *			This parameter is set by logical addition of the following values.
	 *		\n			HPDF_ENABLE_READ	user can read the document.
	 *		\n			HPDF_ENABLE_PRINT	user can print the document.
	 *		\n			HPDF_ENABLE_EDIT_ALL	user can edit the contents of the document other than annotations, form fields.
	 *		\n			HPDF_ENABLE_COPY	user can copy the text and the graphics of the document.
	 *		\n			HPDF_ENABLE_EDIT	user can add or modify the annotations and form fields of the document.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetPermission(UINT permission)
	{
		return ::HPDF_SetPermission(m_handle, permission);
	}

	/**
	 * [設定] set the type of encryption.
	 *		As the side effect, SetEncryptionMode() ups the version of PDF to 1.4 when 
	 *		the mode is set to HPDF_ENCRYPT_R3.
	 *	@param mode The flags specifying which operations are permitted. 
	 *				This parameter is set by logical addition of the following values.
	 *			\n		HPDF_ENCRYPT_R2	Use "Revision 2" algorithm.The length of key is automatically set to 5(40bit).
	 *			\n		HPDF_ENCRYPT_R3	Use "Revision 3" algorithm.Between 5(40bit) and 16(128bit) can be specified for length of the key. 
	 *	@param keyLen
	 *	Specify the byte length of an encryption key. This parameter is valid only when "mode" parameter is set to HPDF_ENCRYPT_R3.
	 *	Between 5(40bit) and 16(128bit) can be specified for length of the key.
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_INVALID_ENCRYPT_KEY_LEN	An invalid key length was specified.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetEncryptionMode(HPDF_EncryptMode mode, size_t keyLen)
	{
		return ::HPDF_SetEncryptionMode(m_handle, mode, keyLen);
	}

	/**
	 * [設定] set the mode of compression.
	 *	@param mode
	 *	The flags specifying which type of contents should be compressed. 
	 *		\n		HPDF_COMP_NONE	All contents are not compressed. 
	 *		\n		HPDF_COMP_TEXT	Compress the contents stream of the page.   
	 *		\n		HPDF_COMP_IMAGE	Compress the streams of the image objects.  
	 *		\n		HPDF_COMP_METADATA	Other stream datas (fonts, cmaps and so on)  are compressed.
	 *		\n		HPDF_COMP_ALL	All stream datas are compressed. (The same as "HPDF_COMP_TEXT | HPDF_COMP_IMAGE | HPDF_COMP_METADATA")
	 *	@retval HPDF_OK succeeds.
	 *	@retval HPDF_INVALID_DOCUMENT	An invalid document handle was set.
	 *	@retval HPDF_INVALID_COMPRESSION_MODE An invalid compression mode was specified.
	 *	@retval HPDF_FAILD_TO_ALLOC_MEM	Memory Allocation Failed.
	 *	@retval HPDF_OK以外 エラー番号。エラー番号は OnError() でも通知されます。また、 GetLastError() でも取得出来ます。
	 */
	HPDF_STATUS SetCompressionMode(UINT mode)
	{
		return ::HPDF_SetCompressionMode(m_handle, mode);
	}
};






}; // TNB



#ifdef _TnbDOXYGEN	//Document作成用シンボル

/**
 * Specify a predefined page-size value
 */
enum HPDF_PageSizes 
{
	HPDF_PAGE_SIZE_LETTER = 0,	///< 8? x 11 (Inches)	/	612 x 792 (pixel)
	HPDF_PAGE_SIZE_LEGAL,		///< 8? x 14 (Inches)	/	612 x 1008 (pixel)
	HPDF_PAGE_SIZE_A3,			///< 297 × 420 (mm)	/	841.89 x 1199.551 (pixel)
	HPDF_PAGE_SIZE_A4,			///< 210 × 297 (mm)	/	595.276 x 841.89 (pixel)
	HPDF_PAGE_SIZE_A5,			///< 148 × 210 (mm)	/	419.528 x 595.276 (pixel)
	HPDF_PAGE_SIZE_B4,			///< 250 × 353 (mm) 	/	708.661 x 1000.63 (pixel)
	HPDF_PAGE_SIZE_B5,			///< 176 × 250 (mm)	/	498.898 x 708.661 (pixel)
	HPDF_PAGE_SIZE_EXECUTIVE,	///< 7? x 10? (Inches)	/	522 x 756 (pixel)
	HPDF_PAGE_SIZE_US4x6,		///< 4 x 6 (Inches)		/	288 x 432 (pixel)
	HPDF_PAGE_SIZE_US4x8,		///< 4 x 8 (Inches)		/	288 x 576 (pixel)
	HPDF_PAGE_SIZE_US5x7,		///< 5 x 7 (Inches)		/	360 x 504 (pixel)
	HPDF_PAGE_SIZE_COMM10,		///< 4.125 x 9.5 (Inches)	/	297x 684 (pixel)
};

/**
 * Specify the direction of the page
 */
enum HPDF_PageDirection
{
    HPDF_PAGE_PORTRAIT = 0,		///< Set the longer value to horizontal. 
    HPDF_PAGE_LANDSCAPE			///< Set the longer value to vertical.
};

///	The transition style.
enum HPDF_TransitionStyle
{
    HPDF_TS_WIPE_RIGHT = 0,
    HPDF_TS_WIPE_UP,
    HPDF_TS_WIPE_LEFT,
    HPDF_TS_WIPE_DOWN,
    HPDF_TS_BARN_DOORS_HORIZONTAL_OUT,
    HPDF_TS_BARN_DOORS_HORIZONTAL_IN,
    HPDF_TS_BARN_DOORS_VERTICAL_OUT,
    HPDF_TS_BARN_DOORS_VERTICAL_IN,
    HPDF_TS_BOX_OUT,
    HPDF_TS_BOX_IN,
    HPDF_TS_BLINDS_HORIZONTAL,
    HPDF_TS_BLINDS_VERTICAL,
    HPDF_TS_DISSOLVE,
    HPDF_TS_GLITTER_RIGHT,
    HPDF_TS_GLITTER_DOWN,
    HPDF_TS_GLITTER_TOP_LEFT_TO_BOTTOM_RIGHT,
    HPDF_TS_REPLACE,
};

/// The style of line-cap.value
enum HPDF_LineCap 
{
	HPDF_BUTT_END = 0,			///< The line is squared off at the endpoint of the path.
	HPDF_ROUND_END,				///< The end of a line becomes a semicircle whose center is the end point of the path.
	HPDF_PROJECTING_SCUARE_END	///< The line continues to the point that exceeds half of the stroke width the end point. 
};

#endif //_TnbDOXYGEN


/*

	HPDF_Date struct
		name	effective values
	1	year	

	2	month	between 1 and 12.
	3	day	between 1 and ether of 28, 29, 30, 31. (It is different by the month.)
	4	hour	0 to 23
	5	minutes	0 to 59
	6	seconds	0 to 59
	7	ind	the relationship of local time to Universal Time.
	" ", +, ?, and Z are available.
	8	off_hour	if ind is not space, 0 to 23 is valid. Otherwise, this value is ignored.
	9	off_minutes	if ind is not space, 0 to 59 is valid. Otherwise, this value is ignored.


	typedef struct _HPDF_DashMode {
		HPDF_UINT16  ptn[8];
		HPDF_UINT    num_ptn;
		HPDF_UINT    phase;
	} HPDF_DashMode;

  

	typedef struct _HPDF_RGBColor {
		HPDF_REAL   r;
		HPDF_REAL   g;
		HPDF_REAL   b;
	} HPDF_RGBColor;
	typedef struct _HPDF_CMYKColor {
		HPDF_REAL   c;
		HPDF_REAL   m;
		HPDF_REAL   y;
		HPDF_REAL   k;
	} HPDF_CMYKColor;

	*/

