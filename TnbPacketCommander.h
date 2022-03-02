#pragma once
/**
 *@file
 * パケットコマンド通信関係のヘッダ
 *
 *		シーケンシャルデータの生成処理と、シーケンシャルデータの解析処理を実装することで、
 *		同期コマンド通信が完成します。
 *
 *	@note	マクロ "_TnbPACKET_LittleEndian" が定義されているなら、
 *			TPacDword , TPacWord のメモリイメージを「リトルエンディアン」とします。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"
#include "TnbCollectionAdapter.h"
#include "TnbQueueingReporter.h"
#include "TnbSet.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
  #ifdef _TnbPACKET_LittleEndian
	#pragma message("message : TPacWord/TPacDword is Little Endian.")
  #else
	#pragma message("message : TPacWord/TPacDword is Big Endian.")
  #endif
#endif



namespace TNB 
{



#pragma pack(push)
#pragma pack(1)



/**@ingroup REPORT
 * パケット用 DWORD 変数型.
 *		ビッグエンディアンのメモリイメージで DWORD を保持します。
 *		ただしマクロ "_TnbPACKET_LittleEndian" が定義されているなら、
 *		リトルエンディアンで保持します。
 *		アクセサで、エンディアン変換をサポートしますので、意識する必要はありません。
 *	@note union 対策でコンストラクタを持っていません。
 *	@note 内部では BYTE の配列なので、奇数アドレスに配置されても問題ありません。
 *
 *	@par必要ファイル
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	新規作成
 *	@date 11/06/23	メンバを隠蔽にし、BYTEの配列にした。
 *	@date 12/02/14	Set() 追加。
 */
struct TPacDword
{
	typedef DWORD TYPE;		///< 型名

	/**
	 * [代入] 代入.
	 *	@param v 設定する値
	 *	@return 自分の参照.
	 */
	TPacDword& operator=(DWORD v)
	{
		Set(v);
		return *this;
	}

	/**
	 * [代入] 代入.
	 *	@param v 設定する値
	 */
	void Set(DWORD v)
	{
		#ifdef _TnbPACKET_LittleEndian
			m_dat[3] = static_cast<BYTE>(v >> 24);
			m_dat[2] = static_cast<BYTE>(v >> 16);
			m_dat[1] = static_cast<BYTE>(v >> 8);
			m_dat[0] = static_cast<BYTE>(v);
		#else
			m_dat[0] = static_cast<BYTE>(v >> 24);
			m_dat[1] = static_cast<BYTE>(v >> 16);
			m_dat[2] = static_cast<BYTE>(v >> 8);
			m_dat[3] = static_cast<BYTE>(v);
		#endif
	}

	/**
	 * [取得] ゲッタ
	 *	@return 保持している値
	 */
	operator DWORD(void) const
	{
		return Get();
	}

	/**
	 * [取得] ゲッタ
	 *	@return 保持している値
	 */
	DWORD Get(void) const
	{
		DWORD r;
		#ifdef _TnbPACKET_LittleEndian
			r = (m_dat[3] << 24) + (m_dat[2] << 16) + (m_dat[1] << 8) + (m_dat[0]);
		#else
			r = (m_dat[0] << 24) + (m_dat[1] << 16) + (m_dat[2] << 8) + (m_dat[3]);
		#endif
		return r;
	}

	/**
	 * [取得] 文字列化.
	 *	@return 文字列
	 */
	CStr ToString(void) const
	{
		return CStr::DwordToString(Get());
	}

private:
	BYTE m_dat[4];				///< 値
};



/**@ingroup REPORT
 * パケット用 WORD 変数型.
 *		ビッグエンディアンのメモリイメージで WORD を保持します。
 *		ただしマクロ "_TnbPACKET_LittleEndian" が定義されているなら、
 *		リトルエンディアンで保持します。
 *		アクセサで、エンディアン変換をサポートしますので、意識する必要はありません。
 *	@note union 対策でコンストラクタを持っていません。
 *	@note 内部では BYTE の配列なので、奇数アドレスに配置されても問題ありません。
 *
 *	@par必要ファイル
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	新規作成
 *	@date 11/06/23	メンバを隠蔽にし、BYTEの配列にした。
 *	@date 12/02/14	Set() 追加。
 */
struct TPacWord
{
	typedef WORD TYPE;		///< 型名

	/**
	 * [代入] 代入.
	 *	@param v 設定する値
	 *	@return 自分の参照.
	 */
	TPacWord& operator=(WORD v)
	{
		Set(v);
		return *this;
	}

	/**
	 * [代入] 代入.
	 *	@param v 設定する値
	 */
	void Set(WORD v)
	{
		#ifdef _TnbPACKET_LittleEndian
			m_dat[1] = static_cast<BYTE>(v >> 8);
			m_dat[0] = static_cast<BYTE>(v);
		#else
			m_dat[0] = static_cast<BYTE>(v >> 8);
			m_dat[1] = static_cast<BYTE>(v);
		#endif
	}

	/**
	 * [取得] ゲッタ
	 *	@return 保持している値
	 */
	operator WORD(void) const
	{
		return Get();
	}

	/**
	 * [取得] ゲッタ
	 *	@return 保持している値
	 */
	WORD Get(void) const
	{
		WORD r;
		#ifdef _TnbPACKET_LittleEndian
			r = static_cast<WORD>((m_dat[1] << 8) + (m_dat[0]));
		#else
			r = static_cast<WORD>((m_dat[0] << 8) + (m_dat[1]));
		#endif
		return r;
	}

	/**
	 * [取得] 文字列化.
	 *	@return 文字列
	 */
	CStr ToString(void) const
	{
		return CStr::DwordToString(Get());
	}

private:
	BYTE m_dat[2];				///< 値
};



/**@ingroup REPORT
 * パケット用 BYTE 変数型.
 *		TPacWord , TPacDword と親和性を持たせるための型です。
 *	@note union 対策でコンストラクタを持っていません。
 *
 *	@par必要ファイル
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	新規作成
 *	@date 11/06/23	メンバを隠蔽にした。
 *	@date 12/02/14	Set() 追加。
 */
struct TPacByte
{
	typedef BYTE TYPE;		///< 型名

	/**
	 * [代入] 代入.
	 *	@param v 設定する値
	 *	@return 自分の参照.
	 */
	TPacByte& operator=(BYTE v)
	{
		m_dat = v;
		return *this;
	}

	/**
	 * [代入] 代入.
	 *	@param v 設定する値
	 */
	void Set(BYTE v)
	{
		m_dat = v;
	}

	/**
	 * [取得] ゲッタ
	 *	@return 保持している値
	 */
	operator BYTE(void) const
	{
		return Get();
	}

	/**
	 * [取得] ゲッタ
	 *	@return 保持している値
	 */
	BYTE Get(void) const
	{
		return m_dat;
	}

	/**
	 * [取得] 文字列化.
	 *	@return 文字列
	 */
	CStr ToString(void) const
	{
		return CStr::DwordToString(Get());
	}

	/**
	 * [参照] 参照
	 *	@return 保持している値の参照
	 */
	BYTE& Ref(void)
	{
		return m_dat;
	}

private:
	BYTE m_dat;			///< 値
};



#pragma pack(pop)



/**@ingroup REPORT
 * パケットコマンド実行インターフェース.
 *		コマンド送信～応答待ちまでをサポートするインターフェースです。
 *		タイムアウト時間は、 ExecuteCommand() メソッドで指定可能です。
 *
 *	@par必要ファイル
 *			TnbPacketCommander.h
 * 
 *	@date 11/02/03	新規、インターフェース化
 *	@date 12/02/29	SendCommand() -> SendPacket() 
 */
struct IPacketCommandExecuter
{
	/**@ingroup REPORT
	 * パケットインターフェース.
	 *		シーケンシャルデータを作成したり、解析したり、文字列化するメソッドを持ちます。
	 *
	 *	@par必要ファイル
	 *			TnbPacketCommander.h
	 * 
	 *	@date 10/08/24	新規作成.
	 *	@date 12/03/22	GetPacket() -> Serialize(), SetPacket() -> Deserialize() に変更。
	 */
	struct IPacket
	{
		/// デストラクタ
		virtual ~IPacket(void) {}

		/**
		 * [取得] シリアライズ.
		 *		本インスタンスで保持している情報から、シーケンシャルデータを作成します。
		 *	@return シーケンシャルデータ.
		 */
		virtual CByteVector Serialize(void) const = 0;

		/**
		 * [設定] デシリアライズ.
		 *		指定のシーケンシャルデータを解析し、パケットを取り出し本インスタンスに格納します。
		 *	@param c データ
		 *	@retval 0				パケット判断できるだけの長さがない
		 *	@retval	0未満			パケット解析エラー。
		 *	@retval 1以上			パケット解析成功。値は解析に使用したデータ長さ。
		 */
		virtual int Deserialize(const IConstCollectionT<BYTE>& c) = 0;

		/**
		 * [取得] 文字列化.
		 *		コマンド名、パラメータを文字列化します。
		 *	@return 文字列。任意ですが、通常[0]はコマンド名。以降はパラメータとします。
		 */
		virtual CStrVector ToStrings(void) const = 0;
	};

	/// デストラクタ
	virtual ~IPacketCommandExecuter(void) {}

	/**
	 * [実行] パケット送信.
	 *	@param[in] packet パケット。
	 *	@retval 0以上 成功。
	 *	@retval 0未満 エラー。
	 */
	virtual int SendPacket(const IPacket& packet) = 0;

	/**
	 * [実行] コマンド実行
	 *	@note 応答があるか、タイムアウトするまで関数はリターンしません。
	 *	@param[out] _response レスポンス型。成功時、結果が格納されます。
	 *	@param[in] command コマンド型。
	 *	@param[in] timeout レスポンスのタイムアウト時間(ms).
	 *	@retval 0以上 成功。
	 *	@retval 0未満 その他のエラー(主に IPacket::Deserialize() の戻り値)。
	 */
	virtual int ExecuteCommand(IPacket& _response, const IPacket& command, DWORD timeout = 500) = 0;
};



/**@ingroup REPORT
 * パケットコマンダー.
 *		コマンド送信～応答待ちまでをサポートしています。
 *		タイムアウト時間は、 ExecuteCommand() メソッドで指定可能です。
 *	
 *	- まず、パケットの形を IPacketCommandExecuter::IPacket を実装して作成します(ex;CBasicPacket)。
 *		これは、 STX～ETX や改行区切りなど、BYTE 列を解析する機能とパケットを作成する機能を
 *		持つものです。
 *		\code
 *			//
 *			// パケットフォーマット型.
 *			//	
 *			//		パケットの形のみを管理する。
 *			//		@note STX[1], LEN[1], CMD[1], DAT[LEN-2], BCC[1], ETX[1]\n
 *			//			- BCCはLEN～DATの合計
 *			//			コマンドコード種、データの中身、長さは、チェックしない。
 *			//
 *			class CPacketFormat : public TNB::CPacketCommander::IPacket
 *			{
 *			public:
 *				// SetData() エラーコード
 *				enum
 *				{
 *					NotFoundStx			= -100,		// STX が見つからない
 *					NotFoundEtx			= -101,		// ETX が見つからない
 *					InvalidBcc			= -103,		// BCC異常
 *					InvalidParameter	= -104,		// パラメータ異常
 *				};
 *				//
 *				// コンストラクタ.
 *				//	@param cc コマンドコード。省略すると０。
 *				//
 *				CPacketFormat(BYTE cc = 0) : m_command(cc);
 *				//
 *				// [設定] コマンドコード設定
 *				//	@param cc コマンドコード
 *				//
 *				void SetCommand(BYTE cc);
 *				//
 *				// [取得] コマンドコード取得
 *				//	@return コマンドコード
 *				//
 *				BYTE GetCommand(void) const;
 *				//
 *				// [取得] データ取得.
 *				//	@return データ
 *				//
 *				const CByteVector& GetData(void) const;
 *				//
 *				// [設定] データ設定.
 *				//	@param vb データ
 *				//	@retval 0			成功
 *				//	@retval	マイナス	エラー。
 *				//
 *				int SetDataV(const CByteVector& vb);
 *				//
 *				// [設定] データ設定.
 *				//	@param len データ長
 *				//	@param P データ
 *				//	@retval 0			成功
 *				//	@retval	マイナス	エラー。
 *				//
 *				virtual int SetData(size_t len, LPCVOID P);
 *				//
 *				// [取得] シリアライズ.
 *				//		本インスタンスで保持している情報から、シーケンシャルデータを作成します。
 * 				//	@return シーケンシャルデータ.
 *				//
 *				virtual CByteVector Serialize(void) const;
 *				//
 *				// [設定] デシリアライズ.
 *				//		指定のシーケンシャルデータを解析し、パケットを取り出し本インスタンスに格納します。
 *				//	@param c データ
 *				//	@retval 0				パケット判断できるだけの長さがない
 *				//	@retval	0未満			パケット解析エラー。
 *				//	@retval 1以上			パケット解析成功。値は解析に使用したデータ長さ。
 *				//
 *				virtual int Deserialize(const IConstCollectionT<BYTE>& c);
 *				//
 *				// [取得] 文字列化.
 *				//		コマンド名、パラメータを文字列化します。
 *				//	@return 文字列。任意ですが、通常[0]はコマンド名。以降はパラメータとします。
 *				//
 *				virtual CStrVector ToStrings(void) const;
 *			protected:
 *				enum
 *				{
 *					STX = 0x02,		// STX
 *					ETX = 0x03,		// ETX
 *				};
 *				BYTE		m_command;			// コマンド
 *				CByteVector	m_data;				// データ
 *				friend class CPacketTest;
 *			}; 
 *		\endcode
 *	- それのインスタンスを SetPacketFormat() に渡します。
 *	- 更に IPacket を実装した個々のコマンドごとに管理パケットクラスを
 *		作成します(ex; CVersionCommand )。
 *		このクラスは、決められたコマンドコードなど、正しくなければ、
 *		解析でエラーを返すようにします。
 *		\n以下は個々のコマンドの抽象クラス例。必要に応じて継承し AnalyzeData をオーバーロードする。
 *		\code
 *			//
 *			// 抽象パケット管理クラス.
 *			//
 *			class CAbstractPacket : public CPacketFormat
 *			{
 *				DEFSUPER(CPacketFormat);
 *			public:
 *				// SetData() エラーコード
 *				enum
 *				{
 *					UnknownCommandCode	= -201,		// コマンドコード不明
 *				};
 *				//
 *				// [設定] パケット設定.
 *				//	@param bp パケット情報
 *				//	@retval 0			成功
 *				//	@retval	マイナス	パケット解析エラー。
 *				//
 *				int SetPacket(const CBasicPacket& bp);
 *				//
 *				// [設定] デシリアライズ.
 *				//		指定のシーケンシャルデータを解析し、パケットを取り出し本インスタンスに格納します。
 *				//	@param c データ
 *				//	@retval 0				パケット判断できるだけの長さがない
 *				//	@retval	0未満			パケット解析エラー。
 *				//	@retval 1以上			パケット解析成功。値は解析に使用したデータ長さ。
 *				//
 *				virtual int Deserialize(const IConstCollectionT<BYTE>& c);
 *				//
 *				// [設定] データ設定.
 *				//	@param len データ長
 *				//	@param P データ
 *				//	@retval 0				データ部解析成功
 *				//	@retval	マイナス		データ部解析エラー。
 *				//
 *				virtual int SetData(size_t len, LPCVOID P);
 *				//
 *				// [取得] 文字列化.
 *				//		コマンド名、パラメータを文字列化します。
 *				//	@return 文字列。任意ですが、通常[0]はコマンド名。以降はパラメータとします。
 *				//
 *				virtual CStrVector ToStrings(void) const;
 *				//
 *				// [取得] コマンド名取得.
 *				//	@return コマンド名.
 *				//
 *				CStr GetCommandName(void) const;
 *				//
 *				// [取得] リザルト値取得.
 *				//	@return リザルト値.
 *				//
 *				virtual BYTE GetResult(void) const;
 *			protected:
 *				//
 *				// 初期化.
 *				//	@param cc コマンドコード。
 *				//	@param lpszName コマンド名.
 *				//
 *				void Init(BYTE cc, LPCTSTR lpszName);
 *				CStr	m_commandName;	// コマンド名
 *			private:
 *				void SetCommand(BYTE cc);
 *			};
 *
 *
 *			// バージョン確認コマンド
 *			class CVersionCommand : public CAbstractPacket
 *			{
 *			public:
 *				enum { CC = 0x2A };		// コマンドコード
 *				// コンストラクタ
 *				CVersionCommand(void)
 *				{
 *					Init(CC, _T("バージョン確認コマンド"));
 *				}
 *			};
 *
 *
 *			// バージョン確認レスポンスデータ
 *			struct TVersionData
 *			{
 *				TPacByte	result;			// 0x00; 成功 0x01; パラメータエラー
 *				char		name[13];		// 名前
 *				TPacByte	version[4];		// バージョン
 *				//
 *				// [取得] 文字列化
 *				//	@return 文字列
 *				//
 *				CStrVector ToStrings(void) const;
 *			};
 *
 *			/// バージョン確認レスポンス
 *			class CVersionResponse : public CAbstractParamPacketT<TVersionData>
 *			{
 *			public:
 *				enum { CC = 0xAA };		// コマンドコード
 *				// コンストラクタ
 *				CVersionResponse(void)
 *				{
 *					Init(CC, _T("バージョン確認レスポンス"));
 *				}
 *				//
 *				// [設定] パラメータ設定.
 *				//	@param prm パラメータ.
 *				//
 *				virtual void SetParameter(const TParam& prm);
 *				{
 *					m_param = prm;
 *					int cnt = 1;
 *					if ( m_param.result == 0 )
 *					{
 *						cnt = sizeof(TFwVersionData);
 *					}
 *					m_data.SetElements(cnt , reinterpret_cast<const BYTE*>(&m_param));
 *				}
 *				// リザルト取得
 *				virtual BYTE GetResult(void) const
 *				{
 *					return GetParameter().result;
 *				}
 *			protected:
 *				//
 *				// [解析] データ部解析.
 *				//	@param len データ長さ
 *				//	@param P データ.
 *				//	@retval 0				データ部解析成功
 *				//	@retval	マイナス		データ部解析エラー。
 *				//
 *				virtual int SetData(size_t len, LPCVOID P)
 *				{
 *					m_data.SetElements(len, static_cast<const BYTE*>(P));
 *					if ( len >= 1 )
 *					{
 *						Zero(m_param);
 *						const BYTE* B = static_cast<const BYTE*>(P);
 *						BYTE r = B[0]; // result;
 *						size_t l = (r == 0) ? sizeof(TFwVersionData) : 1;
 *						if ( l == len )
 *						{
 *							memcpy(&m_param, P, l);
 *							return 0;
 *						}
 *					}
 *					return InvalidParameter;
 *				}
 *			};
 *
 *		\endcode
 *	- ExecuteCommand() で、送信するパケットと、期待する受信パケットを引数にコールする。
 *		\code
 *			CVersionCommand pc;
 *			CVersionResponse pr;
 *			m_commander.ExecuteCommand(pr, pc, 1000);
 *		\endcode
 *		成功すると、 pr に受信した内容が入っている。
 *
 *	@par必要ファイル
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	新規作成
 *	@date 11/01/18	大幅改築
 *	@date 11/03/04	ExecuteCommand() の応答待ち中のキャンセルを追加
 *	@date 11/07/05	SendRawData() , SetLogger() 追加。 ILogger 、追加。
 *	@date 11/07/06	NotOpen を追加。
 *	@date 11/07/06	SetLogger() ILogger 、廃止。
 *	@date 11/07/13	SetAliveTime() 追加。
 *	@date 11/09/20	タイムアウト等で破棄されたデータもきちんとリスナーに上げるように修正。
 *	@date 12/02/29	SendCommand() -> SendPacket() 。 SetBasicPacket() -> SetPacketFormat()
 *	@date 12/03/23	IListener::OnCommanderReceivedExpectationPacket() に対応。
 *	@date 14/05/16	IsConnect() 、追加。
 *	@date 15/02/20	SetTearminalTime() 追加。
 *	@date 15/03/18	ExecuteCommand() を分解したメソッド（Start, Wait, Stop）を追加。
 *	@date 18/09/22	PurgeQueue() 、追加。
 */
class CPacketCommander : public IPacketCommandExecuter, CCopyImpossible
{
public:

	/// 失敗種類
	enum EFailure
	{
		Timeout			= -1,	///< タイムアウト
		SendError		= -2,	///< 送信エラー
		Canceled		= -3,	///< キャンセル
		NotOpen			= -4,	///< オープンしてない
	};

	/**@ingroup REPORT
	 * パケットコマンダーのリスナー
	 *
	 *		CPacketCommander::SetListener() で登録することで、受信を確認することが出来ます。
	 *
	 *	@par必要ファイル
	 *			TnbPacketCommander.h
	 * 
	 *	@date 10/08/24	新規作成
	 *	@date 12/03/23	OnCommanderReceivedExpectationPacket() 追加。
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] パケット受信.
		 *		解析成功したパケットが通知されます。
		 *	@note 応答待ち対象のパケットは通知されません。
		 *	@param pPacket 受信したパケット。 CPacketCommander::SetPacketFormat で渡したオブジェクトそのものが必ず通知されます。
		 *	@param result 解析結果（０以下）。応答待ちでない場合、常に０になります。
		 */
		virtual void OnCommanderReceivedPacket(const IPacket* pPacket, int result) = 0;

		/**
		 * [通知] 期待パケット受信.
		 *		応答待ち対象のパケットのみ通知されます。
		 *	@param pPacket 受信したパケット。 CPacketCommander::SetPacketFormat で渡したオブジェクトそのものが必ず通知されます。
		 */
		virtual void OnCommanderReceivedExpectationPacket(const IPacket* pPacket) {}

		/**
		 * [通知] ごみ受信.
		 *		解析成功したパケット以外のゴミデータが通知されます。
		 *	@param b データ
		 *	@param result ゴミ判断理由。主に IPacket のサブクラスの Deserialize() のエラーコード。
		 */
		virtual void OnCommanderDustyData(BYTE b, int result) = 0;

		/**
		 * [通知] イベント通知.
		 *	@param event イベント
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnReportEvent(EReportEvent event) = 0;
	};


	//-------------------------------------


	/**
	 *  コンストラクタ
	 *	@param iQueueSize 受信Byteキューのサイズ。省略すると10000です。
	 */
	CPacketCommander(int iQueueSize = 10000) : m_pRep(NULL), m_queue(iQueueSize)
	{
	}

	/// デストラクタ
	~CPacketCommander(void)
	{
		CPacketCommander::Close();
	}

	/**
	 * [設定] 解析用パケット指定.
	 *	@attention 本メソッドで指定したインスタンスは本クラス内で使用しますので、
	 *		操作することはしないでください。
	 *	@param pPacket パケット
	 */
	void SetPacketFormat(IPacketCommandExecuter::IPacket* pPacket)
	{
		m_in.m_pPacketFormat = pPacket;
	}

	/**
	 * [設定] 解析用パケット指定.
	 *	@param pPacket パケット
	 * 	@deprecated 廃止予定です。 SetPacketFormat() を使用してください。
	 */
	void _deprecated SetBasicPacket(IPacketCommandExecuter::IPacket* pPacket)
	{
		SetPacketFormat(pPacket);
	}

	/**
	 * [設定] リスナー設定.
	 *	@param pListener リスナー
	 */
	void SetListener(IListener* pListener)
	{
		m_in.m_pListener = pListener;
	}

	/**
	 * [設定] キュー有効時間設定.
	 *		キュー格納後、一定時間受信がない場合、キュー内容を破棄することが出来ます。
	 *	@note Open() 時、 キューを使うを指定してない場合、この設定は無意味です。
	 *	@note SetTearminalTime() と同時に指定できません。
	 *	@param time キューの有効時間(ms)。最後に受信して解析してから指定時間経過したら、キューの先頭の一バイトを破棄して、
	 *					再度解析をします。省略時は無効( DWORD_MAX )です。
	 */
	void SetAliveTime(DWORD time = DWORD_MAX)
	{
		m_queue.SetAliveTime(time);
	}

		/**
	 * [設定] 終端時間設定.
	 *		キュー格納後、一定時間受信がない場合にキュー内容を解析するようにします。
	 *	@note Open() 時、 キューを使うを指定してない場合、この設定は無意味です。
	 *	@note SetAliveTime() と同時に指定できません。
	 *	@param time 終端時間(ms)。
	 */
	void SetTearminalTime(DWORD time = DWORD_MAX)
	{
		m_queue.SetTearminalTime(time);
	}
	
	/**
	 * [取得] オープン確認.
	 *	@retval true オープン済み.
	 *	@retval false 未オープン.
	 */
	bool IsOpened(void) const
	{
		if ( m_pRep != NULL )
		{
			return m_pRep->IsAlive();
		}
		return false;
	}
	
	/**
	 * [取得] 接続確認.
	 *	@retval true 接続.
	 *	@retval false 切断.
	 */
	bool IsConnect(void) const
	{
		if ( m_pRep != NULL )
		{
			return m_pRep->IsConnect();
		}
		return false;
	}

	/**
	 * [設定] オープン.
	 *	@note 指定の レポータを使用してオープンします。
	 *	@note 解析用パケットを登録していないと失敗します。
	 *	@param pRep レポータ。クローズするまで破棄しないでください。
	 *	@param withQueueing ture ならキューを使います。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Open(IReport* pRep, bool withQueueing = false)
	{
		CPacketCommander::Close();
		if ( pRep != NULL && m_in.m_pPacketFormat != NULL )
		{
			m_pRep = pRep;
			if ( withQueueing )
			{
				m_queue.SetEnvironment(m_pRep, &m_in);
			}
			else
			{
				m_pRep->SetListener(&m_in);
			}
			return m_pRep->Start();
		}
		return false;
	}

	/**
	 * [設定] クローズ.
	 *	@note オープンしていなければ何もしません。
	 */
	virtual void Close(void)
	{
		if ( m_pRep != NULL )
		{
			m_pRep->Stop();
		}
		m_queue.ResetEnvironment();
		m_pRep = NULL;
	}

	/**
	 * [実行] パケット送信.
	 *	@note リスナーで受けたメソッドからも使用できます。
	 *	@param packet パケット
	 *	@retval 0以上 成功。
	 *	@retval SendError 送信エラー。
	 *	@retval NotOpen 未オープン。
	 */
	virtual int SendPacket(const IPacketCommandExecuter::IPacket& packet)
	{
		if ( IsOpened() )
		{
			ASSERT( m_pRep != NULL );
			// 送信コマンドデータ作成
			CByteVector vb = packet.Serialize();
			// 受信処理ロック
			m_in.m_sendSync.Lock();
			// 送信
			bool r = m_pRep->Send(vb.GetSize(), vb.ReferBuffer());
			// 受信処理アンロック
			m_in.m_sendSync.Unlock();
			if ( r )
			{
				return 0; //成功
			}
			return SendError; //送信エラー
		}
		return NotOpen;
	}

	/**
	 * [実行] パケット送信.
	 *	@param packet パケット
	 *	@retval 0以上 成功。
	 *	@retval SendError 送信エラー。
	 *	@retval NotOpen 未オープン。
	 * 	@deprecated 廃止予定です。 SendPacket() を使用してください。
	 */
	int _deprecated SendCommand(const IPacketCommandExecuter::IPacket& packet)
	{
		return SendPacket(packet);
	}

	/**
	 * [実行] コマンド実行.
	 *		コマンドパケットを送信し、応答があるか、タイムアウトするまで関数はリターンしません。
	 *	@note コマンドパケットに生成する Serialize で Empty を返すクラスを渡すと送信しないので、レスポンス待ちのみできます。
	 *	@param[out] _response レスポンス型。成功時、結果が格納されます。
	 *	@param[in] command コマンド型。
	 *	@param[in] timeout レスポンスのタイムアウト時間(ms).
	 *	@retval 0以上 成功。
	 *	@retval Timeout タイムアウト。
	 *	@retval SendError 送信エラー。
	 *	@retval NotOpen 未オープン。
	 *	@retval 0未満 その他のエラー(主に IPacketCommandExecuter::IPacket::Deserialize() の戻り値)。
	 */
	virtual int ExecuteCommand(IPacketCommandExecuter::IPacket& _response, const IPacketCommandExecuter::IPacket& command, DWORD timeout = 500)
	{
		int r = ExecuteCommand_Start(_response, command);
		if ( r >= 0 )
		{
			r = ExecuteCommand_Wait(_response, timeout);
			ExecuteCommand_Stop(_response);
		}
		return r;
	}

	/**
	 * [実行] コマンド実行(開始).
	 *		コマンドパケットを送信し、応答待ち準備をします。
	 *	@note 通常 ExecuteCommand() を使用し、本メソッドは使用しません。
	 *	@param[out] _response レスポンス型。成功時、結果が格納されます。
	 *	@param[in] command コマンド型。
	 *	@retval 0以上 成功。終了後、必ず ExecuteCommand_Stop() をコールしてください。
	 *	@retval Timeout タイムアウト。
	 *	@retval SendError 送信エラー。
	 *	@retval NotOpen 未オープン。
	 *	@retval 0未満 その他のエラー(主に IPacketCommandExecuter::IPacket::Deserialize() の戻り値)。
	 */
	int ExecuteCommand_Start(IPacketCommandExecuter::IPacket& _response,const IPacketCommandExecuter::IPacket& command)
	{
		if ( IsOpened() )
		{
			ASSERT( m_pRep != NULL );
			//== 準備
			bool r = true;
			// 送信コマンドデータ作成
			CByteVector vb = command.Serialize();
			// 新タスク
			m_in.NewTask(&_response);
			//== 送信
			if ( ! vb.IsEmpty() )
			{
				// 受信処理ロック
				m_in.m_sendSync.Lock();
				// 送信
				r = m_pRep->Send(vb.GetSize(), vb.ReferBuffer());
				// 受信処理アンロック
				m_in.m_sendSync.Unlock();
			}
			return r ? 1 : SendError;
		}
		return NotOpen;
	}

	/**
	 * [実行] コマンド実行(待ち).
	 *		応答があるか、タイムアウトするまで関数はリターンしません。
	 *	@note 通常 ExecuteCommand() を使用し、本メソッドは使用しません。
	 *	@param[out] _response レスポンス型。成功時、結果が格納されます。 ExecuteCommand_Start() と同じオブジェクトを使用してください。
	 *	@param[in] timeout レスポンスのタイムアウト時間(ms).
	 *	@retval 0以上 成功。
	 *	@retval Timeout タイムアウト。
	 *	@retval SendError 送信エラー（ExecuteCommand_Start() が成功していない）。
	 *	@retval NotOpen 未オープン。
	 *	@retval 0未満 その他のエラー(主に IPacketCommandExecuter::IPacket::Deserialize() の戻り値)。
	 */
	int ExecuteCommand_Wait(IPacketCommandExecuter::IPacket& _response, DWORD timeout = 500)
	{
		if ( IsOpened() )
		{
			// タスク
			CInner::TTask* pTask = m_in.FindTask(&_response);
			if ( pTask != NULL )
			{
				DWORD to = timeout;
				while ( true )
				{
					if ( pTask->recvEvent.Lock(to) )
					{
						int r = pTask->analyzedCode;
						m_in.EndTask(pTask);
						return r;
					}
					if ( ! pTask->isTimeoutExtend )
					{
						break;
					}
					pTask->isTimeoutExtend = false; //タイムアウト延長
					if ( to > 0 )
					{
						DWORD t = pTask->extendTime;
						if ( t == 0 )
						{
							t = timeout;
						}
						DWORD d = pTask->tick.GetPassedCount();
						if ( d > t )
						{
							to = 0;
						}
						else
						{
							to = t - d;
						}
					}
				}
				return Timeout; //タイムアウト
			}
			return SendError; //送信エラー(Startしてない)
		}
		return NotOpen;
	}

	/**
	 * [実行] コマンド実行(終了).
	 *		ExecuteCommand_Start() を使った場合、必ず本メソッドを使用してください。
	 *	@param response レスポンス型。ExecuteCommand_Start() と同じオブジェクトを使用してください。
	 */
	void ExecuteCommand_Stop(IPacketCommandExecuter::IPacket& response)
	{
		CInner::TTask* pTask = m_in.FindTask(&response);
		if ( pTask != NULL )
		{
			m_in.EndTask(pTask);
		}
	}

	/**
	 * [設定] タイムアウト延長.
	 *		コマンドの応答待ちを延長します。これをコールした時にタイムアウト時間
	 *		の再設定が行われます（例えば、タイムアウト5秒の設定の場合で、コマンド送信後、
	 *		2秒後に本メソッドをコールすると、全部で７秒間、レスポンスを待ちます）。
	 *	@note リスナー内や、別スレッドから使用します。
	 *	@param threadId スレッドID。省略すると、全てのタスクを延長。
	 */
	void ExtendTimeout(DWORD threadId = 0)
	{
		m_in.ExtendTimeout(0, threadId);
	}

	/**
	 * [設定] タイムアウト延長.
	 *		コマンドの応答待ちを指定時間、延長します。
	 *	@note リスナー内や、別スレッドから使用します。
	 *	@param time 延長する時間(ms)
	 *	@param threadId スレッドID。省略すると、全てのタスクを延長。
	 */
	void ExtendTimeoutEx(DWORD time, DWORD threadId = 0)
	{
		m_in.ExtendTimeout(time, threadId);
	}

	/**
	 * [設定] レスポンス待ちキャンセル.
	 *	@note ExecuteCommand() で応答待ちを、このメソッドでとめることができます。
	 *		その場合、 ExecuteCommand() の戻り値は {@link CPacketCommander::Canceled Canceled(-4)} になります。
	 *	@param threadId スレッドID。省略すると、全てのタスクをキャンセル。
	 */
	void CancelWaitResponse(DWORD threadId = 0)
	{
		m_in.CancelWaitResponse(threadId);
	}

	/**
	 * [送信] 生データ送信.
	 *		任意のデータを送信することが出来ます。
	 *	@param size サイズ
	 *	@param pData データ
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SendRawData(size_t size, const BYTE* pData)
	{
		bool r = false;
		if ( m_pRep != NULL )
		{
			// 受信処理ロック
			m_in.m_sendSync.Lock();
			// 送信
			r = m_pRep->Send(size, pData);
			// 受信処理アンロック
			m_in.m_sendSync.Unlock();
		}
		return r;
	}

	/**
	 * [取得] キュー空きサイズ取得.
	 *	@return 空きサイズ
	 */
	size_t GetQueueFreeSize(void) const
	{
		return m_queue.GetQueueFreeSize();
	}

	/**
	 * [設定] キュークリア
	 */
	void PurgeQueue(void)
	{
		m_queue.Purge();
	}

	/**
	 * [参照] レポーター参照.
	 *	@retval NULL 未使用
	 *	@retval NULL以外 使用しているレポーター。
	 */
	IReport* ReferReporter(void)
	{
		return IsOpened() ? m_pRep : NULL;
	}

private:

	/// 内部クラス
	class CInner : public IReport::IListener, public CQueueingReporter::IListener
	{
		DEFPARENTLISTENER(CPacketCommander, IParentListener);
	public:
		/// タスク
		struct TTask
		{
			DWORD				threadId;				///< スレッドID
			CSyncEvent			recvEvent;				///< 受信イベント。期待のレシーブがあればシグナルします。
			bool				isAnalyzed;				///< 期待パケット受信解析済み？
			IPacket*			pExpectationPacket;		///< 期待パケット
			bool				isTimeoutExtend;		///< 延長あり？
			DWORD				extendTime;				///< 延長時間(ms)
			CTickCount			tick;					///< 延長時間管理用
			int					analyzedCode;			///< 解析結果
			/// コンストラクタ
			TTask(void) : recvEvent(false, false), pExpectationPacket(NULL), isTimeoutExtend(false), extendTime(0), analyzedCode(0), isAnalyzed(true)
			{}
		};

		//-----

		/// コンストラクタ
		CInner(void) : m_pListener(NULL), m_pPacketFormat(NULL)
		{
		}
		void EndTask(TTask* p)
		{
			EXCLUSIVE( &m_task );
			if ( m_task.Delete(p) )
			{
//				p->recvEvent.Unlock();
				delete p;
			}
		}
		TTask* FindTask(IPacket* pExpectation)
		{
			loop ( i, m_task )
			{
				TTask* p = m_task.At(i);
				if ( p->pExpectationPacket == pExpectation )
				{
					return p;
				}
			}
			return NULL;
		}
		TTask* NewTask(IPacket* pExpectation)
		{
			TTask* p = new TTask;
			p->threadId = ::GetCurrentThreadId();
			p->pExpectationPacket = pExpectation;
			p->isAnalyzed = false;
			p->recvEvent.ToLock();
			p->isTimeoutExtend = false;
			p->extendTime = 0;
			ASSERT( ! m_task.Has(p) );
			m_task.InsertEx(p);
			return p;
		}
		void ExtendTimeout(DWORD time, DWORD threadId = 0)
		{
			if ( threadId == 0 )
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					p->tick.Reset();
					p->isTimeoutExtend = true;
					p->extendTime = time;
				}
			}
			else
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					if ( threadId == p->threadId )
					{
						p->tick.Reset();
						p->isTimeoutExtend = true;
						p->extendTime = time;
						break;
					}
				}
			}
		}
		/// キャンセル
		void CancelWaitResponse(DWORD threadId = 0)
		{
			if ( threadId == 0 )
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					p->analyzedCode = Canceled;
//					p->pExpectationPacket = NULL;
					p->isAnalyzed = true;
					p->recvEvent.Unlock();
				}
			}
			else
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					if ( threadId == p->threadId )
					{
						p->analyzedCode = Canceled;
//						p->pExpectationPacket = NULL;
						p->isAnalyzed = true;
						p->recvEvent.Unlock();
						break;
					}
				}
			}
		}
		/// [通知] イベント発生通知
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( ev.HasEvent() )
			{
				if ( ev.GetEvent() == ReportEvent_End )
				{
					CancelWaitResponse();
				}
				if ( m_pListener != NULL )
				{
					return m_pListener->OnReportEvent(ev.GetEvent());
				}
				return true;
			}
//			EXCLUSIVE( &m_sendSync );
			CConstAdapterT<BYTE> c(ev.GetDataSize(), ev.GetData());
			if ( Analyze(c) == 0 )
			{
				if ( m_pListener != NULL )
				{
					loop ( i, ev.GetDataSize() )
					{
						m_pListener->OnCommanderDustyData(ev.GetData()[i], Timeout);
					}
				}
			}
			return true;
		}
		/// [通知] データ受信通知
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
		{
//			EXCLUSIVE( &m_sendSync );
			size_t r = Analyze(c);
			if ( boIsLast && r == 0 )
			{
				r = c.GetSize();
				if ( m_pListener != NULL )
				{
					loop ( i, r )
					{
						m_pListener->OnCommanderDustyData(c.At(i), Timeout);
					}
				}
			}
			return r;
		}
		/// [通知] ごみ化通知
		virtual void OnReportDustyData(BYTE b)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnCommanderDustyData(b, Timeout);
			}
		}
		/// 解析
		size_t Analyze(const IConstCollectionT<BYTE>& c)
		{
			size_t size = 0;
			if ( m_pPacketFormat != NULL )
			{
				int a = m_pPacketFormat->Deserialize(c);
				if ( a > 0 )
				{
					size = a;
					a = 0;
					bool isDisp = false;
					{
						EXCLUSIVE( &m_task );
						loop ( i, m_task )
						{
							TTask* p = m_task.At(i);
//							if ( p->pExpectationPacket != NULL )
							if ( ! p->isAnalyzed )
							{
								a = p->pExpectationPacket->Deserialize(c);
								if ( a > 0 )
								{
									if ( m_pListener != NULL )
									{
										m_pListener->OnCommanderReceivedExpectationPacket(m_pPacketFormat);
									}
									p->analyzedCode = a;
//									p->pExpectationPacket = NULL;
									p->isAnalyzed = true;
									p->recvEvent.Unlock();
									isDisp = true;
									break;
								}
							}
						}
					}
					if ( ! isDisp && m_pListener != NULL )
					{
						m_pListener->OnCommanderReceivedPacket(m_pPacketFormat, a);
					}
				}
				else if ( a == 0 )
				{
				}
				else
				{
					size = 1;
					if ( m_pListener != NULL )
					{
						m_pListener->OnCommanderDustyData(c.At(0), a);
					}
				}
			}
			return size;
		}
		CSyncSection			m_sendSync;			///< 送信中排他
		IParentListener*		m_pListener;		///< リスナー
		IPacket*				m_pPacketFormat;	///< 解析用
		CSingleSetT<TTask*>		m_task;				///< タスク
	};
	CInner				m_in;				///< 内部クラス
	IReport*			m_pRep;				///< レポータ
	CQueueingReporter	m_queue;			///< キューイング
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 C_ASSERT ( sizeof(TPacDword) == 4 ); ///< サイズチェック
 C_ASSERT ( sizeof(TPacWord) == 2 ); ///< サイズチェック
 C_ASSERT ( sizeof(TPacByte) == 1 ); ///< サイズチェック
#endif



}; // TNB



