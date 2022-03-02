#pragma once
/**
 *@file
 * 圧縮情報関係のヘッダ
 *
 *		圧縮、展開をサポート
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbVector.h"
#include "TnbCollectionAdapter.h"



//TNB Library
namespace TNB
{



/**@ingroup CIPHER
 * 圧縮展開処理クラス
 *
 *		BYTEの羅列を圧縮、展開するクラスです。
 *
 *	@note Encodeしたデータの塊を、クラッドと呼びます。
 *
 *	@note {@link COLLECT コレクション}以外のメモリは、 CConstAdapterT を用いて代入が可能です。
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CByteVector vb;
 *		CSlideCompressor slide;
 *		vb = slide.Encode(CConstAdapterT(5, "12345"));			
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbSlideCompressor.h
 *
 *	@date 99/01/01 新規作成
 *	@date 06/07/20 クラス化
 *	@date 06/08/10 出力先も抽象化指定できるようにした。
 */
class CSlideCompressor
{
	#define TNB_SLD_HEAD	"SLD"				///< ヘッダベース
	#define TNB_SLD_NON		TNB_SLD_HEAD "0"	///< 非圧縮データヘッダ
	#define TNB_SLD_LV1		TNB_SLD_HEAD "1"	///< 圧縮１データヘッダ
	/// 設定値
	enum
	{
		DIC_SIZE	= 4096,	///< 辞書サイズ
		MAXMATCH	= 18,	///< 最大マッチ数
		THRESHOLD	= 3,	///< 敷居
		NIL			= 0,	///< NIL
		DIC_BIT		= 12,	///< 辞書のBIT数
	};

	/// バッファ管理型
	struct TBuf
	{
		/// 設定値
		enum
		{
			DIC_SIZE	= 4096,
			MAXMATCH	= 18,
			MAX_HASH_VAL= (3 * DIC_SIZE + (DIC_SIZE / 512 + 1) * UCHAR_MAX)
		};
		BYTE	text[DIC_SIZE * 2 + MAXMATCH];			///< 辞書バッファ
		int		level[DIC_SIZE + UCHAR_MAX + 1];		///< 文字 ｎ を頂点としての深さ、最大一致長
		int		childcount[DIC_SIZE + UCHAR_MAX + 1];	///< 文字 ｎ の子の数
		int		position[DIC_SIZE + UCHAR_MAX + 1];		///< 位置 ｎ からの最大一致長ポインタ
		int		parent[DIC_SIZE * 2];					///< 位置 ｎ 親ノードをさす
		int		prev[DIC_SIZE * 2];						///< Availフラグ
		int		next[MAX_HASH_VAL + 1];					///< Availフラグ
	};

	TBuf*	m_ptBuf;		///< バッファ
	int		m_iMatchLen;	///< 一致サイズ
	int		m_iMatchPos;	///< 一致位置
	int		m_iPos;			///< 位置
	int		m_iAvail;		///< Availフラグ

	ICollectionT<BYTE> *			m_pDstDat;		///< 書き込み対象参照
	const IConstCollectionT<BYTE>*	m_pSrcDat;		///< 読み込み対象参照
	int								m_iSrcPos;		///< 読み込み位置
	int								m_iSrcSize;		///< 読み込み対象サイズ

	/// 圧縮用
	int m_Hash(int p, int c)
	{
		return ((p) + ((c) << (DIC_BIT - 9)) + DIC_SIZE * 2);
	}

	/// 圧縮用
	void insert_node(void)
	{
		TBuf& B = *m_ptBuf;
		int r, t, q, j;
		BYTE c, *t1, *t2;
		//
		if ( m_iMatchLen >= 4 )
		{
			m_iMatchLen--;
			r = (m_iMatchPos + 1) | DIC_SIZE;
			while ( (q = B.parent[r]) == NIL )
			{
				r = B.next[r];
			}
			while ( B.level[q] >= m_iMatchLen ) 
			{
				r = q;
				q = B.parent[q];
			}
			t = q;
			while ( t < DIC_SIZE ) 
			{
				B.position[t] = m_iPos;
				t = B.parent[t];
			}
		} 
		else 
		{
			q = B.text[m_iPos] + DIC_SIZE;
			c = B.text[m_iPos + 1];
			r = searchchild(q, c);
			if ( r == NIL ) 
			{
				makechild(q, c, m_iPos);
				m_iMatchLen = 1;
				return;
			}
			m_iMatchLen = 2;
		}
		while ( true )
		{
			if ( r >= DIC_SIZE ) 
			{
				j = MAXMATCH;
				m_iMatchPos = r;
			}
			else
			{
				j = B.level[r];
				m_iMatchPos = B.position[r];
			}
			if ( m_iMatchPos >= m_iPos )
			{
				m_iMatchPos -= DIC_SIZE;
			}
			t1 = &B.text[m_iPos + m_iMatchLen];
			t2 = &B.text[m_iMatchPos + m_iMatchLen];
			while ( m_iMatchLen < j ) 
			{
				if ( *t1 != *t2 )
				{
					split(r);
					return;
				}
				m_iMatchLen++;
				t1++;
				t2++;
			}
			if ( m_iMatchLen == MAXMATCH )
			{
				break;
			}
			B.position[r] = m_iPos;
			q = r;
			r = searchchild(q, *t1);
			if ( r == NIL ) 
			{
				makechild(q, *t1, m_iPos);
				return;
			}
			m_iMatchLen++;
		}
		t = B.prev[r];		B.prev[m_iPos] = t;		B.next[t] = m_iPos;
		t = B.next[r];		B.next[m_iPos] = t;		B.prev[t] = m_iPos;
		B.parent[m_iPos] = q;	B.parent[r] = NIL;
		B.next[r] = m_iPos;	
	}

	/// 圧縮用
	void delete_node(void)
	{
		TBuf& B = *m_ptBuf;
		int r, t, s, u;
		if ( B.parent[m_iPos] == NIL ) 
		{
			return;
		}
		r = B.prev[m_iPos];
		s = B.next[m_iPos];
		B.next[r] = s;
		B.prev[s] = r;
		r = B.parent[m_iPos];
		B.parent[m_iPos] = NIL;
		if ( r >= DIC_SIZE || --B.childcount[r] > 1 ) 
		{
			return;
		}
		t = B.position[r];
		if ( t >= m_iPos )
		{
			t -= DIC_SIZE;
		}
		s = searchchild(r, B.text[t + B.level[r]]);
		t = B.prev[s];	u = B.next[s];
		B.next[t] = u;	B.prev[u] = t;
		t = B.prev[r];	B.next[t] = s;	B.prev[s] = t;
		t = B.next[r];	B.prev[t] = s;	B.next[s] = t;
		B.parent[s] = B.parent[r];
		B.parent[r] = NIL;
		B.next[r] = m_iAvail;
		m_iAvail = r;
	}

	/// 圧縮用
	int searchchild(int q, BYTE c)
	{
		TBuf& B = *m_ptBuf;
		int r;
		r = B.next[m_Hash(q, c)];
		B.parent[NIL] = q;				/* sentinel */
		while ( B.parent[r] != q )
		{
			r = B.next[r];
		}
		return r;
	}

	/// 圧縮用
	void makechild(int q, BYTE c, int r)
	{
		TBuf& B = *m_ptBuf;
		int h, t;
		h = m_Hash(q, c);
		t = B.next[h];		B.next[h] = r;		B.next[r] = t;
		B.prev[t] = r;		B.prev[r] = h;
		B.parent[r] = q;
		B.childcount[q]++;
	}

	/// 圧縮用
	void split(int old)
	{
		TBuf& B = *m_ptBuf;
		int sNew, t;
		sNew = m_iAvail;	m_iAvail = B.next[sNew];	B.childcount[sNew] = 0;
		t = B.prev[old];	B.prev[sNew] = t;			B.next[t] = sNew;
		t = B.next[old];	B.next[sNew] = t;			B.prev[t] = sNew;
		B.parent[sNew] = B.parent[old];
		B.level[sNew] = m_iMatchLen;
		B.position[sNew] = m_iPos;
		makechild(sNew, B.text[m_iMatchPos + m_iMatchLen], old);
		makechild(sNew, B.text[m_iPos + m_iMatchLen], m_iPos);
	}

	/// ソースからリザルトへコピー
	bool m_Copy(size_t iLen)
	{
		return m_pDstDat->Append(CConstOffsetAdapterT<BYTE>(m_pSrcDat, m_iSrcPos, iLen)) == iLen;
	}

	/// ソースから読み込み
	int m_Read(LPVOID lpBuf, int iLen)
	{
		if ( iLen + m_iSrcPos > m_iSrcSize )
		{
			iLen = m_iSrcSize - m_iSrcPos;
		}
		BYTE* B = static_cast<BYTE*>(lpBuf);
		loop ( i, iLen )
		{
			*B++ = m_pSrcDat->At(m_iSrcPos++);
		}
		return iLen;
	}

	/// リザルトに追加
	bool m_Write(LPCVOID lpBuf, size_t iLen)
	{
		return m_pDstDat->AddElements(iLen, static_cast<const BYTE*>(lpBuf)) == iLen;
	}

	/**
	 * 非圧縮クラッド作成.
	 *		圧縮が意味ないときに使用される。
	 */
	bool m_EncodePlain(void)
	{
		bool r = true;
		r &= m_pDstDat->RemoveAll();
		m_iSrcPos = 0;
		r &= m_Write(TNB_SLD_NON, 4);
		r &= m_Write(&m_iSrcSize, 4);
		r &= m_Write(&m_iSrcSize, 4);
		r &= m_Copy(m_iSrcSize);
		return r;
	}

	/**
	 * [処理] 圧縮
	 *	@param[out]	_out エンコードデータ格納先。
	 *	@param[in] in 圧縮対象データ
	 *	@param[in] boIsPlain 省略すると、非圧縮/圧縮を自動判断します。<BR>
	 *					trueを指定すると、非圧縮クラッドが作成されます。
	 *	@retval true エンコード成功。
	 *	@retval false 失敗。
	 */
	bool m_Encode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in, bool boIsPlain = false)
	{
		bool r = true;
		TBuf& B = *m_ptBuf;
		m_pDstDat = &_out;
		m_pSrcDat = &in;
		m_iSrcPos = 0;
		m_iSrcSize = ToInt(m_pSrcDat->GetSize());
		r &= m_pDstDat->RemoveAll();
		//
		if ( m_iSrcSize == 0 || boIsPlain )
		{
			// サイズが 0 のは圧縮意味無し
			return m_EncodePlain();
		}
		//--- クラスワーク初期化
		m_iPos = 0;
		m_iMatchPos = 0;
		m_iAvail = 1;
		m_iPos = DIC_SIZE + MAXMATCH;
		m_iMatchLen = 0;
		for ( int i = DIC_SIZE; i <= DIC_SIZE + UCHAR_MAX; i++ )
		{
			B.level[i] = 1;
		}
		for ( int i = DIC_SIZE; i < DIC_SIZE * 2; i++ )
		{
			 B.parent[i] = NIL;
		}
		for ( int i = 1; i < DIC_SIZE - 1; i++ )
		{
			B.next[i] = i + 1;
		}
		B.next[DIC_SIZE - 1] = NIL;
		for ( int i = DIC_SIZE * 2; i <= TBuf::MAX_HASH_VAL; i++ )
		{
			 B.next[i] = NIL;
		}
		//--- ローカルワーク初期化
		BYTE	code[17];
		BYTE	mask = 1;
		code[0] = 0;
		int	codeptr = 1;
		int comp_size = 0;
		//-- ヘッダ書き出し
		r &= m_Write(TNB_SLD_LV1, 4);
		r &= m_Write(&m_iSrcSize, 4);
		r &= m_Write(&m_iSrcSize, 4);
		//-- 解析
		int remainder = m_Read(&B.text[m_iPos], DIC_SIZE);
		insert_node();
		while ( remainder > 0 )
		{
			int lastmatchlen = m_iMatchLen;
			int lastmatchpos = m_iMatchPos;
			remainder--;
			if ( ++m_iPos == DIC_SIZE * 2 )
			{
				MemCopy(&B.text[0], &B.text[DIC_SIZE], DIC_SIZE + MAXMATCH);
				m_Read(&B.text[DIC_SIZE + MAXMATCH], DIC_SIZE);
				remainder += DIC_SIZE;
				m_iPos = DIC_SIZE;
			}
			delete_node();
			insert_node();
			if ( m_iMatchLen > remainder )
			{
				m_iMatchLen = remainder;
			}
			if ( m_iMatchLen > lastmatchlen || lastmatchlen < THRESHOLD )
			{
				code[codeptr++] = B.text[m_iPos - 1];
				code[0] |= mask;
			} 
			else 
			{
				code[codeptr++] = static_cast<BYTE>(m_iPos - 1 - lastmatchpos);
				code[codeptr++] = static_cast<BYTE>((((m_iPos - 1 - lastmatchpos) >> 4) & 0xf0) | (lastmatchlen - THRESHOLD));
				remainder = remainder - lastmatchlen + 1;
				while ( --lastmatchlen > 0 )
				{
					if ( ++m_iPos == DIC_SIZE * 2 )
					{
						MemCopy(&B.text[0], &B.text[DIC_SIZE], DIC_SIZE + MAXMATCH);
						m_Read(&B.text[DIC_SIZE + MAXMATCH], DIC_SIZE);
						remainder += DIC_SIZE;
						m_iPos = DIC_SIZE;
					}
					delete_node();
					insert_node();
				}
				if ( m_iMatchLen > remainder )
				{
					m_iMatchLen = remainder;
				}
			}
			if ( (mask <<= 1) == 0 )
			{
				if ( comp_size + codeptr + 8 > m_iSrcSize )
				{
					// 元よりでかくなってしまう
					return m_EncodePlain();
				}
				r &= m_Write(&code[0], codeptr);
				comp_size += codeptr;
				code[0] = 0;
				codeptr = mask = 1;
			}
		}
		if ( codeptr > 1 ) 
		{
			r &= m_Write(&code[0], codeptr);
			comp_size += codeptr;
		}
		if ( comp_size + 8 > m_iSrcSize )
		{
			// 元よりでかくなってしまう
			return m_EncodePlain();
		}
		ASSERTLIB( ToInt(m_pDstDat->GetSize()) == comp_size + 12 );
		return r; 
	}

	/**
	 * [処理] 展開
	 *	@param[out]	_out デコードデータ格納先。追加されますので、通常Emptyのコレクションを指定します。
	 *	@param[in] in Encode() で作成したクラッド。
	 *	@retval true デコード成功。
	 *	@retval false 失敗。
	 */
	bool m_Decode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in)
	{
		bool r = true;
		TBuf& B = *m_ptBuf;
		m_pDstDat = &_out;
		m_pSrcDat = &in;
		m_iSrcPos = 0;
		m_iSrcSize = ToInt(m_pSrcDat->GetSize());
		r &= m_pDstDat->RemoveAll();
		//
		int	fl;		//展開後のサイズ
		//--- Headﾁｪｯｸ
		{
			BYTE hd[4] = {0};
			LONG lLen1 = 0;
			LONG lLen2 = 1;
			m_Read(hd, 4);
			if ( memcmp(hd, TNB_SLD_HEAD, 3) != 0 )
			{
				return false;
			}
			m_Read(&lLen1, 4);
			m_Read(&lLen2, 4);
			//
			if ( lLen1 != lLen2 )
			{
				return false;
			}
			fl = lLen1;	//ファイルの長さ
			//
			if ( memcmp(hd, TNB_SLD_NON, 4) == 0 )
			{
				//非圧縮
				if ( m_Copy(fl) && ToInt(m_pDstDat->GetSize()) == fl )
				{
					return true;
				}
				return false;
			}
		}
		//--- クラスワーク初期化
		for ( int i = 0; i < DIC_SIZE - MAXMATCH; i++ )
		{
			B.text[i] = 0;
		}
		//--- ローカルワーク初期化
		int		count = 0;
		int		posi = 0;
		BYTE	flags = 0;
		//--- 展開
		while ( true )
		{
			if ( count >= fl )
			{
				break;
			}
			if ( m_Read(&flags, 1) != 1 )
			{
				break;
			}
			int g = 8;
			do 
			{
				if ( (flags & 1) != 0 )
				{
					BYTE	fd;
					if ( m_Read(&fd, 1) != 1 )
					{
						break;
					}
					B.text[posi++] = fd;
					if ( posi == DIC_SIZE || count + posi >= fl )
					{
						r &= m_Write(B.text, posi);
						count += posi;
						posi = 0;
					}
				}
				else 
				{
					BYTE fd[2];
					if ( m_Read(&fd, 2) != 2 )
					{
						break;
					}
					int d1=fd[0];
					int d2=fd[1];
					d1 |= ((d2 & 0xf0) << 4);
					d2 = (d2 & 0x0f) + 2;
					d1 = posi - d1;
					for ( int k = 0; k <= d2; k++ )
					{
						B.text[posi++] = B.text[(d1 + k) & (DIC_SIZE - 1)];
						if ( posi == DIC_SIZE || count + posi >= fl )
						{
							r &= m_Write(B.text, posi);
							count += posi;
							posi = 0;
						}
					}
				}
				flags >>= 1;
				//
				if ( count >= fl )
				{
					break;
				}
			} while ( --g != 0 );
		}
		if ( count < fl )
		{
			return false;
		}
		if ( posi > 0 )
		{
			r &= m_Write(B.text, posi);
			count += posi;
		}
		return r;
	}

public:

	/// コンストラクタ
	CSlideCompressor(void)
	{
		m_ptBuf = new TBuf;
	}

	/// デストラクタ
	~CSlideCompressor(void)
	{
		if ( m_ptBuf != NULL )
		{
			delete m_ptBuf;
			m_ptBuf = NULL;
		}
	}

	/**
	 * [処理] 圧縮
	 *	@param[out]	_out クラッド格納先。追加されますので、通常Emptyのコレクションを指定します。
	 *	@param[in]	in 圧縮対象
	 *	@param[in]	 boIsPlain 省略すると、非圧縮/圧縮を自動判断します。<BR>
	 *					trueを指定すると、非圧縮クラッドが作成されます。
	 *	@retval true エンコード成功。
	 *	@retval false 失敗。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	bool Encode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in, bool boIsPlain = false)
	{
		return m_Encode(_out, in, boIsPlain);
	}

	/**
	 * [処理] 圧縮
	 *	@param in 圧縮対象
	 *	@param boIsPlain 省略すると、非圧縮/圧縮を自動判断します。<BR>
	 *					trueを指定すると、非圧縮クラッドが作成されます。
	 *	@return クラッド(非圧縮/圧縮のいずれか)。Invalid状態ならエラー。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	CByteVector Encode(const IConstCollectionT<BYTE>& in, bool boIsPlain = false)
	{
		CByteVector vb;
		vb.SetIncrementSize(in.GetSize() / 2 + 1);
		if ( ! Encode(vb, in, boIsPlain) )
		{
			vb.Invalid();
		}
		return vb;
	}
	
	/**
	 * [処理] 展開
	 *	@param[out]	_out デコードデータ格納先。追加されますので、通常Emptyのコレクションを指定します。
	 *	@param[in]	in Encode() で作成したクラッド。
	 *	@retval true デコード成功。
	 *	@retval false 失敗。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	bool Decode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in)
	{
		return m_Decode(_out, in);
	}

	/**
	 * [処理] 展開
	 *	@param in Encode() で作成したクラッド。
	 *	@return デコード結果。Invalid状態ならエラー。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	CByteVector Decode(const IConstCollectionT<BYTE>& in)
	{
		CByteVector vb;
		int l = GetSizeAfterDecoding(in);
		if ( l < 0 )
		{
			vb.Invalid();
		}
		else
		{
			vb.SetIncrementSize(l);
			if ( ! Decode(vb, in) )
			{
				vb.Invalid();
			}
		}
		return vb;
	}

	/**
	 * [取得] 展開サイズ
	 *	@param in Encode() で作成したクラッド
	 *	@retval 0以上 展開後のサイズ
	 *	@retval マイナス クラッドではない
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	int GetSizeAfterDecoding(const IConstCollectionT<BYTE>& in)
	{
		m_pSrcDat = &in;
		m_iSrcPos = 0;
		m_iSrcSize = ToInt(m_pSrcDat->GetSize());
		//
		BYTE hd[4] = {0};
		LONG lLen1 = 0;
		LONG lLen2 = 1;
		m_Read(hd, 4);
		if ( memcmp(hd, TNB_SLD_HEAD, 3) != 0 )
		{
			return -1;
		}
		m_Read(&lLen1, 4);
		m_Read(&lLen2, 4);
		if ( lLen1 != lLen2 )
		{
			return -1;
		}
		return lLen1;
	}

private:
	friend class CSlideCompressorTest;
	#undef TNB_SLD_HEAD
	#undef TNB_SLD_NON
	#undef TNB_SLD_LV1
};



};//TNB
