#pragma once
/**
 *@file
 * �ÓI�������Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSingleton.h"



//TNB Library
namespace TNB
{



/**@ingroup MEMORY
 * �ÓI�������Ǘ��N���X
 *
 *		�T�C�Y�̈قȂ�o���N���ő�256�쐬���A�w��̃o���N����w��̃T�C�Y�����z����
 *		�g�p���邱�Ƃ��o���܂��B new/delete ���J��Ԃ����Ƃɂ�郁�����̕��f��A
 *		�������m�ۑ��x���C�ɂȂ�ꍇ�Ɏg�p���邱�Ƃ��o���܂��B
 *
 *		�C���X�^���X�́A Allocate() ���邱�Ƃŗ̈���m�ۂł��܂��B
 *		���邢�͊m�ۂ��Ă���C���X�^���X���R�s�[���邱�Ƃł��m�ۂł��܂�
 *		�i���̏ꍇ�A�����̈���Q�Ƃ��Ă���j�B
 *		�m�ۂ��Ă����Ԃ� Lock() �����邱�ƂŎ��ۂ̃������A�h���X�������܂��B
 *
 *		�s�K�v�ɂȂ�΁A Free() �A���邢�̓C���X�^���X���f�X�g���N�g���܂��B
 *
 *		�Ȃ��A Lock() �� Unlock() ���Ȃ��ƁA ResizeBank() �� DeleteBank() ���o���Ȃ��Ȃ�܂��B
 *		�t�ɂ������g��Ȃ��̂ł���΁A Unlock() ���g���K�v�͗L��܂���B
 *
 *	@note	�̈���m�ۂ��������C���X�^���X���R�s�[����Ƌ��L����܂��i�����Q�Ƃ���Ă����ԁj�B
 *			�����āA�f�X�g���N�^��ʂ̃C���X�^���X�̃R�s�[�A Free()�Ȃǂ�
 *			�Q�Ƃ����Ȃ��Ȃ�΁A���̃������͎����I�ɉ������܂��B
 *
 *		���܂�(!)�ŁA32BIT�̃p�����[�^�������Ƃ��o���܂��B�{�N���X���֐��̖߂�l�Ȃǂ�
 *		�g�p����ꍇ�ɁA�L�����Ǝv���܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CStaticMemory::CreateBank(0, 1000, 10);// Bank0�� 1000x10�o�C�g�̃������m��
 *		     ;
 *		CStaticMemory h1;
 *		h1.Allocate(0, 1000);                 // Bank0���� 1000byte�m��
 *		CStaticMemory h2 = h1;               // h1 �� h2 �͓������������Q��
 *		LPVOID P = h2.Lock();                // ���ۂ̃������A�h���X�擾&Lock
 *		     ;
 *		h2.Unlock();                         // �g�p�� Unlock(Bank��ύX���Ȃ��Ȃ�s�K�v)		
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbStaticMemory.h
 *
 *	@date 06/07/21 �V�K�쐬
 *	@date 06/07/23 �f�t���O�ł���悤�Ɏd�l�C���B
 */
class CStaticMemory
{
public:

	/// ���U���g
	enum EResult
	{
		ER_Success,			///< ����
		ER_ExistBank,		///< ���łɃo���N������܂��B
		ER_NotFoundBank,	///< �o���N��������܂���B
		ER_OutOfMemory,		///< ������������܂���B
		ER_InvalidTell,		///< �s����TELL�ł��B
		ER_InvalidParam,	///< �s���ȃp�����[�^�ł��B
		ER_Locked,			///< ���b�N����Ă��܂��B
	};

private:

	/// �ÓI�������Ǘ��N���X�p�^�錾
	typedef DWORD TELL;

	/// �ÓI�������Ǘ��N���X�p�G���[�l�錾
	#define INVALID_TELL (ULONG_MAX)

	/// CStaticMemory�Ǘ��N���X
	class CManage
	{
		CSyncSection m_syncFunc;	///< �֐��r��
		/**
		 * ���Ǘ��N���X
		 */
		class CInfo
		{
			DWORD	m_dwLockCount;			///< ���b�N�J�E���^
			DWORD	m_dwSectorSize;			///< �Z�N�^�T�C�Y
			WORD	m_wSectorCount;			///< �Z�N�^��
			WORD	m_wCacheIndex;			///< ���̌���Index
			WORD	m_wCacheSecNo;			///< ���̌����Z�N�^No
			WORD	m_wUsedSectors;			///< �g�p���Ă���Z�N�^��
			BYTE*	m_pContent;				///< �v�[��
			/// Index->SecNo�ϊ��e�[�u���̃A�h���X
			WORD *index(void) { return reinterpret_cast<WORD*>(m_pContent); }
			/// �t���O�e�[�u���̃A�h���X
			WORD *flag(void)
			{
				return (m_pContent == NULL)
						? NULL
						: reinterpret_cast<WORD*>(&m_pContent[sizeof(WORD) * m_wSectorCount]); 
			}
			/// �f�[�^�̐擪�A�h���X
			BYTE* data(WORD wSecNo)
			{
				return (m_pContent == NULL)
						? NULL
						: &m_pContent[	sizeof(WORD) * m_wSectorCount + 
										sizeof(WORD) * m_wSectorCount + 
										m_dwSectorSize * wSecNo]; 
			}
			/// Index(TELL�̉���WORD) �� SecNo
			WORD m_IndexToSecNo(WORD i)
			{
				WORD* W = index();
				ASSERTLIB(W[i] != 0);
				return ToWord(W[i] & 0x7FFF);
			}
			/**
			 * �m��
			 *	@param wStart �����J�n�Z�N�^�ԍ�
			 *	@param wEnd �����I���Z�N�^�ԍ�+1
			 *	@param wLen �m�ۂ������Z�N�^��
			 *	@retval INVALID_TELL	�m�ۂł����B
			 *	@retval ��L�ȊO	�m�ۂ����擪�Z�N�^�ԍ��i�����j
			 */
			DWORD m_AllocSub(WORD wStart, WORD wEnd, WORD wLen)
			{
				WORD wNext;
				WORD* F = flag();
				while ( wStart < wEnd )
				{
					wNext = static_cast<WORD>(F[wStart] & 0x7FFF);
					if ( wNext == 0 )
					{
						wNext = m_wSectorCount;
					}
					if ( (F[wStart] & 0x8000) == 0 && (wNext - wStart) >= wLen )
					{
						//���҃Z�N�^���̋󂫂�������
						WORD w = static_cast<WORD>(wStart + wLen);
						if ( w >= m_wSectorCount )
						{ 
							w = 0;
						}
						for ( WORD i = 0; i < wLen; i++ )
						{
							F[wStart + i] = static_cast<WORD>(0x8000 | w);
						}
						m_wCacheSecNo = w;
						return wStart;
					}
					wStart = wNext;
				}
				return INVALID_TELL;
			}
		public:
			CSyncSection syncBank;		///< �r��
			/// �R���X�g���N�^
			CInfo(void)
			{
				m_wCacheIndex	= 0;
				m_wCacheSecNo	= 0;
				m_wUsedSectors	= 0;
				m_dwLockCount	= 0;
				m_dwSectorSize	= 0;
				m_wSectorCount	= 0;
				m_pContent		= NULL;		///< �v�[��
			}
			/// �f�X�g���N�^
			~CInfo(void)
			{
				if ( m_pContent != NULL )
				{
					delete[] m_pContent;
					m_pContent = NULL;
				}
			}
			/// �Z�N�^���擾
			WORD GetSectorCount(void) { return m_wSectorCount; }
			/// ���e�m�ۍς݊m�F
			bool IsValid(void) { return m_pContent != NULL; }
			/**
			 * �v�[���쐬
			 *	@param dwSectorSize �Z�N�^�T�C�Y�B16�ȏ�
			 *	@param wSectorCount �Z�N�^���B10�ȏ� 32768����
			 *	@return EResult
			 */
			EResult Create(DWORD dwSectorSize, WORD wSectorCount)
			{
				EXCLUSIVE(&syncBank);
				if ( m_pContent != NULL ) { return ER_ExistBank; }
				if ( dwSectorSize < 16 ) { return ER_InvalidParam; }
				if ( wSectorCount < 10 || wSectorCount >= 32768) { return ER_InvalidParam; }
				m_pContent = new BYTE[sizeof(WORD) * wSectorCount + (sizeof(WORD) + dwSectorSize) * wSectorCount];
				if ( m_pContent == NULL ){ return ER_OutOfMemory; }
				memset(m_pContent, 0, sizeof(WORD) * wSectorCount + sizeof(WORD) * wSectorCount);
				m_dwSectorSize = dwSectorSize;
				m_wSectorCount= wSectorCount;
				return ER_Success;
			}
			/// �v�[���j��
			void Delete(void)	
			{
				EXCLUSIVE(&syncBank);
				#ifdef _DEBUG
					if ( m_dwLockCount != 0 )
					{
						TRACE0("CStaticMemory::CManage::Delete() ; Lock/Unlock�̉񐔂������Ă��܂���B\n");
					}
				#endif
				if ( m_pContent != NULL )
				{
					delete[] m_pContent;
					m_pContent = NULL;
				}
				m_wCacheSecNo = 0;
				m_wCacheIndex = 0;
				m_wUsedSectors = 0;
				m_dwLockCount = 0;
			}
			/**
			 * �v�[���T�C�Y�ύX
			 *	@note ���������邱�Ƃ͋֎~����Ă��܂��B
			 *	@param wSectorCount �Z�N�^���B10�ȏ� 32768����
			 *	@return EResult
			 */
			EResult Resize(WORD wSectorCount)
			{
				EXCLUSIVE(&syncBank);
				if ( wSectorCount == 0 )
				{
					wSectorCount = m_wSectorCount;
				}
				else if ( m_wUsedSectors > wSectorCount )
				{
					return ER_OutOfMemory;	//����Ȃ��Ȃ�
				}
				else if ( m_wSectorCount > wSectorCount )
				{
					return ER_InvalidParam;	//�������Ȃ�
				}
				if ( m_pContent == NULL ){ return ER_NotFoundBank; }
				CInfo infoNew;
				EResult rc = infoNew.Create(m_dwSectorSize, wSectorCount);
				if ( rc != ER_Success ){ return rc; }
				//
				WORD* W = index();
				WORD* newW = infoNew.index();
				WORD* F = flag();
				WORD wPos;
				for ( WORD i = 0; i < m_wSectorCount; i++ )
				{
					wPos = W[i];
					if ( wPos != 0 )
					{
						wPos &= 0x7FFF;
						WORD wLen = static_cast<WORD>((F[wPos] & 0x7FFF) - wPos);
						DWORD r = infoNew.m_AllocSub(infoNew.m_wCacheSecNo, wSectorCount, wLen);
						if ( r == INVALID_TELL && infoNew.m_wCacheSecNo != 0 )
						{
							r = infoNew.m_AllocSub(0, infoNew.m_wCacheSecNo, wLen);
						}
						ASSERTLIB(r != INVALID_TELL);
						BYTE* P = infoNew.data(ToWord(r));
						ASSERTLIB(P != NULL);
						MemCopy(P, data(wPos), wLen * m_dwSectorSize);
						newW[i] = ToWord(0x8000 | r);
					}
				}
				m_wSectorCount= wSectorCount;
				m_wCacheSecNo = 0;
				m_wCacheIndex = 0;
				delete[] m_pContent;
				m_pContent = infoNew.m_pContent;
				infoNew.m_pContent = NULL;
				return ER_Success;
			}
			/**
			 * �g�p�Z�N�^��
			 */
			WORD GetUsedSectors(void)
			{
				return m_wUsedSectors;
			}
			/**
			 * �m��
			 *	@param size �m�ۂ�����Byte��
			 *	@retval INVALID_TELL	�m�ۂł����B
			 *	@retval ��L�ȊO	�m�ۂ���Index(�����j
			 */
			DWORD Allocate(size_t size)
			{
				EXCLUSIVE(&syncBank);
				if ( m_pContent == NULL ){ return INVALID_TELL; }
				WORD wLen = ToWord((ToDword(size) + m_dwSectorSize - 1) / m_dwSectorSize);
				if ( wLen + m_wUsedSectors > m_wSectorCount ){ return INVALID_TELL; }
				DWORD r = m_AllocSub(m_wCacheSecNo, m_wSectorCount, wLen);
				if ( r == INVALID_TELL && m_wCacheSecNo != 0 )
				{
					r = m_AllocSub(0, m_wCacheSecNo, wLen);
				}
				if ( r == INVALID_TELL )
				{
					return r;
				}
				//
				m_wUsedSectors = static_cast<WORD>(m_wUsedSectors + wLen);
				WORD* W = index();
				for ( WORD i = m_wCacheIndex; i < m_wSectorCount; i++ )
				{
					if ( W[i] == 0 )
					{
						W[i] = ToWord(0x8000 | r);
						m_wCacheIndex = i;
						return i;
					}
				}
				for ( WORD i = 0; i < m_wCacheIndex; i++ )
				{
					if ( W[i] == 0 )
					{
						W[i] = ToWord(0x8000 | r);
						m_wCacheIndex = i;
						return i;
					}
				}
				ASSERTLIB(false);
				return INVALID_TELL;
			}
			/**
			 * ���
			 *	@param wIndex Index(TELL�̉���WORD)
			 *	@retval true ����
			 *	@retval false ���s
			 */
			bool Free(WORD wIndex)
			{
				EXCLUSIVE(&syncBank);
				if ( m_pContent == NULL ){ return false; } // �v�[���Ȃ�
				if ( wIndex >= m_wSectorCount ){ return false; } // �ԍ��ُ�
				WORD wSecNo = m_IndexToSecNo(wIndex);
				ASSERTLIB(wSecNo < m_wSectorCount);
				WORD* F = flag();
				if ( (F[wSecNo] & 0x8000) == 0 ){ return false; } // �m�ۂ��Ă��Ȃ�
				m_wCacheSecNo = wSecNo;
				WORD w;
				WORD wNext = static_cast<WORD>(F[wSecNo] & 0x7FFF);
				if ( wNext == 0 )
				{
					//= �Ō�B�S���O�ɂ����Ⴄ
					wNext = m_wSectorCount;
					w = 0;
				}
				else
				{
					//= ���̃u���b�N�m�F
					if ( (F[wNext] & 0x8000) != 0 )
					{
						//�m�ۂ��Ă���.���̃u���b�N��Next
						w = wNext;
					}
					else
					{
						//�m�ۂ��Ă��Ȃ��B���̃u���b�N�̎��ɂ���B
						w = F[wNext];
					}
				}
				for ( WORD i = wSecNo; i < wNext; i++ )
				{
					F[i] = w;
				}
				index()[wIndex] = 0;
				m_wCacheIndex = wIndex;
				m_wUsedSectors = static_cast<WORD>(m_wUsedSectors - (wNext - wSecNo));
				return true;
			}
			/**
			 * �w��Z�N�^�̊m��Byte��
			 *	@note �m�ۂ����擪�̃Z�N�^�łȂ��ꍇ�̃G���[�ɂȂ�܂��B
			 *	@note ���ۂɊm�ۗv���̃T�C�Y�ł͂Ȃ��A�Z�N�^�T�C�Y�̌��{���Ƀp�f�B���O����܂��B
			 *	@param wIndex Index(TELL�̉���WORD)
			 *	@retval INVALID_TELL �G���[
			 *	@retval INVALID_TELL�ȊO �m��Byte��
			 */
			size_t GetSize(WORD wIndex)
			{
				EXCLUSIVE(&syncBank);
				if ( wIndex >= m_wSectorCount ){ return INVALID_TELL; } // �ԍ��ُ�
				WORD wSecNo = m_IndexToSecNo(wIndex);
				ASSERTLIB(wSecNo < m_wSectorCount);
				WORD* F = flag();
				if ( (F[wSecNo] & 0x8000) == 0 ){ return INVALID_TELL; } //�m�ۂ��ĂȂ�
				if ( wSecNo != 0 )
				{
					if ( F[wSecNo - 1] == F[wSecNo] ){ return INVALID_TELL; }//��O�Ɠ����̂͂����Ȃ�
				}
				return ((F[wSecNo] & 0x7FFF) - wSecNo) * m_dwSectorSize;
			}
			/**
			 * ���b�N�J�E���^�擾
			 *	@return ���b�N�J�E���^
			 */
			DWORD GetLockCount(void) { return m_dwLockCount; }
			/**
			 * ���b�N
			 *	@note ���b�N�J�E���^���{�P���A�A�h���X��Ԃ��܂��B
			 *	@param wIndex Index(TELL�̉���WORD)
			 *	@retval NULL �G���[�i�J�E���^�́{�P���܂���j
			 *	@retval NULL�ȊO �A�h���X
			 */
			LPVOID Lock(WORD wIndex)
			{
				EXCLUSIVE(&syncBank);
				if ( GetSize(wIndex) == INVALID_TELL ){ return NULL; }
				WORD wSecNo = m_IndexToSecNo(wIndex);
				ASSERTLIB(wSecNo < m_wSectorCount);
				ASSERT0(data(wSecNo) != NULL, "CStaticMemory::CManage::CInfo::Lock()", "�m�ۂ��Ă��܂���");
				m_dwLockCount++;
				return data(wSecNo);
			}

			/**
			 * �A�����b�N
			 *	@note ���b�N�J�E���^���[�P���܂��B
			 */
			void Unlock(void) 
			{
				m_dwLockCount--;
			}
		private:
			friend class CStaticMemoryTest;
		}; // CINfo

		//--------------------

		CInfo*	m_pInfo;			///< �C���t�H���
		/// �R���X�g���N�^
		SINGLETON_CONSTRUCTOR(CManage)
		{
			m_pInfo = new CInfo[256];
			ASSERT( m_pInfo != NULL );
		}
		/**
		 * TELL���e�̐����`�F�b�N
		 *	@retval true ������
		 *	@retval false �ُ�B
		 */
		bool m_CheckTell(TELL& tell, BYTE& bank)
		{
			bank = static_cast<BYTE>(tell >> 24);
			tell &= 0x0000FFFF;
			if ( ! m_pInfo[bank].IsValid() ){ return false; }
			return m_pInfo[bank].GetSectorCount() > tell;
		}
	public:
		/// �f�X�g���N�^
		~CManage(void)
		{
			if ( m_pInfo != NULL )
			{
				delete[] m_pInfo;
				m_pInfo = NULL;
			}
		}
		/**
		 * [�ݒ�] �o���N�쐬.
		 *		�w��o���N�̊m�ۃT�C�Y��ݒ�
		 *	@param bank �o���N�i�O�`�Q�T�T�j�B
		 *	@param dwSectorSize �Z�N�^�T�C�Y�B16�ȏ�
		 *	@param wSectorCount �Z�N�^���B10�ȏ� 32768����
		 *	@return EResult
		 */
		EResult CreateBank(BYTE bank, DWORD dwSectorSize, WORD wSectorCount)
		{
			EXCLUSIVE(&m_syncFunc);
			return m_pInfo[bank].Create(dwSectorSize, wSectorCount);
		}
		/**
		 * [�ݒ�] �o���N�폜.
		 *		�w��o���N�̊J��
		 *	@param bank �o���N�i�O�`�Q�T�T�j�B
		 *	@param boIsForce �ȗ�����ƁA Lock�`�F�b�N���s���܂��B<BR>
		 *					true ���w�肷��ƁALock����Ă��Ă�Bank��Delete���܂��B
		 *	@return EResult
		 */
		EResult DeleteBank(BYTE bank, bool boIsForce = false)
		{
			EXCLUSIVE2(&m_syncFunc,&m_pInfo[bank].syncBank);
			if ( ! m_pInfo[bank].IsValid() ){ return ER_NotFoundBank; }
			if ( ! boIsForce && m_pInfo[bank].GetLockCount() != 0 ){ return ER_Locked; }
			m_pInfo[bank].Delete();
			return ER_Success; 
		}
		/**
		 * [�ݒ�] �o���N�̂�T�C�Y.
		 *	@note ���������邱�Ƃ͋֎~����Ă��܂��B
		 *	@note ���b�N���Ă���Ǝ��s���܂��B
		 *	@note �Z�N�^�����ȗ����邱�ƂŁA�K�x�[�W�R���N�V�����ɂȂ�܂��B
		 *	@param bank �o���N�i�O�`�Q�T�T�j�B
		 *	@param wSectorCount �ύX�������Z�N�^��
		 *	@return EResult
		 */
		EResult ResizeBank(BYTE bank, WORD wSectorCount = 0)
		{
			EXCLUSIVE2(&m_syncFunc,&m_pInfo[bank].syncBank);
			if ( ! m_pInfo[bank].IsValid() ){ return ER_NotFoundBank; }
			if ( m_pInfo[bank].GetLockCount() != 0 ){ return ER_Locked; }
			return m_pInfo[bank].Resize(wSectorCount);
		}
		/**
		 * [�擾] �g�p�Z�N�^���擾.
		 *	@note �g�p�Z�N�^�����m�ۃZ�N�^����菬�����Ă��A�m�ۂł���Ƃ͌���܂���
		 *			�i ResizeBank() ������Ǝg�p�ł���͂��ł��j�B
		 *	@param bank �o���N�i�O�`�Q�T�T�j�B
		 *	@return �g�p�Z�N�^��
		 */
		WORD GetUsedSectors(BYTE bank)
		{
			return static_cast<WORD>(m_pInfo[bank].IsValid() ? m_pInfo[bank].GetUsedSectors() : 0);
		}
		/**
		 * [�ݒ�] �m��.
		 *		�w��Bank����w��T�C�Y���m�ۂ��܂��B
		 *	@param bank �o���N�i�O�`�Q�T�T�j�B
		 *	@param size �T�C�Y
		 *	@retval INVALID_TELL ���s
		 *	@retval ��L�ȊO TELL
		 */
		TELL Allocate(BYTE bank, size_t size)
		{
			if ( size == 0 || ! m_pInfo[bank].IsValid() ){ return INVALID_TELL; }
			DWORD dw = m_pInfo[bank].Allocate(size);
			return (dw == INVALID_TELL) ? INVALID_TELL : ((bank << 24) | dw); 
		}
		/**
		 * [�ݒ�] ���.
		 *	@param tell Allocate() �Ŏ擾���� TELL
		 *	@retval true	����
		 *	@retval false	�s����TELL�ł��B
		 */
		bool Free(TELL tell)
		{
			BYTE bank;
			return m_CheckTell(tell, bank) ? m_pInfo[bank].Free(ToWord(tell)) : false;
		}
		/**
		 * [�ݒ�] �擾.
		 *	@note ���ۂɊm�ۗv���̃T�C�Y�ł͂Ȃ��A�Z�N�^�T�C�Y�̌��{���Ƀp�f�B���O����܂��B
		 *	@param tell Allocate() �Ŏ擾���� TELL
		 *	@retval 0		�s����TELL�ł��B
		 *	@retval 0�ȏ�	�m��Byte��
		 */
		size_t GetSize(TELL tell)
		{
			BYTE bank;
			return m_CheckTell(tell, bank) ? m_pInfo[bank].GetSize(ToWord(tell)) : 0;
		}
		/**
		 * [�擾] ���������b�N.
		 *		TELL����A�h���X�𓾂܂��B
		 *	@note ���b�N���Ă���ƁAResizeBank() �� DeleteBank() �����s���܂��B
		 *	@param tell Allocate() �Ŏ擾���� TELL
		 *	@retval NULL�ȊO	�A�h���X
		 *	@retval NULL	�s����TELL�ł��B
		 */
		LPVOID Lock(TELL tell)
		{
			BYTE bank;
			return m_CheckTell(tell, bank) ? m_pInfo[bank].Lock(ToWord(tell)) : NULL;
		}

		/**
		 * [�擾] �������A�����b�N.
		 *	@param tell Allocate() �Ŏ擾���� TELL
		 *	@retval true	����
		 *	@retval false	�s����TELL�ł��B
		 */
		bool Unlock(TELL tell)
		{
			BYTE bank;
			if ( ! m_CheckTell(tell, bank) ){ return false; }
			m_pInfo[bank].Unlock();
			return true;
		}

	private:
		friend class CStaticMemoryTest;
	}; // CManage

	//--------------------

	CManage*	m_pMan;			///< CManage�̃C���X�^���X
	TELL		m_tell;			///< �Ǘ����Ă��� TELL
	DWORD		m_dwParam;		///< �p�����[�^
	/// ������
	void m_Init(void)
	{
		m_dwParam = 0;
		m_pMan = CManage::GetInstance();
		m_tell = INVALID_TELL;
	}

public:

	/**
	 * [�ݒ�] �o���N�쐬.
	 *		�w��o���N�̊m�ۃT�C�Y��ݒ�
	 *	@param bank �o���N
	 *	@param dwSectorSize �Z�N�^�T�C�Y�B16�ȏ�
	 *	@param wSectorCount �Z�N�^���B10�ȏ� 32768����
	 *	@retval ER_Success		����
	 *	@retval ER_ExistBank	���łɃo���N������܂��B
	 *	@retval ER_OutOfMemory	������������܂���B
	 *	@retval ER_InvalidTell	�s����TELL�ł��B
	 *	@retval ER_InvalidParam	�s���ȃp�����[�^�ł��B
	 */
	static EResult CreateBank(BYTE bank, DWORD dwSectorSize, WORD wSectorCount)
	{
		return CManage::GetInstance()->CreateBank(bank, dwSectorSize, wSectorCount);
	}

	/**
	 * [�ݒ�] �o���N�폜.
	 *		�w��o���N�̊J��
	 *	@param bank �o���N
	 *	@param boIsForce �ȗ�����ƁA Lock�`�F�b�N���s���܂��B<BR>
	 *					true ���w�肷��ƁALock����Ă��Ă�Bank��Delete���܂��B
	 *	@retval ER_Success		����
	 *	@retval ER_NotFoundBank	�o���N��������܂���B
	 *	@retval ER_Locked		���b�N����Ă��܂��B
	 */
	static EResult DeleteBank(BYTE bank, bool boIsForce = false)
	{
		return CManage::GetInstance()->DeleteBank(bank, boIsForce);
	}

	/**
	 * [�ݒ�] �o���N�̂�T�C�Y.
	 *	@note ���������邱�Ƃ͋֎~����Ă��܂��B
	 *	@note ���b�N���Ă���Ǝ��s���܂��B
	 *	@note ���T�C�Y���g�p��������擪�ɏW�߂܂��i�f�t���O�j�B
	 *	@note �Z�N�^�����ȗ����邱�ƂŁA����ύX�����f�t���O�̂ݍs���܂��B
	 *	@param bank �o���N
	 *	@param wSectorCount �ύX�������Z�N�^���B�ȗ��\�B
	 *	@retval ER_Success		����
	 *	@retval ER_NotFoundBank	�o���N��������܂���B
	 *	@retval ER_OutOfMemory	������������܂���B
	 *	@retval ER_InvalidParam	�s���ȃp�����[�^�ł��B
	 *	@retval ER_Locked		���b�N����Ă��܂��B
	 */
	static EResult ResizeBank(BYTE bank, WORD wSectorCount = 0)
	{
		return CManage::GetInstance()->ResizeBank(bank, wSectorCount);
	}

	/// �f�X�g���N�^
	~CStaticMemory(void)
	{
		Free();
	}
	
	/// �R���X�g���N�^
	CStaticMemory(void)
	{
		m_Init();
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param h �R�s�[���B
	 */
	CStaticMemory(const CStaticMemory& h)
	{
		m_Init();
		operator=(h);
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[���B
	 *	@return �����̎Q��
	 */
	CStaticMemory& operator=(const CStaticMemory& other)
	{
		Free();
		if ( other.m_tell != INVALID_TELL )
		{
			LONG* B = static_cast<LONG*>(m_pMan->Lock(other.m_tell));
			::InterlockedIncrement(B);
			m_pMan->Unlock(other.m_tell);
			m_tell = other.m_tell;
		}
		m_dwParam = other.m_dwParam;
		return *this;
	}

	/**
	 * [�擾] �p�����[�^�擾
	 *	@return �p�����[�^
	 */
	DWORD GetParam(void)
	{
		return m_dwParam;
	}

	/**
	 * [���] �p�����[�^�ݒ�
	 *	@param dwParam �p�����[�^
	 */
	void SetParam(DWORD dwParam)
	{
		m_dwParam = dwParam;
	}

	/**
	 * [�擾] ���L���擾.
	 *		�C���X�^���X���R�s�[����ƁA���L�����{�P����܂��B
	 *			1�̎��AFree(�j���f�X�g���N�g����ƁA���������������܂��B
	 *	@note �J���p�̃��\�b�h�ł��B�ʏ풼�ڎg���܂���B
	 *	@return ���L��
	 */
	LONG GetJointlyCount(void)
	{
		if ( m_tell == INVALID_TELL){ return 0; }
		LONG* B = static_cast<LONG*>(m_pMan->Lock(m_tell));
		LONG r = *B;
		m_pMan->Unlock(m_tell);
		return r;
	}

	/**
	 * [�m�F] �������ێ��m�F
	 *	@retval true �����Ă���B
	 *	@retval false �����Ă��Ȃ��B
	 */
	bool HasMemory(void)
	{
		return m_tell!=INVALID_TELL;
	}

	/**
	 * [�擾] �������m��
	 *	@note �m�ۂ̑O�ɁA�{�C���X�^���X���������������Ă�����Q�Ƃ���߂܂�
	 *		�i�Q�Ƃ��Ȃ��Ȃ�΃������͉������܂��j�B
	 *	@param bank �o���N
	 *	@param size �T�C�Y
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Allocate(BYTE bank, size_t size)
	{
		Free();
		TELL t = m_pMan->Allocate(bank, size + sizeof(LONG));//�J�E���^������
		if ( t == INVALID_TELL )
		{
			return false;
		}
		LONG* P = static_cast<LONG*>(m_pMan->Lock(t));
		if ( P == NULL )
		{
			m_pMan->Free(t);
			return false;
		}
		*P = 1;	//�J�E���^������
		m_pMan->Unlock(t);
		m_tell = t;
		return true;
	}

	/**
	 * [�擾] ���������
	 *	@note �{�C���X�^���X���������������Ă�����Q�Ƃ���߂܂�
	 *		�i�Q�Ƃ��Ȃ��Ȃ�΃������͉������܂��j�B
	 */
	void Free(void)
	{
		if ( m_tell == INVALID_TELL) { return; }
		LONG* B = static_cast<LONG*>(m_pMan->Lock(m_tell));
		if ( ::InterlockedDecrement(B) == 0 )
		{
			m_pMan->Unlock(m_tell);
			m_pMan->Free(m_tell);
		}
		else
		{
			m_pMan->Unlock(m_tell);
		}
		m_tell=INVALID_TELL;
	}

	/**
	 * [�擾] �m�ۃT�C�Y�擾
	 *	@note ���ۂɊm�ۗv���̃T�C�Y�ł͂Ȃ��A�Z�N�^�T�C�Y�̌��{��-4�Ƀp�f�B���O����܂��B
	 *	@retval 0		�m�ۂ��Ă��Ȃ��B
	 *	@retval 0�ȏ�	�m��Byte��
	 */
	size_t GetSize(void)
	{
		if ( m_tell == INVALID_TELL ){ return 0; }
		size_t l = m_pMan->GetSize(m_tell);
		if ( l != 0 )
		{
			l -= sizeof(LONG); 
		}
		return l;
	}

	/**
	 * [�擾] �A�h���X�擾�����b�N.
	 *	@note ���b�N�J�E���^���{�P���A�A�h���X��Ԃ��܂��B
	 *	@retval NULL �G���[�i�J�E���^�́{�P���܂���j
	 *	@retval NULL�ȊO �A�h���X
	 */
	LPVOID Lock(void)
	{
		if ( m_tell == INVALID_TELL ) { return NULL; }
		LONG* B = static_cast<LONG*>(m_pMan->Lock(m_tell));
		return &B[1];
	}

	/**
	 * [�擾] �A�����b�N
	 *	@note ���b�N�J�E���^���[�P���܂��B
	 */
	void Unlock(void)
	{
		if ( m_tell != INVALID_TELL )
		{
			m_pMan->Unlock(m_tell);
		}
	}

private:
	friend class CStaticMemoryTest;
};



};//TNB



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#if 0
	//m_pContent �̊m�ۓ��e
	[Index�e�[�u��][Flag�e�[�u��][���e]�Ƃ������сB
	//Index�e�[�u��
	���т�Index���B
	TELL�̉���WORD(Index)�ƃZ�N�^No���֘A�t����e�[�u���B�ŏ��BIT
	�������Ă�����g�p�i����15BIT��SecNo�j
	Resize�A�f�t���O���Ă����т͕ϊ����Ȃ��B
	// Flag�e�[�u�������e
	����������т�SecNo���B
	Flag�pDWORD���Z�N�^���m�ہB
	�Z�N�^�T�C�Y�̃��������Z�N�^���m�ہB
	Flag�p�́A���̋󂫃Z�N�^�ԍ���ێ��B�O�Ȃ�Ō���w���B
	�܂�N�����͂��ׂĂO�B
	�m�ۂ���ƁA�����̊m�ۂ����Z�N�^���ׂčŏ�ʃr�b�g�������A
	���̃u���b�N�̐擪�Z�N�^�ԍ�������B
	ex)
			 <0>    <1>    <2>    <3>    <4>    <5>    <6>
		�N����
			0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		2�u���b�N�m��
			0x8002,0x8002,0x0000,0x0000,0x0000,0x0000,0x0000,
		����ɂR�u���b�N�m��
			0x8002,0x8002,0x8005,0x8005,0x8005,0x0000,0x0000,
		�ŏ���2�u���b�N�J��
			0x0002,0x0002,0x8005,0x8005,0x8005,0x0000,0x0000,
		1�u���b�N�m��
			0x8001,0x0002,0x8005,0x8005,0x8005,0x0000,0x0000,

	�܂�`�F�[���Ǝg�p�t���O�ƃu���b�N�����킩��悤�ɂȂ�B
#endif
#endif
