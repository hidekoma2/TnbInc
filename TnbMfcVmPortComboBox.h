#pragma once
/**
 *@file
 * 仮想マシンシリアルポート選択コンボボックス関係のヘッダ
 *		
 *		デバグ、開発用です。仮想マシンとのシリアル通信をおこなうための仕組みを用意します。デバッグビルドの際のみ、
 *		仕組みが現れます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcPortComboBox.h"
#include "TnbRs232c.h"
#include "TnbPipe.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 仮想ポート付き、シルアルポートComboBoxコントロール.
 *	
 *		デバグ、開発用です。 CVmRs232c と対で使用してください。
 *
 *	@par使い方
 *		リソースエディタでコンボボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CComboBox）を本クラスに置き換えて使用します。\n
 *		コンボボックスのプロパティは、ドロップダウンリストにしておいてください。
 */
class CVmPortComboBox : public CPortComboBox
{
	DEFSUPER(CPortComboBox);
public:

	/// コンストラクタ
	CVmPortComboBox(void)
	{
		#ifdef _DEBUG
			AddExtendPort(_T("pipe\\vm"));
		#endif
	}

protected:

	/**
	 * [設定] 拡張ポート名設定.
	 *	@note 本クラスでは使用する必要がありません。
	 *	@param lpszName ダミー
	 */
	void AddExtendPort(LPCTSTR lpszName)
	{
		_super::AddExtendPort(lpszName);
	}
};



}; // MFC



/**@ingroup COMMUNICATION
 * RS232Cアクセスクラス
 *
 *		デバグ、開発用です。 CVmPortComboBox と対で使用してください。
 *
 *		シリアルアクセスを行うクラスです。Port番号が０以外の時、普通のシリアルをオープンしますが、
 *		Portが０の場合はネーム付きパイプ“vmware-serial-port”を使用する用になっています。
 *
 *		@note Virtual PC の設定で COM1 に "\\.\pipe\vmware-serial-port"と言う名前付きパイプを指定
 *		しておくことで、仮想マシン上の CVmRs232c とホストマシン上の CVmRs232c と通信が出来ます。
 */
class CVmRs232c : public ICommunication
{
public:

	/// コンストラクタ
	CVmRs232c(void)
	{
		m_pComm = &m_rs232c;
	}

	/**
	 * [設定] オープン設定.
	 *		RS232Cの設定を行います。通常、 Open の前に使用します。
	 *	@note Open 前に実行した場合必ず成功します。
	 *	@note オープン中、本メソッドでCOMポートナンバーを変更した場合、クローズされます。
	 *	@note オープン中、本メソッドを使用した場合、エラーになる可能性もあります。
	 *	@param port COMポートナンバー。 数値でも文字列でも指定可能です。
	 *	@param dwBaudRate 転送速度[bps]
	 *	@param bByteSize キャラクタビット[bits]
	 *	@param eParity パリティ（偶数／奇数）
	 *	@param eStopBits ストップビット[bits]
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD SetParameter(CRs232c::CPort port, DWORD dwBaudRate, BYTE bByteSize, CRs232c::EParity eParity, CRs232c::EStopBits eStopBits)
	{
		if ( port.GetPortName().Find(_T("\\.\\\\pipe\\")) == 0 || port.GetNumber() == 0 )
		{
			m_pComm = &m_pipe;
			m_pipe.SettingNamedPipe(_T("\\\\.\\pipe\\vmware-serial-port"), true);
			return 0;
		}
		m_pComm = &m_rs232c;
		return m_rs232c.SetParameter(port, dwBaudRate, bByteSize, eParity, eStopBits);
	}

	/**
	 * [取得] 性能取得.
	 *	@note 正確な情報は Open後に取得できます。
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_pComm->GetProperty();
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@retval true 成功。
	 *	@retval false 失敗。サポートできない時もこれが返ります。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return m_pComm->SetReceiveTimeout(to);
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		return m_pComm->Open();
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		m_pComm->Close();
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const
	{
		return m_pComm->IsOpened();
	}

	/**
	 * [確認] 接続確認.
	 *	@retval true 接続中。
	 *	@retval false 切断中。
	 */
	virtual bool IsConnect(void) const
	{
		return m_pComm->IsConnect(); 
	}

	/**
	 * [確認] 送信可能確認.
	 *	@retval true 送信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanSend(void) const
	{ 
		return m_pComm->CanSend(); 
	}

	/**
	 * [確認] 受信可能確認.
	 *	@retval true 受信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanReceive(void) const
	{
		return m_pComm->CanReceive(); 
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
		return m_pComm->Send(size, P); 
	}

	/**
	 * [処理] 受信.
	 *	@param[in] size サイズ。
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 0 受信データなし。
	 *	@retval 上記以外 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		return m_pComm->Receive(size, _P); 
	}

	/**
	 * [確認] 受信データ確認.
	 *	@retval INVALID_SIZE 不明。
	 *	@retval 上記以外 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const 
	{
		return m_pComm->GetReceivingDataSize(); 
	}

	/**
	 * [確認] チェック.
	 *		状態を確認します。
	 *	@retval OPENNING	オープン状態
	 *	@retval CONNECTING	オープン＆接続状態
	 *	@retval CLOSED		クローズ状態	
	 */
	virtual EState Check(void) const 
	{
		return m_pComm->Check(); 
	}

	/**
	 * [処理] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void)
	{
		m_pComm->Purge(); 
	}

	/**
	 * [参照] RS232Cのオプジェクト参照
	 *	@return 参照
	 */
	CRs232c& Rs232c(void)
	{
		return m_rs232c;
	}

	/**
	 * [参照] RS232Cのオプジェクト参照
	 *	@return 参照
	 */
	CPipe& Pipe(void)
	{
		return m_pipe;
	}

private:
	ICommunication*	m_pComm;		///< アクティブな通信オブジェクト
	CRs232c			m_rs232c;		///< RS232C
	CPipe			m_pipe;			///< パイプ
};



}; //TNB
