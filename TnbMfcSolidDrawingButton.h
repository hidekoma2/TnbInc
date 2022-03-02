#pragma once
/**
 *@file
 * ���̕`����{�^���֌W�̃w�b�_
 *
 *		�`����̌`��ɂ��킹���O���t�B�J���ȃ{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapDrawer.h"
#include "TnbMfcRgnButton.h"
#include "TnbSolidBitmapMaker.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * ���̕`����{�^���R���g���[��
 *
 *		�{�N���X�ɕ`�����ݒ�( SetAllDrawers() )����ƁA
 *		�u�ʏ��ԁv�u������ԁv�u������ԁv�u�z�o�[��ԁv�u�t�H�[�J�X��ԁv�̏�Ԃ��쐬���A
 *		�g�p���܂��B���̍ہA�{�^���炵���A���́i�ł��΂� or �ւ����݁j�����܂�
 *		�i���̂��߁A���`���񂩂�c���A���ꂼ�� +3 dot �����{�^���ɂȂ�܂��j�B
 *
 *		- [�ʏ���]\n	�`����������Â������摜���g�p���܂��B
 *		- [�������]\n	�`������O���[�������摜���g�p���܂��B
 *		- [�z�o�[���]
 *		- [�������]\n	�`����A���̂܂܎g�p���܂��B
 *		- [�t�H�[�J�X���]\n	�z�o�[��ԂƓ����摜���g�p���܂��B
 *
 *	@note �L���A�����A�\���A��\���́A MFC �� CButton �Ɠ����悤�ɑ���ł��܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�����X�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton or CStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		\code
 *			[.h �t�@�C��]
 *
 *			class CFooDlg : public CDialog {
 *					;
 *				CSolidDrawingButton	m_buttonExit; // CButton ����ύX�B
 *
 *			[.cpp �t�@�C��]
 *
 *			BOOL CFooDlg::OnInitDialog() {
 *					;
 *				CFont f;
 *				f.CreatePointFont(220, "Smudger LET");	// �t�H���g�쐬	
 *				CRect rc;
 *				m_buttonCancel.GetWindowRect(rc);
 *				CTextDrawer td (rc.Size(), f, RGB(0, 1, 170), "EXIT");	//�R���g���[���̑傫�����g�p���ĕ`����쐬
 *				//	//CTextDrawer td (f, RGB(0, 1, 170), "EXIT");	���t�H���g�̑傫������`����쐬���ł���)
 *				//	//td.AdjustSize(); 
 *				m_buttonExit.SetAllDrawers(td, RGB(255, 0, 255)); // �ݒ�
 *					;
 *
 *		\endcode	
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSolidDrawingButton.h
 * 
 *	@date 09/11/12 �V�K�쐬
 */
class CSolidDrawingButton : public CRgnButton
{
	DEFSUPER(CRgnButton);
public:

	/**
	 * [�ݒ�] �r�b�g�}�b�v�ݒ�.
	 *		��̕`������w�肷��ƁA��������A�u�ʏ��ԁv�u�z�o�[��ԁv�u������ԁv[������ԁv�u�t�H�[�J�X��ԁv��
	 *		�{�^�����쐬���܂��B
	 *	@note �_�C�A���O�\����A�C�x���g�������ɁA�{���\�b�h���g���ăr�b�g�}�b�v���Đݒ肷�邱�Ƃ��\�ł��B
	 *	@param draw �`����B�{���\�b�h���ŁA���̕`���񂩂�摜���쐬���܂��̂ŁA�`����͔j�����Ă����܂��܂���B
	 *	@param transColor ���ߐF�w��.
	 *	@param persent �u�ʏ��ԁv�̐F���������B�O�Ȃ�u�t�H�[�J�X��ԁv�Ɠ������邳�ɂȂ�܂��B100�Ȃ�A�^�����ɂȂ��Ă��܂��܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetAllDrawers(const IDrawable& draw, COLORREF transColor, int persent = 30)
	{
		CBitmapImage base = CBitmapDrawer::ToBitmap(draw, transColor);
		CSize sz = base.GetSize();
		//
		HRGN hRgn = base.CreateRgn(transColor);
		CSolidBitmapMaker sbm;
		sbm.SetTransColor(transColor);
		sbm.SetBaseColor(transColor);
		//
		CBitmapImage normalBmp;					// �m�[�}��
		{
			CBitmapImage mask;
			mask.Set(sz.cx, sz.cy, RGB(0, 0, 0));
			normalBmp.Set(base);
			HDC hDC = normalBmp.GetDC();
			::SelectClipRgn(hDC, hRgn);
			mask.AlphaBlend(hDC, 0, 0, persent);
			normalBmp.ReleaseDC();
			normalBmp = sbm.Make(normalBmp);
		}
		CBitmapImage hoverBmp = sbm.Make(base);	// �z�o�[���t�H�[�J�X
		CBitmapImage pressBmp = sbm.Make(base, true);// �v���X
		CBitmapImage disableBmp;					// ����
		{
			CBitmapImage b;
			b.Set(base);
			b.ChangePixelColor(RGB(85,85,85), RGB(85,85,85), RGB(85,85,85));
			disableBmp.Set(base);
			HDC hDC = disableBmp.GetDC();
			::SelectClipRgn(hDC, hRgn);
			b.BitBlt(hDC, 0, 0);
			disableBmp.ReleaseDC();
			disableBmp = sbm.Make(disableBmp);
		}
		_DeleteObject(hRgn);
		//
		sz = hoverBmp.GetSize();
		CBitmapImage bi;
		bi.Set(sz.cx * 5, sz.cy);
		bi.Insert(sz.cx * 0, 0, normalBmp);
		bi.Insert(sz.cx * 1, 0, hoverBmp);
		bi.Insert(sz.cx * 2, 0, pressBmp);
		bi.Insert(sz.cx * 3, 0, disableBmp);
		bi.Insert(sz.cx * 4, 0, hoverBmp);
		return _super::SetAllBitmaps(bi, transColor);
	}

private:
	/// [�ݒ�] �x�[�X�r�b�g�}�b�v�w��.
	bool SetBitmap(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT);
	/// [�ݒ�] ������ԃr�b�g�}�b�v�w��.
	void SetPushedBitmap(CBitmapHandle bmp);
	/// [�ݒ�] ������ԃr�b�g�}�b�v�w��.
	void SetDisableBitmap(CBitmapHandle bmp);
	/// [�ݒ�] �z�o�[��ԃr�b�g�}�b�v�w��.
	void SetHoverBitmap(CBitmapHandle bmp);
	/// [�ݒ�] �r�b�g�}�b�v�w��.
	bool SetAllBitmaps(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT);
};



}; //MFC
}; //TNB



