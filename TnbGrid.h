#pragma once
/**
 *@file
 * �O���b�h���Ǘ��֌W�̃w�b�_
 *
 *		�i�q��ɏ����Ǘ����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup DBASE
 * �O���b�h�Ǘ��N���X.
 *
 *		1�ȏ�̔C�ӂ̌^�̃t�B�[���h�ƁA�����̃��R�[�h����Ȃ���������܂��B
 *
 *	@note dBase �̃f�[�^����T�|�[�g���Ă��܂��B
 *	@note �����t�B�[���h�̓T�|�[�g���Ă��܂���B
 *
 */
class CGrid
{
public:
	
	/// �t�B�[���h���
	enum EFieldKind
	{
		EK_String,		///< ������
		EK_Integer,		///< ���l
		EK_Float,		///< ����
		EK_Date,		///< ���t
		EK_Invalid,		///< �s��
	};

	/// �t�B�[���h�^�C�v
	enum EFieldType
	{
		ET_Character	= 'C',		///< ������
		ET_Date			= 'D',		///< �N����
		ET_Numeric		= 'N',		///< ���l
		ET_Floating		= 'F',		///< ���������_���l
		ET_Logical		= 'L',		///< �_��(?,Y,N,T,F)
//		ET_Memo			= 'M',		///< ����
	};

	/// �t�B�[���h���
	struct TFieldInfo
	{
		CStr		name;		///< ���O
		size_t		width;		///< ��
		size_t		decimals;	///< �����_����
		EFieldType	type;		///< �^�C�v
		/// �R���X�g���N�^
		TFieldInfo(LPCTSTR lpsz = NULL, size_t w = 0, size_t d = 0, EFieldType t = ET_Character)
			: name(lpsz), width(w), decimals(d), type(t)
		{
		}
		/// �f�V���A���C�Y
		virtual void Deserialize (const IDeserializer& ds)
		{
			ds >> name >> width >> decimals >> type;
		}
		/// �V���A���C�Y
		virtual void Serialize (ISerializer& _sr) const
		{
			_sr << name << width << decimals << type;
		}
	};

	/// ���t���
	struct TDate
	{
		WORD year;		///< �N
		BYTE month;		///< ��
		BYTE day;		///< ��
		/**
		 * �R���X�g���N�^
		 *	@param y �N
		 *	@param m ��
		 *	@param d ��
		 */
		TDate(WORD y = 2000, BYTE m = 1, BYTE d = 1) : year(y), month(m), day(d)
		{
		}
		/**
		 * [�擾] INT�^�擾
		 *	@return INT
		 */
		int Get(void) const
		{
			return year * 10000 + month * 100 + day;
		}
		/**
		 * [�ݒ�] INT�^�ݒ�
		 *	@param i INT
		 */
		void Set(int i)
		{
			year = down_cast<WORD>(i / 10000);
			month = down_cast<BYTE>((i / 100) % 100);
			day = down_cast<BYTE>(i % 100);
		}
	};


	//---------------------------


	/// �R���X�g���N�^
	CGrid(void)
	{
	}

	/**
	 * [�폜] �S�폜.
	 *	@note ���R�[�h���S�폜���܂��B
	 */
	void AllDelete(void)
	{
		m_records.RemoveAll();
		m_fields.RemoveAll();
	}

	/**
	 * [�擾] �t�B�[���h���擾
	 *	@return �t�B�[���h��
	 */
	size_t GetFieldCount(void) const
	{
		return m_fields.GetSize();
	}

	/**
	 * [�擾] ���R�[�h���擾
	 *	@return ���R�[�h��
	 */
	size_t GetRecordCount(void) const
	{
		return m_records.GetSize();
	}

	/**
	 * [����] �t�B�[���h������
	 *	@param lpszFieldName �t�B�[���h��
	 *	@retval INVALID_INDEX�ȊO �����B�l�̓t�B�[���h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX ������Ȃ�
	 */
	INDEX FindFieldName(LPCTSTR lpszFieldName) const
	{
		loop ( i, m_fields.GetSize() )
		{
			if ( m_fields[i].name.IsEqual(lpszFieldName) )
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [�擾] �t�B�[���h���擾
	 *	@param[out] _fi �t�B�[���h���
	 *	@param[in] field �t�B�[�h�C���f�b�N�X
	 *	@retval EK_Invalid�ȊO �擾�����B�l�͎�ށB
	 *	@retval EK_Invalid ���s�i�C���f�b�N�X���͈͊O�j�B
	 */
	EFieldKind GetFieldInfo(TFieldInfo& _fi, INDEX field) const
	{
		if ( m_fields.IsInRange(field) )
		{
			_fi = m_fields[field];
			return m_ToKind(_fi);
		}
		return EK_Invalid;
	}

	/**
	 * [�ǉ�] �t�B�[���h�ǉ�.
	 *	@note �t�B�[���h���������̏ꍇ�A���s���܂��B
	 *	@note ���R�[�h������ꍇ�A���s���܂��B
	 *	@param fi �t�B�[���h�ǉ�
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ������t�B�[���h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX �t�B�[���h�ǉ����s
	 */
	INDEX AddField(const TFieldInfo& fi)
	{
		if ( GetRecordCount() == 0 && ! fi.name.IsEmpty() && FindFieldName(fi.name) == INVALID_INDEX && fi.width >= 1 && fi.width <= 254 )
		{
			return m_fields.Add(fi);
		}
		return INVALID_INDEX;
	}

	/**
	 * [�ǉ�] �����t�B�[���h�ǉ�.
	 *	@note �t�B�[���h���������̏ꍇ�A���s���܂��B
	 *	@note ���R�[�h������ꍇ�A���s���܂��B
	 *	@param lpszFieldName �t�B�[���h��
	 *	@param width �t�B�[���h��
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ������t�B�[���h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX �t�B�[���h�ǉ����s
	 */
	INDEX AddCharacterField(LPCTSTR lpszFieldName, size_t width)
	{
		TFieldInfo fi(lpszFieldName, width, 0, ET_Character);
		return AddField(fi);
	}

	/**
	 * [�ǉ�] ���l�t�B�[���h�ǉ�.
	 *	@note �t�B�[���h���������̏ꍇ�A���s���܂��B
	 *	@note ���R�[�h������ꍇ�A���s���܂��B
	 *�@@note �����`���ŊǗ����܂��B���̂��߁A���l�͈̔͂ɂ��t�B�[���h���A�����_�������l������K�v������܂��B
	 *	@param lpszFieldName �t�B�[���h��
	 *	@param width �t�B�[���h��
	 *	@param dec �����_�����B�ȗ�����ƂO�ɂȂ�܂��B
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ������t�B�[���h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX �t�B�[���h�ǉ����s
	 */
	INDEX AddNumericField(LPCTSTR lpszFieldName, size_t width, size_t dec = 0)
	{
		TFieldInfo fi(lpszFieldName, width, dec, ET_Numeric);
		if ( width >= dec + 1 )
		{
			return AddField(fi);
		}
		return INVALID_INDEX;
	}

	/**
	 * [�ǉ�] ���l�t�B�[���h�ǉ�.
	 *	@note �t�B�[���h���������̏ꍇ�A���s���܂��B
	 *	@note ���R�[�h������ꍇ�A���s���܂��B
	 *�@@note �w���`���ŊǗ����܂��B
	 *	@param lpszFieldName �t�B�[���h��
	 *	@param width �t�B�[���h��
	 *	@param dec �����_�����B�ȗ�����ƂO�ɂȂ�܂��B
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ������t�B�[���h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX �t�B�[���h�ǉ����s
	 */
	INDEX AddFloatingField(LPCTSTR lpszFieldName, size_t width, size_t dec = 0)
	{
		TFieldInfo fi(lpszFieldName, width, dec, ET_Floating);
		if ( width >= dec + 1 )
		{
			return AddField(fi);
		}
		return INVALID_INDEX;
	}

	/**
	 * [�ǉ�] ���t�t�B�[���h�ǉ�.
	 *	@note �t�B�[���h���������̏ꍇ�A���s���܂��B
	 *	@note ���R�[�h������ꍇ�A���s���܂��B
	 *	@param lpszFieldName �t�B�[���h��
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ������t�B�[���h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX �t�B�[���h�ǉ����s
	 */
	INDEX AddDateField(LPCTSTR lpszFieldName)
	{
		TFieldInfo fi(lpszFieldName, 8, 0, ET_Date);
		return AddField(fi);
	}

	/**
	 * [�폜] �t�B�[���h�폜.
	 *	@note ���R�[�h������ꍇ�A���s���܂��B
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool DeleteField(INDEX field)
	{
		if ( GetRecordCount() == 0 )
		{
			return m_fields.Remove(field);
		}
		return false;
	}

	/**
	 * [�폜] ���R�[�h�S�폜.
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool AllDeleteRecord(void)
	{
		return m_records.RemoveAll();
	}

	/**
	 * [�ǉ�] ���R�[�h�ǉ�.
	 *	@note �ǉ��������R�[�h�̑S�t�B�[���h��NULL�ɂȂ��Ă��܂��B
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ��������R�[�h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX ���R�[�h�ǉ����s
	 */
	INDEX AddRecord(void)
	{
		size_t len = GetFieldCount();
		if ( len != 0 )
		{
			CDataVector dv;
			dv.SetSize(len);
			return m_records.Add(dv);
		}
		return INVALID_INDEX;
	}

	/**
	 * [�}��] ���R�[�h�}��.
	 *	@note �}���ǉ��������R�[�h�̑S�t�B�[���h��NULL�ɂȂ��Ă��܂��B
	 *	@param record ���R�[�h�C���f�b�N�X�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool InsertRecord(INDEX record)
	{
		size_t len = GetFieldCount();
		if ( len != 0 )
		{
			CDataVector dv;
			dv.SetSize(len);
			return m_records.Insert(record, dv);
		}
		return false;
	}

	/**
	 * [�폜] ���R�[�h�폜.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool DeleteRecord(INDEX record)
	{
		return m_records.Remove(record);
	}

	/**
	 * [�ݒ�] ���R�[�hNULL�ݒ�.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetNullRecord(INDEX record, INDEX field)
	{
		TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			D->SetString(_T(""));
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] ���R�[�h������ݒ�.
	 *	@note ������̃t�B�[���h�ȊO�ɂ��ݒ�\�ł��B
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@param lpszString ������
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetStringRecord(INDEX record, INDEX field, LPCTSTR lpszString)
	{
		TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			D->SetString(lpszString);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] ���R�[�h���l�ݒ�.
	 *	@note ���l�̃t�B�[���h�ȊO�ɂ͐ݒ�ł��܂���B
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@param value �l
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetIntegerRecord(INDEX record, INDEX field, int value)
	{
		if ( m_ToKind(field) == EK_Integer )
		{
			TData* D = m_Ref(record, field);
			if ( D != NULL )
			{
				D->SetFloat(value);
				return true;
			}
		}
		return false;
	}

	/**
	 * [�ݒ�] ���R�[�h�����ݒ�.
	 *	@note �����̃t�B�[���h�ȊO�ɂ͐ݒ�ł��܂���B
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@param value �l
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetFloatRecord(INDEX record, INDEX field, double value)
	{
		if ( m_ToKind(field) == EK_Float )
		{
			TData* D = m_Ref(record, field);
			if ( D != NULL )
			{
				D->SetFloat(value);
				return true;
			}
		}
		return false;
	}

	/**
	 * [�ݒ�] ���R�[�h���t�ݒ�.
	 *	@note ���t�̃t�B�[���h�ȊO�ɂ͐ݒ�ł��܂���B
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@param date ���t
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetDateRecord(INDEX record, INDEX field, const TDate& date)
	{
		if ( m_ToKind(field) == EK_Date )
		{
			TData* D = m_Ref(record, field);
			if ( D != NULL )
			{
				D->SetFloat(date.Get());
				return true;
			}
		}
		return false;
	}

	/**
	 * [�m�F] ���R�[�hNULL�m�F
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@retval true NULL�B�t�B�[���h�C���f�b�N�X�A���R�[�h�C���f�b�N�X�������̏ꍇ�� true ���Ԃ�܂��B
	 *	@retval false ���s
	 */
	bool IsNullRecord(INDEX record, INDEX field) const
	{
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			return (D->hasString && D->string.IsEmpty());
		}
		return true;
	}

	/**
	 * [�擾] ���R�[�h������擾.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@param isAdjust true �Ȃ�t�B�[���h�̕��ɂȂ�悤�ɃX�y�[�X�Ńp�f�B���O�A�������܂��B
	 *	@return ������
	 */
	CStr GetStringRecord(INDEX record, INDEX field, bool isAdjust = false) const
	{
		CStr str;
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			const TFieldInfo& F = m_fields[field];
			EFieldKind k = m_ToKind(F);
			if ( D->hasString )
			{
				// �f�[�^��������Ȃ̂ł��̂܂�
				str = D->string;
			}
			else if ( k == EK_Float )
			{
				ASSERT( F.decimals > 0 );
				if ( F.type == ET_Floating )
				{
					CStr f;
					f.Format(_T("%%.%de"), F.decimals);
					str.Format(f, D->value);
				}
				else if ( F.type == ET_Numeric )
				{
					CStr f;
					f.Format(_T("%%.%df"), F.decimals);
					str.Format(f, D->value);
				}
				else
				{
					ASSERTLIB( false );
				}
			}
			else
			{
				int v = ToInt(D->value);
				str.Format(_T("%d"), v);
			}
			if ( isAdjust )
			{
				size_t l = str.GetLength();
				if ( l < F.width )
				{
					CStr sp = CStr::Lineup(' ', F.width - l);
					if ( k == EK_String || k == EK_Date )
					{
						str += sp;
					}
					else
					{
						str = sp + str;
					}
				}
				if ( str.GetLength() > F.width )
				{
					if ( k == EK_String || k == EK_Date )
					{
						str = str.Left(F.width);
					}
					else
					{
						str = str.Right(F.width);
					}
				}
			}
		}
		return str;
	}

	/**
	 * [�擾] ���R�[�h���l�擾.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@return ���l
	 */
	int GetIntegerRecord(INDEX record, INDEX field) const
	{
		int r = 0;
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			if ( D->hasString )
			{
				r = D->string.ToInt();
			}
			else
			{
				r = ToInt(D->value);
			}
		}
		return r;
	}

	/**
	 * [�擾] ���R�[�h�����擾.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@return ����
	 */
	double GetFloatRecord(INDEX record, INDEX field) const
	{
		double r = 0;
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			if ( D->hasString )
			{
				r = D->string.ToDouble();
			}
			else
			{
				r = D->value;
			}
		}
		return r;
	}

	/**
	 * [�擾] ���R�[�h���t�擾.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param field �t�B�[���h�C���f�b�N�X
	 *	@return ���t
	 */
	TDate GetDateRecord(INDEX record, INDEX field) const
	{
		TDate d;
		d.Set(GetIntegerRecord(record, field));
		return d;
	}

protected:
	/// Trace�_���v
	void Dump(void)
	{
	#ifdef _DEBUG
		TFieldInfo fi;
		size_t fc = GetFieldCount();
		CWorkMemT<size_t> wd(fc); 
		// �t�B�[���h
		TRACE0("    |");
		loop ( i, fc )
		{
			GetFieldInfo(fi, i);
			wd[i] = max(fi.name.GetLength(), fi.width);
			TRACE1("%s", CStr::Fmt(CStr::Fmt(_T("%%-%ds|"), wd[i]), fi.name));
		}
		TRACE0("\n");
		::Sleep(10);
		// �t�B�[���h(�^)
		TRACE0("    |");
		loop ( i, fc )
		{
			GetFieldInfo(fi, i);
			CStr s = CStr::Fmt(_T("%C,%d,%d"), fi.type, fi.width, fi.decimals);
			TRACE1("%s", CStr::Fmt(CStr::Fmt(_T("%%-%ds|"), wd[i]), s));
		}
		TRACE0("\n");
		::Sleep(10);
		// �Z�p���[�^
		TRACE0("----+");
		loop ( i, fc )
		{
			TRACE1("%s", CStr::Lineup('-', wd[i]) + "+");
		}
		TRACE0("\n");
		::Sleep(10);
		// ���R�[�h
		loop ( j, GetRecordCount() )
		{
			TRACE1("%4d|", j + 1);
			loop ( i, fc )
			{
				TRACE1("%s", CStr::Fmt(CStr::Fmt(_T("%%-%ds|"), wd[i]), GetStringRecord(j, i, true)));
			}
			TRACE0("\n");
			::Sleep(10);
		}
	#endif // _DEBUG
	}

private:
	/// �t�B�[���h��񂩂��ނ�
	EFieldKind m_ToKind(const TFieldInfo& info) const
	{
		switch ( info.type )
		{
		case ET_Character:	// ������
		case ET_Logical:	// �_��(?,Y,N,T,F)
//		case ET_Memo:		// ����
			return EK_String;
		case ET_Date:		// �N����
			return EK_Date;
		case ET_Numeric:	// ���l
		case ET_Floating:
			if ( info.decimals == 0 )
			{
				return EK_Integer;
			}
			return EK_Float;
		default:
			break;
		}
		return EK_String;
//		return EK_Invalid;
	}
	// �t�B�[���h�C���f�b�N�X�����ނ�
	EFieldKind m_ToKind(INDEX field) const
	{
		if ( m_fields.IsInRange(field) )
		{
			return m_ToKind(m_fields[field]);
		}
		return EK_Invalid;
	}
	/// �f�[�^�^
	struct TData
	{
		bool	hasString;	///< true�Ȃ當���� false�Ȃ琔�l
		CStr	string;		///< ������
		double	value;		///< ���l
		/// �R���X�g���N�^
		TData(void) : hasString(true)
		{
		}
		/// ������ݒ�
		void SetString(LPCTSTR lpsz)
		{
			hasString = true;
			string = lpsz;
		}
		/// ���l
		void SetFloat(double v)
		{
			hasString = false;
			value = v;
		}
		virtual void Deserialize (const IDeserializer& ds)
		{
			ds >> hasString >> string >> value;
		}
		virtual void Serialize (ISerializer& _sr) const
		{
			_sr << hasString << string << value;
		}
	};
	/// �f�[�^�Q��
	const TData* m_Ref(INDEX record, INDEX field) const
	{
		if ( m_fields.IsInRange(field) && m_records.IsInRange(record) )
		{
			const CDataVector& v = m_records[record];
			ASSERT( v.IsInRange(field) );
			return &v[field];
		}
		return NULL;
	}
	/// �f�[�^�Q��
	TData* m_Ref(INDEX record, INDEX field)
	{
		if ( m_fields.IsInRange(field) && m_records.IsInRange(record) )
		{
			CDataVector& v = m_records[record];
			ASSERT( v.IsInRange(field) );
			return &v[field];
		}
		return NULL;
	}

	CVectorT<TFieldInfo>	m_fields;		///< �t�B�[���h���
	typedef CVectorT<TData> CDataVector;	///< �ꃌ�R�[�h�^
	CVectorT<CDataVector>	m_records;		///< �S���R�[�h���
	friend class CGridTest;
};



}; // TNB

