#pragma once
/**
 *@file
 * �X���b�h�����֌W�̃w�b�_
 *
 *		���b�Z�[�W�ʒm�ŁA�ʃX���b�h���ŏ����������邱�Ƃ��ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup THREAD
 * �X���b�h��������N���X
 *
 *		���b�Z�[�W�ʒm�ŁA�ʃX���b�h���ŏ����������邱�Ƃ��ł��܂��B
 *
 *	@note	�����́A���X�i�[�����������N���X��o�^���邩�A�{�N���X���p����
 *			OnThreadDispatchReceive() ���I�[�o�[���C�h���܂��B�i�����͑O�ҁj
 *	
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbThreadDispatch.h
 *
 *	@date 06/04/01	�V�K�쐬
 *	@date 06/08/03	ThreadID�擾���\�b�h�ǉ��B�{�N���X���p�����ď�����������������@���ǉ��B
 *	@date 06/10/25	OnSetup() ���\�b�h�ǉ��B
 *	@date 09/08/27	�\���ύX�B
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 *	@date 11/01/06	DispatchMessage() ���s���悤�ɂ����B
 *	@date 11/02/03	IListener::OnThreadDispatchSetup() ���Ăԃ^�C�~���O��ύX�B
 *	@date 12/05/31	IsDispatching() ��ǉ��B
 */
class CThreadDispatch : TNB::CThread::IRunner, CCopyImpossible
{
	typedef TNB::CThread::IRunner __runner;	///< IF���Đ錾
public:

	/**@ingroup THREAD
	 * �X���b�h��������̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		CThreadDispatch�̃��X�i�[�ł��B���b�Z�[�W����M����ƒʒm����܂��B
	 *
	 *	@see CThreadDispatch::SetListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbThreadDispatch.h
	 *
	 *	@date 06/04/01	�V�K�쐬
	 *	@date 06/10/25	OnSetup() ���\�b�h�ǉ��B
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] ���b�Z�[�W��M.
		 *		�Ō�͕K�� nMsg �� WM_QUIT �������Ēʒm����܂��B
		 *	@param nMsg ���b�Z�[�W
		 *	@param wParam WPARAM
		 *	@param lParam LPARAM
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam) = 0;

		/**
		 * [�ʒm] �J�n�ʒm.
		 *		��M�X���b�h���J�n�����ƁA�ʒm����܂��B
		 *	@note CThreadDispatch::Start() ����Ԃ�O�ɍ쐬���ꂽ�X���b�h�ォ��Ă΂�܂��B
		 */
		virtual void OnThreadDispatchSetup(void) {}
	};


	//-------------------------


	/** �R���X�g���N�^ */
	CThreadDispatch(void) : m_isDispatching(false)
	{
		m_lisDef.pDisp = this;
		m_piListener = &m_lisDef;
		m_thread.SetRunner(this);
	}

	/** �f�X�g���N�^ */
	virtual ~CThreadDispatch(void)
	{
		Stop();
	}

	/**
	 * [�o�^] ���X�i�[�o�^
	 *	@note	���X�i�[��o�^���Ȃ��ƁA���b�Z�[�W��M���A�{�N���X�� OnThreadDispatchReceive() ���R�[������܂��B
	 *			�i���X�i�[���g�p����ق��𐄏����܂��j
	 *	@param piListener ���X�i�[
	 */
	void SetListener(IListener* piListener)
	{
		m_piListener = (piListener != NULL) ? piListener : &m_lisDef;
	}

	/**
	 * [�ݒ�] �D��x�ݒ�.
	 *		���b�Z�[�W�L���[�X���b�h�̗D��x���w��ł��܂��B
	 *	@param priority �D��x�i-2�`2, 15�B0���m�[�}���B�傫���ق����D��x��j
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetPriority(int priority)
	{
		return m_thread.SetPriority(priority);
	}

	/**
	 * [�擾] �X���b�h��ID�擾
	 *	@return ID
	 */
	DWORD GetThreadId(void) const
	{
		return m_thread.GetId();
	}

	/**
	 * [�ݒ�] �X���b�h�J�n
	 *	@param lpszName �X���b�h�̖��O�iDEBUG�ȊO�ł͖�������܂��j
	 *	@retval true �����B
	 *	@retval false ���s(�X���b�h�N�����s/���ɃX���b�h���s��)
	 */
	bool Start(LPCTSTR lpszName = NULL)
	{
		m_isDispatching = false;
		if ( m_thread.IsAlive() )
		{
			return false;
		}
		#ifdef _DEBUG
			if ( lpszName == NULL) { lpszName = _T("ThreadDispatch:MessagePump"); }
		#endif
		if ( m_thread.Start(lpszName) )
		{
			DWORD dwTick = ::GetTickCount();
			while ( (::GetTickCount() - dwTick) < 5000 )
			{
				if ( this->PostMessage(WM_NULL) )
				{
					return true;
				}
				::Sleep(100);
			}
			m_thread.Stop();
		}
		return false;
	}

	/**
	 * [�ݒ�] �X���b�h��~
	 *	@retval true �����i�X���b�h���J�n���Ă��Ȃ�����Terminate�������������Ƃ��Ă��܂��j
	 *	@retval false ���s�i���X�i�[�X���b�h���� Stop() ���g�p����Ǝ��s���܂��j
	 */
	bool Stop(void)
	{
		m_isDispatching = false;
		if ( m_thread.IsAlive() )
		{
			m_thread.PostMessage(WM_QUIT);
			return m_thread.Stop();
		}
		return true;
	}

	/**
	 * [�m�F] �X���b�h�������Ă��邩
	 *	@retval true	�����Ă���
	 *	@retval false	���Ȃ�
	 */
	bool IsAlive(void) const
	{
		return m_thread.IsAlive();
	}

	/**
	 * [�ݒ�] �X���b�h��PostMessage
	 *	@param nMsg ���b�Z�[�W
	 *	@param wParam WPARAM�B�ȗ��\�B
	 *	@param lParam LPARAM�B�ȗ��\�B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		if ( m_thread.IsAlive() )
		{
			return m_thread.PostMessage(nMsg, wParam, lParam);
		}
		return false;
	}

	/**
	 * [�m�F] �������H
	 *	@retval true	������
	 *	@retval false	��
	 */
	bool IsDispatching(void) const
	{
		return m_isDispatching;
	}

	/**
	 * [�ʒm] ���b�Z�[�W��M.
	 *		�Ō�͕K�� nMsg��WM_QUIT�������Ēʒm����܂��B
	 *	@note	���X�i�[��o�^���Ȃ��ƁA���b�Z�[�W��M�����̃��\�b�h���R�[������܂��B
	 *			�{�N���X���p�����A�{���\�b�h���p�����邱�Ƃł��A�����������ł��܂��B
	 *			�i���X�i�[���g�p����ق��𐄏����܂��j
	 *	@param nMsg ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
	 */
	virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		ASSERT0(false, "CThreadDispatch", "�p�������X�i�o�^���s��ꂸ�Ɏg�p����Ă��܂��B");
		return false;
	}

	/**
	 * [�ʒm] �J�n�ʒm.
	 *		��M�X���b�h���J�n�����ƁA�ʒm����܂��B
	 *	@note	���X�i�[��o�^���Ȃ��ƁA���b�Z�[�W��M�����̃��\�b�h���R�[������܂��B
	 *			�{�N���X���p�����A�{���\�b�h���p�����邱�Ƃł��A�����������ł��܂��B
	 *			�i���X�i�[���g�p����ق��𐄏����܂��j
	 */
	virtual void OnThreadDispatchSetup(void)
	{
		ASSERT0(false, "CThreadDispatch", "�p�������X�i�o�^���s��ꂸ�Ɏg�p����Ă��܂��B");
	}

	/**
	 * [�Q��] �����X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

private:
	TNB::CThread	m_thread;			///< Dispatch�X���b�h�Ǘ�	
	IListener*		m_piListener;		///< ���X�i�[
	bool			m_isDispatching;	///< ������

	/**
	 * [�ʒm] �X���b�h�J�n�O�ʒm.
	 *		CThread::Start() ���s���A���^�[������O�ɂ��̃��\�b�h���V�K�X���b�h��ŃR�[������܂��B
	 */
	virtual void OnSetupThread(void) 
	{
		if ( m_piListener != NULL )
		{
			m_piListener->OnThreadDispatchSetup();
		}
	}

	/**
	 * �X���b�h�����֐�
	 *	@retval 0�A�Œ�
	 */
	virtual DWORD Run(void)
	{
		MSG msg;
		while ( ::GetMessage(&msg, NULL, 0, 0) > 0 )
		{
			if ( msg.message != WM_NULL && m_piListener != NULL )
			{
				m_isDispatching = true;
				bool r = m_piListener->OnThreadDispatchReceive(msg.message, msg.wParam, msg.lParam);
				m_isDispatching = false;
				if ( ! r )
				{
					this->PostMessage(WM_QUIT);
				}
				else
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
		}
		ASSERTLIB(msg.message == WM_QUIT);
		if ( m_piListener != NULL )
		{
			m_isDispatching = true;
			m_piListener->OnThreadDispatchReceive(msg.message, msg.wParam, msg.lParam);
			m_isDispatching = false;
		}
		return 0;
	}

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	/// �f�t�H���g���X�i
	struct TDefaultListener : IListener
	{
		CThreadDispatch* pDisp;
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			return pDisp->OnThreadDispatchReceive(nMsg, wParam, lParam);
		}
		virtual void OnThreadDispatchSetup(void)
		{
			pDisp->OnThreadDispatchSetup();
		}
	} m_lisDef;

	#endif //_TnbDOXYGEN
};



/**@ingroup THREAD
 * �}���`�X���b�h��������N���X
 *
 *		���b�Z�[�W�ʒm�ŁA�����̃X���b�h�œ��������𓯎��ɏ����������邱�Ƃ��ł��܂��B
 *
 *	@note	�����́A���X�i�[�����������N���X��o�^���܂��B
 *	
 *	@param COUNT �X���b�h��
 *
 *	@par�K�v�t�@�C��
 *			TnbThreadDispatch.h
 *
 *	@date 12/03/23	�V�K�쐬
 */
template<size_t COUNT>
class CMultiThreadDispatchT : CCopyImpossible
{
public:

	/**@ingroup THREAD
	 * �}���`�X���b�h��������̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		CMultiThreadDispatch�̃��X�i�[�ł��B���b�Z�[�W����M����ƒʒm����܂��B
	 *
	 *	@see CMultiThreadDispatch::SetListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbThreadDispatch.h
	 *
	 *	@date 12/03/23	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] ���b�Z�[�W��M.
		 *		�Ō�͕K�� nMsg �� WM_QUIT �������Ēʒm����܂��B
		 *	@param nMsg ���b�Z�[�W
		 *	@param wParam WPARAM
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnMultiDispatchReceive(UINT nMsg, WPARAM wParam) = 0;
	};


	//-----


	/// �R���X�g���N�^
	CMultiThreadDispatchT(void)
	{
		m_in.pListener = NULL;
		loop ( i, COUNT )
		{
			m_in.tasks[i].SetListener(&m_in);
			m_in.isTaskWorking[i] = false;
		}
		m_in.taskIndex = 0;
	}

	/** 
	 * [�ݒ�] ���X�i�[�o�^.
	 *	@param l ���X�i�[
	 */
	void SetListener(IListener* l)
	{
		m_in.pListener = l;
	}

	/**
	 * [���M] ���b�Z�[�W���M.
	 *		�󂢂Ă���X���b�h�Ƀ��b�Z�[�W��ʒm���܂��B�󂢂Ă��Ȃ��ꍇ�A�ŏ��̃X���b�h�ɒʒm���܂��B
	 *	@note ���X�i�[�� OnMultiDispatchReceive() �ɒʒm����܂��B
	 *	@param msg ���b�Z�[�W.
	 *	@param wParam WPARAM
	 */
	void PostMessage(UINT msg, WPARAM wParam = 0)
	{
		INDEX tn = 0;
		loop ( i, COUNT )
		{
			tn = (i + m_in.taskIndex) % COUNT;
			if ( ! m_in.isTaskWorking[tn] )
			{
				break;
			}
		}
		m_in.isTaskWorking[tn] = true;
		m_in.tasks[tn].PostMessage(msg, wParam, tn);
	}

	/**
	 * [���M] ���b�Z�[�W���M.
	 *		�w��̃X���b�h�Ƀ��b�Z�[�W��ʒm���܂��B
	 *	@note ���X�i�[�� OnMultiDispatchReceive() �ɒʒm����܂��B
	 *	@param index �X���b�h�C���f�b�N�X�B 0�ȏ�A COUNT �������w��ł��܂��B
	 *	@param msg ���b�Z�[�W.
	 *	@param wParam WPARAM
	 */
	void PostMessageToOne(INDEX index, UINT msg, WPARAM wParam = 0)
	{
		if ( index < COUNT )
		{
			m_in.isTaskWorking[index] = true;
			m_in.tasks[index].PostMessage(msg, wParam, index);
		}
	}

	/**
	 * [�ݒ�] �X���b�h�J�n
	 *	@retval true �����B
	 *	@retval false ���s(�X���b�h�N�����s/���ɃX���b�h���s��)
	 */
	bool Start(void)
	{
		bool r = true;
		m_in.taskIndex = 0;
		loop ( i, COUNT )
		{
			m_in.isTaskWorking[i] = false;
			#ifdef _DEBUG
				r &= m_in.tasks[i].Start(CStr::Fmt(_T("CMultiDispatch-%d"), i + 1));
			#else
				r &= m_in.tasks[i].Start();
			#endif
		}
		if ( ! r )
		{
			Stop();
		}
		return r;
	}

	/// [�ݒ�] �X���b�h��~
	void Stop(void)
	{
		loop ( i, COUNT )
		{
			m_in.tasks[i].Stop();
			m_in.isTaskWorking[i] = false;
		}
	}

private:
	/// �����N���X
	struct TInner : CThreadDispatch::IListener
	{
		DEFPARENTLISTENER(typename CMultiThreadDispatchT<COUNT>, IParentListener);
		IParentListener*	pListener;					///< ���X�i�[
		CThreadDispatch		tasks[COUNT];				///< �^�X�N
		bool				isTaskWorking[COUNT];		///< �^�X�N�g�p���H
		INDEX				taskIndex;					///< ���Ɏg���^�X�N�C���f�b�N�X	
		/// [�ʒm] ���b�Z�[�W��M.
		///		�Ō�͕K�� nMsg �� WM_QUIT �������Ēʒm����܂��B
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			TRACE1("OnMultiDispatch-%d\n", lParam + 1);
			if ( pListener != NULL )
			{
				pListener->OnMultiDispatchReceive(nMsg, wParam);
			}
			isTaskWorking[lParam] = false;
			return true;
		}
	};
	TInner		m_in;	///< �������
};



}; // TNB

