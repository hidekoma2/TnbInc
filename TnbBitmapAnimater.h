#pragma once
/**
 *@file
 * ビットマップアニメ関係のヘッダ
 *
 *		切り替える方式(ワイプ)を定義する IBitmapWipable インターフェース、
 *		それを実装した、 CPenetrateWiper , CStretchWiper クラス、
 *		アダプタ的な機能をする CReverseAdaptWiper, CMultiAdaptWiper クラス、
 *		そしてそれらを使いアニメを作成、管理する CBitmapAnimater クラスがあります。
 *
 *		TnbBitmapWipers.h にも他のワイパーがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * ビットマップワイパブルインターフェース
 *
 *		二つのビットマップを徐々に切り替えていく処理をサポートします。
 *	
 *	@see CBitmapAnimater 
 *
 *	@par必要ファイル
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 新規作成
 */
struct IBitmapWipable
{
	/// デストラクタ
	virtual ~IBitmapWipable(void) { }

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const = 0;

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵。
	 *	@param overBmp 先絵。　※元絵と同じ大きさ以外は結果保障外になります。
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const = 0;

	/// ポインタハンドル型宣言
	typedef CPointerHandleT<IBitmapWipable> Ptr;
};



/**@ingroup ANIMATE
 * リバースアダプトワイパークラス
 *
 *		ワイプ処理を反転します。
 *		
 *	@par必要ファイル
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 新規作成
 */
class CReverseAdaptWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
protected:
	IBitmapWipable::Ptr m_pWiper;		///< 使用するワイパー
public:

	/// コンストラクタ
	CReverseAdaptWiper(const IBitmapWipable& w) : m_pWiper(w.Clone())
	{
	}

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const
	{
		if ( m_pWiper.IsNull() ) return NULL;
		return new CReverseAdaptWiper(*m_pWiper);
	}

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		return m_pWiper->Wipe(overBmp, baseBmp, max - par, max);
	}
};



/**@ingroup ANIMATE
 * マルチアダプトワイパークラス
 *
 *		ワイプ処理を複数登録できます。
 *		
 *	@note SetWiper() したワイパーを使い、元絵と先絵をワイプします。その結果を、
 *		次に AddWiper() したワイパーを使い、元絵にワイプします。その繰り返しをします。
 *	@par使用例
 *		以下は、一つ目に、大きさを変えるワイパーを登録し、2つ目に透過するワイパーを登録しています。
 *		<table><tr><td><pre>
 *		
 *		CMultiAdaptWiper w;
 *		w.SetWiper(CStretchWiper(2));	///< ストレッチワイパー。
 *		w.AddWiper(CPenetrateWiper());	///< 透過ワイパー。
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 新規作成
 *	@date 09/04/11 CSimpleVectorT の仕様変更に対応。
 */
class CMultiAdaptWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
protected:
	typedef CSimpleVectorT<IBitmapWipable::Ptr> CWipablesVecter;///< ワイパーポインタ配列型
	CWipablesVecter		m_wipers;		///< 使用するワイパー
	/// 全情報けし
	void m_AllRemove(void)
	{
		m_wipers.RemoveAll();
	}
public:

	/// コンストラクタ
	CMultiAdaptWiper(void)
	{
	}

	/// デストラクタ
	~CMultiAdaptWiper(void)
	{
		m_AllRemove();
	}

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const
	{
		CMultiAdaptWiper* P = new CMultiAdaptWiper();
		loop ( i, m_wipers.GetSize() )
		{
			P->AddWiper(*m_wipers[i]);
		}
		return P;
	}

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		loop ( i, m_wipers.GetSize() )
		{
			overBmp = m_wipers[i]->Wipe(baseBmp, overBmp, par, max);
		}
		return overBmp;
	}

	/**
	 * [設定] ワイパー設定.
	 *	@note 先に設定してあったワイパーはクリアし、指定のワイパーを設定します。
	 *	@param w ワイパー。本オブジェクトには複製が保持されるので、 w は破棄してかまいません。
	 */
	void SetWiper(const IBitmapWipable& w)
	{
		m_AllRemove();
		AddWiper(w);
	}

	/**
	 * [設定] ワイパー設定(追加).
	 *	@note 一つのオブジェクトに複数指定が出来ます。追加順に処理されます。
	 *	@param w ワイパー。本オブジェクトには複製が保持されるので、 w は破棄してかまいません。
	 */
	void AddWiper(const IBitmapWipable& w)
	{
		m_wipers.Add(w.Clone());
	}
};



/**@ingroup ANIMATE
 * 透過ワイプ処理クラス
 *
 *	二つの画像を、透過して切り替えます。
 *
 *	@par必要ファイル
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 新規作成
 */
class CPenetrateWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
public:
	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const { return new CPenetrateWiper(); }

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.元絵と同じ大きさ以外は結果保障外になります。
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		bi.InsertOnAlphaBlend(0, 0, overBmp, 100 * par / max);
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * 伸張ワイプ処理クラス
 *
 *	先の画像を、指定の場所から、大きくしていきます。
 *
 *	@par必要ファイル
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 新規作成
 *	@date 08/12/03 幅、高さ固定フラグ、付加
 */
class CStretchWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
	BYTE	m_pos;	///< ポジション
public:

	/// フラグ
	enum
	{
		HEIGHT_FIX = 0x10,	///< 高さ固定
		WIDTH_FIX = 0x20	///< 幅固定
	};
	
	/**
	 * コンストラクタ
	 *	@param uPos どこを中心に広がり始めるか。 １～９。５が中央。８が上中。 更に HEIGHT_FIX , WIDTH_FIX も or で指定できます。
	 */
	CStretchWiper(BYTE uPos = 5) : m_pos(uPos)
	{
	}

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const { return new CStretchWiper(m_pos); }

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.元絵と同じ大きさ以外は結果保障外になります。
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		const SIZE& size = bi.GetSize();
		int cx = size.cx;
		if ( (m_pos & WIDTH_FIX) == 0 )
		{
			cx = cx * par / max;
		}
		int cy = size.cy;
		if ( (m_pos & HEIGHT_FIX) == 0 )
		{
			cy = cy * par / max;
		}
		if ( cx > 0 && cy > 0 )
		{
			int po = m_pos & 0x0F;
			int x = size.cx - cx;
			int y = size.cy - cy;
			if ( po >= 7 ) 
			{
				y = 0; 
			}
			else if ( po >= 4 ) 
			{
				y /= 2;
			}
			switch ( po % 3 )
			{
			case 1: x = 0; break;
			case 2: x /= 2; break;
			}
			bi.Insert(x, y, overBmp, SRCCOPY, cx, cy);
		}
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * ビットマップアニメクラス
 *
 *		IBitmapWipable を実装したワイパーを使い、登録されたビットマップを
 *		加工します。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CBitmapAnimater anime;
 *		anime.SetFirstFrame("1.bmp", CPenetrateWiper());
 *		anime.AddBaseFrame("1.bmp", 5);
 *		anime.AddBaseFrame("2.bmp", 5);
 *		anime.AddBaseFrame("3.bmp", 5);
 *		anime.AddBaseFrame("4.bmp", 5);
 *		ASSERT( anime.GetLength() == 21 ); 
 *		anime.FrameAt(6).Save("1.5.bmp");
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbBitmapAnimater.h
 *
 *	@date 07/10/03 新規作成
 *	@date 09/04/11 CSimpleVectorT の仕様変更に対応。
 */
class CBitmapAnimater //: CCopyImmposible
{
	struct TBaseFrame
	{
		CBitmapHandle		bmp;		///< 変化後の画像
		UINT				count;		///< 変化するコマ数 1 なら補完なし。
		IBitmapWipable::Ptr	pWiper;		///< 使用するワイパー
	};
	CSimpleVectorT<TBaseFrame>		m_frames;	///< ベースフレーム情報
	size_t							m_length;	///< 全フレーム数
	CSimpleVectorT<CBitmapHandle>	m_cache;	///< キャッシュ
	// 全けし
	void m_RemoveAll(void)
	{
		m_frames.RemoveAll();
		m_length = 0;
		m_cache.RemoveAll();
	}
	//
	bool m_AddBaseFrame(CBitmapHandle bmp, UINT count, const IBitmapWipable* pWiper)
	{
		if ( ! bmp.IsNull() )
		{
			if ( count == 0 ) 
			{
				count = 1;
			}
			TBaseFrame b;
			b.bmp = bmp;
			b.count = count;
			if ( pWiper != NULL )
			{
				b.pWiper = pWiper->Clone();
			}
			m_frames.Add(b);
			m_length += b.count;
			return true;
		}
		return false;
	}
public:

	/// コンストラクタ
	CBitmapAnimater(void) : m_length(0)
	{
	}

	/**
	 * [設定] 最初のフレーム画像設定
	 *	@note これを実行すると、今までの情報はクリアされます。
	 *	@param bmp ビットマップ
	 *	@param defaultWiper AddBaseFrame() でワイパーが省略された時に使用されるワイパー。
	 *				複製を記憶しますので、破棄してかまいません。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetFirstFrame(CBitmapHandle bmp, const IBitmapWipable& defaultWiper)
	{
		m_RemoveAll();
		return m_AddBaseFrame(bmp, 1, &defaultWiper);
	}

	/**
	 * [設定] 最初のフレーム画像設定
	 *	@note これを実行すると、今までの情報はクリアされます。
	 *	@param bmp ビットマップ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetFirstFrame(CBitmapHandle bmp)
	{
		m_RemoveAll();
		CPenetrateWiper w;
		return m_AddBaseFrame(bmp, 1, &w);
	}

	/**
	 * [設定] 次のベースフレーム画像設定
	 *	@param bmp ビットマップ
	 *	@param count 何フレーム追加するか。 1 ならワイパーは使わず、切り替わります。
	 *	@param wiper 一つ前のベース画像から本画像の切り替えで使うワイパー。
	 *				複製を記憶しますので、破棄してかまいません。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool AddBaseFrame(CBitmapHandle bmp, UINT count, const IBitmapWipable& wiper)
	{
		if ( m_length > 0 )
		{
			return m_AddBaseFrame(bmp, count, &wiper);
		}
		return false;
	}

	/**
	 * [設定] 次のベースフレーム画像設定
	 *	@note ワイパーは SetFirstFrame() 時に指定したものを使います。
	 *	@param bmp ビットマップ
	 *	@param count 何フレーム追加するか。 1 ならワイパーは使わず、切り替わります。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool AddBaseFrame(CBitmapHandle bmp, UINT count = 1)
	{
		if ( m_length > 0 )
		{
			return m_AddBaseFrame(bmp, count, NULL);
		}
		return false;
	}

	/**
	 * [取得] フレーム数取得
	 *	@return フレーム数。
	 */
	size_t GetLength(void) const
	{
		return m_length;
	}

	/**
	 * [取得] フレーム取得
	 *	@note 取得時に画像を合成します。
	 *	@param index フレームNO。０以上、 GetLength() の値未満です。
	 *	@return フレーム画像。
	 */
	CBitmapHandle FrameAt(INDEX index) const
	{
		if ( index >= m_length ) { return CBitmapHandle(); }
		const TBaseFrame* P = NULL;
		loop ( i, m_frames.GetSize() )
		{
			P = &m_frames[i];
			if ( index < P->count )
			{
				break;
			}
			index -= P->count;
		}
		ASSERTLIB( P != NULL );
		if ( P->count == 1 )
		{
			return P->bmp;
		}
		const IBitmapWipable* pWiper = P->pWiper;
		if ( pWiper == NULL )
		{
			pWiper = m_frames[0].pWiper;
		}
		if ( pWiper == NULL || index + 1 == P->count )
		{
			return P->bmp;
		}
		return pWiper->Wipe((P - 1)->bmp, P->bmp, down_cast<UINT>(index + 1), P->count);
	}

	/**
	 * [取得] フレーム取得
	 *	@note 初めての取得時に画像を合成し、キャッシュします。
	 *	@param index フレームNO。０以上、 GetLength() の値未満です。
	 *	@return フレーム画像。
	 */
	CBitmapHandle FrameAtWithCache(INDEX index)
	{
		CBitmapHandle b;
		if ( index < m_length )
		{
			if ( m_cache.GetSize() != m_length )
			{
				m_cache.SetSize(m_length);
			}
			b = m_cache.At(index);
			if ( b.IsNull() )
			{
				b = FrameAt(index);
				m_cache.Set(index, b);
			}
		}
		return b;
	}

	/// [処理] キャッシュ消去
	void FlashCache(void)
	{
		m_cache.RemoveAll();
	}
};



}; //TNB
