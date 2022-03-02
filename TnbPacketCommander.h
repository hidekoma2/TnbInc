#pragma once
/**
 *@file
 * �p�P�b�g�R�}���h�ʐM�֌W�̃w�b�_
 *
 *		�V�[�P���V�����f�[�^�̐��������ƁA�V�[�P���V�����f�[�^�̉�͏������������邱�ƂŁA
 *		�����R�}���h�ʐM���������܂��B
 *
 *	@note	�}�N�� "_TnbPACKET_LittleEndian" ����`����Ă���Ȃ�A
 *			TPacDword , TPacWord �̃������C���[�W���u���g���G���f�B�A���v�Ƃ��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"
#include "TnbCollectionAdapter.h"
#include "TnbQueueingReporter.h"
#include "TnbSet.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
  #ifdef _TnbPACKET_LittleEndian
	#pragma message("message : TPacWord/TPacDword is Little Endian.")
  #else
	#pragma message("message : TPacWord/TPacDword is Big Endian.")
  #endif
#endif



namespace TNB 
{



#pragma pack(push)
#pragma pack(1)



/**@ingroup REPORT
 * �p�P�b�g�p DWORD �ϐ��^.
 *		�r�b�O�G���f�B�A���̃������C���[�W�� DWORD ��ێ����܂��B
 *		�������}�N�� "_TnbPACKET_LittleEndian" ����`����Ă���Ȃ�A
 *		���g���G���f�B�A���ŕێ����܂��B
 *		�A�N�Z�T�ŁA�G���f�B�A���ϊ����T�|�[�g���܂��̂ŁA�ӎ�����K�v�͂���܂���B
 *	@note union �΍�ŃR���X�g���N�^�������Ă��܂���B
 *	@note �����ł� BYTE �̔z��Ȃ̂ŁA��A�h���X�ɔz�u����Ă���肠��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	�V�K�쐬
 *	@date 11/06/23	�����o���B���ɂ��ABYTE�̔z��ɂ����B
 *	@date 12/02/14	Set() �ǉ��B
 */
struct TPacDword
{
	typedef DWORD TYPE;		///< �^��

	/**
	 * [���] ���.
	 *	@param v �ݒ肷��l
	 *	@return �����̎Q��.
	 */
	TPacDword& operator=(DWORD v)
	{
		Set(v);
		return *this;
	}

	/**
	 * [���] ���.
	 *	@param v �ݒ肷��l
	 */
	void Set(DWORD v)
	{
		#ifdef _TnbPACKET_LittleEndian
			m_dat[3] = static_cast<BYTE>(v >> 24);
			m_dat[2] = static_cast<BYTE>(v >> 16);
			m_dat[1] = static_cast<BYTE>(v >> 8);
			m_dat[0] = static_cast<BYTE>(v);
		#else
			m_dat[0] = static_cast<BYTE>(v >> 24);
			m_dat[1] = static_cast<BYTE>(v >> 16);
			m_dat[2] = static_cast<BYTE>(v >> 8);
			m_dat[3] = static_cast<BYTE>(v);
		#endif
	}

	/**
	 * [�擾] �Q�b�^
	 *	@return �ێ����Ă���l
	 */
	operator DWORD(void) const
	{
		return Get();
	}

	/**
	 * [�擾] �Q�b�^
	 *	@return �ێ����Ă���l
	 */
	DWORD Get(void) const
	{
		DWORD r;
		#ifdef _TnbPACKET_LittleEndian
			r = (m_dat[3] << 24) + (m_dat[2] << 16) + (m_dat[1] << 8) + (m_dat[0]);
		#else
			r = (m_dat[0] << 24) + (m_dat[1] << 16) + (m_dat[2] << 8) + (m_dat[3]);
		#endif
		return r;
	}

	/**
	 * [�擾] ������.
	 *	@return ������
	 */
	CStr ToString(void) const
	{
		return CStr::DwordToString(Get());
	}

private:
	BYTE m_dat[4];				///< �l
};



/**@ingroup REPORT
 * �p�P�b�g�p WORD �ϐ��^.
 *		�r�b�O�G���f�B�A���̃������C���[�W�� WORD ��ێ����܂��B
 *		�������}�N�� "_TnbPACKET_LittleEndian" ����`����Ă���Ȃ�A
 *		���g���G���f�B�A���ŕێ����܂��B
 *		�A�N�Z�T�ŁA�G���f�B�A���ϊ����T�|�[�g���܂��̂ŁA�ӎ�����K�v�͂���܂���B
 *	@note union �΍�ŃR���X�g���N�^�������Ă��܂���B
 *	@note �����ł� BYTE �̔z��Ȃ̂ŁA��A�h���X�ɔz�u����Ă���肠��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	�V�K�쐬
 *	@date 11/06/23	�����o���B���ɂ��ABYTE�̔z��ɂ����B
 *	@date 12/02/14	Set() �ǉ��B
 */
struct TPacWord
{
	typedef WORD TYPE;		///< �^��

	/**
	 * [���] ���.
	 *	@param v �ݒ肷��l
	 *	@return �����̎Q��.
	 */
	TPacWord& operator=(WORD v)
	{
		Set(v);
		return *this;
	}

	/**
	 * [���] ���.
	 *	@param v �ݒ肷��l
	 */
	void Set(WORD v)
	{
		#ifdef _TnbPACKET_LittleEndian
			m_dat[1] = static_cast<BYTE>(v >> 8);
			m_dat[0] = static_cast<BYTE>(v);
		#else
			m_dat[0] = static_cast<BYTE>(v >> 8);
			m_dat[1] = static_cast<BYTE>(v);
		#endif
	}

	/**
	 * [�擾] �Q�b�^
	 *	@return �ێ����Ă���l
	 */
	operator WORD(void) const
	{
		return Get();
	}

	/**
	 * [�擾] �Q�b�^
	 *	@return �ێ����Ă���l
	 */
	WORD Get(void) const
	{
		WORD r;
		#ifdef _TnbPACKET_LittleEndian
			r = static_cast<WORD>((m_dat[1] << 8) + (m_dat[0]));
		#else
			r = static_cast<WORD>((m_dat[0] << 8) + (m_dat[1]));
		#endif
		return r;
	}

	/**
	 * [�擾] ������.
	 *	@return ������
	 */
	CStr ToString(void) const
	{
		return CStr::DwordToString(Get());
	}

private:
	BYTE m_dat[2];				///< �l
};



/**@ingroup REPORT
 * �p�P�b�g�p BYTE �ϐ��^.
 *		TPacWord , TPacDword �Ɛe�a�����������邽�߂̌^�ł��B
 *	@note union �΍�ŃR���X�g���N�^�������Ă��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	�V�K�쐬
 *	@date 11/06/23	�����o���B���ɂ����B
 *	@date 12/02/14	Set() �ǉ��B
 */
struct TPacByte
{
	typedef BYTE TYPE;		///< �^��

	/**
	 * [���] ���.
	 *	@param v �ݒ肷��l
	 *	@return �����̎Q��.
	 */
	TPacByte& operator=(BYTE v)
	{
		m_dat = v;
		return *this;
	}

	/**
	 * [���] ���.
	 *	@param v �ݒ肷��l
	 */
	void Set(BYTE v)
	{
		m_dat = v;
	}

	/**
	 * [�擾] �Q�b�^
	 *	@return �ێ����Ă���l
	 */
	operator BYTE(void) const
	{
		return Get();
	}

	/**
	 * [�擾] �Q�b�^
	 *	@return �ێ����Ă���l
	 */
	BYTE Get(void) const
	{
		return m_dat;
	}

	/**
	 * [�擾] ������.
	 *	@return ������
	 */
	CStr ToString(void) const
	{
		return CStr::DwordToString(Get());
	}

	/**
	 * [�Q��] �Q��
	 *	@return �ێ����Ă���l�̎Q��
	 */
	BYTE& Ref(void)
	{
		return m_dat;
	}

private:
	BYTE m_dat;			///< �l
};



#pragma pack(pop)



/**@ingroup REPORT
 * �p�P�b�g�R�}���h���s�C���^�[�t�F�[�X.
 *		�R�}���h���M�`�����҂��܂ł��T�|�[�g����C���^�[�t�F�[�X�ł��B
 *		�^�C���A�E�g���Ԃ́A ExecuteCommand() ���\�b�h�Ŏw��\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPacketCommander.h
 * 
 *	@date 11/02/03	�V�K�A�C���^�[�t�F�[�X��
 *	@date 12/02/29	SendCommand() -> SendPacket() 
 */
struct IPacketCommandExecuter
{
	/**@ingroup REPORT
	 * �p�P�b�g�C���^�[�t�F�[�X.
	 *		�V�[�P���V�����f�[�^���쐬������A��͂�����A�����񉻂��郁�\�b�h�������܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPacketCommander.h
	 * 
	 *	@date 10/08/24	�V�K�쐬.
	 *	@date 12/03/22	GetPacket() -> Serialize(), SetPacket() -> Deserialize() �ɕύX�B
	 */
	struct IPacket
	{
		/// �f�X�g���N�^
		virtual ~IPacket(void) {}

		/**
		 * [�擾] �V���A���C�Y.
		 *		�{�C���X�^���X�ŕێ����Ă����񂩂�A�V�[�P���V�����f�[�^���쐬���܂��B
		 *	@return �V�[�P���V�����f�[�^.
		 */
		virtual CByteVector Serialize(void) const = 0;

		/**
		 * [�ݒ�] �f�V���A���C�Y.
		 *		�w��̃V�[�P���V�����f�[�^����͂��A�p�P�b�g�����o���{�C���X�^���X�Ɋi�[���܂��B
		 *	@param c �f�[�^
		 *	@retval 0				�p�P�b�g���f�ł��邾���̒������Ȃ�
		 *	@retval	0����			�p�P�b�g��̓G���[�B
		 *	@retval 1�ȏ�			�p�P�b�g��͐����B�l�͉�͂Ɏg�p�����f�[�^�����B
		 */
		virtual int Deserialize(const IConstCollectionT<BYTE>& c) = 0;

		/**
		 * [�擾] ������.
		 *		�R�}���h���A�p�����[�^�𕶎��񉻂��܂��B
		 *	@return ������B�C�ӂł����A�ʏ�[0]�̓R�}���h���B�ȍ~�̓p�����[�^�Ƃ��܂��B
		 */
		virtual CStrVector ToStrings(void) const = 0;
	};

	/// �f�X�g���N�^
	virtual ~IPacketCommandExecuter(void) {}

	/**
	 * [���s] �p�P�b�g���M.
	 *	@param[in] packet �p�P�b�g�B
	 *	@retval 0�ȏ� �����B
	 *	@retval 0���� �G���[�B
	 */
	virtual int SendPacket(const IPacket& packet) = 0;

	/**
	 * [���s] �R�}���h���s
	 *	@note ���������邩�A�^�C���A�E�g����܂Ŋ֐��̓��^�[�����܂���B
	 *	@param[out] _response ���X�|���X�^�B�������A���ʂ��i�[����܂��B
	 *	@param[in] command �R�}���h�^�B
	 *	@param[in] timeout ���X�|���X�̃^�C���A�E�g����(ms).
	 *	@retval 0�ȏ� �����B
	 *	@retval 0���� ���̑��̃G���[(��� IPacket::Deserialize() �̖߂�l)�B
	 */
	virtual int ExecuteCommand(IPacket& _response, const IPacket& command, DWORD timeout = 500) = 0;
};



/**@ingroup REPORT
 * �p�P�b�g�R�}���_�[.
 *		�R�}���h���M�`�����҂��܂ł��T�|�[�g���Ă��܂��B
 *		�^�C���A�E�g���Ԃ́A ExecuteCommand() ���\�b�h�Ŏw��\�ł��B
 *	
 *	- �܂��A�p�P�b�g�̌`�� IPacketCommandExecuter::IPacket ���������č쐬���܂�(ex;CBasicPacket)�B
 *		����́A STX�`ETX ����s��؂�ȂǁABYTE �����͂���@�\�ƃp�P�b�g���쐬����@�\��
 *		�����̂ł��B
 *		\code
 *			//
 *			// �p�P�b�g�t�H�[�}�b�g�^.
 *			//	
 *			//		�p�P�b�g�̌`�݂̂��Ǘ�����B
 *			//		@note STX[1], LEN[1], CMD[1], DAT[LEN-2], BCC[1], ETX[1]\n
 *			//			- BCC��LEN�`DAT�̍��v
 *			//			�R�}���h�R�[�h��A�f�[�^�̒��g�A�����́A�`�F�b�N���Ȃ��B
 *			//
 *			class CPacketFormat : public TNB::CPacketCommander::IPacket
 *			{
 *			public:
 *				// SetData() �G���[�R�[�h
 *				enum
 *				{
 *					NotFoundStx			= -100,		// STX ��������Ȃ�
 *					NotFoundEtx			= -101,		// ETX ��������Ȃ�
 *					InvalidBcc			= -103,		// BCC�ُ�
 *					InvalidParameter	= -104,		// �p�����[�^�ُ�
 *				};
 *				//
 *				// �R���X�g���N�^.
 *				//	@param cc �R�}���h�R�[�h�B�ȗ�����ƂO�B
 *				//
 *				CPacketFormat(BYTE cc = 0) : m_command(cc);
 *				//
 *				// [�ݒ�] �R�}���h�R�[�h�ݒ�
 *				//	@param cc �R�}���h�R�[�h
 *				//
 *				void SetCommand(BYTE cc);
 *				//
 *				// [�擾] �R�}���h�R�[�h�擾
 *				//	@return �R�}���h�R�[�h
 *				//
 *				BYTE GetCommand(void) const;
 *				//
 *				// [�擾] �f�[�^�擾.
 *				//	@return �f�[�^
 *				//
 *				const CByteVector& GetData(void) const;
 *				//
 *				// [�ݒ�] �f�[�^�ݒ�.
 *				//	@param vb �f�[�^
 *				//	@retval 0			����
 *				//	@retval	�}�C�i�X	�G���[�B
 *				//
 *				int SetDataV(const CByteVector& vb);
 *				//
 *				// [�ݒ�] �f�[�^�ݒ�.
 *				//	@param len �f�[�^��
 *				//	@param P �f�[�^
 *				//	@retval 0			����
 *				//	@retval	�}�C�i�X	�G���[�B
 *				//
 *				virtual int SetData(size_t len, LPCVOID P);
 *				//
 *				// [�擾] �V���A���C�Y.
 *				//		�{�C���X�^���X�ŕێ����Ă����񂩂�A�V�[�P���V�����f�[�^���쐬���܂��B
 * 				//	@return �V�[�P���V�����f�[�^.
 *				//
 *				virtual CByteVector Serialize(void) const;
 *				//
 *				// [�ݒ�] �f�V���A���C�Y.
 *				//		�w��̃V�[�P���V�����f�[�^����͂��A�p�P�b�g�����o���{�C���X�^���X�Ɋi�[���܂��B
 *				//	@param c �f�[�^
 *				//	@retval 0				�p�P�b�g���f�ł��邾���̒������Ȃ�
 *				//	@retval	0����			�p�P�b�g��̓G���[�B
 *				//	@retval 1�ȏ�			�p�P�b�g��͐����B�l�͉�͂Ɏg�p�����f�[�^�����B
 *				//
 *				virtual int Deserialize(const IConstCollectionT<BYTE>& c);
 *				//
 *				// [�擾] ������.
 *				//		�R�}���h���A�p�����[�^�𕶎��񉻂��܂��B
 *				//	@return ������B�C�ӂł����A�ʏ�[0]�̓R�}���h���B�ȍ~�̓p�����[�^�Ƃ��܂��B
 *				//
 *				virtual CStrVector ToStrings(void) const;
 *			protected:
 *				enum
 *				{
 *					STX = 0x02,		// STX
 *					ETX = 0x03,		// ETX
 *				};
 *				BYTE		m_command;			// �R�}���h
 *				CByteVector	m_data;				// �f�[�^
 *				friend class CPacketTest;
 *			}; 
 *		\endcode
 *	- ����̃C���X�^���X�� SetPacketFormat() �ɓn���܂��B
 *	- �X�� IPacket �����������X�̃R�}���h���ƂɊǗ��p�P�b�g�N���X��
 *		�쐬���܂�(ex; CVersionCommand )�B
 *		���̃N���X�́A���߂�ꂽ�R�}���h�R�[�h�ȂǁA�������Ȃ���΁A
 *		��͂ŃG���[��Ԃ��悤�ɂ��܂��B
 *		\n�ȉ��͌X�̃R�}���h�̒��ۃN���X��B�K�v�ɉ����Čp���� AnalyzeData ���I�[�o�[���[�h����B
 *		\code
 *			//
 *			// ���ۃp�P�b�g�Ǘ��N���X.
 *			//
 *			class CAbstractPacket : public CPacketFormat
 *			{
 *				DEFSUPER(CPacketFormat);
 *			public:
 *				// SetData() �G���[�R�[�h
 *				enum
 *				{
 *					UnknownCommandCode	= -201,		// �R�}���h�R�[�h�s��
 *				};
 *				//
 *				// [�ݒ�] �p�P�b�g�ݒ�.
 *				//	@param bp �p�P�b�g���
 *				//	@retval 0			����
 *				//	@retval	�}�C�i�X	�p�P�b�g��̓G���[�B
 *				//
 *				int SetPacket(const CBasicPacket& bp);
 *				//
 *				// [�ݒ�] �f�V���A���C�Y.
 *				//		�w��̃V�[�P���V�����f�[�^����͂��A�p�P�b�g�����o���{�C���X�^���X�Ɋi�[���܂��B
 *				//	@param c �f�[�^
 *				//	@retval 0				�p�P�b�g���f�ł��邾���̒������Ȃ�
 *				//	@retval	0����			�p�P�b�g��̓G���[�B
 *				//	@retval 1�ȏ�			�p�P�b�g��͐����B�l�͉�͂Ɏg�p�����f�[�^�����B
 *				//
 *				virtual int Deserialize(const IConstCollectionT<BYTE>& c);
 *				//
 *				// [�ݒ�] �f�[�^�ݒ�.
 *				//	@param len �f�[�^��
 *				//	@param P �f�[�^
 *				//	@retval 0				�f�[�^����͐���
 *				//	@retval	�}�C�i�X		�f�[�^����̓G���[�B
 *				//
 *				virtual int SetData(size_t len, LPCVOID P);
 *				//
 *				// [�擾] ������.
 *				//		�R�}���h���A�p�����[�^�𕶎��񉻂��܂��B
 *				//	@return ������B�C�ӂł����A�ʏ�[0]�̓R�}���h���B�ȍ~�̓p�����[�^�Ƃ��܂��B
 *				//
 *				virtual CStrVector ToStrings(void) const;
 *				//
 *				// [�擾] �R�}���h���擾.
 *				//	@return �R�}���h��.
 *				//
 *				CStr GetCommandName(void) const;
 *				//
 *				// [�擾] ���U���g�l�擾.
 *				//	@return ���U���g�l.
 *				//
 *				virtual BYTE GetResult(void) const;
 *			protected:
 *				//
 *				// ������.
 *				//	@param cc �R�}���h�R�[�h�B
 *				//	@param lpszName �R�}���h��.
 *				//
 *				void Init(BYTE cc, LPCTSTR lpszName);
 *				CStr	m_commandName;	// �R�}���h��
 *			private:
 *				void SetCommand(BYTE cc);
 *			};
 *
 *
 *			// �o�[�W�����m�F�R�}���h
 *			class CVersionCommand : public CAbstractPacket
 *			{
 *			public:
 *				enum { CC = 0x2A };		// �R�}���h�R�[�h
 *				// �R���X�g���N�^
 *				CVersionCommand(void)
 *				{
 *					Init(CC, _T("�o�[�W�����m�F�R�}���h"));
 *				}
 *			};
 *
 *
 *			// �o�[�W�����m�F���X�|���X�f�[�^
 *			struct TVersionData
 *			{
 *				TPacByte	result;			// 0x00; ���� 0x01; �p�����[�^�G���[
 *				char		name[13];		// ���O
 *				TPacByte	version[4];		// �o�[�W����
 *				//
 *				// [�擾] ������
 *				//	@return ������
 *				//
 *				CStrVector ToStrings(void) const;
 *			};
 *
 *			/// �o�[�W�����m�F���X�|���X
 *			class CVersionResponse : public CAbstractParamPacketT<TVersionData>
 *			{
 *			public:
 *				enum { CC = 0xAA };		// �R�}���h�R�[�h
 *				// �R���X�g���N�^
 *				CVersionResponse(void)
 *				{
 *					Init(CC, _T("�o�[�W�����m�F���X�|���X"));
 *				}
 *				//
 *				// [�ݒ�] �p�����[�^�ݒ�.
 *				//	@param prm �p�����[�^.
 *				//
 *				virtual void SetParameter(const TParam& prm);
 *				{
 *					m_param = prm;
 *					int cnt = 1;
 *					if ( m_param.result == 0 )
 *					{
 *						cnt = sizeof(TFwVersionData);
 *					}
 *					m_data.SetElements(cnt , reinterpret_cast<const BYTE*>(&m_param));
 *				}
 *				// ���U���g�擾
 *				virtual BYTE GetResult(void) const
 *				{
 *					return GetParameter().result;
 *				}
 *			protected:
 *				//
 *				// [���] �f�[�^�����.
 *				//	@param len �f�[�^����
 *				//	@param P �f�[�^.
 *				//	@retval 0				�f�[�^����͐���
 *				//	@retval	�}�C�i�X		�f�[�^����̓G���[�B
 *				//
 *				virtual int SetData(size_t len, LPCVOID P)
 *				{
 *					m_data.SetElements(len, static_cast<const BYTE*>(P));
 *					if ( len >= 1 )
 *					{
 *						Zero(m_param);
 *						const BYTE* B = static_cast<const BYTE*>(P);
 *						BYTE r = B[0]; // result;
 *						size_t l = (r == 0) ? sizeof(TFwVersionData) : 1;
 *						if ( l == len )
 *						{
 *							memcpy(&m_param, P, l);
 *							return 0;
 *						}
 *					}
 *					return InvalidParameter;
 *				}
 *			};
 *
 *		\endcode
 *	- ExecuteCommand() �ŁA���M����p�P�b�g�ƁA���҂����M�p�P�b�g�������ɃR�[������B
 *		\code
 *			CVersionCommand pc;
 *			CVersionResponse pr;
 *			m_commander.ExecuteCommand(pr, pc, 1000);
 *		\endcode
 *		��������ƁA pr �Ɏ�M�������e�������Ă���B
 *
 *	@par�K�v�t�@�C��
 *			TnbPacketCommander.h
 * 
 *	@date 10/08/24	�V�K�쐬
 *	@date 11/01/18	�啝���z
 *	@date 11/03/04	ExecuteCommand() �̉����҂����̃L�����Z����ǉ�
 *	@date 11/07/05	SendRawData() , SetLogger() �ǉ��B ILogger �A�ǉ��B
 *	@date 11/07/06	NotOpen ��ǉ��B
 *	@date 11/07/06	SetLogger() ILogger �A�p�~�B
 *	@date 11/07/13	SetAliveTime() �ǉ��B
 *	@date 11/09/20	�^�C���A�E�g���Ŕj�����ꂽ�f�[�^��������ƃ��X�i�[�ɏグ��悤�ɏC���B
 *	@date 12/02/29	SendCommand() -> SendPacket() �B SetBasicPacket() -> SetPacketFormat()
 *	@date 12/03/23	IListener::OnCommanderReceivedExpectationPacket() �ɑΉ��B
 *	@date 14/05/16	IsConnect() �A�ǉ��B
 *	@date 15/02/20	SetTearminalTime() �ǉ��B
 *	@date 15/03/18	ExecuteCommand() �𕪉��������\�b�h�iStart, Wait, Stop�j��ǉ��B
 *	@date 18/09/22	PurgeQueue() �A�ǉ��B
 */
class CPacketCommander : public IPacketCommandExecuter, CCopyImpossible
{
public:

	/// ���s���
	enum EFailure
	{
		Timeout			= -1,	///< �^�C���A�E�g
		SendError		= -2,	///< ���M�G���[
		Canceled		= -3,	///< �L�����Z��
		NotOpen			= -4,	///< �I�[�v�����ĂȂ�
	};

	/**@ingroup REPORT
	 * �p�P�b�g�R�}���_�[�̃��X�i�[
	 *
	 *		CPacketCommander::SetListener() �œo�^���邱�ƂŁA��M���m�F���邱�Ƃ��o���܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPacketCommander.h
	 * 
	 *	@date 10/08/24	�V�K�쐬
	 *	@date 12/03/23	OnCommanderReceivedExpectationPacket() �ǉ��B
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �p�P�b�g��M.
		 *		��͐��������p�P�b�g���ʒm����܂��B
		 *	@note �����҂��Ώۂ̃p�P�b�g�͒ʒm����܂���B
		 *	@param pPacket ��M�����p�P�b�g�B CPacketCommander::SetPacketFormat �œn�����I�u�W�F�N�g���̂��̂��K���ʒm����܂��B
		 *	@param result ��͌��ʁi�O�ȉ��j�B�����҂��łȂ��ꍇ�A��ɂO�ɂȂ�܂��B
		 */
		virtual void OnCommanderReceivedPacket(const IPacket* pPacket, int result) = 0;

		/**
		 * [�ʒm] ���҃p�P�b�g��M.
		 *		�����҂��Ώۂ̃p�P�b�g�̂ݒʒm����܂��B
		 *	@param pPacket ��M�����p�P�b�g�B CPacketCommander::SetPacketFormat �œn�����I�u�W�F�N�g���̂��̂��K���ʒm����܂��B
		 */
		virtual void OnCommanderReceivedExpectationPacket(const IPacket* pPacket) {}

		/**
		 * [�ʒm] ���ݎ�M.
		 *		��͐��������p�P�b�g�ȊO�̃S�~�f�[�^���ʒm����܂��B
		 *	@param b �f�[�^
		 *	@param result �S�~���f���R�B��� IPacket �̃T�u�N���X�� Deserialize() �̃G���[�R�[�h�B
		 */
		virtual void OnCommanderDustyData(BYTE b, int result) = 0;

		/**
		 * [�ʒm] �C�x���g�ʒm.
		 *	@param event �C�x���g
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnReportEvent(EReportEvent event) = 0;
	};


	//-------------------------------------


	/**
	 *  �R���X�g���N�^
	 *	@param iQueueSize ��MByte�L���[�̃T�C�Y�B�ȗ������10000�ł��B
	 */
	CPacketCommander(int iQueueSize = 10000) : m_pRep(NULL), m_queue(iQueueSize)
	{
	}

	/// �f�X�g���N�^
	~CPacketCommander(void)
	{
		CPacketCommander::Close();
	}

	/**
	 * [�ݒ�] ��͗p�p�P�b�g�w��.
	 *	@attention �{���\�b�h�Ŏw�肵���C���X�^���X�͖{�N���X���Ŏg�p���܂��̂ŁA
	 *		���삷�邱�Ƃ͂��Ȃ��ł��������B
	 *	@param pPacket �p�P�b�g
	 */
	void SetPacketFormat(IPacketCommandExecuter::IPacket* pPacket)
	{
		m_in.m_pPacketFormat = pPacket;
	}

	/**
	 * [�ݒ�] ��͗p�p�P�b�g�w��.
	 *	@param pPacket �p�P�b�g
	 * 	@deprecated �p�~�\��ł��B SetPacketFormat() ���g�p���Ă��������B
	 */
	void _deprecated SetBasicPacket(IPacketCommandExecuter::IPacket* pPacket)
	{
		SetPacketFormat(pPacket);
	}

	/**
	 * [�ݒ�] ���X�i�[�ݒ�.
	 *	@param pListener ���X�i�[
	 */
	void SetListener(IListener* pListener)
	{
		m_in.m_pListener = pListener;
	}

	/**
	 * [�ݒ�] �L���[�L�����Ԑݒ�.
	 *		�L���[�i�[��A��莞�Ԏ�M���Ȃ��ꍇ�A�L���[���e��j�����邱�Ƃ��o���܂��B
	 *	@note Open() ���A �L���[���g�����w�肵�ĂȂ��ꍇ�A���̐ݒ�͖��Ӗ��ł��B
	 *	@note SetTearminalTime() �Ɠ����Ɏw��ł��܂���B
	 *	@param time �L���[�̗L������(ms)�B�Ō�Ɏ�M���ĉ�͂��Ă���w�莞�Ԍo�߂�����A�L���[�̐擪�̈�o�C�g��j�����āA
	 *					�ēx��͂����܂��B�ȗ����͖���( DWORD_MAX )�ł��B
	 */
	void SetAliveTime(DWORD time = DWORD_MAX)
	{
		m_queue.SetAliveTime(time);
	}

		/**
	 * [�ݒ�] �I�[���Ԑݒ�.
	 *		�L���[�i�[��A��莞�Ԏ�M���Ȃ��ꍇ�ɃL���[���e����͂���悤�ɂ��܂��B
	 *	@note Open() ���A �L���[���g�����w�肵�ĂȂ��ꍇ�A���̐ݒ�͖��Ӗ��ł��B
	 *	@note SetAliveTime() �Ɠ����Ɏw��ł��܂���B
	 *	@param time �I�[����(ms)�B
	 */
	void SetTearminalTime(DWORD time = DWORD_MAX)
	{
		m_queue.SetTearminalTime(time);
	}
	
	/**
	 * [�擾] �I�[�v���m�F.
	 *	@retval true �I�[�v���ς�.
	 *	@retval false ���I�[�v��.
	 */
	bool IsOpened(void) const
	{
		if ( m_pRep != NULL )
		{
			return m_pRep->IsAlive();
		}
		return false;
	}
	
	/**
	 * [�擾] �ڑ��m�F.
	 *	@retval true �ڑ�.
	 *	@retval false �ؒf.
	 */
	bool IsConnect(void) const
	{
		if ( m_pRep != NULL )
		{
			return m_pRep->IsConnect();
		}
		return false;
	}

	/**
	 * [�ݒ�] �I�[�v��.
	 *	@note �w��� ���|�[�^���g�p���ăI�[�v�����܂��B
	 *	@note ��͗p�p�P�b�g��o�^���Ă��Ȃ��Ǝ��s���܂��B
	 *	@param pRep ���|�[�^�B�N���[�Y����܂Ŕj�����Ȃ��ł��������B
	 *	@param withQueueing ture �Ȃ�L���[���g���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool Open(IReport* pRep, bool withQueueing = false)
	{
		CPacketCommander::Close();
		if ( pRep != NULL && m_in.m_pPacketFormat != NULL )
		{
			m_pRep = pRep;
			if ( withQueueing )
			{
				m_queue.SetEnvironment(m_pRep, &m_in);
			}
			else
			{
				m_pRep->SetListener(&m_in);
			}
			return m_pRep->Start();
		}
		return false;
	}

	/**
	 * [�ݒ�] �N���[�Y.
	 *	@note �I�[�v�����Ă��Ȃ���Ή������܂���B
	 */
	virtual void Close(void)
	{
		if ( m_pRep != NULL )
		{
			m_pRep->Stop();
		}
		m_queue.ResetEnvironment();
		m_pRep = NULL;
	}

	/**
	 * [���s] �p�P�b�g���M.
	 *	@note ���X�i�[�Ŏ󂯂����\�b�h������g�p�ł��܂��B
	 *	@param packet �p�P�b�g
	 *	@retval 0�ȏ� �����B
	 *	@retval SendError ���M�G���[�B
	 *	@retval NotOpen ���I�[�v���B
	 */
	virtual int SendPacket(const IPacketCommandExecuter::IPacket& packet)
	{
		if ( IsOpened() )
		{
			ASSERT( m_pRep != NULL );
			// ���M�R�}���h�f�[�^�쐬
			CByteVector vb = packet.Serialize();
			// ��M�������b�N
			m_in.m_sendSync.Lock();
			// ���M
			bool r = m_pRep->Send(vb.GetSize(), vb.ReferBuffer());
			// ��M�����A�����b�N
			m_in.m_sendSync.Unlock();
			if ( r )
			{
				return 0; //����
			}
			return SendError; //���M�G���[
		}
		return NotOpen;
	}

	/**
	 * [���s] �p�P�b�g���M.
	 *	@param packet �p�P�b�g
	 *	@retval 0�ȏ� �����B
	 *	@retval SendError ���M�G���[�B
	 *	@retval NotOpen ���I�[�v���B
	 * 	@deprecated �p�~�\��ł��B SendPacket() ���g�p���Ă��������B
	 */
	int _deprecated SendCommand(const IPacketCommandExecuter::IPacket& packet)
	{
		return SendPacket(packet);
	}

	/**
	 * [���s] �R�}���h���s.
	 *		�R�}���h�p�P�b�g�𑗐M���A���������邩�A�^�C���A�E�g����܂Ŋ֐��̓��^�[�����܂���B
	 *	@note �R�}���h�p�P�b�g�ɐ������� Serialize �� Empty ��Ԃ��N���X��n���Ƒ��M���Ȃ��̂ŁA���X�|���X�҂��݂̂ł��܂��B
	 *	@param[out] _response ���X�|���X�^�B�������A���ʂ��i�[����܂��B
	 *	@param[in] command �R�}���h�^�B
	 *	@param[in] timeout ���X�|���X�̃^�C���A�E�g����(ms).
	 *	@retval 0�ȏ� �����B
	 *	@retval Timeout �^�C���A�E�g�B
	 *	@retval SendError ���M�G���[�B
	 *	@retval NotOpen ���I�[�v���B
	 *	@retval 0���� ���̑��̃G���[(��� IPacketCommandExecuter::IPacket::Deserialize() �̖߂�l)�B
	 */
	virtual int ExecuteCommand(IPacketCommandExecuter::IPacket& _response, const IPacketCommandExecuter::IPacket& command, DWORD timeout = 500)
	{
		int r = ExecuteCommand_Start(_response, command);
		if ( r >= 0 )
		{
			r = ExecuteCommand_Wait(_response, timeout);
			ExecuteCommand_Stop(_response);
		}
		return r;
	}

	/**
	 * [���s] �R�}���h���s(�J�n).
	 *		�R�}���h�p�P�b�g�𑗐M���A�����҂����������܂��B
	 *	@note �ʏ� ExecuteCommand() ���g�p���A�{���\�b�h�͎g�p���܂���B
	 *	@param[out] _response ���X�|���X�^�B�������A���ʂ��i�[����܂��B
	 *	@param[in] command �R�}���h�^�B
	 *	@retval 0�ȏ� �����B�I����A�K�� ExecuteCommand_Stop() ���R�[�����Ă��������B
	 *	@retval Timeout �^�C���A�E�g�B
	 *	@retval SendError ���M�G���[�B
	 *	@retval NotOpen ���I�[�v���B
	 *	@retval 0���� ���̑��̃G���[(��� IPacketCommandExecuter::IPacket::Deserialize() �̖߂�l)�B
	 */
	int ExecuteCommand_Start(IPacketCommandExecuter::IPacket& _response,const IPacketCommandExecuter::IPacket& command)
	{
		if ( IsOpened() )
		{
			ASSERT( m_pRep != NULL );
			//== ����
			bool r = true;
			// ���M�R�}���h�f�[�^�쐬
			CByteVector vb = command.Serialize();
			// �V�^�X�N
			m_in.NewTask(&_response);
			//== ���M
			if ( ! vb.IsEmpty() )
			{
				// ��M�������b�N
				m_in.m_sendSync.Lock();
				// ���M
				r = m_pRep->Send(vb.GetSize(), vb.ReferBuffer());
				// ��M�����A�����b�N
				m_in.m_sendSync.Unlock();
			}
			return r ? 1 : SendError;
		}
		return NotOpen;
	}

	/**
	 * [���s] �R�}���h���s(�҂�).
	 *		���������邩�A�^�C���A�E�g����܂Ŋ֐��̓��^�[�����܂���B
	 *	@note �ʏ� ExecuteCommand() ���g�p���A�{���\�b�h�͎g�p���܂���B
	 *	@param[out] _response ���X�|���X�^�B�������A���ʂ��i�[����܂��B ExecuteCommand_Start() �Ɠ����I�u�W�F�N�g���g�p���Ă��������B
	 *	@param[in] timeout ���X�|���X�̃^�C���A�E�g����(ms).
	 *	@retval 0�ȏ� �����B
	 *	@retval Timeout �^�C���A�E�g�B
	 *	@retval SendError ���M�G���[�iExecuteCommand_Start() ���������Ă��Ȃ��j�B
	 *	@retval NotOpen ���I�[�v���B
	 *	@retval 0���� ���̑��̃G���[(��� IPacketCommandExecuter::IPacket::Deserialize() �̖߂�l)�B
	 */
	int ExecuteCommand_Wait(IPacketCommandExecuter::IPacket& _response, DWORD timeout = 500)
	{
		if ( IsOpened() )
		{
			// �^�X�N
			CInner::TTask* pTask = m_in.FindTask(&_response);
			if ( pTask != NULL )
			{
				DWORD to = timeout;
				while ( true )
				{
					if ( pTask->recvEvent.Lock(to) )
					{
						int r = pTask->analyzedCode;
						m_in.EndTask(pTask);
						return r;
					}
					if ( ! pTask->isTimeoutExtend )
					{
						break;
					}
					pTask->isTimeoutExtend = false; //�^�C���A�E�g����
					if ( to > 0 )
					{
						DWORD t = pTask->extendTime;
						if ( t == 0 )
						{
							t = timeout;
						}
						DWORD d = pTask->tick.GetPassedCount();
						if ( d > t )
						{
							to = 0;
						}
						else
						{
							to = t - d;
						}
					}
				}
				return Timeout; //�^�C���A�E�g
			}
			return SendError; //���M�G���[(Start���ĂȂ�)
		}
		return NotOpen;
	}

	/**
	 * [���s] �R�}���h���s(�I��).
	 *		ExecuteCommand_Start() ���g�����ꍇ�A�K���{���\�b�h���g�p���Ă��������B
	 *	@param response ���X�|���X�^�BExecuteCommand_Start() �Ɠ����I�u�W�F�N�g���g�p���Ă��������B
	 */
	void ExecuteCommand_Stop(IPacketCommandExecuter::IPacket& response)
	{
		CInner::TTask* pTask = m_in.FindTask(&response);
		if ( pTask != NULL )
		{
			m_in.EndTask(pTask);
		}
	}

	/**
	 * [�ݒ�] �^�C���A�E�g����.
	 *		�R�}���h�̉����҂����������܂��B������R�[���������Ƀ^�C���A�E�g����
	 *		�̍Đݒ肪�s���܂��i�Ⴆ�΁A�^�C���A�E�g5�b�̐ݒ�̏ꍇ�ŁA�R�}���h���M��A
	 *		2�b��ɖ{���\�b�h���R�[������ƁA�S���łV�b�ԁA���X�|���X��҂��܂��j�B
	 *	@note ���X�i�[����A�ʃX���b�h����g�p���܂��B
	 *	@param threadId �X���b�hID�B�ȗ�����ƁA�S�Ẵ^�X�N�������B
	 */
	void ExtendTimeout(DWORD threadId = 0)
	{
		m_in.ExtendTimeout(0, threadId);
	}

	/**
	 * [�ݒ�] �^�C���A�E�g����.
	 *		�R�}���h�̉����҂����w�莞�ԁA�������܂��B
	 *	@note ���X�i�[����A�ʃX���b�h����g�p���܂��B
	 *	@param time �������鎞��(ms)
	 *	@param threadId �X���b�hID�B�ȗ�����ƁA�S�Ẵ^�X�N�������B
	 */
	void ExtendTimeoutEx(DWORD time, DWORD threadId = 0)
	{
		m_in.ExtendTimeout(time, threadId);
	}

	/**
	 * [�ݒ�] ���X�|���X�҂��L�����Z��.
	 *	@note ExecuteCommand() �ŉ����҂����A���̃��\�b�h�łƂ߂邱�Ƃ��ł��܂��B
	 *		���̏ꍇ�A ExecuteCommand() �̖߂�l�� {@link CPacketCommander::Canceled Canceled(-4)} �ɂȂ�܂��B
	 *	@param threadId �X���b�hID�B�ȗ�����ƁA�S�Ẵ^�X�N���L�����Z���B
	 */
	void CancelWaitResponse(DWORD threadId = 0)
	{
		m_in.CancelWaitResponse(threadId);
	}

	/**
	 * [���M] ���f�[�^���M.
	 *		�C�ӂ̃f�[�^�𑗐M���邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y
	 *	@param pData �f�[�^
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SendRawData(size_t size, const BYTE* pData)
	{
		bool r = false;
		if ( m_pRep != NULL )
		{
			// ��M�������b�N
			m_in.m_sendSync.Lock();
			// ���M
			r = m_pRep->Send(size, pData);
			// ��M�����A�����b�N
			m_in.m_sendSync.Unlock();
		}
		return r;
	}

	/**
	 * [�擾] �L���[�󂫃T�C�Y�擾.
	 *	@return �󂫃T�C�Y
	 */
	size_t GetQueueFreeSize(void) const
	{
		return m_queue.GetQueueFreeSize();
	}

	/**
	 * [�ݒ�] �L���[�N���A
	 */
	void PurgeQueue(void)
	{
		m_queue.Purge();
	}

	/**
	 * [�Q��] ���|�[�^�[�Q��.
	 *	@retval NULL ���g�p
	 *	@retval NULL�ȊO �g�p���Ă��郌�|�[�^�[�B
	 */
	IReport* ReferReporter(void)
	{
		return IsOpened() ? m_pRep : NULL;
	}

private:

	/// �����N���X
	class CInner : public IReport::IListener, public CQueueingReporter::IListener
	{
		DEFPARENTLISTENER(CPacketCommander, IParentListener);
	public:
		/// �^�X�N
		struct TTask
		{
			DWORD				threadId;				///< �X���b�hID
			CSyncEvent			recvEvent;				///< ��M�C�x���g�B���҂̃��V�[�u������΃V�O�i�����܂��B
			bool				isAnalyzed;				///< ���҃p�P�b�g��M��͍ς݁H
			IPacket*			pExpectationPacket;		///< ���҃p�P�b�g
			bool				isTimeoutExtend;		///< ��������H
			DWORD				extendTime;				///< ��������(ms)
			CTickCount			tick;					///< �������ԊǗ��p
			int					analyzedCode;			///< ��͌���
			/// �R���X�g���N�^
			TTask(void) : recvEvent(false, false), pExpectationPacket(NULL), isTimeoutExtend(false), extendTime(0), analyzedCode(0), isAnalyzed(true)
			{}
		};

		//-----

		/// �R���X�g���N�^
		CInner(void) : m_pListener(NULL), m_pPacketFormat(NULL)
		{
		}
		void EndTask(TTask* p)
		{
			EXCLUSIVE( &m_task );
			if ( m_task.Delete(p) )
			{
//				p->recvEvent.Unlock();
				delete p;
			}
		}
		TTask* FindTask(IPacket* pExpectation)
		{
			loop ( i, m_task )
			{
				TTask* p = m_task.At(i);
				if ( p->pExpectationPacket == pExpectation )
				{
					return p;
				}
			}
			return NULL;
		}
		TTask* NewTask(IPacket* pExpectation)
		{
			TTask* p = new TTask;
			p->threadId = ::GetCurrentThreadId();
			p->pExpectationPacket = pExpectation;
			p->isAnalyzed = false;
			p->recvEvent.ToLock();
			p->isTimeoutExtend = false;
			p->extendTime = 0;
			ASSERT( ! m_task.Has(p) );
			m_task.InsertEx(p);
			return p;
		}
		void ExtendTimeout(DWORD time, DWORD threadId = 0)
		{
			if ( threadId == 0 )
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					p->tick.Reset();
					p->isTimeoutExtend = true;
					p->extendTime = time;
				}
			}
			else
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					if ( threadId == p->threadId )
					{
						p->tick.Reset();
						p->isTimeoutExtend = true;
						p->extendTime = time;
						break;
					}
				}
			}
		}
		/// �L�����Z��
		void CancelWaitResponse(DWORD threadId = 0)
		{
			if ( threadId == 0 )
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					p->analyzedCode = Canceled;
//					p->pExpectationPacket = NULL;
					p->isAnalyzed = true;
					p->recvEvent.Unlock();
				}
			}
			else
			{
				loop ( i, m_task )
				{
					TTask* p = m_task.At(i);
					if ( threadId == p->threadId )
					{
						p->analyzedCode = Canceled;
//						p->pExpectationPacket = NULL;
						p->isAnalyzed = true;
						p->recvEvent.Unlock();
						break;
					}
				}
			}
		}
		/// [�ʒm] �C�x���g�����ʒm
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( ev.HasEvent() )
			{
				if ( ev.GetEvent() == ReportEvent_End )
				{
					CancelWaitResponse();
				}
				if ( m_pListener != NULL )
				{
					return m_pListener->OnReportEvent(ev.GetEvent());
				}
				return true;
			}
//			EXCLUSIVE( &m_sendSync );
			CConstAdapterT<BYTE> c(ev.GetDataSize(), ev.GetData());
			if ( Analyze(c) == 0 )
			{
				if ( m_pListener != NULL )
				{
					loop ( i, ev.GetDataSize() )
					{
						m_pListener->OnCommanderDustyData(ev.GetData()[i], Timeout);
					}
				}
			}
			return true;
		}
		/// [�ʒm] �f�[�^��M�ʒm
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
		{
//			EXCLUSIVE( &m_sendSync );
			size_t r = Analyze(c);
			if ( boIsLast && r == 0 )
			{
				r = c.GetSize();
				if ( m_pListener != NULL )
				{
					loop ( i, r )
					{
						m_pListener->OnCommanderDustyData(c.At(i), Timeout);
					}
				}
			}
			return r;
		}
		/// [�ʒm] ���݉��ʒm
		virtual void OnReportDustyData(BYTE b)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnCommanderDustyData(b, Timeout);
			}
		}
		/// ���
		size_t Analyze(const IConstCollectionT<BYTE>& c)
		{
			size_t size = 0;
			if ( m_pPacketFormat != NULL )
			{
				int a = m_pPacketFormat->Deserialize(c);
				if ( a > 0 )
				{
					size = a;
					a = 0;
					bool isDisp = false;
					{
						EXCLUSIVE( &m_task );
						loop ( i, m_task )
						{
							TTask* p = m_task.At(i);
//							if ( p->pExpectationPacket != NULL )
							if ( ! p->isAnalyzed )
							{
								a = p->pExpectationPacket->Deserialize(c);
								if ( a > 0 )
								{
									if ( m_pListener != NULL )
									{
										m_pListener->OnCommanderReceivedExpectationPacket(m_pPacketFormat);
									}
									p->analyzedCode = a;
//									p->pExpectationPacket = NULL;
									p->isAnalyzed = true;
									p->recvEvent.Unlock();
									isDisp = true;
									break;
								}
							}
						}
					}
					if ( ! isDisp && m_pListener != NULL )
					{
						m_pListener->OnCommanderReceivedPacket(m_pPacketFormat, a);
					}
				}
				else if ( a == 0 )
				{
				}
				else
				{
					size = 1;
					if ( m_pListener != NULL )
					{
						m_pListener->OnCommanderDustyData(c.At(0), a);
					}
				}
			}
			return size;
		}
		CSyncSection			m_sendSync;			///< ���M���r��
		IParentListener*		m_pListener;		///< ���X�i�[
		IPacket*				m_pPacketFormat;	///< ��͗p
		CSingleSetT<TTask*>		m_task;				///< �^�X�N
	};
	CInner				m_in;				///< �����N���X
	IReport*			m_pRep;				///< ���|�[�^
	CQueueingReporter	m_queue;			///< �L���[�C���O
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 C_ASSERT ( sizeof(TPacDword) == 4 ); ///< �T�C�Y�`�F�b�N
 C_ASSERT ( sizeof(TPacWord) == 2 ); ///< �T�C�Y�`�F�b�N
 C_ASSERT ( sizeof(TPacByte) == 1 ); ///< �T�C�Y�`�F�b�N
#endif



}; // TNB



