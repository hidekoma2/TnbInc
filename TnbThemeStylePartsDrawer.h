#pragma once
/**
 *@file
 * �e�[�}�X�^�C���p�[�c�`����̃w�b�_
 *
 *		UsTheme.h ���g�p���Ă��܂��B SDK ���K�v�ł��B
 *
 *	@note	�}�N�� "_TnbTHEME_ClassicOriginal" ����`����Ă���Ȃ�A�N���V�b�N�� HOT �����͕t���Ȃ��悤�ɂ��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include <UxTheme.h>
#include "TnbPartsDrawable.h"
#include "TnbDynamicFunc.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#ifndef VSCLASS_BUTTON
 // BUTTON class parts and states 
 #define VSCLASS_BUTTON	L"BUTTON"
#endif // VSCLASS_BUTTON

#if ! defined(TMSCHEMA_H) && ! defined(__VSSTYLE_H__)

enum BUTTONPARTS {
	BP_PUSHBUTTON = 1,
	BP_RADIOBUTTON = 2,
	BP_CHECKBOX = 3,
	BP_GROUPBOX = 4,
	BP_USERBUTTON = 5,
};

enum PUSHBUTTONSTATES {
	PBS_NORMAL = 1,
	PBS_HOT = 2,
	PBS_PRESSED = 3,
	PBS_DISABLED = 4,
	PBS_DEFAULTED = 5,
};

enum RADIOBUTTONSTATES {
	RBS_UNCHECKEDNORMAL = 1,
	RBS_UNCHECKEDHOT = 2,
	RBS_UNCHECKEDPRESSED = 3,
	RBS_UNCHECKEDDISABLED = 4,
	RBS_CHECKEDNORMAL = 5,
	RBS_CHECKEDHOT = 6,
	RBS_CHECKEDPRESSED = 7,
	RBS_CHECKEDDISABLED = 8,
};

enum CHECKBOXSTATES {
	CBS_UNCHECKEDNORMAL = 1,
	CBS_UNCHECKEDHOT = 2,
	CBS_UNCHECKEDPRESSED = 3,
	CBS_UNCHECKEDDISABLED = 4,
	CBS_CHECKEDNORMAL = 5,
	CBS_CHECKEDHOT = 6,
	CBS_CHECKEDPRESSED = 7,
	CBS_CHECKEDDISABLED = 8,
	CBS_MIXEDNORMAL = 9,
	CBS_MIXEDHOT = 10,
	CBS_MIXEDPRESSED = 11,
	CBS_MIXEDDISABLED = 12,
};

enum GROUPBOXSTATES {
	GBS_NORMAL = 1,
	GBS_DISABLED = 2,
};

#endif //TMSCHEMA_H

#endif //_TnbDOXYGEN



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 _SELECTANY bool s_isCheckedThemeDll = false; ///< DLL���Ȃ����Ƃ��킩��ƁA ture�ɂȂ�܂��B
#endif



/**@ingroup WINAPI
 * �e�[�}�X�^�C��WINDOW�p�[�c�`��N���X
 *
 *		���݂̃e�[�}�ɂ������p�[�c��`�悵�܂��B
 *
 *	@todo ���� Button�ȊO�͕W���̂��̂��`�悳��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbThemeStylePartsDrawer.h
 * 
 *	@date 07/12/21 �V�K�쐬
 *	@date 08/02/21 DrawButtonControl() �̖߂�l�Ɍ��B�C���B
 */
class CThemeStylePartsDrawer : public CStandardPartsDrawer 
{
	DEFSUPER(CStandardPartsDrawer);
	/// �e�[�}DLL���錾
	#define _TnbTHEMEDLL_NAME "UxTheme.dll"
	mutable HTHEME	m_hTheme;
	typedef HRESULT(WINAPI *P_DrawThemeBackground)(HTHEME, HDC, int, int, const RECT *, const RECT *);
	CDynamicFuncT<P_DrawThemeBackground> m_bkgd;
	/// �`�F�b�N&Open
	bool m_CheckOpen(HDC hdc) const
	{
		if ( m_hTheme == NULL && IsThemeActive() )
		{
			m_hTheme = OpenThemeData(::WindowFromDC(hdc), VSCLASS_BUTTON);
		}
		return m_hTheme != NULL;
	}
public:

	/// �R���X�g���N�^
	CThemeStylePartsDrawer(void) : m_hTheme(NULL)
	{
		if ( ! s_isCheckedThemeDll )
		{
			if ( ! m_bkgd.LoadFunc(_TnbTHEMEDLL_NAME, "DrawThemeBackground") )
			{
				s_isCheckedThemeDll = true;
			}
		}
	}

	/// �f�X�g���N�^
	~CThemeStylePartsDrawer(void)
	{
		ThemeChanged();
	}

	/**
	 * [�ݒ�] �e�[�}�ύX.
	 *	@note WM_THEMECHANGED ���󂯂���A�{���\�b�h���R�[������悤�ɂ��Ă��������B
	 */
	void ThemeChanged(void)
	{
		if ( m_hTheme != NULL )
		{
			CloseThemeData(m_hTheme);
			m_hTheme = NULL;
		}
	}

	/**
	 * [�`��] �{�^���R���g���[���`��.
	 *		�w�肳�ꂽ�^�C�v�ƃX�^�C���������A�{�^���R���g���[����`�悵�܂��B
	 *	@param[in] hdc		�R���g���[���̕`��Ɏg�������f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *						NULL ���w�肷��ƁA���ۂɕ`�悷��傫���� lprc �ɕԂ��܂��B
	 *	@param[in,out] _lprc	�����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B 
	 *	@param[in] uState	�t���[���R���g���[���̏�����Ԃ��w�肵�܂��B IPartsDrawable::DrawButtonControl() �Q�ƁB
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawButtonControl(HDC hdc, LPRECT _lprc, UINT uState) const
	{
		if ( ! m_bkgd.HasFunc() || ! m_CheckOpen(hdc) )
		{
			#ifdef _TnbTHEME_ClassicOriginal
				uState &= ~DFCS_HOT;
			#endif
			return _super::DrawButtonControl(hdc, _lprc, uState);
		}
		int iPartId = -9;
		int iStateId = -9;
		switch ( uState & 0x001F )
		{
		case DFCS_BUTTONPUSH:
			iPartId = BP_PUSHBUTTON;
			iStateId = PBS_NORMAL; 
			break;
		case DFCS_BUTTONCHECK:
			iPartId = BP_CHECKBOX;
			if ( (uState & DFCS_CHECKED) == 0 )
			{
				iStateId = CBS_UNCHECKEDNORMAL;
			}
			else
			{
				iStateId = CBS_CHECKEDNORMAL;
			}
			break;
		case DFCS_BUTTON3STATE:
			iPartId = BP_CHECKBOX;
			if ( (uState & DFCS_CHECKED) == 0 )
			{
				iStateId = CBS_UNCHECKEDNORMAL;
			}
			else
			{
				iStateId = CBS_MIXEDNORMAL;
			}
			break;
		case DFCS_BUTTONRADIO:
			iPartId = BP_RADIOBUTTON;
			if ( (uState & DFCS_CHECKED) == 0 )
			{
				iStateId = RBS_UNCHECKEDNORMAL;
			}
			else
			{
				iStateId = RBS_CHECKEDNORMAL;
			}
			break;
		}
		if ( iPartId < 0 || iStateId < 0 )
		{
			return false;
		}
		if ( (uState & DFCS_HOT) != 0 )
		{
			iStateId += 1;
		}
		else if ( (uState & DFCS_PUSHED) != 0 )
		{
			iStateId += 2;
		}
		else
		if ( (uState & DFCS_INACTIVE) != 0 )
		{
			iStateId += 3;
		}
		//
		switch ( uState & 0xFF )
		{
		case DFCS_BUTTON3STATE:
		case DFCS_BUTTONCHECK:
		case DFCS_BUTTONRADIO:
			_lprc->right = _lprc->left + 13;
			_lprc->bottom = _lprc->top + 13;
			break;
		}
		if ( hdc == NULL ) { return true; }
		HRESULT r = m_bkgd()(m_hTheme, hdc, iPartId, iStateId, _lprc, _lprc);
		if ( r != S_OK )
		{
			CloseThemeData(m_hTheme);
			m_hTheme = NULL;
			if ( ! m_CheckOpen(hdc) )
			{
				#ifdef _TnbTHEME_ClassicOriginal
					uState &= ~DFCS_HOT;
				#endif
				return _super::DrawButtonControl(hdc, _lprc, uState);
			}
			r = m_bkgd()(m_hTheme, hdc, iPartId, iStateId, _lprc, _lprc);
		}
		if ( r == S_OK && m_hTheme != NULL && iPartId == BP_PUSHBUTTON )
		{
			POINT po;
			GetEdgeOffset(po);
			_lprc->left += po.x;
			_lprc->right -= po.x;
			_lprc->top += po.y;
			_lprc->bottom -= po.y;
		}
		return r == S_OK;
	}


	//--------------------------


	/**
	 * [�m�F] Tests if a visual style for the current application is active.
	 *	@retval true	 A visual style is enabled, and windows with visual styles applied should call OpenThemeData to start using theme drawing services. 
	 *	@retval lalse	A visual style is not enabled, and the window message handler does not need to make another call to IsThemeActive until it receives a WM_THEMECHANGED message. 
	 */
	static bool IsThemeActive(void)
	{
		if ( ! s_isCheckedThemeDll )
		{
			try
			{
				typedef BOOL(WINAPI *P)(void);
				return !! CDynamicFuncT<P>(_TnbTHEMEDLL_NAME, "IsThemeActive")()();
			}
			catch(CNullPointerException& e)
			{
				ASSERT0(false, "CThemeStylePartsDrawer", "API��������܂���B");
				e.OnCatch();
				s_isCheckedThemeDll = true;
			}
		}
		return false;
	}

	/**
	 * [����] Opens the theme data for a window and its associated class.
	 *	@param hWnd			Handle of the window for which theme data is required.
	 *	@param pszClassList	Pointer to a string that contains a semicolon-separated list of classes.
	 *	@return OpenThemeData tries to match each class, one at a time, to a class data section in 
	 *			the active theme. If a match is found, an associated HTHEME handle is returned. 
	 *			If no match is found NULL is returned.
	 */
	static HTHEME OpenThemeData(HWND hWnd, LPCWSTR pszClassList)
	{
		typedef HTHEME(WINAPI *P)(HWND, LPCWSTR);
		try
		{
			return CDynamicFuncT<P>(_TnbTHEMEDLL_NAME, "OpenThemeData")()(hWnd, pszClassList);
		}
		catch(CNullPointerException& e)
		{
			ASSERT0(false, "CThemeStylePartsDrawer", "API��������܂���B");
			e.OnCatch();
		}
		return NULL;
	}

	/**
	 * [����] Closes the theme data handle.
	 *	@param hTheme Handle to a window's specified theme data. Use OpenThemeData to create an HTHEME. 
	 *	@retval true ����.
	 *	@retval false ���s.
	 */ 
	static HRESULT CloseThemeData(HTHEME hTheme)
	{
		try
		{
			typedef HRESULT(WINAPI *P)(HTHEME);
			return CDynamicFuncT<P>(_TnbTHEMEDLL_NAME, "CloseThemeData")()(hTheme);
		}
		catch(CNullPointerException& e)
		{
			ASSERT0(false, "CThemeStylePartsDrawer", "API��������܂���B");
			e.OnCatch();
		}
		return E_FAIL;
	}

	/**
	 * [�`��] Draws the background image defined by the visual style for the specified control part.
	 *	@param hTheme	Handle to a window's specified theme data. Use OpenThemeData to create an HTHEME.
	 *	@param hdc		Handle to a device context (HDC) used for drawing the theme-defined background image.
	 *	@param iPartId	Value of type int that specifies the part to draw. See Parts and States.
	 *	@param iStateId	Value of type int that specifies the state of the part to draw. See Parts and States.
	 *	@param pRect	Pointer to a RECT structure that contains the rectangle, in logical coordinates, in which the background image is drawn. 
	 *	@param pClipRect	Pointer to a RECT structure that contains a clipping rectangle. This parameter may be set to NULL.
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	static HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect)
	{
		try
		{
			return CDynamicFuncT<P_DrawThemeBackground>(_TnbTHEMEDLL_NAME, "DrawThemeBackground")()(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
		}
		catch(CNullPointerException& e)
		{
			ASSERT0(false, "CThemeStylePartsDrawer", "API��������܂���B");
			e.OnCatch();
		}
		return E_FAIL;
	}
};



}; //MFC
}; //TNB



