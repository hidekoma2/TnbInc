#pragma once 
/**
 *@file
 * �X���b�h���[�J���I�u�W�F�N�g�Ǘ��֌W�̃w�b�_
 *
 *		��̃C���X�^���X�����A�ʁX�̃X���b�h����A�N�Z�X���Ă�
 *		������Ȃ��X���b�h���[�J���ȃI�u�W�F�N�g���Ǘ�����N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMap.h"



namespace TNB
{



/**@ingroup THREAD
 * �X���b�h���[�J���I�u�W�F�N�g�e���v���[�g�N���X
 *	
 *		��̃C���X�^���X�����A�ʁX�̃X���b�h����A�N�Z�X���Ă�
 *		������Ȃ��X���b�h���[�J���ȃI�u�W�F�N�g���Ǘ����܂��B
 *
 *	@note	�쐬�ł��鐔����v���Z�X��1000�܂łƂȂ��Ă��܂��B
 *			����K�v�Ƃ���ꍇ�A CThreadLocalObjectExT ���g�p���܂��B
 *			�������{�N���X�̂ق��������͍����ł��B
 *
 *	@note	int �� DWORD �Ȃǂ̊�{�^�́A CThreadLocalStorageT ���g�p�ł��܂��B
 *			�{�N���X��荂���ł��B
 *
 *	@param TYP �X���b�h���[�J���ŊǗ��������^�B�R�s�[�@�\�K�{�B
 *
 *	@par�K�v�t�@�C��
 *			TnbThreadLocalObject.h
 *
 *	@date 06/10/23 �V�K
 */
template<typename TYP>
class CThreadLocalObjectT
{
	DWORD						m_dwTlsIndex;	///< TLS �C���f�b�N�X
	TYP							m_typ;			///< �I�u�W�F�N�g�̏����l
	mutable CVectorT<TYP*>		m_vpTyp;		///< �m�ۃI�u�W�F�N�g�Q
	/// �l�i�[�̃|�C���^�擾
	TYP& m_Get(void) const
	{
		TYP* P = static_cast<TYP*>(::TlsGetValue(m_dwTlsIndex));
		if ( P == NULL )
		{
			P = new TYP(m_typ);
			m_vpTyp.Add(P);
			::TlsSetValue(m_dwTlsIndex, P);
		}
		return *P;
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param t �l�̏�����ԁB�ȗ������ TYP() �ɂȂ�܂��B
	 */
	CThreadLocalObjectT(const TYP& t = TYP()) : m_typ(t)
	{
		m_dwTlsIndex = ::TlsAlloc();
		if ( m_dwTlsIndex == TLS_OUT_OF_INDEXES )
		{
			throw CEmptyException();
		}
	}

	/// �f�X�g���N�^
	~CThreadLocalObjectT(void)
	{
		loop ( i, m_vpTyp.GetSize() )
		{
			delete m_vpTyp.At(i);
		}
		m_vpTyp.RemoveAll();
		::TlsFree(m_dwTlsIndex);
	}

	/**
	 * [�擾] �l�擾
	 *	@note Thread���ɊǗ����قȂ�܂��B�R���X�g���N�^�Ŏw�肵���l���A�����X���b�h�� Set() �����l�����܂��B
	 *	@return �l�B
	 */
	const TYP& Get(void) const
	{
		return m_Get();
	}

	/**
	 * [�ݒ�] �l�ݒ�
	 *	@note Thread���ɊǗ����قȂ�܂��B�ݒ肵�Ȃ��ƁA�R���X�g���N�^�Ŏw�肵���l�ɂȂ��Ă��܂��B
	 *	@param t �l�B
	 */
	void Set(const TYP& t)
	{
		m_Get() = t;
	}
};



/**@ingroup THREAD
 * �g���X���b�h���[�J���I�u�W�F�N�g�e���v���[�g�N���X
 *	
 *		��̃C���X�^���X�����A�ʁX�̃X���b�h����A�N�Z�X���Ă�
 *		������Ȃ��X���b�h���[�J���ȃI�u�W�F�N�g���Ǘ����܂��B
 *
 *	@note	MAP���g���ĊǗ����邽�߁A�C���X�^���X�̌��E������܂���i�������̌���쐬�\�j�B
 *			������ CThreadLocalObjectT �̂ق��������ł��B
 *
 *	@note	int �� DWORD �Ȃǂ̊�{�^�́A CThreadLocalStorageT ���g�p�ł��܂��B
 *			�{�N���X��荂���ł��B
 *
 *	@param TYP �X���b�h���[�J���ŊǗ��������^�B�R�s�[�@�\�K�{�B
 *
 *	@par�K�v�t�@�C��
 *			TnbThreadLocalObject.h
 *
 *	@date 06/10/23 �V�K
 */
template<typename TYP>
class CThreadLocalObjectExT
{
	TYP							m_typ;			///< �I�u�W�F�N�g�̏����l
	mutable CMapT<DWORD, TYP>	m_mapTyp;		///< �m�ۃI�u�W�F�N�g�Q
public:

	/**
	 * �R���X�g���N�^
	 *	@param t �l�̏�����ԁB�ȗ������ TYP() �ɂȂ�܂��B
	 */
	CThreadLocalObjectExT(const TYP& t = TYP()) : m_typ(t)
	{
	}

	/// �f�X�g���N�^
	~CThreadLocalObjectExT(void)
	{
	}

	/**
	 * [�擾] �l�擾
	 *	@note Thread���ɊǗ����قȂ�܂��B�R���X�g���N�^�Ŏw�肵���l���A�����X���b�h�� Set() �����l�����܂��B
	 *	@return �l�B
	 */
	const TYP& Get(void) const
	{
		EXCLUSIVE(&m_mapTyp);
		DWORD dw = ::GetCurrentThreadId();
		if ( ! m_mapTyp.HasKey(dw) )
		{
			m_mapTyp[dw] = m_typ;
		}
		return m_mapTyp[dw];
	}

	/**
	 * [�ݒ�] �l�ݒ�
	 *	@note Thread���ɊǗ����قȂ�܂��B�ݒ肵�Ȃ��ƁA�R���X�g���N�^�Ŏw�肵���l�ɂȂ��Ă��܂��B
	 *	@param t �l�B
	 */
	void Set(const TYP& t)
	{
		EXCLUSIVE(&m_mapTyp);
		DWORD dw = ::GetCurrentThreadId();
		if ( ! m_mapTyp.HasKey(dw) )
		{
			m_mapTyp[dw] = m_typ;
		}
		m_mapTyp[dw] = t;
	}
};



};



#if 0

DWORD gdwTlsIndex = 0;

CThreadLocalStorageT<DWORD> s_tsl_dwData;
CThreadLocalObjectT<CStr> s_tsl_strData = "#";



class Receiver : public CSocketUdp::IListener
{
public:
	virtual bool OnSocketEvent(const CSocketEvent& event)
	{
		DWORD r = s_tsl_dwData.Get();
		s_tsl_dwData.Set(::GetCurrentThreadId());
		TRACE2("[%08X] : %08X\n",::GetCurrentThreadId(), r);

		CStr s = s_tsl_strData.Get();
		s_tsl_strData.Set("Thread!!");
		TRACE2("[%08X] : %s\n",::GetCurrentThreadId(),s);




		if ( event.HasEvent() )
		{
			TRACE1("%s\n",event.GetEventString());
		}
		else
		{
			TNB::DumpData(event.GetDataSize(),event.GetData());
			CStr str;
			WORD wPort;
			event.GetPeerName(str, wPort);
			TRACE2("%s %d\n", str, wPort);
			event.Send(2,"AB");
		}
		return true;
	}
};



void CTnbTestDlg::OnBtnVir() 
{
	CDwordVector vdwIndex;
	while ( true )
	{
		DWORD d = ::TlsAlloc();
		if ( d == TLS_OUT_OF_INDEXES )
		{
			break;
		}
		vdwIndex.Add(d);
	}
	TRACE1("max= %d\n",vdwIndex.GetSize());




	DWORD r = s_tsl_dwData.Get();
	s_tsl_dwData.Set(10222);

	CStr s = s_tsl_strData.Get();
	s_tsl_strData.Set("Main");


#endif
