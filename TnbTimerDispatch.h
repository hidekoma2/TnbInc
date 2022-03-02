#pragma once
/**
 *@file
 * �^�C�}�[�����֌W�̃w�b�_
 *
 *		Window�������Ă����b�Z�[�W�|���v���Ȃ��Ă��^�C�}�[���g�����Ƃ��ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME THREAD
 * �^�C�}�[�����N���X
 *
 *		�w��̎��ԊԊu���Ƃɏ����������邱�Ƃ��o���܂��B
 *
 *	@note	�����́A���X�i�[�����������N���X��o�^���邩�A�{�N���X���p����
 *			OnTimer() ���I�[�o�[���C�h���܂��B�i�����͑O�ҁj
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbTimerDispatch.h
 *
 *	@date 06/08/03	�V�K�쐬
 *	@date 06/10/25	OnSetup() ���\�b�h�ǉ��B
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 *	@date 19/06/10	OnSetup() ���\�b�h���@�\���Ă��Ȃ������̂��C���B
 */
class CTimerDispatch
{
public:

	/**@ingroup TIME THREAD
	 * �^�C�}�[�����̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		CTimerDispatch �̃��X�i�[�ł��B�^�C�}�[����������ƒʒm����܂��B
	 *
	 *	@see CTimerDispatch::SetListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTimerDispatch.h
	 *
	 *	@date 06/08/02	�V�K�쐬
	 *	@date 06/10/25	OnSetup() ���\�b�h�ǉ��B
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �^�C�}�[����.
		 *		SetTimer() �Ŏw�肵�����Ԃ��o�߂���ƃR�[������܂��B KillTimer() ����܂ŁA
		 *		����I�ɃR�[������܂��B
		 *	@param uTimerId �^�C�}�[ID
		 *	@param lParam SetTimer() �Ŏw�肵�� �C�ӂ�32BIT�l
		 *	@param timer SetTimer���� CTimerDispatch �I�u�W�F�N�g�B SetTimer() KillTimer() ���g�p�ł��܂��B
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnTimer(UINT_PTR uTimerId, LPARAM lParam, const CTimerDispatch& timer) = 0;

		/**
		 * [�ʒm] �J�n�ʒm.
		 *	��M�X���b�h���J�n�����ƁA�ʒm����܂��B
		 */
		virtual void OnSetup(void) {}
	};

private:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	/// �����C�x���g�p�l
	enum
	{
		WMTT_SETTIMER	= WM_APP,		///< �^�C�}�[�J�n
		WMTT_KILLTIMER	= WM_APP + 1,	///< �^�C�}�[��~
	};

	/// ���������p�p�����[�^�^
	struct TParam
	{
		UINT_PTR	uTimerId;		///< �߂�l
		UINT		uElapse;		///< ����(ms)
		LPARAM		lParam;			///< �C�ӂ̃p�����[�^
		CSyncEvent	eventSuccess;	///< �߂�l�i�[�C�x���g
		/// �R���X�g���N�^
		TParam(UINT uEla, LPARAM lp) : uTimerId(0), uElapse(uEla), lParam(lp)
		{
			eventSuccess.ToLock();
		}
	};

	/// Dispatch���X�i�[
	class CListener : public CThreadDispatch::IListener
	{
		DEFPARENTLISTENER(CTimerDispatch, IParentListener);
	public:

		/// �����ݒ�
		void Set(CTimerDispatch* P, IParentListener* I)
		{
			m_pcTimer	 = P;
			m_piListener = I;
		}

	private:
		/// ���b�Z�[�W��M.
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			switch ( nMsg )
			{
			case WMTT_SETTIMER:
				{
					TParam* P = reinterpret_cast<TParam*>(lParam);
					UINT_PTR i = ::SetTimer(NULL, 0, P->uElapse, reinterpret_cast<TIMERPROC>(P->lParam));
					ASSERT(i != 0);
					P->uTimerId = i;
					P->eventSuccess.Unlock();
				}
				break;
			case WMTT_KILLTIMER:
				::KillTimer(NULL, lParam);
				break;
			case WM_TIMER:
				ASSERTLIB(m_piListener != NULL);
				ASSERTLIB(m_pcTimer != NULL);
				return m_piListener->OnTimer(wParam, lParam, *m_pcTimer);
				break;
			default:
				break;
			}
			return true;
		}
		/// [�ʒm] �J�n�ʒm
		virtual void OnThreadDispatchSetup(void)
		{
			ASSERTLIB(m_piListener != NULL);
			m_piListener->OnSetup();
		}
		IParentListener*	m_piListener;	///< TimerDispatch�̃��X�i�[
		CTimerDispatch*		m_pcTimer;		///< TimerDispatch�̎Q��
	};

	friend class CTimerDispatch::CListener;	///< �t�����h�錾
	CThreadDispatch		m_dispatch;			///< Dispatch�N���X
	CListener			m_listener;			///< Dispatch���X�i�[�N���X

	/// �f�t�H���g���X�i�[
	struct TDefaultListener : IListener
	{
		virtual bool OnTimer(UINT_PTR uTimerId, LPARAM lParam, const CTimerDispatch& timer)
		{
			return const_cast<CTimerDispatch&>(timer).OnTimer(uTimerId, lParam);
		}
	} m_lisDef;

	#endif //_TnbDOXYGEN

public:

	/// �R���X�g���N�^
	CTimerDispatch(void)
	{
		m_listener.Set(this, &m_lisDef);
		m_dispatch.SetListener(&m_listener);
	}

	/// �f�X�g���N�^
	virtual ~CTimerDispatch(void)
	{
		Stop();
	}

	/**
	 * [�o�^] ���X�i�[�o�^
	 *	@note	���X�i�[��o�^���Ȃ��ƁA�^�C�}�[�������A�{�N���X�� OnTimer() ���R�[������܂��B
	 *			�i���X�i�[���g�p����ق��𐄏����܂��j
	 *	@param P ���X�i�[
	 */
	void SetListener(IListener* P)
	{
		m_listener.Set(this, (P != NULL) ? P : &m_lisDef);
	}

	/**
	 * [�ݒ�] �X���b�h�J�n
	 *	@param lpszName �X���b�h�̖��O�iDEBUG�ȊO�ł͖�������܂��j
	 *	@retval true �����B
	 *	@retval false ���s(�X���b�h�N�����s/���ɃX���b�h���s��)
	 */
	bool Start(LPCTSTR lpszName = NULL)
	{
		#ifdef _DEBUG
			if ( lpszName == NULL) { lpszName = _T("TimerDispatch:MessagePump"); }
		#endif
		return m_dispatch.Start(lpszName);
	}

	/**
	 * [�ݒ�] �X���b�h��~
	 *	@retval true �����i�X���b�h���J�n���Ă��Ȃ�����Terminate�������������Ƃ��Ă��܂��j
	 *	@retval false ���s�i���X�i�[�X���b�h���� Stop() ���g�p����Ǝ��s���܂��j
	 */
	bool Stop(void)
	{
		return m_dispatch.Stop();
	}

	/**
	 * [�ݒ�] �^�C�}�[�Z�b�g
	 *	@param uElapse �^�C���A�E�g�l (�~���b�w��)
	 *	@param lParam �C�ӂ�32BIT�̒l�B
	 *	@retval 0 ���s�B
	 *	@retval 0�ȊO �����B�l�̓^�C�}�[ID�B
	 */
	UINT_PTR SetTimer(UINT uElapse, LPARAM lParam = 0) const
	{
		if ( m_dispatch.GetThreadId() == ::GetCurrentThreadId() )
		{
			return ::SetTimer(NULL, 0, uElapse, reinterpret_cast<TIMERPROC>(lParam));
		}
		TParam t(uElapse, lParam);
		if ( m_dispatch.PostMessage(WMTT_SETTIMER, 0, reinterpret_cast<LPARAM>(&t)) )
		{
			t.eventSuccess.Lock();	//Post��̏������I���̂�҂�
			return t.uTimerId;		//Post��Œl�����Ă���Ă���
		}
		return 0;
	}

	/**
	 * [�ݒ�] �^�C�}�[�j��
	 *	@param uTimerId �^�C�}�[ID�B SetTimer() �̖߂�l���g���܂��B
	 *	@retval true �����B(�s���� �^�C�}�[ID���w�肵�Ă������ɂȂ�܂�)
	 *	@retval false ���s�B( �X���b�h�������Ă��Ȃ�)
	 */
	bool KillTimer(UINT_PTR uTimerId) const
	{
		if ( m_dispatch.GetThreadId() == ::GetCurrentThreadId() )
		{
			return !! ::KillTimer(NULL, uTimerId);
		}
		return m_dispatch.PostMessage(WMTT_KILLTIMER, 0, uTimerId);
	}

	/**
	 * [�Q��] �����X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_dispatch.ReferThreadStatus();
	}

	/**
	 * [�ʒm] �^�C�}�[����.
	 *		SetTimer() �Ŏw�肵�����Ԃ��o�߂���ƃR�[������܂��B KillTimer() ����܂ŁA
	 *		����I�ɃR�[������܂��B
	 *	@note	���X�i�[��o�^���Ȃ��ƁA�^�C�}�[���������̃��\�b�h���R�[������܂��B
	 *			�{�N���X���p�����A�{���\�b�h���p�����邱�Ƃł��A�����������ł��܂��B
	 *			�i���X�i�[���g�p����ق��𐄏����܂��j
	 *	@param uTimerId �^�C�}�[ID
	 *	@param lParam SetTimer() �Ŏw�肵�� �C�ӂ�32BIT�l
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
	 */
	virtual bool OnTimer(UINT_PTR uTimerId, LPARAM lParam)
	{
		ASSERT0(false, "CTimerDispatch", "�p�������X�i�o�^���s��ꂸ�Ɏg�p����Ă��܂��B");
		return false;
	}

private:
	friend class CTimerDispatchTest;
};



};//TNB
