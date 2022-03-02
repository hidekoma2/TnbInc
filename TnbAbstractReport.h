#pragma once
/**
 *@file
 * �ʐM�֌W�̃w�b�_
 *
 *		��M�����́A���X�i�[��o�^���邱�Ƃōs���܂��B
 *
 *	@note	�}�N�� "_TnbREPORT_Viewer_ON" ����`����Ă��邩�A�f�o�O�r���h�Ȃ�A Viewer �ɂ��ʐM���O���Ƃ邱�Ƃ��o���܂��B
 *	@note	�f�o�O�r���h�̎��A�}�N�� "_TnbREPORT_Dump_OFF" ����`����Ă���ꍇ�A�A�E�g�v�b�g�ɒʐM���O��\�����܂���B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReport.h"
#include "TnbTickCount.h"
#include "TnbDump.h"
#include "TnbReportViewer.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * �ʐM�A�N�Z�X���ۃN���X
 *
 *		��M�́AListener��o�^���s���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbAbstractReport.h
 * 
 *	@date 07/02/09 ���ۉ����č쐬
 *	@date 07/03/15 index, size�Ǘ��� unsigned�ɕύX�B
 *	@date 09/04/08 IReport ���烁���o�ڍs�B
 *	@date 10/08/05 ITester �p�~�B
 *	@date 11/06/14 GetInterfaceId() ���\�b�h�̖߂�^�ύX�B
 *	@date 12/03/13 DisableReportViewer() ��ǉ��B
 */
class CAbstractReport : public IReport, public CSyncSection
{
	DEFSUPER(IReport);
public:

	/** 
	 * �R���X�g���N�^ 
	 *	@param dwInterfaceId �C���^�[�t�F�[�XID�B�ȗ�����ƂO�B
	 *				(����́A�����̒ʐM�N���X�ŋ��ʂ̃��X�i�[���g�p���鎞�A���ʂƂ��Ďg�p���܂�)
	 */
	CAbstractReport(DWORD dwInterfaceId = 0) 
		: m_dwInterfaceId(dwInterfaceId), m_iIdleEventInterval(0), m_piListner(NULL)
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CAbstractReport(void) 
	{
	}

	/**
	 * [�o�^] ���� Listner �o�^.
	 *	@note �o�^�ł��� IReport::IListener �͈�����ł��B
	 *	@param piCommListener ���X�i�[�B�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL)
	{
		m_piListner = piCommListener;
	}

	/**
	 * [�擾] �C���^�[�t�F�[�XID�擾
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const
	{
		return m_dwInterfaceId;
	}

	/** 
	 * [�ݒ�] �A�C�h���C�x���g�ݒ�
	 *	@note	��M���Ȃ����A�w��̊Ԋu�ŃR�[���o�b�N�𔭐������邱�Ƃ��ł��܂��B
	 *			�J�n��A���邢�͎�M������A�w��ms�b��ɃR�[���o�b�N���n�܂�܂��B
	 *			Open���ɗL���̐؂�ւ��͉\�ł��B
	 *	@param iInterval Idle�C�x���g��������Ԋu���w�肵�܂��B�O�Ȃ�AIdle�C�x���g�Ȃ��B
	 */
	virtual void SetIdleEventMode(int iInterval = 0)
	{
		if ( iInterval > 0 )
		{
			m_iIdleEventInterval = iInterval;
		}
		else
		{
			m_iIdleEventInterval = 0;
		}
	}

	/** 
	 * [�擾] �A�C�h���C�x���g�Ԋu�擾
	 *	@retval 0		�A�C�h���C�x���g��������
	 *	@retval 1�ȏ�	�Ԋu(ms)
	 */
	int GetIdleInterval(void) const
	{
		return m_iIdleEventInterval;
	}

	/**
	 * [�ݒ�] �p�[�W.
	 *		�ʐM�̃S�~����
	 *	@note �{�N���X�ł͉������܂���B
	 */
	virtual void Purge(void) 
	{
	}

	/**
	 * [�m�F] �ڑ��҂�
	 *	@param dwTime �҂�����(ms)
	 *	@retval true �ڑ�
	 *	@retval false �^�C���A�E�g
	 */
	bool WaitConnect(DWORD dwTime) const
	{
		if ( ! IsAlive() )
		{
			return false;
		}
		CTickCount tick;
		while ( ! tick.IsPassed(dwTime) )
		{
			if ( IsConnect() )
			{
				return true;
			}
			::Sleep(10);
		}
		return IsConnect();
	}

	/**
	 * [�ݒ�] ���|�[�g�r�����[��~.
	 *		�{���\�b�h���g�p����ƁA�ʐM���O�̏o�͂��s���܂���B
	 *		�f�t�H���g�́A�o�͂��s���܂��i�������r���h�̐ݒ�ɂ����܂��j�B
	 */
	void DisableReportViewer(void)
	{
		m_viewer.Invalid();
	}

protected:

	/**
	 * [����] ���M���ʒm
	 *	@note �����͊J���pTRACE�̂�
	 *	@param sizeData ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 */
	void m_OnSend(size_t sizeData, LPCVOID lpcvData)
	{
		TRACE1("%s:Send\n", GetReportName());
		#ifndef _TnbREPORT_Dump_OFF
			DebugDump(sizeData, lpcvData);
		#endif
		m_ToViewer(sizeData, lpcvData, TNBVIW_DIRE_SEND | GetReportID());
	}
	
	/**
	 * [����] �R�[���o�b�N�ʒm���s
	 *	@param ev �ʒm������e
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
	 */
	bool m_OnEvent(const CReportEvent& ev)
	{
		if ( ev.HasData() )
		{
			//�J���pTRACE
			TRACE1("%s:Receive\n", GetReportName());
			#ifndef _TnbREPORT_Dump_OFF
				DebugDump(ev.GetDataSize(), ev.GetData());
			#endif
		}
		if ( ev.HasData() )
		{
			m_ToViewer(ev.GetDataSize(), ev.GetData(), TNBVIW_DIRE_RECV | GetReportID());
		}
		else if ( ev.GetEvent() != ReportEvent_Idle )
		{
			m_ToViewer(ev.GetEventString(), GetReportID());
		}
		bool boRc = true;
		if ( m_piListner != NULL )
		{
			EXCLUSIVE(this);
			boRc = m_piListner->OnReportEvent(ev);
		}
		return boRc;
	}

	/**
	 * [�o��] �_���v�o��.
	 *	@param sizeData �f�[�^�T�C�Y.
	 *	@param lpcvData �f�[�^
	 */
	void ToViewer(size_t sizeData, LPCVOID lpcvData)
	{
		m_ToViewer(sizeData, lpcvData, TNBVIW_TYPE_RAWTEXT | GetReportID());
	}

	/**
	 * [�o��] ������o��.
	 *	@param lpszText ������.
	 */
	void ToViewer(LPCTSTR lpszText)
	{
		m_ToViewer(lpszText, TNBVIW_TYPE_RAWTEXT | GetReportID());
	}

private:
	// [�o��] �_���v�o��.
	void m_ToViewer(size_t sizeData, LPCVOID lpcvData, DWORD cookie)
	{
		m_viewer.ToViewer(sizeData, lpcvData, cookie);
	}
	// [�o��] ������o��.
	void m_ToViewer(LPCTSTR lpszText, DWORD cookie)
	{
		m_viewer.ToViewer(lpszText, cookie);
	}
	CReportViewer	m_viewer;				///< T-Report Viewer
	DWORD			m_dwInterfaceId;		///< �C���^�[�t�F�[�XID
	IListener*		m_piListner;			///< ���X�i�[
	int				m_iIdleEventInterval;	///< Idle�ʒm�̊Ԋu(ms)
};



}; // TNB
