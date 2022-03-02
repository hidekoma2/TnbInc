#pragma once
/**
 *@file
 * 描画コンテナ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSimpleVector.h"
#include "TnbBitmapDrawer.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE BITMAP
 * 描画コンテナクラス
 *
 *		{@link IDrawable 描画情報}と座標を対に複数保持するクラスです。
 *
 *		INDEX で{@link IDrawable 描画情報}を管理することが可能です。
 *
 *		管理している{@link IDrawable 描画情報}を、まとめて、デバイスコンテキストに描画することが可能です。
 *
 *	@note	本クラスに描画情報を追加する際、相対座標も同時に記憶しているため、
 *			Draw() で描画時にそれぞれの相対座標に描画することができます。
 *
 *	@note	相対座標は、インスタンス生成時は左上（0,0）になっています。
 *			AddCloneObject() や AddNewPointer() , AddBitmap() , AppendBitmaps() , MoveAppendPosition() , 
 *			SetAppendPosition() , OffsetAppendPosition() で任意に移動します。
 *
 *	@note	指定の インデックスに対する位置に対する相対座標でも、追加可能です。( ELocation )
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		//= 宣言
 *		CDrawingContainer m_bmpc; // コンテナ作成
 *
 *		//= 設定
 *		m_bmpc.AddBitmap("c:\\BMP\\BackTop1.bmp", bc.RIGHT);  //BackTop1.bmp登録後、Offsetを真右に。
 *		m_bmpc.AddBitmap("c:\\BMP\\BackTop2.bmp", bc.RETURN);  //BackTop2.bmp登録後、Offsetを下の左へ。		
 *		m_bmpc.AddBitmap("c:\\BMP\\Back.bmp", bc.DOWN);  //Back.bmp登録後、Offsetを真下へ。
 *		m_bmpc.AddBitmap("c:\\BMP\\BackBottom.bmp");  //BackBottom.bmp登録。
 *
 *		//= 表示 (デバイスコンテキストに表示 )
 *		void CXXX::OnPaint() { 
 *			CPaintDC dc(this);
 *			m_bmpc.Draw(dc);
 *		}
 *		</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbDrawingContainer.h
 *
 *	@date 07/10/03 新規作成
 *	@date 07/11/02 抽象化するために、 IDrawable のコンテナに変更。
 *	@date 07/11/17 BitmapConainer と一本化。
 *	@date 07/11/20 ELocation 追加。 CNullDrawer 引越し。
 *	@date 07/12/12 GetAllRgn() 追加。
 *	@date 07/12/19 Resize() に対応。
 */
class CDrawingContainer : public IDrawable
{
	DEFSUPER(IDrawable);
public:

	/// コンストラクタ
	CDrawingContainer(void)
	{
		m_appendPos.x = 0;
		m_appendPos.y = 0;
	}

	/// デストラクタ
	virtual ~CDrawingContainer(void)
	{
		m_drawers.RemoveAll();
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CDrawingContainer* pNew = new CDrawingContainer();
		pNew->m_appendPos = m_appendPos;
		pNew->m_drawers.RemoveAll();
		size_t l = m_drawers.GetSize();
		pNew->m_drawers.SetSize(l);
		loop ( i, l )
		{
			pNew->m_drawers[i] = m_drawers[i];
		}
		return pNew;
	}

	/**
	 * [取得] サイズ取得.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを得ることが出来ます。
	 *	@note 一番左上に描画する描画情報がなくても、左上から右下のサイズになります。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		RECT rc;
		if ( GetAllRect(rc) )
		{
			_size.cx = rc.right;
			_size.cy = rc.bottom;
			return true;
		}
		return false;
	}

	/**
	 * [設定] サイズ設定.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを設定することが出来ます。
	 *	@note 管理している描画情報のいずれかがサイズ設定に対応していなくても、
	 *			本クラスの本メソッドは成功します。サイズ設定できるもののみ、指定サイズ内に入るようにします。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。(本クラスでは失敗はありません)
	 */
	virtual bool Resize(const SIZE& size)
	{
		loop ( i, m_drawers.GetSize() )
		{
			TParam& A = m_drawers[i];
			SIZE sz = A.orgSize;
			if ( sz.cx + A.pos.x > size.cx )
			{
				sz.cx = size.cx - A.pos.x;
				if ( sz.cx < 0 )
				{
					sz.cx = 0;
				}
			}
			if ( sz.cy + A.pos.y > size.cy )
			{
				sz.cy = size.cy - A.pos.y;
				if ( sz.cy < 0 )
				{
					sz.cy = 0;
				}
			}
			A.pDrawer->Resize(sz);
		}
		return true;
	}

	/**
	 * [設定] サイズリセット.
	 *		Resize() の設定を無効にし、登録時のサイズに戻します。
	 */
	void DefaultSize(void)
	{
		loop ( i, m_drawers.GetSize() )
		{
			TParam& A = m_drawers[i];
			A.pDrawer->Resize(A.orgSize);
		}
	}

	/**
	 * [作成] ビットマップ作成.
	 *		本オブジェクトで管理している描画情報から、ビットマップを作成し返します。
	 *	@param color 背景色を指定します。省略すると不定（塗りつぶししない) になります。
	 *				全エリア描画する描画情報の場合、背景色を指定する必要はありません。
	 *	@return ビットマップ。一つも情報がない場合、 NULL ビットマップが返ります。
	 */
	CBitmapHandle CreateBitmap(COLORREF color = CLR_INVALID) const
	{
		return CBitmapDrawer::ToBitmap(*this, color);
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		DrawEx(dc, x, y, 0);
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 *	@param lParam LPARAM。管理している描画情報に渡します。
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		if ( dc == NULL )
		{
			loop ( i, m_drawers.GetSize() )
			{
				const TParam& A = m_drawers.At(i);
				A.pDrawer->DrawEx(dc, x + A.pos.x, y + A.pos.y, lParam);
			}
		}
		else
		{
			loop ( i, m_drawers.GetSize() )
			{
				const TParam& A = m_drawers.At(i);
				SIZE size;
				if ( A.pDrawer->GetSize(size) )
				{
					RECT rc = { x + A.pos.x, y + A.pos.y, x + A.pos.x + size.cx, y + A.pos.y + size.cy };
					if ( ::RectVisible(dc, &rc) )
					{
						A.pDrawer->DrawEx(dc, rc.left, rc.top, lParam);
					}
				}
			}
		}
	}

	/**
	 * [取得] 格納数取得.
	 *		格納しているビットマップ数を返します。
	 *	@return 数。
	 */
	size_t GetCount(void) const
	{
		return m_drawers.GetSize();
	}
	
	/**
	 * [参照] 指定INDEXの参照.
	 *	@param index インデックス.
	 *	@retval NULL エラー。インデックス指定が異常。
	 *	@retval 描画情報。
	 */
	const IDrawable* At(INDEX index) const
	{
		if ( m_drawers.IsInRange(index) )
		{
			return m_drawers.At(index).pDrawer;
		}
		return NULL;
	}

	/**
	 * [削除] 指定INDEXの削除.
	 *	@param index インデックス.
	 *	@retval true 成功。
	 *	@retval false 失敗。インデックス指定が異常。
	 */
	bool RemoveAt(INDEX index)
	{
		return m_drawers.Remove(index);
	}

	/**
	 * [削除] すべて削除.
	 *		格納している描画情報をすべて破棄し、追加相対座標を左上（0,0） にします。
	 */
	void RemoveAll(void)
	{
		m_drawers.RemoveAll();
		m_appendPos.x = 0;
		m_appendPos.y = 0;
	}

	/**
	 * [取得] ビットマップ取得.
	 *		指定INDEXに登録されている描画情報のビットマップを取得することが出来ます。
	 *	@note ビットマップハンドルは、一時的なものの可能性もあります。
	 *	@param index インデックス。
	 *	@return ビットマップ。もっていない場合やインデックスが異常の場合、 NULL ビットマップが返ります。
	 */
//	CBitmapHandle GetAtBitmap(INDEX index) const
//	{
//		if ( m_drawers.IsInRange(index) )
//		{
//			const TParam& A = m_drawers.At(index);
//			return A.pDrawer->GetBitmap();
//		}
//		return CBitmapHandle();
//	}

	/**
	 * [取得] 矩形範囲取得.
	 *		指定INDEXに登録されている描画情報の範囲がわかります。
	 *	@param[in] index インデックス。
	 *	@param[out] _rect 矩形範囲。
	 *	@retval true 成功。
	 *	@retval false 失敗。インデックス指定が異常。
	 */
	bool GetAtRect(INDEX index, RECT& _rect) const
	{
		if ( ! m_drawers.IsInRange(index) ) { return false; }
		const TParam& A = m_drawers.At(index);
		SIZE size;
		bool r = A.pDrawer->GetSize(size);
		if ( r )
		{
			::SetRect(&_rect, A.pos.x, A.pos.y, A.pos.x + size.cx, A.pos.y + size.cy);
		}
		else
		{
			::SetRectEmpty(&_rect);
		}
		return r;
	}

	/**
	 * [取得] 矩形範囲取得.
	 *		登録されている描画情報の描画範囲がわかります。
	 *	@param[out] _rect 矩形範囲。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GetAllRect(RECT& _rect) const
	{
		bool r = true;
		::SetRectEmpty(&_rect);
		loop ( i, m_drawers.GetSize() )
		{
			RECT rc;
			r &= GetAtRect(i, rc);
			::UnionRect(&_rect, &_rect, &rc);
		}
		return r;
	}

	/**
	 * [取得] 領域範囲取得.
	 *		登録されている描画情報の描画領域がわかります。
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 リージョン。使用後は、破棄する必要があります。
	 */
	HRGN GetAllRgn(void) const
	{
		if ( GetCount() == 0 ) { return NULL; }
		HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
		loop ( i, GetCount() )
		{
			RECT r;
			if ( GetAtRect(i, r) )
			{
				HRGN rgnTemp = ::CreateRectRgnIndirect(&r);
				::CombineRgn(rgn, rgn, rgnTemp, RGN_OR);
				_DeleteObject(rgnTemp);
			}
		}
		return rgn;
	}

	/**
	 * [描画] 描画.
	 *		指定INDEXに登録されている描画情報を指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param[in] index インデックス。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	void DrawAt(INDEX index, HDC dc, int x = 0, int y = 0) const
	{
		if ( m_drawers.IsInRange(index) )
		{
			const TParam& A = m_drawers.At(index);
			A.pDrawer->Draw(dc, x, y);
		}
	}

	/**
	 * [取得] 追加相対座標取得.
	 *		次に追加する描画情報の相対座標になる情報が得られます。
	 *	@return 追加相対座標
	 */
	const POINT& GetAppendPosition(void) const
	{
		return m_appendPos;
	}

	/**
	 * [操作] 追加相対座標移動.
	 *		次に追加する描画情報の相対座標をずらすことが可能です。
	 *	@param x Ｘ方向のOFFSET値。
	 *	@param y Ｙ方向のOFFSET値。
	 */
	void OffsetAppendPosition(int x, int y)
	{
		m_appendPos.x += x;
		m_appendPos.y += y;
	}

	/**
	 * [操作] 追加相対座標設定.
	 *		次に追加する描画情報の相対座標を設定することが可能です。
	 *	@param x Ｘ方向の位置。
	 *	@param y Ｙ方向の位置。
	 */
	void SetAppendPosition(int x, int y)
	{
		m_appendPos.x = x;
		m_appendPos.y = y;
	}

	/// 方向値
	enum EDirection
	{	
		DOWN,	///< 下へ移動
		RIGHT,	///< 右へ移動
		RETURN,	///< 下へ移動して一番左へ
		VRETURN,///< 右へ移動して一番上へ
		STAY	///< 移動なし
	};

	/**
	 * [移動] 追加相対座標設定.
	 *		次に追加する描画情報の相対座標を設定することが可能です。
	 *	@note	指定インデックスに登録されている描画情報と相対座標から、
	 *			指定の方向に移動した座標に、OFFSETを加えた座標を追加相対座標にします。
	 *	@param index インデックス.
	 *	@param dire 移動方向.
	 *	@param x Ｘ方向のOFFSET値。
	 *	@param y Ｙ方向のOFFSET値。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool MoveAppendPosition(INDEX index, EDirection dire = DOWN, int x = 0, int y = 0)
	{
		if ( m_drawers.IsInRange(index) )
		{
			const TParam& A = m_drawers.At(index);
			SIZE size;
			if ( A.pDrawer->GetSize(size) )
			{
				m_appendPos.x = A.pos.x + x;
				m_appendPos.y = A.pos.y + y;
				m_Offset(dire, size.cx, size.cy);
				return true;
			}
		}
		return false;
	}

	/// 位置指定値
	enum ELocation
	{
		RIGHTTOP,		///< 上あわせで、右に追加。
		RIGHTCENTER,	///< 中央あわせで、右に追加。
		RIGHTBOTTOM,	///< 下あわせで、右に追加。
		//
		DOWNLEFT,		///< 左あわせで、下に追加。
		DOWNCENTER,		///< 中央あわせで、下に追加。
		DOWNRIGHT,		///< 右あわせで、下に追加。
		//
		CENTER,			///< 中央に追加。
	};

	/**
	 * [追加] 描画情報追加.
	 *		追加相対座標に指定の描画情報を関連づけて記憶します。
	 *	@note 追加した順にインデックス 0, 1, 2, ･･･ と割り振られます。
	 *	@param pDraw 描画情報. 必ず、 new で作成したインスタンスを渡してください。
	 *				破棄は本オブジェクトが行います（本メソッドが失敗した時は即破棄します）。
	 *	@param dire 移動方向.
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval INVALID_INDEX以外 成功。値は、追加したインデックスです。
	 */
	INDEX AddNewPointer(IDrawable* pDraw, EDirection dire = DOWN)
	{
		TParam p;
		if ( pDraw != NULL && pDraw->GetSize(p.orgSize) )
		{
			p.pos = m_appendPos;
			p.pDrawer = pDraw;
			INDEX r = m_drawers.Add(p);
			if ( r != INVALID_INDEX )
			{
				m_Offset(dire, p.orgSize.cx, p.orgSize.cy);
				return r;
			}
		}
		delete pDraw;
		return INVALID_INDEX;
	};

	/**
	 * [追加] 描画情報追加.
	 *		指定の座標に指定の描画情報を関連づけて記憶します。
	 *	@note	指定インデックスに登録されている描画情報と相対座標から、
	 *			指定の位置に画像情報を追加します。
	 *	@note 追加した順にインデックス 0, 1, 2, ･･･ と割り振られます。
	 *	@param index インデックス.
	 *	@param locate 位置
	 *	@param pDraw 描画情報. 必ず、 new で作成したインスタンスを渡してください。
	 *				破棄は本オブジェクトが行います（本メソッドが失敗した時は即破棄します）。
	 *	@param dire 移動方向.
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval INVALID_INDEX以外 成功。値は、追加したインデックスです。
	 */
	INDEX AddNewPointer(INDEX index, ELocation locate, IDrawable* pDraw, EDirection dire = DOWN)
	{
		if ( ! m_Locate(index, locate, pDraw) ) { return INVALID_INDEX; }
		return AddNewPointer(pDraw, dire);
	}
	
	/**
	 * [追加] 描画情報追加.
	 *		追加相対座標に指定の描画情報を関連づけて記憶します。
	 *	@note 追加した順にインデックス 0, 1, 2, ･･･ と割り振られます。
	 *	@param draw 描画情報. 本オブジェクトにはクローンが記憶されるので、 draw は破棄してもかまいません。
	 *	@param dire 移動方向.
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval INVALID_INDEX以外 成功。値は、追加したインデックスです。
	 */
	INDEX Add(const IDrawable& draw, EDirection dire = DOWN)
	{
		return AddNewPointer(draw.Clone(), dire);
	};

	/**
	 * [追加] 描画情報追加.
	 *		指定の座標に指定の描画情報を関連づけて記憶します。
	 *	@note	指定インデックスに登録されている描画情報と相対座標から、
	 *			指定の位置に画像情報を追加します。
	 *	@note 追加した順にインデックス 0, 1, 2, ･･･ と割り振られます。
	 *	@param index インデックス.
	 *	@param locate 位置
	 *	@param draw 描画情報. 本オブジェクトにはクローンが記憶されるので、 draw は破棄してもかまいません。
	 *	@param dire 移動方向.
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval INVALID_INDEX以外 成功。値は、追加したインデックスです。
	 */
	INDEX Add(INDEX index, ELocation locate, const IDrawable& draw, EDirection dire = DOWN)
	{
		return AddNewPointer(index, locate, draw.Clone(), dire);
	};

	/**
	 * [追加] ビットマップ描画情報追加.
	 *		追加相対座標に指定の描画情報を関連づけて記憶します。
	 *	@note 追加した順にインデックス 0, 1, 2, ･･･ と割り振られます。
	 *	@param bmp ビットマップ。
	 *	@param dire 移動方向.
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval INVALID_INDEX以外 成功。値は、追加したインデックスです。
	 */
	INDEX AddBitmap(CBitmapHandle bmp, EDirection dire = DOWN)
	{
		return AddNewPointer(new CBitmapDrawer(bmp), dire);
	}

	/**
	 * [追加] ビットマップ描画情報追加.
	 *		指定座標に指定の描画情報を関連づけて記憶します。
	 *	@note 追加した順にインデックス 0, 1, 2, ･･･ と割り振られます。
	 *	@note	指定インデックスに登録されている描画情報と相対座標から、
	 *			指定の位置に画像情報を追加します。
	 *	@param index インデックス.
	 *	@param locate 位置
	 *	@param bmp ビットマップ。
	 *	@param dire 移動方向.
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval INVALID_INDEX以外 成功。値は、追加したインデックスです。
	 */
	INDEX AddBitmap(INDEX index, ELocation locate, CBitmapHandle bmp, EDirection dire = DOWN)
	{
		return AddNewPointer(index, locate, new CBitmapDrawer(bmp), dire);
	}

	/**
	 * [追加] 一括ビットマップファイル読込み.
	 *		指定ファイル名に関するビットマップファイルを読み込みます。
	 *	@note 最初に RemoveAll() が実行されます。
	 *
	 *	@par使用例
	 *		Back001a.bmp , Back002.bmp , Back004.bmp , Back006_DC.bmp , Back010.bmp のファイルがある場合、
	 *		以下のコードを実行すると、 INDEX 1 に Back001a.bmp 、INDEX 2 に Back002.bmp 、
	 *		INDEX 4 に Back004.bmp 、INDEX 6 に Back006_DC.bmp 、INDEX 10 に Back010.bmp が割り当てられます。
	 *		これらのビットマップハンドルは、 GetAtBitmap() で容易に取り出すことが出来ます。また、 DrawAt() 
	 *		で容易にデバイスコンテキストに描画も出来ます。
	 *		<table><tr><td><pre>
	 *
	 *		CBitmapContainer m_bmpc; // コンテナ作成
	 *		m_bmpc.AppendBitmaps("c:\\BMP\\Back%03d*.bmp", 10, bc.DOWN);		
	 *		</pre></td></tr></table>
	 *		※上記例で Back001a.bmp , Back001b.bmp がある場合、どちらが読み込まれるかは不定になります
	 *		（先に検索で見つかったほうが使用されます）。
	 *
	 *	@param lpszPath パス名。ファイル名の数字になるところに、 ％d を埋めておきます。
	 *	@param maxIndex インデックスの最大値。 5 であれば、 0～5 の6個検索します。
	 *	@param dire 移動方向.
	 *	@retval マイナス	エラー。
	 *	@retval 0			一つもなかった
	 *	@retval 1以上		読み込んだ数。INDEXの最大値は GetCount() で確認してください。
	 */
	int AppendBitmaps(LPCTSTR lpszPath, INDEX maxIndex, EDirection dire = DOWN)
	{
		RemoveAll();
		WIN32_FIND_DATA wfd;
		SIZE nullSize = { 0, 0 };
		CWorkMemT<TCHAR> str(MAX_PATH + 1);
		INDEX lastValidIndex = INVALID_INDEX;
		int foundCount = 0;
		loop( i, maxIndex + 1 )
		{
			if ( ! STRLIB::PrintF(str, MAX_PATH, lpszPath, i) )
			{
				return -1;
			}
			IDrawable* pDraw = NULL; 
			HANDLE hFind = ::FindFirstFile(str, &wfd);
			if ( hFind != INVALID_HANDLE_VALUE )
			{
				::FindClose(hFind);
				CWorkMemT<TCHAR> s(MAX_PATH + 1);
				STRLIB::Copy(s, str);
				INT_PTR l = STRLIB::PathIndexOf(s) + 1;
				STRLIB::Copy(&s[l], wfd.cFileName);
				CBitmapHandle bmp(s);
				if ( ! bmp.IsNull() )
				{
					pDraw = new CBitmapDrawer(bmp);
					lastValidIndex = i;
					foundCount++;
				}
			}
			if ( pDraw == NULL )
			{
				pDraw = new CNullDrawer(nullSize);
			}
			INDEX idx = AddNewPointer(pDraw, dire);
			ASSERTLIB( idx == i );
		}
		if ( lastValidIndex != INVALID_INDEX )
		{
			while ( RemoveAt(lastValidIndex + 1 ));
		}
		else
		{
			RemoveAll();
		}
		return foundCount;
	}

private:

	/// 管理パラメータ
	struct TParam
	{
		POINT						pos;		///< 相対座標
		SIZE						orgSize;	///< オリジナルサイズ
		IDrawable::Ptr				pDrawer;	///< 描画情報
	};
	CSimpleVectorT<TParam>	m_drawers;			///< 描画情報群
	POINT					m_appendPos;		///< 追加相対座標
	// OFFSET
	void m_Offset(EDirection dire, int x, int y)
	{
		if ( dire == DOWN ) 
		{
			m_appendPos.y += y;
		}
		else if ( dire == RIGHT ) 
		{
			m_appendPos.x += x;
		}
		else if ( dire == RETURN )
		{ 
			m_appendPos.x = 0;
			m_appendPos.y += y;
		}
		else if ( dire == VRETURN )
		{ 
			m_appendPos.x += x;
			m_appendPos.y = 0;
		}
	}
	/** LOCATE
	 *	@param index インデックス.
	 *	@param locate 位置
	 *	@param pDraw 描画情報
	 */
	bool m_Locate(INDEX index, ELocation locate, const IDrawable* pDraw)
	{
		if ( ! m_drawers.IsInRange(index) ) { return false; }
		const TParam& A = m_drawers.At(index);
		SIZE baseSize;
		if ( ! A.pDrawer->GetSize(baseSize) ) { return false; }
		SIZE mySize;
		if ( ! pDraw->GetSize(mySize) ) { return false; }
		m_appendPos.x = A.pos.x;
		m_appendPos.y = A.pos.y;
		// Y調整
		if ( locate == RIGHTCENTER || locate == CENTER )
		{
			m_appendPos.y += (baseSize.cy - mySize.cy) / 2;
		}
		else if( locate == RIGHTBOTTOM )
		{
			m_appendPos.y += baseSize.cy - mySize.cy;
		}
		else if ( locate != RIGHTTOP )//下に追加
		{
			m_appendPos.y += baseSize.cy;
		}
		// X調整
		if ( locate == DOWNCENTER || locate == CENTER )
		{
			m_appendPos.x += (baseSize.cx - mySize.cx) / 2;
		}
		else if( locate == DOWNRIGHT )
		{
			m_appendPos.x += baseSize.cx - mySize.cx;
		}
		else if ( locate != DOWNLEFT )//右に追加
		{
			m_appendPos.x += baseSize.cx;
		}
		return true;
	}
};



};///TNB

