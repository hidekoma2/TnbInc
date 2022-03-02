#pragma once
/**
 *@file
 * �P�ʕt�l�Ǘ��֌W�̃w�b�_
 *
 *		���݁A{@link TNB::CValueDistance ����}�A{@link TNB::CValueArea �ʐ�}�A{@link TNB::CValueCubic �̐�}�A
 *		{@link TNB::CValueWeight �d��}�A{@link TNB::CValueTemperature ���x}�A{@link TNB::CValueTime ����}�A
 *		{@link TNB::CValueTime2 ����^2}�A{@link TNB::CValueFrequency ���g��}�A{@link TNB::CValueDegree �p�x}�A
 *		{@link TNB::CUnitPressure ����}�A{@link TNB::CUnitRatio ����}�A
 *		{@link TNB::CValueAmpere �d��(�A���y�A)}�A{@link TNB::CValueVolt �d��(�{���g)}
 *		�̒P�ʕt�l�N���X�����݂��܂��B
 *
 *	@note	�}�N�� "_TnbUnit_SpecialChar_DISABLE" ����`����Ă���ƁA UNICODE �ł�
 *			���ꕶ��(SJIS�ɕϊ��ł��Ȃ�����)���g��Ȃ��悤�ɂȂ�܂��B
 *
 *	@see http://homepage2.nifty.com/NG/unit/index.htm
 *	@see http://homepage2.nifty.com/NG/unit/e90608kj.pdf
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbRealNumber.h"
#include "TnbException.h"



//TNB Library
namespace TNB	
{



/// �~����
#define _PAI 3.14159265358979323846264338327950288 

#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#if defined(_UNICODE) && ! defined(_TnbUnit_SpecialChar_DISABLE)
	#define _S2		L"\x00B2"
	#define _M3		L"\x33A5"
	#define _S3		L"\x00B3"
	#define _LITRE	L"\x2113"
	#define _DL		L"\x3397"
#else
	#define _S2		_T("^2")
	#define _M3		_T("m^3")
	#define _S3		_T("^3")
	#define _LITRE	_T("l")
	#define _DL		_T("dl")
#endif
#endif //_TnbDOXYGEN



/**@ingroup VALUEUNIT
 * �P�ʊǗ����ۃN���X
 *
 *		�P�ʊǗ��N���X�͂��̃N���X���p�����č쐬�B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CAbstractUnit
{
public:
	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/// �萔
	enum {
		RASING = 1,				///< �搔�l
		EXCODE = _BIT(29),		///< �g���P�ʃR�[�h
		PFCODE = _BIT(30),		///< �ړ��P�ʃR�[�h
		TNBLIB_VALUEUNIT = -1	///< ���ʎq
	};
	#endif
};



/**@ingroup VALUEUNIT
 * �P�ʏ��e�[�u���^
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 */
struct TUnitParam
{
	LPCTSTR lpszMarkString;		///< �P�ʋL��������
	LPCTSTR lpszJapanString;	///< �P�ʓ��{�ꕶ����
	double	doCoefficient;		///< �ϊ��W��
	double	doRevise;			///< �덷�W��
};



/**@ingroup VALUEUNIT
 * �ړ��P�ʊǗ��N���X
 *
 *		�ړ��P�ʂ������N���X�͂��̃N���X���p�����č쐬�B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 *	@date 10/11/18 2�i�ړ����A�ǉ�
 */
class CPfUnit
{
public:
	/// �ړ��P�ʃR�[�h.
	enum EPfUnitCode
	{
		DEF = _BIT(30),		///< -
		YOTTA,				///< ���^		(1.0E+24)
		ZETTA,				///< �[�^		(1.0E+21)
		EXA,				///< �G�N�T		(1.0E+18)
		PETA,				///< �؃^		(1.0E+15)
		TERA,				///< �e��		(1.0E+12)
		GIGA,				///< �M�K		(1.0E+9)
		MEGA,				///< ���K		(1.0E+6)
		KILO,				///< �L��		(1.0E+3)
		DECI,				///< �f�V		(1.0E-1)
		CENTI,				///< �Z���`		(1.0E-2)
		MILLI,				///< �~��		(1.0E-3)
		MICRO,				///< �}�C�N��	(1.0E-6)
		NANO,				///< �i�m		(1.0E-9)
		PICO,				///< �s�R		(1.0E-12)
		FEMTO,				///< �t�F���g	(1.0E-15)
		ATTO,				///< �A�g		(1.0E-18)
		ZEPTO,				///< �[�v�g		(1.0E-21)
		YOCTO,				///< ���N�g		(1.0E-24)
		//
		YOBI,				///< ���r		(2^80)
		ZEBI,				///< �[�r		(2^70)
		EXBI,				///< �G�N�X�r	(2^60)
		PEBI,				///< �؃r		(2^50)
		TEBI,				///< �e�r		(2^40)
		GIBI,				///< �M�r		(2^30)
		MEBI,				///< ���r		(2^20)
		KIBI,				///< �L�r		(2^10)

		KIRO = KILO,				///< �L��		(1.0E+3)
	};

	/**
	 * [�擾] �ړ��P�ʃR�[�h�p�����[�^�擾
	 *	@param code �ړ��P�ʃR�[�h
	 *	@return �p�����[�^
	 *	@throw CNoSuchTypeException �R�[�h�l�ُ�̎��A�X���[����܂��B
	 */
	static const TUnitParam* GetPfParam(EPfUnitCode code)
	{
		const static TUnitParam A[] = 
		{
			//	�P�ʕ�����		���{��				�W��
			{	_T(""),			_T(""),				1.0			},	// -
			{	_T("Y"),		_T("���^"),			1.0E-24		},	// yotta
			{	_T("Z"),		_T("�[�^"),			1.0E-21		},	// zetta
			{	_T("E"),		_T("�G�N�T"),		1.0E-18		},	// exa
			{	_T("P"),		_T("�؃^"),			1.0E-15		},	// peta
			{	_T("T"),		_T("�e��"),			1.0E-12		},	// tera
			{	_T("G"),		_T("�M�K"),			1.0E-9		},	// giga
			{	_T("M"),		_T("���K"),			1.0E-6		},	// mega
			{	_T("k"),		_T("�L��"),			1.0E-3		},	// kilo
			{	_T("d"),		_T("�f�V"),			1.0E+1		},	// deci
			{	_T("c"),		_T("�Z���`"),		1.0E+2		},	// centi
			{	_T("m"),		_T("�~��"),			1.0E+3		},	// milli
			{	_T("��"),		_T("�}�C�N��"),		1.0E+6		},	// micro
			{	_T("n"),		_T("�i�m"),			1.0E+9		},	// nano
			{	_T("p"),		_T("�s�R"),			1.0E+12		},	// pico
			{	_T("f"),		_T("�t�F���g"),		1.0E+15		},	// femto
			{	_T("a"),		_T("�A�g"),			1.0E+18		},	// atto
			{	_T("z"),		_T("�[�v�g"),		1.0E+21		},	// zepto
			{	_T("y"),		_T("���N�g"),		1.0E+24		},	// yocto
			//
			{	_T("Yi"),		_T("���r"),			1.0 / 1208925819614629174706176.0	},
			{	_T("Zi"),		_T("�[�r"),			1.0 / 1180591620717411303424.0 },
			{	_T("Ei"),		_T("�G�N�X�r"),		1.0 / (1ui64 << 60)	} ,
			{	_T("Pi"),		_T("�y�r"),			1.0 / (1ui64 << 50) },
			{	_T("Ti"),		_T("�e�r"),			1.0 / (1ui64 << 40) },
			{	_T("Gi"),		_T("�M�r"),			1.0 / (1ui64 << 30) },
			{	_T("Mi"),		_T("���r"),			1.0 / (1ui64 << 20) },
			{	_T("Ki"),		_T("�L�r"),			1.0 / (1ui64 << 10)	},
		};
		int i = code - DEF;		
		if ( i >= 0 && i < countof(A) )
		{
			return &A[i];
		}
		throw CNoSuchTypeException();
	}
};



/**@ingroup VALUEUNIT
 * �P�ʕt�l�Ǘ��e���v���[�g�N���X
 *
 *		�^�ɒP�ʊǗ��N���X���w�肷�邱�ƂŁA���̒P�ʂƒl�������N���X�ɂȂ�܂��B
 *
 *	@note �p�����g�킸�A�e���v���[�g�ɂ�鑽�̉����s�Ȃ��Ă��܂��B
 *
 *	@param U �P�ʊǗ��N���X�B����ɂ��P�ʎ킪���܂�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
template<typename U>
class CValueUnitT : public U, public CPfUnit
{
public:

	/**@ingroup VALUEUNIT
	 * �P�ʃR�[�h�Ǘ��N���X
	 *
	 *	@note	���\�b�h�̈����ɖ{�N���X���w�肳��Ă�����̂́A
	 *			U::EUnitCode �� {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}���w��ł��܂��B
	 *			�������A�P�ʎ�ɂ���Ă� �ړ��P�ʃR�[�h���g�p���K���Ȃ����̂�
	 *			����܂��i�Ⴆ�΁u���x�v�Ȃǁj�B
	 */
	class CCode
	{
		int		m_code;		///< �P�ʃR�[�h
	public:
		typedef typename U::EUnitCode EUnitCode;	///< ����q�^���Ē�`
		/// �R���X�g���N�^
		CCode(void) : m_code(0) {}
		/**
		 * �R���X�g���N�^
		 *	@param code �P�ʃR�[�h
		 */
		CCode(EUnitCode code) : m_code(code) {}
		/**
		 * �R���X�g���N�^
		 *	@param code �ړ��P�ʃR�[�h
		 */
		CCode(EPfUnitCode code) : m_code(code) {}
		/**
		 * [�擾] �P�ʃR�[�h�擾
		 *	@return �P�ʃR�[�h
		 */
		operator EUnitCode(void) const { return EUnitCode(HasPfCode() ? 0 : m_code); }
		/**
		 * [�擾] �ړ��P�ʃR�[�h�擾
		 *	@return �ړ��P�ʃR�[�h
		 */
		operator EPfUnitCode(void) const { return EPfUnitCode(HasPfCode() ? m_code : 0); }
		/**
		 * [�m�F] ��r
		 *	@param c ��r�Ώ�
		 *	@retval true ��v
		 *	@retval false �s��v
		 */
		bool operator==(const CCode& c) const { return m_code == c.m_code; }
		/**
		 * [�m�F] ��r
		 *	@param c ��r�Ώ�
		 *	@retval true �s��v
		 *	@retval false ��v
		 */
		bool operator!=(const CCode& c) const { return m_code != c.m_code; }
		/**
		 * [�m�F] �ړ��P�ʃR�[�h�ێ��H
		 *	@retval true SI�R�[�h�ێ��B
		 *	@retval false �ہB
		 */
		bool HasPfCode(void) const { return (m_code & PFCODE) != 0; }
		/**
		 * [�擾] int�Ŏ擾
		 *	@return �R�[�h
		 */
		int ToInt(void) const { return m_code; };
		/**
		 * [�ݒ�] int�Őݒ�
		 *	@param i �R�[�h
		 */
		void SetInt(int i) { m_code = i; }
	};

private:

	CRealNumber			m_value;	///< �l
	CCode				m_code;		///< �P�ʃR�[�h
	
	void m_Test(const CAbstractUnit* u){}
	void m_Test(LPCVOID u){ ASSERT(false); }

	/// �덷�W���擾
	double m_GetRevise(const CCode& c) const
	{
		const TUnitParam* P = c.HasPfCode() ? GetPfParam(c) : U::GetParam(c);
		return P->doRevise;
	}

	/// �ϊ��W���擾	
	double m_GetCoefficient(const CCode& c) const
	{
		double r;
		if ( c.HasPfCode() )
		{
			double a = GetPfParam(c)->doCoefficient;
			r = 1;
			loop ( i, U::RASING )
			{
				r *= a;
			}
		}
		else
		{
			r = U::GetParam(c)->doCoefficient;
		}
		return r;
	}

public:

	/**
	 * �R���X�g���N�^.
	 *		�P�ʂ͒P�ʎ�̊�P�ʂɁB�l�� empty �ɂȂ�܂��B
	 */
	CValueUnitT(void) : m_value(), m_code()
	{
		DEBUG_ONLY(m_Test(this));
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̒l�̃C���X�^���X���쐬���܂��B�P�ʂ͒P�ʎ�̊�P�ʂɂȂ�܂��B
	 *	@param v �l�B
	 */
	explicit CValueUnitT(const CRealNumber& v) : m_value(v), m_code()
	{
		DEBUG_ONLY(m_Test(this));
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̒l�A�P�ʂ̃C���X�^���X���쐬���܂��B
	 *	@param v �l�B
	 *	@param code �P�ʃR�[�h�B
	 */
	CValueUnitT(const CRealNumber& v, CCode code) : m_value(v), m_code(code)
	{
		DEBUG_ONLY(m_Test(this));
	}

	/**
	 * [���] �l�ݒ�
	 *		�w��̒l��ݒ肵�܂��B�P�ʂ͕ω����܂���B
	 *	@param v �l�B
	 */
	void Set(const CRealNumber& v)
	{
		m_value	= v;
	}

	/**
	 * [���] �l�A�P�ʐݒ�
	 *		�w��̒l�A�P�ʂ�ݒ肵�܂��B
	 *	@param v �l�B
	 *	@param code �P�ʃR�[�h�B
	 */
	void Set(const CRealNumber& v, CCode code)
	{
		m_value	= v;
		m_code	= code;
	}

	/**
	 * [�擾] ������Ŏ擾 
	 *	@note �w��̃t�H�[�}�b�g�ŁA���݂̒P�ʕ�����t���ŕ�����ɕϊ����܂��B\n
	 *		ex) "5cm" 
	 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
	 *	@return ������ 
	 */
	CStr ToString(LPCTSTR lpszFormat = NULL) const
	{
		return m_value.ToString(lpszFormat) + GetUnitMarkString(m_code);
	}

	/**
	 * [�擾] ������Ŏ擾�i�P�ʎw�肠��j
	 *	@note �w��̃t�H�[�}�b�g�ŁA�w��̒P�ʂɕϊ����ĕ�����ɕϊ����܂��B\n
	 *		ex) "5cm" 
	 *	@param code �P�ʃR�[�h�B 
	 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
	 *	@return ������ 
	 */
	CStr ToString(CCode code, LPCTSTR lpszFormat = NULL) const
	{
		CValueUnitT r = *this;
		r.ToUnit(code);
		return r.ToString(lpszFormat);
	}

	/**
	 * [�擾] �P�ʃR�[�h�擾.
	 *		���ݕێ����Ă���P�ʃR�[�h�擾�B
	 *	@return �P�ʃR�[�h�B
	 */
	CCode GetUnitCode(void) const
	{
		return m_code;
	}

	/**
	 * [�擾] �P�ʃ}�[�N������擾.
	 *		���ݕێ����Ă���P�ʂ̕������Ԃ��܂��B
	 *	@return �P�ʃ}�[�N������B
	 */
	CStr GetUnitMarkString(void) const
	{
		return GetUnitMarkString(m_code);
	}

	/**
	 * [�擾] �P�ʈӖ�������擾.
	 *		���ݕێ����Ă���P�ʂ̕������Ԃ��܂��B
	 *	@return �P�ʈӖ�������B
	 */
	CStr GetUnitJapanString(void) const
	{
		return GetUnitJapanString(m_code);
	}

	/**
	 * [�ϊ�] �P�ʕύX.
	 *	@note �ێ����Ă���P�ʂ�ύX���܂��B�l���v�Z����܂��B
	 *	@param code �P�ʃR�[�h�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& ToUnit(CCode code)
	{
		if ( m_code != code )
		{
			m_value -= m_GetRevise(m_code);
			m_value *= m_GetCoefficient(code) / m_GetCoefficient(m_code);
			m_value += m_GetRevise(code);
			m_code = code;
		}
		return *this;
	}

	/**
	 * [�擾] �l�擾.
	 *		���݂̒P�ʃR�[�h�̂܂܁A�l��Ԃ��܂��B
	 *	@return �l�B
	 */
	CRealNumber GetValue(void) const
	{
		return m_value;
	}

	/**
	 * [�擾] �l�擾.
	 *		���݂̒P�ʃR�[�h�̂܂܁A�l��Ԃ��܂��B
	 *	@return �l�B
	 */
	operator CRealNumber(void) const
	{
		return m_value;
	}

	/**
	 * [�擾] �l�擾(�P�ʎw��).
	 *		�ێ����Ă���l���A�w��P�ʂɌv�Z���ĕԂ��܂��B
	 *	@param code �P�ʃR�[�h�B
	 *	@return �l�B
	 */
	CRealNumber GetValue(CCode code) const
	{
		CValueUnitT r = *this;
		r.ToUnit(code);
		return r.GetValue();
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z���.
	 *		�P�ʂ��l�����l�����Z������܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& operator+=(const CValueUnitT& u)
	{
		m_value += u.GetValue(m_code);
		return *this;
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z.
	 *		�P�ʂ��l�����l�����Z���܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return ���Z�������ʁB
	 */
	CValueUnitT operator+(const CValueUnitT& u) const
	{
		CValueUnitT r = *this;
		return r += u;
	}

	/**
	 * [�v�Z] ���Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& operator+=(const CRealNumber& v)
	{
		m_value += v;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueUnitT operator+(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r += v;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@param u �P�ʕt�l�B
	 *	@return �P�ʕt�l�B
	 */
	friend CValueUnitT operator+(const CRealNumber& v, const CValueUnitT& u)
	{
		return u + v;
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z���.
	 *		�P�ʂ��l�����l�����Z���܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& operator-=(const CValueUnitT& u)
	{
		m_value -= u.GetValue(m_code);
		return *this;
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z.
	 *		�P�ʂ��l�����l�����Z���܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return ���Z��������
	 */
	CValueUnitT operator-(const CValueUnitT& u) const
	{
		CValueUnitT r = *this;
		return r -= u;
	}

	/**
	 * [�v�Z] ���Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& operator-=(const CRealNumber& v)
	{
		m_value -= v;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueUnitT operator-(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r -= v;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@param u �P�ʕt�l�B
	 *	@return �P�ʕt�l�B
	 */
	friend CValueUnitT operator-(const CRealNumber& v, const CValueUnitT& u)
	{
		CValueUnitT r = u;
		u.m_value = v - u.m_value;
		return *this;
	}

	/**
	 * [�v�Z] �|�Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& operator*=(const CRealNumber& v)
	{
		m_value *= v;
		return *this;
	}

	/**
	 * [�v�Z] �|�Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueUnitT operator*(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r *= v;
	}

	/**
	 * [�v�Z] �|�Z.
	 *	@param v �l�B
	 *	@param u �P�ʕt�l�B
	 *	@return �P�ʕt�l�B
	 */
	friend CValueUnitT operator*(const CRealNumber& v, const CValueUnitT& u)
	{
		return u * v;
	}

	/**
	 * [�v�Z] �P�ʕt�l���Z.
	 *	@note ���P�ʌn���m�̊���Z�́A�P�ʂ��Ȃ��Ȃ�܂��B
	 *	@param u �P�ʕt�l�B
	 *	@return �l�i�P�ʂȂ��j�B
	 */
	CRealNumber operator/(const CValueUnitT& u) const
	{
		return m_value / u.GetValue(m_code);
	}

	/**
	 * [�v�Z] ���Z���.
	 *	@param v �l�B
	 *	@return �����̎Q�ƁB
	 */
	CValueUnitT& operator/=(const CRealNumber& v)
	{
		m_value /= v;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param v �l�B
	 *	@return �P�ʕt�l�B
	 */
	CValueUnitT operator/(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r /= v;
	}

	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ���
	 *	@retval false	������ t �ƈقȂ�
	 */
	bool operator==(const CValueUnitT& t) const { return m_value == t.GetValue(m_code); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �ƈقȂ�
	 *	@retval false	������ t �Ɠ���
	 */
	bool operator!=(const CValueUnitT& t) const { return m_value != t.GetValue(m_code); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t ��菬����
	 *	@retval false	������ t �Ɠ������傫��
	 */
	bool operator<(const CValueUnitT& t) const { return m_value < t.GetValue(m_code); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ�����������
	 *	@retval false	������ t ���傫��
	 */
	bool operator<=(const CValueUnitT& t) const {return m_value <= t.GetValue(m_code); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t ���傫��
	 *	@retval false	������ t �Ɠ�����������
	 */
	bool operator>(const CValueUnitT& t) const { return m_value > t.GetValue(m_code); }
	
	/**
	 * [�m�F] ��r
	 *	@note �P�ʂ����낦�Ĕ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ������傫��
	 *	@retval false	������ t ��菬����
	 */
	bool operator>=(const CValueUnitT& t) const { return m_value >= t.GetValue(m_code); }


	//----------------


	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static CStr GetUnitKindString(void)
	{
		return U::GetKindString();
	}

	/**
	 * [�擾] �P�ʎ�R�[�h�擾
	 *	@return �P�ʎ�R�[�h
	 */
	static DWORD GetUnitKindCode(void)
	{
		return U::KINDCODE;
	}

	/**
	 * [�擾] �P�ʃ}�[�N������擾.
	 *	@param code �P�ʃR�[�h�B
	 *	@return �P�ʃ}�[�N������B
	 */
	static CStr GetUnitMarkString(CCode code)
	{
		CStr s;
		if ( code.HasPfCode() )
		{
			s = GetPfParam(code)->lpszMarkString;
			s += U::GetParam(U::EUnitCode(0))->lpszMarkString;
		}
		else
		{
			s = U::GetParam(code)->lpszMarkString;
		}
		return s;
	}

	/**
	 * [�擾] �P�ʈӖ�������擾.
	 *	@param code �P�ʃR�[�h�B
	 *	@return �P�ʈӖ�������B
	 */
	static CStr GetUnitJapanString(CCode code) 
	{
		CStr s;
		if ( code.HasPfCode() )
		{
			s = GetPfParam(code)->lpszJapanString;
			s += U::GetParam(U::EUnitCode(0))->lpszJapanString;
		}
		else
		{
			s = U::GetParam(code)->lpszJapanString;
		}
		return s;
	}
};



//--------------------------------



/**@ingroup VALUEUNIT
 * [�쐬] ������쐬
 *	@param t �P�ʕt�l�N���X�̃C���X�^���X�B
 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
 *	@return ������
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
template<typename T>
inline CStr ToStringWithJapanUnit(const T& t, LPCTSTR lpszFormat = NULL)
{
	return t.GetValue().ToString(lpszFormat) + t.GetUnitJapanString();
}



/**@ingroup VALUEUNIT
 * [�쐬] ������쐬
 *	@param t �P�ʕt�l�N���X�̃C���X�^���X�B
 *	@param lpszFormat printf()�� double ���\������ꍇ�̃t�H�[�}�b�g���w��B�ȗ������ "%f" �ɂȂ�܂��B
 *	@return ������
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
template<typename T>
inline CStr ToStringWithUnitMark(const T& t, LPCTSTR lpszFormat = NULL)
{
	return t.GetValue().ToString(lpszFormat) + t.GetUnitMarkString();
}



/**@ingroup VALUEUNIT
 * [�擾] �P�ʎ핶����擾
 *	@param t �P�ʕt�l�N���X�̃C���X�^���X�B
 *	@return �P�ʎ핶����
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
template<typename T>
inline CStr GetUnitKindString(const T& t)
{
	return T::GetKindString();
}

/**@ingroup VALUEUNIT
 * [�擾] �P�ʎ�R�[�h�擾
 *	@param t �P�ʕt�l�N���X�̃C���X�^���X�B
 *	@return �P�ʎ�R�[�h
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
template<typename T>
inline DWORD GetUnitKindCode(const T& t)
{
	return T::KINDCODE;
}



//--------------------------------



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(����)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@note �V���P��;
 *			�n�������z�̎�������ȉ~�O���̒����a(�ȒP�ɂ͑��z����n���܂ł̕��ϓI����)��1�V���P��
 *			�Ƃ��Ē�`����Ă���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitDistance : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("����"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Di' };

	/// �����P�ʃR�[�h
	enum EUnitCode
	{
		M,			///< ���[�g��
		INCH,		///< �C���`
		FEET,		///< �t�B�[�g
		YARD,		///< ���[�h
		MILE,		///< �}�C��
		KAIRI,		///< �C��
		SHAKU,		///< ��
		KEN,		///< ��
		CHOU,		///< ��
		SATO,		///< ��
		ANG,		///< �I���O�X�g���[��
		AU,			///< �V���P��
		LY,			///< ���N
		PC,			///< �p�[�Z�N

		MM		= CPfUnit::MILLI,	///< �~�����[�g��
		CM		= CPfUnit::CENTI,	///< �Z���`���[�g��
		KM		= CPfUnit::KIRO,	///< �L�����[�g��
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
			{	_T("m"),		_T("���[�g��"),			1.0			},	// m
			{	_T("in"),		_T("�C���`"),			39.37		},	// inch
			{	_T("ft"),		_T("�t�B�[�g"),			3.28084		},	// feet
			{	_T("yd"),		_T("���[�h"),			1.09361		},	// yard
			{	_T("mile"),		_T("�}�C��"),			0.000621	},	// mile
			{	_T("n.m."),		_T("�C��"),				0.000539	},	// kairi
			{	_T("��"),		_T("��"),				3.3			},	// ��
			{	_T("��"),		_T("��"),				3.3 / 6.0	},	// ��
			{	_T("��"),		_T("��"),				3.3	/ 360.0	},	// ��
			{	_T("��"),		_T("��"),				3.3 / 12960.0},	// ��
			{	_T("��"),		_T("�I���O�X�g���[��"),	1.0E+10		},	// �I���O�X�g���[��
			{	_T("AU"),		_T("�V���P��"),			6.68459E-12	},	// �V���P��
			{	_T("ly"),		_T("���N"),				1.05700E-16	},	// ���N
			{	_T("pc"),		_T("�p�[�Z�N"),			3.24078E-17	},	// �p�[�Z�N
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �����Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitDistance::EUnitCode �����P�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A���[�g��({@link CUnitDistance::M CValueDistance::M}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3m
 *		CValueDistance d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (m) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //m�ŕ\��
 *		// 9.2�s�R���[�g��
 *		CValueDistance d2(9.2, CValueDistance::PICO);
 *		TRACE1("%s\n", d2.ToString()); //pm�ŕ\��
 *		// 9.2���[�h
 *		d2.Set(9.2, CValueDistance::YARD);
 *		TRACE1("%s\n", d2.ToString(CValueDistance::M)); //m�ŕ\��
 *		// 1.2���N
 *		d2.Set(1.2, CValueDistance::LY);
 *		// 100���N
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitDistance> CValueDistance;

/**@ingroup VALUEUNIT
 * [�v�Z] �΂ߋ����v�Z (��(��������^2 + ��������^2))
 *	@param x ��������
 *	@param y ��������
 *	@return �΂ߋ���
 */
inline CValueDistance CalcSlantingDistance(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	const CRealNumber& xx = x.GetValue();
	const CRealNumber& yy = y.GetValue(code);
	CRealNumber r = sqrt(xx * xx + yy * yy);
	return CValueDistance(r, code);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �~���͒��v�Z (2 �~ �� �~ ���a)
 *	@param r ���a����
 *	@return ����
 */
inline CValueDistance CalcCircleDistance(const CValueDistance& r)
{
	return r * 2 * _PAI;
}

/**@ingroup VALUEUNIT
 * [�v�Z] �ȉ~���͒��v�Z (�� �~ ��( 2 (�������a^2 + �������a^2))
 *	@param x �������a����
 *	@param y �������a����
 *	@return ����
 */
inline CValueDistance CalcEllipseDistance(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	const CRealNumber& xx = x.GetValue();
	const CRealNumber& yy = y.GetValue(code);
	CRealNumber r = _PAI * sqrt((xx * xx + yy * yy) * 2);
	return CValueDistance(r, code);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �ȉ~���͒��v�Z (type2)
 *	@note	A = ���a�P�AB=���a�Q�B�@�������AA > B �Ƃ���B\n
 *			�΁E��(2 �~ (A^2 + B^2)) - (A - B)^2 �� 2.2
 *	@param x �������a����
 *	@param y �������a����
 *	@return ����
 */
inline CValueDistance CalcEllipseDistance_t2(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	CRealNumber A = x.GetValue();
	CRealNumber B = y.GetValue(code);
	if ( A < B )
	{
		Swap(A, B);
	}
	CRealNumber r = _PAI * sqrt((A * A + B * B) * 2 - (A - B) * (A - B) / 2.2);
	return CValueDistance(r, code);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �ȉ~���͒��v�Z (type3)
 *	@note	A = ���a�P�AB=���a�Q�B�@�������AA > B �Ƃ���B\n
 *			2 �~ A �~ �� * (1 - (B^2 �� A^2)) �� 4 - (B^2 �� A^2))^2 �~ 3 �� 64 - (B^2 �� A^2))^3 �~ 5 �� 256)
 *	@param x �������a����
 *	@param y �������a����
 *	@return ����
 */
inline CValueDistance CalcEllipseDistance_t3(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	CRealNumber A = x.GetValue();
	CRealNumber B = y.GetValue(code);
	if ( A < B )
	{
		Swap(A, B);
	}
	if ( B == 0.0 )
	{
		return CValueDistance(A * 4.0, code);
	}
	CRealNumber a = 1 - (B * B) / (A * A);
	CRealNumber r = 2 * A * _PAI * (1 - a / 4 - (a * a) * 3 / 64 - (a * a * a) * 5 / 256);	
	return CValueDistance(r, code);
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�ʐ�)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitArea : public CAbstractUnit
{
public:
	enum { RASING = 2 };///< �搔�l

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�ʐ�"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Ar' };

	/// �ʐϒP�ʃR�[�h
	enum EUnitCode
	{
		M2,		///< �������[�g��
		A,		///< �A�[��
		HA,		///< �w�N�g�A�[��
		INCH2,	///< �����C���`
		YARD2,	///< �������[�h
		ACRE,	///< �G�[�J�[

		MM2		= CPfUnit::MILLI,	///< �~���������[�g��
		CM2		= CPfUnit::CENTI,	///< �Z���`�������[�g��
		KM2		= CPfUnit::KIRO,	///< �L���������[�g��
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
			//	�P�ʕ�����		���{��						�W��
			{	_T("�u"),		_T("�������[�g��"),			1.0				},	// m2
			{	_T("a"),		_T("�A�[��"),				0.01			},	// a
			{	_T("ha"),		_T("�w�N�g�A�[��"),			0.0001			},	// ha
			{	_T("in") _S2,	_T("�����C���`"),			1550.0			},	// inch2
			{	_T("yd") _S2,	_T("�������[�h"),			1.19599			},	// yard2
			{	_T("acre"),		_T("�G�[�J�["),				0.000247		},	// acre
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �ʐϊǗ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitArea::EUnitCode �ʐϒP�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�������[�g��({@link CUnitArea::M2 CValueArea::M2}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3m2
 *		CValueArea d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (m2) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //m2�ŕ\��
 *		// 9.2��m2
 *		CValueArea d2(9.2, CValueArea::MICRO);
 *		TRACE1("%s\n", d2.ToString()); //��m2�ŕ\��
 *		// 9.2�A�[��
 *		d2.Set(9.2, CValueArea::A);
 *		TRACE1("%s\n", d2.ToString(CValueArea::M2)); //m2�ŕ\��
 *		// 1.2�G�[�J�[
 *		d2.Set(1.2, CValueArea::ACRE);
 *		// 100�G�[�J�[
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitArea> CValueArea;

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�ʐ� = ���� �~ ����) 
 *	@param d1 ����
 *	@param d2 ����
 *	@return �ʐ�(�P�ʂ͕������[�g���Ŋi�[����܂�)�B
 */
inline CValueArea operator*(const CValueDistance& d1, const CValueDistance& d2)
{
	return CValueArea(d1.GetValue(d1.M) * d2.GetValue(d2.M), CValueArea::M2);
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���� = �ʐ� �� ����)
 *	@param a �ʐ�
 *	@param d ����
 *	@return ����(�P�ʂ̓��[�g���Ŋi�[����܂�)�B
 */
inline CValueDistance operator/(const CValueArea& a, const CValueDistance& d)
{
	return CValueDistance(a.GetValue(a.M2) / d.GetValue(d.M), CValueDistance::M);
}

/**@ingroup VALUEUNIT
 * [�v�Z] ������ (���� = ��ʐ�)
 *	@param a �ʐ�
 *	@return �����i�P�ʂ̓��[�g���ɂȂ�܂��j
 */
inline CValueDistance sqrt(const CValueArea& a)
{
	CRealNumber r = sqrt(a.GetValue(a.M2));
	return CValueDistance(r, CValueDistance::M);
}
	
/**@ingroup VALUEUNIT
 * [�v�Z] �O�p�ʐόv�Z (��� �~ ���� �� 2)
 *	@param x ��Ӌ���
 *	@param y ��������
 *	@return �ʐρi�P�ʂ� �u �ɂȂ�܂��j
 */
inline CValueArea CalcTriangleArea(const CValueDistance& x, const CValueDistance& y)
{
	return x * y / 2.0;
}

/**@ingroup VALUEUNIT
 * [�v�Z] �~�ʐόv�Z (�� �~ ���a �~ ���a)
 *	@param r ���a����
 *	@return �ʐρi�P�ʂ� m2 �ɂȂ�܂��j
 */
inline CValueArea CalcCircleArea(const CValueDistance& r)
{
	return r * r * _PAI;
}

/**@ingroup VALUEUNIT
 * [�v�Z] �ȉ~�ʐόv�Z (�� �~ �������a �~ �������a)
 *	@param x �������a����
 *	@param y �������a����
 *	@return �ʐρi�P�ʂ� m2 �ɂȂ�܂��j
 */
inline CValueArea CalcEllipseArea(const CValueDistance& x, const CValueDistance& y)
{
	return x * y * _PAI;
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�̐�)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitCubic : public CAbstractUnit
{
public:
	enum { RASING = 3 };///< �搔�l

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�̐�"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Cu' };

	/// �̐ϒP�ʃR�[�h
	enum EUnitCode
	{
		M3,		///< �������[�g��
		INCH3,	///< �����C���`
		YARD3,	///< �������[�h
		BBL,	///< �o����
		LITRE,	///< ���b�g��
		DL,		///< �f�V���b�g��
		CC,		///< cc

		MM3		= CPfUnit::MILLI,	///< �~���������[�g��
		CM3		= CPfUnit::CENTI,	///< �Z���`�������[�g��
		KM3		= CPfUnit::KIRO,	///< �L���������[�g��
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
			//	�P�ʕ�����		���{��						�W��
			{	_M3,			_T("�������[�g��"),			1.0						},	// m3
			{	_T("in") _S3,	_T("�����C���`"),			61023.6					},	// inch3
			{	_T("yd") _S3,	_T("�������[�h"),			1.30795					},	// yard3
			{	_T("bbl"),		_T("�o����"),				6.28982					},	// bbl
			{	_LITRE,			_T("���b�g��"),				1000.0					},	// litre
			{	_DL,			_T("�f�V���b�g��"),			10000.0					},	// dl
			{	_T("cc"),		_T("cc"),					1000000.0				},	// cc
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �̐ϊǗ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitCubic::EUnitCode �̐ϒP�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�������[�g��({@link CUnitCubic::M3 CValueCubic::M3}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3m3
 *		CValueCubic d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (m3) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //m3�ŕ\��
 *		// 9.2��m3
 *		CValueCubic d2(9.2, CValueCubic::MICRO);
 *		TRACE1("%s\n", d2.ToString()); //��m3�ŕ\��
 *		// 9.2���b�g��
 *		d2.Set(9.2, CValueCubic::LITRE);
 *		TRACE1("%s\n", d2.ToString(CValueCubic::M3)); //m3�ŕ\��
 *		// 1.2�o����
 *		d2.Set(1.2, CValueCubic::BBL);
 *		// 100�o����
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitCubic> CValueCubic;

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�̐� = �ʐ� �~ ����)
 *	@param a �ʐ�
 *	@param d ����
 *	@return ��(�P�ʂ͕������[�g���ɂȂ�܂�)
 */
inline CValueCubic operator*(const CValueArea& a, const CValueDistance& d)
{
	return CValueCubic(a.GetValue(a.M2) * d.GetValue(d.M), CValueCubic::M3);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�̐� = ���� �~ �ʐ�)
 *	@param d ����
 *	@param a �ʐ�
 *	@return �̐�(�P�ʂ͕������[�g���ɂȂ�܂�)
 */
inline CValueCubic operator*(const CValueDistance& d, const CValueArea& a)
{
	return a * d;
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�d��)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitWeight : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�d��"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'We' };

	/// �d�ʒP�ʃR�[�h
	enum EUnitCode
	{
		G,				///< �O����
		T,				///< �g��
		OZ,				///< �I���X
		LB,				///< �|���h
		CT,				///< �J���b�g
		KAN,			///< ��
		MONME,			///< ��

		MG		= CPfUnit::MILLI,	///< �~���O����
		KG		= CPfUnit::KIRO,	///< �L���O����
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
			{	_T("g"),		_T("�O����"),			1.0					},	// g
			{	_T("t"),		_T("�g��"),				0.000001			},	// t
			{	_T("oz"),		_T("�I���X"),			35.274 * 0.001		},	// oz
			{	_T("lb"),		_T("�|���h"),			2.20462291 * 0.001	},	// lb
			{	_T("ct"),		_T("�J���b�g"),			1.0 / 0.2			},	// ct
			{	_T("��"),		_T("��"),				0.001 / 3.750		},	// kan
			{	_T("mon"),		_T("��"),				1.0 / 3.750			},	//monme
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �d�ʊǗ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitWeight::EUnitCode �d�ʒP�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�O����({@link CUnitWeight::G CValueWeight::G}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3g
 *		CValueWeight d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (g) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //g�ŕ\��
 *		// 9.2�s�R�O����
 *		CValueWeight d2(9.2, CValueWeight::PICO);
 *		TRACE1("%s\n", d2.ToString()); //pg�ŕ\��
 *		// 1.2�J���b�g
 *		d2.Set(1.2, CValueWeight::CT);
 *		TRACE1("%s\n", d2.ToString(CValueWeight::G)); //g�ŕ\��
 *		// 100�J���b�g
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //ct
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitWeight> CValueWeight;



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(���x)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitTemperature : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("���x"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Te' };

	/// ���x�P�ʃR�[�h
	enum EUnitCode
	{
		C,				///< �ێ�(�Z���V�E�X)���x ��
		F,				///< �؎����x
		K,				///< �P���r��

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
			//	�P�ʕ�����		���{��			�W��		�␳�l
			{	_T("��"),		_T("�ێ�"),		1.0,		0		},	// C
			{	_T("�KF"),		_T("�؎�"),		9.0 / 5.0,	32,		},	// F
			{	_T("K"),		_T("�P���r��"),	1.0,		273.15	},	// K
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ���x�Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitTemperature::EUnitCode ���x�P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A��({@link CUnitTemperature::C CValueTemperature::C}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3��
 *		CValueTemperature d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (��) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //���ŕ\��
 *		// 9.2��F
 *		CValueTemperature d2(9.2, CValueTemperature::F);
 *		TRACE1("%s\n", d2.ToString()); //��F�ŕ\��
 *		// 1.2�P���r��
 *		d2.Set(1.2, CValueTemperature::K);
 *		TRACE1("%s\n", d2.ToString(CValueTemperature::C)); //���ŕ\��
 *		// 100�P���r��
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); // K�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitTemperature> CValueTemperature;



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(����)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitTime : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("����"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Ti' };

	/// ���ԒP�ʃR�[�h
	enum EUnitCode
	{
		SEC,				///< �b
		MIN,				///< ��
		HOUR,				///< ��
		DAY,				///< ��
		WEEK,				///< �T

		MSEC		= CPfUnit::MILLI,	///< �~���b
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
			//	�P�ʕ�����		���{��			�W��
			{	_T("s"),		_T("�b"),		1.0						},	// sec
			{	_T("m"),		_T("��"),		1.0 / 60.0				},	// min
			{	_T("h"),		_T("��"),		1.0 / 3600				},	// hour
			{	_T("day"),		_T("��"),		1.0 / (3600 * 24)		},	// day
			{	_T("week"),		_T("�T"),		1.0 / (3600 * 24 * 7)	},	// week
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ���ԊǗ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitTime::EUnitCode ���ԒP�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�b({@link CUnitTime::SEC CValueTime::SEC}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3�b
 *		CValueTime d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (�b) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //�b�ŕ\��
 *		// 9.2�s�R�b
 *		CValueTime d2(9.2, CValueTime::PICO);
 *		TRACE1("%s\n", d2.ToString()); //�s�R�b�ŕ\��
 *		// 1.2WEEK
 *		d2.Set(1.2, CValueTime::WEEK);
 *		TRACE1("%s\n", d2.ToString(CValueTime::DAY)); //���ŕ\��
 *		// 100WEEK
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //week
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitTime> CValueTime;

/**@ingroup VALUEUNIT
 * �P�ʊǗ�(����^2)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitTime2 : public CAbstractUnit
{
public:
	enum { RASING = 2 };///< �搔�l

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("����^2"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Tt' };

	/// ����^2�P�ʃR�[�h
	enum EUnitCode
	{
		SEC2,				///< �b^2
		MIN2,				///< ��^2
		HOUR2,				///< ��^2
		DAY2,				///< ��^2
		WEEK2,				///< �T^2

		MSEC2	= CPfUnit::MILLI,	///< �~���b^2
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
			//	�P�ʕ�����		���{��			�W��
			{	_T("s^2"),		_T("�b^2"),		1.0								},	// sec
			{	_T("m^2"),		_T("��^2"),		(1.0 / 60.0) * (1.0 / 60.0)		},	// min
			{	_T("h^2"),		_T("��^2"),		(1.0 / 3600) * (1.0 / 3600)		},	// hour
			{	_T("day^2"),	_T("��^2"),		(1.0 / (3600 * 24))	* (1.0 / (3600 * 24))},	// day
			{	_T("week^2"),	_T("�T^2"),		(1.0 / (3600 * 24 * 7)) * (1.0 / (3600 * 24 * 7))},	// week
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ����^2�Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitTime2::EUnitCode ����^2�P�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�b^2({@link CUnitTime2::SEC2 CValueTime2::SEC2}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3�b^2
 *		CValueTime2 d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (�b^2) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //�b^2�ŕ\��
 *		// 9.2�s�R�b^2
 *		CValueTime2 d2(9.2, CValueTime2::PICO);
 *		TRACE1("%s\n", d2.ToString()); //�s�R�b^2�ŕ\��
 *		// 1.2WEEK^2
 *		d2.Set(1.2, CValueTime2::WEEK2);
 *		TRACE1("%s\n", d2.ToString(CValueTime2::DAY2)); //��^2�ŕ\��
 *		// 100WEEK^2
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //week^2
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
typedef CValueUnitT<CUnitTime2> CValueTime2;

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (����^2 = ���� �~ ����)
 *	@param t1 ����
 *	@param t2 ����
 *	@return ����^2(�P�ʂ�SEC^2�ɂȂ�܂�)
 */
inline CValueTime2 operator*(const CValueTime& t1, const CValueTime& t2)
{
	return CValueTime2(t1.GetValue(t1.SEC) * t2.GetValue(t2.SEC), CValueTime2::SEC2);
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���� = ����^2 �� ����)
 *	@param t2 ����^2
 *	@param t ����
 *	@return ����(�P�ʂ�SEC�ɂȂ�܂�)
 */
inline CValueTime operator/(const CValueTime2& t2, const CValueTime& t)
{
	return CValueTime(t2.GetValue(t2.SEC2) / t.GetValue(t.SEC), CValueTime::SEC);
}

/**@ingroup VALUEUNIT
 * [�v�Z] ������ (���� = ��(����^2))
 *	@param t2 ����^2
 *	@return ����(�P�ʂ�SEC�ɂȂ�܂�)
 */
inline CValueTime sqrt(const CValueTime2& t2)
{
	CRealNumber r = sqrt(t2.GetValue(t2.SEC2));
	return CValueTime(r, CValueTime::SEC);
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(���g��)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 �V�K�쐬
 */
class CUnitFrequency : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("���g��"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Fr' };

	/// ���g���P�ʃR�[�h
	enum EUnitCode
	{
		HZ,				///< Hz

		KHZ		= CPfUnit::KIRO,	///< kHz
		MHZ		= CPfUnit::MEGA,	///< MHz
		GHZ		= CPfUnit::GIGA,	///< GHz
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
			//	�P�ʕ�����		���{��				�W��
			{	_T("Hz"),		_T("�w���c"),		1.0			},	// Hz
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ���g���Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitFrequency::EUnitCode ���g���P�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́AHz({@link CUnitFrequency::HZ CValueFrequency::HZ}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3Hz
 *		CValueFrequency d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (Hz) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //Hz�ŕ\��
 *		// 9.2kHz
 *		CValueFrequency d2(9.2, CValueFrequency::KHZ);
 *		TRACE1("%s\n", d2.ToString()); //kHz�ŕ\��
 *		// 1.2MHz
 *		d2.Set(1.2, CValueFrequency::MHZ);
 *		TRACE1("%s\n", d2.ToString(CValueFrequency::HZ)); //hz�ŕ\��
 *		// 100MHz
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //MHz�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 �V�K�쐬
 */
typedef CValueUnitT<CUnitFrequency> CValueFrequency;

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�l = ���g�� �~ ����)
 *	@param f ���g��
 *	@param t ����
 *	@return �l
 */
inline CRealNumber operator*(const CValueFrequency& f, const CValueTime& t)
{
	return f.GetValue(f.HZ) * t.GetValue(t.SEC);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �|�Z (�l = ���� �~ ���g��)
 *	@param t ����
 *	@param f ���g��
 *	@return �l
 */
inline CRealNumber operator*(const CValueTime& t, const CValueFrequency& f)
{
	return f * t;
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���� = �l �� ���g��)
 *	@param v �l
 *	@param f ���g��
 *	@return ����(�P�ʂ͕b�ɂȂ�܂�)
 */
inline CValueTime operator/(const CRealNumber& v, const CValueFrequency& f)
{
	return CValueTime(v / f.GetValue(f.HZ), CValueTime::SEC);
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���Z (���g�� = �l �� ����)
 *	@param v �l
 *	@param t ���ԒP
 *	@return ���g��(�P�ʂ�Hz�ɂȂ�܂�)
 */
inline CValueFrequency operator/(const CRealNumber& v, const CValueTime& t)
{
	return CValueFrequency(v / t.GetValue(t.SEC), CValueFrequency::HZ);
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�p�x)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/18 �V�K�쐬
 *	@date 07/01/22 ���W�A���ǉ�
 */
class CUnitDegree : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�p�x"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'De' };

	/// �p�x�P�ʃR�[�h
	enum EUnitCode
	{
		DEG,				///< �p�x(�x)
		DEGS,				///< �p�x(�b)
		RAD,				///< ���W�A��
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
			//	�P�ʕ�����		���{��			�W��
			{	_T("��"),		_T("�x"),		1.0			},	// DEG
			{	_T("\""),		_T("�b"),		60.0 * 60.0	},	// DEG(�b)
			{	_T(""),			_T(""),			_PAI / 180.0},	// RAD
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �p�x�Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitDegree::EUnitCode �p�x�P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A��(Degree)({@link CUnitDegree::DEG CValueDegree::DEG}) �ł��B
 *	@note �{�N���X�̃C���X�^���X�́A�O�p�֐��usin,sinh,cos,cosh,tan,tanh�v�ɒ��ڈ����œn�����Ƃ��\�ł�
 *			�i�ێ��P�ʂ��p�x�̏ꍇ�A���W�A���ϊ����Čv�Z���܂��j�B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3��
 *		CValueDegree d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (��) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //���ŕ\��
 *		TRACE1("%f\n", sin(d1));		// sin(123.3��)���v�Z
 *		// 3453.4�b
 *		CValueDegree d2(2453.4, CValueDegree::DEGS);
 *		TRACE1("%s\n", d2.ToString()); //�b�ŕ\��
 *		// 1.2�iRAD�j
 *		d2.Set(1.2, CValueDegree::RAD);
 *		TRACE1("%s\n", d2.ToString(CValueDegree::DEG)); //���ŕ\��
 *		// .5(RAD)
 *		d2.Set(.5); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //���W�A���ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 �V�K�쐬
 */
typedef CValueUnitT<CUnitDegree> CValueDegree;

/**@ingroup VALUEUNIT
 * [�v�Z] sin (�l = sin(�p�x))
 *	@param d �p�x
 *	@return �l
 */
inline CRealNumber sin(const CValueDegree& d) { return sin(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [�v�Z] sinh (�l = sinh(�p�x))
 *	@param d �p�x
 *	@return �l
 */
inline CRealNumber sinh(const CValueDegree& d) { return sinh(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [�v�Z] cos (�l = cos(�p�x))
 *	@param d �p�x
 *	@return �l
 */
inline CRealNumber cos(const CValueDegree& d) { return cos(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [�v�Z] cosh (�l = cosh(�p�x))
 *	@param d �p�x
 *	@return �l
 */
inline CRealNumber cosh(const CValueDegree& d) { return cosh(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [�v�Z] tan (�l = tan(�p�x))
 *	@param d �p�x
 *	@return �l
 */
inline CRealNumber tan(const CValueDegree& d) { return tan(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [�v�Z] tanh (�l = tanh(�p�x))
 *	@param d �p�x
 *	@return �l
 */
inline CRealNumber tanh(const CValueDegree& d) { return tanh(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [�v�Z] �p�x(�x,��,�b)���.
 *	@note ���̊p�x������ꍇ�Add�𕉂ɂ��Ă��������B
 *	@param[in] dd �x
 *	@param[in] mm ��
 *	@param[in] ss �b
 *	@return �p�x
 */
inline CValueDegree MakeDegree(int dd, int mm = 0, double ss = 0.0)
{
	if ( ss < 0.0 )
	{
		ss = -ss;
	}
	double r = abs(dd) * 60 * 60 + abs(mm * 60) + ss;
	if ( dd < 0 )
	{
		r = -r;
	}
	return CValueDegree(r, CValueDegree::DEGS);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �p�x(�x,��,�b)�v�Z.
 *	@param[in] dd �x
 *	@param[in] mm ��
 *	@param[in] ss �b
 *	@return �p�x
 */
inline CValueDegree MakeDegree(BYTE dd, BYTE mm, double ss)
{
	double r = dd * 60 * 60 + mm * 60 + ss;
	return CValueDegree(r, CValueDegree::DEGS);
}

/**@ingroup VALUEUNIT
 * [�v�Z] ���p�O�p�`�̍����v�Z
 *	@param di ��Ӓ���
 *	@param de �p�x�i��ӂɐڂ��钼�p�ȊO�̊p�x�j
 *	@return ���p�O�p�`�̍���
 */
inline CValueDistance CalcRightTriangleHeight(const CValueDistance& di, const CValueDegree& de)
{
	CValueDistance::CCode c = di.GetUnitCode();
	return CValueDistance(di.GetValue(c) * tan(de), c);
}

/**@ingroup VALUEUNIT
 * [�v�Z] �p�x�̕��όv�Z.
 *	@note �P�ʃx�N�g�����Z�@�ɂ��v�Z�B
 *	@param size �z��
 *	@param pDegs �p�x�P�ʂ̔z��
 *	@return ���ϊp (�P�� DEG)
 */
inline CValueDegree CalcDegreeAverage(size_t size, const CValueDegree* pDegs)
{
	CRealNumber x = 0;
	CRealNumber y = 0;
	loop ( i, size )
	{
		x += sin(pDegs[i]);
		y += cos(pDegs[i]);
	}
	CValueDegree d;
	d.Set(atan2(x, y), CValueDegree::RAD);
	return d;
}

/**@ingroup VALUEUNIT
 * [�v�Z] �p�x�̕��όv�Z.
 *	@note �P�ʃx�N�g�����Z�@�ɂ��v�Z�B
 *	@param d1 �p�x�P
 *	@param d2 �p�x�Q
 *	@param r1 �p�x�P�̔䗦�B�ȗ������ 1.0�B
 *	@param r2 �p�x�Q�̔䗦�B�ȗ������ 1.0�B
 *	@return ���ϊp
 */
inline CValueDegree CalcDegreeAverage(const CValueDegree& d1, const CValueDegree& d2, double r1 = 1.0, double r2 = 1.0)
{
	CValueDegree::CCode c = d1.GetUnitCode();
	CRealNumber x = sin(d1) * r1 + sin(d2) * r2;
	CRealNumber y = cos(d1) * r1 + cos(d2) * r2;
	CValueDegree d;
	d.Set(atan2(x, y), CValueDegree::RAD);
	d.ToUnit(c);
	return d;
}

/**@ingroup VALUEUNIT
 * [�v�Z] �p�x�̊J���v�Z.
 *	@note �K���O�`180���ɂȂ�܂��B
 *	@param d1 �p�x�P
 *	@param d2 �p�x�Q
 *	@return �J���p�x(�P�� DEG)
 */
inline CValueDegree CalcDegreeDifference(const CValueDegree& d1, const CValueDegree& d2)
{
	CRealNumber v1 = d1.GetValue(CValueDegree::DEG);
	CRealNumber v2 = d2.GetValue(CValueDegree::DEG);
	CValueDegree d;
	v1 = fmod(v1 + 360.0 * 1000.0, 360.0);
	v2 = fmod(v2 + 360.0 * 1000.0, 360.0);
	v1 = fabs(v1 - v2);
	if ( v1 > 180.0 )
	{
		v1 = 360.0 - v1;
	}
	d.Set(v1, CValueDegree::DEG);
	return d;
}



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(����)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/18 �V�K�쐬
 */
class CUnitPressure : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("����"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Pr' };

	/// ���͒P�ʃR�[�h
	enum EUnitCode
	{
		MMHG,		///< mmHg
		TORR,		///< Torr
		INHG,		///< inHg
		PA,			///< �p�X�J��
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
			//	�P�ʕ�����		���{��						�W��
			{	_T("mmHg"),		_T("���⒌�~�����[�g��"),	1.0			},	// mmHg
			{	_T("Torr"),		_T("�g��"),					1.0			},	// Torr
			{	_T("inHg"),		_T("���⒌�C���`"),			3.937E-02	},	// inHg
			{	_T("Pa"),		_T("�p�X�J��"),				133.3224	},	// �p�X�J��
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * ���͊Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitPressure::EUnitCode ���͒P�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A���⒌mm({@link CUnitPressure::MMHG CValuePressure::MMHG}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3���⒌mm
 *		CValuePressure d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (���⒌mm) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //���⒌mm�ŕ\��
 *		// 9.2Torr
 *		CValuePressure d2(9.2, CValuePressure::Torr);
 *		TRACE1("%s\n", d2.ToString()); //Torr�ŕ\��
 *		// 1.2Pa
 *		d2.Set(1.2, CValuePressure::PA);
 *		TRACE1("%s\n", d2.ToString(CValuePressure::MMHG)); //mmHg�ŕ\��
 *		// 100pa
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //pa�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 �V�K�쐬
 */
typedef CValueUnitT<CUnitPressure> CValuePressure;



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(����)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/18 �V�K�쐬
 */
class CUnitRatio : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("����"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'Re' };

	/// �����P�ʃR�[�h
	enum EUnitCode
	{
		PERCENT,				///< �p�[�Z���g
		WARI,					///< ��
		DEC,					///< �����_
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
			//	�P�ʕ�����		���{��				�W��
			{	_T("%"),		_T("�p�[�Z���g"),	1.0			},	// ��
			{	_T("��"),		_T("��"),			0.1			},	// ��
			{	_T(""),			_T(""),				0.01		},	// ��
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �����Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitRatio::EUnitCode �����P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�p�[�Z���g({@link CUnitRatio::PERCENT CValueRatio::PERCENT}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3��
 *		CValueRatio d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (��) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //���ŕ\��
 *		// 9.2��
 *		CValueRatio d2(9.2, CValueRatio::WARI);
 *		TRACE1("%s\n", d2.ToString()); //���ŕ\��
 *		// 1.2
 *		d2.Set(1.2, CValueRatio::DEC);
 *		TRACE1("%s\n", d2.ToString(CValueRatio::PERCENT)); //���ŕ\��
 *		// 100.0
 *		d2.Set(100.0); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); // 100.0 �ƕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 �V�K�쐬
 */
typedef CValueUnitT<CUnitRatio> CValueRatio;



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�d��)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 �V�K�쐬
 *	@date 09/02/10 Bi �ǉ�
 */
class CUnitAmpere : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�d��"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'A' };

	/// �d���P�ʃR�[�h
	enum EUnitCode
	{
		A,				///< A (�A���y�A)
		BI,				///< Bi (�r�I)
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
			//	�P�ʕ�����		���{��				�W��
			{	_T("A"),		_T("�A���y�A"),		1.0			},	// A
			{	_T("Bi"),		_T("�r�I"),			0.1			},	// Bi
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �d���Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitAmpere::EUnitCode �d���P�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�A���y�A({@link CUnitAmpere::A CValueAmpere::A}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3A
 *		CValueAmpere d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (A) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //A�ŕ\��
 *		// 9.2��A
 *		CValueAmpere d2(9.2, CValueAmpere::MICRO);
 *		TRACE1("%s\n", d2.ToString()); //��A�ŕ\��
 *		// 0.2Bi
 *		d2.Set(0.2, CValueAmpere::BI);
 *		TRACE1("%s\n", d2.ToString(CValueAmpere::A)); //A�ŕ\��
 *		// 100Bi
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //Bi�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 �V�K�쐬
 */
typedef CValueUnitT<CUnitAmpere> CValueAmpere;



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�d��)�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 �V�K�쐬
 */
class CUnitVolt : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T("�d��"); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = 'V' };

	/// �d���P�ʃR�[�h
	enum EUnitCode
	{
		V,				///< V (�{���g)

		MV		= CPfUnit::MILLI,	///< mV (�~���{���g)
		KV		= CPfUnit::KIRO,	///< kV (�L���{���g)
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
			//	�P�ʕ�����		���{��				�W��
			{	_T("V"),		_T("�{���g"),		1.0			},	// V
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �d���Ǘ��N���X
 *
 *	@note �P�ʂƂ��� {@link CUnitVolt::EUnitCode �d���P�ʃR�[�h} �� 
 *					{@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h} ���g�p�ł��܂��B
 *	@note {@link CPfUnit::EPfUnitCode �ړ��P�ʃR�[�h}�́A��P�ʂ���ɂ��܂��B
 *	@note ��P�ʂ́A�{���g({@link CUnitVolt::V CValueVolt::V}) �ł��B
 *
 *	@see �g�p�ł��郁�\�b�h�� CValueUnitT ���Q�Ƃ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *		// 123.3V
 *		CValueVolt d1(123.3); //�R���X�g���N�^�ŒP�ʏȗ�����Ɗ�{�P�� (V) �ɂȂ�܂��B		
 *		TRACE1("%s\n", d1.ToString()); //V�ŕ\��
 *		// 9.2�s�RV
 *		CValueVolt d2(9.2, CValueVolt::PICO);
 *		TRACE1("%s\n", d2.ToString(CValueVolt::MV)); //MV�ŕ\��
 *		// 100�s�RV
 *		d2.Set(100); //�P�ʂ��ȗ�����Ɛ��l�����̕ύX�ɂȂ�܂��B
 *		TRACE1("%s\n", d2.ToString()); //�s�RV�ŕ\��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 �V�K�쐬
 */
typedef CValueUnitT<CUnitVolt> CValueVolt;



/**@ingroup VALUEUNIT
 * �P�ʊǗ�(�_�~�[)�e���v���[�g�N���X
 *
 *	@attention �P�ʊǗ��p�̃N���X�ł��B���ڎg���P�[�X�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/22 �V�K�쐬
 */
class CUnitDummy : public CAbstractUnit
{
public:

	/**
	 * [�擾] �P�ʎ핶����擾
	 *	@return �P�ʎ핶����
	 */
	static LPCTSTR GetKindString(void) { return _T(""); }

	/// �P�ʎ�R�[�h
	enum { KINDCODE = ' ' * 0x100 };

	/// �d���P�ʃR�[�h
	enum EUnitCode
	{
		NON,				///< �P�ʂȂ�
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
			//	�P�ʕ�����		���{��				�W��
			{	_T(""),			_T(""),				1.0			},
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * �P�ʂȂ��Ǘ��N���X
 *
 *	@note	�P�ʂȂ��̒l�������������Ɏg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbValueUnit.h
 * 
 *	@date 07/01/22 �V�K�쐬
 */
typedef CValueUnitT<CUnitDummy> CValueNonUnit;



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	#undef _S2		
	#undef _M3		
	#undef _S3		
	#undef _LITRE	
	#undef _DL		
#endif



}; // TNB
