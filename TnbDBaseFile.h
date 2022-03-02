#pragma once
/**
 *@file
 * dBase�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"
#include "TnbGrid.h"



//TNB Library
namespace TNB
{



#pragma pack(push)
#pragma pack(1)



/**@ingroup DBASE FILE
 * dBase�t�@�C���Ǘ��N���X
 *
 *		1�ȏ�̔C�ӂ̌^�̃t�B�[���h�ƁA�����̃��R�[�h����Ȃ���������܂��B
 *		�܂��A dBase �`���̃t�@�C���̓ǂݏ������T�|�[�g���܂��B
 *
 *	@note dBase �̃f�[�^����T�|�[�g���Ă��܂��B
 *	@note �����t�B�[���h�̓T�|�[�g���Ă��܂���B
 *
 *	@attention UNICODE �v���W�F�N�g�ł́A�S�p�̕������̍l�������قȂ邽�߁A�g�p�ɂ͒��ӂ��K�v�B
 *
 *	@par�K�v�t�@�C��
 *			TnbDBaseFile.h
 *
 *	@date 06/04/20 �V�K�쐬
 *	@date 10/10/07 UNICODE ���̃t�@�C���������݂��b��C���B
 */
class CDBaseFile : public CGrid
{
	DEFSUPER(CGrid);
public:

	/**
	 * [�Ǎ�] DBF�t�@�C���Ǎ���
	 *	@param lpszFileName �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool LoadFile(LPCTSTR lpszFileName)
	{
		_super::AllDeleteRecord();
		CFileReader fr;
		if ( fr.Open(lpszFileName) )
		{
			try
			{
				m_Load(fr);
			}
			catch ( CTnbException& e )
			{
				e.OnCatch();
				_super::AllDeleteRecord();
				return false;
			}
			catch ( ... )
			{
				throw;
			}
			return true;
		}
		return false;
	}

	/**
	 * [���o] DBF�t�@�C�������o��
	 *	@param lpszFileName �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SaveFile(LPCTSTR lpszFileName) const
	{
		CFileWriter fw;
		if ( fw.New(lpszFileName) )
		{
			try
			{
				m_Save(fw);
			}
			catch ( CTnbException& e )
			{
				e.OnCatch();
				return false;
			}
			catch ( ... )
			{
				throw;
			}
			return true;
		}
		return false;
	}

private:
	/**
	 * �t�@�C���w�b�_�^
	 */
	struct THead
	{
		BYTE	version			:3;		///< �o�[�W����
		BYTE	withMemoFile4	:1;		///< dBASE IV�����t�@�C���̗L��
		BYTE	withSqlTable	:3;		///< SQL�e�[�u���̗L��
		BYTE	withMemoFile	:1;		///< �t�@�C���̗L��S
		BYTE	year;					///< �N���� 1900 �Ђ�����
		BYTE	month;					///< ��
		BYTE	day;					///< ��
		DWORD	recordCount;			///< ���R�[�h��
		WORD	headBytes;				///< �w�b�_�̃o�C�g��
		WORD	recordBytes;			///< ���R�[�h�̃o�C�g��
		BYTE	padding1[2];			///< �\��
		BYTE	transaction;			///< �g�����U�N�V���������t���O
		BYTE	encryptionFlag;			///< �Í����t���O
		BYTE	padding2[12];			///< �\��
		BYTE	withMdxFile;			///< MDX�t�@�C���t���O �O�G�Ȃ�
		BYTE	language;				///< ����h���C�oID
		BYTE	padding3[2];			///< �\��
		THead(void)
		{
			Zero(*this);
			version = 3;
		}
		void SetTime(const SYSTEMTIME& st)
		{
			year = static_cast<BYTE>(st.wYear - 1900);
			month = static_cast<BYTE>(st.wMonth);
			day = static_cast<BYTE>(st.wDay);
		}
	};
	C_ASSERT ( sizeof(THead) == 32 ); ///< �T�C�Y�`�F�b�N

	/**
	 * �t�B�[���h�L�q�q�^
	 */
	struct TField
	{
		char	name[11];				///< �t�B�[���h��
		char	type;					///< �^
		BYTE	padding1[4];			///< �\��
		BYTE	fieldLength;			///< �t�B�[���h�T�C�Y
		BYTE	decimalLength;			///< ���A�����_�̒���
		BYTE	padding2[2];			///< �\��
		BYTE	wordId;					///< ��Ɨ̈�ID
		BYTE	padding3[10];			///< �\��
		BYTE	withMdxField;			///< MDX�t�B�[���h�t���O
		TField(void)
		{
			Zero(*this);
		}
		/// �ő�
		void SetName(LPCTSTR lpsz)
		{
			Zero(name);
			CAscii s = lpsz;
			if ( s.GetLength() > 10 )
			{
				s = s.Left(10);
			}
			STRLIB::Copy(name, s);
		}
	};
	C_ASSERT ( sizeof(TField) == 32 ); ///< �T�C�Y�`�F�b�N

	/**
	 * [�Ǎ�] �Ǎ���
	 *	@param r ���[�_
	 *	@throw CNotSupportException �o�[�W�����Ⴂ
	 *	@throw CInvalidParamException �t�B�[���h���ُ�A�}�[�N����
	 */
	void m_Load(const IReader& r)
	{
		//=== �w�b�_�Ǎ���
		THead hd;
		r.ReadExactly(sizeof(THead), &hd);
		if ( hd.version != 3 )
		{
			throw CNotSupportException();
		}
		//=== �t�B�[���h�Ǎ���
		size_t fieldCount = (hd.headBytes - sizeof(THead)) / sizeof(TField); //�t�B�[���h��
		size_t recordBytesTemp = 0;
		loop ( i, fieldCount )
		{
			TField fd;
			r.ReadExactly(sizeof(TField), &fd);
			TFieldInfo fi(CStr(fd.name), fd.fieldLength, fd.decimalLength, static_cast<EFieldType>(fd.type));
			if ( _super::AddField(fi) == INVALID_INDEX )
			{
				throw CAppendFailureException();
			}
			recordBytesTemp += fd.fieldLength;
		}
		BYTE b;
		r.ReadExactly(sizeof(BYTE), &b);
		if ( b != 0x0D )
		{
			throw CInvalidParamException();
		}
		recordBytesTemp++;
		if ( recordBytesTemp != hd.recordBytes )
		{
			TRACE2("�w�b�_�̃��R�[�h�T�C�Y(%d)�Ɗe�t�B�[���h�ɏ�����Ă���T�C�Y�̍��v(%d)���قȂ�܂��B\n", hd.recordBytes, recordBytesTemp);
			throw CInvalidParamException();
		}
		//=== ���R�[�h�Ǎ���
		loop ( i, hd.recordCount )
		{
			CWorkMemT<char> buf(hd.recordBytes);	//���R�[�h�̃o�C�g��
			char* B = buf.Ref();
			r.ReadExactly(hd.recordBytes, B);
			if ( B[0] != 0x20 )
			{
				continue; //sp�ȊO�̓f���[�g���R�[�h
			}
			B++;
			INDEX rec = _super::AddRecord();
			ASSERT( rec != INVALID_INDEX );
			loop ( i, fieldCount )
			{
				TFieldInfo fi;
				EFieldKind k = _super::GetFieldInfo(fi, i);
				ASSERT( k != EK_Invalid );
				CAscii a;
				a.SetFromLeft(B, fi.width);
				if ( ! _super::SetStringRecord(rec, i, CStr(a)) )
				{
					throw CInvalidParamException();
				}
				B += fi.width;
			}
		}
		//=== �I�[
		r.ReadExactly(sizeof(BYTE), &b);
		if ( b != 0x1A )
		{
			throw CInvalidParamException();
		}
		_super::Dump();
	}

	/**
	 * [���o] �����o��
	 *	@param[in,out] _w ���C�^
	 *	@throw CNotSupportException �o�[�W�����Ⴂ
	 *	@throw CInvalidParamException �t�B�[���h���ُ�A�}�[�N����
	 */
	void m_Save(IWriter& _w) const
	{
		//=== �w�b�_�����o��
		THead hd;
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			hd.SetTime(st);
			size_t rb = 0;
			loop ( i, _super::GetFieldCount() )
			{
				TFieldInfo fi;
				_super::GetFieldInfo(fi, i);
				rb += fi.width;
			}
			hd.recordCount = ToDword(_super::GetRecordCount());
			hd.recordBytes = ToWord(rb + 1);
			hd.headBytes = ToWord(_super::GetFieldCount() * sizeof(TField) + sizeof(THead) + 1);
		}
		_w.Write(sizeof(THead), &hd);
		//=== �t�B�[���h�����o��
		CWorkMemT<size_t> widths(_super::GetFieldCount());
		loop ( i, _super::GetFieldCount() )
		{
			TFieldInfo fi;
			if ( _super::GetFieldInfo(fi, i) == EK_Invalid )
			{
				throw CInvalidParamException();
			}
			widths[i] = fi.width;
			TField fd;
			fd.SetName(fi.name);
			fd.fieldLength = static_cast<BYTE>(fi.width);
			fd.decimalLength = static_cast<BYTE>(fi.decimals);
			fd.type = static_cast<char>(fi.type);
			_w.Write(sizeof(TField), &fd);
		}
		BYTE b = 0x0D;
		_w.Write(sizeof(BYTE), &b);
		//=== ���R�[�h�����o��
		loop ( i, GetRecordCount() )
		{
			CWorkMemT<char> buf(hd.recordBytes);	//���R�[�h�̃o�C�g��
			char* B = buf.Ref();
			*B++ = 0x20;
			INDEX rec = i;
			size_t len = 1;
			loop ( j, _super::GetFieldCount() )
			{
				size_t l = widths[j];
				CAscii a = _super::GetStringRecord(rec, j, true) + CStr::Lineup(' ', l);
				a = a.Left(l);
				TNB::MemCopy(B, a, l);
				B += l;
				len += l;
			}
			ASSERT ( len == hd.recordBytes );
			_w.Write(hd.recordBytes, buf.Ref());
		}
		//=== �I�[
		b = 0x1A;
		_w.Write(sizeof(BYTE), &b);
	}

	friend class CDBaseFileTest;
};



#pragma pack(pop)



}; // TNB


