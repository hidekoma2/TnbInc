#pragma once
/**
 *@file
 * レイアウト関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * レイアウトインターフェース.
 *
 *		レイアウトを管理しているインターフェースです。
 *
 *		本インターフェースを実装することで、 CGroupLayout などで管理、処理できます。
 *
 *	@par必要ファイル
 *			TnbLayout.h
 *
 *	@date 08/07/17 新規作成
 */
struct ILayout
{
	/// デストラクタ
	virtual ~ILayout(void) {}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const = 0;

	/**
	 * [取得] サイズ取得.
	 *		現在のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const = 0;
	
	/**
	 * [取得] 最小サイズ取得.
	 *		最小のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetMinimumSize(SIZE& _size) const = 0;

	/**
	 * [設定] サイズ設定.
	 *		現在のサイズを変更することが出来ます。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。
	 */
	virtual bool Resize(const SIZE& size) = 0;
	
	/**
	 * [処理] 決定.
	 *	@note オブジェクトが管理しているサイズと指定のサイズにレイアウトします。
	 *	@param x  X座標。-1なら指定なし。
	 *	@param y  Y座標。-1なら指定なし。
	 *	@param wParam W パラメータ。。
	 *	@param lParam L パラメータ。
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam) = 0;

	/// ポインタハンドル型宣言
	typedef CPointerHandleT<ILayout> Ptr;
};



/**@ingroup LAYOUT
 * レイアウトスペーサー.
 *
 *		レイアウトで隙間を空けたいときにスペーサーとして使用できます。
 *
 *	@par必要ファイル
 *			TnbLayout.h
 *
 *	@date 09/04/15 新規作成
 */
class CLayoutSpacer : public ILayout
{
	DEFSUPER(ILayout);
public:

	/**
	 * コンストラクタ
	 *	@param cx Ⅹサイズ
	 *	@param cy Ｙサイズ
	 */
	CLayoutSpacer(int cx = 0, int cy = 0)
	{
		m_size.cx = cx;
		m_size.cy = cy;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		return new CLayoutSpacer(m_size.cx, m_size.cy);
	}

	/**
	 * [取得] サイズ取得.
	 *		現在のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		_size = m_size;
		return true;
	}
	
	/**
	 * [取得] 最小サイズ取得.
	 *		最小のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		return GetSize(_size);
	}

	/**
	 * [設定] サイズ設定.
	 *		現在のサイズを変更することが出来ます。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。
	 */
	virtual bool Resize(const SIZE& size)
	{
		m_size = size;
		return true;
	}
	
	/**
	 * [処理] 決定.
	 *	@note 何もしません。
	 *	@param x  X座標。-1なら指定なし。
	 *	@param y  Y座標。-1なら指定なし。
	 *	@param wParam W パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 *	@param lParam L パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
	}

private:
	SIZE	m_size;		///< 保持サイズ
};



/**@ingroup LAYOUT
 * レイアウトアイテム抽象クラス.
 *
 *		レイアウトを定義しているクラスの抽象クラスです。
 *
 *	@note CEでは、 SetMargnSize() , SetGapSize() の値は 96DPIの場合での数値を指定します。
 *
 *	@par必要ファイル
 *			TnbLayout.h
 *
 *	@date 08/07/17 新規作成
 *	@date 08/07/29 DPI考慮
 */
class CAbstractLayout : public ILayout
{
	DEFSUPER(ILayout);
public:

	/**
	 * レイアウトアイテム配置方法.
	 *	@note 配置する場所の大きさと、レイアウトアイテムの大きさが異なるケースが発生します。
	 *		その際、どのようにレイアウトアイテムを置くかを定義しています。
	 */
	enum ELocation
	{
		DEFAULT,				///< 横方向は左寄せ、縦方向は中央
		TOP,					///< 上寄せ（縦方向用）
		CENTER,					///< 中央
		BOTTOM,					///< 下寄せ（縦方向用）
		ADJUST,					///< 調整（Resize出来ない場合中央） 
		LEFT = TOP,				///< 左寄せ（横方向用）
		RIGHT = BOTTOM,			///< 右寄せ（横方向用）
/*		EL_Default,				///< 横方向は左寄せ、縦方向は中央
		EL_Top,					///< 上寄せ（縦方向用）
		EL_Center,				///< 中央
		EL_Bottom,				///< 下寄せ（縦方向用）
		EL_Adjust,				///< 調整（Resize出来ない場合中央） 
		EL_Left = EL_Top,		///< 左寄せ（横方向用）
		EL_Right = EL_Bottom,	///< 右寄せ（横方向用）
*/	};

	/// コンストラクタ
	CAbstractLayout(void) : m_isValidLayoutSize(false)
	{
		Zero(m_layoutSize);
		Zero(m_gapSize);
		::SetRectEmpty(&m_margn);
	}

	/**
	 * [設定] 外周マージン設定
	 *	@param rc マージン 
	 */
	void SetMargnSize(const RECT& rc)
	{
		m_margn = rc;
		CalcScale(m_margn);
	}
	
	/**
	 * [設定] 外周マージン設定
	 *	@param mg マージン
	 */
	void SetMargnSize(int mg)
	{
		::SetRect(&m_margn, mg, mg, mg, mg);
		CalcScale(m_margn);
	}
	
	/**
	 * [設定] 各アイテム間のギャップ設定
	 *	@param size ギャップ
	 */
	void SetGapSize(const SIZE& size)
	{
		m_gapSize = size;
		CalcScale(m_gapSize);
	}

	/**
	 * [設定] 各アイテム間のギャップ設定
	 *	@param size ギャップ
	 */
	void SetGapSize(int size)
	{
		m_gapSize.cx = CalcScaleX(size);
		m_gapSize.cy = CalcScaleY(size);
	}

	/**
	 * [取得] サイズ取得.
	 *		現在のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		if ( ! m_isValidLayoutSize )
		{
			if ( ! GetMinimumSize(m_layoutSize) )
			{
				return false;
			}
			m_isValidLayoutSize = true;
		}
		_size = m_layoutSize;
		return true;
	}

	/**
	 * [設定] サイズ設定.
	 *		現在のサイズを変更することが出来ます。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。
	 */
	virtual bool Resize(const SIZE& size)
	{
		m_layoutSize = size;
		m_isValidLayoutSize = true;
		return true;
	}

protected:
	/// 管理パラメータ型
	struct TParam
	{
		ELocation		horizontalLocate;	///< アイテムの水平配置種
		ELocation		verticalLocate;		///< アイテムの垂直配置種
		ILayout::Ptr	pLayout;			///< レイアウトアイテム
		/// コンストラクタ
		TParam(void)
		{}
		/** コンストラクタ
		 *	@param hloc	アイテムの水平配置種
		 *	@param vloc	アイテムの垂直配置種
		 *	@param lay	レイアウトアイテム
		 */
		TParam(ELocation hloc, ELocation vloc, const ILayout& lay)
			: horizontalLocate(hloc), verticalLocate(vloc), pLayout(lay.Clone())
		{}
	};
	mutable bool		m_isValidLayoutSize;		///< レイアウト全体サイズの設定の有効フラグ
	mutable SIZE		m_layoutSize;				///< レイアウト全体サイズ（縦、横）
	RECT				m_margn;					///< 外周のマージン
	SIZE				m_gapSize;					///< 各アイテム間のギャップ（縦、横）

	/**
	 * [設定] 水平方向アイテム決定.
	 *	@param pLayout レイアウト
	 *	@param x X 位置。 Decide()の x を渡します。
	 *	@param xx X 位置オフセット
	 *	@param width 幅
	 *	@param loc 配置
	 *	@param wParam W パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 *	@param lParam L パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 */
	void HorizontalItemDecide(ILayout* pLayout, INT_PTR x, INT_PTR xx, INT_PTR width, ELocation loc, WPARAM wParam, LPARAM lParam)
	{
		SIZE sz;
		if ( x < 0 || pLayout == NULL || ! pLayout->GetSize(sz) )
		{
			return;
		}
		INT_PTR w = width - sz.cx;
		switch ( loc )
		{
		case DEFAULT: // デフォルト
		case LEFT: // 左寄せ
			w = 0;
			break;
		case RIGHT: // 右寄せ
			break;
		case ADJUST: // 調整（Resize出来ない場合中央） 
			sz.cx = ToInt(width);
			if ( pLayout->Resize(sz) )
			{
				w = 0;
				break;
			}
			//以下に続く
		case CENTER: // 中央
			w /= 2;
			break;
		}
		pLayout->Decide(ToInt(x + xx + w), -1, wParam, lParam);
	}

	/**
	 * [設定] 垂直方向アイテム決定.
	 *	@param pLayout レイアウト
	 *	@param y Y 位置。 Decide()の y を渡します。
	 *	@param yy Y 位置オフセット
	 *	@param height 高さ
	 *	@param loc 配置
	 *	@param wParam W パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 *	@param lParam L パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 */
	void VerticalItemDecide(ILayout* pLayout, INT_PTR y, INT_PTR yy, INT_PTR height, ELocation loc, WPARAM wParam, LPARAM lParam)
	{
		SIZE sz;
		if ( y < 0 || pLayout == NULL || ! pLayout->GetSize(sz) )
		{
			return;
		}
		INT_PTR w = height - sz.cy;
		switch ( loc )
		{
		case TOP: // 左寄せ
			w = 0;
			break;
		case BOTTOM: // 右寄せ
			break;
		case ADJUST: // 調整（Resize出来ない場合中央） 
			sz.cy = ToInt(height);
			if ( pLayout->Resize(sz) )
			{
				w = 0;
				break;
			}
			//以下に続く
		case CENTER: // 中央
		case DEFAULT: // デフォルト
			w /= 2;
			break;
		}
		pLayout->Decide(-1, ToInt(y + yy + w), wParam, lParam);
	}

	/**
	 * [取得] マージンサイズ取得.
	 *	@param[out] _size マージンサイズ
	 */
	void GetMargnSize(SIZE& _size) const
	{
		_size.cx = m_margn.left + m_margn.right;	//両端のマージン
		_size.cy = m_margn.top + m_margn.bottom;	//両端のマージン
	}

	/**
	 * [加算] マージンサイズ加算.
	 *	@note マージンを加算
	 *	@param[in,out] _size マージンサイズ
	 */
	void AddMargnSize(SIZE& _size) const
	{
		_size.cx += m_margn.left + m_margn.right;	//両端のマージン
		_size.cy += m_margn.top + m_margn.bottom;	//両端のマージン
	}

	/**
	 * [加算] 隙間サイズ加算.
	 *	@note マージン、ギャップの合計を加算
	 *	@param[in,out] _size 隙間サイズ
	 *	@param[in] cx 横のアイテム数
	 *	@param[in] cy 縦のアイテム数
	 */
	void AddChinkSize(SIZE& _size, size_t cx, size_t cy) const
	{
		AddMargnSize(_size);
		if ( cx > 0 )
		{
			_size.cx += static_cast<LONG>(m_gapSize.cx * (cx - 1)); //アイテム間のサイズ
		}
		if ( cy > 0 )
		{
			_size.cy += static_cast<LONG>(m_gapSize.cy * (cy - 1)); //アイテム間のサイズ
		}
	}
};



}; // TNB
