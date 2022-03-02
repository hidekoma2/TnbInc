#pragma once
/**
 *@file
 * �ʐM�񍐊֌W�̃w�b�_
 *
 *		��M�����́A���X�i�[��o�^���邱�Ƃōs���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * �ʐM��M�C�x���g��.
 *		IReport �̃T�u�N���X���ʒm����C�x���g��ł��B
 */
enum EReportEvent
{
	ReportEvent_Non,			///< �C�x���g�Ȃ�.	@note �ʒm�������̂ł͂���܂���B
	ReportEvent_Idle,			///< �A�C�h��.		@note �g�p�ł��Ȃ��T�u�N���X������܂��B
	ReportEvent_End,			///< �I��.			@note Alive��Ԃ��I���܂����B
	ReportEvent_Error,			///< �G���[.		@note �G���[�R�[�h���ɒʒm����܂��B
	ReportEvent_Connect,		///< �ڑ�.			@note �ؒf�ƃy�A�Ɏg�p����܂��B
	ReportEvent_Disconnect,		///< �ؒf.			@note �ڑ��ƃy�A�Ŏg�p����܂��B
	ReportEvent_IrregularEnd,	///< �s���I��.		@note Stop�ł͂Ȃ��A�����I�ȏI�����ɒʒm����܂��B

	ECE_Non = ReportEvent_Non,				///< �C�x���g�Ȃ�(���V���{��)
	ECE_Idle = ReportEvent_Idle,			///< �A�C�h��(���V���{��)
	ECE_End = ReportEvent_End,				///< �I��(���V���{��)
	ECE_Error = ReportEvent_Error,			///< �G���[(���V���{��)
	ECE_Connect = ReportEvent_Connect,		///< �ڑ�(���V���{��)
	ECE_Disconnect = ReportEvent_Disconnect,///< �ؒf(���V���{��)
};

/**@ingroup REPORT
 * �ʐM��M�C�x���g��.
 *		@note �����ł��B EReportEvent ���g�p���Ă��������B
 */
typedef EReportEvent EReportEventId;



/**@ingroup REPORT
 * �ʐM��M�C�x���g�Ǘ��N���X
 *
 *		���X�i�[�̃��\�b�h OnReportEvent() �Œʒm�������ł��B
 *		�f�[�^�i�����ƃA�h���X�j���C�x���g�̂����ꂩ��ێ����܂��B
 *
 *	@see IReport::IListener::OnReportEvent()
 *
 *	@par�K�v�t�@�C��
 *			TnbReport.h
 * 
 *	@date 06/05/11 �V�K�쐬
 *	@date 06/07/07 InterfaceID�T�O�A�ǉ�
 *	@date 11/03/10 ReportEvent_IrregularEnd ��ǉ�
 *	@date 11/06/14 GetInterfaceId() ���\�b�h�̖߂�l�ύX�B
 */
class CReportEvent
{
	const BYTE*		m_lpcbData;			///< ��M�f�[�^�A�h���X
	size_t			m_dataSize;			///< ��M�f�[�^�T�C�Y
	EReportEvent	m_eEvent;			///< ��M�C�x���g�T��
	DWORD			m_dwErrorCode;		///< �G���[�R�[�h
	friend struct IReport;				///< �p�[�g�i�[�N���X�錾

protected:

	mutable IReport*	m_piReport;	///< Report Object�̎Q��

public:

	/**
	 * �R���X�g���N�^
	 *	@param piComm IReport �̃T�u�N���X�̃|�C���^
	 *	@param eEvent �C�x���g�R�[�h�i�ȗ��\�j
	 *	@param dwError �G���[�R�[�h�i�ȗ��\�j
	 */
	explicit 
	CReportEvent(IReport* piComm, EReportEvent eEvent = ReportEvent_Non, DWORD dwError = 0)
	{
		m_lpcbData			= NULL;
		m_dataSize			= 0;
		m_eEvent			= eEvent;
		m_dwErrorCode		= dwError;
		m_piReport	= piComm;
	}

	/**
	 * [�擾] �C���^�[�t�F�[�XID�擾
	 *		@note ��{�I�� IReport �̃T�u�N���X�̃R���X�g���N�^�Ŏw�肵���l��Ԃ��܂��B
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const;	//�{�̂̓t�@�C�����ʂɋL�q

	/**
	 * [���M] �f�[�^���M
	 *	@attention �{�N���X�ł͎�M���ɑ��M�����͏o���܂���B
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@return ���false�B
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData) const;	//�{�̂̓t�@�C�����ʂɋL�q

	/**
	 * [�m�F] Data�������Ă��邩
	 *	@retval true	Data�������Ă���
	 *	@retval false	�����Ă��Ȃ�
	 */
	bool HasData(void) const
	{
		return m_lpcbData != NULL;
	}

	/**
	 * [�m�F] Event(Error)�������Ă���
	 *	@retval true	Event(Error)�������Ă���
	 *	@retval false	�����Ă��Ȃ�
	 */
	bool HasEvent(void) const
	{
		return ! HasData();
	}

	/**
	 * [�擾] �f�[�^���擾
	 *	@return �ێ����Ă���f�[�^��
	 */
	size_t GetDataSize(void) const
	{
		return HasData() ? m_dataSize : 0;
	}

	/**
	 * [�擾] �f�[�^�A�h���X�擾
	 *	@return �ێ����Ă���f�[�^�A�h���X
	 */
	const BYTE* GetData(void) const
	{
		return m_lpcbData;
	}

	/**
	 * [�擾] �C�x���g�R�[�h�擾
	 *	@return �ێ����Ă���C�x���g�R�[�h
	 */
	EReportEvent GetEvent(void) const
	{
		return HasEvent() ? m_eEvent : ReportEvent_Non;
	}

	/**
	 * [�擾] �G���[�R�[�h�擾
	 *	@return �ێ����Ă���G���[�R�[�h
	 */
	DWORD GetErrorCode(void) const
	{
		if ( HasEvent() && GetEvent() == ReportEvent_Error )
		{
			return m_dwErrorCode;
		}
		return 0;
	}

	/**
	 * [�擾] �C�x���g�R�[�h������擾
	 *	@return �ێ����Ă���C�x���g�R�[�h�𕶎��񉻂�������
	 */
	CStr GetEventString(void) const
	{
		switch ( GetEvent() )
		{
		case ReportEvent_Non:			return _T("�Ȃ�");
		case ReportEvent_Idle:			return _T("�A�C�h��");
		case ReportEvent_End:			return _T("�I��");
		case ReportEvent_IrregularEnd:	return _T("�����I��");
		case ReportEvent_Connect:		return _T("�ڑ�");
		case ReportEvent_Disconnect:	return _T("�ؒf");
		case ReportEvent_Error:			return CStr::Fmt(_T("�G���[[%u]"), GetErrorCode());
		}
		return _T("");
	}

	/**
	 * [�ݒ�] �f�[�^�ݒ�.
	 *	@note �|�C���^�͎Q�Ƃ݂̂ŁA���͖̂{�C���X�^���X�ŕێ����܂���B
	 *	@param dataSize �f�[�^��
	 *	@param lpcData �f�[�^�A�h���X
	 */
	void SetData(size_t dataSize, LPCVOID lpcData)
	{
		m_dataSize = dataSize;
		m_lpcbData = static_cast<const BYTE*>(lpcData);
	}
};



/**@ingroup REPORT
 * �ʐM�A�N�Z�X�C���^�[�t�F�[�X
 *
 *		��M�́AListener��o�^���s���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbReport.h
 * 
 *	@date 06/05/11 �V�K�쐬
 *	@date 06/07/07 InterfaceID�T�O�A�ǉ�
 *	@date 06/08/31 CommViewer �ɑΉ��B
 *	@date 06/09/08 Idle�C�x���g�T�|�[�g�B Purge() / IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *	@date 07/02/09 ���ۉ��B
 *	@date 07/06/11 Start() ���\�b�h��ǉ��B
 *	@date 09/04/08 �����o�ϐ������ׂĔp�~�B���\�b�h��ǉ��B
 *	@date 11/06/14 GetInterfaceId() GetReportID() ���\�b�h�̖߂�l�ύX�B
 */
struct IReport
{
	friend class CReportEvent;		///< �p�[�g�i�[�N���X�錾

	/**@ingroup REPORT
	 * �ʐM�̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		IReport �̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *	@see IReport::SetCommListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbReport.h
	 * 
	 *	@date 06/05/11	�V�K�쐬
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �C�x���g����.
		 *		 �f�[�^����M����A�C�x���g�������ɃR�[������܂��B
		 *		ev ��{@link CReportEvent::HasData() HasData()}�A
		 *		{@link CReportEvent::HasEvent() HasEvent()}�ɂ��A
		 *		�ǂ���̃^�C�~���O�ŃR�[�����ꂽ���킩��܂��B
		 *	@par���C�x���g������
		 *		ev ��{@link CReportEvent::GetEvent() GetEvent()} �ɂ��A
		 *		�ǂ̃C�x���g�������������m�邱�Ƃ��o���܂��B<BR>
		 *		ReportEvent_Idle :		Alive��ԂŃf�[�^��M���Ȃ��Ƃ��w��̃^�C�~���O�Œʒm����܂��B
		 *								�������g�p�ł��Ȃ��T�u�N���X������܂��B<BR>
		 *		ReportEvent_Error :		�G���[�������ɒʒm����܂��Bev ��
		 *								{@link CReportEvent::GetErrorCode() GetErrorCode()}
		 *								�ŃG���[�R�[�h��m�邱�Ƃ��o���܂��B<BR>
		 *		ReportEvent_Connect :	�ʐM�\�ɂȂ������ɒʒm����܂��B
		 *								�߂�l�� false ��Ԃ����ꍇ�A ReportEvent_Disconnect �͒ʒm����܂��񂪁A
		 *								�ʐM�s�\��ԂɂȂ�܂��i ReportEvent_End ���ʒm�����T�u�N���X������܂��j�B<BR>
		 *		ReportEvent_Disconnect :�ʐM�s�\�ɂȂ������ɒʒm����܂��B�߂�l�͈Ӗ��������܂���B
		 *								�܂��Aev ��{@link CReportEvent::Send() Send()}�͎g�p�ł��܂���(���ɒʐM�s�\�̂���)�B<BR>
		 *		ReportEvent_End :		Alive��Ԃ��I���鎞�ɒʒm����܂��B�߂�l�͈Ӗ��������܂���B
		 *								�܂��Aev ��{@link CReportEvent::Send() Send()}�͎g�p�ł��܂���(���ɒʐM�s�\�̂���)�B<BR>
		 *		ReportEvent_IrregularEnd : �����I�ɐؒf���ꂽ��(�f�o�C�X�������ꂽ���Ȃ�)�ɁAReportEvent_End ��ʒm����O�ɁA
		 *								�ʒm����܂��i����̌�� �K�� ReportEvent_End ���ʒm����܂��j�B
		 *								�߂�l�͈Ӗ��������܂���B�܂��Aev ��{@link CReportEvent::Send() Send()}�͎g�p�ł��܂���(���ɒʐM�s�\�̂���)�B<BR>
		 *	@par���f�[�^��M��
		 *		ev ��{@link CReportEvent::GetDataSize() GetDataSize()} �ɂ��A��M����
		 *		�f�[�^�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
		 *		ev ��{@link CReportEvent::GetData() GetData()} �ɂ��A��M����
		 *		�f�[�^�̃A�h���X�𓾂邱�Ƃ��o���܂��B
		 *
		 *	@param ev ��M�������e�i�f�[�^���C�x���g�j�B
		 *	@retval true	����
		 *	@retval false	�ؒf�v���B ReportEvent_End , ReportEvent_IrregularEnd �C�x���g�̏ꍇ�A�ؒf�v���͖�������܂��B
		 */
		virtual bool OnReportEvent(const CReportEvent& ev) = 0;
	};


	//------------------------------


	/// �f�X�g���N�^
	virtual ~IReport(void) 
	{
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const = 0; 

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const = 0; 

	/**
	 * [�o�^] ����Listner�o�^.
	 *	@note �o�^�ł��� IReport::IListener �͈�����ł��B
	 *	@param piCommListener ���X�i�[�B�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL) = 0;

	/**
	 * [�擾] �C���^�[�t�F�[�XID�擾
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const = 0;

	/** 
	 * [�ݒ�] �A�C�h���C�x���g�ݒ�
	 *	@note	��M���Ȃ����A�w��̊Ԋu�ŃR�[���o�b�N�𔭐������邱�Ƃ��ł��܂��B
	 *			�J�n��A���邢�͎�M������A�w��ms�b��ɃR�[���o�b�N���n�܂�܂��B
	 *			Open���ɗL���̐؂�ւ��͉\�ł��B
	 *	@param iInterval Idle�C�x���g��������Ԋu���w�肵�܂��B�O�Ȃ�AIdle�C�x���g�Ȃ��B
	 */
	virtual void SetIdleEventMode(int iInterval = 0) = 0;

	/**
	 * [����] �J�n.
	 *	@note �ݒ胁�\�b�h�Őݒ���s���Ă���{���\�b�h���g�p���A�J�n���܂��B
	 *	@retval true �����BAlive��ԂɂȂ�܂����B
	 *	@retval false ���s�BAlive��Ԃɂ͂Ȃ�܂���ł����B
	 */
	virtual bool Start(void) = 0;

	/**
	 * [����] ��~.
	 *		Alive��Ԃ��I�����܂��B
	 */
	virtual void Stop(void) = 0;

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		�e�T�u�N���X�̊J�n���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const = 0;
	
	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@retval true	�ڑ����B
	 *	@retval	false	�ؒf�B
	 */
	virtual bool IsConnect(void) const = 0;

	/**
	 * [���M] �f�[�^���M
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcData ���M����f�[�^�B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcData) = 0;

	/**
	 * [�ݒ�] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void) = 0;
};



/// ���M
inline bool CReportEvent::Send(size_t dataSize, LPCVOID lpcvData) const
{
	return m_piReport->Send(dataSize, lpcvData);
}

/// �C���^�[�t�F�[�XID�擾
inline DWORD CReportEvent::GetInterfaceId(void) const
{
	return m_piReport->GetInterfaceId();
}



}; // TNB
