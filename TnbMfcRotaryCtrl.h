#pragma once
/**
 *@file
 * ���[�^���[���l�R���g���[���֌W�̃w�b�_
 *
 *		0�`15�܂ł̃��[�^���[�R���g���[�����쐬���邽�߂̃t���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingStatic.h"
#include "TnbBitmapImage.h"
#include "TnbGdiPlus.h"



#define ROTARY_CHANGED_T			0x100



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ���[�^���[���l�R���g���[���N���X.
 *
 *		���[�^���[�������A0x00�`0x0F �܂ł̒l��\�����Ƃ��o���܂��B
 *
 *	@par�g����
 *		�l�̏����ꂽ�r�b�g�}�b�v�ƁA0�̌����̖��̏����ꂽ�r�b�g�}�b�v���Q���p�ӂ��܂��B
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		�ŏ��� SetImage() ���Ă��������B
 *
 *	@note �N���b�N�ɂ�胍�[�^���[�̏�Ԃ��ω��������A ROTARY_CHANGED_T ���A�e�ɒʒm���Ă��܂��B
 *
 *	@par�g�p��
 *		\code
 *		//= �錾
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_CONTROL(ROTARY_CHANGED_T, IDC_STATIC_DIPSW, OnChangedDipsw)
 *			:
 *		\endcode
 *
 *	@todo �t�H�[�J�X���󂯂܂���B����ăL�[�{�[�h�ł̑���͏o���܂���B
 *		
 *	@par�K�v�t�@�C��
 *			TnbMfcRotaryCtrl.h
 * 
 *	@date 16/07/26 �V�K�쐬
 */
class CRotaryCtrl : public CDrawingStatic
{
	DEFSUPER(CDrawingStatic);
public:

	/// �R���X�g���N�^
	CRotaryCtrl(void) : m_value(0x00)
	{
	}

	/**
	 * [�ݒ�] �C���[�W�w��.
	 *	@param b �x�[�X�摜.
	 *	@param a ���摜.
	 *	@param transColor ���摜�̓��ߐF.
	 */
	void SetImage(CBitmapHandle b, CBitmapHandle a, COLORREF transColor = RGB(255, 0, 255))
	{
		m_baseImage = b;
		m_arrowImage = a;
		m_arrowTransColor = transColor;
		m_ResetBitmap();
	}

	/**
	 * [�擾] �l�擾.
	 *	@return �l�B
	 */
	BYTE GetValue(void) const
	{
		return m_value;
	}

	/**
	 * [�ݒ�] �l�C���N�������g.
	 */
	void Increment(void)
	{
		SetValue(static_cast<BYTE>((m_value + 1) & 0xF));
	}

	/**
	 * [�ݒ�] �l�f�N�������g.
	 */
	void Decrement(void)
	{
		SetValue(static_cast<BYTE>((m_value - 1) & 0xF));
	}

	/**
	 * [�ݒ�] �l�ݒ�.
	 *	@param value �l�B
	 */
	void SetValue(BYTE value)
	{
		m_value = value;
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			m_ResetBitmap();
		}
	}

protected:
	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		m_ResetBitmap();
	}

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
		switch ( message )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			m_value++;
			m_value &= 0xF;
			m_ResetBitmap();
			SetFocus();
			MFCLIB::SendCommandMessage(this, ROTARY_CHANGED_T);
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			m_value--;
			m_value &= 0xF;
			m_ResetBitmap();
			SetFocus();
			MFCLIB::SendCommandMessage(this, ROTARY_CHANGED_T);
			break;
		case WM_MOUSEWHEEL:
			if ( static_cast<short>(HIWORD(wParam)) > 0 )
			{
				m_value++;
			}
			else
			{
				m_value--;
			}
			m_value &= 0xF;
			m_ResetBitmap();
			SetFocus();
			MFCLIB::SendCommandMessage(this, ROTARY_CHANGED_T);
			break;
		case WM_GETDLGCODE:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				return (r & ~DLGC_STATIC);
			}
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// �摜�č\�z
	void m_ResetBitmap(void)
	{
		CBitmapImage base;
		base.Set(m_baseImage);
		if ( base.IsEmpty() )
		{
			return;
		}
		m_size = base.GetSize();
		CBitmapImage arrow;
		arrow.Set(m_size.cx, m_size.cy, m_arrowTransColor);
		{
			CBitmapHandle h = m_arrowImage;
			CGdiPlus g(arrow.GetDC());
			g.SetImageInterpolationMode(CGdiPlus::NEAREST_NEIGHBOR);
			g.DrawRotateImage(CGdiPlus::CPos(m_size.cx / 2, m_size.cy / 2), h, (360.0 / 16.0) * m_value);
		}
		arrow.ReleaseDC();
		base.InsertOnTransparent(0, 0, arrow);
		_super::SetDrawingBitmap(base, true, true);
	}

	CBitmapHandle	m_baseImage;		///< �x�[�X�摜
	CBitmapHandle	m_arrowImage;		///< ���摜
	COLORREF		m_arrowTransColor;	///< ���ߐF
	BYTE			m_value;			///< �l
	SIZE			m_size;				///< �R���g���[���T�C�Y
};



}; //MFC
}; //TNB
