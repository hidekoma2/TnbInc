#pragma once
/**
 *@file
 * 7SEG表示コントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingStatic.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 7SEG 風値表示コントロールクラス.
 *
 *		7SEGの表示で０～９の値を表すことが出来ます。桁数は DIGIT で指定します。
 *
 *	@par使い方
 *		マジックカラーコードでボタンを現したビットマップを一枚用意します。
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *		最初に SetImage() してください。
 *
 *		マジックカラーコードは 7SEG に以下のように対応します。
 *		\code
 *				 000
 *				2   1
 *				 333
 *				5   4
 *				 666 7
 *		\endcode
 *	@param DIGIT 桁数
 *
 *	@par必要ファイル
 *			TnbMfcSevenSegCtrl.h
 * 
 *	@date 16/07/26 新規作成
 *	@date 16/08/09 パラメータ DIGIT を新規追加。２桁以上にも対応するようにした。
 */
template<int DIGIT = 1>
class CSevenSegCtrlT : public CDrawingStatic
{
	DEFSUPER(CDrawingStatic);
public:

	/**
	 * マジックカラーコード.
	 */
	enum EMagicColorCode
	{
		MCC0 = RGB(128, 0, 0),		///< 暗い赤
		MCC1 = RGB(255, 0, 0),		///< 赤
		MCC2 = RGB(0, 0, 128),		///< 暗い青
		MCC3 = RGB(0, 0, 255),		///< 青
		MCC4 = RGB(0, 128, 0),		///< 暗い緑
		MCC5 = RGB(0, 255, 0),		///< 緑
		MCC6 = RGB(0, 128, 128),	///< 暗い水色
		MCC7 = RGB(0, 255, 255),	///< 水色
	};

	/// コンストラクタ
	CSevenSegCtrlT(void)
	{
		Zero(m_value);
	}

	/**
	 * [設定] イメージ指定.
	 *	@note 指定後、値を設定しないとイメージが表示されません。
	 *	@param b ベース画像。7seg の部分がマジックカラーコードで書かれているもの。
	 *	@param onColor ONの際の色。マジックカラーコード以外の色にすること。
	 *	@param offColor OFFの際の色。マジックカラーコード以外の色にすること。
	 */
	void SetImage(CBitmapHandle b, COLORREF onColor = RGB(254, 0, 0), COLORREF offColor = RGB(60, 0, 0))
	{
		m_baseImage = b;
		m_onColor = onColor;
		m_offColor = offColor;
	}

	/**
	 * [設定] 値設定.
	 *	@param value 値。有効範囲は 0～'(10^DIGIT)-1' になります。
	 *	@param hasDot ドットありなら true を指定。
	 *	@param isZeroPadding true ならゼロパディングあり。 false ならなし。
	 */
	void SetValues(DWORD value, bool hasDot = false, bool isZeroPadding = false)
	{
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			Zero(m_value);
			loop ( i, DIGIT )
			{
				BYTE v = m_NumberImage(value % 10);
				if ( i == 0 && hasDot )
				{
					v |= _BIT(7);
				}
				m_value[DIGIT - i - 1] = v;
				value /= 10;
				if ( ! isZeroPadding && value == 0 )
				{
					break;
				}
			}
			m_ResetBitmap();
		}
	}

	/**
	 * [設定] 値設定.
	 *		各桁ごとに数値、ドットを指定できます。
	 *	@param idx 桁。
	 *	@param value 値。
	 *	@param hasDot ドットありなら true を指定。
	 */
	void SetValue(INDEX idx, int value, bool hasDot = false)
	{
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			BYTE v = m_NumberImage(value);
			if ( hasDot )
			{
				v |= _BIT(7);
			}
			m_value[DIGIT - idx - 1] = v;
			m_ResetBitmap();
		}
	}

	/**
	 * [設定] 7seg設定.
	 *		各桁ごとに各パーツを自由にON/OFF出来ます。
	 *	\code
	 *			 000
	 *			2   1
	 *			 333
	 *			5   4
	 *			 666 7
	 *	\endcode
	 *	例えば _BIT(4) | _BIT(5) | _BIT(6) を指定すると小文字の u になります。
	 *	@param idx 桁。
	 *	@param b 値。
	 */
	void SetBitset(INDEX idx, BYTE b)
	{
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			m_value[DIGIT - idx - 1] = b;
			m_ResetBitmap();
		}
	}

protected:
	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
	}

private:
	BYTE m_NumberImage(int val)
	{
		//		 000
		//		2   1
		//		 333
		//		5   4
		//		 666 7
		const static BYTE pats[11] = 
		{
			//   76543210
			_BIN(01110111),	//0
			_BIN(00010010),	//1
			_BIN(01101011),	//2
			_BIN(01011011),	//3
			_BIN(00011110),	//4
			_BIN(01011101),	//5
			_BIN(01111100),	//6
			_BIN(00010011),	//7
			_BIN(01111111),	//8
			_BIN(01011111),	//9
			_BIN(11101101),	//E
		};
		BYTE v = (val >= 0 && val < 10) ? pats[val] : pats[10] ;
		return v;
	}

	/// 画像再構築
	void m_ResetBitmap(void)
	{
		CSize sz;
		m_baseImage.GetSize(sz);
		CBitmapImage bmp;
		bmp.Set(sz.cx * DIGIT, sz.cy);
		loop ( i, DIGIT )
		{
			CBitmapImage b = m_ResetBitmap(m_value[i]);
			bmp.Insert(i * sz.cx, 0, b);
		}
		_super::SetDrawingBitmap(bmp, true, true);
	}

	/// 画像再構築
	CBitmapImage m_ResetBitmap(BYTE v)
	{
		CBitmapImage bi;
		bi.Set(m_baseImage);
		COLORREF on = m_onColor;
		COLORREF of = m_offColor;
		bi.ChangePixelColor(MCC0, (v & _BIT(0)) ? on : of);
		bi.ChangePixelColor(MCC1, (v & _BIT(1)) ? on : of);
		bi.ChangePixelColor(MCC2, (v & _BIT(2)) ? on : of);
		bi.ChangePixelColor(MCC3, (v & _BIT(3)) ? on : of);
		bi.ChangePixelColor(MCC4, (v & _BIT(4)) ? on : of);
		bi.ChangePixelColor(MCC5, (v & _BIT(5)) ? on : of);
		bi.ChangePixelColor(MCC6, (v & _BIT(6)) ? on : of);
		bi.ChangePixelColor(MCC7, (v & _BIT(7)) ? on : of);
		return bi;
	}

	BYTE			m_value[DIGIT];	///< 値

	CBitmapHandle	m_baseImage;	///< ベース画像.
	COLORREF		m_onColor;		///< オン時のカラー
	COLORREF		m_offColor;		///< オフ時のカラー
};



}; //MFC
}; //TNB
