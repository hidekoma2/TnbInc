#pragma once
/**
 *@file
 * DIPSW ���{�^���R���g���[���֌W�̃w�b�_
 *
 *		4��sw�����ԃR���g���[�����쐬���邽�߂̃t���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingStatic.h"
#include "TnbBitmapImage.h"



#define DIPSW_CHANGED_T			0x100



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * DIPSW ���{�^���R���g���[���N���X.
 *
 *		�X���C�h�X�C�b�`��4�����A0x00�`0x0F �܂ł̒l��\�����Ƃ��o���܂��B
 *
 *	@par�g����
 *		�}�W�b�N�J���[�R�[�h�Ń{�^�����������r�b�g�}�b�v���ꖇ�p�ӂ��܂��B
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		�ŏ��� SetImage() ���Ă��������B
 *
 *	@note �N���b�N�ɂ�� DIPSW �̏�Ԃ��ω��������A DIPSW_CHANGED_T ���A�e�ɒʒm���Ă��܂��B
 *
 *	@par�g�p��
 *		\code
 *		//= �錾
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_CONTROL(DIPSW_CHANGED_T, IDC_STATIC_DIPSW, OnChangedDipsw)
 *			:
 *		\endcode
 *
 *	@todo �t�H�[�J�X���󂯂܂���B����ăL�[�{�[�h�ł̑���͏o���܂���B
 *		
 *	@par�K�v�t�@�C��
 *			TnbMfcDipswButton.h
 * 
 *	@date 16/07/26 �V�K�쐬
 */
class CDipswButton : public CDrawingStatic
{
	DEFSUPER(CDrawingStatic);
public:

	/**
	 * �}�W�b�N�J���[�R�[�h.
	 */
	enum EMagicColorCode
	{
		MCC0 = RGB(128, 0, 0),		///< �Â���
		MCC1 = RGB(255, 0, 0),		///< ��
		MCC2 = RGB(0, 0, 128),		///< �Â���
		MCC3 = RGB(0, 0, 255),		///< ��
		MCC4 = RGB(0, 128, 0),		///< �Â���
		MCC5 = RGB(0, 255, 0),		///< ��
		MCC6 = RGB(0, 128, 128),	///< �Â����F
		MCC7 = RGB(0, 255, 255),	///< ���F
	};

	/// �R���X�g���N�^
	CDipswButton(void) : m_value(0x00)
	{
	}

	/**
	 * [�ݒ�] �C���[�W�w��.
	 *	@param b �x�[�X�摜�Bsw�̕������}�W�b�N�J���[�R�[�h�ŏ�����Ă�����́B
	 *	@param onColor ON�̍ۂ̐F�B�}�W�b�N�J���[�R�[�h�ȊO�̐F�ɂ��邱�ƁB
	 *	@param offColor OFF�̍ۂ̐F�B�}�W�b�N�J���[�R�[�h�ȊO�̐F�ɂ��邱�ƁB
	 */
	void SetImage(CBitmapHandle b, COLORREF onColor = RGB(255, 255, 255), COLORREF offColor = RGB(1, 1, 1))
	{
		m_onColor = onColor;
		m_offColor = offColor;
		m_base = b;
		m_base.ToHaveRgbData();
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
			{
				CPoint po(lParam);
				int z = m_CheckPoint(po);
				if ( z >= 0 )
				{
					BYTE b = m_value;
					m_value ^= _BIT(z);
					if ( b != m_value )
					{
						m_ResetBitmap();
						MFCLIB::SendCommandMessage(this, DIPSW_CHANGED_T);
					}
				}
			}
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// �`�F�b�N�|�C���g @param po �`�F�b�N����N���C�A���g���W�B
	int m_CheckPoint(CPoint po)
	{
		CSize sz = m_base.GetSize();
		if ( sz.cy <= po.y || sz.cx <= po.x )
		{
			return -1;
		}
		const RGBQUAD* pRgb = m_base.GetRgbDataPtr(po.y);
		RGBQUAD q = pRgb[po.x];
		switch ( RGB(q.rgbRed, q.rgbGreen, q.rgbBlue) )
		{
		case MCC0:
		case MCC1:
			return 0;
		case MCC2:
		case MCC3:
			return 1;
		case MCC4:
		case MCC5:
			return 2;
		case MCC6:
		case MCC7:
			return 3;
		default:
			break;
		}
		return -1;
	}

	/// �摜�č\�z
	void m_ResetBitmap(void)
	{
		CBitmapImage bi;
		bi.Set(m_base);
		COLORREF on = m_onColor;
		COLORREF of = m_offColor;
		bi.ChangePixelColor(MCC0, (m_value & _BIT(0)) ? on : of);
		bi.ChangePixelColor(MCC1, (m_value & _BIT(0)) ? of : on);
		bi.ChangePixelColor(MCC2, (m_value & _BIT(1)) ? on : of);
		bi.ChangePixelColor(MCC3, (m_value & _BIT(1)) ? of : on);
		bi.ChangePixelColor(MCC4, (m_value & _BIT(2)) ? on : of);
		bi.ChangePixelColor(MCC5, (m_value & _BIT(2)) ? of : on);
		bi.ChangePixelColor(MCC6, (m_value & _BIT(3)) ? on : of);
		bi.ChangePixelColor(MCC7, (m_value & _BIT(3)) ? of : on);
		_super::SetDrawingBitmap(bi, true, true);
	}

	CBitmapImage	m_base;			///< ���G. @note �}�W�b�N�J���[�R�[�h�ŏ�����Ă��邱�ƁB
	COLORREF		m_onColor;		///< ON�̍ۂ̐F. @note �}�W�b�N�J���[�R�[�h�ȊO�̐F�ɂ��邱��
	COLORREF		m_offColor;		///< OFF�̍ۂ̐F. @note �}�W�b�N�J���[�R�[�h�ȊO�̐F�ɂ��邱��
	BYTE			m_value;		///< �l	
};



}; //MFC
}; //TNB



