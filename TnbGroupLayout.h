#pragma once
/**
 *@file
 * レイアウト関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSimpleVector.h"
#include "TnbLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * グループレイアウトアイテムクラス.
 *
 *		レイアウトインターフェースを持つオブジェクトを複数管理するクラスです。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		// ダイアログクラス内		
 *
 *		CStatic	m_txtFirst;	
 *		CStatic	m_txtSecond;
 *		CEdit	m_editFirst;
 *		CEdit	m_editSecond;
 *		
 *		BOOL CFoo::OnInitDialog()
 *		{
 *				;
 *		
 *			CGroupLayout layout;		
 *
 *			// ウィンドウ枠から 2 dot 隙間を指定
 *			layout.SetMargnSize(CSize(2, 2));	
 *			// 各アイテムの間は、横 5, 縦 10 空くように指定
 *			layout.SetGapSize(CSize(5, 10));
 *			// 水平方向のレイアウト
 *			layout.SetHorizontalGroup			
 *			(
 *				AddGroup(
 *				    Add(Li(&m_txtFirst))  //TEXTはりソースの大きさのまま		
 *				    .Add(Li(&m_txtSecond))
 *				)
 *				.AddGroup(
 *				    Add(Li(&m_editFirst), CGroupLayout::ADJUST)  //EDITは枠一杯に広げる 
 *				    .Add(Li(&m_editSecond))
 *				)
 *			);
 *			// 垂直方向のレイアウト
 *			layout.SetVerticalGroup
 *			(
 *				AddGroup(
 *				    Add(Li(&m_txtFirst))
 *				    .Add(Li(&m_editFirst), CGroupLayout::ADJUST)
 *				)
 *				.AddGroup(
 *				    Add(Li(&m_txtSecond))
 *				    .Add(Li(&m_editSecond), CGroupLayout::ADJUST)
 *				)
 *				.AutoEqualityDistance()  // 縦方向は、均等化しよう
 *			);
 *
 *				;
 *		}
 *		
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbGroupLayout.h
 *
 *	@date 08/07/17 新規作成
 */
class CGroupLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**@ingroup LAYOUT
	 * パラレルグループ定義クラス.
	 *
	 *		複数のレイアウトアイテムを管理。
	 *
	 *	@note 通常 Add() 以外、意識して使う必要はありません。
	 *
	 *	@par必要ファイル
	 *			TnbGroupLayout.h
	 *	
	 *	@date 08/07/17 新規作成
	 */
	class CParallelGroup
	{
	public:

		/**
		 * コンストラクタ
		 *	@note 規定の配置種を指定することが出来ます。以降、配置種を省略して Add() すると規定の配置種が使われます。
		 *	@param defloc 既定の配置種。省略すると、 DEFAULT です。
		 */
		CParallelGroup(ELocation defloc = DEFAULT) : m_defaultLocation(defloc)
		{
			m_params = new CParamVector;
		}

		/**
		 * [設定] アイテム追加.
		 *	@param layout レイアウトアイテム.
		 *	@param loc 配置種.
		 *	@return 自分の参照.
		 */
		CParallelGroup& Add(const ILayout& layout, ELocation loc)
		{
			TParam p(loc, layout.Clone());
			m_params->Add(p);
			return *this;
		}

		/**
		 * [設定] アイテム追加.
		 *	@note 既定の配置種でアイテムを追加します。
		 *	@param layout レイアウトアイテム.
		 *	@return 自分の参照.
		 */
		CParallelGroup& Add(const ILayout& layout)
		{
			return Add(layout, m_defaultLocation);
		}

		/**
		 * [取得] アイテム数取得.
		 *	@return アイテム数
		 */
		size_t GetItemCount(void) const
		{
			return m_params->GetSize();
		}

		/**
		 * [取得] レイアウトアイテム取得.
		 *	@param index インデックス。 GetItemCount() の結果より小さい値を指定します。
		 *	@return レイアウトアイテム
		 */
		ILayout* GetItemLayout(INDEX index)
		{
			return m_params->At(index).pLayout;
		}

		/**
		 * [取得] レイアウトアイテム取得.
		 *	@param index インデックス。 GetItemCount() の結果より小さい値を指定します。
		 *	@return レイアウトアイテム
		 */
		const ILayout* GetItemLayout(INDEX index) const
		{
			return m_params->At(index).pLayout;
		}

		/**
		 * [取得] アイテムの配置種取得.
		 *	@param index インデックス。 GetItemCount() の結果より小さい値を指定します。
		 *	@return アイテムの配置種
		 */
		ELocation GetItemLocation(INDEX index) const
		{
			return m_params->At(index).locate;
		}

		/**
		 * [取得] 最小サイズ取得.
		 *	@note 設定されているアイテムの最小サイズがすべて入るサイズを計算して返します。
		 *	@return サイズ。
		 */
		SIZE GetMinimumSize(void) const
		{
			SIZE ret = { 0, 0 };
			loop ( i, GetItemCount() )
			{
				SIZE sz;
				GetItemLayout(i)->GetMinimumSize(sz);
				ret.cx = max(ret.cx, sz.cx);
				ret.cy = max(ret.cy, sz.cy);
			}
			return ret;
		}

	private:
		/// 管理パラメータ型
		struct TParam
		{
			ELocation		locate;		///< アイテムの配置種
			ILayout::Ptr	pLayout;	///< レイアウトアイテム
			/// コンストラクタ
			TParam(void) { }
			/// コンストラクタ
			TParam(ELocation loc, ILayout* p) : locate(loc), pLayout(p) { }
		};
		typedef CSimpleVectorT<TParam> CParamVector;		///< TParam の配列型
		CPointerHandleT<CParamVector>	m_params;			///< アイテム群
		ELocation						m_defaultLocation;	///< 既定の配置
	};

	/**
	 * ParallelGroup の長さ種.
	 *	CSequentialGroup に CParallelGroup を登録する時に指定します。
	 */
	enum EDistance
	{
		Distance_Default,		///< デフォルト（ Distance_ItemMax 、最後のみ Distance_Rast になります)。
		Distance_Absolute,		///< 絶対値（value は縦横の絶対値 MAKELONG(X, Y) で格納）
		Distance_Rate,			///< 比率（value は比率を格納）
		Distance_ItemMax,		///< アイテム最大
		Distance_Rast,			///< 余り（一つのシーケンシャルグループに複数指定できません)
	};
	
	/**@ingroup LAYOUT
	 * シーケンシャルグループ定義クラス.
	 *
	 *		複数のパラレルグループを管理
	 *
	 *	@note 通常 AddGroup() 以外、意識して使う必要はありません。
	 *
	 *	@par必要ファイル
	 *			TnbGroupLayout.h
	 *	
	 *	@date 08/07/17 新規作成
	 */
	class CSequentialGroup
	{
	public:

		/// コンストラクタ
		CSequentialGroup(void) : m_params(new CParamVector)
		{
		}

		/**
		 * [追加] パラレルグループ追加.
		 *	@param g パラレルグループ。本クラスにはコピーが格納されます。
		 *	@param dis 長さ種。省略すると Distance_Default が指定されます。
		 *	@param val 値。長さ種が Distance_Absolute の場合は縦横の絶対値 MAKELONG(X, Y) を指定。 
		 *					Distance_Rate の場合は比率（％）を指定。それ以外の場合は省略可能。
		 *	@return 自分の参照。
		 */
		CSequentialGroup& AddGroup(CParallelGroup& g, EDistance dis = Distance_Default, int val = 0)
		{
			TParam p(dis, val, g);
			m_params->Add(p);
			return *this;
		}

		/**
		 * [設定] 長さ平均化.
		 *	@note 必要な AddGroup() 終了後、このメソッドを使用すると、長さを平均化します。
		 *	@param p パーセント。省略すると100です。
		 *	@return 自分の参照。
		 */
		CSequentialGroup& AutoEqualityDistance(int p = 100)
		{
			size_t num = GetGroupCount();
			size_t v = 0;
			loop ( i, num )
			{
				TParam& par = m_params->At(i);
				par.distance = Distance_Rate;
				double a = p;
				a /= num;
				a *= (i + 1);
				par.value = 1;
				if ( a >= v )
				{
					par.value = static_cast<size_t>(a - v);
					if ( par.value <= 0 )
					{
						par.value = 1;
					}
				}
				v = static_cast<size_t>(a);
			}
			return *this;
		}
		
		/**
		 * [取得] グループ数取得.
		 *	@return グループ数
		 */
		size_t GetGroupCount(void) const
		{
			return m_params->GetSize();
		}

		/**
		 * [取得] グループ取得.
		 *	@param index インデックス。 GetGroupCount() の結果より小さい値を指定します。
		 *	@return グループ情報
		 */
		CParallelGroup& GetGroup(INDEX index)
		{
			return m_params->At(index).parallel;
		}

		/**
		 * [取得] グループ取得.
		 *	@param index インデックス。 GetGroupCount() の結果より小さい値を指定します。
		 *	@return グループ情報
		 */
		const CParallelGroup& GetGroup(INDEX index) const
		{
			return m_params->At(index).parallel;
		}

		/**
		 * [取得] グループ長さ種取得.
		 *	@param index インデックス。 GetGroupCount() の結果より小さい値を指定します。
		 *	@return 長さ種
		 */
		EDistance GetGroupDistance(INDEX index) const
		{
			return m_params->At(index).distance;
		}

		/**
		 * [取得] グループ長さ値取得.
		 *	@param index インデックス。 GetGroupCount() の結果より小さい値を指定します。
		 *	@return 長さ値
		 */
		size_t GetGroupValue(INDEX index) const
		{
			return m_params->At(index).value;
		}

		/**
		 * [取得] 最小サイズ取得.
		 *	@note 設定されている情報から、最小のサイズを計算して返します。
		 *	@return サイズ。
		 */
		SIZE GetMinimumSize(void) const
		{
			SIZE ret = { 0, 0 };
			CWorkMemT<SIZE> sizes(GetGroupCount());
			loop ( i, GetGroupCount() )
			{
				const CParallelGroup& pg = GetGroup(i);
				SIZE sz;
				switch ( GetGroupDistance(i) )
				{
				case Distance_Absolute:// 絶対値
					{
						size_t val = GetGroupValue(i);
						sz.cx = LOWORD(val);
						sz.cy = HIWORD(val);
					}
					break;
				case Distance_Default:// デフォルト
				case Distance_Rate:// 比率
				case Distance_ItemMax:// アイテム最大
				case Distance_Rast:// 余り
					sz = pg.GetMinimumSize();
					break;
				default:
					sz .cx = 0;
					sz .cy = 0;
					ASSERT( false );
					break;
				}
				ret.cx += sz.cx;
				ret.cy += sz.cy;
				sizes[i] = sz;
			}
			// Rateチェック
			SIZE adSize = { 0, 0 };
			loop ( i, GetGroupCount() )
			{
				if ( GetGroupDistance(i) == Distance_Rate )
				{
					size_t val = GetGroupValue(i);
					INT_PTR x = ret.cx * val / 100;
					if ( sizes[i].cx > x )
					{
						adSize.cx = max(adSize.cx, down_cast<LONG>(sizes[i].cx * 100 / val));
					}
					INT_PTR y = ret.cy * val / 100; 
					if ( sizes[i].cy > y )
					{
						adSize.cy = max(adSize.cy, ToInt(sizes[i].cy * 100 / val));
					}
				}
			}
			if ( adSize.cx != 0 || adSize.cy != 0 )
			{
				ret.cx = max(ret.cx, adSize.cx);
				ret.cy = max(ret.cy, adSize.cy);
			}
			return ret;
		}

	private:
		/// 管理パラメータ型
		struct TParam
		{
			EDistance		distance;	///< 長さ種。
			size_t			value;		///< 長さ値。 Distance_Absolute , Distance_Rate を指定した時必須。
			CParallelGroup	parallel;	///< パラレルグループ。
			/// コンストラクタ
			TParam(void) { }
			/// コンストラクタ
			TParam(EDistance dis, int v, CParallelGroup& para) : distance(dis), value(v), parallel(para) { }
		};
		/// TParam の配列型
		typedef CSimpleVectorT<TParam> CParamVector;
		CPointerHandleT<CParamVector>	m_params;	///< パラレルグループ群
	};


	//-------------------------------------


	/// コンストラクタ
	CGroupLayout(void)
	{
	}

	/**
	 * [登録] 水平シーケンシャルグループ登録.
	 *	@param g シーケンシャルグループ。
	 */
	void SetHorizontalGroup(CSequentialGroup& g)
	{
		m_hGroup = g;
	}

	/**
	 * [登録] 垂直シーケンシャルグループ登録.
	 *	@param g シーケンシャルグループ。
	 */
	void SetVerticalGroup(CSequentialGroup& g)
	{
		m_vGroup = g;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		CGroupLayout* P = new CGroupLayout();
		*P = *this;
		return P;
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
		// Xを計算
		size_t hcnt = m_hGroup.GetGroupCount();
		_size.cx = (hcnt == 0) ? 0 :m_hGroup.GetMinimumSize().cx;
		// Yを計算
		size_t vcnt = m_vGroup.GetGroupCount();
		_size.cy = (vcnt == 0) ? 0 : m_vGroup.GetMinimumSize().cy;
		// 補正
		AddChinkSize(_size, hcnt, vcnt);
		return true;
	}

	/**
	 * [処理] 決定.
	 *	@note オブジェクトが管理しているサイズと指定のサイズにレイアウトします。
	 *	@param x  X座標。-1なら指定なし。
	 *	@param y  Y座標。-1なら指定なし。
	 *	@param wParam W パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 *	@param lParam L パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		size_t hcnt = m_hGroup.GetGroupCount();
		size_t vcnt = m_vGroup.GetGroupCount();
		SIZE chinkSize = { 0, 0 };
		AddChinkSize(chinkSize, hcnt, vcnt);
		//======= X座標 
		if ( x >= 0 && hcnt > 0 )
		{
			//-- 有効幅
			INT_PTR width = m_layoutSize.cx - chinkSize.cx;
			INT_PTR widthOrg = width;
			//
			INT_PTR xx = m_margn.left;
			CWorkMemT<INT_PTR> xpos(hcnt);
			CWorkMemT<size_t> xlen(hcnt);
			INDEX rastItem = INVALID_INDEX;
			loop ( index, hcnt )
			{
				xpos[index] = xx;
				const CParallelGroup& pg = m_hGroup.GetGroup(index);
				size_t val = m_hGroup.GetGroupValue(index);
				EDistance dis = m_hGroup.GetGroupDistance(index);
				if ( dis == Distance_Default )
				{
					dis = Distance_ItemMax;
					if ( index == hcnt - 1 && rastItem == INVALID_INDEX )
					{
						dis = Distance_Rast;
					}
				}
				switch ( dis )
				{
				case Distance_Absolute:// 絶対値
					val = LOWORD(val);
					break;
				case Distance_Rate:// 比率
					val = widthOrg * val / 100;
					break;
				case Distance_ItemMax:// アイテム最大
					val = pg.GetMinimumSize().cx;
					break;
				case Distance_Rast:// 余り(二つ指定禁止)
					val = 0;
					ASSERT( rastItem == INVALID_INDEX );
					rastItem = index;
					break;
				default:
					ASSERT( false );
					break;
				}
				xlen[index] = val;
				width -= val;
				xx += val;
				xx += m_gapSize.cx; //アイテム間のサイズ
			}
			if ( rastItem != INVALID_INDEX )
			{
				// rastアイテムあり。以降、ずらす
				xlen[rastItem] = width;
				for ( size_t i = rastItem + 1; i < hcnt; i++ )
				{
					xpos[i] += width;
				}
			}
			loop ( index, hcnt )
			{
				CParallelGroup& pg = m_hGroup.GetGroup(index);
				loop ( i, pg.GetItemCount() )
				{
					HorizontalItemDecide(pg.GetItemLayout(i), x, xpos[index], xlen[index], pg.GetItemLocation(i), wParam, lParam);
				}
			}
		}
		//======= Y座標 
		if ( y >= 0 && vcnt > 0 )
		{
			//-- 有効幅
			INT_PTR height = m_layoutSize.cy - chinkSize.cy;
			INT_PTR heightOrg = height;
			//
			INT_PTR yy = m_margn.top;
			CWorkMemT<INT_PTR> ypos(vcnt);
			CWorkMemT<size_t> ylen(vcnt);
			INDEX rastItem = INVALID_INDEX;
			loop ( index, vcnt )
			{
				ypos[index] = yy;
				const CParallelGroup& pg = m_vGroup.GetGroup(index);
				size_t val = m_vGroup.GetGroupValue(index);
				EDistance dis = m_vGroup.GetGroupDistance(index);
				if ( dis == Distance_Default )
				{
					dis = Distance_ItemMax;
					if ( index == vcnt - 1 && rastItem == INVALID_INDEX )
					{
						dis = Distance_Rast;
					}
				}
				switch ( dis )
				{
				case Distance_Absolute:// 絶対値
					val = HIWORD(val);
					break;
				case Distance_Rate:// 比率
					val = heightOrg * val / 100;
					break;
				case Distance_ItemMax:// アイテム最大
					val = pg.GetMinimumSize().cy;
					break;
				case Distance_Rast:// 余り(二つ指定禁止)
					val = 0;
					ASSERT( rastItem == INVALID_INDEX );
					rastItem = index;
					break;
				default:
					ASSERT( false );
					break;
				}
				ylen[index] = val;
				height -= val;
				yy += val;
				yy += m_gapSize.cy; //アイテム間のサイズ
			}
			if ( rastItem != INVALID_INDEX )
			{
				// rastアイテムあり。以降、ずらす
				ylen[rastItem] = height;
				for ( size_t i = rastItem + 1; i < vcnt; i++ )
				{
					ypos[i] += height;
				}
			}
			loop ( index, vcnt )
			{
				CParallelGroup& pg = m_vGroup.GetGroup(index);
				loop ( i, pg.GetItemCount() )
				{
					VerticalItemDecide(pg.GetItemLayout(i), y, ypos[index], ylen[index], pg.GetItemLocation(i), wParam, lParam);
				}
			}
		}
	}


	//------------------------


	/**
	 * [作成] パラレルグループ作成.
	 *	@note グローバル関数です。これを使うことで、パラレルグループクラス作成を暗黙的に済ますことが出来ます。
	 *	@note 規定の配置種を指定します。以降、配置種を省略して Add すると規定の配置種が使われます。
	 *	@param defloc 既定の配置種
	 *	@return CParallelGroup.
	 */
	inline friend CParallelGroup Add(ELocation defloc)
	{
		return CParallelGroup(defloc);
	}

	/**
	 * [作成] パラレルグループ作成＆アイテム追加.
	 *	@note グローバル関数です。これを使うことで、パラレルグループクラス作成を暗黙的に済ますことが出来ます。
	 *	@param layout レイアウトアイテム.
	 *	@return CParallelGroup.
	 */
	inline friend CParallelGroup Add(const ILayout& layout)
	{
		return CParallelGroup().Add(layout);
	}

	/**
	 * [作成] パラレルグループ作成＆アイテム追加.
	 *	@note グローバル関数です。これを使うことで、パラレルグループクラス作成を暗黙的に済ますことが出来ます。
	 *	@param layout レイアウトアイテム.
	 *	@param loc 配置種.
	 *	@return CParallelGroup.
	 */
	inline friend CParallelGroup Add(const ILayout& layout, ELocation loc)
	{
		return CParallelGroup().Add(layout, loc);
	}

	/**
	 * [作成] シーケンシャルグループ作成＆パラレルグループ追加.
	 *	@note グローバル関数です。これを使うことで、シーケンシャルグループクラス作成を暗黙的に済ますことが出来ます。
	 *	@param g パラレルグループ。本クラスにはコピーが格納されます。
	 *	@param dis 長さ種。省略すると Distance_Default が指定されます。
	 *	@param val 値。長さ種が Distance_Absolute の場合は縦横の絶対値 MAKELONG(X, Y) を指定。 
	 *					Distance_Rate の場合は比率（％）を指定。それ以外の場合は省略可能。
	 *	@return CSequentialGroup.
	 */
	inline friend CSequentialGroup AddGroup(CParallelGroup& g, EDistance dis = Distance_Default, int val = 0)
	{
		return CSequentialGroup().AddGroup(g, dis, val);
	}

private:
	CSequentialGroup	m_hGroup;			///< 水平グループ X座標の管理
	CSequentialGroup	m_vGroup;			///< 垂直グループ Y座標の管理
};



}; // TNB