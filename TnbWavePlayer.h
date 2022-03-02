#pragma once
/**
 *@file
 * WAVE�Đ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * WAVE�Đ��N���X
 *	
 *	@par�K�v�t�@�C��
 *			TnbWavePlayer.h
 *
 *	@date 08/08/12	�V�K�쐬
 */
class CWavePlayer : CCopyImpossible
{
public:
	/// �R���X�g���N�^
	CWavePlayer(void) : m_bankNumber(0), m_hWave(NULL), m_currentBank(0), m_pBuffers(NULL), m_pSyncBank(NULL), m_hasBuffer(false)
	{
	}

	/// �f�X�g���N�^
	~CWavePlayer(void)
	{
		Close();
	}

	/// �N���[�Y
	void Close(void)
	{
		if ( m_hWave != NULL )
		{
			::waveOutReset(m_hWave);
			::waveOutClose(m_hWave);
			m_hWave = NULL;
		}
		if ( m_pBuffers != NULL )
		{
			delete[] m_pBuffers;
			m_pBuffers = NULL;
		}
		if ( m_pSyncBank != NULL )
		{
			delete m_pSyncBank;
			m_pSyncBank = NULL;
		}
		m_bankNumber = 0;
		m_currentBank = 0;
	}

	/** 
	 * [����] �I�[�v��
	 *	@param format �t�H�[�}�b�g
	 *	@param bankNumber �o���N��
	 *	@param bankSize �e�o���N�T�C�Y
	 *	@retval MMSYSERR_NOERROR ����
	 *	@retval MMSYSERR_NOERROR�ȊO ���s
	 */
	MMRESULT Open(const WAVEFORMATEX& format, int bankNumber, UINT bankSize)
	{
		Close();
		m_bankNumber = bankNumber;
		m_pSyncBank = new CSyncSemaphore(NULL, bankNumber);
		MMRESULT mr = ::waveOutOpen(&m_hWave, WAVE_MAPPER, &format,
							reinterpret_cast<DWORD_PTR>(ms_WaveCallback),
							reinterpret_cast<DWORD_PTR>(m_pSyncBank), CALLBACK_FUNCTION);
		if ( mr == MMSYSERR_NOERROR )
		{
			m_pBuffers = new CBuffer[m_bankNumber];
			loop ( i, m_bankNumber )
			{
				mr = m_pBuffers[i].Init(m_hWave, bankSize);
				if ( mr != MMSYSERR_NOERROR )
				{
					break;
				}
			}
			m_hasBuffer = false;
		}
		if ( mr != MMSYSERR_NOERROR )
		{
			Close();
		}
		return mr;
	}

	/**
	 * [����] �Đ�
	 *	@note �o�b�t�@����t�Ȃ�΍Đ����܂����A��t�ɂȂ�Ȃ���΁A�Đ����܂���B
	 *	@note �o�b�t�@�C�ɋ󂫂��Ȃ���΁A�֐������^�[�����܂���B
	 *	@param[out] _isPlayed true �Ȃ�Đ��������s�A false�Ȃ�o�b�t�@�����O���������{�B
	 *	@param[in] pData �f�[�^
	 *	@param[in] size �T�C�Y
	 *	@retval MMSYSERR_NOERROR ����
	 *	@retval MMSYSERR_NOERROR�ȊO ���s
	 */
	MMRESULT Write(bool& _isPlayed, const BYTE* pData, size_t size)
	{
		if ( m_bankNumber == 0 ) { return MMSYSERR_ERROR; }
		size_t nWritten = 0;
		while ( size > 0 ) 
		{
			if ( ! m_hasBuffer )
			{
				m_pSyncBank->Lock();
				m_hasBuffer = true;
			}
			bool isPlayed;
			MMRESULT r = m_pBuffers[m_currentBank].Write(isPlayed, nWritten, pData, size);
			if ( r != MMSYSERR_NOERROR )
			{
				return r;
			}
			if ( isPlayed )
			{
				m_hasBuffer = false;
				m_currentBank = (m_currentBank + 1) % m_bankNumber;
			}
			size -= nWritten;
			pData += nWritten;
		}
		return MMSYSERR_NOERROR;
	}

	/**
	 * [����] �Đ�
	 *	@note �o�b�t�@�ɁA�f�[�^������΍Đ����A�o�b�t�@���N���A���܂��B
	 *	@retval MMSYSERR_NOERROR ����
	 *	@retval MMSYSERR_NOERROR�ȊO ���s
	 */
	MMRESULT Flush(void)
	{
		MMRESULT r = MMSYSERR_NOERROR;
		if ( m_bankNumber != 0 )
		{
			if ( m_hasBuffer )
			{
				r = m_pBuffers[m_currentBank].Flush();
				m_hasBuffer = false;
			}
			m_currentBank = (m_currentBank + 1) % m_bankNumber;
		}
		return r;
	}

	/**
	 * [�ݒ�] �N���A
	 */
	void Clear(void)
	{
		loop ( i, m_bankNumber )
		{
			m_pBuffers[i].Clear();
		}
		m_currentBank = 0;
	}
	
	/// [����] �ꎞ��~
	void Pause(void)
	{
		::waveOutPause(m_hWave);
	}

	/// [����] �ăX�^�[�g
	void Restart(void)
	{
		::waveOutRestart(m_hWave);
	}

	/// [�Đ�] ��~
	void Reset(void)
	{
		::waveOutReset(m_hWave);
	}

	/**
	 * [�ݒ�] �{�����[���ݒ�.
	 *	@note ���E�ʁX�ɐݒ�͏o���܂���B
	 *	@param vol �{�����[�� �O�`�P�Q�V
	 */
	void SetVolume(int vol)
	{
		DWORD	lvol = vol * 512;
		DWORD	rvol = vol * 512;
		::waveOutSetVolume(m_hWave, MAKELONG(lvol, rvol));
	}

	/**
	 * [�擾] �{�����[���擾.
	 *	@return �{�����[�� �O�`�P�Q�V.
	 */
	int GetVolume(void) const 
	{
		DWORD	vol;
		::waveOutGetVolume(m_hWave, &vol);
		return LOWORD(vol) / 512;
	}

private:
	/// �o�b�t�@�Ǘ��N���X
	class CBuffer
	{
		WAVEHDR		m_hdr;			///< WAVE���
		HWAVEOUT	m_hWave;		///< �n���h��
		size_t		m_sentBytes;	///< ���M�o�C�g��
	public:
		/// �R���X�g���N�^
		CBuffer(void) : m_hWave(NULL), m_sentBytes(0)
		{
			Zero(m_hdr);
		}
		/// �f�X�g���N�^
		~CBuffer(void)
		{
			if ( m_hdr.lpData != NULL )
			{
				::waveOutUnprepareHeader(m_hWave, &m_hdr, sizeof(WAVEHDR));
				::LocalFree(m_hdr.lpData);
				m_hdr.lpData = NULL;
			}
		}
		/**
		 * [�ݒ�] ������
		 *	@param hWave WAVE�n���h��
		 *	@param size �o�b�t�@�T�C�Y
		 *	@retval MMSYSERR_NOERROR ����
		 *	@retval MMSYSERR_NOERROR�ȊO ���s
		 */
		MMRESULT Init(HWAVEOUT hWave, UINT size)
		{
			m_hWave = hWave;
			m_sentBytes = 0;
			Zero(m_hdr);
			m_hdr.lpData = static_cast<LPSTR>(::LocalAlloc(LMEM_FIXED, size));
			if ( m_hdr.lpData == NULL )
			{
				return MMSYSERR_ERROR;
			}
			m_hdr.dwBufferLength = size;
			return ::waveOutPrepareHeader(hWave, &m_hdr, sizeof(WAVEHDR));
		}
		/// [�ݒ�] �t���b�V��
		MMRESULT Flush(void)
		{
			size_t s = m_hdr.dwBufferLength;
			m_hdr.dwBufferLength = m_sentBytes;
			MMRESULT r = ::waveOutWrite(m_hWave, &m_hdr, sizeof(WAVEHDR));
			m_sentBytes = 0;
			m_hdr.dwBufferLength = s;
			return r;
		}
		/// [�ݒ�]
		void Clear(void)
		{
			m_sentBytes = 0;
		}
		/**
		 * [����] �Đ�
		 *	@param[out] _isPlayed true �Ȃ�Đ��������s�A false�Ȃ�o�b�t�@�����O���������{�B
		 *	@param[out] _writtenSize �����݃T�C�Y(size �惊�������Ȃ�܂�)
		 *	@param[in] pData �f�[�^
		 *	@param[in] size �T�C�Y
		 *	@retval MMSYSERR_NOERROR ����
		 *	@retval MMSYSERR_NOERROR�ȊO ���s
		 */
		MMRESULT Write(bool& _isPlayed, size_t& _writtenSize, const BYTE* pData, size_t size)
		{
			_writtenSize = min(m_hdr.dwBufferLength - m_sentBytes, size);
			::CopyMemory(m_hdr.lpData + m_sentBytes, pData, _writtenSize);
			m_sentBytes += _writtenSize;
			if ( m_sentBytes == m_hdr.dwBufferLength )
			{
				m_sentBytes = 0;
				_isPlayed = true;
				return ::waveOutWrite(m_hWave, &m_hdr, sizeof(WAVEHDR));
			}
			_isPlayed = false;
			return MMSYSERR_NOERROR;
		}
	};
	/// Callback Function
	static void CALLBACK ms_WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
	{
		if ( uMsg == WOM_DONE )
		{
			CSyncSemaphore* P = reinterpret_cast<CSyncSemaphore*>(dwUser);
			P->Unlock();
		}
	}
	CSyncSemaphore*	m_pSyncBank;		///< �o���N�Ǘ��p�r��
	HWAVEOUT		m_hWave;			///< WAVE�n���h��
	int				m_bankNumber;		///< �o���N��
	int				m_currentBank;		///< ���݂̃o���N
	CBuffer*		m_pBuffers;			///< �o�b�t�@
	bool			m_hasBuffer;		///< �o�b�t�@����
};



}; //TNB
