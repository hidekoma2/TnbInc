#pragma once
/**
 *@file
 * Grid�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"
#include "TnbGrid.h"



//TNB Library
namespace TNB
{



/**@ingroup DBASE FILE
 * Grid�t�@�C���Ǘ��N���X
 *
 *		1�ȏ�̔C�ӂ̌^�̃t�B�[���h�ƁA�����̃��R�[�h����Ȃ���������܂��B
 *		�܂��A CSV�`���̃t�@�C���̓ǂݏ������T�|�[�g���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbGridFile.h
 *
 *	@date 09/04/16 �V�K�쐬
 */
class CGridFile : public CGrid
{
	DEFSUPER(CGrid);
public:

	/**
	 * [�Ǎ�] CSV�t�@�C���Ǎ���.
	 *	@note ���R�[�h�͓ǂݍ��݂܂���B�ǂݍ��ޑO�ɐݒ肵�Ă����K�v������܂��B
	 *	@param lpszFileName �t�@�C����
	 *	@param hasHeadLine true �Ȃ�w�b�_������Ƃ��A�ǂݍ��ݑΏۂɂ��܂���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool LoadCsvFile(LPCTSTR lpszFileName, bool hasHeadLine = true)
	{
		_super::AllDeleteRecord();
		CFileReader fr;
		if ( fr.Open(lpszFileName) )
		{
			try
			{
				m_Load(fr, hasHeadLine);
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
	 * [���o] CSV�t�@�C�������o��
	 *	@param lpszFileName �t�@�C����
	 *	@param hasHeadLine true �Ȃ��s�ڂɃw�b�_�������o���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SaveCsvFile(LPCTSTR lpszFileName, bool hasHeadLine = true) const
	{
		CFileWriter fw;
		if ( fw.New(lpszFileName) )
		{
			try
			{
				m_Save(fw, hasHeadLine);
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
	 * [�Ǎ�] �Ǎ���
	 *	@param r ���[�_
	 *	@throw CNotSupportException �o�[�W�����Ⴂ
	 *	@throw CInvalidParamException �t�B�[���h���ُ�A�}�[�N����
	 */
	void m_Load(const IReader& r, bool hasHeadLine)
	{
		CByteVector vb = r.ReadExactly();
		vb.Add(0); //�I�[
		CVectorT<CAscii> vaa = CAsciiOperator::SeparateLine(reinterpret_cast<LPCSTR>(vb.ReferBuffer()));
		if ( hasHeadLine )
		{
			vaa.Remove(0); //��s�ڂ��폜
		}
		//=== ���R�[�h�Ǎ���
		while ( ! vaa.IsEmpty() )
		{
			CVectorT<CAscii> va = CAsciiOperator::SeparatePeriod(vaa[0], ',');
			if ( ! va.IsEmpty() )
			{
				INDEX rec = _super::AddRecord();
				ASSERT( rec != INVALID_INDEX );
				loop ( i, min(va.GetSize(), _super::GetFieldCount()) )
				{
					TFieldInfo fi;
					EFieldKind k = _super::GetFieldInfo(fi, i);
					ASSERT( k != EK_Invalid );
					CAscii a;
					a.SetFromLeft(va[i], fi.width);
					if ( ! _super::SetStringRecord(rec, i, CStr(a)) )
					{
						throw CInvalidParamException();
					}
				}
			}
			vaa.Remove(0);
		}
		_super::Dump();
	}

	/**
	 * [���o] �����o��
	 *	@param[in,out] _w ���C�^
	 *	@throw CNotSupportException �o�[�W�����Ⴂ
	 *	@throw CInvalidParamException �t�B�[���h���ُ�A�}�[�N����
	 */
	void m_Save(IWriter& _w, bool hasHeadLine) const
	{
		CAscii s;
		BYTE CR = 0x0D;
		//=== �w�b�_�����o��
		if ( hasHeadLine )
		{
			loop ( i, _super::GetFieldCount() )
			{
				if ( i != 0 )
				{
					s += ",";
				}
				TFieldInfo fi;
				if ( _super::GetFieldInfo(fi, i) == EK_Invalid )
				{
					throw CInvalidParamException();
				}
				s += CAscii(fi.name);
			}
			_w.Write(s.GetLength(), s.operator LPCSTR());
			_w.Write(sizeof(BYTE), &CR);
		}
		//=== ���R�[�h�����o��
		loop ( i, GetRecordCount() )
		{
			INDEX rec = i;
			loop ( i, _super::GetFieldCount() )
			{
				if ( i != 0 )
				{
					s += ",";
				}
				s += _super::GetStringRecord(rec, i, true);
			}
			_w.Write(s.GetLength(), s.operator LPCSTR());
			_w.Write(sizeof(BYTE), &CR);
		}
	}
	friend class CGridFileTest;
};



}; // TNB


