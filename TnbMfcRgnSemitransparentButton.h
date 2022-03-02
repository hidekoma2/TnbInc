#pragma once
/**
 *@file
 * ���R�^�������r�b�g�}�b�v�{�^���֌W�̃w�b�_
 *
 *		�r�b�g�}�b�v�̌`��ɂ��킹���O���t�B�J���ȃ{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcRgnButton.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * ���R�^�������r�b�g�}�b�v�{�^���R���g���[��
 *
 *		�r�b�g�}�b�v�̌`�ɍ��킹���{�^�����쐬���邱�Ƃ��o���܂��B�C�ӂ̏�Ԃ𔼓����ɂ��邱�Ƃ��o���܂��B
 *		��{�I�Ȏg�����́A�X�[�p�[�N���X�� CRgnButton ���Q�Ƃ��Ă��������B
 *
 *	@note	�������w��̓r�b�g�P�ʂōs���܂��B bit0���u�ʏ��ԁv�Abit1���u�z�o�[��ԁv�Abit2���u������ԁv�A
 *			bit3��[������ԁv�A	bit4���u�t�H�[�J�X��ԁv�Ɋ��蓖�Ă��Ă���A 1 �ɂȂ��Ă���ƁA�������ɂȂ�܂��B
 *			�Ⴆ�΁A 8 ���w�肷��ƁA�u������ԁv�̃{�^���������������ɐ���܂��B
 *
 *	@attention �e�̃_�C�A���O�� WM_PRINTCLIENT ���b�Z�[�W�𐳂����������Ă���K�v������܂��B
 *
 *	@attention ���߂Ă̕`��̍ۂɁA�R���g���[�����̔w�i�Ɣ������������s���A�ێ����܂��B���̂��߁A
 *				�{�^���̃r�b�g�}�b�v��w�i���r���ŕω�������ƁA���҂���\���ɂ͂Ȃ�܂���B���̏ꍇ�A
 *				�S�r�b�g�}�b�v�{�^�����Đݒ肵�A ReCalc() ���g�p���邱�ƂŁA�č������s�����Ƃ��o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�����X�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton or CStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@param MODE �������w��B �ȗ������ 0x1F �ɂȂ�܂��B
 *	@param PAR �������x�B �O%���Ɣw�i�̂݁A 100% ���Ɠ����ɂȂ�܂���B
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcRgnSemitransparentButton.h
 * 
 *	@date 07/10/24 �V�K�쐬
 *	@date 07/10/30 ���p���B������ Windows2000�ȍ~�̂ݓ���B
 *	@date 07/10/30 �������B
 */
template<BYTE MODE = 0x1F, DWORD PAR = 50>
class CRgnSemitransparentButtonT : public CRgnButton
{
	DEFSUPER(CRgnButton);
	BYTE		m_semitransMode;		///< ���߂����Ԏw�� bit�Ŏw��
	DWORD		m_parsent;				///< �����x
	bool		m_boIsDoGetParentImage;	///< ���߉摜�쐬�w���t���O
	bool		m_boIsDoIniting;		///< ���������t���O
	/// ����������
	void m_Compose(CBitmapHandle& _hh, const CBitmapImage& parentBitmap)
	{
		CBitmapImage bi;
		bi.Attach(_hh);
		bi.InsertOnAlphaBlend(0, 0, parentBitmap, 100 - m_parsent);
		_hh = bi.GetBitmapHandle();
	}
	/// ����������
	void m_AllComp(const CBitmapImage& parentBitmap)
	{
		loop ( i, 4 )
		{
			if ( (m_semitransMode & _BIT(i)) != 0 )
			{
				m_Compose(_super::m_bmpButtons[i], parentBitmap);
			}
		}
		if ( (m_semitransMode & _BIT(4)) != 0 && ! IS_RGBVALUE(m_focusMaskColor) )
		{
			CBitmapHandle h = m_focusBitmap.GetBitmapHandle();
			m_Compose(h, parentBitmap);
			m_focusBitmap.Attach(h);
		}
	}
protected:

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_PAINT )
		{
			if ( m_boIsDoIniting ) { return 0; }
			if ( m_boIsDoGetParentImage )
			{
				LRESULT rr = _super::WindowProc(message, wParam, lParam);
				m_boIsDoIniting = true;
				m_boIsDoGetParentImage = false;
#if 0
				CRect rect;
				GetWindowRect(&rect);
				CWnd* pWnd = GetParent();
				ASSERTLIB( pWnd != NULL );
				DWORD dwMod = 0;
				if ( (pWnd->GetExStyle() & WS_EX_LAYERED) == 0 )
				{
					MFCLIB::SetLayeredWindow(pWnd, 0, 255, MFCLIB::E_Alpha | MFCLIB::E_SetLayeredStyle);
					dwMod = WS_EX_LAYERED;
				}
				pWnd->ScreenToClient(&rect);
				pWnd->RedrawWindow(&rect);
				CDC* pDC = pWnd->GetDC();
				ASSERT( pDC != NULL );
				CBitmapImage parentBitmap;
				parentBitmap.Set(*pDC, rect);
				pWnd->ReleaseDC(pDC);
				m_AllComp(parentBitmap);
				pWnd->ModifyStyleEx(dwMod, 0);
#else
				CWnd* pWnd = _super::GetParent();
				CBitmapImage bi;
				bi.SetFromClient(*pWnd);
				CRect rect;
				_super::GetWindowRect(rect);
				pWnd->ScreenToClient(rect);
				m_AllComp(bi.Cut(rect));
#endif
				m_boIsDoIniting = false;
				return rr;
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
public:

	/**
	 * �R���X�g���N�^
	 *	@param semitransMode �������w��B
					�ȗ�����ƁA�e���v���[�g�Ŏw�肵���l�ɂȂ�܂��B
	 *	@param parsent �������x�B �O%���Ɣw�i�̂݁A 100% ���Ɠ����ɂȂ�܂���B
					�ȗ�����ƁA�e���v���[�g�Ŏw�肵���l�ɂȂ�܂��B
	 */
	CRgnSemitransparentButtonT(BYTE semitransMode = MODE, DWORD parsent = PAR) 
		: _super(), m_boIsDoGetParentImage(true), m_boIsDoIniting(false)
		, m_semitransMode(semitransMode), m_parsent(parsent)
	{
	}

	/**
	 * [�ݒ�] �č���.
	 *	@note	���߂Ă̕`��̍ۂɁA�R���g���[�����̔w�i�Ɣ������������s���A�ێ����܂��B���̂��߁A
	 *			�{�^���̃r�b�g�}�b�v��w�i���r���ŕω�������ƁA���҂���\���ɂ͂Ȃ�܂���B���̏ꍇ�A
	 *			�S�r�b�g�}�b�v�{�^�����Đݒ肵�A �{���\�b�h���g�p���邱�ƂŁA�č������s�����Ƃ��o���܂��B
	 */
	void ReCalc(void)
	{
		m_boIsDoGetParentImage = true;
	}
};



}; //MFC
}; //TNB



