#pragma once
/**
 *@file
 * MIDI関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"
#include "TnbMciPlayer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * MIDIメッセージクラス
 *
 *	@par必要ファイル
 *			TnbMidi.h
 *
 *	@date 10/05/25	新規作成
 */
class CMidiMessage : CCopyImpossible
{
public:

	/// コンストラクタ
	CMidiMessage(void) : m_hMidi(NULL), m_lastError(MMSYSERR_NOERROR)
	{
	}

	/// デストラクタ
	~CMidiMessage(void)
	{
		Close();
	}

	/**
	 * [設定] クローズ.
	 *	@note オープンしていない時は何もしません。
	 */
	void Close(void)
	{
		if ( m_hMidi != NULL )
		{
			::midiOutClose(m_hMidi);
			m_hMidi = NULL;
		}
	}

	/**
	 * [設定] オープン.
	 *	@param dev デバイス番号。 0～ GetDeviceCount() - 1 の範囲が指定可能。
	 *				省略するとデフォルトが選択されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Open(UINT dev = MIDI_MAPPER)
	{
		Close();
		return m_Rst(::midiOutOpen(&m_hMidi, dev, NULL, 0, CALLBACK_NULL));
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	bool IsOpened(void) const
	{
		return m_hMidi != NULL;
	}

	/**
	 * [設定] プログラムチェンジ.
	 *		音色を切替えます。
	 *	@param channel チャネル 0～15。
	 *	@param programNumber 音色。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool ProgramChange(BYTE channel, BYTE programNumber)
	{
		return m_Rst(channel, TParam(Midi_ProgramChange, channel, programNumber));
	}

	/**
	 * [設定] バンクセレクト.
	 *		音色のバンクを切替えます。
	 *	@note 本メソッドだけでは音色は変わりません。本メソッド後 ProgramChange() を使用します。
	 *	@param channel チャネル 0～15。
	 *	@param msb MSB.
	 *	@param lsb LSB.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool BankSelect(BYTE channel, BYTE msb, BYTE lsb)
	{
		CWorkMemT<TParam> vp(2);
		vp[0] = TParam(CCC_BackSelectMsb, channel, msb);
		vp[1] = TParam(CCC_BackSelectLsb, channel, lsb);
		return m_Rst(channel, vp);
	}

	/**
	 * [設定] モジュレーション.
	 *		音にビブラートを加える
	 *	@param channel チャネル 0～15。
	 *	@param modulation ビブラートの強さ。０がなし。127が最大。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Modulation(BYTE channel, BYTE modulation)
	{
		ASSERT ( modulation <= 127 );
		return m_Rst(channel, TParam(CCC_Modulation, channel, modulation));
	}

	/**
	 * [設定] ボリューム.
	 *		音量を調節する。
	 *	@note 慣例で足るなチャンネルの音量制御用に使います。
	 *	@param channel チャネル 0～15。
	 *	@param volume 音量。 0 が無音。 127 が最大。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Volume(BYTE channel, int volume)
	{
		ASSERT ( volume >= 0 && volume <= 127 );
		return m_Rst(channel, TParam(CCC_Volume, channel, down_cast<BYTE>(volume)));
	}

	/**
	 * [設定] パンポット.
	 *		音の出力先を左右に割り振ります。
	 *	@param channel チャネル 0～15。
	 *	@param pan 0 が中央。左端が -63。右端が 63。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Panpot(BYTE channel, int pan)
	{
		if ( pan < -63 )
		{
			pan = -63;
		}
		else if ( pan > 63 )
		{
			pan = 63;
		}
		return m_Rst(channel, TParam(CCC_Panpot, channel, down_cast<BYTE>(pan + 64)));
	}

	/**
	 * [設定] エクスプレッション.
	 *		音量を調節する。
	 *	@note 発音中に変化させることが出来る。
	 *	@param channel チャネル 0～15。
	 *	@param volume 音量。 0 が無音。 127 が最大。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Expression(BYTE channel, int volume)
	{
		ASSERT ( volume >= 0 && volume <= 127 );
		return m_Rst(channel, TParam(CCC_Expression, channel, down_cast<BYTE>(volume)));
	}

	/**
	 * [設定] リバーブセンドレベル.
	 *		音響レベルを指定する。
	 *	@note 音源によっては、機能しない。
	 *	@param channel チャネル 0～15。
	 *	@param level レベル。 0 が効果なし。 127 が効果最大。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool ReverbSendLevel(BYTE channel, int level)
	{
		ASSERT ( level >= 0 && level <= 127 );
		return m_Rst(channel, TParam(CCC_ReverbSendLevel, channel, down_cast<BYTE>(level)));
	}

	/**
	 * [設定] コーラスセンドレベル.
	 *		コーラスレベルを指定する。
	 *	@note 音源によっては、機能しない。
	 *	@param channel チャネル 0～15。
	 *	@param level レベル。 0 が効果なし。 127 が効果最大。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool ChorusSendLevel(BYTE channel, int level)
	{
		ASSERT ( level >= 0 && level <= 127 );
		return m_Rst(channel, TParam(CCC_ChorusSendLevel, channel, down_cast<BYTE>(level)));
	}

	/**
	 * [設定] コントロールセンドレベル.
	 *		指定のコントロール番号のレベルを指定する。
	 *	@note 音源によっては、機能しない。
	 *	@param channel チャネル 0～15。
	 *	@param controlNo コントロール番号。例えばリバーブが 0x5B, コーラスなら 0x5D を指定する。
	 *	@param level レベル。 0 が効果なし。 127 が効果最大。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool ControlSendLevel(BYTE channel, BYTE controlNo, int level)
	{
		ASSERT ( level >= 0 && level <= 127 );
		return m_Rst(channel, TParam(ECtrlChangeCode(controlNo), channel, down_cast<BYTE>(level)));
	}

	/**
	 * [設定] ノートオン.
	 *		鍵盤を押す動作です。
	 *	@param channel チャネル 0～15。
	 *	@param note ノート番号 0～127。音階。ピアノの中央のドが 60。
	 *	@param velocity 押す強さ 0～127。\n値の目安\n
	 *		ppp=16, pp=32, p=48, mp=64, mf=80, f=96, ff=112, fff=127
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool NoteOn(BYTE channel, BYTE note, BYTE velocity = 0)
	{
		ASSERT ( velocity <= 127 );
		return m_Rst(channel, TParam(Midi_NoteOn, channel, note, velocity));
	}

	/**
	 * [設定] ノートオフ.
	 *		鍵盤を離す動作です。
	 *	@param channel チャネル 0～15。
	 *	@param note ノート番号 0～127。音階。ピアノの中央のドが 60。
	 *	@param velocity 離す強さ 0～127。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool NoteOff(BYTE channel, BYTE note, BYTE velocity = 0)
	{
		ASSERT ( velocity <= 127 );
		return m_Rst(channel, TParam(Midi_NoteOff, channel, note));
	}

	/**
	 * [設定] ピッチベンドチェンジ.
	 *		鍵盤を離す動作です。
	 *	@param channel チャネル 0～15。
	 *	@param pitchBend ピッチベンド。 -8192 ～ 8181 。0がセンター。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool PitchBendChange(BYTE channel, int pitchBend = 0)
	{
		if ( pitchBend < -8192 )
		{
			pitchBend = -8192;
		}
		else if ( pitchBend > 8181 )
		{
			pitchBend = 8181;
		}
		int b = pitchBend + 8192;
		BYTE msb = down_cast<BYTE>((b >> 7) & 0x7F);
		BYTE lsb = down_cast<BYTE>((b     ) & 0x7F);
		return m_Rst(channel, TParam(Midi_PitchBendChange, channel, lsb, msb));
	}

	/**
	 * [設定] ピッチベンドセンシビティ.
	 *		ピッチベンドの範囲を設定します。
	 *	@param channel チャネル 0～15。
	 *	@param v 範囲。 12 で１オクターブ。値の最大は 127 。しかし通常、最大値は 24。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool PitchBendSensitivity(BYTE channel, BYTE v)
	{
		ASSERT ( v <= 127 );
		CWorkMemT<TParam> vp(6);
		// ピッチベンドセンシビティ
		vp[0] = TParam(CCC_RpnMsb, channel, 0);
		vp[1] = TParam(CCC_RpnLsb, channel, 0);
		// データエントリ
		vp[2] = TParam(CCC_DataEntryMsb, channel, v);
		vp[3] = TParam(CCC_DataEntryLsb, channel, 0);
		//// 終端
		vp[4] = TParam(CCC_RpnMsb, channel, 0x7F);
		vp[5] = TParam(CCC_RpnLsb, channel, 0x7F);
		return m_Rst(channel, vp);
	}

#if 0
	bool OutMessage(size_t len, LPCVOID pMessage)
	{
		MIDIHDR hdr = { 0 };
		CWorkMemT<char> mm;
		mm.Reset(len, static_cast<const char*>(pMessage));
		hdr.lpData = mm;
		hdr.dwBufferLength = len;
		hdr.dwBytesRecorded = len;
		if ( ! m_Rst(::midiOutPrepareHeader(m_hMidi, &hdr, sizeof(MIDIHDR))) )
		{
			return false;
		}
		if ( ! m_Rst(::midiOutLongMsg(m_hMidi, &hdr, sizeof(MIDIHDR))) )
		{
			return false;
		}
		/* GMリセット完了待機 */
		while ( (hdr.dwFlags & MHDR_DONE) == 0 )
		{
			::Sleep(0);
		}
		return m_Rst(::midiOutUnprepareHeader(m_hMidi, &hdr, sizeof(MIDIHDR)));
	}
#endif

	/**
	 * [取得] ラストエラーコード取得.
	 *	@return エラーコード. GetErrorString() で文字列に変換することが可能です。
	 */
	DWORD GetLastError(void) const
	{
		return m_lastError;
	}

	/**
	 * [設定] ラストエラーコード設定.
	 *	@param e エラーコード. 
	 */
	void SetLastError(DWORD e)
	{
		m_lastError = e;
	}

	/**
	 * [取得] デバイス数取得.
	 *		接続している MIDIのデバイス数を返します。
	 *	@return デバイス数。
	 */
	static UINT GetDeviceCount(void)
	{
		return ::midiOutGetNumDevs();
	}

	/**
	 * [取得] エラー文字列取得.
	 *	@param e エラーコード. 
	 *	@return 文字列。
	 */
	static CStr GetErrorString(DWORD e)
	{
		CStr s;
		if ( e != MMSYSERR_NOERROR )
		{
			::midiOutGetErrorText(e, s.GetBuffer(1000), 1000);
			s.ReleaseBuffer();
		}
		return s;
	}

private:

	/// メッセージ種
	enum EMidiMessageKind
	{
		Midi_NoteOn				= 0x9,	///< ノートオン, d1=Note#, d2=Velocity
		Midi_NoteOff			= 0x8,	///< ノートオフ, d1=Note#, d2=Verocity
		Midi_KeyPressure		= 0xA,	///< ポリフォニックキープレッシャー, d1=Note#, d2=プレッシャー値
		Midi_CtrlChange			= 0xB,	///< コントロールチェンジ
		Midi_ProgramChange		= 0xC,	///< プログラム変更, d1=Program#, d2=未使用
		Midi_ChannelPressure	= 0xD,	///< チャネルプレッシャー, d1=プレッシャー値, d2=未使用
		Midi_PitchBendChange	= 0xE,	///< ピッチベンド, d1=ベンド値(LSM), d2=同(MSB)
	};

	/// コントロールチェンジコード
	enum ECtrlChangeCode
	{
		CCC_BackSelectMsb		= 0x00,	///< バンクセレクト(MSB)
		CCC_BackSelectLsb		= 0x20,	///< バンクセレクト(LSB)
		CCC_Modulation			= 0x01,	///< モジュレーション
		CCC_Volume				= 0x07,	///< ボリューム
		CCC_Panpot				= 0x0A,	///< パンポット
		CCC_Expression			= 0x0B,	///< エクスプレッション
		CCC_DataEntryMsb		= 0x06,	///< データエントリ(MSB)
		CCC_DataEntryLsb		= 0x26,	///< データエントリ(LSB)
		CCC_RpnMsb				= 0x65,	///< RegisteredParameterNumber(MSB)
		CCC_RpnLsb				= 0x64,	///< RegisteredParameterNumber(LSB)
		CCC_ReverbSendLevel		= 0x5B,	///< リバーブセンドレベル
		CCC_TremoloDepth		= 0x5C,	///< トレモロ効果
		CCC_ChorusSendLevel		= 0x5D,	///< コーラスセンドレベル
		CCC_VarSendLevel		= 0x5E,	///< バリエーションセンドレベル(SC88以降はディレイ)
	};

	/// メッセージパラメータ
	struct TParam
	{
		union {
			struct {
				union {
					struct {
						BYTE	channel:4;	///< チャネル
						BYTE	kind:4;		///< 種類
					};
					BYTE	state;			///< ステータスバイト
				};
				BYTE	data1;				///< データバイト１
				BYTE	data2;				///< データバイト２
				BYTE	padding;			///< (空)
			};
			DWORD msg;						///< メッセージ
		};
		TParam(EMidiMessageKind k, BYTE ch = 0, BYTE d1 = 0, BYTE d2 = 0)
			: padding(0), kind(down_cast<BYTE>(k)), channel(ch), data1(d1), data2(d2)
		{
		}
		TParam(ECtrlChangeCode ccc, BYTE ch = 0, BYTE d = 0)
			: padding(0), kind(down_cast<BYTE>(Midi_CtrlChange)), channel(ch), data1(down_cast<BYTE>(ccc)), data2(d)
		{
		}
		TParam(void) : msg(0)
		{
		}
	};
	friend struct CMidiMessage::TParam;
	C_ASSERT( sizeof(TParam) == sizeof(DWORD) );
	///
	/// 戻り値処理
	bool m_RstFx(MMRESULT r)
	{
		m_lastError = r;
		return r == MMSYSERR_NOERROR;
	}
	/// 戻り値処理
	bool m_Rst(MMRESULT r)
	{
		m_lastError = r;
		if ( r != MMSYSERR_NOERROR )
		{
			TRACE2("MMRESULT = %d[%s]\n", r, GetErrorString(r));
		}
		return r == MMSYSERR_NOERROR;
	}
	/// メッセージ送信
	bool m_Rst(BYTE channel, const TParam& p)
	{
		if ( channel > 15 )
		{
			return m_RstFx(MMSYSERR_BADERRNUM);
		}
		TTRACE3( "MIDI msg = %02X %02X %02X\n", p.state, p.data1, p.data2 );
		return m_Rst(::midiOutShortMsg(m_hMidi, p.msg));
	}
	/// メッセージ送信
	bool m_Rst(BYTE channel, const CWorkMemT<TParam>& vp)
	{
		loop ( i, vp.GetSize() )
		{
			if ( ! m_Rst(channel, vp[i]) )
			{
				return false;
			}
		}
		return true;
	}
	HMIDIOUT			m_hMidi;		///< ハンドル
	mutable MMRESULT	m_lastError;	///< 最終エラー
};



}; //TNB