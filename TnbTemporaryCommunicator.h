#pragma once
/**
 *@file
 * テンポラリ通信関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCommunication.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * テンポラリ通信クラス
 *
 *		ICommunication を実装しています。
 *		送信で本クラスの保持するByte区切りのメモリにプールし、受信でそのメモリから読み出します。
 *
 *	@note 主に 通信モジュールの単体テストに使用されます。
 *
 *	@par必要ファイル
 *			TnbTemporaryCommunicator.h
 * 
 *	@date 06/05/31 新規作成
 *	@date 06/06/08 名前変更
 *	@date 06/07/07 InterfaceID概念、追加
 *	@date 06/08/31 CommViewer に対応。
 *	@date 06/09/09 IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *					ECE_Connect , ECE_Disconnect イベント、追加。
 *	@date 07/02/09 親クラス変更。
 *	@date 07/05/10 クラス名、変更。
 *	@date 07/06/11 親クラスの Start() メソッドに対応。
 *	@date 07/09/13 親クラス変更。
 *	@date 08/12/08 受信タイミングを変えられるようにした。
 *	@date 10/05/28 SetReceiveTimeout() を実装。
 *	@date 10/08/24 構造変更。受信タイミングを自由化。
 *	@date 11/07/06 名前変更。 SetSendErrorMode() 追加。
 *	@date 11/07/07 オープン状態を用意.
 *	@date 12/03/15 ReferSentRowData() を新規。
 */
class CTemporaryCommunicator : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);
public:

	/// コンストラクタ 
	CTemporaryCommunicator(void) : m_property(_T("TemporaryCommunicator"), TNBVIW_KIND_TEMP), m_isOpened(false), m_isSendError(false)
	{
	}

	/**
	 * [取得] 性能取得
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const 
	{
		return m_property; 
	}

	/**
	 * [設定] パケットサイズ設定.
	 *		送信できるサイズや受信できるサイズを指定できます。性能に影響します。
	 *	@param receivePacketSize
	 *	@param sendPacketSize
	 */
	void SetPacketSize(DWORD receivePacketSize, DWORD sendPacketSize)
	{
		m_property.receivePacketSize = receivePacketSize;
		m_property.sendPacketSize = sendPacketSize;
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return 常に true。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_property.receiveTimeout = to;
		return true;
	}

	/**
	 * [処理] オープン.
	 *	@return true
	 */
	virtual bool Open(void)
	{ 
		Purge();
		m_isOpened = true;
		return true;
	}
	
	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		Purge();
		m_isOpened = false;
	}
	
	/**
	 * [確認] オープン確認.
	 *	@return true
	 */
	virtual bool IsOpened(void) const
	{
		return m_isOpened;
	}

	/**
	 * [処理] 送信.
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! m_isOpened || m_isSendError )
		{
			return INVALID_SIZE;
		}
		CByteVector vb;
		vb.SetElements(size, static_cast<const BYTE*>(P));
		EXCLUSIVE( &m_sentData );
		m_sentRowData.Add(vb);
		m_sentData.Append(vb);
		return size;
	}

	/**
	 * [処理] 受信.
	 *	@param[in] size サイズ。
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@return 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( m_isOpened )
		{
			INDEX idx = m_Find();
			if ( idx != INVALID_INDEX )
			{
				TParam& p = m_receiveReserves[idx];
				BYTE* B = static_cast<BYTE*>(_P);
				size_t l = min(p.data.GetSize(), size);
				MemCopy(B, p.data.ReferBuffer(), l);
				if ( l >= p.data.GetSize() )
				{
					m_receiveReserves.Remove(idx);
				}
				else
				{
					p.data.RemoveElements(0, l);
				}
				return l;
			}
		}
		return 0;
	}

	/**
	 * [確認] 受信データ確認.
	 *	@return 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( m_isOpened )
		{
			INDEX idx = m_Find();
			if ( idx != INVALID_INDEX )
			{
				return m_receiveReserves[idx].data.GetSize();
			}
		}
		return 0;
	}

	/**
	 * [確認] チェック.
	 *		状態を確認します。
	 *	@return CONNECTING	オープン＆接続状態
	 *	@return CLOSED	クローズ
	 */
	virtual EState Check(void) const 
	{ 
		return m_isOpened ? CONNECTING : CLOSED;
	}

	/**
	 * [処理] パージ.
	 */
	virtual void Purge(void)
	{
		EXCLUSIVE2( &m_receiveReserves, &m_sentData );
		m_receiveReserves.RemoveAll();
		m_sentData.RemoveAll();
		m_sentRowData.RemoveAll();
	}

	/**
	 * [処理] 受信情報に追加.
	 *		Receive() で受信されるデータを追加します。
	 *	@param size データサイズ。
	 *	@param pData データ。
	 *	@param delay 遅延(ms)。受信されるまでの時間を指定出来ます。
	 */
	void AddReceivingData(size_t size, LPCVOID pData, DWORD delay = 0)
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( size > 0 )
		{
			TParam p;
			p.tick = ::GetTickCount();
			p.delay = delay;
			p.data.SetElements(size, static_cast<const BYTE*>(pData));
			m_receiveReserves.Add(p);
		}
	}

	/**
	 * [処理] 受信情報に追加.
	 *		Receive() で受信されるデータを追加します。
	 *	@param data データ。
	 *	@param delay 遅延(ms)。受信されるまでの時間を指定出来ます。
	 */
	void AddReceivingData(const IConstCollectionT<BYTE>& data, DWORD delay = 0)
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( data.GetSize() > 0 )
		{
			TParam p;
			p.tick = ::GetTickCount();
			p.delay = delay;
			p.data.Copy(data);
			m_receiveReserves.Add(p);
		}
	}

	/**
	 * [取得] 送信情報参照.
	 *		Send() で送信したデータを取得できます。データは連結されています。
	 *	@note 変更する場合、 Lock して行うようにしてください。
	 *	@return データ。
	 */
	CByteVector& ReferSentData(void)
	{
		return m_sentData;
	}

	/**
	 * [取得] 送信情報参照.
	 *		Send() で送信したデータを取得できます。
	 *	@note 変更する場合、 Lock して行うようにしてください。
	 *	@return データ。
	 */
	CVectorT<CByteVector>& ReferSentRowData(void)
	{
		return m_sentRowData;
	}

	/**
	 * [設定] 送信エラーモード設定.
	 *	@param isSendError true を指定すると、 Send() で INVALID_SIZE を返すようになります。
	 *						false なら Send() は正常動作します。
	 */
	void SetSendErrorMode(bool isSendError)
	{
		m_isSendError = isSendError;
	}

private:

	INDEX m_Find(void) const
	{
		loop ( i, m_receiveReserves )
		{
			const TParam& p = m_receiveReserves[i];
			DWORD t = ::GetTickCount() - p.tick;
			if ( p.delay == 0 || p.delay <= t )
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

	
	/// パラメータ
	struct TParam
	{
		DWORD		tick;		///< 登録時のTICK
		DWORD		delay;		///< 遅延時間
		CByteVector	data;		///< データ
	};
	CVectorT<TParam>		m_receiveReserves;	///< 受信予約
	CByteVector				m_sentData;			///< 送信したデータ。
	CVectorT<CByteVector>	m_sentRowData;		///< 送信したデータ。
	_super::TProperty		m_property;			///< プロパティ
	bool					m_isSendError;		///< 送信はエラー？
	bool					m_isOpened;			///< オープン状態？
};



}; // TNB

