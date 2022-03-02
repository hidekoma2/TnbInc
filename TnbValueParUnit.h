#pragma once
/**
 *@file
 * �P�ʕt�l�Ǘ��֌W�̃w�b�_
 *
 *		�u�P��/�P�ʁv�Ƃ����`�̒P�ʂ��Ǘ�����N���X������܂��B
 *
 *		���݁A {@link TNB::CValueSpeed ���x(����/����)}�A{@link TNB::CValueAcceleration �����x(����/����^2)}�A
 *		{@link TNB::CValueDensity2 �ʐϖ��x(�d��/�ʐ�)}�A{@link TNB::CValueDensity3 �̐ϖ��x(�d��/�̐�)}�A
 *		{@link TNB::CValueFlux ����(�̐�/����)}�A
 *		�̒P�ʕt�l�N���X���p�ӂ��Ă���܂��B
 *
 *		�܂��A��L�ȊO�̑g�ݍ��킹�� CValueParT �e���v���[�g�N���X�ɂđΉ����邱�Ƃ��\�ł��B
 *
 *	@note	�}�N�� "_TnbUnit_SpecialChar_DISABLE" ����`����Ă���ƁA UNICODE �ł�
 *			���ꕶ��(SJIS�ɕϊ��ł��Ȃ�����)���g��Ȃ��悤�ɂȂ�܂��B
 *
 *	@see http://homepage2.nifty.com/NG/unit/index.htm
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbValueUnit.h"



//TNB Library
namespace TNB	
{



/**@ingroup VALUEUNIT
 * �P�ʕt�l�Ǘ��e���v���[�g�N���X
 *
 *		�^�ɒP�ʊǗ��N���X���Q�w�肷�邱�ƂŁA���̒P�ʂƒl�������N���X�ɂȂ�܂��B
 *
 *	@note �p�����g�킸�A�e���v���[�g�ɂ�鑽�̉����s�Ȃ��Ă��܂��B
 *
 *	@param U1	���q�ɂȂ�P�ʊǗ��N���X�B���ꂪ���q�P�ʃR�[�h�ɂȂ�܂��B
 *	@param U2	����ɂȂ�P�ʊǗ��N���X�B���ꂪ����P�ʃR�[�h�ɂȂ�܂��B
 *	@param EX	�x�[�X��p�̒P�ʊǗ��N���X�B���ꂪ�g���P�ʃR�[�h�ɂȂ�܂��B
 *
 *	@note ���q�P�ʃR�[�h�́AU1 �̒P�ʃR�[�h���A{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}���w��ł��܂��B
 *	@note ����P�ʃR�[�h�́AU2 �̒P�ʃR�[�h���A{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}���w��ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
template<typename U1, typename U2, typename EX>
class CValueParUnitT : public U1, public U2, public EX, public CPfUnit
{
	DEFSUPER(CPfUnit);
protected:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	/**
	 * �P�ʃR�[�h�Ǘ��N���X.
	 *	�g���P�ʃR�[�h���Ǘ����܂��B
	 */
	class CExCode
	{
		int		m_code;		///< �P�ʃR�[�h
	public:
		typedef typename EX::EUnitCode EExCode;
		CExCode(void) : m_code(0) {}
		CExCode(EExCode code) : m_code(code) {}
		int ToInt(void) const { return m_code; };
	};

	/**
	 * �P�ʃR�[�h�Ǘ��N���X.
	 *	���q�P�ʃR�[�h���A�ړ��P�ʃR�[�h���A�g���P�ʃR�[�h���Ǘ����܂��B
	 */
	class CNumeCode
	{
		int		m_code;		///< �P�ʃR�[�h
	public:
		typedef typename U1::EUnitCode EU1Code;
		typedef typename EX::EUnitCode EExCode;
		typedef typename CValueUnitT<U1>::CCode CVuU1Code;
		CNumeCode(void) : m_code(0) {}
		CNumeCode(EU1Code code) : m_code(code) {}
		CNumeCode(EPfUnitCode code) : m_code(code) {}
		CNumeCode(CVuU1Code code) : m_code(code.ToInt()) {}
		explicit CNumeCode(CExCode code) : m_code(code.ToInt()) {}
		operator EU1Code(void) const { return EU1Code(HasPfCode() ? 0 : m_code); }
		operator EPfUnitCode(void) const { return EPfUnitCode(HasPfCode() ? m_code : 0); }
		operator EExCode(void) const { return EExCode(HasExCode() ? m_code : 0); }
		bool operator==(const CNumeCode& c) const { return m_code == c.m_code; }
		bool operator!=(const CNumeCode& c) const { return m_code != c.m_code; }
		bool HasPfCode(void) const { return (m_code & PFCODE) != 0; }
		bool HasExCode(void) const { return (m_code & U1::EXCODE) != 0; }
		int ToInt(void) const { return m_code; };
		EU1Code GetU1Code(void) const { return EU1Code(m_code); };
		EExCode GetExCode(void) const { return EExCode(m_code); };
		CVuU1Code GetCode(void) const 
		{
			CVuU1Code c;
			c.SetInt(m_code);
			return c;
		};
	};
	
	/**
	 * �P�ʃR�[�h�Ǘ��N���X.
	 *	����P�ʃR�[�h���A�ړ��P�ʃR�[�h���Ǘ����܂��B
	 */
	class CDenoCode
	{
		int		m_code;		///< �P�ʃR�[�h
	public:
		typedef typename U2::EUnitCode EU2Code;
		typedef typename CValueUnitT<U2>::CCode CVuU2Code;
		CDenoCode(void) : m_code(0) {}
		CDenoCode(EU2Code code) : m_code(code) {}
		CDenoCode(EPfUnitCode code) : m_code(code) {}
		CDenoCode(CVuU2Code code) : m_code(code.ToInt()) {}
		operator EU2Code(void) const { return EU2Code(HasPfCode() ? 0 : m_code); }
		operator EPfUnitCode(void) const { return EPfUnitCode(HasPfCode() ? m_code : 0); }
		bool operator==(const CDenoCode& c) const { return m_code == c.m_code; }
		bool operator!=(const CDenoCode& c) const { return m_code != c.m_code; }
		bool HasPfCode(void) const { return (m_code & U2::PFCODE) != 0; }
		int ToInt(void) const { return m_code; };
		EU2Code GetU2Code(void) const { return EU2Code(m_code); };
		CVuU2Code GetCode(void) const 
		{
			CVuU2Code c;
			c.SetInt(m_code);
			return c;
		};
	};

	#endif //_TnbDOXYGEN

private:
	
	CRealNumber		m_value;			///< �l
	CNumeCode		m_numeCode;			///< ���q/�g���P�ʃR�[�h
	CDenoCode		m_denoCode;			///< ����P�ʃR�[�h

	/// �ϊ�
	void m_ToUnit(CNumeCode code)
	{
		if ( m_numeCode != code )
		{
			ToNormalUnit();
			if ( code.HasExCode() )
			{
				if ( m_numeCode.ToInt() != 0 )
				{
					CValueUnitT<U1> u1;
					u1.Set(m_value, m_numeCode.GetU1Code());
					m_value = u1.GetValue(U1::EUnitCode(0));
				}
				if ( m_denoCode.ToInt() != 0 )
				{
					CValueUnitT<U2> u2;
					u2.Set(1, U2::EUnitCode(0));
					m_value *= u2.GetValue(m_denoCode.GetU2Code());
					m_denoCode = U2::EUnitCode(0);
				}
				const TUnitParam* P = EX::GetParam(code.GetExCode());
				m_value *= P->doCoefficient;
				m_value += P->doRevise;
			}
			else
			{
				CValueUnitT<U1> u;
				u.Set(m_value, m_numeCode.GetU1Code());
				u.ToUnit(code.GetU1Code());
				m_value = u.GetValue();
			}
			m_numeCode = code;
		}
	}

public:

	/// �P�ʎ�R�[�h
	enum { KINDCODE = EX::KINDCODE };

	/**
	 * �R���X�g���N�^.
	 *		�P�ʂ͕��q����͂��ꂼ��̊�P�ʁA�l�� empty �ɂȂ�܂��B
	 */
	CValueParUnitT(void) : _super(), m_value(), m_numeCode(), m_denoCode()
	{
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̒l�̃C���X�^���X���쐬���܂��B
	 *	@param v �l�B
	 */
	CValueParUnitT(const CRealNumber& v) 
			: _super(), m_value(v), m_numeCode(), m_denoCode()
	{
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̒l�A�P�ʂ̃C���X�^���X���쐬���܂��B
	 *	@param v �l�B
	 *	@param code �g���P�ʃR�[�h�B
	 */
	CValueParUnitT(const CRealNumber& v, CExCode code) 
			: _super(), m_value(v), m_numeCode(code), m_denoCode()
	{
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̒l�A�P�ʂ̃C���X�^���X���쐬���܂��B
	 *	@param v �l�B
	 *	@param c1 ���q�̒P�ʃR�[�h�B
	 *	@param c2 ����̒P�ʃR�[�h�B
	 */
	CValueParUnitT(const CRealNumber& v, CNumeCode c1, CDenoCode c2) 
			: _super(), m_value(v), m_numeCode(c1), m_denoCode(c2)
	{
	}

	/**
	 * [���] �l�A�P�ʐݒ�
	 *		�w��̒l���L�����܂��B�P�ʂ͕ω����܂���B
	 *	@param v �l�B
	 */
	void Set(const CRealNumber& v)
	{
		m_value		= v;
	}

	/**
	 * [���] �l�A�P�ʐݒ�
	 *		�w��̒l�A�P�ʂ��L�����܂��B
	 *	@param v �l�B
	 *	@param code �g���P�ʃR�[�h�B
	 */
	void Set(const CRealNumber& v, CExCode code)
	{
		m_value		= v;
		m_numeCode	= CNumeCode(code);
		m_denoCode	= CDenoCode();
	}

	/**
	 * [���] �l�A�P�ʐݒ�
	 *		�w��̒l�A�P�ʂ��L�����܂��B
	 *	@param v �l�B
	 *	@param c1 ���q�P�ʃR�[�h�B
	 *	@param c2 ����P�ʃR�[�h�B
	 */
	void Set(const CRealNumber& v, CNumeCode c1, CDenoCode c2)
	{
		m_value		= v;
		m_numeCode	= c1;
		m_denoCode	= c2;
	}

	/**
	 * [�擾] ������Ŏ擾 
	 *	@note �w��̃t�H�[�}�b�g�ŁA���݂̒P�ʕ�����t���ŕ�����ɕϊ����܂��B\n
	 *		ex) "5cm/s" 
	 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
	 *	@return ������ 
	 */
	CStr ToString(LPCTSTR lpszFormat = NULL) const
	{
		return m_value.ToString(lpszFormat) + GetUnitMarkString();
	}

	/**
	 * [�擾] ������Ŏ擾 (�g���P�ʎw�肠��)
	 *	@note �w��̃t�H�[�}�b�g�ŁA�w��̒P�ʂɕϊ����ĕ�����ɕϊ����܂��B\n
	 *		ex) "5cm/s" 
	 *	@param code �g���P�ʃR�[�h�B
	 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
	 *	@return ������ 
	 */
	CStr ToString(CExCode code, LPCTSTR lpszFormat = NULL) const
	{
		CValueParUnitT u = *this;
		u.ToUnit(code);
		return u.ToString(lpszFormat);
	}

	/**
	 * [�擾] ������Ŏ擾 (�g���P�ʎw�肠��)
	 *	@note �w��̃t�H�[�}�b�g�ŁA�w��̒P�ʂɕϊ����ĕ�����ɕϊ����܂��B\n
	 *		ex) "5cm/s" 
	 *	@param c1 ���q�P�ʃR�[�h�B
	 *	@param c2 ����P�ʃR�[�h�B
	 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
	 *	@return ������B
	 */
	CStr ToString(CNumeCode c1, CDenoCode c2, LPCTSTR lpszFormat = NULL) const
	{
		CValueParUnitT u = *this;
		u.ToUnit(c1, c2);
		return u.ToString(lpszFormat);
	}

	/**
	 * [�m�F] �P�ʃR�[�h�m�F.
	 *		���ݕێ����Ă���P�ʃR�[�h���A�g���P�ʃR�[�h���ۂ����m�F���܂��B
	 *	@retval true �g���P�ʃR�[�h��ێ����Ă���B
	 *	@retval false �ێ����Ă��Ȃ��B
	 */
	bool HasExCode(void) const
	{
		return m_numeCode.HasExCode();
	}

	/**
	 * [�擾] �P�ʃR�[�h�擾.
	 *		���ݕێ����Ă��镪�q�P�ʃR�[�h���A�g���P�ʃR�[�h���擾���܂��B
	 *	@return �P�ʃR�[�h�B
	 */
	CNumeCode GetUnitCode(void) const
	{
		return m_numeCode;
	}

	/**
	 * [�擾] ����P�ʃR�[�h�擾.
	 *		���ݕێ����Ă��镪��P�ʃR�[�h�𓾂܂��B 
	 *		�{�C���X�^���X���g���P�ʂ�ێ����Ă���ꍇ�A�߂�l�͖��Ӗ��ł��B
	 *	@return �P�ʃR�[�h�B
	 */
	CDenoCode GetDenoUnitCode(void) const
	{
		return m_denoCode;
	}

	/**
	 * [�擾] �P�ʃ}�[�N������擾.
	 *	@note ���ݕێ����Ă���P�ʂ̃}�[�N�������Ԃ��܂��B
	 *	@return �P�ʃR�[�h�B
	 */
	CStr GetUnitMarkString(void) const
	{
		CStr s;
		if ( m_numeCode.HasExCode() )
		{
			s = CValueUnitT<EX>::GetUnitMarkString(m_numeCode.GetExCode());
		}
		else
		{
			s = CValueUnitT<U1>::GetUnitMarkString(m_numeCode.GetU1Code());
			s += _T("/") + CValueUnitT<U2>::GetUnitMarkString(m_denoCode.GetU2Code());
		}
		return s;
	}

	/**
	 * [�擾] �P�ʃ}�[�N������擾.
	 *	@note ���ݕێ����Ă���P�ʂ̃}�[�N�������Ԃ��܂��B
	 *	@return �P�ʃR�[�h�B
	 */
	CStr GetUnitJapanString(void) const
	{
		CStr s;
		if ( m_numeCode.HasExCode() )
		{
			s = CValueUnitT<EX>::GetUnitJapanString(m_numeCode.GetExCode());
		}
		else
		{
			s = CValueUnitT<U1>::GetUnitJapanString(m_numeCode.GetU1Code());
			s += _T(" �� ") + CValueUnitT<U2>::GetUnitJapanString(m_denoCode.GetU2Code());
		}
		return s;
	}

	/**
	 * [�ϊ�] �P�ʕύX (��{�P�ʂ�).
	 *	@note �ێ����Ă���P�ʂ��g���P�ʂ̏ꍇ�A��{�P�ʂɕύX���܂��B�l���v�Z����܂��B
	 */
	void ToNormalUnit(void)
	{
		if ( m_numeCode.HasExCode() )
		{
			//���AEX�R�[�h�Ȃ��{�P�ʂɕϊ�
			const TUnitParam* P = EX::GetParam(m_numeCode.GetExCode());
			m_value -= P->doRevise;
			m_value /= P->doCoefficient;
			m_numeCode = U1::EUnitCode(0);
			m_denoCode = U2::EUnitCode(0);
		}
	}

	/**
	 * [�ϊ�] �P�ʕύX.
	 *	@note �ێ����Ă���P�ʂ�ύX���܂��B�l���v�Z����܂��B
	 *	@param code �g���P�ʃR�[�h�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& ToUnit(CExCode code)
	{
		m_ToUnit(CNumeCode(code));
		return *this;
	}

	/**
	 * [�ϊ�] �P�ʕύX.
	 *	@note �ێ����Ă���P�ʂ�ύX���܂��B�l���v�Z����܂��B
	 *	@param c1 ���q�P�ʃR�[�h�B
	 *	@param c2 ����P�ʃR�[�h�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& ToUnit(CNumeCode c1, CDenoCode c2)
	{
		m_ToUnit(c1);
		return ToDenoUnit(c2);
	}

	/**
	 * [�ϊ�] �P�ʕύX.
	 *	@note �ێ����Ă��镪��̒P�ʂ�����ύX���܂��B�l���v�Z����܂��B
	 *	@param code ����P�ʃR�[�h�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& ToDenoUnit(CDenoCode code)
	{
		if ( m_numeCode.HasExCode() )
		{
			const TUnitParam* P = EX::GetParam(m_numeCode.GetExCode());
			m_value -= P->doRevise;
			m_value /= P->doCoefficient;
			m_numeCode = U1::EUnitCode(0);
			m_denoCode = U2::EUnitCode(0);
		}
		if ( m_denoCode != code )
		{
			CValueUnitT<U2> u;
			u.Set(1, code.GetU2Code());
			u.ToUnit(m_denoCode.GetU2Code());
			m_value *= u.GetValue();
			m_denoCode = code;
		}
		return *this;
	}

	/**
	 * [�擾] �l�擾.
	 *		���ݕێ����Ă���P�ʃR�[�h�̂܂܁A�l��Ԃ��܂��B
	 *	@return �l�B
	 */
	CRealNumber GetValue(void) const
	{
		return m_value;
	}

	/**
	 * [�擾] �l�擾 (�P�ʎw��).
	 *		�ێ����Ă���l���A�w��P�ʂɌv�Z���ĕԂ��܂��B
	 *	@param code �g���P�ʃR�[�h�B
	 *	@return �l�B
	 */
	CRealNumber GetValue(CExCode code) const
	{
		CValueParUnitT r = *this;
		r.ToUnit(code);
		return r.GetValue();
	}

	/**
	 * [�擾] �l�擾 (�P�ʎw��).
	 *		�ێ����Ă���l���A�w��P�ʂɌv�Z���ĕԂ��܂��B
	 *	@param c1 ���q�P�ʃR�[�h�B
	 *	@param c2 ����P�ʃR�[�h�B
	 *	@return �l�B
	 */
	CRealNumber GetValue(CNumeCode c1, CDenoCode c2) const
	{
		CValueParUnitT r = *this;
		r.ToUnit(c1, c2);
		return r.GetValue();
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z���.
	 *		�P�ʂ��l�����l�����Z������܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& operator+=(const CValueParUnitT& u)
	{
		m_value += u.GetValue(m_numeCode, m_denoCode);
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *		�P�ʂ��l�����l�����Z���܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return ���Z���ʁB
	 */
	CValueParUnitT operator+(const CValueParUnitT& u) const
	{
		CValueParUnitT r = *this;
		return r += u;
	}

	/**
	 * [�v�Z] ���Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& operator+=(const CRealNumber& v)
	{
		m_value += v;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueParUnitT operator+(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r += v;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@param u �P�ʕt�l�B
	 *	@return �P�ʕt�l�B
	 */
	friend CValueParUnitT operator+(const CRealNumber& v, const CValueParUnitT& u)
	{
		return u + v;
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z���.
	 *		�P�ʂ��l�����l�����Z������܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& operator-=(const CValueParUnitT& u)
	{
		m_value -= u.GetValue(m_numeCode, m_denoCode);
		return *this;
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z.
	 *		�P�ʂ��l�����l�����Z���܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return ���Z���ʁB
	 */
	CValueParUnitT operator-(const CValueParUnitT& u) const
	{
		CValueParUnitT r = *this;
		return r -= u;
	}

	/**
	 * [�v�Z] ���Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& operator-=(const CRealNumber& v)
	{
		m_value -= v;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueParUnitT operator-(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r -= v;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@param u �P�ʕt�l�B
	 *	@return �P�ʕt�l�B
	 */
	friend CValueParUnitT operator-(const CRealNumber& v, const CValueParUnitT& u)
	{
		CValueParUnitT r = u;
		u.m_value = v - u.m_value;
		return *this;
	}

	/**
	 * [�v�Z] �|�Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& operator*=(const CRealNumber& v)
	{
		m_value *= v;
		return *this;
	}

	/**
	 * [�v�Z] �|�Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueParUnitT operator*(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r *= v;
	}

	/**
	 * [�v�Z] �|�Z.
	 *	@param v �l�B
	 *	@param u �P�ʕt�l�B
	 *	@return �P�ʕt�l�B
	 */
	friend CValueParUnitT operator*(const CRealNumber& v, const CValueParUnitT& u)
	{
		return u * v;
	}

	/**
	 * [�v�Z] �|�Z (���q�P�� = �{�N���X �~ ����P��).
	 *	@param t ����P�ʂ̒l
	 *	@return ���q�P�ʂ̒l
	 */
	CValueUnitT<U1> operator*(const CValueUnitT<U2>& t) const
	{
		CValueParUnitT v = *this;
		v.ToNormalUnit();
		v.ToDenoUnit(t.GetUnitCode());
		CValueUnitT<U1> r;
		r.Set(v.GetValue() * t.GetValue(), v.GetUnitCode().GetCode());
		return r;
	}

	/**
	 * [�v�Z] �|�Z (���q�P�� = ����P�� �~ �{�N���X)
	 *	@param t ����P�ʂ̒l
	 *	@param s �{�N���X�̒l
	 *	@return ���q�P�ʂ̒l
	 */
	friend CValueUnitT<U1> operator*(const CValueUnitT<U2>& t, const CValueParUnitT& s)
	{
		return s * t;
	}

	/**
	 * [�v�Z] ���Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueParUnitT& operator/=(const CRealNumber& v)
	{
		m_value /= v;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueParUnitT operator/(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r /= v;
	}
	
	/**
	 * [�v�Z] ���Z (����P�� = ���q�P�� �� �{�N���X).
	 *	@note ���q�P�ʂ�{�N���X�̒P�ʂŊ���ƁA����P�ʂɂȂ�܂��B
	 *	@param d ���q�P�ʂ̒l
	 *	@param s �{�N���X�̒l
	 *	@return ����P�ʂ̒l
	 */
	friend CValueUnitT<U2> operator/(const CValueUnitT<U1>& d, const CValueParUnitT& s)
	{
		CValueUnitT<U2>::CCode tu = s.GetDenoUnitCode().GetCode();
		CValueUnitT<U2> r;
		r.Set(d.GetValue() / s.GetValue(d.GetUnitCode(), tu), tu);
		return r;
	}

	/**
	 * [�v�Z] ���Z (�l = �{�N���X �� �{�N���X).
	 *	@note �����P�ʌn���m�̊���Z�́A�P�ʂ��Ȃ��Ȃ�܂��B
	 *	@param u �{�N���X�̒l�B
	 *	@return �l�i�P�ʂȂ��j
	 */
	CRealNumber operator/(const CValueParUnitT& u) const
	{
		return m_value / u.GetValue(m_numeCode, m_denoCode);
	}

	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ���
	 *	@retval false	������ t �ƈقȂ�
	 */
	bool operator==(const CValueParUnitT& t) const { return m_value == t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �ƈقȂ�
	 *	@retval false	������ t �Ɠ���
	 */
	bool operator!=(const CValueParUnitT& t) const { return m_value != t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t ��菬����
	 *	@retval false	������ t �Ɠ������傫��
	 */
	bool operator<(const CValueParUnitT& t) const { return m_value < t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ�����������
	 *	@retval false	������ t ���傫��
	 */
	bool operator<=(const CValueParUnitT& t) const { return m_value <= t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t ���傫��
	 *	@retval false	������ t �Ɠ�����������
	 */
	bool operator>(const CValueParUnitT& t) const { return m_value > t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ������傫��
	 *	@retval false	������ t ��菬����
	 */
	bool operator>=(const CValueParUnitT& t) const { return m_value >= t.GetValue(m_numeCode, m_denoCode); }


	//-----------------------


	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static CStr GetUnitKindString(void)
	{
		return EX::GetKindString();
	}

	/**
	 * [�擾] �P�ʎ�R�[�h�擾
	 *	@return �P�ʎ�R�[�h
	 */
	static DWORD GetUnitKindCode(void)
	{
		return EX::KINDCODE;
	}
};



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(���x)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitSpeed : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("���x"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'SP' };

	/// ���x�g���P�ʃR�[�h
	enum EUnitCode
	{
		KPH = EXCODE,		///< km/h
		FPM,				///< ft/min
		KT,					///< �m�b�g
	};

	/**
	 * [�擾] �R�[�h�p�����[�^�擾
	 *	@param code �P�ʃR�[�h
	 *	@return �p�����[�^
	 *	@throw CNoSuchTypeException �R�[�h�l�ُ�̎��A�X���[����܂��B
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		const static TUnitParam A[] = 
		{
			//	�P�ʕ�����		���{��					�W��
			{	_T("kph"),		_T("����"),				3600.0 * 1.0E-3		},	// km/h
			{	_T("fpm"),		_T("fpm"),				60.0 * 3.28084		},	// ft/min
			{	_T("kt"),		_T("�m�b�g"),			3600.0 / 1852.0		},	// kt
		};
		int c = code - EXCODE;
		if ( c >= 0 && c < countof(A) )
		{
			return &A[c];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ���x�Ǘ��N���X
 *
 *	@note ���q�P�ʂ� {@link CUnitDistance::EUnitCode ����}�A����P�ʂ� {@link CUnitTime::EUnitCode ����} �ƂȂ�܂��B
 *				���ꂼ�� {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *				�܂��A���{@link CUnitSpeed::EUnitCode �g���P�ʃR�[�h}�ŁA�P�ʂ��w�肷�邱�Ƃ��o���܂��B
 *	@note ��P�ʂ́Am/s({@link CUnitDistance::M CValueDistance::M}, {@link CUnitTime::SEC CValueTime::SEC}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueParUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3m/s
 *		CValueSpeed d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (m/s) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); // m/s�ŕ\��
 *		// 9.2 �s�R���[�g��/s
 *		CValueSpeed d2(9.2, CValueDistance::PICO, CValueTime::SEC);
 *		TRACE1("%s\n", d2.ToString()); // pm/s �ŕ\��
 *		// 1.2km/h
 *		d2.Set(9.2, CValueSpeed::KPH);
 *		TRACE1("%s\n", d2.ToString(CValueSpeed::KT)); //�m�b�g�ŕ\��
 *		TRACE1("%s\n", d2.ToString(CValueDistance::KM, CValueTime::SEC)); // km/s�ŕ\��
 *		// 100km/h
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString(); // km/h�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueParUnitT<CUnitDistance, CUnitTime, CUnitSpeed> CValueSpeed;

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���x = ���� �� ����).
 *	@param d ����
 *	@param t ����
 *	@return ���x
 */
inline CValueSpeed operator/(const CValueDistance& d, const CValueTime& t)
{
	return CValueSpeed(d.GetValue() / t.GetValue(), d.GetUnitCode(), t.GetUnitCode());
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���� = ���x �� ���g��).
 *	@param s ���x
 *	@param f ���g��
 *	@return ����
 */
inline CValueDistance operator/(const CValueSpeed& s, const CValueFrequency& f)
{
	CRealNumber one(1.0);
	return s * (one / f);
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�����x)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitAcceleration : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�����x"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'AC' };

	/// �����x�g���P�ʃR�[�h
	enum EUnitCode
	{
		GAL = EXCODE,		///< Gal			1 Gal = 1 cm/s2 = 0.01 m/s2
		G					///< �d�͉����x		1 G = 9.80665 m/s2
	};

	/**
	 * [�擾] �R�[�h�p�����[�^�擾
	 *	@param code �P�ʃR�[�h
	 *	@return �p�����[�^
	 *	@throw CNoSuchTypeException �R�[�h�l�ُ�̎��A�X���[����܂��B
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		const static TUnitParam A[] = 
		{
			//	�P�ʕ�����		���{��					�W��
			{	_T("Gal"),		_T("�K��"),				100				},	// Gal
			{	_T("G"),		_T("�d�͉����x"),		1.0 / 9.80665	},	// G
		};
		int c = code - EXCODE;
		if ( c >= 0 && c < countof(A) )
		{
			return &A[c];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �����x�Ǘ��N���X
 *
 *	@note ���q�P�ʂ� {@link CUnitDistance::EUnitCode ����}�A����P�ʂ� {@link CUnitTime2::EUnitCode ����^2} �ƂȂ�܂��B
 *				���ꂼ�� {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *				�܂��A���{@link CUnitAcceleration::EUnitCode �g���P�ʃR�[�h}�ŁA�P�ʂ��w�肷�邱�Ƃ��o���܂��B
 *	@note ��P�ʂ́Am/s^2({@link CUnitDistance::M CValueDistance::M}, {@link CUnitTime2::SEC2 CValueTime2::SEC2}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueParUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3m/s^2
 *		CValueAcceleration d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (m/s^2) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); // m/s^2�ŕ\��
 *		// 9.2 �s�R���[�g��/s^2
 *		CValueAcceleration d2(9.2, CValueDistance::PICO, CValueTime2::SEC2);
 *		TRACE1("%s\n", d2.ToString()); // pm/s^2 �ŕ\��
 *		// 1.2G
 *		d2.Set(9.2, CValueAcceleration::G);
 *		TRACE1("%s\n", d2.ToString(CValueAcceleration::GAL)); // Gal�ŕ\��
 *		TRACE1("%s\n", d2.ToString(CValueDistance::KM, CValueTime2::SEC2)); // km/s^2�ŕ\��
 *		// 100G
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString(); // G�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueParUnitT<CUnitDistance, CUnitTime2, CUnitAcceleration> CValueAcceleration;

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�����x = ���x �~ ���g��) .
 *	@param s ���x
 *	@param f ���g��
 *	@return �����x(�P�ʂ� m/sec^2 �ɂȂ�܂�)
 */
inline CValueAcceleration operator*(const CValueSpeed& s, const CValueFrequency& f)
{
	return CValueAcceleration(s.GetValue(s.M, s.SEC) * f.GetValue(f.HZ), CValueSpeed::M, CValueTime2::SEC2);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�����x = ���g�� �~ ���x) .
 *	@param f ���g��
 *	@param s ���x
 *	@return �����x(�P�ʂ� m/sec^2 �ɂȂ�܂�)
 */
inline CValueAcceleration operator*(const CValueFrequency& f, const CValueSpeed& s)
{
	return s * f;
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (�����x = ���� �� ����^2) .
 *	@param d ����
 *	@param t ����^2
 *	@return �����x
 */
inline CValueAcceleration operator/(const CValueDistance& d, const CValueTime2& t)
{
	return CValueAcceleration(d.GetValue() / t.GetValue(), d.GetUnitCode(), t.GetUnitCode());
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (�����x = ���x �� ����) .
 *	@param s ���x
 *	@param t ����
 *	@return �����x (�P�ʂ� m/sec^2 �ɂȂ�܂�)
 */
inline CValueAcceleration operator/(const CValueSpeed& s, const CValueTime& t)
{
	return CValueAcceleration(s.GetValue(s.M, s.SEC) / t.GetValue(t.SEC), CValueSpeed::M, CValueTime2::SEC2);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (���x = �����x �~ ����) .
 *	@param a �����x
 *	@param t ����
 *	@return ���x (�P�ʂ� m/s �ɂȂ�܂�)
 */
inline CValueSpeed operator*(const CValueAcceleration& a, const CValueTime& t)
{
	return CValueSpeed(a.GetValue(a.M, a.SEC2) * t.GetValue(t.SEC), a.M, t.SEC);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (���x = ���� �~ �����x) .
 *	@param t ����
 *	@param a �����x
 *	@return ���x (�P�ʂ� m/s �ɂȂ�܂�)
 */
inline CValueSpeed operator*(const CValueTime& t, const CValueAcceleration& a)
{
	return a * t;
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���x = �����x �� ���g��) .
 *	@param a �����x
 *	@param f ���g��
 *	@return ���x (�P�ʂ� m/s �ɂȂ�܂�)
 */
inline CValueSpeed operator/(const CValueAcceleration& a, const CValueFrequency& f)
{
	return CValueSpeed(a.GetValue(a.M, a.SEC2) / f.GetValue(f.HZ), a.M, CValueSpeed::SEC);
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�ʐϖ��x)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/22 �V�K�쐬
 */
class CUnitDensity2 : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�ʐ�"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = '2D' };

	/// �ʐϖ��x�g���P�ʃR�[�h
	enum EUnitCode {};

	/**
	 * [�擾] �R�[�h�p�����[�^�擾
	 *	@param code �P�ʃR�[�h
	 *	@return �p�����[�^
	 *	@throw CNoSuchTypeException �R�[�h�l�ُ�̎��A�X���[����܂��B
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �ʐϖ��x�Ǘ��N���X
 *
 *	@note ���q�P�ʂ� {@link CUnitWeight::EUnitCode �d��}�A����P�ʂ� {@link CUnitArea::EUnitCode �ʐ�} �ƂȂ�܂��B
 *				���ꂼ�� {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note ��P�ʂ́Ag/�u({@link CUnitWeight::G CValueWeight::G}, {@link CUnitArea::M2 CValueArea::M2}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueParUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3 g/�u
 *		CValueDensity2 d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (g/�u) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); // g/�u�ŕ\��
 *		// 9.2 �s�Rg/�u
 *		CValueDensity2 d2(9.2, CValueWeight::PICO, CUnitArea::M2);
 *		TRACE1("%s\n", d2.ToString()); // pg/�u �ŕ\��
 *		TRACE1("%s\n", d2.ToString(CValueWeight::KG, CValueArea::M2)); // kg/�u�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/22 �V�K�쐬
 */
typedef CValueParUnitT<CUnitWeight, CUnitArea, CUnitDensity2> CValueDensity2;

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (�ʐϖ��x = �d�� �� �ʐ�).
 *	@param d �d��
 *	@param a �ʐ�
 *	@return �ʐϖ��x
 */
inline CValueDensity2 operator /(const CValueWeight& d, const CValueArea& a)
{
	return CValueDensity2(d.GetValue() / a.GetValue(), d.GetUnitCode(), a.GetUnitCode());
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�̐ϖ��x)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitDensity3 : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�̐ϖ��x"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'DE' };

	/// �̐ϖ��x�g���P�ʃR�[�h
	enum EUnitCode {};

	/**
	 * [�擾] �R�[�h�p�����[�^�擾
	 *	@param code �P�ʃR�[�h
	 *	@return �p�����[�^
	 *	@throw CNoSuchTypeException �R�[�h�l�ُ�̎��A�X���[����܂��B
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �̐ϖ��x�Ǘ��N���X
 *
 *	@note ���q�P�ʂ� {@link CUnitWeight::EUnitCode �d��}�A����P�ʂ� {@link CUnitCubic::EUnitCode �̐�} �ƂȂ�܂��B
 *				���ꂼ�� {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note ��P�ʂ́Ag/m^3({@link CUnitWeight::G CValueWeight::G}, {@link CUnitCubic::M3 CValueCubic::M3}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueParUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3 g/m^3
 *		CValueDensity3 d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (g/m^3) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); // g/m^3�ŕ\��
 *		// 9.2 �s�Rg/m^3
 *		CValueDensity3 d2(9.2, CValueWeight::PICO, CValueCubic::M3);
 *		TRACE1("%s\n", d2.ToString()); // pg/m^3 �ŕ\��
 *		TRACE1("%s\n", d2.ToString(CValueWeight::KG, CValueCubic::M3)); // kg/m^3�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueParUnitT<CUnitWeight, CUnitCubic, CUnitDensity3> CValueDensity3;

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (�̐ϖ��x = �d�� �� �̐�).
 *	@param d �d��
 *	@param t �̐�
 *	@return �̐ϖ��x
 */
inline CValueDensity3 operator /(const CValueWeight& d, const CValueCubic& t)
{
	return CValueDensity3(d.GetValue() / t.GetValue(), d.GetUnitCode(), t.GetUnitCode());
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(����)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitFlux : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("����"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'FL' };

	/// ���x�g���P�ʃR�[�h
	enum EUnitCode {};

	/**
	 * [�擾] �R�[�h�p�����[�^�擾
	 *	@param code �P�ʃR�[�h
	 *	@return �p�����[�^
	 *	@throw CNoSuchTypeException �R�[�h�l�ُ�̎��A�X���[����܂��B
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ���ʊǗ��N���X
 *
 *	@note ���q�P�ʂ� {@link CUnitCubic::EUnitCode �̐�}�A����P�ʂ� {@link CUnitTime::EUnitCode ����} �ƂȂ�܂��B
 *				���ꂼ�� {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note ��P�ʂ́Am^3/s({@link CUnitCubic::M3 CValueCubic::M3}, {@link CUnitTime::SEC CValueTime::SEC}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueParUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3 m^3/s
 *		CValueFlux d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (m^3/s) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); // m^3/s�ŕ\��
 *		// 9.2 mm^3/ms
 *		CValueFlux d2(9.2, CValueCubic::MM3, CValueTime::MSEC);
 *		TRACE1("%s\n", d2.ToString()); // mm^3/ms �ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/18 �V�K�쐬
 */
typedef CValueParUnitT<CUnitCubic, CUnitTime, CUnitFlux> CValueFlux;

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���� = �̐� �� ����).
 *	@param c �̐�
 *	@param t ����
 *	@return ����
 */
inline CValueFlux operator /(const CValueCubic& c, const CValueTime& t)
{
	return CValueFlux(c.GetValue() / t.GetValue(), c.GetUnitCode(), t.GetUnitCode());
}



//-------------------



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(Par�p�_�~�[)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/11 �V�K�쐬
 */
class CUnitParDummy : public CAbstractUnit
{
public:

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 0 };

	/// �_�~�[�g���P�ʃR�[�h
	enum EUnitCode	{};

	/**
	 * [�擾] �R�[�h�p�����[�^�擾
	 *	@param code �P�ʃR�[�h
	 *	@throw CNoSuchTypeException �K���A�X���[����܂��B
	 *	@return �A��܂���B
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �ėp�Ǘ��N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/11 �V�K�쐬
 */
template <typename U1, typename U2>
class CValueParT : public CValueParUnitT<U1, U2, CUnitParDummy>
{
	typedef CValueParUnitT<U1, U2, CUnitParDummy> _super;	///< �e�N���X���Ē�`
	DEFSUPER(_super);
public:

	/// �P�ʎ�R�[�h
	enum { KINDCODE = U1::KINDCODE * 0x1000 + U2::KINDCODE };

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static CStr GetKindString(void)
	{
		CStr s;
		s.Format(_T("%s �� %s"), U1::GetKindString(), U2::GetKindString());
		return s; 
	}

	/**
	 * �R���X�g���N�^.
	 *		�P�ʂ̓R�[�h�O�A�l�� empty �ɂȂ�܂��B
	 */
	CValueParT(void) : _super()
	{
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̒l�A�P�ʂ̃C���X�^���X���쐬���܂��B
	 *	@param v �l�B
	 *	@param c1 ���q�̒P�ʃR�[�h�B
	 *	@param c2 ����̒P�ʃR�[�h�B
	 */
	CValueParT(const CRealNumber& v, CNumeCode c1, CDenoCode c2) 
			: _super(v, c1, c2)
	{
	}

	/**
	 * [�v�Z] ���Z (�{�N���X = ���q�P�� �� ����P��).
	 *	@param u1 ���q�P�ʂ̒l�B
	 *	@param u2 ����P�ʂ̒l�B
	 *	@return �v�Z���ʂ����A�{�N���X�̃C���X�^���X�B
	 */
	friend CValueParT operator /(const CValueUnitT<U1>& u1, const CValueUnitT<U2>& u2)
	{
		return CValueParT(u1.GetValue() / u2.GetValue(), u1.GetUnitCode(), u2.GetUnitCode());
	}
};



}; // TNB
