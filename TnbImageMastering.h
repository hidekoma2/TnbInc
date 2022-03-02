#pragma once
/**
 *@file
 * IMAPI2関係のヘッダ
 *
 *		IMAPI2 (Image Mastering API Version 2)
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef __ATLCOM_H__
 #error TnbImageMastering.h is necessary fot OLE Object.
#endif



#include "TnbPointerHandle.h"
#include "TnbStaticMap.h"
#include "TnbStrVector.h"
#include "TnbTickCount.h"
#include "TnbDynamicFunc.h"

#ifndef _TnbDOXYGEN	//Document作成用シンボル

#ifndef __imapi2_h__

#ifndef IID_PPV_ARGS
	template<typename T> void** IID_PPV_ARGS_Helper(T** pp) 
	{	// make sure everyone derives from IUnknown
		static_cast<IUnknown*>(*pp);
		return reinterpret_cast<void**>(pp);
	}
	#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)
#endif

#ifndef __RPC__in
	#define __RPC__in
	#define __RPC__in_opt
	#define __RPC__in_ecount_full(X)
	#define __RPC__out
	#define __RPC__out_ecount_full(X)
	#define __RPC__out_ecount_part(X, Y)
	#define __RPC__deref_out_opt
	#define __RPC__deref_out_ecount_full_opt(X)
#endif

#include "sdk/imapi2.h"
#include "sdk/imapi2error.h"
#include "sdk/imapi2fs.h"
#include "sdk/imapi2fserror.h"

#endif //__imapi2_h__

#endif //_TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@defgroup IMAPI2		IMAPI2 関係
 *@ingroup FILE
 * IMAP2 関係.
 *
 *			IMAPI2 (Image Mastering API Version 2)を使った、
 *			CD-R/RW,DVD±R/RW の書込みをサポートするクラスが用意してあります。
 *		
 *		\attention CoInitialize() ではなく OleInitialize() を使う必要があります
 *			(プロジェクト作成時に、「オートメーション有り」にする、など)。
 *
 *		\par 例:
 *		- ドライブ一覧
 *		\code
 *			CImageMastering im;
 *			loop ( i, im.GetDriveCount() )
 *			{
 *				CImageMastering::CDrive drv = im.CreateDrive(i);
 *				if ( drv.IsValid() )
 *				{  // 有効なドライブ発見
 *					CImageMastering::CDrive::TInformation info = drv.GetInformation();
 *					TRACE1("ProductId = %s\n", info.productId);
 *					TRACE1("ProductRevision = %s\n", info.productRevision);
 *					TRACE1("VendorId = %s\n", info.vendorId);
 *					TRACE1("VolumeName = %s\n", dev.GetVolumeOnDisc());
 *					TRACE1("DiscType = %s\n", drv.GetTypeStringOnDisc());
 *					TRACE1("DiscState = %04X\n", drv.GetStatusOnDisc());
 *				}
 *			}
 *		\endcode
 *		- フォルダ c:\\Temp 以下を ディスクの \\ABC 以下に書き込み、 d:\\data\\memo.txt をディスクの \\ABC\\001 に書き込む
 *		\code
 *			// CImageMastering::CDrive drv; // 有効なドライブ
 *			if ( ! drv.IsBlankDisc() ) {
 *				drv.Erase();	//ブランクでないので削除
 *			}
 *			CImageMastering::CDiscImage di;
 *			di.AddSourceDirectory(_T("c:\\Temp"), _T("\\ABC"));
 *			di.AddSourceFile(_T("d:\\data\\memo.txt"), _T("\\ABC\\001"));
 *			di.SetVolumeName(_T("ボリューム"));		//ボリューム名
 *			drv.Burn(di, true);		// 書いて、閉じる
 *		\endcode
 *		- クローズしていないディスクの \\ABC\\A.txt を削除する。
 *		\code
 *			// CImageMastering::CDrive drv; // 有効なドライブ
 *			CImageMastering::CDiscImage di;
 *			drv.ReadMultiSession(di); //セッション情報読み込む
 *			di.RemoveFile(_T("\\ABC\\A.txt"));
 *			drv.Burn(di, false);		// 書く
 *		\endcode
 *		- クローズしていないディスクをクローズするだけ
 *		\code
 *			// CImageMastering::CDrive drv; // 有効なドライブ
 *			CImageMastering::CDiscImage di;
 *			di.SetVolumeName(_T("ボリューム")); //ボリューム名を変えるなら
 *			drv.Burn(di, true);		// 書いて、閉じる
 *		\endcode
 *
 *	@see CImageMastering
 *
 */



}; //TNB



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#define EXTERN_IMAPI2_GUID(itf,id)   EXTERN_GUID(itf, id, 0x7F64, 0x5B0F,  0x8F, 0x00,  0x5D, 0x77, 0xAF, 0xBE, 0x26, 0x1E)
#define EXTERN_IMAPI2FS_GUID(itf,id) EXTERN_GUID(itf, id, 0x975B, 0x59BE,  0xA9, 0x60,  0x9A, 0x2A, 0x26, 0x28, 0x53, 0xA5)

EXTERN_IMAPI2_GUID(CLSID_MsftDiscMaster2		, 0x2735412E);
EXTERN_IMAPI2_GUID(CLSID_MsftDiscRecorder2		, 0x2735412D);
EXTERN_IMAPI2_GUID(CLSID_MsftDiscFormat2Erase	, 0x2735412B);
EXTERN_IMAPI2_GUID(CLSID_MsftDiscFormat2Data	, 0x2735412A);
EXTERN_IMAPI2FS_GUID(CLSID_MsftFileSystemImage	, 0x2C941FC5);

EXTERN_IMAPI2_GUID(LIBID_IMAPILib2				, 0x2735412F);
EXTERN_IMAPI2_GUID(IID_IDiscMaster2				, 0x2735412E);
EXTERN_IMAPI2_GUID(IID_IDiscRecorder2			, 0x2735412D);
EXTERN_IMAPI2_GUID(IID_IWriteEngine2			, 0x2735412C);
EXTERN_IMAPI2_GUID(IID_IDiscFormat2Erase		, 0x2735412B);
EXTERN_IMAPI2_GUID(IID_IDiscFormat2Data			, 0x2735412A);
EXTERN_IMAPI2_GUID(IID_IDiscFormat2TrackAtOnce	, 0x27354129);
EXTERN_IMAPI2_GUID(IID_IDiscFormat2RawCD		, 0x27354128);
EXTERN_IMAPI2_GUID(IID_IWriteSpeedDescriptor	, 0x27354123);
EXTERN_IMAPI2_GUID(IID_IMultisessionSequential	, 0x27354122);
EXTERN_IMAPI2_GUID(IID_DDiscFormat2EraseEvents	, 0x2735413A);
EXTERN_IMAPI2_GUID(IID_DDiscFormat2DataEvents	, 0x2735413C);
EXTERN_IMAPI2_GUID(IID_DDiscMaster2Events		, 0x27354131);

EXTERN_IMAPI2FS_GUID(LIBID_IMAPI2FS				, 0x2C941FD0);
EXTERN_IMAPI2FS_GUID(IID_IBootOptions			, 0x2C941FCE);
EXTERN_IMAPI2FS_GUID(IID_IFsiStream				, 0x2C941FCD);
EXTERN_IMAPI2FS_GUID(IID_IFileSystemImageResult	, 0x2C941FCC);
EXTERN_IMAPI2FS_GUID(IID_IProgressItem			, 0x2C941FCB);
EXTERN_IMAPI2FS_GUID(IID_IEnumProgressItems		, 0x2C941FCA);
EXTERN_IMAPI2FS_GUID(IID_IProgressItems			, 0x2C941FC9);
EXTERN_IMAPI2FS_GUID(IID_IFsiDirectoryItem		, 0x2C941FC8);
EXTERN_IMAPI2FS_GUID(IID_IFsiFileItem			, 0x2C941FC7);
EXTERN_IMAPI2FS_GUID(IID_IEnumFsiItems			, 0x2C941FC6);
EXTERN_IMAPI2FS_GUID(IID_IFileSystemImage		, 0x2C941FC5);
EXTERN_IMAPI2FS_GUID(IID_DFileSystemImageEvents	, 0x2C941FDF);

// LIB は 'Lib2' か '2FS'
#define _TnbImageMasterEventSuper(CLS, SID, LIB)						\
	public ::ATL::CComObjectRootEx<::ATL::CComSingleThreadModel>,		\
	public ::ATL::CComCoClass<CLS>,										\
	public ::ATL::IDispatchImpl<    IDispatch, &SID, &LIBID_IMAPI##LIB, IMAPI##LIB##_MajorVersion, IMAPI##LIB##_MinorVersion>,	\
	public ::ATL::IDispEventImpl<1, CLS,       &SID, &LIBID_IMAPI##LIB, IMAPI##LIB##_MajorVersion, IMAPI##LIB##_MinorVersion>

#define _TnbImageMasterEventMember(CLS)			\
	DECLARE_NOT_AGGREGATABLE(CLS)				\
	BEGIN_COM_MAP(CLS)							\
		COM_INTERFACE_ENTRY(IDispatch)			\
	END_COM_MAP()

/// 書き込みリスナー
struct 	IImageMasteringWriterListener
{
	virtual void OnWriterEvent(IDispatch* pObject, IDispatch* pProgressDisp) = 0;
};

/// 書き込みコールバッククラス
class CImageMasteringWriterEvent : _TnbImageMasterEventSuper(CImageMasteringWriterEvent, IID_DDiscFormat2DataEvents, Lib2)
{
	_TnbImageMasterEventMember(CImageMasteringWriterEvent)
	BEGIN_SINK_MAP(CImageMasteringWriterEvent)
		SINK_ENTRY_EX(1, IID_DDiscFormat2DataEvents, DISPID_DDISCFORMAT2DATAEVENTS_UPDATE, &Update)
	END_SINK_MAP()
public:
	STDMETHOD_(VOID, Update)(IDispatch* pObject, IDispatch* pProgressDisp)
	{
		m_pListener->OnWriterEvent(pObject, pProgressDisp);
	}
	IImageMasteringWriterListener* m_pListener;
};

/// 消去リスナー
struct 	IImageMasteringEraseListener
{
	virtual void OnEraseEvent(IDispatch* pObject, LONG elapsedSeconds, LONG expectedSeconds) = 0;
};

/// 消去コールバッククラス
class  CImageMasteringEraseEvent : _TnbImageMasterEventSuper(CImageMasteringEraseEvent, IID_DDiscFormat2EraseEvents, Lib2)
{
	_TnbImageMasterEventMember(CImageMasteringEraseEvent)
	BEGIN_SINK_MAP(CImageMasteringEraseEvent)
		SINK_ENTRY_EX(1, IID_DDiscFormat2EraseEvents, DISPID_IDISCFORMAT2ERASEEVENTS_UPDATE, &Update)
	END_SINK_MAP()
public:
	STDMETHOD_(VOID, Update)(IDispatch* pObject, LONG elapsedSeconds, LONG expectedSeconds)
	{
		m_pListener->OnEraseEvent(pObject, elapsedSeconds, expectedSeconds);
	}
	IImageMasteringEraseListener* m_pListener;
};

/// Fileリスナー
struct 	IImageMasteringFileListener
{
	virtual void OnFileEvent(IDispatch* pObject, BSTR currentFile, LONG copiedSectors, LONG totalSectors) = 0;
};

/// ファイルコールバッククラス
class  CImageMasteringFileEvent : _TnbImageMasterEventSuper(CImageMasteringFileEvent, IID_DFileSystemImageEvents, 2FS)
{
	_TnbImageMasterEventMember(CImageMasteringFileEvent)
	BEGIN_SINK_MAP(CImageMasteringFileEvent)
		SINK_ENTRY_EX(1, IID_DFileSystemImageEvents, DISPID_DFILESYSTEMIMAGEEVENTS_UPDATE, &Update)
	END_SINK_MAP()
public:
	STDMETHOD_(VOID, Update)(IDispatch* pObject, BSTR currentFile, LONG copiedSectors, LONG totalSectors)
	{
		m_pListener->OnFileEvent(pObject, currentFile, copiedSectors, totalSectors);
	}
	IImageMasteringFileListener* m_pListener;
};

/// マスターリスナー
struct 	IImageMasteringListener
{
	virtual void OnAddedDevice(IDispatch* pObject, BSTR value);
	virtual void OnRemovedDevice(IDispatch* pObject, BSTR value);
};

/// マスターイベント
class CImageMasteringEvent : _TnbImageMasterEventSuper(CImageMasteringEvent, IID_DDiscMaster2Events, Lib2)
{
public:
	_TnbImageMasterEventMember(CImageMasteringEvent)
	BEGIN_SINK_MAP(CImageMasteringEvent)
		SINK_ENTRY_EX(1, IID_DDiscMaster2Events, DISPID_DDISCMASTER2EVENTS_DEVICEADDED,   &NotifyDeviceAdded  )
		SINK_ENTRY_EX(1, IID_DDiscMaster2Events, DISPID_DDISCMASTER2EVENTS_DEVICEREMOVED, &NotifyDeviceRemoved)
	END_SINK_MAP()
public:
	STDMETHOD_(VOID, NotifyDeviceAdded)(IDispatch* pObject, BSTR value)
	{
		m_pListener->OnAddedDevice(pObject, value);
	}
	STDMETHOD_(VOID, NotifyDeviceRemoved)(IDispatch* pObject, BSTR value)
	{
		m_pListener->OnRemovedDevice(pObject, value);
	}
	IImageMasteringListener* m_pListener;
};

/// PointerHandleBase用 汎用 ポインタリリーステンプレートクラス
template<typename T> struct TImReleasePointerT
{
	/// リリース. @param P リリース対象
	void operator()(T P) { P->Release(); }
};

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup IMAPI2
 * IMAPI2リザルト管理.
 *
 *		リザルトを一つだけ保持するクラスです。
 *
 *		CImageMastering クラス以下に、使用されています。
 *
 *	@par必要ファイル
 *			TnbImageMastering.h
 *
 *	@date 09/08/08 新規作成
 */
class CImageMasteringResult
{
public:
	
	/// コンストラクタ
	CImageMasteringResult(void) : m_lastResult(S_OK)
	{
		#ifdef __MSVCPP6__
			ASSERT( _pModule != NULL );
		#else 
			if ( _pAtlModule == NULL )
			{
				static CAtlMfcModule a;
				VERIFY( ::OleInitialize(NULL) == S_OK );
			}
		#endif
	}

	/**
	 * [取得] リザルトコード取得.
	 *	@return リザルトコード.
	 */
	HRESULT GetLastResult(void) const
	{
		return m_lastResult;
	}

	/**
	 * [取得] リザルト文字列取得.
	 *	@return リザルト文字列.
	 */
	CStr GetLastResultString(void) const
	{
		return ToString(m_lastResult);
	}

	/**
	 * [変換] リザルト文字列変換.
	 *		リザルトコードを文字列に変換
	 *	@param hr リザルトコード
	 *	@return 文字列
	 */
	static CStr ToString(HRESULT hr)
	{
		CStr s = _T("Unknown");
		DWORD flag = FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;
		LPVOID lpMsgBuf;
		LPWSTR lpBuff = reinterpret_cast<LPWSTR>(&lpMsgBuf);
		DWORD ret = ::FormatMessageW(flag | FORMAT_MESSAGE_FROM_HMODULE
						, ::GetModuleHandle(_T("imapi2.dll"))
						, hr, LANG_USER_DEFAULT, lpBuff, 0, NULL);
		if ( ret == 0 )
		{
			_GetLastError("FormatMessage");
			ret = ::FormatMessageW(flag | FORMAT_MESSAGE_FROM_HMODULE
						, ::GetModuleHandle(_T("imapi2fs.dll"))
						, hr, LANG_USER_DEFAULT, lpBuff, 0, NULL);
		}
		if (ret == 0)
		{
			_GetLastError("FormatMessage");
			ret = ::FormatMessageW(flag | FORMAT_MESSAGE_FROM_SYSTEM
						, NULL
						, hr, LANG_USER_DEFAULT, lpBuff, 0, NULL);
		}
		if ( ret == 0 )
		{
			_GetLastError("FormatMessage");
		}
		else
		{
			s = static_cast<LPCWSTR>(lpMsgBuf);
//			s.TrimRight('\n').TrimRight('\r');
			s.TrimRight(_T("\n\r"));
			::LocalFree(lpMsgBuf);
		}
		return s;
	}

	/**
	 * [作成] ファイルストリーム作成.
	 *	@note ::SHCreateStreamOnFile() をコールしているだけです。
	 *	@param[in] lpszFile ファイル名
	 *	@param[in] grfMode モード
	 *	@param[out] ppstm ストリーム
	 *	@return リザルト
	 */
	static HRESULT CreateStreamOnFile(LPCTSTR lpszFile, DWORD grfMode, IStream **ppstm)
	{
		try
		{
			#ifdef _UNICODE
				typedef HRESULT (WINAPI* P_CSOFW)(LPCWSTR, DWORD, IStream**);
				CDynamicFuncT<P_CSOFW> func("shlwapi.dll", "SHCreateStreamOnFileW");
			#else
				typedef HRESULT (WINAPI* P_CSOFA)(LPCSTR, DWORD, IStream**);
				CDynamicFuncT<P_CSOFA> func("shlwapi.dll", "SHCreateStreamOnFileA");
			#endif
			return func()(lpszFile, grfMode, ppstm);
		}
		catch ( CNullPointerException& e )
		{
			e.OnCatch();
		}
		return E_FAIL;
	}

protected:

	/**
	 * [設定] リザルトチェック.
	 *		リザルトコードをチェックし、記憶します。
	 *	@param hr リザルトコード
	 *	@retval true リザルトコードが「成功」だ
	 *	@retval false リザルトコードが「失敗」だ
	 */
	bool CheckRes(HRESULT hr) const
	{
		m_lastResult = hr;
		if ( SUCCEEDED(hr) )
		{
			return true;
		}
		TRACE2(" Result =(0x%08X,[%s]\n", hr, ToString(hr));
		return false;
	}

private:
	mutable HRESULT m_lastResult;
};



/**@ingroup IMAPI2
 * IMAPI2管理.
 *
 *	@note デフォルトの XP に IMAPI2 は含まれていません。
 *			以下のサーバからダウンロードしてインストールする必要があります。
 *	- (必須) Windows XP 用 Image Mastering API v2.0 (IMAPIv2.0)\n
 *		http://www.microsoft.com/downloads/details.aspx?familyid=B5F726F1-4ACE-455D-BAD7-ABC4DD2F147B&displaylang=ja
 *	- (任意) Windows Feature Pack 1.0\n
 *		http://www.microsoft.com/downloads/details.aspx?FamilyID=63ab51ea-99c9-45c0-980a-c556746fcf05&DisplayLang=ja
 *
 *	@par必要ファイル
 *			TnbImageMastering.h
 *
 *	@date 09/08/08 新規作成
 */
class CImageMastering : public CImageMasteringResult, CCopyImpossible
{
public:

	/// DiscMaster ハンドル
	typedef CPointerHandleBaseT<IDiscMaster2*, TImReleasePointerT<IDiscMaster2*> > CDiscMasterHandle;

	/// DiscRecorder ハンドル
	typedef CPointerHandleBaseT<IDiscRecorder2*, TImReleasePointerT<IDiscRecorder2*> > CDiscRecorderHandle;

	/// FileSystemImage ハンドル
	typedef CPointerHandleBaseT<IFileSystemImage*, TImReleasePointerT<IFileSystemImage*> > CFileSystemImageHandle;

	/// DiscFormatData ハンドル
	typedef CPointerHandleBaseT<IDiscFormat2Data*, TImReleasePointerT<IDiscFormat2Data*> > CDiscFormatDataHandle;

	/// DiscFormatErase ハンドル
	typedef CPointerHandleBaseT<IDiscFormat2Erase*, TImReleasePointerT<IDiscFormat2Erase*> > CDiscFormatEraseHandle;


	//-------------------------------------------------------


	/**@ingroup IMAPI2
	 * ディスクイメージ管理.
	 *
	 *		ディスク上のファイル配置を管理するクラスです。
	 *		CDrive クラスでディスクにファイルを書き込むために、
	 *		まず、このクラスにファイルを配置します。
	 *
	 *	@note CDrive::ReadMultiSession() を使い、
	 *			このクラスにマルチセッション情報を読み込み、操作することも出来ます。
	 *
	 *	@par必要ファイル
	 *			TnbImageMastering.h
	 *
	 *	@date 09/08/08 新規作成
	 *	@date 09/08/20 リスナー追加。イメージファイル作成をサポート
	 */
	class CDiscImage : public CImageMasteringResult, CCopyImpossible, IImageMasteringFileListener
	{
	public:

		/**@ingroup IMAPI2
		 * ディスクイメージ管理リスナー.
		 *
		 *		ファイル登録時に進捗を通知するためのリスナーです。
		 *
		 *	@see CDrive::SetListener 
		 *	
		 *	@par必要ファイル
		 *			TnbImageMastering.h
		 *
		 *	@date 09/08/20 新規作成
		 */
		struct IListener
		{
			/// デストラクタ
			virtual ~IListener(void) {}

			/**
			 * [通知] 進捗通知.
			 *	@note セクタ数は、追加中ファイルだけの値になります。全体ではありません。
			 *	@param file 追加中ファイル名.
			 *	@param copiedSectors 追加セクタ数
			 *	@param totalSectors 全セクタ数
			 */
			virtual void OnDiscImageAdded(LPCTSTR file, LONG copiedSectors, LONG totalSectors) = 0;
		};

		/// コンストラクタ
		CDiscImage(void) : m_useJoliet(true), m_pListener(NULL), m_pEventSink(NULL), m_pRoot(NULL)
		{
			Reset();
		}

		/// デストラクタ
		~CDiscImage(void)
		{
			m_ReleaseSink();
			if ( m_pRoot != NULL )
			{
				m_pRoot->Release();
				m_pRoot = NULL;
			}
		}

		/**
		 * [確認] 有効確認.
		 *		本インスタンスが有効か、確認できます。
		 *	@note 無効な場合、メソッドは、すべて失敗します。
		 *	@retval true 有効。
		 *	@retval false 無効。
		 */
		bool IsValid(void) const
		{
			return m_pRoot != NULL;
		}

		/**
		 * [取得] FileSystemImageインターフェース取得.
		 *	@return FileSystemImageインターフェース
		 */
		IFileSystemImage* operator->(void) const
		{
			return m_hpImage;
		}

		/**
		 * [設定] 初期化.
		 *		登録したファイルなどをクリアします。
		 *	@retval true 成功。
		 *	@retval false 失敗。
		 */
		bool Reset(void)
		{
			m_pListener = NULL;
			m_ReleaseSink();
			if ( m_pRoot != NULL )
			{
				m_pRoot->Release();
				m_pRoot = NULL;
			}
			IFileSystemImage* P = NULL;
			if ( CheckRes(::CoCreateInstance(CLSID_MsftFileSystemImage, NULL, CLSCTX_ALL, IID_PPV_ARGS(&P))) )
			{
				m_hpImage = P;
				if ( CheckRes(m_hpImage->put_FreeMediaBlocks(0/*infinite*/)) )
				{
					CheckRes(m_hpImage->get_Root(&m_pRoot));
				}
			}
			return m_pRoot != NULL;
		}

		/**
		 * [設定] Jolietモード設定.
		 *	@note CD-R/RW に書く場合、Joliet 拡張で記録するか、指定できます。
		 *		デフォルトでは Joliet 拡張で記録します。
		 *	@param isEnable false を指定すると、 ISO9660 で記録します。
		 *					true を指定すると、 Joliet で記録します。
		 */
		void SetJolietSystem(bool isEnable)
		{
			m_useJoliet = isEnable;
		}

		/**
		 * [設定］リスナー登録.
		 *	@note リスナーを追加することで、ファイル追加進捗を得ることが出来ます。
		 *	@param P リスナー。 NULL を指定することで、登録を解除できます。
		 */
		void SetListener(IListener* P)
		{
			m_pListener = P;
			m_ReleaseSink();
			if ( P != NULL )
			{
				if ( CheckRes(CComObject<CImageMasteringFileEvent>::CreateInstance(&m_pEventSink)) )
				{
					m_pEventSink->AddRef();
					m_pEventSink->m_pListener = this;
					CheckRes(m_pEventSink->DispEventAdvise(m_hpImage));
				}
			}
		}

		/**
		 * [取得] セクタサイズ取得.
		 *	@note １セクタのバイト数を返します。
		 *	@param isXA true なら CD MODE2 XA の場合のセクタサイズを返します。
		 *	@return １セクタのバイト数
		 */
		size_t GetSectorSize(bool isXA = false) const
		{
			return isXA ? 2336 : 2048;
		}

		/**
		 * [取得] 登録済みセクタ数取得.
		 *	@note AddSourceDirectory() , AddSourceFile() で登録した情報を書き込むのに、
		 *		必要なセクタ数が返ります。
		 *	@return セクタ数
		 */
		size_t GetSectorsCount(void) const
		{
			LONG r;
			return CheckRes(m_hpImage->get_UsedBlocks(&r)) ? r : 0;
		}

		/**
		 * [取得] 登録済みファイル数取得.
		 *	@note AddSourceDirectory() , AddSourceFile() で登録したファイル数が返ります。
		 *	@return ファイル数
		 */
		size_t GetFileCount(void) const
		{
			LONG r;
			return CheckRes(m_hpImage->get_FileCount(&r)) ? r : 0;
		}

		/**
		 * [取得] 登録済みディレクトリ数取得
		 *	@note AddSourceDirectory() , AddSourceFile() で登録したディレクトリ数が返ります。
		 *	@return ディレクトリ数
		 */
		size_t GetDirectoryCount(void) const
		{
			LONG r;
			return CheckRes(m_hpImage->get_DirectoryCount(&r)) ? r : 0;
		}

		/**
		 * [取得] ボリューム名取得.
		 *	@return ボリューム名
		 */
		CStr GetVolumeName(void) const
		{
			CStr r;
			CComBSTR s;
			if ( CheckRes(m_hpImage->get_VolumeName(&s)) )
			{
				r = s;
			}
			return r;
		}

		/**
		 * [設定] ボリューム名設定.
		 *	@param lpszVolume ボリューム名
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool SetVolumeName(LPCTSTR lpszVolume)
		{
			CComBSTR s = lpszVolume;
			return CheckRes(m_hpImage->put_VolumeName(s));
		}

		/**
		 * [追加] ディレクトリ追加.
		 *		書き込むファイルをディレクトリごと登録します。
		 *	@note 指定したディレクトリ自体は登録されません。その中身が追加対象です。
		 *	\code
		 *			c:\temp\foo1.txt
		 *			c:\temp\foo2.txt
		 *		がある場合、 AddSourceDirectory("c:\\temp", "\\xyz"); とすると、ディスクには
		 *			\xyz\foo1.txt
		 *			\xyz\foo2.txt
		 *		と書かれる。
		 *	\endcode
		 *	@param lpszPath 追加する PC のディレクトリ名。
		 *	@param lpszImagePath ディスク上のディレクトリ。省略するとディスクのルートが指定されます。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool AddSourceDirectory(LPCTSTR lpszPath, LPCTSTR lpszImagePath = NULL)
		{
			IFsiDirectoryItem* pDir = NULL;
			CComBSTR name = lpszPath;
			if ( lpszImagePath == NULL )
			{
				return CheckRes(m_pRoot->AddTree(name, false));
			}
			HRESULT hr = m_AddDirectory(m_pRoot, lpszImagePath);
			if ( SUCCEEDED(hr) )
			{
				CComBSTR path = lpszImagePath;
				hr = m_pRoot->get_Item(path, reinterpret_cast<IFsiItem**>(&pDir));
				if ( SUCCEEDED(hr) && lpszPath != NULL )
				{
					hr = pDir->AddTree(name, false);
					pDir->Release();
					pDir = NULL;
				}
			}
			return CheckRes(hr);
		}

		/**
		 * [追加] ファイル追加.
		 *		書き込むファイルを登録します。
		 *	@param lpszName 登録する PC のファイル名。
		 *	@param lpszImagePath ディスク上のディレクトリ。省略するとディスクのルートが指定されます。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool AddSourceFile(LPCTSTR lpszName, LPCTSTR lpszImagePath = NULL)
		{
			IFsiDirectoryItem* pDir = NULL;
			if ( lpszImagePath == NULL )
			{
				return CheckRes(m_AddFile(m_pRoot, lpszName));
			}
			HRESULT hr = m_AddDirectory(m_pRoot, lpszImagePath);
			if ( SUCCEEDED(hr) )
			{
				CComBSTR path = lpszImagePath;
				hr = m_pRoot->get_Item(path, reinterpret_cast<IFsiItem**>(&pDir));
				if ( SUCCEEDED(hr) && lpszName != NULL )
				{
					hr = m_AddFile(pDir, lpszName);
					pDir->Release();
					pDir = NULL;
				}
			}
			return CheckRes(hr);
		}

		/**
		 * [削除] ディレクトリ削除.
		 *		指定したディレクトリ以下を登録情報から削除します。
		 *	@note CDrive::ReadMultiSession() で以前のセッションを読み込んでから、
		 *		これを使うことで、ディスク上のファイルを削除することが出来ます。
		 *	@param lpszImagePath 削除するディスク上のディレクトリ。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool RemoveDirectory(LPCTSTR lpszImagePath)
		{
			CComBSTR s = lpszImagePath;
			return CheckRes(m_pRoot->RemoveTree(s));
		}

		/**
		 * [削除] ファイル削除.
		 *		指定したファイルを登録情報から削除します。
		 *	@note CDrive::ReadMultiSession() で以前のセッションを読み込んでから、
		 *		これを使うことで、ディスク上のファイルを削除することが出来ます。
		 *	@param lpszImageName 削除するディスク上のファイル名。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool RemoveFile(LPCTSTR lpszImageName)
		{
			CComBSTR s = lpszImageName;
			return CheckRes(m_pRoot->Remove(s));
		}

		/**
		 * [設定] Bootイメージ設定.
		 *		ブータブル CD を作成する際、Boot イメージを指定します。
		 *	@param lpszBootFileName ブートイメージファイル名
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool SetBootImage(LPCTSTR lpszBootFileName)
		{
			IBootOptions* pBootOptions = NULL;
			if ( ! CheckRes(::CoCreateInstance(CLSID_BootOptions, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pBootOptions))) )
			{
				return false;
			}
			IStream* pBootStream = NULL;
			HRESULT hr = CreateStreamOnFile(lpszBootFileName
						, STGM_READ | STGM_SHARE_DENY_WRITE, &pBootStream);
			if ( SUCCEEDED(hr) )
			{
				hr = pBootOptions->AssignBootImage(pBootStream);
			}
			if ( SUCCEEDED(hr) )
			{
				hr = m_hpImage->put_BootImageOptions(pBootOptions);
			}
			if ( pBootStream != NULL )
			{
				pBootStream->Release();
				pBootStream = NULL;
			}
			pBootOptions->Release();
			pBootOptions = NULL;
			return CheckRes(hr);
		}

		/**
		 * [設定] ワークディレクトリ設定.
		 *		通常、システムのテンポラリフォルダがワークディレクトリとして使用されます。
		 *		本メソッドを使うことで、変更することが出来ます。
		 *	@param lpszPath ワークディレクトリ
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool SetWorkingDirestory(LPCTSTR lpszPath)
		{
			CComBSTR s = lpszPath;
			return CheckRes(m_hpImage->put_WorkingDirectory(s));
		}

		/**
		 * ファイルアイテム情報.
		 *	@see EnumerationFiles
		 */
		struct TFileItem
		{
			bool	isDirectory;	///< ディレクトリフラグ. true ならディレクトリ、 false ならファイル
			CStr	name;			///< ファイル/ディレクトリ名
		};

		/**
		 * [取得] ファイル一覧.
		 *		本インスタンスに登録されているファイル、ディレクトリの一覧を返します。
		 *	@note CDrive::ReadMultiSession() , AddSourceFile() , AddSourceDirectory() , 
		 *		RemoveDirectory() , RemoveFile() による操作の結果がわかります。
		 *	@param[out] _items ファイルアイテム一覧が格納されます。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool EnumerationFiles(CVectorT<TFileItem>& _items) const
		{
			return m_EnumFiles(_items, m_pRoot);
		}

		/**
		 * [作成] イメージファイル作成.
		 *		本インスタンスに登録されているファイル、ディレクトリから、イメージファイルを作成します。
		 *	@param lpszFileName 作成イメージファイル名。
		 *	@param type 対象ディスクタイプ。 CD,CDR,CDRWを指定すると ISO9660(Joliet) 、それ以外は UDF システムでイメージファイルが作成されます。
		 *	@note CD,CDR,CDRW の場合、 SetJolietSystem() により、作成されるイメージが異なります。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool CreateImageFile(LPCTSTR lpszFileName, IMAPI_MEDIA_PHYSICAL_TYPE type = IMAPI_MEDIA_TYPE_CDR)
		{
			IStream* pSrcStream = CreateImageSteam(type);
			if ( pSrcStream != NULL )
			{
				IStream* pDstStream = NULL;
				HRESULT hr = CreateStreamOnFile(lpszFileName
							, STGM_WRITE | STGM_CREATE, &pDstStream);
				if ( SUCCEEDED(hr) )
				{
					STATSTG stat = { 0 };
					hr = pSrcStream->Stat(&stat, STATFLAG_NONAME);
					if ( SUCCEEDED(hr) )
					{
						ULARGE_INTEGER cbRead = {0};
						ULARGE_INTEGER cbWritten = {0};
						hr = pSrcStream->CopyTo(pDstStream, stat.cbSize, &cbRead, &cbWritten);
					}
					pDstStream->Release();
					pDstStream = NULL;
				}
				pSrcStream->Release();
				pSrcStream = NULL;
				return CheckRes(hr);
			}
			return false;
		}

		/** [作成] イメージストリーム作成.
		 *		本インスタンスに登録されているファイル、ディレクトリから、イメージストリームを作成します。
		 *	@param type 対象ディスクタイプ。 CD,CDR,CDRWを指定すると ISO9660(Joliet) 、それ以外は UDF システムでイメージファイルが作成されます。
		 *	@note CD,CDR,CDRW の場合、 SetJolietSystem() により、作成されるイメージが異なります。
		 *	@retval NULL 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 *	@retval NULL以外 成功。使用後は Release() する必要があります。
		 */
		IStream* CreateImageSteam(IMAPI_MEDIA_PHYSICAL_TYPE type = IMAPI_MEDIA_TYPE_CDR) const
		{
			HRESULT hr = m_hpImage->put_FileSystemsToCreate(m_ToFileSystem(type));
			if ( SUCCEEDED(hr) )
			{
				IFileSystemImageResult* pResultStream = NULL;
				hr = m_hpImage->CreateResultImage(&pResultStream);
				if ( SUCCEEDED(hr) )
				{
					IStream* pStream = NULL;
					hr = pResultStream->get_ImageStream(&pStream);
					pResultStream->Release();
					pResultStream = NULL;
					if ( SUCCEEDED(hr) )
					{
						return pStream;
					}
				}
			}
			VERIFY( ! CheckRes(hr) );
			return NULL;
		}

	private:
		// [取得] ファイルシステム種類取得.
		FsiFileSystems m_ToFileSystem(IMAPI_MEDIA_PHYSICAL_TYPE type = IMAPI_MEDIA_TYPE_CDR) const
		{
			FsiFileSystems f = FsiFileSystemUDF;
			switch ( type )
			{
			case IMAPI_MEDIA_TYPE_CDROM:
			case IMAPI_MEDIA_TYPE_CDR:
			case IMAPI_MEDIA_TYPE_CDRW:
				if ( m_useJoliet )
				{
					f = static_cast<FsiFileSystems>(FsiFileSystemJoliet | FsiFileSystemISO9660);
				}
				else
				{
					f = FsiFileSystemISO9660;
				}
				break;
			default:
				break;
			}
			return f;
		}
		// ディレクトリ追加
		HRESULT m_AddDirectory(IFsiDirectoryItem* _pDir, LPCTSTR lpszPath)
		{
			CStr path = lpszPath;
//			path.TrimLeft('\\').TrimLeft('/').TrimRight('\\').TrimRight('/');
			path.Trim(_T("\\/"));
			if ( path.IsEmpty() )
			{
				return S_OK;
			}
//			INT_PTR f = path.ReverseFind('\\');
//			if ( f < 0 )
//			{
//				f = path.ReverseFind('/');
//			}
			INT_PTR f = path.ReverseFindOneOf(_T("\\/"));
			if ( f >= 0 )
			{
				HRESULT hr = m_AddDirectory(_pDir, path.Left(f));
				if ( ! SUCCEEDED(hr) )
				{
					return hr;
				}
			}
			CComBSTR p = path;
			return _pDir->AddDirectory(p);
		}
		// ファイル追加
		HRESULT m_AddFile(IFsiDirectoryItem* _pDir, LPCTSTR lpszFileName)
		{
			IStream* pStream = NULL;
			HRESULT hr = CreateStreamOnFile(lpszFileName
						, STGM_READ | STGM_SHARE_DENY_WRITE, &pStream);
			if ( SUCCEEDED(hr) )
			{
				CStr s = lpszFileName;
//				INT_PTR f = s.ReverseFind(_T("\\"));
//				if ( f < 0 )
//				{
//					f = s.ReverseFind(_T("/"));
//				}
				INT_PTR f = s.ReverseFindOneOf(_T("\\/"));
				if ( f >= 0 )
				{
					s = s.Mid(f + 1);
				}
				CComBSTR name = s;
				//hr = _pDir->AddFile(name, pStream); //これだと日付が今になってしまう
				STATSTG stat = { 0 };
				hr = pStream->Stat(&stat, STATFLAG_NONAME);
				if ( SUCCEEDED(hr) )
				{
					IFsiFileItem* I = NULL;
					hr = m_hpImage->CreateFileItem(name, &I);
					if ( SUCCEEDED(hr) )
					{
						I->put_Data(pStream);
						I->put_CreationTime(COleDateTime(stat.ctime));
						I->put_LastAccessedTime(COleDateTime(stat.atime));
						I->put_LastModifiedTime(COleDateTime(stat.mtime));
						hr = _pDir->Add(I);
						I->Release();
						I =NULL;
					}
				}
				pStream->Release();
				pStream = NULL;
			}
			return hr;
		}
		/// EnumFiles、サブ
		bool m_EnumFiles(CVectorT<TFileItem>& _items, IFsiDirectoryItem* pDirs) const
		{
			IEnumFsiItems* pItems = NULL;
			HRESULT hr = pDirs->get_EnumFsiItems(&pItems);
			ULONG celt = 0;
			while ( true )
			{
				IFsiItem* pFI = NULL;
				hr = pItems->Next(1, &pFI, &celt);
				if ( hr != S_OK )
				{
					break;
				}
				if ( pFI != NULL )
				{
					TFileItem fi;
					IFsiDirectoryItem* P = NULL;
					fi.isDirectory = SUCCEEDED(pFI->QueryInterface(&P));
					CComBSTR s;
					pFI->get_FullPath(&s);
					fi.name = s;
					_items.Add(fi);
					if ( P != NULL && ! m_EnumFiles(_items, P) )
					{
						break;
					}
					pFI->Release();
					pFI = NULL;
				}
				pItems->Skip(celt);
			}
			pItems->Release();
			pItems = NULL;
			return CheckRes(hr);
		}
		void m_ReleaseSink(void)
		{
			if ( m_pEventSink != NULL )
			{
				m_pEventSink->DispEventUnadvise(m_pRoot);
				m_pEventSink->Release();
				m_pEventSink = NULL;
			}
		}
		/// IMAPI2 からのコールバック
		virtual void OnFileEvent(IDispatch* object, BSTR currentFile, LONG copiedSectors, LONG totalSectors)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnDiscImageAdded(CStr(currentFile), copiedSectors, totalSectors);
			}
		}
		bool									m_useJoliet;	///< true の場合、 CDは Joliet システム
		IFsiDirectoryItem*						m_pRoot;		///< ルートアイテム
		CFileSystemImageHandle					m_hpImage;		///< イメージ
		IListener*								m_pListener;	///< リスナー
		CComObject<CImageMasteringFileEvent>*	m_pEventSink;	///< Sink
	};


	//-------------------------------------------------------------


	/**@ingroup IMAPI2
	 * ドライブ管理.
	 *
	 *		一つの CD/DVD/BD ドライブを管理します。
	 *
	 *		挿入されているディスクに対しての書き込みや、ディスクの種類や状態の確認、トレイのオープンクローズをサポートします。
	 *
	 *	@note 書き込みや消去の進捗は、 IListener で得ることが出来ます。また、書き込みの停止も、 IListener を使います。
	 *	
	 *	@note 本インスタンスは CImageMastering::CreateDrive() を使って、生成します。
	 *
	 *	@par必要ファイル
	 *			TnbImageMastering.h
	 *
	 *	@date 09/08/08 新規作成
	 */
	class CDrive : public CImageMasteringResult
	{
	public:

		/**@ingroup IMAPI2
		 * ドライブ管理リスナー.
		 *
		 *		書込み、Erase時、進捗を通知するためのリスナーです。
		 *
		 *	@see CDrive::SetListener 
		 *	
		 *	@par必要ファイル
		 *			TnbImageMastering.h
		 *
		 *	@date 09/08/08 新規作成
		 */
		struct IListener
		{
			/**
			 * 進捗データ.
			 */
			class CProgressData
			{
			public:
				/**
				 * [取得] 残り時間.
				 *	@return 残り時間(秒)
				 */
				virtual LONG GetRemainingTime(void) const = 0;
				/**
				 * [取得] 経過時間.
				 *	@return 経過時間(秒)
				 */
				virtual LONG GetElapsedTime(void) const = 0;
				/**
				 * [取得] 予想全時間.
				 *	@return 予想全時間(秒)
				 */
				virtual LONG GetExpectedTime(void) const = 0;
				/**
				 * [取得] 開始 LBA.
				 *	@return 開始 LBA
				 */
				virtual LONG GetStartLba(void) const = 0;
				/**
				 * [取得] 全セクタ数.
				 *	@return 全セクタ数
				 */
				virtual LONG GetTotalSectorCount(void) const = 0;
				/**
				 * [取得] 書込み LBA 数.
				 *	@return 書込み LBA 数
				 */
				virtual LONG GetWrittenLba(void) const = 0;
				/**
				 * [取得] 書込みセクタ数.
				 *	@return 書込みセクタ数
				 */
				virtual LONG GetWrittenSector(void) const = 0;
			};

			/// デストラクタ
			virtual ~IListener(void) {}

			/**
			 * [通知] エラー発生通知.
			 *	@note エラーが発生した場合、通知します。
			 *	@param hr リザルトコード
			 */
			virtual void OnDriveErrorResult(HRESULT hr) = 0;

			/**
			 * [通知] 書き込み進捗通知.
			 *		書き込みの進捗を通知します。
			 *	@note false を返すことで、書き込みを停止することが出来ます。
			 *	@param pd 進捗データ.
			 *	@param action 書きこみアクション.
			 *	@retval true 書き込み続行.
			 *	@retval false 書き込み中止要求.
			 */
			virtual bool OnDriveWriteEvent(const CProgressData& pd, IMAPI_FORMAT2_DATA_WRITE_ACTION action) = 0;

			/**
			 * [通知] 消去進捗通知.
			 *	@note 消去の進捗を通知します。
			 *	@param pd 進捗データ。時間情報のみ、有効です.
			 */
			virtual void OnDriveEraseEvent(const CProgressData& pd) = 0;
		};


		//-----------------------------------


		/**
		 * コンストラクタ.
		 *	@note 正しい引数を指定しないと、無効なインスタンスになります。
		 *	@param dr DiscRecorder
		 *	@param name ドライブ名
		 */
		CDrive(CDiscRecorderHandle dr, LPCTSTR name)
		{
			m_Recorder() = dr;
			if ( ! m_Recorder().IsNull() )
			{
				CComBSTR s = name;
				if ( ! CheckRes(m_Recorder()->InitializeDiscRecorder(s)) )
				{
					m_Recorder().Null();
				}
				m_Writer() = m_CreateDiscFormatData();
				if ( m_Writer().IsNull() )
				{
					m_Recorder().Null();
				}
			}
		}

		/**
		 * [確認] 有効確認.
		 *		本インスタンスが有効か、確認できます。
		 *	@note 無効な場合、メソッドは、すべて失敗します。
		 *	@retval true 有効。
		 *	@retval false 無効。
		 */
		bool IsValid(void) const
		{
			return ! m_Recorder().IsNull();
		}

		/**
		 * [取得] IDiscRecorder2インターフェース取得.
		 *	@return IDiscRecorder2インターフェース
		 */
		IDiscRecorder2* operator->(void)
		{
			return m_Recorder();
		}

		/**
		 * [設定] リスナー設定.
		 *	@param pListener リスナー
		 */
		void SetListener(IListener* pListener)
		{
			m_member.pListener = pListener;
		}

		/**
		 * [消去] 消去.
		 *		CD-RW, DVD±RW, BD-RE ディスクを初期化します。
		 *	@note 消去が完了するか、エラーが発生するまで、メソッドはリターンしません。
		 *　	消去の進捗は、 IListener で得られます。
		 *	@param isFull false を指定すると高速消去、 true なら標準消去します。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool Erase(bool isFull = false)
		{
			CDiscFormatEraseHandle hpEraser = m_CreateDiscFormatErase();
			HRESULT hr = S_OK;
			if ( isFull )
			{
				hr = hpEraser->put_FullErase(VARIANT_TRUE);
			}
			if ( CheckRes(hr) )
			{
				CComObject<CImageMasteringEraseEvent>* pEventSink = NULL;
				if ( CheckRes(CComObject<CImageMasteringEraseEvent>::CreateInstance(&pEventSink)) )
				{
					pEventSink->AddRef();
					pEventSink->m_pListener = &m_member;
					pEventSink->DispEventAdvise(hpEraser);
					hr = hpEraser->EraseMedia();
					bool r = CheckRes(hr);
					pEventSink->DispEventUnadvise(hpEraser);
					pEventSink->Release();
					pEventSink = NULL;
					return r;
				}
			}
			return false;
		}

		/**
		 * [書込] 書き込み.
		 *		指定の ディスクイメージ管理クラスの内容をディスクに書き込みます。
		 *	@note 追記の場合、マルチセッションとして処理します。
		 *	@note 書き込みが完了するか、エラーが発生するまで、メソッドはリターンしません。
		 *　	書き込みの進捗は、 IListener で得られます。
		 *	@param discImage ディスクイメージ管理.
		 *	@param isClose true なら書き込み後、クローズします。 
		 *					false ならクローズしません（追記可能になります）。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool Burn(const CDiscImage& discImage, bool isClose = true)
		{
			IFileSystemImage* pImage = discImage.operator->();
			if ( pImage == NULL )
			{
				return false;
			}
			LONG changePoint = -1;
			bool r = CheckRes(pImage->get_ChangePoint(&changePoint));
			IStream* pSrcStream = NULL;
			if ( r )
			{
				pSrcStream = discImage.CreateImageSteam(GetTypeOnDisc());
				if ( pSrcStream == NULL )
				{
					r = CheckRes(discImage.GetLastResult());
					ASSERT( ! r );
				}
			}
			r = m_SetCloseDiscMode(isClose);
			if ( r )
			{
				r = m_CheckDvdplusrdl();
			}
			if ( r && ! IsBlankDisc() && IsAppendableDisc() )
			{
				r = m_MultiDisp(pImage);
			}
			if ( r )
			{
				r = m_Burn(pSrcStream);
			}
			if ( pSrcStream != NULL )
			{
				pSrcStream->Release();
				pSrcStream = NULL;
			}
			if ( changePoint >= 0 )
			{
				pImage->RollbackToChangePoint(changePoint);
			}
			return r;
		}

		/**
		 * [書込] イメージファイル書き込み.
		 *		指定のディスクイメージファイルをディスクに書き込みます。
		 *	@note 書き込みが完了するか、エラーが発生するまで、メソッドはリターンしません。
		 *　	書き込みの進捗は、 IListener で得られます。
		 *	@param lpszFileName イメージファイル名。
		 *	@param isClose true なら書き込み後、クローズします。 
		 *					false ならクローズしません（追記可能になります）。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool Burn(LPCTSTR lpszFileName, bool isClose = true)
		{
			if ( m_Writer().IsNull() )
			{
				return false;
			}
			if ( ! m_SetCloseDiscMode(isClose) )
			{
				return false;
			}
			IStream* pStream = NULL;
			HRESULT hr = CreateStreamOnFile(lpszFileName
						, STGM_READ | STGM_SHARE_DENY_WRITE, &pStream);
			bool r = CheckRes(hr);
			if ( r )
			{
				r = m_Burn(pStream);
			}
			if ( pStream != NULL )
			{
				pStream->Release();
				pStream = NULL;
			}
			return r;
		}

		/**
		 * [処理] トレイオープン.
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool OpenTray(void)
		{
			return m_Tray(true);
		}

		/**
		 * [処理] トレイクローズ.
		 *	@note ドライブによっては、機能しません。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool CloseTray(void)
		{
			return m_Tray(false);
		}
 
		/**
		 * ドライブ情報.
		 *	@see GetInformation()
		 */
		struct TInformation
		{
			CStr	vendorId;		///< ベンダーID
			CStr	productId;		///< プロダクトID
			CStr	productRevision;///< プロダクトリビジョン
		};

		/**
		 * [取得] ドライブ情報情報.
		 *	@return ドライブ情報.
		 */
		TInformation GetInformation(void) const
		{
			TInformation info;
			if ( ! m_Recorder().IsNull() )
			{
				info.vendorId		= m_GetBstrKindString(BstrKind_VendorId);
				info.productId		= m_GetBstrKindString(BstrKind_ProductId);
				info.productRevision= m_GetBstrKindString(BstrKind_ProductRevision);
			}
			return info;
		}

		/**
		 * [取得] マウントパス取得.
		 *		このドライブがマウントされているパス一覧を返します。
		 *		通常、マウント場所は一箇所です（ q:\ など）。
		 *	@param[out] _mountPaths パス一覧が格納されます。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool EnumerationMountPath(CStrVector& _mountPaths) const
		{
			_mountPaths.RemoveAll();
			SAFEARRAY* pSA = NULL;
			if ( ! m_Recorder().IsNull() && CheckRes(m_Recorder()->get_VolumePathNames(&pSA)) )
			{
				VARIANT* P = static_cast<VARIANT*>(pSA->pvData);
				loop ( i, pSA->rgsabound[0].cElements )
				{
					ASSERT( P[i].vt == VT_BSTR );
					_mountPaths.Add(CStr(P[i].bstrVal));
				}
				::SafeArrayDestroyData(pSA);
				pSA = NULL;
				return true;
			}
			return false;
		}

		/**
		 * [取得] セクタサイズ取得.
		 *	@note １セクタのバイト数を返します。
		 *	@return １セクタのバイト数
		 */
		size_t GetSectorSize(void) const
		{
			return 2048;
		}

		/**
		 * [取得] サポート書き込み速度一覧.
		 *	@note ディスクがないと、失敗します。
		 *	@param[out] _speeds スピード(sector/second)の配列が格納されます。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool EnumerationSupportWriteSpeeds(CWordVector& _speeds) const
		{
			_speeds.RemoveAll();
			SAFEARRAY* pSA = NULL;
			if ( ! m_Writer().IsNull() && CheckRes(m_Writer()->get_SupportedWriteSpeeds(&pSA)) )
			{
				VARIANT* P = static_cast<VARIANT*>(pSA->pvData);
				loop ( i, pSA->rgsabound[0].cElements )
				{
					_speeds.Add(static_cast<WORD>(P[i].ulVal));
				}
				::SafeArrayDestroyData(pSA);
				pSA = NULL;
				return true;
			}
			return false;
		}

		/**
		 * [設定] 速度設定.
		 *	@param speed スピード(sector/second)。-1 を指定したらデバイスの最速が指定されます。
		 *	@param isPureCAV
		 *		Requested rotational-speed control type. 
		 *		Set to \b true to request constant angular velocity (CAV) rotational-speed control type. 
		 *		Set to \b false to use another rotational-speed control type that the recorder supports. 
		 *		The default is \b false.
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool SetWriteSpeed(int speed = -1, bool isPureCAV = false)
		{
			if ( ! m_Writer().IsNull() )
			{
				VARIANT_BOOL r = isPureCAV ? VARIANT_TRUE : VARIANT_FALSE;
				return CheckRes(m_Writer()->SetWriteSpeed(speed, r));
			}
			return false;
		}

		/**
		 * [取得] ディスク全セクタ数取得.
		 *	@note ディスクの全セクタ数を取得できます。
		 *	@retval 0 失敗。エラーか、ディスクが未挿入。
		 *	@retval 1以上 成功。値はセクタ数。
		 */
		size_t GetTotalSectorsOnDisc(void) const
		{
			if ( ! m_Writer().IsNull() )
			{
				LONG l;
				if ( CheckRes(m_Writer()->get_TotalSectorsOnMedia(&l)) )
				{
					return l;
				}
			}
			return 0;
		}

		/**
		 * [取得] ディスク空きセクタ数取得.
		 *	@note ディスクの空きセクタ数を取得できます。
		 *	@return 空きセクタ数。
		 */
		size_t GetFreeSectorsOnDisc(void) const
		{
			if ( ! m_Writer().IsNull() )
			{
				LONG l;
				if ( CheckRes(m_Writer()->get_FreeSectorsOnMedia(&l)) )
				{
					return l;
				}
			}
			return 0;
		}

		/**
		 * [確認] ディスク確認.
		 *		ディスクの有無を確認します。
		 *	@note ディスクがない場合、指定時間待たせることも出来ます（その間、メソッドからリターンしません）。
		 *	@param time ディスク無い時の待ち時間(ms)
		 *	@retval true ディスク有り。
		 *	@retval false なし。
		 */
		bool ExistDisc(DWORD time = 0) const
		{
			if ( ! m_Writer().IsNull() )
			{
				VARIANT_BOOL isBlank = FALSE;
				CTickCount ti;
				while ( true )
				{
					HRESULT hr = m_Writer()->get_MediaHeuristicallyBlank(&isBlank);
					if ( hr == E_IMAPI_RECORDER_MEDIA_NO_MEDIA || hr == E_IMAPI_RECORDER_MEDIA_BECOMING_READY )
					{
						if ( ! ti.IsPassed(time) )
						{
							::Sleep(200);
							continue;
						}
						CheckRes(S_OK);
						return false;
					}
					return CheckRes(hr);
				}
			}
			return false;
		}

		/**
		 * [取得] ディスク状態取得.
		 *	@return ディスク状態。 IMAPI_FORMAT2_DATA_MEDIA_STATE の値と and をとることで、
		 *			状態がわかります。
		 *	@par 例：
		 *	ファイナライズしているかチェック.
		 *	\code
		 *		// CDrive drv;
		 *		int status = drv.GetStatusOnDisc();
		 *		bool isFinalized = (status & IMAPI_FORMAT2_DATA_MEDIA_STATE_FINALIZED) != 0);
		 *		
		 *	\endcode
		 */
		int GetStatusOnDisc(void) const
		{
			IMAPI_FORMAT2_DATA_MEDIA_STATE s = IMAPI_FORMAT2_DATA_MEDIA_STATE_UNKNOWN;
			if ( ! m_Writer().IsNull() )
			{
				CheckRes(m_Writer()->get_CurrentMediaStatus(&s));
			}
			return s;
		}

		/**
		 * [取得] ディスクボリューム名取得.
		 *	@return ディスクボリューム名取得。ディスクがないと Empty 状態が返ります。
		 */
		CStr GetVolumeOnDisc(void) const
		{
			CStrVector mountPaths;
			if ( EnumerationMountPath(mountPaths) )
			{
				loop ( i, mountPaths.GetSize() )
				{
					CStr path;
					DWORD serialNumber;
					DWORD maxLen;
					DWORD fileSystemFlags;
					bool r = !!::GetVolumeInformation(mountPaths[i], path.GetBuffer(1000), 1000
									, &serialNumber, &maxLen, &fileSystemFlags, NULL, 0);
					if ( r )
					{
						path.ReleaseBuffer();
						return path;
					}
				}
			}
			return _T("");
		}

		/**
		 * [確認] ブランクディスク確認.
		 *	@retval true ブランクディスク。
		 *	@retval false ディスク無しか、データのあるディスク。
		 */
		bool IsBlankDisc(void) const
		{
			if ( ! m_Writer().IsNull() )
			{
				VARIANT_BOOL isBlank = FALSE;
				HRESULT hr = m_Writer()->get_MediaHeuristicallyBlank(&isBlank);
				if ( CheckRes(hr) )
				{
					return !! isBlank;
				}
			}
			return false;
		}

		/**
		 * [確認] 追記可能ディスク確認.
		 *	@retval true 追記可能なディスク。
		 *	@retval false 不可能。
		 */
		bool IsAppendableDisc(void) const
		{
			return (GetStatusOnDisc() & IMAPI_FORMAT2_DATA_MEDIA_STATE_APPENDABLE) != 0;
		}

		/// ディスクタイプ配列型
		typedef CVectorT<IMAPI_MEDIA_PHYSICAL_TYPE> CTypeVector;

		/**
		 * [取得] 対応ディスクタイプ一覧取得.
		 *	@param[out] _types 対応ディスクタイプの配列が格納されます。
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool EnumerationSupportedDiscTypes(CTypeVector& _types) const
		{
			_types.RemoveAll();
			SAFEARRAY* pSA = NULL;
			if ( ! m_Writer().IsNull() && CheckRes(m_Writer()->get_SupportedMediaTypes(&pSA)) )
			{
				VARIANT* P = static_cast<VARIANT*>(pSA->pvData);
				loop ( i, pSA->rgsabound[0].cElements )
				{
					ASSERT( P[i].vt == VT_I4 );
					IMAPI_MEDIA_PHYSICAL_TYPE t = static_cast<IMAPI_MEDIA_PHYSICAL_TYPE>(P[i].lVal);
					_types.Add(t);
				}
				::SafeArrayDestroyData(pSA);
				pSA = NULL;
				return true;
			}
			return false;
		}

		/**
		 * [取得] ディスク種類取得.
		 *		デバイスに入っているディスクの種類を取得できます。
		 *	@retval IMAPI_MEDIA_TYPE_UNKNOWN ディスクが入っていません。
		 *	@retval 上記以外 ディスク種類
		 */
		IMAPI_MEDIA_PHYSICAL_TYPE GetTypeOnDisc(void) const
		{
			IMAPI_MEDIA_PHYSICAL_TYPE type = IMAPI_MEDIA_TYPE_UNKNOWN;
			if ( ! m_Writer().IsNull() )
			{	
				CheckRes(m_Writer()->get_CurrentPhysicalMediaType(&type));
			}
			return type;
		}

		/**
		 * [取得] ディスク種類名取得.
		 *		デバイスに入っているディスクの種類名を取得できます。
		 *	@retval empty ディスクが入っていません。
		 *	@retval empty以外 ディスク種類名
		 */
		CStr GetTypeStringOnDisc(void) const
		{
			return CImageMastering::ToString(GetTypeOnDisc());
		}

		/**
		 * [取得] セッション情報取得.
		 *		指定のディスクイメージクラスに、セッション情報を読み込みます。
		 *		同時にボリューム名も読み込みます。
		 *	@param[in,out] _diskImage ディスクイメージ.
		 *	@retval true 成功。
		 *	@retval false 失敗。詳細は GetLastResult() / GetLastResultString() を使います。
		 */
		bool ReadMultiSession(CDiscImage& _diskImage) const
		{
			IFileSystemImage* pImage = _diskImage.operator ->();
			bool r = m_MultiDisp(pImage);
			CStr s = GetVolumeOnDisc();
			if ( ! s.IsEmpty() )
			{
				_diskImage.SetVolumeName(s);
			}
			return r;
		}

	private:
		/// 書き込み
		bool m_Burn(IStream* pStream)
		{
			ASSERT( pStream != NULL );
			CComObject<CImageMasteringWriterEvent>* pEventSink = NULL;
			HRESULT hr = CComObject<CImageMasteringWriterEvent>::CreateInstance(&pEventSink);
			if ( SUCCEEDED(hr) )
			{
				ASSERT( pEventSink != NULL );
				pEventSink->AddRef();
				pEventSink->m_pListener = &m_member;
				pEventSink->DispEventAdvise(m_Writer());
				hr = m_Writer()->Write(pStream);
				pEventSink->DispEventUnadvise(m_Writer());
				pEventSink->Release();
				pEventSink = NULL;
			}
			return CheckRes(hr);
		}
		// Close the disc if specified
		bool m_SetCloseDiscMode(bool isClose = true)
		{
			HRESULT hr = m_Writer()->put_ForceMediaToBeClosed(isClose ? VARIANT_TRUE : VARIANT_FALSE);
			return CheckRes(hr);
		}
		// verify the StartAddressOfPreviousSession property 
		// ALSO -- for DVD+R DL, if from sector zero, set to finalize media
		bool m_CheckDvdplusrdl(void)
		{
			return true;
#if 0
			LONG value = 0;
			HRESULT hr = m_Writer()->get_StartAddressOfPreviousSession(&value);
			if ( SUCCEEDED(hr) && value == -1 )
			{
				hr = m_Writer()->put_ForceMediaToBeClosed(VARIANT_TRUE);
			}
			return CheckRes(hr);
#endif
		}
		// ImportFileSystem - Import file data from disc
		bool m_MultiDisp(IFileSystemImage* pImage) const
		{
			SAFEARRAY* pSA = NULL;
			if ( SUCCEEDED(pImage->get_MultisessionInterfaces(&pSA)) )
			{
				ULONG l = pSA->rgsabound[0].cElements;
				::SafeArrayDestroyData(pSA);
				if ( l > 0 )
				{
					// セッション登録済み
					return true;
				}
			}
			SAFEARRAY* pMultiSession = NULL;
			// Get mutlisession interface to set in image
			HRESULT hr = m_Writer()->get_MultisessionInterfaces(&pMultiSession);
			// Set the multisession interface in the image
			if ( SUCCEEDED(hr) )
			{
				hr = pImage->put_MultisessionInterfaces(pMultiSession);
			}
			if ( SUCCEEDED(hr) )
			{
				FsiFileSystems fileSystems;
				hr = pImage->ImportFileSystem(&fileSystems);
				if ( hr == IMAPI_E_EMPTY_DISC )
				{
					// Empty Disc は OK 
					hr = S_OK;
				}
			}
			if ( pMultiSession != NULL )
			{
				::SafeArrayDestroy(pMultiSession);
				pMultiSession = NULL;
			}
			return CheckRes(hr);
		}
		/// トレイ操作
		bool m_Tray(bool isOpen)
		{
			if ( ! m_Recorder().IsNull() )
			{
				if ( CheckRes(m_Recorder()->DisableMcn()) )
				{
					bool r = CheckRes(isOpen ? m_Recorder()->EjectMedia() : m_Recorder()->CloseTray());
					m_Recorder()->EnableMcn();
					return r;
				}
			}
			return false;
		}
		/// 文字列取得種類
		enum EBstrKind
		{
			BstrKind_VendorId,				///< VendorId
			BstrKind_ProductId,				///< ProductId
			BstrKind_ProductRevision,		///< ProductRevision
			BstrKind_ExclusiveAccessOwner,	///< ExclusiveAccessOwner
		};
		/// 文字列取得
		CStr m_GetBstrKindString(EBstrKind k) const
		{
			CStr s;
			if ( ! m_Recorder().IsNull() )
			{
				CComBSTR str;
				HRESULT hr = E_FAIL;
				switch ( k )
				{
				case BstrKind_VendorId:
					hr = m_Recorder()->get_VendorId(&str);
					break;
				case BstrKind_ProductId:
					hr = m_Recorder()->get_ProductId(&str);
					break;
				case BstrKind_ProductRevision:
					hr = m_Recorder()->get_ProductRevision(&str);
					break;
				case BstrKind_ExclusiveAccessOwner:
					hr = m_Recorder()->get_ExclusiveAccessOwner(&str);
					break;
				default:
					break;
				}
				if ( CheckRes(hr) )
				{
					s = str;
				}
			}
			return s;
		}
		// PutRecorder 済み DiscFormat 作成
		template<typename TYP>
		void m_AdjustHandle(TYP& hp) const
		{
			if ( ! CheckRes(hp->put_Recorder(m_Recorder())) )
			{
				hp.Null();
			}
			else
			{
				CComBSTR s = _T("TnbImageMastering");
				if ( ! CheckRes(hp->put_ClientName(s)) )
				{
					hp.Null();
				}
			}
		}
		// PutRecorder 済み DiscFormatData 作成
		CDiscFormatDataHandle m_CreateDiscFormatData(void) const
		{
			CDiscFormatDataHandle hp;
			if ( ! m_Recorder().IsNull() )
			{
				IDiscFormat2Data* P = NULL;
				if ( CheckRes(::CoCreateInstance(CLSID_MsftDiscFormat2Data, NULL, CLSCTX_ALL, IID_PPV_ARGS(&P))) )
				{
					hp = P;
					m_AdjustHandle(hp);
				}
			}
			return hp;
		}
		// PutRecorder 済み DiscFormatErase 作成
		CDiscFormatEraseHandle m_CreateDiscFormatErase(void) const
		{
			CDiscFormatEraseHandle hp;
			if ( ! m_Recorder().IsNull() )
			{
				IDiscFormat2Erase* P = NULL;
				if ( CheckRes(::CoCreateInstance(CLSID_MsftDiscFormat2Erase, NULL, CLSCTX_ALL, IID_PPV_ARGS(&P))) )
				{
					hp = P;
					m_AdjustHandle(hp);
				}
			}
			return hp;
		}

		//----

		#ifndef _TnbDOXYGEN	//Document作成用シンボル
		
		/// レコーダー
		CDiscRecorderHandle& m_Recorder(void) const
		{
			return m_member.hpRecorder;
		}
		/// ライター
		CDiscFormatDataHandle& m_Writer(void) const
		{
			return m_member.hpWriter;
		}
		/// プログレスデータ
		class CMyProgressData : public IListener::CProgressData
		{
		public:
			/// コンストラクタ
			CMyProgressData(void) : m_elapsedTime(0), m_expectedTime(0)
				, m_startLba(0), m_sectorCount(0), m_lastWrittenLba(0)
			{}
			LONG GetRemainingTime(void) const { return m_expectedTime - m_elapsedTime; }
			LONG GetElapsedTime(void) const { return m_elapsedTime; }
			LONG GetExpectedTime(void) const { return m_expectedTime; }
			LONG GetStartLba(void) const { return m_startLba; }
			LONG GetTotalSectorCount(void) const { return m_sectorCount; }
			LONG GetWrittenLba(void) const { return m_lastWrittenLba; }
			LONG GetWrittenSector(void) const { return m_lastWrittenLba - m_startLba; }
			//
			LONG m_elapsedTime;			///< 経過時間
			LONG m_expectedTime;		///< 予想全時間
			LONG m_startLba;			///< 開始LBA
			LONG m_sectorCount;			///< 書込みセクタサイズ
			LONG m_lastWrittenLba;		///< 書込み中のLBA
		};
		/// メンバー型
		struct TMember : IImageMasteringWriterListener, IImageMasteringEraseListener
		{
			/// コンストラクタ
			TMember(void) : pListener(NULL)
			{}
			//書き込みイベント
			virtual void OnWriterEvent(IDispatch* pObjectDisp, IDispatch* pProgressDisp)
			{
				if ( pListener == NULL ) { return; }
				IDiscFormat2DataEventArgs* pProgress = NULL;
				HRESULT hr = pProgressDisp->QueryInterface(IID_PPV_ARGS(&pProgress));
				if ( FAILED(hr) )
				{
					pListener->OnDriveErrorResult(hr);
					return;
				}
				IMAPI_FORMAT2_DATA_WRITE_ACTION currentAction = IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA;
				hr = pProgress->get_CurrentAction(&currentAction);
				if ( FAILED(hr) )
				{
					pListener->OnDriveErrorResult(hr);
					return;
				}
				CMyProgressData pd;
				pProgress->get_ElapsedTime(&pd.m_elapsedTime);
				pProgress->get_TotalTime(&pd.m_expectedTime);
				pProgress->get_StartLba(&pd.m_startLba);
				pProgress->get_SectorCount(&pd.m_sectorCount);
				pProgress->get_LastWrittenLba(&pd.m_lastWrittenLba);
				switch ( currentAction )
				{
				case IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION:
				case IMAPI_FORMAT2_DATA_WRITE_ACTION_COMPLETED:
					pd.m_elapsedTime = pd.m_expectedTime;
					pd.m_lastWrittenLba = pd.m_startLba + pd.m_sectorCount;
					break;
				default:
					break;
				}
				if ( ! pListener->OnDriveWriteEvent(pd, currentAction) )
				{
					hpWriter->CancelWrite();
				}
			}	
			//消去イベント
			virtual void OnEraseEvent(IDispatch* object, LONG elapsedSeconds, LONG expectedSeconds)
			{
				if ( pListener != NULL )
				{
					CMyProgressData pd;
					pd.m_elapsedTime = elapsedSeconds;
					pd.m_expectedTime = expectedSeconds;
					pListener->OnDriveEraseEvent(pd);
				}
			}
			IListener*				pListener;	///< リスナー
			CDiscRecorderHandle		hpRecorder;	///< レコーダー
			CDiscFormatDataHandle	hpWriter;	///< ライター
		};
		mutable TMember			m_member;		///< メンバー

		#endif //_TnbDOXYGEN
	};


	//------------------------------------------------


	/// コンストラクタ
	CImageMastering(void)
	{
	}

	/**
	 * [確認] サポート確認.
	 *		PCが本機能をサポートしているか、確認出来ます。
	 *	@retval true サポートしている.
	 *	@retval false サポートしていない。
	 */
	bool IsSupported(void) const
	{
		m_CheckInst();
		if ( ! m_hpDiscMaster.IsNull() )
		{
			VARIANT_BOOL b;
			HRESULT hr = m_hpDiscMaster->get_IsSupportedEnvironment(&b);
			if ( SUCCEEDED(hr) )
			{
				return b != 0;
			}
		}
		return false;
	}

	/**
	 * [取得] ドライブ数取得.
	 *	@note 書き込みをサポートしていないドライブも数に含まれます。
	 *	@return ドライブ数.
	 */
	size_t GetDriveCount(void) const
	{
		m_CheckInst();
		if ( ! m_hpDiscMaster.IsNull() )
		{
			LONG cnt;
			HRESULT hr = m_hpDiscMaster->get_Count(&cnt);
			if ( SUCCEEDED(hr) )
			{
				return cnt;
			}
		}
		return 0;
	}

	/**
	 * [取得] ドライブ名取得.
	 *	@note "q:\\"などではありません。ドライブを示すユニークな文字列です。
	 *	@param index インデックス。０～ GetDriveCount() 未満を指定出来ます。
	 *	@return ドライブ名.
	 */
	CStr GetDriveName(INDEX index) const
	{
		CStr s;
		if ( GetDriveCount() > index )
		{
			CComBSTR str;
			HRESULT hr = m_hpDiscMaster->get_Item(index, &str);
			if ( SUCCEEDED(hr) )
			{
				s = str;
			}
		}
		return s;
	}

	/**
	 * [取得] ドライブ管理クラス取得.
	 *		指定のドライブ管理クラスを取得します。書き込みをサポートしていないドライブの場合、
	 *		無効なクラスが取得されます。
	 *	@param index インデックス。０～ GetDriveCount() 未満を指定出来ます。
	 *	@return デバイス管理クラス. CDrive::IsValid() で、有効なクラスか確認できます。
	 */
	CDrive CreateDrive(INDEX index) const
	{
		IDiscRecorder2* P = NULL;
		CheckRes(::CoCreateInstance(CLSID_MsftDiscRecorder2, NULL, CLSCTX_ALL, IID_PPV_ARGS(&P)));
		return CDrive(P, GetDriveName(index));
	}

	/**
	 * [取得] ディスク種類文字取得.
	 *	@param type ディスク種類
	 *	@return ディスク種類文字
	 */
	static CStr ToString(IMAPI_MEDIA_PHYSICAL_TYPE type)
	{
		const static CStaticMapT<IMAPI_MEDIA_PHYSICAL_TYPE,LPCTSTR> s_map[] = 
		{
			IMAPI_MEDIA_TYPE_UNKNOWN,				_T(""),
			IMAPI_MEDIA_TYPE_CDROM,					_T("CD-ROM"),
			IMAPI_MEDIA_TYPE_CDR,					_T("CD-R"),
			IMAPI_MEDIA_TYPE_CDRW,					_T("CD-RW"),
			IMAPI_MEDIA_TYPE_DVDROM,				_T("DVD-ROM"),
			IMAPI_MEDIA_TYPE_DVDRAM,				_T("DVD-RAM"),
			IMAPI_MEDIA_TYPE_DVDPLUSR,				_T("DVD+R"),
			IMAPI_MEDIA_TYPE_DVDPLUSRW,				_T("DVD+RW"),
			IMAPI_MEDIA_TYPE_DVDPLUSR_DUALLAYER,	_T("DVD+R DL"),
			IMAPI_MEDIA_TYPE_DVDDASHR,				_T("DVD-R"),
			IMAPI_MEDIA_TYPE_DVDDASHRW,				_T("DVD-RW"),
			IMAPI_MEDIA_TYPE_DVDDASHR_DUALLAYER,	_T("DVD-R DL"),
			IMAPI_MEDIA_TYPE_DISK,					_T("DISK"),
			IMAPI_MEDIA_TYPE_DVDPLUSRW_DUALLAYER,	_T("DVD+RW DL"),
			IMAPI_MEDIA_TYPE_HDDVDROM,				_T("HD DVD-ROM"),
			IMAPI_MEDIA_TYPE_HDDVDR,				_T("HD DVD-R"),
			IMAPI_MEDIA_TYPE_HDDVDRAM,				_T("HD DVD-RAM"),
			IMAPI_MEDIA_TYPE_BDROM,					_T("BD-ROM"),
			IMAPI_MEDIA_TYPE_BDR,					_T("BD-R"),
			IMAPI_MEDIA_TYPE_BDRE,					_T("BD-RE"),
		};
		return CStaticMapT<IMAPI_MEDIA_PHYSICAL_TYPE, LPCTSTR>::Find(type, s_map);
	}

private:
	// インスタンスチェック.
	bool m_CheckInst(void) const
	{
		if ( m_hpDiscMaster.IsNull() )
		{
			IDiscMaster2* P = NULL;
			CheckRes(::CoCreateInstance(CLSID_MsftDiscMaster2, NULL, CLSCTX_ALL, IID_PPV_ARGS(&P)));
			m_hpDiscMaster = P;
		}
		return ! m_hpDiscMaster.IsNull();
	}
	mutable CDiscMasterHandle	m_hpDiscMaster;
};



}; //TNB



#ifdef _TnbDOXYGEN	//Document作成用シンボル

/**@ingroup IMAPI2
 * 書き込みアクション.
 *	@see TNB::CImageMastering::CDrive::IListener::OnDriveWriteEvent()
 */
enum IMAPI_FORMAT2_DATA_WRITE_ACTION
{
	IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA,		///< ディスク確認中
	IMAPI_FORMAT2_DATA_WRITE_ACTION_FORMATTING_MEDIA,		///< ディスクフォーマット中
	IMAPI_FORMAT2_DATA_WRITE_ACTION_INITIALIZING_HARDWARE,	///< 初期化中
	IMAPI_FORMAT2_DATA_WRITE_ACTION_CALIBRATING_POWER,		///< パワー調整中
	IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA,			///< データ書き込み中
	IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION,			///< ファイナライズ中
	IMAPI_FORMAT2_DATA_WRITE_ACTION_COMPLETED,				///< 完了
};

/**@ingroup IMAPI2
 * ディスク状態.
 *	@see TNB::CImageMastering::CDrive::GetStatusOnDisc()
 */
enum IMAPI_FORMAT2_DATA_MEDIA_STATE
{
	IMAPI_FORMAT2_DATA_MEDIA_STATE_OVERWRITE_ONLY	= 0x1,			///< 上書きのみ
	IMAPI_FORMAT2_DATA_MEDIA_STATE_BLANK	= 0x2,					///< ブランク
	IMAPI_FORMAT2_DATA_MEDIA_STATE_APPENDABLE	= 0x4,				///< 追記可能
	IMAPI_FORMAT2_DATA_MEDIA_STATE_FINAL_SESSION	= 0x8,			///< 最後のセクション
	IMAPI_FORMAT2_DATA_MEDIA_STATE_DAMAGED	= 0x400,				///< ダメージ
	IMAPI_FORMAT2_DATA_MEDIA_STATE_ERASE_REQUIRED	= 0x800,		///< 消去必要あり
	IMAPI_FORMAT2_DATA_MEDIA_STATE_NON_EMPTY_SESSION	= 0x1000,	///< セクション有り
	IMAPI_FORMAT2_DATA_MEDIA_STATE_WRITE_PROTECTED	= 0x2000,		///< 書き込み禁止
	IMAPI_FORMAT2_DATA_MEDIA_STATE_FINALIZED	= 0x4000,			///< ファイナライズ済み
	IMAPI_FORMAT2_DATA_MEDIA_STATE_UNSUPPORTED_MEDIA	= 0x8000	///< 未サポートディスク
};

/**@ingroup IMAPI2
 * ディスクタイプ.
 *	@see TNB::CImageMastering::CDrive::GetTypeOnDisc()
 */
enum IMAPI_MEDIA_PHYSICAL_TYPE
{
	IMAPI_MEDIA_TYPE_UNKNOWN,				///< 不明,
	IMAPI_MEDIA_TYPE_CDROM,					///< CD-ROM
	IMAPI_MEDIA_TYPE_CDR,					///< CD-R
	IMAPI_MEDIA_TYPE_CDRW,					///< CD-RW
	IMAPI_MEDIA_TYPE_DVDROM,				///< DVD-ROM
	IMAPI_MEDIA_TYPE_DVDRAM,				///< DVD-RAM
	IMAPI_MEDIA_TYPE_DVDPLUSR,				///< DVD+R
	IMAPI_MEDIA_TYPE_DVDPLUSRW,				///< DVD+RW
	IMAPI_MEDIA_TYPE_DVDPLUSR_DUALLAYER,	///< DVD+R DL
	IMAPI_MEDIA_TYPE_DVDDASHR,				///< DVD-R
	IMAPI_MEDIA_TYPE_DVDDASHRW,				///< DVD-RW
	IMAPI_MEDIA_TYPE_DVDDASHR_DUALLAYER,	///< DVD-R DL
	IMAPI_MEDIA_TYPE_DISK,					///< DISK
	IMAPI_MEDIA_TYPE_DVDPLUSRW_DUALLAYER,	///< DVD+RW DL
	IMAPI_MEDIA_TYPE_HDDVDROM,				///< HD DVD-ROM
	IMAPI_MEDIA_TYPE_HDDVDR,				///< HD DVD-R
	IMAPI_MEDIA_TYPE_HDDVDRAM,				///< HD DVD-RAM
	IMAPI_MEDIA_TYPE_BDROM,					///< BD-ROM
	IMAPI_MEDIA_TYPE_BDR,					///< BD-R
	IMAPI_MEDIA_TYPE_BDRE,					///< BD-RE
};

#endif


#if 0

todo;

	When unlocking a recorder, the lock count must reach zero to free 
	the device for other clients. Calling the IDiscRecorder2::ReleaseExclusiveAccess 
	method decrements the lock count by one.

	An equal number of calls to the AcquireExclusiveAccess and 
	ReleaseExclusiveAccess methods are needed to free a device. 
	Should the application exit unexpectedly or crash while holding 
	the exclusive access, the CDROM.SYS driver will automatically 
	release these exclusive locks.

	If the device is already locked, you can call IDiscRecorder2::get_ExclusiveAccessOwner 
	to retrieve the name of the client application that currently has exclusive access.

todo;

	VC++6 、MFC で作成したダイアログ上で、書き込みしている時、
	フォーカス移動で、なぞの アクセスバイオレーション発生！

#endif

