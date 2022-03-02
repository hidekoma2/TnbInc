#pragma once
/**
 *@file
 * EXIF規格ファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbGdiPlus.h"
#include "TnbBitmapHandle.h"
#include "TnbVector.h"
#include "TnbMap.h"



//TNB Library
namespace TNB
{



/**@ingroup GDIPLUS BITMAP
 * EXIF規格ファイル管理クラス
 *
 *		一組の画像情報とEXIFタグ情報を管理し、ファイル入出力を行うことができます。
 *
 *	@attention 非 UNICODEプロジェクトでも、本クラスの文字列は UNICODE で指定する必要があります。
 *				CUnicode 等、使用してください。
 *
 *	@todo Windows7 においてユーザーコメントが正しくアクセスできない現象発生中。
 *
 *	@par使用例
 *		\code
 *		CExifFile ef;
 *		ef.Attach(L"c:\\A.bmp");		// 対象ファイルアタッチ
 *		//時間
 *		SYSTEMTIME st;
 *		::GetLocalTime(&st);
 *		ef.SetTagInfo(PropertyTagExifDTOrig, st);// 日付設定
 *
 *		ef.Save(L"c:\\B.jpg");		// 保存
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbExifFile.h
 * 
 *	@date 09/01/21 新規作成。
 *	@date 09/02/02 Save処理改善。
 *	@date 15/01/07 ユーザーコメントが正しくアクセス出来ない現象発生。未対応。
 */
class CExifFile : CCopyImpossible
{
public:
	/// 符号無し有理数
	struct TRational
	{
		DWORD	molecular;		///< 分子
		DWORD	denominator;	///< 分母
	};
	/// 符号付き有理数
	struct TSRational
	{
		long	molecular;		///< 分子
		long	denominator;	///< 分母
	};

	/// データ型
	enum EType
	{
		ET_Byte			= PropertyTagTypeByte,		///< 8ビット符号無し整数。
		ET_Ascii		= PropertyTagTypeASCII,		///< ASCII文字列。
		ET_Short		= PropertyTagTypeShort,		///< 16ビット符号無し整数。
		ET_Long			= PropertyTagTypeLong,		///< 32ビット符号無し整数。
		ET_Rational		= PropertyTagTypeRational,	///< 32ビット符号無し有理数（整数 2個。分子、分母）。
		ET_Undefined	= PropertyTagTypeUndefined,	///< 任意の 8bitデータ。
		ET_SLong		= PropertyTagTypeSLONG,		///< 32ビット符号付き整数。
		ET_SRational	= PropertyTagTypeSRational,	///< 32ビット符号付き有理数（整数 2個。分子、分母）。
	};

	/**@ingroup GDIPLUS BITMAP
	 * タグ情報.
	 *	@note タグ一つ分の情報
	 */
	class CTagInfo
	{
	public:
		/// コンストラクタ.
		CTagInfo(void) : m_type(ET_Undefined), m_length(0)
		{
		}
		/**
		 * [確認] データ有無確認
		 *	@retval true データあり
		 *	@retval false なし。
		 */
		bool IsEmpty(void) const
		{
			return m_length == 0;
		}
		/**
		 * [取得] データ型取得
		 *	@return 型
		 */
		EType GetType(void) const
		{
			return m_type;
		}
		/**
		 * [取得] データ数取得.
		 *	@note BYTE数ではありません。
		 *	@return データ数
		 */
		size_t GetLength(void) const
		{
			return m_length;
		}
		/**
		 * [取得] 8bit符号無し整数取得.
		 *	@return データ。
		 */
		CVectorT<BYTE> GetByte(void) const
		{
			return m_Get(BYTE(), ET_Byte);
		}
		/**
		 * [設定] 8bit符号無し整数設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetByte(size_t len, const BYTE* P)
		{
			m_Set(len, P, ET_Byte);
		}
		/**
		 * [設定] 8bit符号無し整数設定.
		 *	@param b データ
		 */
		void SetByte(BYTE b)
		{
			SetByte(1, &b);
		}
		/**
		 * [取得] ASCII取得.
		 *	@retval NULL 型がASCII文字列以外か、サイズ０．
		 *	@retval NULL以外 文字列
		 */
		LPCSTR GetAscii(void) const
		{
			LPCSTR P = NULL;
			if ( m_type == ET_Ascii )
			{
				P = reinterpret_cast<LPCSTR>(m_data.ReferBuffer());
			}
			return P;
		}
		/**
		 * [設定] ASCII設定.
		 *	@param lpsz 文字列。
		 *	@param len NULLを含む文字列長。省略可能。指定する場合、lpsz がそのサイズアクセス可能であるようにすること。
		 */
		void SetAscii(LPCSTR lpsz, size_t len = INVALID_SIZE)
		{
			if ( lpsz == NULL )
			{
				m_Set(0, lpsz, ET_Ascii);
			}
			else
			{
				if ( len == INVALID_SIZE )
				{
					len = STRLIB::GetLen(lpsz) + 1;
				}
				ASSERT( ! ::IsBadReadPtr(lpsz, len) );
				m_Set(len, lpsz, ET_Ascii);
			}
		}
		/**
		 * [取得] 16bit符号無し整数取得.
		 *	@return データ。
		 */
		CVectorT<WORD> GetShort(void) const
		{
			return m_Get(WORD(), ET_Short);
		}
		/**
		 * [設定] 16bit符号無し整数設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetShort(size_t len, const WORD* P)
		{
			m_Set(len, P, ET_Short);
		}
		/**
		 * [設定] 16bit符号無し整数設定.
		 *	@param w データ
		 */
		void SetShort(WORD w)
		{
			SetShort(1, &w);
		}
		/**
		 * [取得] 32bit符号無し整数取得.
		 *	@return データ。
		 */
		CVectorT<DWORD> GetLong(void) const
		{
			return m_Get(DWORD(), ET_Long);
		}
		/**
		 * [設定] 32bit符号無し整数設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetLong(size_t len, const DWORD* P)
		{
			m_Set(len, P, ET_Long);
		}
		/**
		 * [設定] 32bit符号無し整数設定.
		 *	@param d データ
		 */
		void SetLong(DWORD d)
		{
			SetLong(1, &d);
		}
		/**
		 * [取得] 32bit符号無し有理数取得.
		 *	@return データ。
		 */
		CVectorT<TRational> GetRational(void) const
		{
			return m_Get(TRational(), ET_Rational);
		}
		/**
		 * [設定] 32bit符号無し有理数設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetRational(size_t len, const TRational* P)
		{
			m_Set(len, P, ET_Rational);
		}
		/**
		 * [設定] 32bit符号無し有理数設定.
		 *	@param d1 分子
		 *	@param d2 分母
		 */
		void SetRational(DWORD d1, DWORD d2)
		{
			TRational r = { d1, d2 };
			SetRational(1, &r);
		}
		/**
		 * [取得] 任意の 8bitデータ取得.
		 *	@return データ。
		 */
		CByteVector GetUndefined(void) const
		{
			return m_Get(BYTE(), ET_Undefined);
		}
		/**
		 * [設定] 任意の 8bitデータ設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetUndefined(size_t len, const BYTE* P)
		{
			m_Set(len, P, ET_Undefined);
		}
		/**
		 * [取得] 32bit符号付き整数取得.
		 *	@return データ。
		 */
		CVectorT<long> GetSLong(void) const
		{
			return m_Get(long(), ET_SLong);
		}
		/**
		 * [設定] 32bit符号無し整数設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetSLong(size_t len, const long* P)
		{
			m_Set(len, P, ET_SLong);
		}
		/**
		 * [設定] 32bit符号無し整数設定.
		 *	@param l データ
		 */
		void SetSLong(long l)
		{
			SetSLong(1, &l);
		}
		/**
		 * [取得] 32bit符号付き有理数取得.
		 *	@return データ。
		 */
		CVectorT<TSRational> GetSRational(void) const
		{
			return m_Get(TSRational(), ET_SRational);
		}
		/**
		 * [設定] 32bit符号付き有理数設定.
		 *	@param len データ数
		 *	@param P データ
		 */
		void SetSRational(size_t len, const TSRational* P)
		{
			m_Set(len, P, ET_SRational);
		}
		/**
		 * [設定] 32bit符号付き有理数設定.
		 *	@param d1 分子
		 *	@param d2 分母
		 */
		void SetSRational(long d1, long d2)
		{
			TSRational r = { d1, d2 };
			SetSRational(1, &r);
		}
		/**
		 * [取得] プロパティアイテム取得
		 *	@note 本インスタンスの情報を PropertyItemの形に取得します。
		 *	@note _pi.id は操作しません。
		 *	@attention _pi.value のアドレスは、本インスタンス内のアドレスをさしています。
		 *				使用終わるまで本インスタンスは破棄しないよう、ご注意。
		 *	@param[out] _pi アイテム
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool Get(Gdiplus::PropertyItem& _pi)
		{
			if ( m_length != 0 )
			{
				_pi.type = static_cast<WORD>(m_type);
				_pi.length = ToDword(m_data.GetSize());
				_pi.value = m_data.GetBuffer();
				m_data.ReleaseBuffer();
				return true;
			}
			return false;
		}
		/**
		 * [設定] プロパティアイテム設定
		 *	@note PropertyItemの情報を本インスタンスに設定します。
		 *	@param p アイテム
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool Set(const Gdiplus::PropertyItem& p)
		{
			switch ( p.type )
			{
			case ET_Byte: // 8ビット符号無し整数。
				SetByte(p.length, static_cast<const BYTE*>(p.value));
				break;
			case ET_Ascii: // ASCII文字列。
				SetAscii(static_cast<LPCSTR>(p.value));
				break;
			case ET_Short: // 16ビット符号無し整数。
				SetShort(p.length / sizeof(WORD), static_cast<const WORD*>(p.value));
				break;
			case ET_Long: // 32ビット符号無し整数。
				SetLong(p.length / sizeof(DWORD), static_cast<const DWORD*>(p.value));
				break;
			case ET_Rational: // 32ビット符号無し有理数（整数 2個。分子、分母）。
				SetRational(p.length / sizeof(TRational), static_cast<const TRational*>(p.value));
				break;
			case ET_Undefined: // 任意の 8bitデータ。
				SetUndefined(p.length, static_cast<const BYTE*>(p.value));
				break;
			case ET_SLong: // 32ビット符号付き整数。
				SetSLong(p.length / sizeof(long), static_cast<const long*>(p.value));
				break;
			case ET_SRational: // 32ビット符号付き有理数（整数 2個。分子、分母）。
				SetSRational(p.length / sizeof(TSRational), static_cast<const TSRational*>(p.value));
				break;
			default:
				return false;
			}
			return true;
		}
		/**
		 * [取得] 文字列化
		 *	@note 本インスタンスの情報を文字列にします。
		 *	@param mx 配列の最大変換数。省略すると全部。
		 *	@return 文字列
		 */
		CStr ToString(size_t mx = INVALID_SIZE) const
		{
			CStr s;
			size_t ln = min(mx, m_length);
			switch ( m_type )
			{
			case ET_Undefined: // 任意の 8bitデータ。
			case ET_Byte: // 8ビット符号無し整数。
				{
					const BYTE* P = m_data.ReferBuffer();
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u "), P[i]);
					}
				}
				break;
			case ET_Ascii: // ASCII文字列。
				s = reinterpret_cast<LPCSTR>(m_data.ReferBuffer());
				break;
			case ET_Short: // 16ビット符号無し整数。
				{
					const WORD* P = reinterpret_cast<const WORD*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u "), P[i]);
					}
				}
				break;
			case ET_Long: // 32ビット符号無し整数。
				{
					const DWORD* P = reinterpret_cast<const DWORD*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u "), P[i]);
					}
				}
				break;
			case ET_Rational: // 32ビット符号無し有理数（整数 2個。分子、分母）。
				{
					const TRational* P = reinterpret_cast<const TRational*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u/%u "), P[i].molecular, P[i].denominator);
					}
				}
				break;
			case ET_SLong: // 32ビット符号付き整数。
				{
					const long* P = reinterpret_cast<const long*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%d "), P[i]);
					}
				}
				break;
			case ET_SRational: // 32ビット符号付き有理数（整数 2個。分子、分母）。
				{
					const TSRational* P = reinterpret_cast<const TSRational*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%d/%d "), P[i].molecular, P[i].denominator);
					}
				}
				break;
			default:
				break;
			}
			s += CStr::Fmt(_T("(l=%d)"), m_length);
			return s;
		}
	private:
		EType		m_type;		///< 型
		CByteVector	m_data;		///< データ
		size_t		m_length;	///< データ数(byte数ではない)
		// ゲッター
		template<typename T>
		CVectorT<T> m_Get(T dmy, EType t) const
		{
			CVectorT<T> r;
			if ( m_type == t )
			{
				ASSERT( m_data.GetSize() / sizeof(T) == m_length );
				r.SetElements(m_length, reinterpret_cast<const T*>(m_data.ReferBuffer()));
			}
			return r;
		}
		// セッター
		template<typename T>
		void m_Set(size_t len, T* P, EType t)
		{
			m_type = t;
			m_length = len;
			m_data.SetElements(len * sizeof(T), reinterpret_cast<const BYTE*>(P));
		}
	};

	typedef CMapT<PROPID, CTagInfo>	CMapTagInfos;	///<タグ情報のマップ


	//----


	/// コンストラクタ
	CExifFile(void) : m_pBmp(NULL)
	{
	}

	/// デストラクタ
	~CExifFile(void)
	{
		Empty();
	}

	/**
	 * [設定] エンプティ化
	 *	@note 保持してる情報を開放します。
	 */
	void Empty(void)
	{
		if ( m_pBmp != NULL )
		{
			delete m_pBmp;
			m_pBmp = NULL;
		}
		m_tags.RemoveAll();
	}

	/**
	 * [読込] ファイル読込み(Attach).
	 *		指定のファイルを読み込み、 EXIF タグ情報も読み込み、本インスタンスに記憶します。
	 *		指定のファイルはアタッチ中は、ロックされます。
	 *	@param lpszFileName 画像ファイル名。
	 *	@retval true 読込み成功.
	 *	@retval false 失敗.
	 */
	bool Attach(LPCWSTR lpszFileName)
	{
		CGdiPlus::Startup();
		Empty();
		m_pBmp = Gdiplus::Bitmap::FromFile(lpszFileName);
		if ( m_pBmp !=NULL && m_pBmp->GetLastStatus() == Gdiplus::Ok )
		{
			UINT cnt = m_pBmp->GetPropertyCount();
			CWorkMemT<PROPID> propIDs(cnt);
			m_pBmp->GetPropertyIdList(cnt, propIDs);
			loop( i, cnt )
			{
				PROPID id = propIDs[i];
				UINT size = m_pBmp->GetPropertyItemSize(id);
				if ( size > 0 )
				{
					CWorkMem mem(size);
					Gdiplus::PropertyItem* P = reinterpret_cast<Gdiplus::PropertyItem*>(mem.Ref());
					m_pBmp->GetPropertyItem(id, size, P);
					m_tags[id].Set(*P);
				}
			}
			return true;
		}
		Empty();
		return false;
	}

	/**
	 * [読込] ファイル読込み.
	 *		指定のファイルを読み込み、 EXIF タグ情報も読み込み、本インスタンスに記憶します。
	 *	@note この関数は画像情報を複製します。上書きを目的としていない場合、 Attach() のほうが高速です。
	 *	@param lpszFileName 画像ファイル名。
	 *	@retval true 読込み成功.
	 *	@retval false 失敗.
	 */
	bool Load(LPCWSTR lpszFileName)
	{
		if ( Attach(lpszFileName) )
		{
			if ( m_pBmp != NULL )
			{
				delete m_pBmp;
				m_pBmp = NULL;
			}
			HPALETTE hPal = NULL;
			CBitmapHandle b = CGdiPlus::LoadBitmap(lpszFileName);
			m_pBmp = Gdiplus::Bitmap::FromHBITMAP(b, hPal);
			if ( m_pBmp != NULL && m_pBmp->GetLastStatus() == Gdiplus::Ok )
			{
				return true;
			}
		}
		Empty();
		return false;
	}

	/**
	 * [読込] HBITMAP読込み.
	 *		指定のHBITMAPを本インスタンスに記憶します。
	 *	@param hBmp 画像。
	 *	@param hPal パレット。省略可能。
	 *	@retval true 読込み成功.
	 *	@retval false 失敗.
	 */
	bool Load(HBITMAP hBmp, HPALETTE hPal = NULL)
	{
		CGdiPlus::Startup();
		Empty();
		m_pBmp = Gdiplus::Bitmap::FromHBITMAP(hBmp, hPal);
		if ( m_pBmp !=NULL && m_pBmp->GetLastStatus() == Gdiplus::Ok )
		{
			return true;
		}
		Empty();
		return false;
	}

	/**
	 * [作成] Jpegファイル作成.
	 *		本インスタンスで管理している画像と EXIFタグ情報を jpegファイルに書き出します。
	 *	@note Attach() している場合、上書きできません。
	 *	@param lpsz ファイル名。
	 *	@param jpegQuality JPEGのクオリティ(0～100)。省略すると80。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SaveJpeg(LPCWSTR lpsz, LONG jpegQuality = 80)
	{
		return Save(lpsz, CGdiPlus::JPG, jpegQuality);
	}

	/**
	 * [作成] 画像ファイル作成.
	 *		本インスタンスで管理している画像と EXIFタグ情報を BMP, JPEG などのファイルに書き出します。
	 *	@note JPEG 以外の場合 EXIFタグ情報が書き出されない可能性もあります（規格）。
	 *	@note Attach() している場合、上書きできません。
	 *	@param lpsz ファイル名。
	 *	@param fmt フォーマット。省略するとBMPです。
	 *	@param jpegQuality JPEGのクオリティ(0～100)。省略すると80。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Save(LPCWSTR lpsz, CGdiPlus::EBitmapFormat fmt = CGdiPlus::JPG, LONG jpegQuality = 80)
	{
		bool r = m_Save(lpsz, m_pBmp, fmt, jpegQuality);
		if ( ! r && m_pBmp != NULL )
		{
			TRACE0("Exif；画像再構築\n");
			Gdiplus::Color c;
			c.SetFromCOLORREF(RGB(0, 0, 0));
			HBITMAP hBmp = NULL;
			if ( m_pBmp->GetHBITMAP(c, &hBmp) == Gdiplus::Ok && hBmp != NULL )
			{
				HPALETTE h = NULL;
				Gdiplus::Bitmap bm(hBmp, h);
				r = m_Save(lpsz, &bm, fmt, jpegQuality);
				_DeleteObject(hBmp);
			}
		}
		return r;
	}

	/**
	 * [取得] Bitmap取得.
	 *	@note 保持している画像を返します。
	 *	@retval NULL 保持していない
	 *	@return NULL以外 画像。 delete しないでください。
	 */
	const Gdiplus::Bitmap* ReferBitmap(void) const
	{
		return m_pBmp;
	}

	/**
	 * [取得] Bitmap取得.
	 *	@note 保持している画像を返します。
	 *	@return 画像
	 */
	CBitmapHandle GetBitmap(void) const
	{
		CBitmapHandle b;
		if ( m_pBmp != NULL )
		{
			Gdiplus::Color c;
			c.SetFromCOLORREF(RGB(0, 0, 0));
			HBITMAP hBmp = NULL;
			if ( m_pBmp->GetHBITMAP(c, &hBmp) == Gdiplus::Ok && hBmp != NULL )
			{
				b = hBmp;
			}
		}
		return b;
	}

	/**
	 * [参照] EXIFタグ情報一覧参照.
	 *	@return 全EXIFタグ情報
	 */
	const CMapTagInfos& GetTagInfoMap(void) const
	{
		#ifdef _DEBUG
			loop ( i, m_tags.GetSize() )
			{
				TRACE2("tag [%04X] = %s\n", m_tags.Get(i).first, m_tags.Get(i).second.ToString(16));
			}
		#endif
		return m_tags;
	}
	
	/**
	 * [取得] EXIFタグ情報取得
	 *	@param id ID
	 *	@retval NULL 指定のIDの情報なし
	 *	@retval NULL以外 情報
	 */
	const CTagInfo* GetTagInfo(PROPID id) const
	{
		return m_tags.GetValuePtr(id);
	}

	/**
	 * [取得] UserCommentタグ取得.
	 *	@note PropertyTagExifUserComment タグから情報を取得します。
	 *	@return ユーザコメント
	 */
	CStr GetUserCommentTag(void) const
	{
		CStr s;
		const CTagInfo* P = GetTagInfo(PropertyTagExifUserComment);
		if ( P != NULL )
		{
			CByteVector vb = P->GetUndefined();
			if ( vb.GetSize() >= 8 )
			{
				vb.Add(0); //NULL終端追加
				vb.Add(0); //NULL終端追加
				LPCSTR lpsz = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
				bool isUnicode = (STRLIB::Compare(lpsz, "UNICODE") == 0);
				if ( lpsz[0] == 0 && sizeof(TCHAR) == 2 )
				{
					isUnicode = true;
				}
				if ( isUnicode )
				{
					s = reinterpret_cast<LPCWSTR>(&lpsz[8]);
				}
				else
				{
					s = &lpsz[8];
				}
			}
		}
		return s;
	}

	/**
	 * [取得] Copyrightタグ取得.
	 *	@note PropertyTagCopyright タグから情報を取得します。
	 *	@param[out] _s1 撮影著作権者
	 *	@param[out] _s2 編集著作権者
	 */
	void GetCopyrightTag(CStr& _s1, CStr& _s2) const
	{
		_s1.Empty();
		_s2.Empty();
		const CTagInfo* P = GetTagInfo(PropertyTagCopyright);
		if ( P != NULL )
		{
			size_t len = P->GetLength();
			LPCSTR lpsz = P->GetAscii();
			_s1 = lpsz;
			if ( _s1.GetLength() + 1 < len && lpsz != NULL )
			{
				_s2 = &lpsz[_s1.GetLength() + 1];
			}
		}
	}

	/**
	 * [設定] EXIFタグ情報削除.
	 *	@param id ID
	 */
	void RemoveTagInfo(PROPID id)
	{
		m_tags.RemoveKey(id);
	}
	
	/**
	 * [設定] EXIFタグ情報設定.
	 *		任意のIDに任意の情報を設定できます。
	 *	@param id ID
	 *	@param ti 情報
	 */
	void SetTagInfo(PROPID id, const CTagInfo& ti)
	{
		m_tags[id] = ti;
	}

	/**
	 * [設定] EXIFタグ情報設定(ASCII文字列).
	 *		任意のIDに文字列の情報を設定できます。
	 *	@param id ID
	 *	@param lpsz 文字列
	 *	@param len NULLを含む文字列長。省略可能。指定する場合、lpsz がそのサイズアクセス可能であるようにすること。
	 */
	void SetTagInfo(PROPID id, LPCSTR lpsz, size_t len = INVALID_SIZE)
	{
		m_tags[id].SetAscii(lpsz, len);
	}

	/**
	 * [設定] EXIFタグ情報設定(時間ASCII文字列).
	 *	@note 指定の時間を ASCI 20文字で格納します。
	 *	@param id ID。撮影時間の場合、 PropertyTagExifDTOrig を指定します。
	 *	@param st 時間
	 */
	void SetTagInfo(PROPID id, const SYSTEMTIME& st)
	{
		char buf[64];//おおめ
		STRLIB::PrintF(buf, 64, "%04d:%02d:%02d %02d:%02d:%02d"
			, st.wYear, st.wMonth, st.wDay
			,st.wHour, st.wMinute, st.wSecond);
		SetTagInfo(id, buf);
	}

	/**
	 * [設定] バージョンタグ設定.
	 *		バージョン情報に指定のバージョンを設定できます。
	 *	@note PropertyTagExifVer タグに情報を設定します。
	 *	@param ver バージョン。省略すると 0x0210 が割り当てられます。
	 */
	void SetVersionTag(DWORD ver = 0x0210)
	{
		CTagInfo ti;
		CAscii s;
		s.Format("%04X", ver);
		LPCSTR lpsz = s;
		ti.SetUndefined(4, reinterpret_cast<const BYTE*>(lpsz));
		SetTagInfo(PropertyTagExifVer, ti);
	}

	/**
	 * [設定] UserCommentタグ設定(ASCII/JIS).
	 *		ユーザコメント情報に指定の文字列を設定できます。
	 *	@note PropertyTagExifUserComment タグに情報を設定します。
	 *	@param lpsz コメント
	 */
	void SetUserCommentTag(LPCSTR lpsz)
	{
		size_t len = STRLIB::GetLen(lpsz);
		CWorkMemT<BYTE> mm(8 + len);
		if ( STRLIB::IsAsciiOnly(lpsz) )
		{
			const BYTE bb[8] = { 'A', 'S', 'C', 'I', 'I', 0, 0, 0 };
			TNB::MemCopy(&mm[0], bb, 8);
		}
		else
		{
			const BYTE bb[8] = { 'J', 'I', 'S', 0, 0, 0, 0, 0 };
			TNB::MemCopy(&mm[0], bb, 8);
		}
		TNB::MemCopy(&mm[8], lpsz, len);
		CTagInfo ti;
		ti.SetUndefined(8 + len, mm);
		SetTagInfo(PropertyTagExifUserComment, ti);
	}

	/**
	 * [設定] UserCommentタグ設定(UNICODE).
	 *		ユーザコメント情報に指定の文字列を設定できます。
	 *	@note PropertyTagExifUserComment タグに情報を設定します。
	 *	@param lpsz コメント
	 */
	void SetUserCommentTag(LPCWSTR lpsz)
	{
		size_t len = STRLIB::GetLen(lpsz) * sizeof(WCHAR);
		CWorkMemT<BYTE> mm(8 + len);
		const BYTE bb[8] = { 'U', 'N', 'I', 'C', 'O', 'D', 'E', 0 };
		TNB::MemCopy(&mm[0], bb, 8);
		TNB::MemCopy(&mm[8], lpsz, len);
		CTagInfo ti;
		ti.SetUndefined(8 + len, mm);
		SetTagInfo(PropertyTagExifUserComment, ti);
	}

	/**
	 * [取得] Copyrightタグ取得.
	 *	@note PropertyTagCopyright タグに情報を設定します。
	 *	@param lpsz1 撮影著作権者。
	 *	@param lpsz2 編集著作権者。
	 */
	void SetCopyrightTag(LPCSTR lpsz1, LPCSTR lpsz2 = NULL)
	{
		CAscii s2 = lpsz2;
		if ( s2.IsEmpty() )
		{
			SetTagInfo(PropertyTagCopyright, lpsz1);
		}
		else
		{
			CAscii s1 = lpsz1;
			if ( s1.IsEmpty() )
			{
				s1 = " ";
			}
			CWorkMemT<char> mm(s1.GetLength() + s2.GetLength() + 2);
			STRLIB::Copy(&mm[0], s1);
			STRLIB::Copy(&mm[s1.GetLength() + 1], s2);
			SetTagInfo(PropertyTagCopyright, &mm[0], mm.GetSize());
		}
	}

	/**
	 * [設定] GPSバージョン情報設定
	 *	@note PropertyTagGpsVer タグに情報を設定します。
	 */
	void SetGpsVersionTag(void)
	{
		const BYTE a[4] = {2, 0, 0, 0};
		CTagInfo ti;
		ti.SetByte(4, a);
		SetTagInfo(PropertyTagGpsVer, ti);
	}

	/**
	 * [設定] 緯度設定
	 *	@note PropertyTagGpsVer / PropertyTagGpsLatitudeRef / PropertyTagGpsLatitude タグに情報を設定します。
	 *	@param isNorth true なら北緯、 false なら南緯
	 *	@param dd D.
	 *	@param mm M.
	 *	@param ss S.
	 */
	void SetGpsLatitudeTag(bool isNorth, DWORD dd, DWORD mm, double ss)
	{
		SetGpsVersionTag();
		SetTagInfo(PropertyTagGpsLatitudeRef, isNorth ? "N" : "S");
		TRational r[3] = { dd, 1, mm, 1, static_cast<DWORD>(ss * 1000), 1000 };
		CTagInfo ti;
		ti.SetRational(3, r);
		SetTagInfo(PropertyTagGpsLatitude, ti);
	}

	/**
	 * [設定] 経度設定
	 *	@note PropertyTagGpsVer / PropertyTagGpsLongitudeRef / PropertyTagGpsLongitude タグに情報を設定します。
	 *	@param isEast true なら西経、 false なら東経
	 *	@param dd D.
	 *	@param mm M.
	 *	@param ss S.
	 */
	void SetGpsLongitudeTag(bool isEast, DWORD dd, DWORD mm, double ss)
	{
		SetGpsVersionTag();
		SetTagInfo(PropertyTagGpsLongitudeRef, isEast ? "E" : "W");
		TRational r[3] = { dd, 1,  mm, 1,  static_cast<DWORD>(ss * 1000), 1000 };
		CTagInfo ti;
		ti.SetRational(3, r);
		SetTagInfo(PropertyTagGpsLongitude, ti);
	}

	/**
	 * [設定] 高度設定
	 *	@note PropertyTagGpsVer / PropertyTagGpsAltitudeRef / PropertyTagGpsAltitude タグに情報を設定します。
	 *	@param d1 値1。
	 *	@param d2 値2。省略すると１。
	 */
	void SetGpsAltitudeTag(DWORD d1, DWORD d2 = 1)
	{
		SetGpsVersionTag();
		CTagInfo ti;
		ti.SetByte(0);
		SetTagInfo(PropertyTagGpsAltitudeRef, ti);
		ti.SetRational(d1, d2);
		SetTagInfo(PropertyTagGpsAltitude, ti);
	}

private:
	// 画像ファイル作成.
	bool m_Save(LPCWSTR lpsz, Gdiplus::Bitmap* B, CGdiPlus::EBitmapFormat fmt = CGdiPlus::JPG, LONG jpegQuality = 80)
	{
		CGdiPlus::Startup();
		if ( B != NULL )
		{
			UINT cnt = B->GetPropertyCount();
			CWorkMemT<PROPID> propIDs(cnt);
			B->GetPropertyIdList(cnt, propIDs);
			loop( i, cnt )
			{
				PROPID id = propIDs[i];
				B->RemovePropertyItem(id);
			}
			Gdiplus::PropertyItem pi;
			loop ( i, m_tags.GetSize() )
			{
				pi.id = m_tags.Ref(i).first;
				if ( m_tags.Ref(i).second.Get(pi) )
				{
					if ( B->SetPropertyItem(&pi) != Gdiplus::Ok )
					{
						return false;
					}
				}
			}
			return CGdiPlus::SaveBitmap(lpsz, *B, fmt, jpegQuality);
		}
		return false;
	}

	Gdiplus::Bitmap*		m_pBmp;	///< 保持している画像
	CMapTagInfos			m_tags;	///< タグ情報
};



}; // TNB

