#pragma once
/**
 *@file
 * �����_�t�����l��p����Edit�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcEditValue.h"
#include <math.h>
#include <wchar.h>



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN

/// 10�̗ݏ�萔�̃}�N���Q
template<UINT n> struct TPowerValue {
	enum {
		VALUE = TPowerValue<n - 1>::VALUE * 10
	};
};
template<> struct TPowerValue<0> {
	enum {VALUE = 1};
};
#define _POW10(X) (TPowerValue<X>::VALUE)

#endif //_TnbDOXYGEN



/**@ingroup MFCCONTROL
 * �����_�t�����l��pEdit�R���g���[���e���v���[�g
 *
 *		���l�œ��o�͂ł��܂��B
 *		�͈͂��w��ł��܂��B
 *		�X�e�b�v�����w��ł��܂��i��������_�͂O�j�B�����l�͍ŏ��l�ł�( POW �� 3 �Ȃ� 0.001)�B
 *		�X�s���R���g���[�������邱�Ƃ��o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃG�f�B�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCEdit�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@param POW �����_�ȉ��̌���.
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditFloat.h
 * 
 *	@date 08/01/02	�V�K�쐬
 *	@date 08/03/18	�͈̓`�F�b�N���@�C��
 *	@date 08/04/15	SetValue() ���ɔ͈̓`�F�b�N��ǉ��B
 */
template<int POW = 1>
class CEditFloatT : public CEditValueT<double, 0, 0>
{
	typedef CEditValueT<double, 0, 0> _super;
protected:
	enum { POWER = _POW10(POW) };
	/**
	 * [�m�F] ���͋��`�F�b�N
	 *	@param[in,out] _nChar WM_CHAR �� WPARAM
	 *	@param[in,out] _lFlags WM_CHAR �� LPARAM
	 *	@param[in] strNowText ���͍ς݂̕�����
	 *	@retval true ���͂�F�߂�B
	 *	@retval false ���͂�F�߂Ȃ��B
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		if ( _nChar == '.' && strNowText.Find('.') < 0 )
		{
			return true;
		}
		return _super::IsValidChar(_nChar, _lFlags, strNowText);
	}

	/**
	 * [�ʒm] ���͎��ɃR�[��
	 *	@note �Ȃɂ����Ȃ��B
	 */
	virtual void OnUpdateChar(void)
	{
	}

	/**
	 * [�ʒm] �t�H�[�J�X�����������ɃR�[��
	 *	@note	�͈̓`�F�b�N���A�����Ă�����ۂ߂��Ⴄ
	 */
	virtual void OnEditEnd(void)
	{
		CString s = _super::GetText();
		if ( ! s.IsEmpty() )
		{
			double d = _tcstod(s, NULL);
			double dd = d;
			double mx = static_cast<double>(m_xMax) / POWER;
			double mn = static_cast<double>(m_xMin) / POWER;
			if ( d > mx )
			{
				d = mx;
			}
			else if ( d < mn )
			{
				d = mn;
			}
			LONGLONG i = ToInteger(s);
			LONGLONG ii = AdjustValue(i);
			_super::SetText(ToString(ii));
			if ( i != ii || d != dd )
			{
				::MessageBeep(MB_ICONASTERISK); //���͐��������e���ω�������
			}
		}
	}

	/** 
	 * [�ϊ�] ���l���當����쐬.
	 *	@note �����p�̃��\�b�h�ł��B
	 *	@param ll ���l
	 *	@return ������
	 */
	virtual CString ToString(LONGLONG ll) const
	{
		double r = static_cast<double>(ll) / POWER;
		CString str;
		if ( m_iZeroPlace > 0 )
		{
			CString s;
			s.Format(_T("%%0%d.%df"), m_iZeroPlace, POW); 
			str.Format(s, r);
		}
		else
		{
			CString s;
			s.Format(_T("%%.%df"), POW); 
			str.Format(s, r);
		}
		return str;
	}
	
	/** 
	 * [�ϊ�] �����񂩂琔�l�쐬.
	 *	@note �����p�̃��\�b�h�ł��B
	 *	@param s ������
	 *	@return ���l
	 */
	virtual LONGLONG ToInteger(LPCTSTR s) const
	{
		double d = _tcstod(s, NULL) * POWER;
		d += (d < 0.0) ? -0.5 : 0.5;
		return static_cast<LONGLONG>(d);
	}

public:
	
	/// �R���X�g���N�^
	CEditFloatT(void)
	{
		m_xMin = 0;
		m_xMax = 0;
		m_step = 1;
	}

	/**
	 * [�ϊ�] �l������
	 *	@param xValue �ϊ�����l
	 *	@return �ϊ���̕�����
	 */
	virtual CString ValueToString(double xValue) const
	{
		double d = xValue * POWER;
		d += (d < 0.0) ? -0.5 : 0.5;
		LONGLONG l = AdjustValue(static_cast<LONGLONG>(d));
		return ToString(l);
	}

	/**
	 * [�ݒ�] �l�ݒ�
	 *	@param xValue �ݒ肷��l
	 */
	virtual void SetValue(double xValue)
	{
		_super::SetText(ValueToString(xValue));
	}

	/**
	 * [�擾] �l�擾
	 *	@param[out] _xValue �ݒ肳��Ă���l
	 *	@retval true �l�L��B
	 *	@return false Empty�B
	 */
	virtual bool GetValue(double& _xValue) const
	{
		CString s = _super::GetText();
		if ( s.IsEmpty() )
		{
			return false;
		}
		_xValue = static_cast<double>(AdjustValue(ToInteger(s))) / POWER;
		return true;
	}
	
	/**
	 * [�ݒ�] �͈͎w��
	 *	@param xMin �ŏ��l
	 *	@param xMax �ő�l
	 *	@param step �X�e�b�v���B�ȗ�����ƍŏ��l( POW �� 3 �Ȃ� 0.001)�ɂȂ�܂��B
	 */
	virtual void SetRange(double xMin, double xMax, double step = 0.0)
	{
		m_xMin = static_cast<LONGLONG>(xMin * POWER);
		m_xMax = static_cast<LONGLONG>(xMax * POWER);
		if ( m_xMin > m_xMax )
		{
			Swap(m_xMin, m_xMax);
		}
		if ( step <= 0.0 )
		{
			step = 1.0 / POWER;
		}
		m_step = static_cast<int>(step * POWER);
	}
};



}; //MFC
}; //TNB

