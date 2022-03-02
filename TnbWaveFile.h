#pragma once
/**
 *@file
 * WAVEファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <TnbMap.h>
#include <TnbConstFileMapping.h>
#include <TnbDummyCollection.h>
#include <TnbCollectionAdapter.h>
#include <mmsystem.h>



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * WAVEフォーマットクラス
 *	
 *		wavファイルをチェックし読み込むことが出来ます。	
 *
 *	@par必要ファイル
 *			TnbWaveFile.h
 *
 *	@date 08/08/12	新規作成
 */
class CWaveFormat : public WAVEFORMATEX
{
public:

	/** 
	 * コンストラクタ
	 *	@note 他のメンバも計算して設定します。
	 *	@param samplesPerSec 周波数。省略すると 44100。
	 *	@param bitsPerSample 一データのビットサイズ。省略すると 16。 
	 *	@param channels チャネル数。省略すると 2。
	 */
	CWaveFormat(DWORD samplesPerSec = 44100, WORD bitsPerSample = 16, WORD channels = 2)
	{
		wFormatTag = WAVE_FORMAT_PCM;
		cbSize = sizeof(WAVEFORMATEX);
		Set(samplesPerSec, bitsPerSample, channels);
	}

	/**
	 * 代入コンストラクタ
	 *	@param f 代入
	 */
	CWaveFormat(const WAVEFORMATEX& f)
	{
		WAVEFORMATEX* P = this;
		*P = f;
	}

	/** 
	 * [設定] 設定
	 *	@note 他のメンバも計算して設定します。
	 *	@param samplesPerSec 周波数。
	 *	@param bitsPerSample 一データのビットサイズ。 
	 *	@param channels チャネル数。
	 */
	void Set(DWORD samplesPerSec, WORD bitsPerSample, WORD channels)
	{
		nSamplesPerSec	= samplesPerSec;
		wBitsPerSample	= bitsPerSample;
		nChannels		= channels;
		nBlockAlign		= static_cast<WORD>(nChannels * wBitsPerSample / 8);
		nAvgBytesPerSec	= nSamplesPerSec * nBlockAlign;
	}

	/**
	 * [計算] サンプル数から時間を計算
	 *	@param samples サンプル数
	 *	@return 時間(ms)
	 */
	DWORD CalcTime(size_t samples) const
	{
		ULONGLONG r = samples;
		r *= 1000;
		return static_cast<DWORD>((r + nSamplesPerSec - 1) / nSamplesPerSec);
	}

	/**
	 * [計算] 時間からサンプル数を計算
	 *	@param time 時間(ms)
	 *	@return サンプル数
	 */
	size_t CalcSamples(DWORD time) const
	{
		ULONGLONG r = time;
		r *= nSamplesPerSec;
		return static_cast<size_t>(r / 1000);
	}
};



/**@ingroup AUDIO
 * WAVEファイルクラス
 *	
 *		wavファイルをチェックし読み込むことが出来ます。
 *
 *	@note マップドファイルを使用しています。
 *
 *		対応しているファイルは、非圧縮の 8bitか16bitのPCMです。　
 *
 *	@attention RefData() メソッドは、ローカルメモリに読み込んだデータを返します。
 *			そのため、 1G を超える WAVEファイルの場合、メモリ不足になる可能性があります。
 *
 *	@par必要ファイル
 *			TnbWaveFile.h
 *
 *	@date 08/08/12	新規作成
 *	@date 11/08/24	GetChunk() 、追加。 CConstFileMapping を使用するように変更。 4GB までのファイルに対応。
 *	@date 11/08/30	LIST INFO チャンククラス、追加。 GetData() 廃止。 GetByteData() , GetShortData() 追加。
 */
class CWaveFile : CCopyImpossible
{
public:

	/// チャンクヘッダー型
	struct TChunkHeader
	{
		FOURCC type;	///< 型
		LONG length;	///< 長さ
	};

	/**@ingroup AUDIO
	 * LIST INFO チャンク管理
	 *	
	 *	@par必要ファイル
	 *			TnbWaveFile.h
	 *
	 *	@date 11/08/29	暫定作成
	 */
	class CListInfoChunk
	{
	public:

		/**
		 * [設定] 空化
		 */
		void Empty(void)
		{
			m_map.RemoveAll();
		}

		/**
		 * [取得] タイプ一覧取得.
		 *		持っているタイプを一覧します。
		 *	@return 一覧
		 */
		CVectorT<FOURCC> EnumType(void) const
		{
			CVectorT<FOURCC> v;
			v.Copy(m_map.GetKeysAdapter());
			return v;
		}

		/**
		 * [確認] タイプ確認.
		 *		指定のタイプを持っているか、確認します。
		 *	@param cc タイプコード。 MAKEFOURCC() マクロが便利です。 \n ex) MAKEFOURCC('n', 'o', 't', 'e')
		 *	@retval true 持っている.
		 *	@retval false  持っていない
		 */
		bool Has(FOURCC cc) const
		{
			return m_map.HasKey(cc);
		}

		/**
		 * [取得] 文字列取得.
		 *		指定のタイプに関連付けられている文字列を返します。
		 *	@param[out] _str 文字列が格納されます。
		 *	@param[in] cc タイプコード。 MAKEFOURCC() マクロが便利です。 \n ex) MAKEFOURCC('n', 'o', 't', 'e')
		 *	@retval true 持っている.
		 *	@retval false  持っていない
		 */
		bool Get(CAscii& _str, FOURCC cc) const
		{
			if ( m_map.HasKey(cc) )
			{
				_str = m_map[cc];
				return true;
			}
			return false;
		}

		/**
		 * [設定] 文字列取得.
		 *		指定のタイプに関連付ける文字列を指定します。
		 *	@param cc タイプコード。 MAKEFOURCC() マクロが便利です。 \n ex) MAKEFOURCC('n', 'o', 't', 'e')
		 *	@param lpsz 文字列。
		 */
		void Set(FOURCC cc, LPCTSTR lpsz)
		{
			m_map[cc] = lpsz;
		}

		/**
		 * [作成] チャンクブロック作成.
		 *	@note "INFO" から始まるチャンクブロックを返します。
		 *	@return ブロック
		 */
		CByteVector Make(void) const
		{
			CByteVector vb;
			vb.AddElements(4, reinterpret_cast<const BYTE*>("INFO"));
			loop ( i, m_map.GetSize() )
			{
				FOURCC cc = m_map.At(i).first;
				const CAscii& s = m_map.At(i).second;
				DWORD len = s.GetLength() + 1;
				vb.AddElements(4, reinterpret_cast<const BYTE*>(&cc));
				vb.AddElements(4, reinterpret_cast<const BYTE*>(&len));
				vb.AddElements(len, reinterpret_cast<const BYTE*>(s.operator LPCSTR()));
				if ( (len & 1) != 0 )
				{
					vb.Add(0);
				}
			}
			return vb;
		}

		/**
		 * [解析] チャンクブロック解析.
		 *	@note "INFO" から始まるチャンクブロックを解析し、本インスタンスに設定します。
		 *	@param c シリアライズデータ.
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool Analyze(const IConstCollectionT<BYTE>& c)
		{
			m_map.RemoveAll();
			size_t sz = 0;
			DWORD pos = 0;
			FOURCC type;
			sz = c.GetElements(sizeof(FOURCC), reinterpret_cast<BYTE*>(&type), pos);
			if ( sz != sizeof(FOURCC) )
			{
				return false;
			}
			pos += sz;
			if ( type != MAKEFOURCC('I', 'N', 'F', 'O') )
			{
				return false;
			}
			TChunkHeader chunk;
			while ( pos < c.GetSize() )
			{
				sz = c.GetElements(sizeof(chunk), reinterpret_cast<BYTE*>(&chunk), pos);
				if ( sz != sizeof(chunk) )
				{
					return false;
				}
				pos += sz;
				size_t len = chunk.length;
				if ( (len & 1) != 0 )
				{
					len ++; //偶数にする
				}
				CAscii& a = m_map[chunk.type];
				sz = c.GetElements(len, reinterpret_cast<BYTE*>(a.GetBuffer(len + 1)), pos);
				if ( sz != len )
				{
					return false;
				}
				pos += sz;
				a.ReleaseBuffer();
			}
			return true;
		}

		/**
		 * [解析] チャンクブロック解析.
		 *	@note "INFO" から始まるチャンクブロックを解析し、本インスタンスに設定します。
		 *	@param size サイズ
		 *	@param P シリアライズデータ.
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool Analyze(size_t size, const BYTE* P)
		{
			return Analyze(CConstAdapterT<BYTE>(size, P));
		}

	private:
		CMapT<FOURCC, CAscii>	m_map;	///< マップ
	};


	//--------------


	/// コンストラクタ
	CWaveFile(void) : m_dataSize(0), m_dataOffset(0), m_adapter(NULL, 0)
	{
	}

	/// デストラクタ
	~CWaveFile(void)
	{
		Close();
	}

	/**
	 * [確認] オープン確認.
	 *	@note オープンしていないと、Get,Refer系メソッドは無意味な値を返します。
	 *	@retval true オープン済み
	 *	@retval false 未オープン。
	 */
	bool IsOpened(void) const
	{
		return m_file.IsOpened();
	}

	/// [設定] クローズ
	void Close(void)
	{
		m_file.Close();
		m_file16.Close();
		m_chunkMap.RemoveAll();
		m_refData.RemoveAll();
		m_dataOffset = 0;
		m_dataSize = 0;
	}

	/**
	 * [設定] PCMファイルオープン
	 *	@note PCMファイルを読み込み、使用できるようにします。
	 *	@param fileName ファイル名
	 *	@param format フォーマット
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Open(LPCTSTR fileName, const WAVEFORMATEX& format)
	{
		Close();
		m_format = format;
		if ( ! m_file.Open(fileName) )
		{
			return false;
		}
		if ( ! m_file16.Open(fileName) )
		{
			return false;
		}
		m_dataOffset = 0;
		m_dataSize = m_file.GetSize();
		return true;
	}

	/**
	 * [設定] WAVファイルオープン
	 *	@note WAVファイルを読み込み、使用できるようにします。
	 *	@param fileName ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR fileName)
	{
		Close();
		if ( ! m_file.Open(fileName) )
		{
			return false;
		}
		INDEX pos = 0;
		TChunkHeader fileChunk;
		pos += m_file.GetElements(sizeof(fileChunk), reinterpret_cast<BYTE*>(&fileChunk), pos);
		FOURCC riffType;
		pos += m_file.GetElements(sizeof(riffType), reinterpret_cast<BYTE*>(&riffType), pos);
		if ( fileChunk.type == FC_RIFF && fileChunk.length <= ToInt(m_file.GetSize()) && riffType == FC_WAVE )
		{
			while( pos < m_file.GetSize() )
			{
				TChunkHeader chunk;
				pos += m_file.GetElements(sizeof(chunk), reinterpret_cast<BYTE*>(&chunk), pos);
				if ( chunk.type == FC_FMT )
				{
					m_file.GetElements(sizeof(m_format), reinterpret_cast<BYTE*>(&m_format), pos);
					WORD bps = m_format.wBitsPerSample;
					if ( bps != 8 && bps != 16 )
					{
						break;
					}
					if ( m_format.wFormatTag != WAVE_FORMAT_PCM )
					{
						break;
					}
					if ( m_format.nChannels != 1 && m_format.nChannels != 2 )
					{
						break;
					}
					m_format.cbSize = 0;
				}
				else if ( chunk.type == FC_DATA )
				{
					m_dataOffset = pos;
					m_dataSize = chunk.length;
				}
				else
				{
					TChunkMap cm;
					cm.offset = pos;
					cm.size = chunk.length;
					m_chunkMap[chunk.type] = cm;
				}
				if ( chunk.length == 0 )
				{
					break;
				}
				pos += chunk.length;
			}
			if ( pos >= m_file.GetSize() )
			{
				if ( m_file16.Open(fileName, m_dataOffset, m_dataSize / sizeof(short)) )
				{
					return true;
				}
			}
		}
		Close();
		return false;
	}

	/**
	 * [参照] Waveフォーマット参照
	 *	@return Waveフォーマット
	 */
	const CWaveFormat& RefFormat(void) const
	{
		return m_format;	///< 保持データフォーマット
	}

	/**
	 * [取得] 波形データ数取得.
	 *	@return 波形データ数
	 */
	size_t GetSamples(void) const
	{
		if ( m_format.nBlockAlign == 0 )
		{
			return 0;
		}
		return m_dataSize / m_format.nBlockAlign;
	}

	/**
	 * [取得] 波形データ時間取得.
	 *	@return 波形データ時間(ms)
	 */
	DWORD GetTotalTime(void) const
	{
		if ( m_format.nBlockAlign == 0 || m_format.nSamplesPerSec == 0 )
		{
			return 0;
		}
		ULONGLONG r = (m_dataSize / m_format.nBlockAlign);
		r *= 1000;
		return static_cast<DWORD>((r + m_format.nSamplesPerSec - 1) / m_format.nSamplesPerSec);
	}

	/**
	 * [取得] 波形データサイズ取得.
	 *	@return 波形データサイズ
	 */
	size_t GetDataSize(void) const
	{
		return m_dataSize;
	}

	/**
	 * [取得] 波形データポインタ取得.
	 *	@param i インデックス
	 *	@return 波形データポインタ
	 */
	const BYTE* RefData(INDEX i = 0) const
	{
		if ( m_refData.IsEmpty() )
		{
			m_refData.Copy(GetByteData());
		}
		return m_refData.ReferBuffer() + i;
	}

	/**
	 * [取得] 8bit波形データ取得.
	 *	@attention 戻り値は一時的なオブジェクトです。本インスタンスの別のメソッドを使用すると無意味なものになる可能性が有ります。
	 *			必要に応じて CByteVector などに Copy() してください。
	 *	@return 波形データ
	 */
	const IConstCollectionT<BYTE>& GetByteData(void) const
	{
		m_adapter = CConstOffsetAdapterT<BYTE>(&m_file, m_dataOffset, m_dataSize);
		return m_adapter;
	}

	/**
	 * [取得] 16bit波形データ取得.
	 *	@attention 戻り値は一時的なオブジェクトです。本インスタンスの別のメソッドを使用すると無意味なものになる可能性が有ります。
	 *			必要に応じて CVectorT<short> などに Copy() してください。
	 *	@return 波形データ
	 */
	const IConstCollectionT<short>& GetShortData(void) const
	{
		return m_file16;
	}

	/**
	 * [取得] チャンクデータ取得.
	 *	@attention 戻り値は一時的なオブジェクトです。本インスタンスの別のメソッドを使用すると無意味なものになる可能性が有ります。
	 *			必要に応じて CByteVector などに Copy() してください。
	 *	@param[in] cc チャンクコード。 MAKEFOURCC() マクロが便利です。 \n ex) MAKEFOURCC('n', 'o', 't', 'e')
	 *	@return チャンクデータ。 指定チャンクがない場合、サイズが０のオブジェクトが返ります。
	 */
	const IConstCollectionT<BYTE>& GetChunk(FOURCC cc) const
	{
		if ( m_chunkMap.HasKey(cc) )
		{
			m_adapter = CConstOffsetAdapterT<BYTE>(&m_file, m_chunkMap[cc].offset, m_chunkMap[cc].size);
			return m_adapter;
		}
		return m_nullCollection;
	}

	/**
	 * [取得] LIST,INFO チャンクデータ取得
	 *	@param[out] _lic データが格納されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetListInfoChunk(CListInfoChunk& _lic) const
	{
		const IConstCollectionT<BYTE>& c = GetChunk(MAKEFOURCC('L','I','S','T'));
		if ( c.GetSize() > 0 )
		{
			return _lic.Analyze(c);
		}
		return false;
	}

	/**
	 * [作成] Wavファイルヘッダ作成.
	 *	@param[out] _head 作成したヘッダを格納します。
	 *	@param[in] channels チャネル数
	 *	@param[in] rate レート
	 *	@param[in] bit ビット
	 *	@param[in] dataSize データサイズ(Byte数)。
	 *	@param[in] extendSize その他のサイズ(Byte数)。他のチャンクを付加するときに使用します
	 *	@return ヘッダサイズ
	 */
	static size_t MakeWaveHeader(CWorkMem& _head, int channels, long rate, WORD bit, size_t dataSize, size_t extendSize = 0)
	{
		_head.Resize(sizeof(TWaveFileHeader));
		TWaveFileHeader* P = reinterpret_cast<TWaveFileHeader*>(_head.Ref());
		P->fileChunk.type				= FC_RIFF;
		P->fileChunk.length				= dataSize + extendSize + sizeof(TWaveFileHeader) - 8;
		P->fileType						= FC_WAVE;
		P->format.wf.wFormatTag			= WAVE_FORMAT_PCM;
		P->format.wf.nChannels			= static_cast<WORD>(channels);
		P->format.wf.nSamplesPerSec		= rate;
		P->format.wBitsPerSample		= bit;
		P->format.wf.nBlockAlign		= static_cast<WORD>(channels * bit / 8);
		P->format.wf.nAvgBytesPerSec	= channels * bit / 8 * rate;
		P->formatChunk.type				= FC_FMT;
		P->formatChunk.length			= sizeof(PCMWAVEFORMAT);
		P->dataChunk.type				= FC_DATA;
		P->dataChunk.length				= dataSize;
		return sizeof(TWaveFileHeader);
	}

private:
	/// 型種類
	enum
	{
		FC_RIFF		= MAKEFOURCC('R', 'I', 'F', 'F'),	///< RIFF
		FC_WAVE		= MAKEFOURCC('W', 'A', 'V', 'E'),	///< WAVE
		FC_FMT		= MAKEFOURCC('f', 'm', 't', ' '),	///< fmt
		FC_DATA		= MAKEFOURCC('d', 'a', 't', 'a'),	///< data
	};
	/// waveファイルヘッダ型
	struct TWaveFileHeader 
	{
		TChunkHeader	fileChunk;		///< タイプヘッダ
		FOURCC			fileType;		///< タイプ
		TChunkHeader	formatChunk;	///< フォーマットヘッダ
		PCMWAVEFORMAT	format;			///< フォーマット
		TChunkHeader	dataChunk;		///< データヘッダ
	};
	/// チャンクマップ型
	struct TChunkMap
	{
		size_t		size;	///< 長さ
		INDEX		offset;	///< オフセット
	};

	CWaveFormat							m_format;			///< フォーマット
	CConstFileMapping					m_file;				///< ファイルアクセス（ヘッダ、byteデータ用）
	CConstFileMappingT<short>			m_file16;			///< ファイルアクセス（shortデータ用）
	size_t								m_dataSize;			///< データByteサイズ
	INDEX								m_dataOffset;		///< データファイル位置
	CMapT<FOURCC, TChunkMap>			m_chunkMap;			///< その他のチャンクデータ
	mutable CByteVector					m_refData;			///< 戻り値用オブジェクト
	mutable CConstOffsetAdapterT<BYTE>	m_adapter;			///< 戻り値用アダプタ
	CDummyCollectionT<BYTE>				m_nullCollection;	///< 戻り値用オブジェクト
};



}; // TNB

