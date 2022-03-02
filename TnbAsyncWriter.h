#pragma once
/**
 *@file
 * �񓯊������ݏ����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbWriter.h"
#include "TnbAbstractReport.h"
#include "TnbCollectionAdapter.h"
#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE THREAD
 * �񓯊��������݃N���X
 *
 *		�����ݏ������̂��X���b�h�ōs���܂��̂ŁA
 *		�֐��R�[���͑������^�[�����܂��B
 *
 *	@note �{�N���X�͓Ǎ��݋@�\�͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbAsyncWriter.h
 *
 *	@date 06/05/02 �V�K�쐬
 *	@date 06/05/30 �N���X���ύX�B�X���b�h�ȊO�œ������[�h���ǉ��B ICommunication �����B
 *	@date 06/07/07 InterfaceID�T�O�A�ǉ�
 *	@date 06/08/23 CFileWriterEx ��������B
 *	@date 06/08/31 CommViewer �ɑΉ��B
 *	@date 06/09/09 IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *					ECE_Connect , ECE_Disconnect �C�x���g�A�ǉ��B
 *	@date 06/10/16 IWriter ���g�p����悤�ɕύX
 *	@date 07/02/08 �N���X���A�ύX�B
 *	@date 07/02/09 �e�N���X�ύX�B
 *	@date 07/06/11 �e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 09/08/27 SetPriority() �̃R�����g�␳�B�\�������B
 *	@date 10/07/30 IReport �����̔p�~
 */
class CAsyncWriter : public CThreadDispatch::IListener
{
public:

	/**@ingroup FILE THREAD
	 * �񓯊��������݂̃R�}���h�C���^�[�t�F�[�X
	 *
	 *		�ʏ�������邱�Ƃ͗L��܂��񂪁A����ȃt�@�C��������s���������Ɏ������A
	 *		Command() �ɓn�����Ƃ��o���܂��B
	 *
	 *	@see CAsyncWriter::Command
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbAsyncWriter.h
	 *
	 *	@date 06/05/02 �V�K�쐬
	 */
	struct ICommand
	{
		/// �f�X�g���N�^
		virtual~ ICommand(void) {}
		/** 
		 * [����] ����
		 *	@param fw �I�[�v�����Ă���Writer���n����܂��B
		 *	@return false�Ȃ�X���b�h�I�������܂��B
		 */
		virtual bool Exec(IWriter& fw) = 0;
	};


	//-------------------------


	/// �R���X�g���N�^ 
	explicit CAsyncWriter(void)
	{
		m_pWriter = NULL;
		m_threadDispatch.SetListener(this);
	}

	/// �f�X�g���N�^
	virtual ~CAsyncWriter(void)
	{
		Stop();
	}

	/**
	 * [�m�F] �J�n�`�F�b�N.
	 *		Start() ���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	bool IsAlive(void) const 
	{
		return (m_pWriter != NULL) ? m_pWriter->CanWrite() : false;
	}
	
	/**
	 * [�ݒ�] �D��x�ݒ�.
	 *		�����݃X���b�h�̗D��x���w��o���܂��B
	 *	@param priority �D��x�i-2�`2, 15�B0���m�[�}���B�傫���ق����D��x��j
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetPriority(int priority)
	{
		return m_threadDispatch.SetPriority(priority);
	}

	/**
	 * [�ݒ�] ���C�^�[�ݒ�
	 *	@param pfw �I�[�v�����Ă���Writer���w�肵�܂��B
	 *				Open���Ă��Ȃ��I�u�W�F�N�g��n���Ǝ��s���܂��B
	 *				pfw �͎Q�Ƃ��܂��̂ŁA�g�p���͔j�����Ȃ��ŉ������B
	 *	@retval true �ݒ萬��
	 *	@retval false ���s
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
	 * [�ݒ�] �J�n.
	 *		�{���\�b�h���s��A Send() Write() SetEnd() Seek() ���g�p�ł���悤�ɂȂ�܂��B
	 *	@note	SetWriter() �Ń��C�^�[�̐ݒ��Ɏg�p���܂��B
	 *	@retval true �v������
	 *	@retval false ���s
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
		return m_threadDispatch.Start(_T("AsyncWriter:����"));
	}

	/**
	 * [�ݒ�] �I��.
	 *	@note �I�[�v�����Ă���Writer��Close���܂��B
	 *		�N���[�Y��������܂ŁA���^�[�����܂���B
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
	 * [���s] �t�@�C����������
	 *	@attention 2GByte�ȏ�͈�x�ɏ������߂܂���B
	 *	@note �����ň�x ByteVectoer �ɕϊ����Ă��܂��B
	 *	@param c �������݃f�[�^�B
	 *	@retval true �������ݗv������
	 *	@retval	false	���s�B Start() �����s���Ă��Ȃ��ꍇ�����s���܂��B
	 */
	bool Write(const IConstCollectionT<BYTE>& c)
	{
		return m_OnDispatch(new CWriteCmd(c));
	}

	/**
	 * [���s] �V�[�N
	 *	@param iOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ IWriter::TOP �ɂȂ�܂��B
	 *	@retval true �V�[�N�v������
	 *	@retval	false	���s�B Start() �����s���Ă��Ȃ��ꍇ�����s���܂��B
	 */
	bool Seek(int iOffset, IWriter::ESeekMode eSeek = IWriter::TOP)
	{
		return m_OnDispatch(new CSeekCmd(iOffset, eSeek));
	}

	/**
	 * [���s] EOF���w��.
	 *		���݂̃t�@�C���|�C���^�̈ʒu��EOF�ɂ��܂��B
	 *	@see https://msdn.microsoft.com/ja-jp/library/cc429774.aspx
	 *	@retval true EOF�������ݗv������
	 *	@retval	false	���s�B Start() �����s���Ă��Ȃ��ꍇ�����s���܂��B
	 */
	bool SetEnd(void)
	{
		return m_OnDispatch(new CSetEndCmd);
	}

	/**
	 * [���s] �N���[�Y.
	 *		�I�[�v�����Ă���Writer��Close���܂��B
	 *	@retval true �v������
	 *	@retval	false	���s�B Start() �����s���Ă��Ȃ��ꍇ�����s���܂��B
	 */
	bool Close(void)
	{
		return m_OnDispatch(new CCloseCmd);
	}

	/**
	 * [���s] �R�}���h
	 *	@param piCmd new �ō쐬���� ICommand �C���^�[�t�F�[�X�����������N���X�B
	 *	@retval true �v������
	 *	@retval	false	���s�B Start() �����s���Ă��Ȃ��ꍇ�����s���܂��B
	 */
	bool Command(ICommand* piCmd)
	{
		return m_OnDispatch(piCmd);
	}

private:

	CThreadDispatch	m_threadDispatch;	///< Dispatch�N���X
	IWriter*		m_pWriter;			///< �t�@�C���N���X

	enum{ WMWF_COMMAND = WM_APP };		///< �R�}���h�˗��C�x���g�R�[�h�B

	/**
	 * [�ʒm] ���b�Z�[�W��M
	 *	@param nMsg ���b�Z�[�W
	 *	@param wParam IWriter �̃|�C���^
	 *	@param lParam ICommand �̃|�C���^�B
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
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
	 * ���b�Z�[�W�U�蕪��.
	 *	@param P ICommand �̃|�C���^�B
	 *	@retval true �������ݗv������
	 *	@retval false ���s
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

	///�������݃R�}���h
	class CWriteCmd : public ICommand
	{
		CByteVector m_vb;		///< �������݃f�[�^
	public:	
		CWriteCmd(const IConstCollectionT<BYTE>& c) { m_vb.Copy(c); }
		bool Exec(IWriter& fw) { fw.WriteFrom(m_vb); return true; }
	};

	///�V�[�N�R�}���h
	class CSeekCmd : public ICommand
	{
		int					m_iOffset;	///< SEEK�ړ���
		IWriter::ESeekMode	m_eSeek;	///< SEEK�J�n�ʒu
	public:	
		CSeekCmd(int iOffset, IWriter::ESeekMode eSeek) { m_iOffset = iOffset; m_eSeek = eSeek; }
		bool Exec(IWriter& fw) { fw.Seek(m_iOffset, m_eSeek); return true; }
	};

	/// EOF�R�}���h
	class CSetEndCmd : public ICommand
	{
	public:	bool Exec(IWriter& fw) { fw.SetEnd(); return true; }
	};

	/// �N���[�Y�R�}���h
	class CCloseCmd : public ICommand
	{
	public:	bool Exec(IWriter& fw) { fw.Close(); return false; }
	};
};



}; // TNB
