#pragma once
/**
 *@file
 * dBaseファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"
#include "TnbGrid.h"



//TNB Library
namespace TNB
{



#pragma pack(push)
#pragma pack(1)



/**@ingroup DBASE FILE
 * dBaseファイル管理クラス
 *
 *		1つ以上の任意の型のフィールドと、複数のレコードからなる情報を扱います。
 *		また、 dBase 形式のファイルの読み書きをサポートします。
 *
 *	@note dBase のデータ種をサポートしています。
 *	@note メモフィールドはサポートしていません。
 *
 *	@attention UNICODE プロジェクトでは、全角の文字数の考え方が異なるため、使用には注意が必要。
 *
 *	@par必要ファイル
 *			TnbDBaseFile.h
 *
 *	@date 06/04/20 新規作成
 *	@date 10/10/07 UNICODE 時のファイル書き込みを暫定修正。
 */
class CDBaseFile : public CGrid
{
	DEFSUPER(CGrid);
public:

	/**
	 * [読込] DBFファイル読込み
	 *	@param lpszFileName ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [書出] DBFファイル書き出し
	 *	@param lpszFileName ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * ファイルヘッダ型
	 */
	struct THead
	{
		BYTE	version			:3;		///< バージョン
		BYTE	withMemoFile4	:1;		///< dBASE IVメモファイルの有無
		BYTE	withSqlTable	:3;		///< SQLテーブルの有無
		BYTE	withMemoFile	:1;		///< ファイルの有無S
		BYTE	year;					///< 年から 1900 ひいた数
		BYTE	month;					///< 月
		BYTE	day;					///< 日
		DWORD	recordCount;			///< レコード数
		WORD	headBytes;				///< ヘッダのバイト数
		WORD	recordBytes;			///< レコードのバイト数
		BYTE	padding1[2];			///< 予約
		BYTE	transaction;			///< トランザクション未了フラグ
		BYTE	encryptionFlag;			///< 暗号化フラグ
		BYTE	padding2[12];			///< 予約
		BYTE	withMdxFile;			///< MDXファイルフラグ ０；なし
		BYTE	language;				///< 言語ドライバID
		BYTE	padding3[2];			///< 予約
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
	C_ASSERT ( sizeof(THead) == 32 ); ///< サイズチェック

	/**
	 * フィールド記述子型
	 */
	struct TField
	{
		char	name[11];				///< フィールド名
		char	type;					///< 型
		BYTE	padding1[4];			///< 予約
		BYTE	fieldLength;			///< フィールドサイズ
		BYTE	decimalLength;			///< 内、小数点の長さ
		BYTE	padding2[2];			///< 予約
		BYTE	wordId;					///< 作業領域ID
		BYTE	padding3[10];			///< 予約
		BYTE	withMdxField;			///< MDXフィールドフラグ
		TField(void)
		{
			Zero(*this);
		}
		/// 最大
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
	C_ASSERT ( sizeof(TField) == 32 ); ///< サイズチェック

	/**
	 * [読込] 読込み
	 *	@param r リーダ
	 *	@throw CNotSupportException バージョン違い
	 *	@throw CInvalidParamException フィールド情報異常、マーク相違
	 */
	void m_Load(const IReader& r)
	{
		//=== ヘッダ読込み
		THead hd;
		r.ReadExactly(sizeof(THead), &hd);
		if ( hd.version != 3 )
		{
			throw CNotSupportException();
		}
		//=== フィールド読込み
		size_t fieldCount = (hd.headBytes - sizeof(THead)) / sizeof(TField); //フィールド数
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
			TRACE2("ヘッダのレコードサイズ(%d)と各フィールドに書かれているサイズの合計(%d)が異なります。\n", hd.recordBytes, recordBytesTemp);
			throw CInvalidParamException();
		}
		//=== レコード読込み
		loop ( i, hd.recordCount )
		{
			CWorkMemT<char> buf(hd.recordBytes);	//レコードのバイト数
			char* B = buf.Ref();
			r.ReadExactly(hd.recordBytes, B);
			if ( B[0] != 0x20 )
			{
				continue; //sp以外はデリートレコード
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
		//=== 終端
		r.ReadExactly(sizeof(BYTE), &b);
		if ( b != 0x1A )
		{
			throw CInvalidParamException();
		}
		_super::Dump();
	}

	/**
	 * [書出] 書き出し
	 *	@param[in,out] _w ライタ
	 *	@throw CNotSupportException バージョン違い
	 *	@throw CInvalidParamException フィールド情報異常、マーク相違
	 */
	void m_Save(IWriter& _w) const
	{
		//=== ヘッダ書き出し
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
		//=== フィールド書き出し
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
		//=== レコード書き出し
		loop ( i, GetRecordCount() )
		{
			CWorkMemT<char> buf(hd.recordBytes);	//レコードのバイト数
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
		//=== 終端
		b = 0x1A;
		_w.Write(sizeof(BYTE), &b);
	}

	friend class CDBaseFileTest;
};



#pragma pack(pop)



}; // TNB


