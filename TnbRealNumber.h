#pragma once
/**
 *@file
 * �����Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



#include <float.h>
#include <math.h>



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * �����Ǘ��N���X
 *
 *		double �Ə�Ԃƃp�����[�^�������܂��B��Ԃ͒l�Ȃ��A�����l�A�G���[��Ԃ�ێ����܂��B
 *		�p�����[�^��16�̃t���O�ł��B
 *
 *		�����Ǘ��N���X���m���v�Z����ƁA��Ԃƃp�����[�^���v�Z����܂��B���Ƃ��΁A
 *		�����Ȓl�Ƒ��̏�Ԃ̒l���v�Z����ƁA���ʂ͖����Ȓl�ɂȂ�܂��B
 *		�p�����[�^�́A or ����A���ʂƂ��܂��B
 *
 *	@note �p�����[�^�͖{�N���X�ł� or �v�Z����ȊO�A�g�p���Ă��܂���B
 *			�g�p����v���W�F�N�g�ŔC�ӂł��B
 *
 *	@par�Ή��֐�
 *		<table><tr><td><pre>
 *		CRealNumber	acos(CRealNumber _X);
 *		CRealNumber	asin(CRealNumber _X);
 *		CRealNumber	atan(CRealNumber _X);
 *		CRealNumber	atan2(CRealNumber _Y, CRealNumber _X);
 *		CRealNumber	cos(CRealNumber _X);
 *		CRealNumber	cosh(CRealNumber _X);
 *		CRealNumber	exp(CRealNumber _X);
 *		CRealNumber	fabs(CRealNumber _X);
 *		CRealNumber	fmod(CRealNumber _X, CRealNumber _Y);
 *		CRealNumber	log(CRealNumber _X);
 *		CRealNumber	log10(CRealNumber _X);
 *		CRealNumber	pow(CRealNumber _X, CRealNumber _Y);
 *		CRealNumber	sin(CRealNumber _X);
 *		CRealNumber	sinh(CRealNumber _X);
 *		CRealNumber	tan(CRealNumber _X);
 *		CRealNumber	tanh(CRealNumber _X);
 *		CRealNumber	sqrt(CRealNumber _X);
 *		CRealNumber	ceil(CRealNumber _X);
 *		CRealNumber	floor(CRealNumber _X);
 *		CRealNumber	frexp(CRealNumber _X, __out int * _Y);
 *		CRealNumber	_hypot(CRealNumber _X, CRealNumber _Y);
 *		CRealNumber	_j0(CRealNumber _X );
 *		CRealNumber	_j1(CRealNumber _X );
 *		CRealNumber	_jn(int _X, CRealNumber _Y);
 *		CRealNumber	ldexp(CRealNumber _X, int _Y);
 *		CRealNumber	modf(CRealNumber _X, __out CRealNumber * _Y);
 *		CRealNumber	_y0(CRealNumber _X);
 *		CRealNumber	_y1(CRealNumber _X);
 *		CRealNumber	_yn(int _X, CRealNumber _Y);
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbRealNumber.h
 *
 *	@date 06/11/18	�V�K�쐬
 */
class CRealNumber
{
public:

	/// ���
	enum EStatus
	{
		ES_Valid,				///< �L�����
		ES_Empty,				///< Empty���
		ES_Invalid,				///< �������
		ES_ZeroDivied,			///< Zero Divied���
		ES_OverFlow,			///< Overflow���
	};

private:

	double	m_doValue;			///< ���ۂ̒l
	WORD	m_wStatus;			///< �l�̏�ԏ��
	WORD	m_wParam;			///< �l�̕t�����

	/**
	 * [�m�F] �l�`�F�b�N.
	 *		�w�肳�ꂽ�l���`�F�b�N���A��Ԃ��Đݒ肵�܂��B
	 *	@param[in] value �l�B
	 *	@param[in,out] _status ��ԁB
	 */
	static void ms_CheckValue(double value, WORD& _status)
	{
		if ( _status == ES_Valid )
		{
			if ( ::_isnan(value) != 0 )
			{
				//�ُ�Ȓl��
				_status = ES_Invalid;
			}
			else if ( ::_finite(value) == 0 )
			{
				//�I�[�o�[�t���[�����Ă���
				_status = ES_OverFlow;
			}
		}
	}

	/**
	 * [�v�Z] ��̏�Ԍv�Z.
	 *		�E�ӂƍ��ӂ̏�Ԃ��v�Z���A��ɂ��܂��B
	 *	@param left ���ӂ̏��
	 *	@param right �E�ӂ̏��
	 *	@return ���킹���񂾏�ԁB
	 */
	static WORD ms_CalcStatus(WORD left, WORD right)
	{
		if ( left == ES_Empty || right == ES_Empty )
		{
			// �Е��ł��G���v�e�B�Ȃ�G���v�e�B
			left = ES_Empty;
		}
		else if ( left == ES_Invalid || right == ES_Invalid )
		{
			// �Е��ł��s���Ȃ猋�ʂ͕s��
			left = ES_Invalid;
		}
		else if ( left == ES_Valid )
		{
			//���ӂ��L���Ȃ猋�ʂ͉E�ӂ̏��
			left = right;
		}
		else if ( right == ES_Valid )
		{
			//�E�ӂ��L���Ȃ猋�ʂ͍��ӂ̏��
		}
		else
		{
			//���炩�̃G���[��
			left = ES_Invalid;
		}
		return left;
	}

	/**
	 * [�m�F] �l�`�F�b�N.
	 *		�w�肳�ꂽ�l���`�F�b�N���A��Ԃ��Đݒ肵�܂��B
	 */
	void m_CheckValue(void)
	{
		ms_CheckValue(m_doValue, m_wStatus);
	}

	/**
	 * �R���X�g���N�^
	 *	@note �w��̏�ԂɂȂ�܂��B
	 */
	CRealNumber(double value, int status, int param) 
		: m_doValue(value), m_wStatus(static_cast<WORD>(status)), m_wParam(static_cast<WORD>(param))
	{
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@note ���ԂɂȂ�܂��B
	 */
	CRealNumber(void) : m_doValue(0), m_wStatus(ES_Empty), m_wParam(0)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[���B
	 */
	CRealNumber(const CRealNumber& other) 
		: m_doValue(other.m_doValue), m_wStatus(other.m_wStatus), m_wParam(other.m_wParam)
	{
	}

	/**
	 * ����R���X�g���N�^
	 *	@note �w��̒l��ێ����܂��B
	 *	@param value �l�B
	 */
	CRealNumber(double value) : m_doValue(value), m_wStatus(ES_Valid), m_wParam(0)
	{
		m_CheckValue();
	}

	/**
	 * ����R���X�g���N�^
	 *	@note �w��̒l��ێ����܂��B
	 *	@param value �l�B
	 */
	CRealNumber(int value) : m_doValue(value), m_wStatus(ES_Valid), m_wParam(0)
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *	@note ������𐔒l�ɕϊ������l��ێ����܂��BEmpty������Ȃ�A���ԂɂȂ�܂��B
	 *	@param str ������B
	 */
	CRealNumber(const CStr& str) : m_doValue(0), m_wStatus(ES_Empty), m_wParam(0)
	{
		if ( ! str.IsEmpty() )
		{
			m_doValue = str.ToDouble();
			m_wStatus = ES_Valid;
			m_CheckValue();
		}
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[���B
	 *	@return �����̎Q�ƁB
	 */
	CRealNumber& operator=(const CRealNumber& other)
	{
		m_doValue = other.m_doValue;
		m_wStatus = other.m_wStatus;
		m_wParam = other.m_wParam;
		return *this;
	}

	/**
	 * [���] ���
	 *	@note �w��̒l��ێ����܂��B
	 *	@param value �l�B
	 *	@return �����̎Q�ƁB
	 */
	CRealNumber& operator=(double value)
	{
		m_doValue = value;
		m_wStatus = ES_Valid;
		m_wParam = 0;
		m_CheckValue();
		return *this;
	}

	/**
	 * [���] ���
	 *	@note �w��̒l��ێ����܂��B
	 *	@param value �l�B
	 *	@return �����̎Q�ƁB
	 */
	CRealNumber& operator=(int value)
	{
		m_doValue = value;
		m_wStatus = ES_Valid;
		m_wParam = 0;
		m_CheckValue();
		return *this;
	}

	/**
	 * [���] ���
	 *	@note ������𐔒l�ɕϊ������l��ێ����܂��BEmpty������Ȃ�A���ԂɂȂ�܂��B
	 *	@param str ������
	 *	@return �����̎Q�ƁB
	 */
	CRealNumber& operator=(const CStr& str)
	{
		if ( ! str.IsEmpty() )
		{
			m_doValue = str.ToDouble();
			m_wStatus = ES_Valid;
			m_CheckValue();
		}
		else
		{
			Empty();
		}
	}

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	//---------------------------------
	//
	//	����n�`�[��
	//
	#define FUNCMAC2(EQ, DST, VL)										\
		CRealNumber& operator EQ (DST){									\
			m_doValue EQ VL.m_doValue;									\
			m_wStatus = ms_CalcStatus(m_wStatus, VL.m_wStatus);			\
			m_wParam |= VL.m_wParam;									\
			m_CheckValue();												\
			return *this;												\
		}

	#define FUNCMAC1(DST, VL)											\
		FUNCMAC2( +=, DST, VL );										\
		FUNCMAC2( -=, DST, VL );										\
		FUNCMAC2( *=, DST, VL );										\
		CRealNumber& operator/=(DST) {									\
			if ( VL.m_doValue != 0.0 ) {								\
				m_doValue /= VL.m_doValue;								\
				m_wStatus = ms_CalcStatus(m_wStatus, VL.m_wStatus);		\
				m_wParam |= VL.m_wParam;								\
			} else {													\
				m_doValue = 0;											\
				if ( VL.m_wStatus == ES_Empty ) {						\
					m_wStatus = ms_CalcStatus(m_wStatus, ES_Empty);		\
				} else {												\
					m_wStatus = ms_CalcStatus(m_wStatus, ES_ZeroDivied);\
				}														\
			}															\
			m_CheckValue();												\
			return *this;												\
		}

	FUNCMAC1( const CRealNumber& value, value );
	FUNCMAC1( double doValue, CRealNumber(doValue) );
	FUNCMAC1( int iValue, CRealNumber(iValue) );

	#undef FUNCMAC1
	#undef FUNCMAC2

	//-------------------------------------
	//
	//	���Z�̉ԓ�
	//	
	#define FUNCMAC2(EQ, DST, VL)										\
		CRealNumber	operator EQ (DST) const {							\
			CRealNumber r = *this;										\
			r EQ##= VL;													\
			return r;													\
		}

	#define FUNCMAC1(DST, VL)											\
		FUNCMAC2( +, DST, VL )											\
		FUNCMAC2( -, DST, VL )											\
		FUNCMAC2( *, DST, VL )											\
		FUNCMAC2( /, DST, VL )

	FUNCMAC1( const CRealNumber& value, value );
	FUNCMAC1( double doValue, CRealNumber(doValue) );
	FUNCMAC1( int iValue, CRealNumber(iValue) );

	#undef FUNCMAC1
	#undef FUNCMAC2

	#define FUNCMAC2(EQ,DST,VL)											\
		friend CRealNumber operator EQ (DST, const CRealNumber& value){	\
			CRealNumber r = VL;											\
			r EQ##= value;												\
			return r;													\
		}//---------------------------------------------------------------

	#define FUNCMAC1(DST, VL)											\
		FUNCMAC2( +, DST, VL );											\
		FUNCMAC2( -, DST, VL );											\
		FUNCMAC2( *, DST, VL );											\
		FUNCMAC2( /, DST, VL );

	FUNCMAC1( double doValue, doValue )
	FUNCMAC1( int iValue, iValue )

	#undef FUNCMAC1
	#undef FUNCMAC2

	//---------------------------------------------
	//
	//	�֐�
	//
	#define FUNCMAC1(CMD)											\
		friend CRealNumber CMD(const CRealNumber& value) {			\
			if ( value.m_wStatus == ES_Valid ) {					\
				CRealNumber r(::CMD(value.m_doValue),				\
								value.m_wStatus, value.m_wParam);	\
				r.m_CheckValue();									\
				return r;											\
			}														\
			return CRealNumber(0, value.m_wStatus, value.m_wParam);	\
		}															\
		friend double CMD(double value) { return ::CMD(value); }

	FUNCMAC1( acos );
	FUNCMAC1( asin );
	FUNCMAC1( atan );
	FUNCMAC1( cos );
	FUNCMAC1( cosh );
	FUNCMAC1( exp );
	FUNCMAC1( fabs );
	FUNCMAC1( log );
	FUNCMAC1( log10 );
	FUNCMAC1( sin );
	FUNCMAC1( sinh );
	FUNCMAC1( tan );
	FUNCMAC1( tanh );
	FUNCMAC1( sqrt );
	FUNCMAC1( ceil );
	FUNCMAC1( floor );
	FUNCMAC1( _j0 );
	FUNCMAC1( _j1 );
	FUNCMAC1( _y0 );
	FUNCMAC1( _y1 );

	#undef FUNCMAC1

	#define FUNCMAC1(CMD, DSP, VL)										\
		friend CRealNumber CMD(const CRealNumber& value, DSP) {			\
			double d = 0;												\
			if ( value.m_wStatus == ES_Valid && VL.m_wStatus == ES_Valid ){\
				d = ::CMD(value.m_doValue, VL.m_doValue);				\
			}															\
			CRealNumber r(d, ms_CalcStatus(value.m_wStatus, VL.m_wStatus),\
							value.m_wParam|VL.m_wParam);				\
			r.m_CheckValue();											\
			return r;													\
		}

	#define FUNCMAC2(CMD)												\
		FUNCMAC1( CMD, const CRealNumber& value2, value2 );				\
		FUNCMAC1( CMD, double doValue, CRealNumber(doValue) );			\
		FUNCMAC1( CMD, int iValue, CRealNumber(iValue) );				\
		friend double CMD(double v1, double v2) { return ::CMD(v1, v2); }

	FUNCMAC2( atan2 );
	FUNCMAC2( fmod );
	FUNCMAC2( pow );
	FUNCMAC2( _hypot );

	#undef FUNCMAC2
	#undef FUNCMAC1

	friend CRealNumber frexp(const CRealNumber& value, int* _Y)
	{
		CRealNumber r(::frexp(value.m_doValue, _Y), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double frexp(double value, int* _Y)
	{
		return ::frexp(value, _Y);
	}

	friend CRealNumber _jn(int _X, const CRealNumber& value)
	{
		CRealNumber r(::_jn(_X, value.m_doValue), value.m_wStatus,value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double _jn(int _X, double value)
	{
		return ::_jn(_X, value);
	}

	friend CRealNumber ldexp(const CRealNumber& value, int iValue)
	{
		CRealNumber r(::ldexp(value.m_doValue, iValue), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double ldexp(double value, int iValue)
	{
		return ::ldexp(value, iValue);
	}

	friend CRealNumber modf(const CRealNumber& value, double* _Y)
	{
		CRealNumber r(::modf(value.m_doValue, _Y), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double modf(double value, double* _Y)
	{
		return ::modf(value, _Y);
	}

	friend CRealNumber _yn(int _X, const CRealNumber& value)
	{
		CRealNumber r(::_yn(_X, value.m_doValue), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double _yn(int _X, double value)
	{
		return ::_yn(_X, value);
	}

	//---------------------------------------------
	//
	//	��r(�L)
	//
	#define FUNCMAC2(EQ,TY,VL)										\
		friend bool operator EQ(TY VL##1, TY VL##2) {				\
			if ( VL##1.m_wStatus != ES_Valid ){ return false; }		\
			if ( VL##2.m_wStatus != ES_Valid ){ return false; }		\
			return VL##1.m_doValue EQ VL##2.m_doValue;				\
		}															\
		friend bool operator EQ(TY VL, double d) {					\
			return operator EQ(VL, CRealNumber(d));					\
		}															\
		friend bool operator EQ(TY VL, int d) {						\
			return operator EQ(VL, CRealNumber(d));					\
		}															
	#define FUNCMAC1(TY, VL)										\
		friend bool operator==(TY VL##1, TY VL##2) {				\
			if ( VL##1.m_wStatus != VL##2.m_wStatus ){ return false; }	\
			if ( VL##1.m_wStatus != ES_Valid && VL##1.m_wStatus == VL##2.m_wStatus ){ return true; }\
			return VL##1.m_doValue == VL##2.m_doValue;				\
		}															\
		friend bool operator==(TY VL, double d) {					\
			return operator==(VL, CRealNumber(d));					\
		}															\
		friend bool operator==(TY VL, int d) {						\
			return operator==(VL, CRealNumber(d));					\
		}															\
		friend bool operator!=(TY VL##1, TY VL##2) {				\
			return ! operator==(VL##1, VL##2);						\
		}															\
		friend bool operator!=(TY VL, double d) {					\
			return operator!=(VL, CRealNumber(d));					\
		}															\
		friend bool operator!=(TY VL, int d) {						\
			return operator!=(VL, CRealNumber(d));					\
		}															\
		FUNCMAC2( <, TY, VL );										\
		FUNCMAC2( <=, TY, VL );										\
		FUNCMAC2( >, TY, VL );										\
		FUNCMAC2( >=, TY, VL );

	FUNCMAC1( const CRealNumber&, value );

	#undef FUNCMAC1
	#undef FUNCMAC2

	#endif // _TnbDOXYGEN

	//------------------------------------------


	/**
	 * [�擾] double�^�擾.
	 *		double�̌`���ɒ��o���܂��B
	 *	@return �l
	 */
	double ToDouble(void) const
	{
		#ifdef _DEBUG
			if ( m_wStatus != ES_Valid )
			{
				ASSERT1(false, "CRealNumber::ToDouble()", "��Ԃ� VALID(0)�ȊO�ł� (r=%u) �B", m_wStatus);
				return 0;
			}
		#endif
		return m_doValue;
	}

	/**
	 * [�擾] double�^�擾.
	 *		double�̌`���ɒ��o���܂��B
	 *	@return �l
	 */
	operator double() const
	{
		return ToDouble();
	}

	/**
	 * [�擾] int�^�擾.
	 *		int�̌`���ɒ��o���܂��B
	 *	@return �l
	 */
	int ToInt(void) const
	{
		return TNB::ToInt(ToDouble());
	}

	/**
	 * [�ݒ�] Empty.
	 *		EMPTY ��Ԃɂ��܂��B
	 */
	void Empty(void)
	{
		m_doValue = 0;
		m_wStatus = ES_Empty;
		m_wParam = 0;
	}

	/**
	 * [�m�F] �L���m�F
	 *	@retval true �l�L��
	 *	@retval false �L���ȊO
	 */
	bool IsValid(void) const
	{
		return m_wStatus == ES_Valid;
	}

	/**
	 * [�m�F] EMPTY�m�F
	 *	@retval true EMPTY
	 *	@retval false EMPTY�ȊO
	 */
	bool IsEmpty(void) const
	{
		return m_wStatus == ES_Empty;
	}

	/**
	 * [�m�F] ��Ԋm�F
	 *	@return ���
	 */
	EStatus GetStatus(void) const
	{
		return static_cast<EStatus>(m_wStatus);
	}

	/**
	 * [�ݒ�] ��Ԑݒ�
	 *	@note �l�͂O�ɂȂ�܂��B�p�����[�^�͕ω����܂���B
	 *	@param status ���
	 */
	void SetStatus(EStatus status)
	{
		m_doValue = 0;
		m_wStatus = static_cast<WORD>(status);
	}

	/**
	 * [�擾] �p�����[�^�擾
	 *	@return �p�����[�^
	 */
	WORD GetParam(void) const
	{
		return m_wParam;
	}

	/**
	 * [�擾] �p�����[�^�Q��
	 *	@note �C���X�^���X���̃p�����[�^�̎Q�Ƃ������܂��B�ύX���\�ł��B
	 *	@return �p�����[�^
	 */
	WORD& RefParam(void)
	{
		return m_wParam;
	}

	/**
	 * [�ݒ�] �p�����[�^�ݒ�
	 *	@param wParam �p�����[�^
	 */
	void SetParam(WORD wParam)
	{
		m_wParam = wParam;
	}

	/**
	 * [�m�F] ��r
	 *	@note �l���������A���ׂ܂��B���A == , != , <= �Ȃǂ̔�r�����g�p�\�ł��B 
	 *	@param r ��r�Ώےl�B
	 *	@retval true �����B
	 *	@retval true �قȂ�B
	 */
	bool IsEqual(const CRealNumber& r) const
	{
		return *this == r;
	}

	/**
	 * [�m�F] �����܂���r. 
	 *		�l���قړ������A���ׂ܂��B
	 *	@note ���ۂ̒l��B���x�Ŋ���A�����_�Ɉȉ����폜����r���Ă��܂��B
	 *	@param r ��r�Ώےl�B
	 *	@param doDepth �B���x�B �ȗ������ 0.01 ���w�肳��܂��B�܂�A�����_��Q�܂łŔ�r�B
	 *	@retval true �����B
	 *	@retval true �قȂ�B
	 */
	bool IsAlmostEqual(const CRealNumber& r, double doDepth = 0.01) const
	{
		if ( m_wStatus != r.m_wStatus ){ return false; }
		if ( m_wStatus != ES_Valid && m_wStatus == r.m_wStatus ){ return true; }
		//TRACE2("IsAlmostEqual(%f,%f)\n",::floor(m_doValue * doDepth + 0.5) ,::floor(r.m_doValue * doDepth + 0.5));
		return ::floor(m_doValue / doDepth + 0.5) == ::floor(r.m_doValue / doDepth + 0.5);
	}

	/**
	 * [�擾] ������쐬
	 *	@param lpszFormat sprintf()�̃t�H�[�}�b�g�B ��f ��K������邱�ƁB
	 *	@return ������B
	 */
	CStr ToString(LPCTSTR lpszFormat = NULL) const
	{
		CStr s;
		switch ( m_wStatus )
		{
		case ES_Valid: // �L���Ȓl��ێ�
			if ( lpszFormat == NULL || lpszFormat[0] == 0 )
			{
				lpszFormat = _T("%f");
			}
			s.Format(lpszFormat, m_doValue);
			break;
		case ES_Empty: // �l�Ȃ�
			break;
		case ES_Invalid: // �����Ȓl��ێ�
			s = _T("-Invalid-");
			break;
		case ES_ZeroDivied: // Zero Divied���
			s = _T("-ZeroDivied-");
			break;
		case ES_OverFlow: // Overflow���
			s = _T("-OverFlow-");
			break;
		default:
			ASSERT(false);
			break;
		}
		return s;
	}
};



}; // TNB

