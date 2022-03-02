#pragma once
/**
 *@file
 * MIDI�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"
#include "TnbMciPlayer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * MIDI���b�Z�[�W�N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbMidi.h
 *
 *	@date 10/05/25	�V�K�쐬
 */
class CMidiMessage : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CMidiMessage(void) : m_hMidi(NULL), m_lastError(MMSYSERR_NOERROR)
	{
	}

	/// �f�X�g���N�^
	~CMidiMessage(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �N���[�Y.
	 *	@note �I�[�v�����Ă��Ȃ����͉������܂���B
	 */
	void Close(void)
	{
		if ( m_hMidi != NULL )
		{
			::midiOutClose(m_hMidi);
			m_hMidi = NULL;
		}
	}

	/**
	 * [�ݒ�] �I�[�v��.
	 *	@param dev �f�o�C�X�ԍ��B 0�` GetDeviceCount() - 1 �͈̔͂��w��\�B
	 *				�ȗ�����ƃf�t�H���g���I������܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Open(UINT dev = MIDI_MAPPER)
	{
		Close();
		return m_Rst(::midiOutOpen(&m_hMidi, dev, NULL, 0, CALLBACK_NULL));
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	bool IsOpened(void) const
	{
		return m_hMidi != NULL;
	}

	/**
	 * [�ݒ�] �v���O�����`�F���W.
	 *		���F��ؑւ��܂��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param programNumber ���F�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool ProgramChange(BYTE channel, BYTE programNumber)
	{
		return m_Rst(channel, TParam(Midi_ProgramChange, channel, programNumber));
	}

	/**
	 * [�ݒ�] �o���N�Z���N�g.
	 *		���F�̃o���N��ؑւ��܂��B
	 *	@note �{���\�b�h�����ł͉��F�͕ς��܂���B�{���\�b�h�� ProgramChange() ���g�p���܂��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param msb MSB.
	 *	@param lsb LSB.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool BankSelect(BYTE channel, BYTE msb, BYTE lsb)
	{
		CWorkMemT<TParam> vp(2);
		vp[0] = TParam(CCC_BackSelectMsb, channel, msb);
		vp[1] = TParam(CCC_BackSelectLsb, channel, lsb);
		return m_Rst(channel, vp);
	}

	/**
	 * [�ݒ�] ���W�����[�V����.
	 *		���Ƀr�u���[�g��������
	 *	@param channel �`���l�� 0�`15�B
	 *	@param modulation �r�u���[�g�̋����B�O���Ȃ��B127���ő�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Modulation(BYTE channel, BYTE modulation)
	{
		ASSERT ( modulation <= 127 );
		return m_Rst(channel, TParam(CCC_Modulation, channel, modulation));
	}

	/**
	 * [�ݒ�] �{�����[��.
	 *		���ʂ𒲐߂���B
	 *	@note ����ő���ȃ`�����l���̉��ʐ���p�Ɏg���܂��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param volume ���ʁB 0 �������B 127 ���ő�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Volume(BYTE channel, int volume)
	{
		ASSERT ( volume >= 0 && volume <= 127 );
		return m_Rst(channel, TParam(CCC_Volume, channel, down_cast<BYTE>(volume)));
	}

	/**
	 * [�ݒ�] �p���|�b�g.
	 *		���̏o�͐�����E�Ɋ���U��܂��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param pan 0 �������B���[�� -63�B�E�[�� 63�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Panpot(BYTE channel, int pan)
	{
		if ( pan < -63 )
		{
			pan = -63;
		}
		else if ( pan > 63 )
		{
			pan = 63;
		}
		return m_Rst(channel, TParam(CCC_Panpot, channel, down_cast<BYTE>(pan + 64)));
	}

	/**
	 * [�ݒ�] �G�N�X�v���b�V����.
	 *		���ʂ𒲐߂���B
	 *	@note �������ɕω������邱�Ƃ��o����B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param volume ���ʁB 0 �������B 127 ���ő�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Expression(BYTE channel, int volume)
	{
		ASSERT ( volume >= 0 && volume <= 127 );
		return m_Rst(channel, TParam(CCC_Expression, channel, down_cast<BYTE>(volume)));
	}

	/**
	 * [�ݒ�] ���o�[�u�Z���h���x��.
	 *		�������x�����w�肷��B
	 *	@note �����ɂ���ẮA�@�\���Ȃ��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param level ���x���B 0 �����ʂȂ��B 127 �����ʍő�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool ReverbSendLevel(BYTE channel, int level)
	{
		ASSERT ( level >= 0 && level <= 127 );
		return m_Rst(channel, TParam(CCC_ReverbSendLevel, channel, down_cast<BYTE>(level)));
	}

	/**
	 * [�ݒ�] �R�[���X�Z���h���x��.
	 *		�R�[���X���x�����w�肷��B
	 *	@note �����ɂ���ẮA�@�\���Ȃ��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param level ���x���B 0 �����ʂȂ��B 127 �����ʍő�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool ChorusSendLevel(BYTE channel, int level)
	{
		ASSERT ( level >= 0 && level <= 127 );
		return m_Rst(channel, TParam(CCC_ChorusSendLevel, channel, down_cast<BYTE>(level)));
	}

	/**
	 * [�ݒ�] �R���g���[���Z���h���x��.
	 *		�w��̃R���g���[���ԍ��̃��x�����w�肷��B
	 *	@note �����ɂ���ẮA�@�\���Ȃ��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param controlNo �R���g���[���ԍ��B�Ⴆ�΃��o�[�u�� 0x5B, �R�[���X�Ȃ� 0x5D ���w�肷��B
	 *	@param level ���x���B 0 �����ʂȂ��B 127 �����ʍő�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool ControlSendLevel(BYTE channel, BYTE controlNo, int level)
	{
		ASSERT ( level >= 0 && level <= 127 );
		return m_Rst(channel, TParam(ECtrlChangeCode(controlNo), channel, down_cast<BYTE>(level)));
	}

	/**
	 * [�ݒ�] �m�[�g�I��.
	 *		���Ղ���������ł��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param note �m�[�g�ԍ� 0�`127�B���K�B�s�A�m�̒����̃h�� 60�B
	 *	@param velocity �������� 0�`127�B\n�l�̖ڈ�\n
	 *		ppp=16, pp=32, p=48, mp=64, mf=80, f=96, ff=112, fff=127
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool NoteOn(BYTE channel, BYTE note, BYTE velocity = 0)
	{
		ASSERT ( velocity <= 127 );
		return m_Rst(channel, TParam(Midi_NoteOn, channel, note, velocity));
	}

	/**
	 * [�ݒ�] �m�[�g�I�t.
	 *		���Ղ𗣂�����ł��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param note �m�[�g�ԍ� 0�`127�B���K�B�s�A�m�̒����̃h�� 60�B
	 *	@param velocity �������� 0�`127�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool NoteOff(BYTE channel, BYTE note, BYTE velocity = 0)
	{
		ASSERT ( velocity <= 127 );
		return m_Rst(channel, TParam(Midi_NoteOff, channel, note));
	}

	/**
	 * [�ݒ�] �s�b�`�x���h�`�F���W.
	 *		���Ղ𗣂�����ł��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param pitchBend �s�b�`�x���h�B -8192 �` 8181 �B0���Z���^�[�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool PitchBendChange(BYTE channel, int pitchBend = 0)
	{
		if ( pitchBend < -8192 )
		{
			pitchBend = -8192;
		}
		else if ( pitchBend > 8181 )
		{
			pitchBend = 8181;
		}
		int b = pitchBend + 8192;
		BYTE msb = down_cast<BYTE>((b >> 7) & 0x7F);
		BYTE lsb = down_cast<BYTE>((b     ) & 0x7F);
		return m_Rst(channel, TParam(Midi_PitchBendChange, channel, lsb, msb));
	}

	/**
	 * [�ݒ�] �s�b�`�x���h�Z���V�r�e�B.
	 *		�s�b�`�x���h�͈̔͂�ݒ肵�܂��B
	 *	@param channel �`���l�� 0�`15�B
	 *	@param v �͈́B 12 �łP�I�N�^�[�u�B�l�̍ő�� 127 �B�������ʏ�A�ő�l�� 24�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool PitchBendSensitivity(BYTE channel, BYTE v)
	{
		ASSERT ( v <= 127 );
		CWorkMemT<TParam> vp(6);
		// �s�b�`�x���h�Z���V�r�e�B
		vp[0] = TParam(CCC_RpnMsb, channel, 0);
		vp[1] = TParam(CCC_RpnLsb, channel, 0);
		// �f�[�^�G���g��
		vp[2] = TParam(CCC_DataEntryMsb, channel, v);
		vp[3] = TParam(CCC_DataEntryLsb, channel, 0);
		//// �I�[
		vp[4] = TParam(CCC_RpnMsb, channel, 0x7F);
		vp[5] = TParam(CCC_RpnLsb, channel, 0x7F);
		return m_Rst(channel, vp);
	}

#if 0
	bool OutMessage(size_t len, LPCVOID pMessage)
	{
		MIDIHDR hdr = { 0 };
		CWorkMemT<char> mm;
		mm.Reset(len, static_cast<const char*>(pMessage));
		hdr.lpData = mm;
		hdr.dwBufferLength = len;
		hdr.dwBytesRecorded = len;
		if ( ! m_Rst(::midiOutPrepareHeader(m_hMidi, &hdr, sizeof(MIDIHDR))) )
		{
			return false;
		}
		if ( ! m_Rst(::midiOutLongMsg(m_hMidi, &hdr, sizeof(MIDIHDR))) )
		{
			return false;
		}
		/* GM���Z�b�g�����ҋ@ */
		while ( (hdr.dwFlags & MHDR_DONE) == 0 )
		{
			::Sleep(0);
		}
		return m_Rst(::midiOutUnprepareHeader(m_hMidi, &hdr, sizeof(MIDIHDR)));
	}
#endif

	/**
	 * [�擾] ���X�g�G���[�R�[�h�擾.
	 *	@return �G���[�R�[�h. GetErrorString() �ŕ�����ɕϊ����邱�Ƃ��\�ł��B
	 */
	DWORD GetLastError(void) const
	{
		return m_lastError;
	}

	/**
	 * [�ݒ�] ���X�g�G���[�R�[�h�ݒ�.
	 *	@param e �G���[�R�[�h. 
	 */
	void SetLastError(DWORD e)
	{
		m_lastError = e;
	}

	/**
	 * [�擾] �f�o�C�X���擾.
	 *		�ڑ����Ă��� MIDI�̃f�o�C�X����Ԃ��܂��B
	 *	@return �f�o�C�X���B
	 */
	static UINT GetDeviceCount(void)
	{
		return ::midiOutGetNumDevs();
	}

	/**
	 * [�擾] �G���[������擾.
	 *	@param e �G���[�R�[�h. 
	 *	@return ������B
	 */
	static CStr GetErrorString(DWORD e)
	{
		CStr s;
		if ( e != MMSYSERR_NOERROR )
		{
			::midiOutGetErrorText(e, s.GetBuffer(1000), 1000);
			s.ReleaseBuffer();
		}
		return s;
	}

private:

	/// ���b�Z�[�W��
	enum EMidiMessageKind
	{
		Midi_NoteOn				= 0x9,	///< �m�[�g�I��, d1=Note#, d2=Velocity
		Midi_NoteOff			= 0x8,	///< �m�[�g�I�t, d1=Note#, d2=Verocity
		Midi_KeyPressure		= 0xA,	///< �|���t�H�j�b�N�L�[�v���b�V���[, d1=Note#, d2=�v���b�V���[�l
		Midi_CtrlChange			= 0xB,	///< �R���g���[���`�F���W
		Midi_ProgramChange		= 0xC,	///< �v���O�����ύX, d1=Program#, d2=���g�p
		Midi_ChannelPressure	= 0xD,	///< �`���l���v���b�V���[, d1=�v���b�V���[�l, d2=���g�p
		Midi_PitchBendChange	= 0xE,	///< �s�b�`�x���h, d1=�x���h�l(LSM), d2=��(MSB)
	};

	/// �R���g���[���`�F���W�R�[�h
	enum ECtrlChangeCode
	{
		CCC_BackSelectMsb		= 0x00,	///< �o���N�Z���N�g(MSB)
		CCC_BackSelectLsb		= 0x20,	///< �o���N�Z���N�g(LSB)
		CCC_Modulation			= 0x01,	///< ���W�����[�V����
		CCC_Volume				= 0x07,	///< �{�����[��
		CCC_Panpot				= 0x0A,	///< �p���|�b�g
		CCC_Expression			= 0x0B,	///< �G�N�X�v���b�V����
		CCC_DataEntryMsb		= 0x06,	///< �f�[�^�G���g��(MSB)
		CCC_DataEntryLsb		= 0x26,	///< �f�[�^�G���g��(LSB)
		CCC_RpnMsb				= 0x65,	///< RegisteredParameterNumber(MSB)
		CCC_RpnLsb				= 0x64,	///< RegisteredParameterNumber(LSB)
		CCC_ReverbSendLevel		= 0x5B,	///< ���o�[�u�Z���h���x��
		CCC_TremoloDepth		= 0x5C,	///< �g����������
		CCC_ChorusSendLevel		= 0x5D,	///< �R�[���X�Z���h���x��
		CCC_VarSendLevel		= 0x5E,	///< �o���G�[�V�����Z���h���x��(SC88�ȍ~�̓f�B���C)
	};

	/// ���b�Z�[�W�p�����[�^
	struct TParam
	{
		union {
			struct {
				union {
					struct {
						BYTE	channel:4;	///< �`���l��
						BYTE	kind:4;		///< ���
					};
					BYTE	state;			///< �X�e�[�^�X�o�C�g
				};
				BYTE	data1;				///< �f�[�^�o�C�g�P
				BYTE	data2;				///< �f�[�^�o�C�g�Q
				BYTE	padding;			///< (��)
			};
			DWORD msg;						///< ���b�Z�[�W
		};
		TParam(EMidiMessageKind k, BYTE ch = 0, BYTE d1 = 0, BYTE d2 = 0)
			: padding(0), kind(down_cast<BYTE>(k)), channel(ch), data1(d1), data2(d2)
		{
		}
		TParam(ECtrlChangeCode ccc, BYTE ch = 0, BYTE d = 0)
			: padding(0), kind(down_cast<BYTE>(Midi_CtrlChange)), channel(ch), data1(down_cast<BYTE>(ccc)), data2(d)
		{
		}
		TParam(void) : msg(0)
		{
		}
	};
	friend struct CMidiMessage::TParam;
	C_ASSERT( sizeof(TParam) == sizeof(DWORD) );
	///
	/// �߂�l����
	bool m_RstFx(MMRESULT r)
	{
		m_lastError = r;
		return r == MMSYSERR_NOERROR;
	}
	/// �߂�l����
	bool m_Rst(MMRESULT r)
	{
		m_lastError = r;
		if ( r != MMSYSERR_NOERROR )
		{
			TRACE2("MMRESULT = %d[%s]\n", r, GetErrorString(r));
		}
		return r == MMSYSERR_NOERROR;
	}
	/// ���b�Z�[�W���M
	bool m_Rst(BYTE channel, const TParam& p)
	{
		if ( channel > 15 )
		{
			return m_RstFx(MMSYSERR_BADERRNUM);
		}
		TTRACE3( "MIDI msg = %02X %02X %02X\n", p.state, p.data1, p.data2 );
		return m_Rst(::midiOutShortMsg(m_hMidi, p.msg));
	}
	/// ���b�Z�[�W���M
	bool m_Rst(BYTE channel, const CWorkMemT<TParam>& vp)
	{
		loop ( i, vp.GetSize() )
		{
			if ( ! m_Rst(channel, vp[i]) )
			{
				return false;
			}
		}
		return true;
	}
	HMIDIOUT			m_hMidi;		///< �n���h��
	mutable MMRESULT	m_lastError;	///< �ŏI�G���[
};



}; //TNB