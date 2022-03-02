#pragma once
/**
 *@file
 * 非同期書込み処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbWriter.h"
#include "TnbAbstractReport.h"
#include "TnbCollectionAdapter.h"
#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE THREAD
 * 非同期書き込みクラス
 *
 *		書込み処理自体をスレッドで行いますので、
 *		関数コールは即時リターンします。
 *
 *	@note 本クラスは読込み機能はありません。
 *
 *	@par必要ファイル
 *			TnbAsyncWriter.h
 *
 *	@date 06/05/02 新規作成
 *	@date 06/05/30 クラス名変更。スレッド以外で動くモードも追加。 ICommunication 実装。
 *	@date 06/07/07 InterfaceID概念、追加
 *	@date 06/08/23 CFileWriterEx から改名。
 *	@date 06/08/31 CommViewer に対応。
 *	@date 06/09/09 IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *					ECE_Connect , ECE_Disconnect イベント、追加。
 *	@date 06/10/16 IWriter を使用するように変更
 *	@date 07/02/08 クラス名、変更。
 *	@date 07/02/09 親クラス変更。
 *	@date 07/06/11 親クラスの Start() メソッドに対応。
 *	@date 09/08/27 SetPriority() のコメント補正。構成整理。
 *	@date 10/07/30 IReport 実装の廃止
 */
class CAsyncWriter : public CThreadDispatch::IListener
{
public:

	/**@ingroup FILE THREAD
	 * 非同期書き込みのコマンドインターフェース
	 *
	 *		通常実装することは有りませんが、特殊なファイル操作を行いたい時に実装し、
	 *		Command() に渡すことが出来ます。
	 *
	 *	@see CAsyncWriter::Command
	 *
	 *	@par必要ファイル
	 *			TnbAsyncWriter.h
	 *
	 *	@date 06/05/02 新規作成
	 */
	struct ICommand
	{
		/// デストラクタ
		virtual~ ICommand(void) {}
		/** 
		 * [処理] 処理
		 *	@param fw オープンしてあるWriterが渡されます。
		 *	@return falseならスレッド終了させます。
		 */
		virtual bool Exec(IWriter& fw) = 0;
	};


	//-------------------------


	/// コンストラクタ 
	explicit CAsyncWriter(void)
	{
		m_pWriter = NULL;
		m_threadDispatch.SetListener(this);
	}

	/// デストラクタ
	virtual ~CAsyncWriter(void)
	{
		Stop();
	}

	/**
	 * [確認] 開始チェック.
	 *		Start() メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	bool IsAlive(void) const 
	{
		return (m_pWriter != NULL) ? m_pWriter->CanWrite() : false;
	}
	
	/**
	 * [設定] 優先度設定.
	 *		書込みスレッドの優先度を指定出来ます。
	 *	@param priority 優先度（-2～2, 15。0がノーマル。大きいほうが優先度大）
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetPriority(int priority)
	{
		return m_threadDispatch.SetPriority(priority);
	}

	/**
	 * [設定] ライター設定
	 *	@param pfw オープンしてあるWriterを指定します。
	 *				Openしていないオブジェクトを渡すと失敗します。
	 *				pfw は参照しますので、使用中は破棄しないで下さい。
	 *	@retval true 設定成功
	 *	@retval false 失敗
	 */
	bool SetWriter(IWriter* pfw)
	{
		Stop();
		if ( ! pfw->CanWrite() )
		{
			return false;
		}
		m_pWriter = pfw;
		return true;
	}

	/**
	 * [設定] 開始.
	 *		本メソッド実行後、 Send() Write() SetEnd() Seek() 等使用できるようになります。
	 *	@note	SetWriter() でライターの設定後に使用します。
	 *	@retval true 要求成功
	 *	@retval false 失敗
	 */
	bool Start(void)
	{
		Stop();
		if ( m_pWriter == NULL )
		{
			return false;
		}
		if ( ! m_pWriter->CanWrite() )
		{
			return false;
		}
		return m_threadDispatch.Start(_T("AsyncWriter:書込"));
	}

	/**
	 * [設定] 終了.
	 *	@note オープンしているWriterをCloseします。
	 *		クローズ完了するまで、リターンしません。
	 */
	void Stop(void)
	{
		if ( IsAlive() )
		{
			Close();
		}
		m_threadDispatch.Stop();
	}

	/**
	 * [実行] ファイル書き込み
	 *	@attention 2GByte以上は一度に書き込めません。
	 *	@note 内部で一度 ByteVectoer に変換しています。
	 *	@param c 書き込みデータ。
	 *	@retval true 書き込み要求成功
	 *	@retval	false	失敗。 Start() を実行していない場合も失敗します。
	 */
	bool Write(const IConstCollectionT<BYTE>& c)
	{
		return m_OnDispatch(new CWriteCmd(c));
	}

	/**
	 * [実行] シーク
	 *	@param iOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると IWriter::TOP になります。
	 *	@retval true シーク要求成功
	 *	@retval	false	失敗。 Start() を実行していない場合も失敗します。
	 */
	bool Seek(int iOffset, IWriter::ESeekMode eSeek = IWriter::TOP)
	{
		return m_OnDispatch(new CSeekCmd(iOffset, eSeek));
	}

	/**
	 * [実行] EOFを指定.
	 *		現在のファイルポインタの位置をEOFにします。
	 *	@see https://msdn.microsoft.com/ja-jp/library/cc429774.aspx
	 *	@retval true EOF書き込み要求成功
	 *	@retval	false	失敗。 Start() を実行していない場合も失敗します。
	 */
	bool SetEnd(void)
	{
		return m_OnDispatch(new CSetEndCmd);
	}

	/**
	 * [実行] クローズ.
	 *		オープンしているWriterをCloseします。
	 *	@retval true 要求成功
	 *	@retval	false	失敗。 Start() を実行していない場合も失敗します。
	 */
	bool Close(void)
	{
		return m_OnDispatch(new CCloseCmd);
	}

	/**
	 * [実行] コマンド
	 *	@param piCmd new で作成した ICommand インターフェースを実装したクラス。
	 *	@retval true 要求成功
	 *	@retval	false	失敗。 Start() を実行していない場合も失敗します。
	 */
	bool Command(ICommand* piCmd)
	{
		return m_OnDispatch(piCmd);
	}

private:

	CThreadDispatch	m_threadDispatch;	///< Dispatchクラス
	IWriter*		m_pWriter;			///< ファイルクラス

	enum{ WMWF_COMMAND = WM_APP };		///< コマンド依頼イベントコード。

	/**
	 * [通知] メッセージ受信
	 *	@param nMsg メッセージ
	 *	@param wParam IWriter のポインタ
	 *	@param lParam ICommand のポインタ。
	 *	@retval true	正常
	 *	@retval false	切断要求
	 */
	virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		if ( nMsg == WMWF_COMMAND )
		{
			IWriter* pFw = reinterpret_cast<IWriter*>(wParam);
			ICommand* piCmd = reinterpret_cast<ICommand*>(lParam);
			if ( ! piCmd->Exec(*pFw) )
			{
				m_threadDispatch.PostMessage(WM_QUIT);
			}
			delete piCmd;
		}
		return true;
	}

	/**
	 * メッセージ振り分け.
	 *	@param P ICommand のポインタ。
	 *	@retval true 書き込み要求成功
	 *	@retval false 失敗
	 */
	bool m_OnDispatch(ICommand* P)
	{
		bool b = m_threadDispatch.PostMessage(
				WMWF_COMMAND, reinterpret_cast<WPARAM>(m_pWriter), reinterpret_cast<LPARAM>(P));
		if ( ! b )
		{
			delete P;
			return false;
		}
		return true;
	}

	///書き込みコマンド
	class CWriteCmd : public ICommand
	{
		CByteVector m_vb;		///< 書き込みデータ
	public:	
		CWriteCmd(const IConstCollectionT<BYTE>& c) { m_vb.Copy(c); }
		bool Exec(IWriter& fw) { fw.WriteFrom(m_vb); return true; }
	};

	///シークコマンド
	class CSeekCmd : public ICommand
	{
		int					m_iOffset;	///< SEEK移動量
		IWriter::ESeekMode	m_eSeek;	///< SEEK開始位置
	public:	
		CSeekCmd(int iOffset, IWriter::ESeekMode eSeek) { m_iOffset = iOffset; m_eSeek = eSeek; }
		bool Exec(IWriter& fw) { fw.Seek(m_iOffset, m_eSeek); return true; }
	};

	/// EOFコマンド
	class CSetEndCmd : public ICommand
	{
	public:	bool Exec(IWriter& fw) { fw.SetEnd(); return true; }
	};

	/// クローズコマンド
	class CCloseCmd : public ICommand
	{
	public:	bool Exec(IWriter& fw) { fw.Close(); return false; }
	};
};



}; // TNB
