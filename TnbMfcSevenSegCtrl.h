#pragma once
/**
 *@file
 * 7SEG�\���R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingStatic.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 7SEG ���l�\���R���g���[���N���X.
 *
 *		7SEG�̕\���łO�`�X�̒l��\�����Ƃ��o���܂��B������ DIGIT �Ŏw�肵�܂��B
 *
 *	@par�g����
 *		�}�W�b�N�J���[�R�[�h�Ń{�^�����������r�b�g�}�b�v���ꖇ�p�ӂ��܂��B
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		�ŏ��� SetImage() ���Ă��������B
 *
 *		�}�W�b�N�J���[�R�[�h�� 7SEG �Ɉȉ��̂悤�ɑΉ����܂��B
 *		\code
 *				 000
 *				2   1
 *				 333
 *				5   4
 *				 666 7
 *		\endcode
 *	@param DIGIT ����
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSevenSegCtrl.h
 * 
 *	@date 16/07/26 �V�K�쐬
 *	@date 16/08/09 �p�����[�^ DIGIT ��V�K�ǉ��B�Q���ȏ�ɂ��Ή�����悤�ɂ����B
 */
template<int DIGIT = 1>
class CSevenSegCtrlT : public CDrawingStatic
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
	CSevenSegCtrlT(void)
	{
		Zero(m_value);
	}

	/**
	 * [�ݒ�] �C���[�W�w��.
	 *	@note �w���A�l��ݒ肵�Ȃ��ƃC���[�W���\������܂���B
	 *	@param b �x�[�X�摜�B7seg �̕������}�W�b�N�J���[�R�[�h�ŏ�����Ă�����́B
	 *	@param onColor ON�̍ۂ̐F�B�}�W�b�N�J���[�R�[�h�ȊO�̐F�ɂ��邱�ƁB
	 *	@param offColor OFF�̍ۂ̐F�B�}�W�b�N�J���[�R�[�h�ȊO�̐F�ɂ��邱�ƁB
	 */
	void SetImage(CBitmapHandle b, COLORREF onColor = RGB(254, 0, 0), COLORREF offColor = RGB(60, 0, 0))
	{
		m_baseImage = b;
		m_onColor = onColor;
		m_offColor = offColor;
	}

	/**
	 * [�ݒ�] �l�ݒ�.
	 *	@param value �l�B�L���͈͂� 0�`'(10^DIGIT)-1' �ɂȂ�܂��B
	 *	@param hasDot �h�b�g����Ȃ� true ���w��B
	 *	@param isZeroPadding true �Ȃ�[���p�f�B���O����B false �Ȃ�Ȃ��B
	 */
	void SetValues(DWORD value, bool hasDot = false, bool isZeroPadding = false)
	{
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			Zero(m_value);
			loop ( i, DIGIT )
			{
				BYTE v = m_NumberImage(value % 10);
				if ( i == 0 && hasDot )
				{
					v |= _BIT(7);
				}
				m_value[DIGIT - i - 1] = v;
				value /= 10;
				if ( ! isZeroPadding && value == 0 )
				{
					break;
				}
			}
			m_ResetBitmap();
		}
	}

	/**
	 * [�ݒ�] �l�ݒ�.
	 *		�e�����Ƃɐ��l�A�h�b�g���w��ł��܂��B
	 *	@param idx ���B
	 *	@param value �l�B
	 *	@param hasDot �h�b�g����Ȃ� true ���w��B
	 */
	void SetValue(INDEX idx, int value, bool hasDot = false)
	{
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			BYTE v = m_NumberImage(value);
			if ( hasDot )
			{
				v |= _BIT(7);
			}
			m_value[DIGIT - idx - 1] = v;
			m_ResetBitmap();
		}
	}

	/**
	 * [�ݒ�] 7seg�ݒ�.
	 *		�e�����ƂɊe�p�[�c�����R��ON/OFF�o���܂��B
	 *	\code
	 *			 000
	 *			2   1
	 *			 333
	 *			5   4
	 *			 666 7
	 *	\endcode
	 *	�Ⴆ�� _BIT(4) | _BIT(5) | _BIT(6) ���w�肷��Ə������� u �ɂȂ�܂��B
	 *	@param idx ���B
	 *	@param b �l�B
	 */
	void SetBitset(INDEX idx, BYTE b)
	{
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			m_value[DIGIT - idx - 1] = b;
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
	}

private:
	BYTE m_NumberImage(int val)
	{
		//		 000
		//		2   1
		//		 333
		//		5   4
		//		 666 7
		const static BYTE pats[11] = 
		{
			//   76543210
			_BIN(01110111),	//0
			_BIN(00010010),	//1
			_BIN(01101011),	//2
			_BIN(01011011),	//3
			_BIN(00011110),	//4
			_BIN(01011101),	//5
			_BIN(01111100),	//6
			_BIN(00010011),	//7
			_BIN(01111111),	//8
			_BIN(01011111),	//9
			_BIN(11101101),	//E
		};
		BYTE v = (val >= 0 && val < 10) ? pats[val] : pats[10] ;
		return v;
	}

	/// �摜�č\�z
	void m_ResetBitmap(void)
	{
		CSize sz;
		m_baseImage.GetSize(sz);
		CBitmapImage bmp;
		bmp.Set(sz.cx * DIGIT, sz.cy);
		loop ( i, DIGIT )
		{
			CBitmapImage b = m_ResetBitmap(m_value[i]);
			bmp.Insert(i * sz.cx, 0, b);
		}
		_super::SetDrawingBitmap(bmp, true, true);
	}

	/// �摜�č\�z
	CBitmapImage m_ResetBitmap(BYTE v)
	{
		CBitmapImage bi;
		bi.Set(m_baseImage);
		COLORREF on = m_onColor;
		COLORREF of = m_offColor;
		bi.ChangePixelColor(MCC0, (v & _BIT(0)) ? on : of);
		bi.ChangePixelColor(MCC1, (v & _BIT(1)) ? on : of);
		bi.ChangePixelColor(MCC2, (v & _BIT(2)) ? on : of);
		bi.ChangePixelColor(MCC3, (v & _BIT(3)) ? on : of);
		bi.ChangePixelColor(MCC4, (v & _BIT(4)) ? on : of);
		bi.ChangePixelColor(MCC5, (v & _BIT(5)) ? on : of);
		bi.ChangePixelColor(MCC6, (v & _BIT(6)) ? on : of);
		bi.ChangePixelColor(MCC7, (v & _BIT(7)) ? on : of);
		return bi;
	}

	BYTE			m_value[DIGIT];	///< �l

	CBitmapHandle	m_baseImage;	///< �x�[�X�摜.
	COLORREF		m_onColor;		///< �I�����̃J���[
	COLORREF		m_offColor;		///< �I�t���̃J���[
};



}; //MFC
}; //TNB
