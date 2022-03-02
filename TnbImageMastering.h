#pragma once
/**
 *@file
 * IMAPI2�֌W�̃w�b�_
 *
 *		IMAPI2 (Image Mastering API Version 2)
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef __ATLCOM_H__
 #error TnbImageMastering.h is necessary fot OLE Object.
#endif



#include "TnbPointerHandle.h"
#include "TnbStaticMap.h"
#include "TnbStrVector.h"
#include "TnbTickCount.h"
#include "TnbDynamicFunc.h"

#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

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



/**@defgroup IMAPI2		IMAPI2 �֌W
 *@ingroup FILE
 * IMAP2 �֌W.
 *
 *			IMAPI2 (Image Mastering API Version 2)���g�����A
 *			CD-R/RW,DVD�}R/RW �̏����݂��T�|�[�g����N���X���p�ӂ��Ă���܂��B
 *		
 *		\attention CoInitialize() �ł͂Ȃ� OleInitialize() ���g���K�v������܂�
 *			(�v���W�F�N�g�쐬���ɁA�u�I�[�g���[�V�����L��v�ɂ���A�Ȃ�)�B
 *
 *		\par ��:
 *		- �h���C�u�ꗗ
 *		\code
 *			CImageMastering im;
 *			loop ( i, im.GetDriveCount() )
 *			{
 *				CImageMastering::CDrive drv = im.CreateDrive(i);
 *				if ( drv.IsValid() )
 *				{  // �L���ȃh���C�u����
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
 *		- �t�H���_ c:\\Temp �ȉ��� �f�B�X�N�� \\ABC �ȉ��ɏ������݁A d:\\data\\memo.txt ���f�B�X�N�� \\ABC\\001 �ɏ�������
 *		\code
 *			// CImageMastering::CDrive drv; // �L���ȃh���C�u
 *			if ( ! drv.IsBlankDisc() ) {
 *				drv.Erase();	//�u�����N�łȂ��̂ō폜
 *			}
 *			CImageMastering::CDiscImage di;
 *			di.AddSourceDirectory(_T("c:\\Temp"), _T("\\ABC"));
 *			di.AddSourceFile(_T("d:\\data\\memo.txt"), _T("\\ABC\\001"));
 *			di.SetVolumeName(_T("�{�����[��"));		//�{�����[����
 *			drv.Burn(di, true);		// �����āA����
 *		\endcode
 *		- �N���[�Y���Ă��Ȃ��f�B�X�N�� \\ABC\\A.txt ���폜����B
 *		\code
 *			// CImageMastering::CDrive drv; // �L���ȃh���C�u
 *			CImageMastering::CDiscImage di;
 *			drv.ReadMultiSession(di); //�Z�b�V�������ǂݍ���
 *			di.RemoveFile(_T("\\ABC\\A.txt"));
 *			drv.Burn(di, false);		// ����
 *		\endcode
 *		- �N���[�Y���Ă��Ȃ��f�B�X�N���N���[�Y���邾��
 *		\code
 *			// CImageMastering::CDrive drv; // �L���ȃh���C�u
 *			CImageMastering::CDiscImage di;
 *			di.SetVolumeName(_T("�{�����[��")); //�{�����[������ς���Ȃ�
 *			drv.Burn(di, true);		// �����āA����
 *		\endcode
 *
 *	@see CImageMastering
 *
 */



}; //TNB



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

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

// LIB �� 'Lib2' �� '2FS'
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

/// �������݃��X�i�[
struct 	IImageMasteringWriterListener
{
	virtual void OnWriterEvent(IDispatch* pObject, IDispatch* pProgressDisp) = 0;
};

/// �������݃R�[���o�b�N�N���X
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

/// �������X�i�[
struct 	IImageMasteringEraseListener
{
	virtual void OnEraseEvent(IDispatch* pObject, LONG elapsedSeconds, LONG expectedSeconds) = 0;
};

/// �����R�[���o�b�N�N���X
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

/// File���X�i�[
struct 	IImageMasteringFileListener
{
	virtual void OnFileEvent(IDispatch* pObject, BSTR currentFile, LONG copiedSectors, LONG totalSectors) = 0;
};

/// �t�@�C���R�[���o�b�N�N���X
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

/// �}�X�^�[���X�i�[
struct 	IImageMasteringListener
{
	virtual void OnAddedDevice(IDispatch* pObject, BSTR value);
	virtual void OnRemovedDevice(IDispatch* pObject, BSTR value);
};

/// �}�X�^�[�C�x���g
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

/// PointerHandleBase�p �ėp �|�C���^�����[�X�e���v���[�g�N���X
template<typename T> struct TImReleasePointerT
{
	/// �����[�X. @param P �����[�X�Ώ�
	void operator()(T P) { P->Release(); }
};

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup IMAPI2
 * IMAPI2���U���g�Ǘ�.
 *
 *		���U���g��������ێ�����N���X�ł��B
 *
 *		CImageMastering �N���X�ȉ��ɁA�g�p����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbImageMastering.h
 *
 *	@date 09/08/08 �V�K�쐬
 */
class CImageMasteringResult
{
public:
	
	/// �R���X�g���N�^
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
	 * [�擾] ���U���g�R�[�h�擾.
	 *	@return ���U���g�R�[�h.
	 */
	HRESULT GetLastResult(void) const
	{
		return m_lastResult;
	}

	/**
	 * [�擾] ���U���g������擾.
	 *	@return ���U���g������.
	 */
	CStr GetLastResultString(void) const
	{
		return ToString(m_lastResult);
	}

	/**
	 * [�ϊ�] ���U���g������ϊ�.
	 *		���U���g�R�[�h�𕶎���ɕϊ�
	 *	@param hr ���U���g�R�[�h
	 *	@return ������
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
	 * [�쐬] �t�@�C���X�g���[���쐬.
	 *	@note ::SHCreateStreamOnFile() ���R�[�����Ă��邾���ł��B
	 *	@param[in] lpszFile �t�@�C����
	 *	@param[in] grfMode ���[�h
	 *	@param[out] ppstm �X�g���[��
	 *	@return ���U���g
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
	 * [�ݒ�] ���U���g�`�F�b�N.
	 *		���U���g�R�[�h���`�F�b�N���A�L�����܂��B
	 *	@param hr ���U���g�R�[�h
	 *	@retval true ���U���g�R�[�h���u�����v��
	 *	@retval false ���U���g�R�[�h���u���s�v��
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
 * IMAPI2�Ǘ�.
 *
 *	@note �f�t�H���g�� XP �� IMAPI2 �͊܂܂�Ă��܂���B
 *			�ȉ��̃T�[�o����_�E�����[�h���ăC���X�g�[������K�v������܂��B
 *	- (�K�{) Windows XP �p Image Mastering API v2.0 (IMAPIv2.0)\n
 *		http://www.microsoft.com/downloads/details.aspx?familyid=B5F726F1-4ACE-455D-BAD7-ABC4DD2F147B&displaylang=ja
 *	- (�C��) Windows Feature Pack 1.0\n
 *		http://www.microsoft.com/downloads/details.aspx?FamilyID=63ab51ea-99c9-45c0-980a-c556746fcf05&DisplayLang=ja
 *
 *	@par�K�v�t�@�C��
 *			TnbImageMastering.h
 *
 *	@date 09/08/08 �V�K�쐬
 */
class CImageMastering : public CImageMasteringResult, CCopyImpossible
{
public:

	/// DiscMaster �n���h��
	typedef CPointerHandleBaseT<IDiscMaster2*, TImReleasePointerT<IDiscMaster2*> > CDiscMasterHandle;

	/// DiscRecorder �n���h��
	typedef CPointerHandleBaseT<IDiscRecorder2*, TImReleasePointerT<IDiscRecorder2*> > CDiscRecorderHandle;

	/// FileSystemImage �n���h��
	typedef CPointerHandleBaseT<IFileSystemImage*, TImReleasePointerT<IFileSystemImage*> > CFileSystemImageHandle;

	/// DiscFormatData �n���h��
	typedef CPointerHandleBaseT<IDiscFormat2Data*, TImReleasePointerT<IDiscFormat2Data*> > CDiscFormatDataHandle;

	/// DiscFormatErase �n���h��
	typedef CPointerHandleBaseT<IDiscFormat2Erase*, TImReleasePointerT<IDiscFormat2Erase*> > CDiscFormatEraseHandle;


	//-------------------------------------------------------


	/**@ingroup IMAPI2
	 * �f�B�X�N�C���[�W�Ǘ�.
	 *
	 *		�f�B�X�N��̃t�@�C���z�u���Ǘ�����N���X�ł��B
	 *		CDrive �N���X�Ńf�B�X�N�Ƀt�@�C�����������ނ��߂ɁA
	 *		�܂��A���̃N���X�Ƀt�@�C����z�u���܂��B
	 *
	 *	@note CDrive::ReadMultiSession() ���g���A
	 *			���̃N���X�Ƀ}���`�Z�b�V��������ǂݍ��݁A���삷�邱�Ƃ��o���܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbImageMastering.h
	 *
	 *	@date 09/08/08 �V�K�쐬
	 *	@date 09/08/20 ���X�i�[�ǉ��B�C���[�W�t�@�C���쐬���T�|�[�g
	 */
	class CDiscImage : public CImageMasteringResult, CCopyImpossible, IImageMasteringFileListener
	{
	public:

		/**@ingroup IMAPI2
		 * �f�B�X�N�C���[�W�Ǘ����X�i�[.
		 *
		 *		�t�@�C���o�^���ɐi����ʒm���邽�߂̃��X�i�[�ł��B
		 *
		 *	@see CDrive::SetListener 
		 *	
		 *	@par�K�v�t�@�C��
		 *			TnbImageMastering.h
		 *
		 *	@date 09/08/20 �V�K�쐬
		 */
		struct IListener
		{
			/// �f�X�g���N�^
			virtual ~IListener(void) {}

			/**
			 * [�ʒm] �i���ʒm.
			 *	@note �Z�N�^���́A�ǉ����t�@�C�������̒l�ɂȂ�܂��B�S�̂ł͂���܂���B
			 *	@param file �ǉ����t�@�C����.
			 *	@param copiedSectors �ǉ��Z�N�^��
			 *	@param totalSectors �S�Z�N�^��
			 */
			virtual void OnDiscImageAdded(LPCTSTR file, LONG copiedSectors, LONG totalSectors) = 0;
		};

		/// �R���X�g���N�^
		CDiscImage(void) : m_useJoliet(true), m_pListener(NULL), m_pEventSink(NULL), m_pRoot(NULL)
		{
			Reset();
		}

		/// �f�X�g���N�^
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
		 * [�m�F] �L���m�F.
		 *		�{�C���X�^���X���L�����A�m�F�ł��܂��B
		 *	@note �����ȏꍇ�A���\�b�h�́A���ׂĎ��s���܂��B
		 *	@retval true �L���B
		 *	@retval false �����B
		 */
		bool IsValid(void) const
		{
			return m_pRoot != NULL;
		}

		/**
		 * [�擾] FileSystemImage�C���^�[�t�F�[�X�擾.
		 *	@return FileSystemImage�C���^�[�t�F�[�X
		 */
		IFileSystemImage* operator->(void) const
		{
			return m_hpImage;
		}

		/**
		 * [�ݒ�] ������.
		 *		�o�^�����t�@�C���Ȃǂ��N���A���܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B
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
		 * [�ݒ�] Joliet���[�h�ݒ�.
		 *	@note CD-R/RW �ɏ����ꍇ�AJoliet �g���ŋL�^���邩�A�w��ł��܂��B
		 *		�f�t�H���g�ł� Joliet �g���ŋL�^���܂��B
		 *	@param isEnable false ���w�肷��ƁA ISO9660 �ŋL�^���܂��B
		 *					true ���w�肷��ƁA Joliet �ŋL�^���܂��B
		 */
		void SetJolietSystem(bool isEnable)
		{
			m_useJoliet = isEnable;
		}

		/**
		 * [�ݒ�n���X�i�[�o�^.
		 *	@note ���X�i�[��ǉ����邱�ƂŁA�t�@�C���ǉ��i���𓾂邱�Ƃ��o���܂��B
		 *	@param P ���X�i�[�B NULL ���w�肷�邱�ƂŁA�o�^�������ł��܂��B
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
		 * [�擾] �Z�N�^�T�C�Y�擾.
		 *	@note �P�Z�N�^�̃o�C�g����Ԃ��܂��B
		 *	@param isXA true �Ȃ� CD MODE2 XA �̏ꍇ�̃Z�N�^�T�C�Y��Ԃ��܂��B
		 *	@return �P�Z�N�^�̃o�C�g��
		 */
		size_t GetSectorSize(bool isXA = false) const
		{
			return isXA ? 2336 : 2048;
		}

		/**
		 * [�擾] �o�^�ς݃Z�N�^���擾.
		 *	@note AddSourceDirectory() , AddSourceFile() �œo�^���������������ނ̂ɁA
		 *		�K�v�ȃZ�N�^�����Ԃ�܂��B
		 *	@return �Z�N�^��
		 */
		size_t GetSectorsCount(void) const
		{
			LONG r;
			return CheckRes(m_hpImage->get_UsedBlocks(&r)) ? r : 0;
		}

		/**
		 * [�擾] �o�^�ς݃t�@�C�����擾.
		 *	@note AddSourceDirectory() , AddSourceFile() �œo�^�����t�@�C�������Ԃ�܂��B
		 *	@return �t�@�C����
		 */
		size_t GetFileCount(void) const
		{
			LONG r;
			return CheckRes(m_hpImage->get_FileCount(&r)) ? r : 0;
		}

		/**
		 * [�擾] �o�^�ς݃f�B���N�g�����擾
		 *	@note AddSourceDirectory() , AddSourceFile() �œo�^�����f�B���N�g�������Ԃ�܂��B
		 *	@return �f�B���N�g����
		 */
		size_t GetDirectoryCount(void) const
		{
			LONG r;
			return CheckRes(m_hpImage->get_DirectoryCount(&r)) ? r : 0;
		}

		/**
		 * [�擾] �{�����[�����擾.
		 *	@return �{�����[����
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
		 * [�ݒ�] �{�����[�����ݒ�.
		 *	@param lpszVolume �{�����[����
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool SetVolumeName(LPCTSTR lpszVolume)
		{
			CComBSTR s = lpszVolume;
			return CheckRes(m_hpImage->put_VolumeName(s));
		}

		/**
		 * [�ǉ�] �f�B���N�g���ǉ�.
		 *		�������ރt�@�C�����f�B���N�g�����Ɠo�^���܂��B
		 *	@note �w�肵���f�B���N�g�����͓̂o�^����܂���B���̒��g���ǉ��Ώۂł��B
		 *	\code
		 *			c:\temp\foo1.txt
		 *			c:\temp\foo2.txt
		 *		������ꍇ�A AddSourceDirectory("c:\\temp", "\\xyz"); �Ƃ���ƁA�f�B�X�N�ɂ�
		 *			\xyz\foo1.txt
		 *			\xyz\foo2.txt
		 *		�Ə������B
		 *	\endcode
		 *	@param lpszPath �ǉ����� PC �̃f�B���N�g�����B
		 *	@param lpszImagePath �f�B�X�N��̃f�B���N�g���B�ȗ�����ƃf�B�X�N�̃��[�g���w�肳��܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�ǉ�] �t�@�C���ǉ�.
		 *		�������ރt�@�C����o�^���܂��B
		 *	@param lpszName �o�^���� PC �̃t�@�C�����B
		 *	@param lpszImagePath �f�B�X�N��̃f�B���N�g���B�ȗ�����ƃf�B�X�N�̃��[�g���w�肳��܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�폜] �f�B���N�g���폜.
		 *		�w�肵���f�B���N�g���ȉ���o�^��񂩂�폜���܂��B
		 *	@note CDrive::ReadMultiSession() �ňȑO�̃Z�b�V������ǂݍ���ł���A
		 *		������g�����ƂŁA�f�B�X�N��̃t�@�C�����폜���邱�Ƃ��o���܂��B
		 *	@param lpszImagePath �폜����f�B�X�N��̃f�B���N�g���B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool RemoveDirectory(LPCTSTR lpszImagePath)
		{
			CComBSTR s = lpszImagePath;
			return CheckRes(m_pRoot->RemoveTree(s));
		}

		/**
		 * [�폜] �t�@�C���폜.
		 *		�w�肵���t�@�C����o�^��񂩂�폜���܂��B
		 *	@note CDrive::ReadMultiSession() �ňȑO�̃Z�b�V������ǂݍ���ł���A
		 *		������g�����ƂŁA�f�B�X�N��̃t�@�C�����폜���邱�Ƃ��o���܂��B
		 *	@param lpszImageName �폜����f�B�X�N��̃t�@�C�����B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool RemoveFile(LPCTSTR lpszImageName)
		{
			CComBSTR s = lpszImageName;
			return CheckRes(m_pRoot->Remove(s));
		}

		/**
		 * [�ݒ�] Boot�C���[�W�ݒ�.
		 *		�u�[�^�u�� CD ���쐬����ہABoot �C���[�W���w�肵�܂��B
		 *	@param lpszBootFileName �u�[�g�C���[�W�t�@�C����
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�ݒ�] ���[�N�f�B���N�g���ݒ�.
		 *		�ʏ�A�V�X�e���̃e���|�����t�H���_�����[�N�f�B���N�g���Ƃ��Ďg�p����܂��B
		 *		�{���\�b�h���g�����ƂŁA�ύX���邱�Ƃ��o���܂��B
		 *	@param lpszPath ���[�N�f�B���N�g��
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool SetWorkingDirestory(LPCTSTR lpszPath)
		{
			CComBSTR s = lpszPath;
			return CheckRes(m_hpImage->put_WorkingDirectory(s));
		}

		/**
		 * �t�@�C���A�C�e�����.
		 *	@see EnumerationFiles
		 */
		struct TFileItem
		{
			bool	isDirectory;	///< �f�B���N�g���t���O. true �Ȃ�f�B���N�g���A false �Ȃ�t�@�C��
			CStr	name;			///< �t�@�C��/�f�B���N�g����
		};

		/**
		 * [�擾] �t�@�C���ꗗ.
		 *		�{�C���X�^���X�ɓo�^����Ă���t�@�C���A�f�B���N�g���̈ꗗ��Ԃ��܂��B
		 *	@note CDrive::ReadMultiSession() , AddSourceFile() , AddSourceDirectory() , 
		 *		RemoveDirectory() , RemoveFile() �ɂ�鑀��̌��ʂ��킩��܂��B
		 *	@param[out] _items �t�@�C���A�C�e���ꗗ���i�[����܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool EnumerationFiles(CVectorT<TFileItem>& _items) const
		{
			return m_EnumFiles(_items, m_pRoot);
		}

		/**
		 * [�쐬] �C���[�W�t�@�C���쐬.
		 *		�{�C���X�^���X�ɓo�^����Ă���t�@�C���A�f�B���N�g������A�C���[�W�t�@�C�����쐬���܂��B
		 *	@param lpszFileName �쐬�C���[�W�t�@�C�����B
		 *	@param type �Ώۃf�B�X�N�^�C�v�B CD,CDR,CDRW���w�肷��� ISO9660(Joliet) �A����ȊO�� UDF �V�X�e���ŃC���[�W�t�@�C�����쐬����܂��B
		 *	@note CD,CDR,CDRW �̏ꍇ�A SetJolietSystem() �ɂ��A�쐬�����C���[�W���قȂ�܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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

		/** [�쐬] �C���[�W�X�g���[���쐬.
		 *		�{�C���X�^���X�ɓo�^����Ă���t�@�C���A�f�B���N�g������A�C���[�W�X�g���[�����쐬���܂��B
		 *	@param type �Ώۃf�B�X�N�^�C�v�B CD,CDR,CDRW���w�肷��� ISO9660(Joliet) �A����ȊO�� UDF �V�X�e���ŃC���[�W�t�@�C�����쐬����܂��B
		 *	@note CD,CDR,CDRW �̏ꍇ�A SetJolietSystem() �ɂ��A�쐬�����C���[�W���قȂ�܂��B
		 *	@retval NULL ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 *	@retval NULL�ȊO �����B�g�p��� Release() ����K�v������܂��B
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
		// [�擾] �t�@�C���V�X�e����ގ擾.
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
		// �f�B���N�g���ǉ�
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
		// �t�@�C���ǉ�
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
				//hr = _pDir->AddFile(name, pStream); //���ꂾ�Ɠ��t�����ɂȂ��Ă��܂�
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
		/// EnumFiles�A�T�u
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
		/// IMAPI2 ����̃R�[���o�b�N
		virtual void OnFileEvent(IDispatch* object, BSTR currentFile, LONG copiedSectors, LONG totalSectors)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnDiscImageAdded(CStr(currentFile), copiedSectors, totalSectors);
			}
		}
		bool									m_useJoliet;	///< true �̏ꍇ�A CD�� Joliet �V�X�e��
		IFsiDirectoryItem*						m_pRoot;		///< ���[�g�A�C�e��
		CFileSystemImageHandle					m_hpImage;		///< �C���[�W
		IListener*								m_pListener;	///< ���X�i�[
		CComObject<CImageMasteringFileEvent>*	m_pEventSink;	///< Sink
	};


	//-------------------------------------------------------------


	/**@ingroup IMAPI2
	 * �h���C�u�Ǘ�.
	 *
	 *		��� CD/DVD/BD �h���C�u���Ǘ����܂��B
	 *
	 *		�}������Ă���f�B�X�N�ɑ΂��Ă̏������݂�A�f�B�X�N�̎�ނ��Ԃ̊m�F�A�g���C�̃I�[�v���N���[�Y���T�|�[�g���܂��B
	 *
	 *	@note �������݂�����̐i���́A IListener �œ��邱�Ƃ��o���܂��B�܂��A�������݂̒�~���A IListener ���g���܂��B
	 *	
	 *	@note �{�C���X�^���X�� CImageMastering::CreateDrive() ���g���āA�������܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbImageMastering.h
	 *
	 *	@date 09/08/08 �V�K�쐬
	 */
	class CDrive : public CImageMasteringResult
	{
	public:

		/**@ingroup IMAPI2
		 * �h���C�u�Ǘ����X�i�[.
		 *
		 *		�����݁AErase���A�i����ʒm���邽�߂̃��X�i�[�ł��B
		 *
		 *	@see CDrive::SetListener 
		 *	
		 *	@par�K�v�t�@�C��
		 *			TnbImageMastering.h
		 *
		 *	@date 09/08/08 �V�K�쐬
		 */
		struct IListener
		{
			/**
			 * �i���f�[�^.
			 */
			class CProgressData
			{
			public:
				/**
				 * [�擾] �c�莞��.
				 *	@return �c�莞��(�b)
				 */
				virtual LONG GetRemainingTime(void) const = 0;
				/**
				 * [�擾] �o�ߎ���.
				 *	@return �o�ߎ���(�b)
				 */
				virtual LONG GetElapsedTime(void) const = 0;
				/**
				 * [�擾] �\�z�S����.
				 *	@return �\�z�S����(�b)
				 */
				virtual LONG GetExpectedTime(void) const = 0;
				/**
				 * [�擾] �J�n LBA.
				 *	@return �J�n LBA
				 */
				virtual LONG GetStartLba(void) const = 0;
				/**
				 * [�擾] �S�Z�N�^��.
				 *	@return �S�Z�N�^��
				 */
				virtual LONG GetTotalSectorCount(void) const = 0;
				/**
				 * [�擾] ������ LBA ��.
				 *	@return ������ LBA ��
				 */
				virtual LONG GetWrittenLba(void) const = 0;
				/**
				 * [�擾] �����݃Z�N�^��.
				 *	@return �����݃Z�N�^��
				 */
				virtual LONG GetWrittenSector(void) const = 0;
			};

			/// �f�X�g���N�^
			virtual ~IListener(void) {}

			/**
			 * [�ʒm] �G���[�����ʒm.
			 *	@note �G���[�����������ꍇ�A�ʒm���܂��B
			 *	@param hr ���U���g�R�[�h
			 */
			virtual void OnDriveErrorResult(HRESULT hr) = 0;

			/**
			 * [�ʒm] �������ݐi���ʒm.
			 *		�������݂̐i����ʒm���܂��B
			 *	@note false ��Ԃ����ƂŁA�������݂��~���邱�Ƃ��o���܂��B
			 *	@param pd �i���f�[�^.
			 *	@param action �������݃A�N�V����.
			 *	@retval true �������ݑ��s.
			 *	@retval false �������ݒ��~�v��.
			 */
			virtual bool OnDriveWriteEvent(const CProgressData& pd, IMAPI_FORMAT2_DATA_WRITE_ACTION action) = 0;

			/**
			 * [�ʒm] �����i���ʒm.
			 *	@note �����̐i����ʒm���܂��B
			 *	@param pd �i���f�[�^�B���ԏ��̂݁A�L���ł�.
			 */
			virtual void OnDriveEraseEvent(const CProgressData& pd) = 0;
		};


		//-----------------------------------


		/**
		 * �R���X�g���N�^.
		 *	@note �������������w�肵�Ȃ��ƁA�����ȃC���X�^���X�ɂȂ�܂��B
		 *	@param dr DiscRecorder
		 *	@param name �h���C�u��
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
		 * [�m�F] �L���m�F.
		 *		�{�C���X�^���X���L�����A�m�F�ł��܂��B
		 *	@note �����ȏꍇ�A���\�b�h�́A���ׂĎ��s���܂��B
		 *	@retval true �L���B
		 *	@retval false �����B
		 */
		bool IsValid(void) const
		{
			return ! m_Recorder().IsNull();
		}

		/**
		 * [�擾] IDiscRecorder2�C���^�[�t�F�[�X�擾.
		 *	@return IDiscRecorder2�C���^�[�t�F�[�X
		 */
		IDiscRecorder2* operator->(void)
		{
			return m_Recorder();
		}

		/**
		 * [�ݒ�] ���X�i�[�ݒ�.
		 *	@param pListener ���X�i�[
		 */
		void SetListener(IListener* pListener)
		{
			m_member.pListener = pListener;
		}

		/**
		 * [����] ����.
		 *		CD-RW, DVD�}RW, BD-RE �f�B�X�N�����������܂��B
		 *	@note �������������邩�A�G���[����������܂ŁA���\�b�h�̓��^�[�����܂���B
		 *�@	�����̐i���́A IListener �œ����܂��B
		 *	@param isFull false ���w�肷��ƍ��������A true �Ȃ�W���������܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [����] ��������.
		 *		�w��� �f�B�X�N�C���[�W�Ǘ��N���X�̓��e���f�B�X�N�ɏ������݂܂��B
		 *	@note �ǋL�̏ꍇ�A�}���`�Z�b�V�����Ƃ��ď������܂��B
		 *	@note �������݂��������邩�A�G���[����������܂ŁA���\�b�h�̓��^�[�����܂���B
		 *�@	�������݂̐i���́A IListener �œ����܂��B
		 *	@param discImage �f�B�X�N�C���[�W�Ǘ�.
		 *	@param isClose true �Ȃ珑�����݌�A�N���[�Y���܂��B 
		 *					false �Ȃ�N���[�Y���܂���i�ǋL�\�ɂȂ�܂��j�B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [����] �C���[�W�t�@�C����������.
		 *		�w��̃f�B�X�N�C���[�W�t�@�C�����f�B�X�N�ɏ������݂܂��B
		 *	@note �������݂��������邩�A�G���[����������܂ŁA���\�b�h�̓��^�[�����܂���B
		 *�@	�������݂̐i���́A IListener �œ����܂��B
		 *	@param lpszFileName �C���[�W�t�@�C�����B
		 *	@param isClose true �Ȃ珑�����݌�A�N���[�Y���܂��B 
		 *					false �Ȃ�N���[�Y���܂���i�ǋL�\�ɂȂ�܂��j�B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [����] �g���C�I�[�v��.
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool OpenTray(void)
		{
			return m_Tray(true);
		}

		/**
		 * [����] �g���C�N���[�Y.
		 *	@note �h���C�u�ɂ���ẮA�@�\���܂���B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
		 */
		bool CloseTray(void)
		{
			return m_Tray(false);
		}
 
		/**
		 * �h���C�u���.
		 *	@see GetInformation()
		 */
		struct TInformation
		{
			CStr	vendorId;		///< �x���_�[ID
			CStr	productId;		///< �v���_�N�gID
			CStr	productRevision;///< �v���_�N�g���r�W����
		};

		/**
		 * [�擾] �h���C�u�����.
		 *	@return �h���C�u���.
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
		 * [�擾] �}�E���g�p�X�擾.
		 *		���̃h���C�u���}�E���g����Ă���p�X�ꗗ��Ԃ��܂��B
		 *		�ʏ�A�}�E���g�ꏊ�͈�ӏ��ł��i q:\ �Ȃǁj�B
		 *	@param[out] _mountPaths �p�X�ꗗ���i�[����܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�擾] �Z�N�^�T�C�Y�擾.
		 *	@note �P�Z�N�^�̃o�C�g����Ԃ��܂��B
		 *	@return �P�Z�N�^�̃o�C�g��
		 */
		size_t GetSectorSize(void) const
		{
			return 2048;
		}

		/**
		 * [�擾] �T�|�[�g�������ݑ��x�ꗗ.
		 *	@note �f�B�X�N���Ȃ��ƁA���s���܂��B
		 *	@param[out] _speeds �X�s�[�h(sector/second)�̔z�񂪊i�[����܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�ݒ�] ���x�ݒ�.
		 *	@param speed �X�s�[�h(sector/second)�B-1 ���w�肵����f�o�C�X�̍ő����w�肳��܂��B
		 *	@param isPureCAV
		 *		Requested rotational-speed control type. 
		 *		Set to \b true to request constant angular velocity (CAV) rotational-speed control type. 
		 *		Set to \b false to use another rotational-speed control type that the recorder supports. 
		 *		The default is \b false.
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�擾] �f�B�X�N�S�Z�N�^���擾.
		 *	@note �f�B�X�N�̑S�Z�N�^�����擾�ł��܂��B
		 *	@retval 0 ���s�B�G���[���A�f�B�X�N�����}���B
		 *	@retval 1�ȏ� �����B�l�̓Z�N�^���B
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
		 * [�擾] �f�B�X�N�󂫃Z�N�^���擾.
		 *	@note �f�B�X�N�̋󂫃Z�N�^�����擾�ł��܂��B
		 *	@return �󂫃Z�N�^���B
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
		 * [�m�F] �f�B�X�N�m�F.
		 *		�f�B�X�N�̗L�����m�F���܂��B
		 *	@note �f�B�X�N���Ȃ��ꍇ�A�w�莞�ԑ҂����邱�Ƃ��o���܂��i���̊ԁA���\�b�h���烊�^�[�����܂���j�B
		 *	@param time �f�B�X�N�������̑҂�����(ms)
		 *	@retval true �f�B�X�N�L��B
		 *	@retval false �Ȃ��B
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
		 * [�擾] �f�B�X�N��Ԏ擾.
		 *	@return �f�B�X�N��ԁB IMAPI_FORMAT2_DATA_MEDIA_STATE �̒l�� and ���Ƃ邱�ƂŁA
		 *			��Ԃ��킩��܂��B
		 *	@par ��F
		 *	�t�@�C�i���C�Y���Ă��邩�`�F�b�N.
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
		 * [�擾] �f�B�X�N�{�����[�����擾.
		 *	@return �f�B�X�N�{�����[�����擾�B�f�B�X�N���Ȃ��� Empty ��Ԃ��Ԃ�܂��B
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
		 * [�m�F] �u�����N�f�B�X�N�m�F.
		 *	@retval true �u�����N�f�B�X�N�B
		 *	@retval false �f�B�X�N�������A�f�[�^�̂���f�B�X�N�B
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
		 * [�m�F] �ǋL�\�f�B�X�N�m�F.
		 *	@retval true �ǋL�\�ȃf�B�X�N�B
		 *	@retval false �s�\�B
		 */
		bool IsAppendableDisc(void) const
		{
			return (GetStatusOnDisc() & IMAPI_FORMAT2_DATA_MEDIA_STATE_APPENDABLE) != 0;
		}

		/// �f�B�X�N�^�C�v�z��^
		typedef CVectorT<IMAPI_MEDIA_PHYSICAL_TYPE> CTypeVector;

		/**
		 * [�擾] �Ή��f�B�X�N�^�C�v�ꗗ�擾.
		 *	@param[out] _types �Ή��f�B�X�N�^�C�v�̔z�񂪊i�[����܂��B
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		 * [�擾] �f�B�X�N��ގ擾.
		 *		�f�o�C�X�ɓ����Ă���f�B�X�N�̎�ނ��擾�ł��܂��B
		 *	@retval IMAPI_MEDIA_TYPE_UNKNOWN �f�B�X�N�������Ă��܂���B
		 *	@retval ��L�ȊO �f�B�X�N���
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
		 * [�擾] �f�B�X�N��ޖ��擾.
		 *		�f�o�C�X�ɓ����Ă���f�B�X�N�̎�ޖ����擾�ł��܂��B
		 *	@retval empty �f�B�X�N�������Ă��܂���B
		 *	@retval empty�ȊO �f�B�X�N��ޖ�
		 */
		CStr GetTypeStringOnDisc(void) const
		{
			return CImageMastering::ToString(GetTypeOnDisc());
		}

		/**
		 * [�擾] �Z�b�V�������擾.
		 *		�w��̃f�B�X�N�C���[�W�N���X�ɁA�Z�b�V��������ǂݍ��݂܂��B
		 *		�����Ƀ{�����[�������ǂݍ��݂܂��B
		 *	@param[in,out] _diskImage �f�B�X�N�C���[�W.
		 *	@retval true �����B
		 *	@retval false ���s�B�ڍׂ� GetLastResult() / GetLastResultString() ���g���܂��B
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
		/// ��������
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
					// �Z�b�V�����o�^�ς�
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
					// Empty Disc �� OK 
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
		/// �g���C����
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
		/// ������擾���
		enum EBstrKind
		{
			BstrKind_VendorId,				///< VendorId
			BstrKind_ProductId,				///< ProductId
			BstrKind_ProductRevision,		///< ProductRevision
			BstrKind_ExclusiveAccessOwner,	///< ExclusiveAccessOwner
		};
		/// ������擾
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
		// PutRecorder �ς� DiscFormat �쐬
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
		// PutRecorder �ς� DiscFormatData �쐬
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
		// PutRecorder �ς� DiscFormatErase �쐬
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

		#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
		
		/// ���R�[�_�[
		CDiscRecorderHandle& m_Recorder(void) const
		{
			return m_member.hpRecorder;
		}
		/// ���C�^�[
		CDiscFormatDataHandle& m_Writer(void) const
		{
			return m_member.hpWriter;
		}
		/// �v���O���X�f�[�^
		class CMyProgressData : public IListener::CProgressData
		{
		public:
			/// �R���X�g���N�^
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
			LONG m_elapsedTime;			///< �o�ߎ���
			LONG m_expectedTime;		///< �\�z�S����
			LONG m_startLba;			///< �J�nLBA
			LONG m_sectorCount;			///< �����݃Z�N�^�T�C�Y
			LONG m_lastWrittenLba;		///< �����ݒ���LBA
		};
		/// �����o�[�^
		struct TMember : IImageMasteringWriterListener, IImageMasteringEraseListener
		{
			/// �R���X�g���N�^
			TMember(void) : pListener(NULL)
			{}
			//�������݃C�x���g
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
			//�����C�x���g
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
			IListener*				pListener;	///< ���X�i�[
			CDiscRecorderHandle		hpRecorder;	///< ���R�[�_�[
			CDiscFormatDataHandle	hpWriter;	///< ���C�^�[
		};
		mutable TMember			m_member;		///< �����o�[

		#endif //_TnbDOXYGEN
	};


	//------------------------------------------------


	/// �R���X�g���N�^
	CImageMastering(void)
	{
	}

	/**
	 * [�m�F] �T�|�[�g�m�F.
	 *		PC���{�@�\���T�|�[�g���Ă��邩�A�m�F�o���܂��B
	 *	@retval true �T�|�[�g���Ă���.
	 *	@retval false �T�|�[�g���Ă��Ȃ��B
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
	 * [�擾] �h���C�u���擾.
	 *	@note �������݂��T�|�[�g���Ă��Ȃ��h���C�u�����Ɋ܂܂�܂��B
	 *	@return �h���C�u��.
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
	 * [�擾] �h���C�u���擾.
	 *	@note "q:\\"�Ȃǂł͂���܂���B�h���C�u���������j�[�N�ȕ�����ł��B
	 *	@param index �C���f�b�N�X�B�O�` GetDriveCount() �������w��o���܂��B
	 *	@return �h���C�u��.
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
	 * [�擾] �h���C�u�Ǘ��N���X�擾.
	 *		�w��̃h���C�u�Ǘ��N���X���擾���܂��B�������݂��T�|�[�g���Ă��Ȃ��h���C�u�̏ꍇ�A
	 *		�����ȃN���X���擾����܂��B
	 *	@param index �C���f�b�N�X�B�O�` GetDriveCount() �������w��o���܂��B
	 *	@return �f�o�C�X�Ǘ��N���X. CDrive::IsValid() �ŁA�L���ȃN���X���m�F�ł��܂��B
	 */
	CDrive CreateDrive(INDEX index) const
	{
		IDiscRecorder2* P = NULL;
		CheckRes(::CoCreateInstance(CLSID_MsftDiscRecorder2, NULL, CLSCTX_ALL, IID_PPV_ARGS(&P)));
		return CDrive(P, GetDriveName(index));
	}

	/**
	 * [�擾] �f�B�X�N��ޕ����擾.
	 *	@param type �f�B�X�N���
	 *	@return �f�B�X�N��ޕ���
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
	// �C���X�^���X�`�F�b�N.
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



#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��

/**@ingroup IMAPI2
 * �������݃A�N�V����.
 *	@see TNB::CImageMastering::CDrive::IListener::OnDriveWriteEvent()
 */
enum IMAPI_FORMAT2_DATA_WRITE_ACTION
{
	IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA,		///< �f�B�X�N�m�F��
	IMAPI_FORMAT2_DATA_WRITE_ACTION_FORMATTING_MEDIA,		///< �f�B�X�N�t�H�[�}�b�g��
	IMAPI_FORMAT2_DATA_WRITE_ACTION_INITIALIZING_HARDWARE,	///< ��������
	IMAPI_FORMAT2_DATA_WRITE_ACTION_CALIBRATING_POWER,		///< �p���[������
	IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA,			///< �f�[�^�������ݒ�
	IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION,			///< �t�@�C�i���C�Y��
	IMAPI_FORMAT2_DATA_WRITE_ACTION_COMPLETED,				///< ����
};

/**@ingroup IMAPI2
 * �f�B�X�N���.
 *	@see TNB::CImageMastering::CDrive::GetStatusOnDisc()
 */
enum IMAPI_FORMAT2_DATA_MEDIA_STATE
{
	IMAPI_FORMAT2_DATA_MEDIA_STATE_OVERWRITE_ONLY	= 0x1,			///< �㏑���̂�
	IMAPI_FORMAT2_DATA_MEDIA_STATE_BLANK	= 0x2,					///< �u�����N
	IMAPI_FORMAT2_DATA_MEDIA_STATE_APPENDABLE	= 0x4,				///< �ǋL�\
	IMAPI_FORMAT2_DATA_MEDIA_STATE_FINAL_SESSION	= 0x8,			///< �Ō�̃Z�N�V����
	IMAPI_FORMAT2_DATA_MEDIA_STATE_DAMAGED	= 0x400,				///< �_���[�W
	IMAPI_FORMAT2_DATA_MEDIA_STATE_ERASE_REQUIRED	= 0x800,		///< �����K�v����
	IMAPI_FORMAT2_DATA_MEDIA_STATE_NON_EMPTY_SESSION	= 0x1000,	///< �Z�N�V�����L��
	IMAPI_FORMAT2_DATA_MEDIA_STATE_WRITE_PROTECTED	= 0x2000,		///< �������݋֎~
	IMAPI_FORMAT2_DATA_MEDIA_STATE_FINALIZED	= 0x4000,			///< �t�@�C�i���C�Y�ς�
	IMAPI_FORMAT2_DATA_MEDIA_STATE_UNSUPPORTED_MEDIA	= 0x8000	///< ���T�|�[�g�f�B�X�N
};

/**@ingroup IMAPI2
 * �f�B�X�N�^�C�v.
 *	@see TNB::CImageMastering::CDrive::GetTypeOnDisc()
 */
enum IMAPI_MEDIA_PHYSICAL_TYPE
{
	IMAPI_MEDIA_TYPE_UNKNOWN,				///< �s��,
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

	VC++6 �AMFC �ō쐬�����_�C�A���O��ŁA�������݂��Ă��鎞�A
	�t�H�[�J�X�ړ��ŁA�Ȃ��� �A�N�Z�X�o�C�I���[�V���������I

#endif

