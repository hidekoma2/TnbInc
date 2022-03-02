#pragma once
/**
 *@file
 * Gridファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"
#include "TnbGrid.h"



//TNB Library
namespace TNB
{



/**@ingroup DBASE FILE
 * Gridファイル管理クラス
 *
 *		1つ以上の任意の型のフィールドと、複数のレコードからなる情報を扱います。
 *		また、 CSV形式のファイルの読み書きをサポートします。
 *
 *	@par必要ファイル
 *			TnbGridFile.h
 *
 *	@date 09/04/16 新規作成
 */
class CGridFile : public CGrid
{
	DEFSUPER(CGrid);
public:

	/**
	 * [読込] CSVファイル読込み.
	 *	@note レコードは読み込みません。読み込む前に設定しておく必要があります。
	 *	@param lpszFileName ファイル名
	 *	@param hasHeadLine true ならヘッダがあるとし、読み込み対象にしません。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [書出] CSVファイル書き出し
	 *	@param lpszFileName ファイル名
	 *	@param hasHeadLine true なら一行目にヘッダを書き出します。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [読込] 読込み
	 *	@param r リーダ
	 *	@throw CNotSupportException バージョン違い
	 *	@throw CInvalidParamException フィールド情報異常、マーク相違
	 */
	void m_Load(const IReader& r, bool hasHeadLine)
	{
		CByteVector vb = r.ReadExactly();
		vb.Add(0); //終端
		CVectorT<CAscii> vaa = CAsciiOperator::SeparateLine(reinterpret_cast<LPCSTR>(vb.ReferBuffer()));
		if ( hasHeadLine )
		{
			vaa.Remove(0); //一行目を削除
		}
		//=== レコード読込み
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
	 * [書出] 書き出し
	 *	@param[in,out] _w ライタ
	 *	@throw CNotSupportException バージョン違い
	 *	@throw CInvalidParamException フィールド情報異常、マーク相違
	 */
	void m_Save(IWriter& _w, bool hasHeadLine) const
	{
		CAscii s;
		BYTE CR = 0x0D;
		//=== ヘッダ書き出し
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
		//=== レコード書き出し
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


