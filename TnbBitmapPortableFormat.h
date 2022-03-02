#pragma once
/**
 *@file
 * ビットマップポータブルフォーマット管理関係のヘッダ.
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"
#include "TnbBlockLinkVector.h"
#include "TnbFile.h"
#include "TnbStrOperator.h"



//TNB Library
namespace TNB
{



/**@ingroup BITMAP
 * ビットマップポータブルフォーマットクラス.
 *		PNM (Portable aNyMap 形式、Netpbm形式) 画像形式の作成(書き込み)、
 *		および解析(読み込み)をサポートしています。
 *
 *	@todo 16bitは未対応。8bitのみ対応しています。
 *	@todo 読み込み時、区切り文字はスペースか改行に限定されています。
 *
 *	@par必要ファイル
 *			TnbBitmapPortableFormat.h
 *
 *	@date 15/05/01 新規作成。
 *	@date 15/05/07 P4 実装。P1 の白黒が逆だったのを修正。
 */
class CBitmapPortableFormat
{
public:

	/// 種類
	enum EDescriptor
	{
		P1,				///< Portable BitMap ASCII (.PBM)
		P2,				///< Portable GrayMap ASCII (.PGM)
		P3,				///< Portable PixMap ASCII (.PPM)
		P4,				///< Portable BitMap Binary (.PBM)
		P5,				///< Portable GrayMap Binary (.PGM)
		P6,				///< Portable PixMap Binary (.PPM)
	};

	/**
	 * [取得] 最終設定種類取得.
	 *	@note 戻り値は Set() / Load() 直後しか意味を持ちません。
	 *	@return 種類.
	 */
	EDescriptor GetLastDescriptor(void) const
	{
		return m_desc;
	}

	/**
	 * [設定] 設定.
	 *		指定のデータを読み込み、本インスタンスに24bitカラーで記憶します。バイナリ、テキストともにOKです。
	 *	@param size データサイズ
	 *	@param pData データ
	 *	@retval true 成功。種類は GetLastDescriptor() で知ることが出来ます。
	 *	@retval false 失敗
	 */
	bool Set(size_t size, LPCVOID pData)
	{
		CBlockLinkVectorT<TParam> params;
		const char* contents = static_cast<const char*>(pData);
		const char* lpszText = contents;
		INDEX ind = 0;
		CAscii pa;
		bool isFinishied = false;
		while ( ! isFinishied )
		{
			const char* p = lpszText;
			size_t len = 0;
			while ( true )
			{
				if ( *p == 0 )
				{
					if ( len == 0 )
					{
						isFinishied = true;
					}
					break;
				}
				else if ( *p == 0x0D || *p == 0x0A )
				{
					p++;
					if ( *p == 0x0A )
					{
						p++;
					}
					break;
				}
				int s = STRLIB::GetCharSize(*p);
				p += s;
				len += s;
			}
			if ( ! isFinishied )
			{
				char c = contents[ind];
				if ( c == '#' || c == 0 )
				{
					; //記憶しない
				}
				else if ( c < 0x20 && c > 'P' )
				{
					break; //バイナリデータ
				}
				else
				{
					params.Add(TParam(ind, len));
					if ( pa.IsEmpty() )
					{
						pa.SetFromLeft(&contents[params[0].index], params[0].size);
					}
					if ( pa == "P4" && params.GetSize() >= 2 )
					{
						break;
					}
					if ( (pa == "P5" || pa == "P6") && params.GetSize() >= 3 )
					{
						break;
					}
				}
				ind += p - lpszText;
				lpszText += p - lpszText;
			}
		}
		//
		if ( params.GetSize() < 2 )
		{
			return false; 
		}
		DWORD colorDips = 0;	// カラー深さ
		SIZE sz;			// 画像サイズ
		// サイズチェック
		CAscii a;
		a.SetFromLeft(&contents[params[1].index], params[1].size);
		while ( a.Replace("\t", " ") != 0 );
		while ( a.Replace("  ", " ") != 0 );
		CAsciiVector va = CAsciiOperator::SeparatePeriod(a, ' ');
		if ( va.GetSize() != 2 )
		{
			return false;
		}
		sz.cx = va[0].ToInt();
		sz.cy = va[1].ToInt();
		if ( sz.cx == 0 || sz.cy == 0 )
		{
			return false;
		}
		//
		if ( pa == "P2" || pa == "P3" || pa == "P5" || pa == "P6" )
		{
			// P2, P3, P5, P6 はカラー深さが存在
			if ( params.GetSize() < 3 )
			{
				return false; 
			}
			const char * pp = &contents[params[2].index];
			colorDips = STRLIB::ToInt(pp);
		}
		if ( pa == "P1" || pa == "P2" || pa == "P3" )
		{
			// P1, P2, P3
			int ex = (pa == "P1") ? 2 : 3;
			CVectorT<DWORD> bm;
			loop ( i, params.GetSize() - ex )
			{
				a.SetFromLeft(&contents[params[i + ex].index], params[i + ex].size);
				while ( a.Replace("\t", " ") != 0 );
				while ( a.Replace("  ", " ") != 0 );
				CAsciiVector va = CAsciiOperator::SeparatePeriod(a, ' ');
				loop ( j, va )
				{
					bm.Add(va[j].ToInt());
				}
			}
			if ( pa == "P1" )
			{
				// P1
				if ( size_t(sz.cx * sz.cy) > bm.GetSize() )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx;
					loop ( x, sz.cx )
					{
						if ( bm[m + x] == 0 )
						{
							p->rgbRed = 255;
							p->rgbGreen = 255;
							p->rgbBlue = 255;
						}
						p++;
					}
				}
				m_desc = P1;
				return true;
			}
			else if ( pa == "P2" )
			{
				// P2
				if ( size_t(sz.cx * sz.cy) > bm.GetSize() )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx;
					loop ( x, sz.cx )
					{
						BYTE c = static_cast<BYTE>(bm[m + x] * 255 / colorDips);
						p->rgbRed = c;
						p->rgbGreen = c;
						p->rgbBlue = c;
						p++;
					}
				}
				m_desc = P2;
				return true;
			}
			else // pa == "P3"
			{
				// P3
				if ( size_t(sz.cx * sz.cy * 3) > bm.GetSize() )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx * 3;
					loop ( x, sz.cx )
					{
						p->rgbRed = static_cast<BYTE>(bm[m + x * 3 + 0] * 255 / colorDips);
						p->rgbGreen = static_cast<BYTE>(bm[m + x * 3 + 1] * 255 / colorDips);
						p->rgbBlue = static_cast<BYTE>(bm[m + x * 3 + 2] * 255 / colorDips);
						p++;
					}
				}
				m_desc = P3;
				return true;
			}
		}
		else if ( pa == "P4" || pa == "P5" || pa == "P6" )
		{
			int ex = (pa == "P4") ? 1 : 2;
			const char* q0 = &contents[params[ex].index + params[ex].size];
			size_t len = size - (params[ex].index + params[ex].size);
			if ( *q0 == 0x0D || *q0 == 0x0A )
			{
				q0++;
				len--;
				//if ( *q0 == 0x0A )
				//{
				//	q0++;
				//	len--;
				//}
			}
			const BYTE* q = reinterpret_cast<const BYTE*>(q0);
			if ( pa == "P4" )
			{
				// P4
				if ( size_t(((sz.cx + 7 ) / 8) * sz.cy) > len )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * ((sz.cx + 7) / 8);
					loop ( x, sz.cx )
					{
						if ( (q[m + (x / 8)] & _BIT(7 - (x & 7))) == 0 )
						{
							p->rgbRed = 255;
							p->rgbGreen = 255;
							p->rgbBlue = 255;
						}
						p++;
					}
				}
				m_desc = P4;
				return true;
			}
			else if ( pa == "P5" )
			{
				// P5
				if ( size_t(sz.cx * sz.cy) > len )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx;
					loop ( x, sz.cx )
					{
						BYTE c = static_cast<BYTE>(q[m + x] * 255 / colorDips);
						p->rgbRed = c;
						p->rgbGreen = c;
						p->rgbBlue = c;
						p++;
					}
				}
				m_desc = P5;
				return true;
			}
			else // pa == "P6"
			{
				// P6
				if ( size_t(sz.cx * sz.cy * 3) > len )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx * 3;
					loop ( x, sz.cx )
					{
						p->rgbRed = static_cast<BYTE>(q[m + x * 3 + 0] * 255 / colorDips);
						p->rgbGreen = static_cast<BYTE>(q[m + x * 3 + 1] * 255 / colorDips);
						p->rgbBlue = static_cast<BYTE>(q[m + x * 3 + 2] * 255 / colorDips);
						p++;
					}
				}
				m_desc = P6;
				return true;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * [設定] ファイル読込み.
	 *		指定のファイルを読み込み、本インスタンスに24bitカラーで記憶します。
	 *	@param lpszFileName ファイル名
	 *	@retval true 成功。種類は GetLastDescriptor() で知ることが出来ます。
	 *	@retval false 失敗
	 */
	bool Load(LPCTSTR lpszFileName)
	{
		CFileReader rr;
		if ( rr.Open(lpszFileName) )
		{
			CByteVector vb = rr.ReadExactly();
			return Set(vb.GetSize(), vb.ReferBuffer());
		}
		return false;
	}

	/**
	 * [変換] バイナリ変換.
	 *	@param desc 種類
	 *	@param prm パラメータ。 種類が P1,P4 の場合「敷居値」、その他の種類の場合「色スケールの最大値」を指定します。
	 *	@return 変換後バイナリ
	 */
	CByteVector ToVector(EDescriptor desc, DWORD prm, LPCTSTR lpszComment = NULL) const
	{
		CByteVector vb;
		if ( ! m_bmp.IsEmpty() )
		{
			CAscii a;
			CAscii asz;
			SIZE sz = m_bmp.GetSize();
			asz.Format("%d %d\n", sz.cx, sz.cy);
			if ( lpszComment != NULL )
			{
				asz.Format("#%s\n%d %d\n", CAscii(lpszComment), sz.cx, sz.cy);
			}
			else
			{
				asz.Format("%d %d\n", sz.cx, sz.cy);
			}
			switch ( desc )
			{
			case P1:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P1\n" + asz;
					INDEX idx = 0;
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							if ( p->rgbRed < prm )
							{
								a += "1 "; 
							}
							else
							{
								a += "0 "; 
							}
							if ( idx++ >= 32 )
							{
								a.TrimRight(' ');
								a += "\n";
								idx = 0;
							}
							p++;
						}
					}
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
				}
				break;
			case P2:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P2\n" + asz + CAscii::Fmt("%d\n", prm);
					INDEX idx = 0;
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int q = p->rgbRed * prm / 255;
							a += CAscii::Fmt("%d ", q);
							if ( idx++ >= 32 )
							{
								a.TrimRight(' ');
								a += "\n";
								idx = 0;
							}
							p++;
						}
					}
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
				}
				break;
			case P3:
				{
					a = "P3\n" + asz + CAscii::Fmt("%d\n", prm);
					INDEX idx = 0;
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int r = p->rgbRed * prm / 255;
							int g = p->rgbGreen * prm / 255;
							int b = p->rgbBlue * prm / 255;
							a += CAscii::Fmt("%d %d %d ", r, g, b);
							if ( idx++ >= 32 )
							{
								a.TrimRight(' ');
								a += "\n";
								vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
								a.Empty();
								idx = 0;
							}
							p++;
						}
					}
					if ( a.GetLength() > 0 )
					{
						vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					}
				}
				break;
			case P4:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P4\n" + asz;
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					CByteVector v;
					int yyy = (sz.cx + 7) / 8;
					v.SetSize(sz.cy * yyy);
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							BYTE& b = v.Ref((y * yyy) + (x / 8));
							if ( p->rgbRed < prm )
							{
								b |= _BIT(7 - (x & 7)); 
							}
							else
							{
								b  &= ~(_BIT(7 - (x & 7))); 
							}
							p++;
						}
					}
					vb += v;
				}
				break;
			case P5:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P5\n" + asz + CAscii::Fmt("%d\n", prm);
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int q = p->rgbRed * prm / 255;
							vb.Add(static_cast<BYTE>(q));
							p++;
						}
					}
				}
				break;
			case P6:
				{
					a = "P6\n" + asz + CAscii::Fmt("%d\n", prm);
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int r = p->rgbRed * prm / 255;
							int g = p->rgbGreen * prm / 255;
							int b = p->rgbBlue * prm / 255;
							vb.Add(static_cast<BYTE>(r));
							vb.Add(static_cast<BYTE>(g));
							vb.Add(static_cast<BYTE>(b));
							p++;
						}
					}
				}
				break;
			}
		}
		return vb;
	}

	/**
	 * [保存] ファイル書き込み.
	 *		本インスタンスに記憶している画像を指定の種類で指定のファイルへ書き込みます。
	 *	@param lpszFileName ファイル名
	 *	@param desc 種類
	 *	@param prm パラメータ。 種類が P1,P4 の場合「敷居値」、その他の種類の場合「色スケールの最大値」を指定します。
	 *	@param lpszComment コメント。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Save(LPCTSTR lpszFileName, EDescriptor desc, DWORD prm, LPCTSTR lpszComment = NULL) const
	{
		CByteVector vb = ToVector(desc, prm, lpszComment);
		if ( vb.GetSize() > 0 )
		{
			return CFileWriter::NewFile(lpszFileName, vb.GetSize(), vb.ReferBuffer());
		}
		return false;
	}

	/**
	 * [参照] 画像参照.
	 *	@return 画像クラス
	 */
	CBitmapImage& Bitmap(void) 
	{
		return m_bmp;
	}

private:
	/// パラメータ
	struct TParam
	{
		INDEX	index;		///< インデックス
		size_t	size;		///< サイズ
		// コンストラクタ.
		TParam(INDEX i = 0, size_t s = 0) : index(i), size(s)
		{
		}
	};

	CBitmapImage	m_bmp;		///< 保持画像
	EDescriptor		m_desc;		///< 保持フォーマット
};



}; // TNB
