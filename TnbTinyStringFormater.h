#pragma once
/**
 *@file
 * �ȈՕ����񏑎��֌W�̃w�b�_
 *
 *		�����������������N���X���L��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include <TnbDef.h>



//TNB Library
namespace TNB
{



/**@ingroup STRING 
 * �ȈՕ����񏑎�.
 *
 *		�ǉ�����镶�������z�肳���ꍇ�ŁA���x�����߂���ꍇ�ɁA���ɗL���ȕ�����t�H�[�}�b�^�[�ł��B
 *
 *	@see CTinyStringFormater
 *
 *	@par�K�v�t�@�C��
 *			TnbTinyStringFormater.h
 *
 *	@date 11/08/25	�V�K�쐬 
 *	@date 11/09/27	AddFormatHexInt() , operator LPCSTR() �ǉ��B
 *	@date 12/03/07	�e���v���[�g��
 */
template<typename TYP = TCHAR>
class CTinyStringFormaterT : CCopyImpossible
{
public:

	/// �R���X�g���N�^.
	CTinyStringFormaterT(void) : m_pos(0), m_pWork(NULL), m_workSize(0)
	{
	}

	/**
	 * [�ݒ�] ������.
	 *		�{�C���X�^���X�Ŋi�[���Ă������[�N�������̃A�h���X�ƒ������w�肵�܂��B
	 *	@note �K���͂��߂Ɏ��s���邱�ƁB
	 *	@param _P ��������i�[���Ă������[�N�������̃A�h���X
	 *	@param len _P �̃��[�N�������̒���
	 */
	void Initialize(TYP* _P, size_t len)
	{
		m_pWork = _P;
		m_workSize = len;
		m_pos = 0;
	}

	/**
	 * [�擾] �����񐔎擾.
	 *	@return �ǉ�����������.
	 */
	size_t GetLength(void) const
	{
		return m_pos;
	}

	/**
	 * [�擾] ������擾.
	 *	@return Initialize() �Ŏw�肵�����[�N������.
	 */
	operator const TYP*(void) const
	{
		return m_pWork;
	}

	/**
	 * [�擾] ���[�N�������c��擾.
	 *	@return �c��̃��[�N�������T�C�Y.
	 */
	size_t GetRemainingLength(void) const
	{
		if ( m_workSize < m_pos )
		{
			return 0;
		}
		return m_workSize - m_pos;
	}

	/**
	 * [�ǉ�] �I�[�����ǉ�.
	 *		���̃��\�b�h�ł͏I�[�����͕t������܂���B
	 */
	void AddTerminateChar(void)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + 1 <= m_workSize );
		m_pWork[m_pos] = 0;
	}

	/**
	 * [�ǉ�] int �^�ǉ�.
	 *		�w��� int�^���l�𕶎��񉻂��Ēǉ����܂��B
	 *	@param value int �^���l
	 *	@param width ������.
	 */
	void AddFormatInt(int value, size_t width)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + width <= m_workSize );
		FormatInt(&m_pWork[m_pos], value, width);
		m_pos += width;
	}

	/**
	 * [�ǉ�] int �^�ǉ�.
	 *		�w��� int�^���l�𕶎��񉻂��Ēǉ����܂��B
	 *	@note �ő�11�����ɂȂ�̂ŁA�c��o�b�t�@�� 11�ȏ゠��悤�ɂ��Ă��������B
	 *	@param value int �^���l
	 */
	void AddFormatInt(int value)
	{
		ASSERT( m_pWork != NULL );
		m_pos += FormatInt(&m_pWork[m_pos], value);
	}

	/**
	 * [�ǉ�] DWORD �^ HEX �ǉ�.
	 *		�w��� DWORD �^���l�� HEX �����񉻂��Ēǉ����܂��B
	 *	@param value DWORD �^���l
	 *	@param width ������.
	 */
	void AddFormatHexInt(DWORD value, size_t width)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + width <= m_workSize );
		FormatHex(&m_pWork[m_pos], value, width);
		m_pos += width;
	}

	/**
	 * [�ǉ�] �ꕶ���ǉ�.
	 *	@param c ����.
	 */
	void AddChar(TCHAR c)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + 1 <= m_workSize );
		m_pWork[m_pos] = c;
		m_pos++;
	}

	/**
	 * [�ǉ�] �����񃊃e�����ǉ�.
	 *	@param literal �����񃊃e����.
	 */
	template<typename XX>
	void AddLiteral(const XX& literal)
	{
		const TYP* lpsz = literal;			// �����ŃG���[�o��ꍇ�A�����ɕ����񃊃e�����ȊO��n���Ă���\���L��ł�
		size_t len = sizeof(literal) / sizeof(TYP);
		Add(lpsz, len - 1); //�I�[�͓���Ȃ�
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@note ���������킩���Ă���ꍇ�A �������w��� Add(const TYP*, size_t) ���g�����ق��������ł��B
	 *	@param lpsz ������.
	 */
	void Add(const TYP* lpsz)
	{
		ASSERT( m_pWork != NULL );
		size_t len = STRLIB::GetLen(lpsz);
		ASSERT( m_pos + len <= m_workSize );
		memcpy(&m_pWork[m_pos], lpsz, len * sizeof(TYP));
		m_pos += len;
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@note ���Z�ɂȂ�܂����A�Ⴆ�΁A�ȉ��̂悤�Ɏw�肷�邱�ƂŏI�[�𓯎��ɒǉ��\�ł��B
	 *	\code
	 *		CTinyStringFormater sf;
	 *		sf.Initilaize(...);
	 *		sf.Add("ABC", 4);		// ���������B
	 *	\endcode
	 *	@param lpsz ������.
	 *	@param len �����񒷂�
	 */
	void Add(const TYP* lpsz, size_t len)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + len <= m_workSize );
		memcpy(&m_pWork[m_pos], lpsz, len * sizeof(TYP));
		m_pos += len;
	}


	//-------------------------------


	/**
	 * [�ݒ�] int�^������(�������w��).
	 *		�w��� int �^�̐��l���w��̕������̕�����ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatInt(buf, -100, 6);
	 *		buf[6] = 0; //�I�[
	 *		// buf �� "  -100" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B width �ȏ�̃T�C�Y���K�v.
	 *	@param[in] value int�^���l.
	 *	@param[in] width �ϊ���̕�����
	 */
	static void FormatInt(TYP* _P, int value, size_t width)
	{
		unsigned int v = (value < 0) ? -value : value;
		size_t j = 0;
		loop_dn ( i, width )
		{
			j = i;
			_P[i] = static_cast<TYP>('0' + (v % 10));
			v /= 10;
			if ( v == 0 )
			{
				break;
			}
		}
		if ( value < 0 && j > 0 )
		{
			_P[j - 1] = '-';
			j--;
		}
		if ( j > 0 )
		{
			loop ( i , j )
			{
				_P[i] = ' ';
			}
		}
	}

	/**
	 * [�ݒ�] int�^������.
	 *		�w��� int �^�̐��l�𕶎���ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		int r = CTinyStringFormater::FormatInt(buf, -100);
	 *		buf[r] = 0; //�I�[
	 *		// buf �� "-100" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B �ő�11�����ɂȂ�\��������̂ŁA����ȏ�̃T�C�Y��p��.
	 *	@param[in] value int�^���l.
	 *	@return ����������������
	 */
	static INT_PTR FormatInt(TYP* _P, int value)
	{
		TYP* Q = _P;
		if ( value == 0 )
		{
			*_P++ = '0';
		}
		else
		{
			unsigned int v = (value < 0) ? -value : value;
			if ( value < 0 )
			{
				*_P++ = '-';
			}
			int j = 0;
			TYP buf[32];
			while ( v > 0 )
			{
				buf[j++] = static_cast<TYP>('0' + (v % 10));
				v /= 10;
			}
			loop_dn ( i, j )
			{
				*_P++ = buf[i];
			}
		}
		return _P - Q;
	}

	/**
	 * [�ݒ�] int�^������(�O�p�f�B���O�t��).
	 *		�w��� int �^�̐��l���w��̕������̕�����ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatInt0(buf, -100, 6);
	 *		buf[6] = 0; //�I�[
	 *		// buf �� "-00100" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B width �ȏ�̃T�C�Y���K�v.
	 *	@param[in] value int�^���l.
	 *	@param[in] width �ϊ���̕�����
	 */
	static void FormatInt0(TYP* _P, int value, size_t width)
	{
		unsigned int v = (value < 0) ? -value : value;
		loop_dn ( i, width )
		{
			_P[i] = static_cast<TYP>('0' + (v % 10));
			v /= 10;
		}
		if ( value < 0 )
		{
			_P[0] = '-';
		}
	}

	/**
	 * [�ݒ�] DWORD�^������(�������w��).
	 *		�w��� DWORD �^�̐��l���w��̕������̕�����ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatDword(buf, 100, 6);
	 *		buf[6] = 0; //�I�[
	 *		// buf �� "   100" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B width �ȏ�̃T�C�Y���K�v.
	 *	@param[in] value DWORD�^���l.
	 *	@param[in] width �ϊ���̕�����
	 */
	static void FormatDword(TYP* _P, DWORD value, size_t width)
	{
		loop_dn ( i, width )
		{
			_P[i] = static_cast<TYP>('0' + (v % 10));
			v /= 10;
			if ( v == 0 )
			{
				break;
			}
		}
		if ( j > 0 )
		{
			loop ( i , j )
			{
				_P[i] = ' ';
			}
		}
	}

	/**
	 * [�ݒ�] DWORD�^������.
	 *		�w��� DWORD �^�̐��l�𕶎���ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		int r = CTinyStringFormater::FormatDword(buf, 100);
	 *		buf[r] = 0; //�I�[
	 *		// buf �� "100" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B �ő�11�����ɂȂ�\��������̂ŁA����ȏ�̃T�C�Y��p��.
	 *	@param[in] value int�^���l.
	 *	@return ����������������
	 */
	static INT_PTR FormatDword(TYP* _P, DWORD value)
	{
		TYP* Q = _P;
		if ( value == 0 )
		{
			*_P++ = '0';
		}
		else
		{
			int j = 0;
			TCHAR buf[32];
			while ( value > 0 )
			{
				buf[j++] = static_cast<TYP>('0' + (value % 10));
				value /= 10;
			}
			loop_dn ( i, j )
			{
				*_P++ = buf[i];
			}
		}
		return _P - Q;
	}

	/**
	 * [�ݒ�] DWORD�^������(�O�p�f�B���O�t��).
	 *		�w��� DWORD �^�̐��l���w��̕������̕�����ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatDword0(buf, 100, 6);
	 *		buf[6] = 0; //�I�[
	 *		// buf �� "000100" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B width �ȏ�̃T�C�Y���K�v.
	 *	@param[in] value DWORD�^���l.
	 *	@param[in] width �ϊ���̕�����
	 */
	static void FormatDword0(TYP* _P, DWORD value, size_t width)
	{
		loop_dn ( i, width )
		{
			_P[i] = static_cast<TYP>('0' + (value % 10));
			value /= 10;
		}
	}

	/**
	 * [�ݒ�] DWORD�^ HEX ������(�O�p�f�B���O�t��).
	 *		�w��� DWORD �^�̐��l���w��̕������̕�����ɕϊ����܂��B
	 *		�I�[�͏�����܂���B
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatHex(buf, 100, 4);
	 *		buf[4] = 0; //�I�[
	 *		// buf �� "0064" �ɂȂ�B
	 *	\endcode
	 *	@param[out] _P ������������������i�[����B width �ȏ�̃T�C�Y���K�v.
	 *	@param[in] value DWORD �^���l.
	 *	@param[in] width �ϊ���̕�����
	 */
	static void FormatHex(TYP* _P, DWORD value, size_t width)
	{
		loop_dn ( i, width )
		{
			_P[i] =  STRLIB::IntToHexChar(value & 0xF);
			value >>= 4;
		}
	}

private:
	TYP*			m_pWork;		///< ��ƃA�h���X
	size_t			m_workSize;		///< ��ƃT�C�Y
	INDEX			m_pos;			///< �ʒu
	friend class CTinyStringFormaterTest;
};



/**@ingroup STRING 
 * �ȈՕ����񏑎�.
 *
 *		�ǉ�����镶�������z�肳���ꍇ�ŁA���x�����߂���ꍇ�ɁA���ɗL���ȕ�����t�H�[�}�b�^�[�ł��B
 *
 *		�{�C���X�^���X���̃o�b�t�@�������܂���B�ł��̂ŁA�͂��߂� Initialize() �Ń��[�N���������w�肷��K�v������܂��B
 *		�����I�Ƀ������T�C�Y���g�����邱�Ƃ͂��܂���̂ŁA�����ӂ��������i���̑���A�����ɏ��������邱�Ƃ��o���܂��j�B
 *
 *	\par �g�p��
 *	\code
 *		#define CR "\r"
 *		// [�擾] LOG ������擾.
 *		//	@param[out] _lpsz 'datax, xxxx, xxxx, xxxx, xxxx, xxxx' �� 40 ����
 *		int ToLogString(LPSTR _lpsz, INDEX index)
 *		{
 *			CTinyStringFormater lsf;
 *			lsf.Initialize(_lpsz, 40);	
 *			lsf.AddLiteral("data");		lsf.AddFormatInt(index, 1);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(1, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(2, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(3, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(4, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(5, index), 4);
 *			lsf.Add(CR ,sizeof(CR));	//�I�[���t��
 *			return lsf.GetLength() - 1;
 *		}
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbTinyStringFormater.h
 *
 *	@date 11/08/25	�V�K�쐬 
 *	@date 11/09/27	AddFormatHexInt() , operator LPCSTR() �ǉ��B
 *	@date 12/03/07	�e���v���[�g���B UNICODE �ɂ��Ή��B
 */
class CTinyStringFormater : public CTinyStringFormaterT<>
{
};



}; // TNB

