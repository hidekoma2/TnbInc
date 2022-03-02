#pragma once
/**
 *@file
 * ���L�������֌W�̃w�b�_
 *
 *		�قȂ�v���W�F�N�g�ŏ������L���邽�߂̂��̂ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup MEMORY
 * �ȈՋ��L�������Ǘ��N���X
 *
 *		��̋��L��������ێ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSharedMemory.h
 * 
 *	@date 06/01/01 �V�K
 *	@date 10/12/01 �R���X�g���N�^�̈����̐������C���B�f�X�g���N�^�� MAP �͊J������悤�ɏC���B
 */
class CSharedMemoryManager : CCopyImpossible
{
//protected:
	HANDLE	m_hFileMap;			///< ���L�������̃n���h��
	LPVOID	m_lpvMapAdds;		///< �}�b�v�����A�h���X
	bool	m_boCanClose;		///< �f�X�g���N�^���̓���
public:

	/**
	 * �R���X�g���N�^
	 *	@note ���̒i�K�ł͋��L�������͕ێ����Ă��܂���B
	 *	@param boCanClose false ���w�肷��ƁA�f�X�g���N�^���A���L���������N���[�Y���܂���B
	 */
	explicit CSharedMemoryManager(bool boCanClose = false)
	{
		m_lpvMapAdds = NULL;
		m_hFileMap = NULL;
		m_boCanClose = boCanClose;
	}

	/** �f�X�g���N�^ */
	virtual ~CSharedMemoryManager(void)
	{
		if ( m_boCanClose )
		{
			Close();
		}
		else
		{
			if ( m_lpvMapAdds != NULL )
			{
				::UnmapViewOfFile(m_lpvMapAdds);
			}
		}
	};

	/**
	 * [�m�F] ���L�������̗L��
	 *	@retval true �����Ă���
	 *	@retval false �����Ă��Ȃ�(Open���Ă��Ȃ�)
	 */
	bool HasMemory(void)
	{
		return Get() != NULL;
	}

	/**
	 * [�擾] �A�h���X�Q��.
	 *		�{�N���X�ŊǗ����Ă��鋤�L�������̃A�h���X��Ԃ��܂��B
	 *	@retval	NULL		Open���Ă��Ȃ�
	 *	@retval NULL�ȊO	�������̃A�h���X
	 */
	LPVOID Get(void)
	{
		if ( m_lpvMapAdds != NULL )
		{
			// ���̃N���X�̋��L�������͂��łɃI�[�v�����Ă���
			return m_lpvMapAdds;
		}
		return NULL;
	}

	/**
	 * [����] �I�[�v��
	 *	@note ���łɃI�[�v�����Ă�����Close���Ă���I�[�v�����܂�
	 *	@param lpszMemName ���L���������B�C���X�^���X���̂ݗL���ȋ��L�������ɂȂ�܂��B
	 *	@param uMemSize ���L�������T�C�Y�B
	 *	@retval	-1		Open���s
	 *	@retval 0		Open�����i���߂Ă̍쐬�j
	 *	@retval 1		Open�����i�����������ɐڑ��BuMemSize�͖�������܂����j
	 */
	int Open(LPCTSTR lpszMemName, UINT uMemSize)
	{
		if ( m_lpvMapAdds != NULL )
		{
			// ���̃N���X�̋��L�������͂��łɃI�[�v�����Ă���
			Close();
		}
		// ���L�������n���h���I�[�v��
		m_hFileMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, uMemSize, lpszMemName);
		if ( m_hFileMap != NULL )
		{
			// ���L�������I�[�v�������I
			DWORD dwError = ::GetLastError();
			//�@�}�b�s���O����B
			m_lpvMapAdds = ::MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if ( m_lpvMapAdds != NULL )
			{
				return (dwError == ERROR_ALREADY_EXISTS) ? 1/*����*/ : 0/*�V�K*/;
			}
			// �}�b�s���O���s
			_GetLastError("MapViewOfFile");
			::CloseHandle(m_hFileMap);		// �n���h������Ȃ��Ă�
			m_lpvMapAdds = NULL;
		}
		else
		{
			// ���L�������I�[�v�����s
			_GetLastError("CreateFileMapping");
			m_lpvMapAdds = NULL;
		}
		return -1;
	}

	/**
	 * [����] �N���[�Y.
	 *		�{�N���X�ŊǗ����Ă��鋤�L���������N���[�Y���܂�
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool Close(void)
	{
		if ( m_lpvMapAdds == NULL )
		{
			// �J���Ă��Ȃ������i���g�p�������j
			return true;
		}
		if ( ! ::UnmapViewOfFile(m_lpvMapAdds) )
		{
			//�A���}�b�v�h���s
			return false;
		}
		m_lpvMapAdds = NULL;
		if ( m_hFileMap != 0 )	//�O�̂��߃`�F�b�N
		{
			if ( ! ::CloseHandle(m_hFileMap) )
			{
				//�n���h�����N���[�Y�ł��Ȃ�����			
				return false;
			}
		}
		return true;
	}
};



/**@ingroup MEMORY
 * ���L�\���̊Ǘ��e���v���[�g
 *
 *		��̎w��̍\���̌^�̋��L��������ێ����܂��B
 *
 *		�I�[�v�������X�i�[( IListener )���w�肷�邱�ƂŁA���߂ẴI�[�v���̎�����
 *		���L�������̏��������ł��܂��B
 *		
 *		Open�����C���X�^���X�͎w�肵���^�̃|�C���^�Ƃ��đ��삪�\�ł��B
 *
 *	@note	���L�������ɂ����̂ŁA�R���X�g���N�^�������Ȃ��Ă����̂Ȃ��^��
 *			�w�肵�Ă��������B
 *	
 *	@param TYP ���L�������ɂ����\���́i 'operator��' ���g���Ă���̂� int�Ȃǂ͎g���܂���j
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		struct ISM : CSharedMemoryT<Bool>::IListener
 *		{
 *		    virtual void OnAction(Bool* P)
 *		    {
 *		        *P = false;
 *		    }
 *		} ism;
 *		CSharedMemoryT<Bool> ll1;
 *		CSharedMemoryT<Bool> ll2;
 *		ll1.Open( _T("AAA"), &ism );
 *		ll2.Open( _T("AAA"), &ism ); //�������L���������Ȃ̂Ń��X�i�[�͌Ă΂�Ȃ�
 *		ASSERT( ll2->IsFalse() ); //�iTYP�̃|�C���^�Ɠ����L�q���\�j
 *		(*ll1) = true;
 *		ASSERT( ll2->IsTrue() ); //�������������w���Ă���̂ŉe��������
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbSharedMemory.h
 * 
 *	@date 06/01/01 �V�K�쐬
 *	@date 06/04/20 ���X�i�[�ǉ�
 *	@date 06/04/26 �e���v���[�g���ύX�B�Ō�̈�͏����Ȃ��I�v�V�����ǉ��B
 *	@date 06/09/12 const���l���B
 */
template<typename TYP>
class CSharedStructT : CCopyImpossible
{
	CSharedMemoryManager m_cSharedMem;	///< ���L�������Ǘ�
	TYP*	m_pType;					///< �Ώۂ̃������A�h���X
	bool	m_boCanClose;				///< true�Ȃ� Close���ɋ��L��������Close

public:

	/**@ingroup MEMORY
	 * ���L�\���̊Ǘ��̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		���߂Ă�Open,�Ō��Close���̒ʒm���s���܂��B
	 *
	 *	@see CSharedStructT::Open()
	 *	@see CSharedStructT::Close()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbSharedMemory.h
	 * 
	 *	@date 06/04/20 �V�K�쐬
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �A�N�V��������
		 *	@note Open�Ŏg�p�����ꍇ�A���߂č쐬���ꂽ���ɃR�[�������B
		 *	@note Close�Ŏg�p�����ꍇ�A���L��������j�����钼�O�ɃR�[�������B
		 *	@param P �m�ۂ����������̃|�C���^
		 */
		virtual void OnAction(TYP* P) = 0;
	};

	/**
	 * �R���X�g���N�^
	 *	@note ���̒i�K�ł͋��L�������͕ێ����Ă��܂���B
	 *	\par�d�v:
	 *			���L�������́A���ׂĂ̎Q�Ƃ��N���[�Y�����ƁA�j������܂��B
	 *			�������AboCanClose ��false���w�肵�Ă����ƁA�{�N���X�ň�x�I�[�v������
	 *			���L�������́A�v���Z�X���I������܂ŃN���[�Y����Ȃ��Ȃ�܂��B		<BR>
	 *			�V�X�e���Ƃ��ăO���[�o���ɏ���ێ���������K�v������ꍇ�ɁA
	 *			���̃I�v�V�������g�p���邩�A�{�C���X�^���X�̈���O���[�o����
	 *			���������Ă��������B
	 *	@param boCanClose false �Ȃ�N���[�Y�����L����������Ȃ��Btrue���ȗ��Ȃ����B
	 */
	explicit CSharedStructT(bool boCanClose = true) : m_cSharedMem(boCanClose)
	{
		m_boCanClose = boCanClose;
		m_pType = NULL;
	}

	/** �f�X�g���N�^ */
	virtual ~CSharedStructT(void)
	{
		Close();
	}

	/**
	 * [�m�F] ���L�������̗L��
	 *	@retval true �����Ă���
	 *	@retval false �����Ă��Ȃ�(Open���Ă��Ȃ�)
	 */
	bool HasMemory(void) const
	{
		return m_pType != NULL;
	}

	/**
	 * [�擾] �������̎Q��.
	 *	@note Open���Ă��Ȃ���NULL���Ԃ�܂��B
	 *	@return �A�h���X
	 */
	operator TYP*(void)
	{
		return m_pType;
	}

	/**
	 * [�擾] �������̎Q��.
	 *	@note Open���Ă��Ȃ���NULL���Ԃ�܂��B
	 *	@return �A�h���X
	 */
	operator const TYP*(void) const
	{
		return m_pType;
	}

	/**
	 * [�擾] �������̎Q��.
	 *	@note Open���Ă��Ȃ���NULL���Ԃ�܂��B
	 *	@return �A�h���X
	 */
	TYP* operator->(void)
	{
		return m_pType;
	}

	/**
	 * [�擾] �������̎Q��.
	 *	@note Open���Ă��Ȃ���NULL���Ԃ�܂��B
	 *	@return �A�h���X
	 */
	const TYP* operator->(void) const
	{
		return m_pType;
	}

	/**
	 * [����] �I�[�v��
	 *	@note	�ŏ��ɃI�[�v������ƃ��X�i�[���R�[������܂��B���X�i�[���s���́A
	 *			��ڈȍ~�̃I�[�v���̓u���b�N����܂��B���̂��߁A���X�i�[�ɂ�鏉������
	 *			�ۏႳ��܂��B
	 *	@note	�{�N���X�ł��łɃI�[�v�����Ă�����Close���Ă���I�[�v�����܂��B
	 *	@param lpMemName ���L���������B
	 *			�ȗ�����ƃC���X�^���X���̂ݗL���ȋ��L�������ɂȂ�܂��B
	 *			����TYP�Ɠ������O�ɂȂ�Ȃ��悤�ɂ��Ă��������B
	 *	@param piListener ���X�i�[�B�ȗ��\�B�w�肷��Ə��߂�Open�������� OnAction ���R�[������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR lpMemName = NULL, IListener* piListener = NULL)
	{
		int iRc = m_cSharedMem.Open(lpMemName, sizeof(LONG) * 2 + sizeof(TYP));
		if ( iRc < 0 )
		{
			return false;
		}
		LONG* P = static_cast<LONG*>(m_cSharedMem.Get());
		m_pType = reinterpret_cast<TYP*>(&P[2]);
		if ( iRc == 0 )
		{
			//==�V�K
			if ( piListener != NULL )
			{
				piListener->OnAction(m_pType);
			}
			P[1]++;	//�Q�ƃJ�E���^
			P[0] = 1;	//�������ς�
		}
		else
		{
			//==����
			while ( P[0] != 1 )
			{
				::Sleep(1); 
			}
			P[1]++;	//�Q�ƃJ�E���^
		}
		return true;
	}

	/** 
	 * [����] �N���[�Y
	 *	@param piListener ���X�i�[�B�ȗ��\�B�w�肷��Ɣj�����O��OnAction���R�[������܂��B
	 *					�������A�R���X�g���N�^�� boCanClose �� false ���w�肵�Ă���ƁA�Ō��
	 *					�N���[�Y�͂��Ȃ��̂ŁAOnAction()�͐�΃R�[������܂���B
	 */
	void Close(IListener* piListener = NULL)
	{
		LONG* P = static_cast<LONG*>(m_cSharedMem.Get());
		if ( P == NULL ){ return; }
		while ( true )
		{
			while ( P[0] != 1 )
			{
				::Sleep(1); 
			}
			if ( ::InterlockedDecrement(&P[0]) == 0 )
			{
				break;
			}
		}
		if ( ::InterlockedDecrement(&P[1]) <= 0 )
		{
			//�Ō�̈��
			if ( piListener != NULL )
			{
				piListener->OnAction(m_pType);
			}
			if ( ! m_boCanClose )
			{
				//���Ȃ��A������J�E���^�͂P�̂܂܂�
				P[1] = 1;
			}
			P[0] = 1;
			if ( m_boCanClose )
			{
				//����
				m_cSharedMem.Close();
			}
		}
		else
		{
			P[0] = 1;
			m_cSharedMem.Close();
		}
		m_pType = NULL;
	}
};



}; // TNB
