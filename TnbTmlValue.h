#pragma once
/**
 *@file
 * TinyMacroLang �l�֌W�̃w�b�_
 *	
 *		TML�̒l�Ǘ��̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAttributedVector.h"
#include "TnbDummyCollection.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang �l�Ǘ�
 *
 *		�f�[�^(������)�A�����A�����AEmpty �̂S��ނ̌^�ŏ��Ǘ����\�ł��B
 *
 *	@note �f�[�^(������)�̏ꍇ�A�����Ƀ��C���h�J�[�h�����Ǘ��ł��܂��B
 *
 *	@note �^�ɂ͋���������A�f�[�^(������)�A�����A�����AEmpty �̏��ԂƂȂ��Ă��܂��B
 *		�@�Q�����Z�̍ہA�����ق��ɂ��킹�ď��������āA���ʂ����̌^�ɂȂ�悤�ɂȂ��Ă��܂��B
 *			���Ƃ��΁A ���� + �f�[�^ �Ȃ�A�������f�[�^(������)�ɕϊ����Ă��珈�������āA���ʂ̓f�[�^�ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlValue.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTmlValue : public IComparableT<CTmlValue>
{
public:

	/// ���
	enum EKind
	{
		EMPTY	= 0,	///< �Ȃ�
		INTEGER	= 1,	///< �����l
		FLOAT	= 2,	///< �����l
		DATA	= 3		///< �f�[�^
	};

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other
	 */
	CTmlValue(const CTmlValue& other) : m_param(other.m_param)
	{
	}

	/// �R���X�g���N�^
	CTmlValue(void) : m_param()
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param i ����
	 */
	CTmlValue(int i) : m_param()
	{
		m_param.Set(i);
	}

	/**
	 * �R���X�g���N�^
	 *	@param i ����
	 */
	CTmlValue(double i) : m_param()
	{
		m_param.Set(i);
	}

	/**
	 * [���] �R�s�[�I�y���[�^.
	 *	@param v �R�s�[��
	 *	@return �����̎Q��
	 */
	CTmlValue& operator=(const CTmlValue& v)
	{
		m_param = v.m_param;
		return *this;
	}

	/**
	 * [���] �f�[�^���.
	 *		������w��^
	 *	@param lpszData ������
	 *	@return �����̎Q��
	 */
	CTmlValue& SetString(LPCSTR lpszData)
	{
		m_param.Set(STRLIB::GetLen(lpszData), lpszData);
		return *this;
	}

	/**
	 * [���] �f�[�^���.
	 *		�f�[�^�w��^
	 *	@param v �f�[�^
	 *	@return �����̎Q��
	 */
	CTmlValue& operator=(const CByteVector& v)
	{
		m_param.Set(v.GetSize(), v.ReferBuffer());
		return *this;
	}

	/**
	 * [���] �f�[�^���.
	 *		�f�[�^�w��^
	 *	@param len �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@return �����̎Q��
	 */
	CTmlValue& SetData(size_t len, LPCVOID P)
	{
		m_param.Set(len, P);
		return *this;
	}

	/**
	 * [�ݒ�] ���C���h�J�[�h�|�C���g�ǉ�.
	 *	@param index ���C���h�J�[�h�|�C���g�̃C���f�b�N�X.
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool AddWildPoint(INDEX index)
	{
		return HasData() && m_param.dataBytes.SetAttribute(index, WILDCARD);
	}

	/**
	 * [��r] ��r.
	 *		Value �̔�r���s���܂��B�f�[�^�����̏ꍇ�A���C���h�J�[�h���l������܂��B
	 *		���C���h�J�[�h�̕��������o���܂��B
	 *	@param _vvbWild [out]WildCard�������Ƃ���ɂ����� DATA
	 *	@param other ��r����B
	 *	@retval �O		������ other �Ɠ���
	 *	@retval �P�ȏ�	������ other ���傫��
	 *	@retval �O����	������ other ��菬����
	 */
	INT_PTR Compare(ICollectionT<CByteVector>& _vvbWild, const CTmlValue& other) const
	{
		_vvbWild.RemoveAll();
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		if ( kind != DATA )
		{
			return m_CompareNum(m_param, other.m_param);
		}
		const CDataBytes& leftData = m_param.GetData();
		const CDataBytes& rightData = other.m_param.GetData();
		//
		size_t srcPos = 0;
		size_t wildPos = 0;
		size_t srcPosMax = leftData.GetSize();
		size_t wildPosMax = rightData.GetSize();
		BYTE B;
		while ( true )
		{
			if ( wildPos >= wildPosMax )
			{
				return 1;
			}
			if ( rightData.GetAttribute(wildPos) == WILDCARD )
			{
				//���C���h�����I
				wildPos++;
				while ( wildPos < wildPosMax )
				{
					if ( rightData.GetAttribute(wildPos) != WILDCARD )
					{
						break;
					}
					wildPos++;
				}
				CByteVector vb;
				if ( wildPos >= wildPosMax )
				{
					//���C���h���Ō�B
					while ( true )
					{
						if ( srcPos >= srcPosMax )
						{
							_vvbWild.Add(vb);
							break;
						}
						B = leftData.Get(srcPos);
						vb.Add(B);
						srcPos++;
					}
					return 0;
				}
				while ( true )
				{
					B = leftData.Get(srcPos);
					if ( B == rightData.Get(wildPos) )
					{
						//���C���h�̎��̕����𔭌�
						_vvbWild.Add(vb);
						break;
					}
					vb.Add(B);
					srcPos++;
					if ( srcPos >= srcPosMax )
					{
						return -1;
					}
				}
			}
			if ( srcPos >= srcPosMax )
			{
				return 1;
			}
			INT_PTR r = leftData.Get(srcPos);
			r -= rightData.Get(wildPos);
			if ( r != 0 )
			{
				return r;
			}
			srcPos++;
			wildPos++;
			if ( wildPos >= wildPosMax && srcPos >= srcPosMax )
			{
				break;
			}
		}
		return 0;
	}

	/**
	 * [�m�F] ��r.
	 *	@param other ��r�Ώ�
	 *	@retval �O		������ other �Ɠ���
	 *	@retval �P�ȏ�	������ other ���傫��
	 *	@retval �O����	������ other ��菬����
	 */
	virtual INT_PTR Compare(const CTmlValue& other) const
	{
		CDummyCollectionT<CByteVector> vv;
		return Compare(vv, other);
	}
 
	/**
	 * [�v�Z] ���Z.
	 *	@param other ��������
	 *	@return ����������
	 */
	CTmlValue operator+(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() + other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() + other.m_param.GetFloat();
			break;
		case DATA:
			v.m_param.Set(m_param.GetData() + other.m_param.GetData());
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}

	/**
	 * [�v�Z] �����Z.
	 *	@param other ��������
	 *	@return ����������
	 */
	CTmlValue operator-(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() - other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() - other.m_param.GetFloat();
			break;
		case DATA:
			v = *this;
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}
 
	/**
	 * [�v�Z] �|���Z.
	 *	@note DATA �� INTEGER ���|����ƁADATA�� INTEGER���A�J��Ԃ����f�[�^�ɂȂ�܂��B
	 *	@param other �|���鑊��
	 *	@return �|��������
	 */
	CTmlValue operator*(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() * other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() * other.m_param.GetFloat();
			break;
		case DATA:
			v = *this;
			if ( m_param.eKind == DATA )
			{
				v.m_param.dataBytes.RemoveAll();
				loop ( i, other.m_param.GetInteger() )
				{
					v.m_param.dataBytes += m_param.dataBytes;
				}
			}
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}
 
	/**
	 * [�v�Z] ����Z.
	 *	@param other ���鑊��
	 *	@return ��������
	 */
	CTmlValue operator/(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() / other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() / other.m_param.GetFloat();
			break;
		case DATA:
			v = *this;
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}
 
	/**
	 * [�擾] WildCard���擾.
	 *	@return ���C���h�J�[�h��Index���i�[
	 */
	CVectorT<INDEX> GetWildCardPositions(void) const
	{
		CVectorT<INDEX> vid;
		if ( HasData() )
		{
			loop ( i, m_param.dataBytes.GetSize() )
			{
				if ( m_param.dataBytes.GetAttribute(i) == WILDCARD )
				{
					vid.Add(i);
				}
			}
		}
		return vid;
	}

	/**
	 * [�m�F] �f�[�^�^�ێ��m�F.
	 *	@retval true �f�[�^�ێ�
	 *	@retval false ����ȊO
	 */
	bool HasData(void) const
	{
		return m_param.eKind == DATA;
	}

	/**
	 * [�m�F] �ێ��^�m�F.
	 *		�ێ����Ă���f�[�^�̌^��Ԃ��܂��B
	 *	@return �^�B
	 */
	EKind Getkind(void) const
	{
		return m_param.eKind;
	}

	/**
	 * [�擾] �����擾.
	 *		�ێ����Ă���̂��f�[�^�Ȃ琔�l�ɕϊ����ĕԂ��܂��B
	 *	@return �l
	 */
	int GetInteger(void) const
	{
		return HasData() ? atoi(m_param.GetString()) : m_param.GetInteger();
	}

	/**
	 * [�擾] �����擾.
	 *		�ێ����Ă���̂��f�[�^�Ȃ琔�l�ɕϊ����ĕԂ��܂��B
	 *	@return �l
	 */
	double GetFloat(void) const
	{
		return HasData() ? atof(m_param.GetString()) : m_param.GetFloat();
	}

	/**
	 * [�擾] ������擾.
	 *		�f�[�^�𕶎���Ƃ��ĕԂ��܂��B�f�[�^�r����NIL������ƕ�����͂����ŏI���܂��B
	 *		�ێ����Ă���̂����l�^�Ȃ�A������ɕϊ����ĕԂ�܂��B
	 *	@return ������B
	 */
	CAscii GetString(void) const
	{
		return m_param.GetString();
	}

	/**
	 * [�擾] �f�[�^�擾.
	 *		�ێ����Ă���̂����l�^�Ȃ�A�f�[�^�ɕϊ����ĕԂ�܂��B
	 *	@return �f�[�^
	 */
	CByteVector GetData(void) const
	{
//		return m_param.GetData().Refer();
		return m_param.GetData();
	}

	/**
	 * [�擾] �f�[�^���擾.
	 *		�ێ����Ă���̂����l�^�Ȃ�A�f�[�^�ɕϊ����ĕԂ�܂��B
	 *	@return �l
	 */
	size_t GetDataSize(void) const
	{
		return m_param.GetData().GetSize();
	}

private:

	///�o�C�i���̑���
	enum EAttr
	{
		NORMAL = 0,		///< �m�[�}��
		WILDCARD = 1,	///< ���C���h�J�[�h
	};

	/// �f�[�^�^�錾
	typedef CAttributedVectorT<BYTE, BYTE> CDataBytes;

	/// �p�����[�^
	struct TParam
	{
		EKind		eKind;			///< �ێ����Ă�����̎��
		CDataBytes	dataBytes;		///< �ێ����Ă���o�C�i���f�[�^
		int			iValue;			///< �ێ����Ă��鐮��
		double		doValue;		///< �ێ����Ă������
		/// �R���X�g���N�^
		TParam(void) : iValue(0), doValue(0.0), eKind(EMPTY)
		{
		}
		/// �R�s�[�R���X�g���N�^
		TParam(const TParam& p) : eKind(p.eKind), dataBytes(p.dataBytes), iValue(p.iValue), doValue(p.doValue)
		{
		}
		/// �R�s�[�I�y���[�^
		TParam& operator=(const TParam& p)
		{
			eKind = p.eKind;
			dataBytes = p.dataBytes;
			iValue = p.iValue;
			doValue = p.doValue;
			return *this;
		}
		/// �Z�b�^�[
		void Set(EKind e, int i = 0, double d = 0.0, const CDataBytes* D = NULL)
		{
			eKind = e;
			if ( D == NULL )
			{
				dataBytes.RemoveAll();
			}
			else
			{
				dataBytes = *D;
			}
			iValue = i;
			doValue = d;
		}
		/// �N���A
		void Empty(void) 
		{
			Set(EMPTY); 
		}
		/// �Z�b�g
		void Set(int i)
		{
			Set(INTEGER, i, i);
		}
		/// �Z�b�g
		void Set(double i) 
		{
			Set(FLOAT, ToInt(i), i); 
		}
		/// �Z�b�g
		void Set(const CDataBytes& d)
		{
			Set(DATA, 0, 0.0, &d); 
		}
		/// �Z�b�g
		void Set(size_t l, LPCVOID P)
		{
			CDataBytes d;
			d.SetElements(l, static_cast<const BYTE*>(P));
			Set(d);
		}
		/// INT�擾
		int GetInteger(void) const
		{
			return iValue;
		}
		/// FLOAT�擾
		double GetFloat(void) const
		{
			return (eKind <= INTEGER) ? iValue : doValue;
		}
		/// ������擾
		CAscii GetString(void) const
		{
			CAscii a;
			switch ( eKind )
			{
			case INTEGER:
				a.Format("%d", iValue);
				break;
			case FLOAT:
				a.Format("%.0f", doValue);
				break; 
			case DATA:
				{
					size_t l = dataBytes.GetSize();
					if ( l > 0 )
					{
						LPSTR P = a.GetBuffer(l);
						dataBytes.GetElements(l, reinterpret_cast<BYTE*>(P));
						P[l] = 0;
						a.ReleaseBuffer();
					}
				}
				break;
			default:
				break;
			}
			return a;
		}
		/// �f�[�^�擾
		CDataBytes GetData(void) const
		{
			CDataBytes d;
			switch ( eKind )
			{
			case DATA:
				d = dataBytes;
				break;
			case INTEGER:
			case FLOAT:
				{
					CAscii a = GetString();
					d.SetElements(a.GetLength(), reinterpret_cast<const BYTE*>(LPCSTR(a)));
				}
				break;
			default:
				break;
			}
			return d;
		}
	};

	TParam m_param;	///< �p�����[�^

	/// ������ DATA �ȊO�̏ꍇ�̂ݎg�p�\
	INT_PTR m_CompareNum(const TParam& p1, const TParam& p2) const
	{
		if ( p1.eKind == DATA || p2.eKind == DATA )
		{
			ASSERT(false);
			return 0;
		}
		if ( p1.eKind <= INTEGER && p2.eKind <= INTEGER )
		{
			return IComparatorT<int>::GetDefault().CompareTo(p1.iValue, p2.iValue);
		}
		return IComparatorT<double>::GetDefault().CompareTo(p1.doValue, p2.doValue);
	}

	friend class CTmlValueTest;
};



/**@ingroup TML
 * TinyMacroLang �l�z��^.
 *	
 *	@par�K�v�t�@�C��
 *			TnbTmlValue.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
typedef CVectorT<CTmlValue> CTmlValuesVector;



/**@ingroup TML
 * TinyMacroLang �l�Q�ƊǗ�
 *
 *		�l�̌v�Z�Ɏg�p����鐔�l�N���X�B
 *		TmlValue �̎��̂��Q�Ƃ̂����ꂩ���Ǘ����܂��B
 *		�Q�Ƃ̏ꍇ�A�l��ύX������Q�Ɛ�̒l���ύX����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlValue.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTmlValueCarrier
{
public:

	/// �R���X�g���N�^
	CTmlValueCarrier(void) : m_pValueRef(NULL), m_valueReal(), m_strExName()
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CTmlValueCarrier(const CTmlValueCarrier& other) 
		: m_pValueRef(other.m_pValueRef), m_valueReal(other.m_valueReal), m_strExName(other.m_strExName)
	{
	}

	/**
	 * �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CTmlValueCarrier& operator=(const CTmlValueCarrier& other) 
	{
		m_pValueRef = other.m_pValueRef;
		m_valueReal = other.m_valueReal;
		m_strExName = other.m_strExName;
		return *this;
	}

	/**
	 * [���] int���.
	 *	@note �g���l�[���ƎQ�Ƃ�OFF�ɂȂ�܂�
	 *	@param i �l
	 *	@return �����̎Q��
	 */
	CTmlValueCarrier& operator=(int i)
	{
		m_strExName.Empty();
		m_valueReal = i;
		m_pValueRef = NULL;
		return *this;
	}

	/**
	 * [���] double���.
	 *	@note �g���l�[���ƎQ�Ƃ�OFF�ɂȂ�܂�
	 *	@param i �l
	 *	@return �����̎Q��
	 */
	CTmlValueCarrier& operator=(double i)
	{
		m_strExName.Empty();
		m_valueReal = i;
		m_pValueRef = NULL;
		return *this;
	}

	/**
	 * [���] Value����.
	 *	@note �g���l�[���ƎQ�Ƃ�OFF�ɂȂ�܂�
	 *	@param v �l
	 *	@return �����̎Q��
	 */
	CTmlValueCarrier& operator=(const CTmlValue& v)
	{
		m_strExName.Empty();
		m_valueReal = v;
		m_pValueRef = NULL;
		return *this;
	}

	/**
	 * [���] Value�Q�Ǝw��.
	 *	@note �w���Value�̎Q�Ƃ��L�����܂��B
	 *	@param v �l
	 */
	void SetRef(CTmlValue& v)
	{
		m_pValueRef = &v;
	}

	/**
	 * [���] �g���l�[���w��.
	 *	@param lpsz �g���l�[��
	 */
	void Set(LPCSTR lpsz)
	{
		m_strExName = lpsz;
	}

	/**
	 * [�ݒ�] ��B.
	 *	@note �g���l�[���ƎQ�Ƃ�OFF�ɂȂ�܂�
	 */
	void Empty(void)
	{
		m_strExName.Empty();
		m_valueReal = 0;
		m_pValueRef = NULL;
	}

	/**
	 * [�擾] �g���l�[���擾
	 *	@return �g���l�[��
	 */
	LPCSTR GetExName(void) const
	{
		return m_strExName;
	}

	/**
	 * [�擾] �l�Q��
	 *	@return �l�̎Q�ƁB
	 */
	const CTmlValue& operator()(void) const
	{
		return (m_pValueRef == NULL) ? m_valueReal : *m_pValueRef;
	}

	/**
	 * [�擾] �l�Q��
	 *	@note �ێ����Ă���Value�̎Q�Ƃ�Ԃ��܂��B�Q�ƂȂ̂ŁA���OK�ł�.
	 *	@return �l�̎Q�ƁB
	 */
	CTmlValue& operator()(void)
	{
		return (m_pValueRef == NULL) ? m_valueReal : *m_pValueRef;
	}

private:
	CTmlValue*		m_pValueRef;	///< �{�N���X�����l�̎Q��
	CTmlValue		m_valueReal;	///< �{�N���X�����l�̎���
	CAscii			m_strExName;	///< �g���l�[��
};



}; // TNB

