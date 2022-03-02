#pragma once
/**
 *@file
 * レイアウト関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbRatioLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * ワイドレイアウトアイテムクラス.
 *
 *	スクリーンの状態により、２つのレイアウトアイテムの並びを縦か横にすることが出来ます。
 *	Windows Moblie でコントロールの配置を目的としたものです。
 *			\code
 *			┌－－－－－－－－－┐
 *			ｌ┏───┳───┓ｌ画面が縦長の時
 *			ｌ│　１　│　２　│ｌ
 *			ｌ┗───┻───┛ｌ
 *			└－－－－－－－－－┘
 *			※各レイアウトアイテムの幅は、全体の幅から、比率指定できます。
 *			※各レイアウトアイテムの高さは、全体の高さになります。
 *			┌－－－－－┐
 *			ｌ┏───┓ｌ画面が横長の時
 *			ｌ│　１　│ｌ
 *			ｌ┣───┫ｌ
 *			ｌ│　２　│ｌ
 *			ｌ┗───┛ｌ
 *			└－－－－－┘
 *			※各レイアウトアイテムの高さは、全体の高さから、比率指定できます。
 *			※各レイアウトアイテムの幅は、全体の幅になります。
 *			\endcode
 *
 *	@note 敷居は、縦画面の場合と横画面の場合で別々に指定できます( SetCenter() )。
 *
 *	@note スクリーン状態と縦横並びの関係は、 SetReverseMode() で変更できます。
 *
 *	@par必要ファイル
 *			TnbWideLayout.h
 *
 *	@date 09/09/15 新規作成
 */
class CWideLayout : public CRatioLayout
{
	DEFSUPER(CRatioLayout);
public:

	/// コンストラクタ
	CWideLayout(void) : m_isReverse(false)
	{
		m_center[0] = 50;
		m_center[1] = 50;
	}

	/**
	 * [設定] 敷居変更.
	 *	@note 設定していない場合 50％ です。
	 *	@param isWide true なら横画面の場合の設定。 false なら縦画面の場合の設定。
	 *	@param parsent 上か左のレイアウトのパーセント。
	 */
	void SetCenter(bool isWide, DWORD parsent = 50)
	{
		m_center[isWide ? 1 : 0] = parsent;
	}

	/**
	 * [設定] リバース設定.
	 *	@param isReverse true を指定すると、縦画面の場合左右、横画面の場合、上下に配置するようになります。
	 */
	void SetReverseMode(bool isReverse = false)
	{
		m_isReverse = isReverse;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		CWideLayout* P = new CWideLayout();
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
		m_SetParameter();
		return _super::GetMinimumSize(_size);
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
		m_SetParameter();
		_super::Decide(x, y, wParam, lParam);
	}

private:
	// パラメタ設定
	void SetParameter(bool isHorizental, DWORD parsent)
	{
		_super::SetParameter(isHorizental, parsent);
	}
	// パラメタ設定
	void m_SetParameter(void) const
	{
		bool r = m_IsWideMode();
		CWideLayout* P = const_cast<CWideLayout*>(this); //constはがし
		P->SetParameter(r, m_center[r ? 1 : 0]);
	}
	// true なら 横画面
	bool m_IsWideMode(void) const
	{
		int nWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int nHeight = ::GetSystemMetrics(SM_CYSCREEN);
		return (nHeight < nWidth) ^ (m_isReverse);
	}
	size_t	m_center[2];	///< 中央の割合 [0];縦画面の場合 [1];横画面
	bool	m_isReverse;	///< true なら　レイアウト条件反転
};



}; // TNB
