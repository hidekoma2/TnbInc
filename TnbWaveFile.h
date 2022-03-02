#pragma once
/**
 *@file
 * WAVE�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include <TnbMap.h>
#include <TnbConstFileMapping.h>
#include <TnbDummyCollection.h>
#include <TnbCollectionAdapter.h>
#include <mmsystem.h>



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * WAVE�t�H�[�}�b�g�N���X
 *	
 *		wav�t�@�C�����`�F�b�N���ǂݍ��ނ��Ƃ��o���܂��B	
 *
 *	@par�K�v�t�@�C��
 *			TnbWaveFile.h
 *
 *	@date 08/08/12	�V�K�쐬
 */
class CWaveFormat : public WAVEFORMATEX
{
public:

	/** 
	 * �R���X�g���N�^
	 *	@note ���̃����o���v�Z���Đݒ肵�܂��B
	 *	@param samplesPerSec ���g���B�ȗ������ 44100�B
	 *	@param bitsPerSample ��f�[�^�̃r�b�g�T�C�Y�B�ȗ������ 16�B 
	 *	@param channels �`���l�����B�ȗ������ 2�B
	 */
	CWaveFormat(DWORD samplesPerSec = 44100, WORD bitsPerSample = 16, WORD channels = 2)
	{
		wFormatTag = WAVE_FORMAT_PCM;
		cbSize = sizeof(WAVEFORMATEX);
		Set(samplesPerSec, bitsPerSample, channels);
	}

	/**
	 * ����R���X�g���N�^
	 *	@param f ���
	 */
	CWaveFormat(const WAVEFORMATEX& f)
	{
		WAVEFORMATEX* P = this;
		*P = f;
	}

	/** 
	 * [�ݒ�] �ݒ�
	 *	@note ���̃����o���v�Z���Đݒ肵�܂��B
	 *	@param samplesPerSec ���g���B
	 *	@param bitsPerSample ��f�[�^�̃r�b�g�T�C�Y�B 
	 *	@param channels �`���l�����B
	 */
	void Set(DWORD samplesPerSec, WORD bitsPerSample, WORD channels)
	{
		nSamplesPerSec	= samplesPerSec;
		wBitsPerSample	= bitsPerSample;
		nChannels		= channels;
		nBlockAlign		= static_cast<WORD>(nChannels * wBitsPerSample / 8);
		nAvgBytesPerSec	= nSamplesPerSec * nBlockAlign;
	}

	/**
	 * [�v�Z] �T���v�������玞�Ԃ��v�Z
	 *	@param samples �T���v����
	 *	@return ����(ms)
	 */
	DWORD CalcTime(size_t samples) const
	{
		ULONGLONG r = samples;
		r *= 1000;
		return static_cast<DWORD>((r + nSamplesPerSec - 1) / nSamplesPerSec);
	}

	/**
	 * [�v�Z] ���Ԃ���T���v�������v�Z
	 *	@param time ����(ms)
	 *	@return �T���v����
	 */
	size_t CalcSamples(DWORD time) const
	{
		ULONGLONG r = time;
		r *= nSamplesPerSec;
		return static_cast<size_t>(r / 1000);
	}
};



/**@ingroup AUDIO
 * WAVE�t�@�C���N���X
 *	
 *		wav�t�@�C�����`�F�b�N���ǂݍ��ނ��Ƃ��o���܂��B
 *
 *	@note �}�b�v�h�t�@�C�����g�p���Ă��܂��B
 *
 *		�Ή����Ă���t�@�C���́A�񈳏k�� 8bit��16bit��PCM�ł��B�@
 *
 *	@attention RefData() ���\�b�h�́A���[�J���������ɓǂݍ��񂾃f�[�^��Ԃ��܂��B
 *			���̂��߁A 1G �𒴂��� WAVE�t�@�C���̏ꍇ�A�������s���ɂȂ�\��������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbWaveFile.h
 *
 *	@date 08/08/12	�V�K�쐬
 *	@date 11/08/24	GetChunk() �A�ǉ��B CConstFileMapping ���g�p����悤�ɕύX�B 4GB �܂ł̃t�@�C���ɑΉ��B
 *	@date 11/08/30	LIST INFO �`�����N�N���X�A�ǉ��B GetData() �p�~�B GetByteData() , GetShortData() �ǉ��B
 */
class CWaveFile : CCopyImpossible
{
public:

	/// �`�����N�w�b�_�[�^
	struct TChunkHeader
	{
		FOURCC type;	///< �^
		LONG length;	///< ����
	};

	/**@ingroup AUDIO
	 * LIST INFO �`�����N�Ǘ�
	 *	
	 *	@par�K�v�t�@�C��
	 *			TnbWaveFile.h
	 *
	 *	@date 11/08/29	�b��쐬
	 */
	class CListInfoChunk
	{
	public:

		/**
		 * [�ݒ�] ��
		 */
		void Empty(void)
		{
			m_map.RemoveAll();
		}

		/**
		 * [�擾] �^�C�v�ꗗ�擾.
		 *		�����Ă���^�C�v���ꗗ���܂��B
		 *	@return �ꗗ
		 */
		CVectorT<FOURCC> EnumType(void) const
		{
			CVectorT<FOURCC> v;
			v.Copy(m_map.GetKeysAdapter());
			return v;
		}

		/**
		 * [�m�F] �^�C�v�m�F.
		 *		�w��̃^�C�v�������Ă��邩�A�m�F���܂��B
		 *	@param cc �^�C�v�R�[�h�B MAKEFOURCC() �}�N�����֗��ł��B \n ex) MAKEFOURCC('n', 'o', 't', 'e')
		 *	@retval true �����Ă���.
		 *	@retval false  �����Ă��Ȃ�
		 */
		bool Has(FOURCC cc) const
		{
			return m_map.HasKey(cc);
		}

		/**
		 * [�擾] ������擾.
		 *		�w��̃^�C�v�Ɋ֘A�t�����Ă��镶�����Ԃ��܂��B
		 *	@param[out] _str �����񂪊i�[����܂��B
		 *	@param[in] cc �^�C�v�R�[�h�B MAKEFOURCC() �}�N�����֗��ł��B \n ex) MAKEFOURCC('n', 'o', 't', 'e')
		 *	@retval true �����Ă���.
		 *	@retval false  �����Ă��Ȃ�
		 */
		bool Get(CAscii& _str, FOURCC cc) const
		{
			if ( m_map.HasKey(cc) )
			{
				_str = m_map[cc];
				return true;
			}
			return false;
		}

		/**
		 * [�ݒ�] ������擾.
		 *		�w��̃^�C�v�Ɋ֘A�t���镶������w�肵�܂��B
		 *	@param cc �^�C�v�R�[�h�B MAKEFOURCC() �}�N�����֗��ł��B \n ex) MAKEFOURCC('n', 'o', 't', 'e')
		 *	@param lpsz ������B
		 */
		void Set(FOURCC cc, LPCTSTR lpsz)
		{
			m_map[cc] = lpsz;
		}

		/**
		 * [�쐬] �`�����N�u���b�N�쐬.
		 *	@note "INFO" ����n�܂�`�����N�u���b�N��Ԃ��܂��B
		 *	@return �u���b�N
		 */
		CByteVector Make(void) const
		{
			CByteVector vb;
			vb.AddElements(4, reinterpret_cast<const BYTE*>("INFO"));
			loop ( i, m_map.GetSize() )
			{
				FOURCC cc = m_map.At(i).first;
				const CAscii& s = m_map.At(i).second;
				DWORD len = s.GetLength() + 1;
				vb.AddElements(4, reinterpret_cast<const BYTE*>(&cc));
				vb.AddElements(4, reinterpret_cast<const BYTE*>(&len));
				vb.AddElements(len, reinterpret_cast<const BYTE*>(s.operator LPCSTR()));
				if ( (len & 1) != 0 )
				{
					vb.Add(0);
				}
			}
			return vb;
		}

		/**
		 * [���] �`�����N�u���b�N���.
		 *	@note "INFO" ����n�܂�`�����N�u���b�N����͂��A�{�C���X�^���X�ɐݒ肵�܂��B
		 *	@param c �V���A���C�Y�f�[�^.
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool Analyze(const IConstCollectionT<BYTE>& c)
		{
			m_map.RemoveAll();
			size_t sz = 0;
			DWORD pos = 0;
			FOURCC type;
			sz = c.GetElements(sizeof(FOURCC), reinterpret_cast<BYTE*>(&type), pos);
			if ( sz != sizeof(FOURCC) )
			{
				return false;
			}
			pos += sz;
			if ( type != MAKEFOURCC('I', 'N', 'F', 'O') )
			{
				return false;
			}
			TChunkHeader chunk;
			while ( pos < c.GetSize() )
			{
				sz = c.GetElements(sizeof(chunk), reinterpret_cast<BYTE*>(&chunk), pos);
				if ( sz != sizeof(chunk) )
				{
					return false;
				}
				pos += sz;
				size_t len = chunk.length;
				if ( (len & 1) != 0 )
				{
					len ++; //�����ɂ���
				}
				CAscii& a = m_map[chunk.type];
				sz = c.GetElements(len, reinterpret_cast<BYTE*>(a.GetBuffer(len + 1)), pos);
				if ( sz != len )
				{
					return false;
				}
				pos += sz;
				a.ReleaseBuffer();
			}
			return true;
		}

		/**
		 * [���] �`�����N�u���b�N���.
		 *	@note "INFO" ����n�܂�`�����N�u���b�N����͂��A�{�C���X�^���X�ɐݒ肵�܂��B
		 *	@param size �T�C�Y
		 *	@param P �V���A���C�Y�f�[�^.
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool Analyze(size_t size, const BYTE* P)
		{
			return Analyze(CConstAdapterT<BYTE>(size, P));
		}

	private:
		CMapT<FOURCC, CAscii>	m_map;	///< �}�b�v
	};


	//--------------


	/// �R���X�g���N�^
	CWaveFile(void) : m_dataSize(0), m_dataOffset(0), m_adapter(NULL, 0)
	{
	}

	/// �f�X�g���N�^
	~CWaveFile(void)
	{
		Close();
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@note �I�[�v�����Ă��Ȃ��ƁAGet,Refer�n���\�b�h�͖��Ӗ��Ȓl��Ԃ��܂��B
	 *	@retval true �I�[�v���ς�
	 *	@retval false ���I�[�v���B
	 */
	bool IsOpened(void) const
	{
		return m_file.IsOpened();
	}

	/// [�ݒ�] �N���[�Y
	void Close(void)
	{
		m_file.Close();
		m_file16.Close();
		m_chunkMap.RemoveAll();
		m_refData.RemoveAll();
		m_dataOffset = 0;
		m_dataSize = 0;
	}

	/**
	 * [�ݒ�] PCM�t�@�C���I�[�v��
	 *	@note PCM�t�@�C����ǂݍ��݁A�g�p�ł���悤�ɂ��܂��B
	 *	@param fileName �t�@�C����
	 *	@param format �t�H�[�}�b�g
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Open(LPCTSTR fileName, const WAVEFORMATEX& format)
	{
		Close();
		m_format = format;
		if ( ! m_file.Open(fileName) )
		{
			return false;
		}
		if ( ! m_file16.Open(fileName) )
		{
			return false;
		}
		m_dataOffset = 0;
		m_dataSize = m_file.GetSize();
		return true;
	}

	/**
	 * [�ݒ�] WAV�t�@�C���I�[�v��
	 *	@note WAV�t�@�C����ǂݍ��݁A�g�p�ł���悤�ɂ��܂��B
	 *	@param fileName �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR fileName)
	{
		Close();
		if ( ! m_file.Open(fileName) )
		{
			return false;
		}
		INDEX pos = 0;
		TChunkHeader fileChunk;
		pos += m_file.GetElements(sizeof(fileChunk), reinterpret_cast<BYTE*>(&fileChunk), pos);
		FOURCC riffType;
		pos += m_file.GetElements(sizeof(riffType), reinterpret_cast<BYTE*>(&riffType), pos);
		if ( fileChunk.type == FC_RIFF && fileChunk.length <= ToInt(m_file.GetSize()) && riffType == FC_WAVE )
		{
			while( pos < m_file.GetSize() )
			{
				TChunkHeader chunk;
				pos += m_file.GetElements(sizeof(chunk), reinterpret_cast<BYTE*>(&chunk), pos);
				if ( chunk.type == FC_FMT )
				{
					m_file.GetElements(sizeof(m_format), reinterpret_cast<BYTE*>(&m_format), pos);
					WORD bps = m_format.wBitsPerSample;
					if ( bps != 8 && bps != 16 )
					{
						break;
					}
					if ( m_format.wFormatTag != WAVE_FORMAT_PCM )
					{
						break;
					}
					if ( m_format.nChannels != 1 && m_format.nChannels != 2 )
					{
						break;
					}
					m_format.cbSize = 0;
				}
				else if ( chunk.type == FC_DATA )
				{
					m_dataOffset = pos;
					m_dataSize = chunk.length;
				}
				else
				{
					TChunkMap cm;
					cm.offset = pos;
					cm.size = chunk.length;
					m_chunkMap[chunk.type] = cm;
				}
				if ( chunk.length == 0 )
				{
					break;
				}
				pos += chunk.length;
			}
			if ( pos >= m_file.GetSize() )
			{
				if ( m_file16.Open(fileName, m_dataOffset, m_dataSize / sizeof(short)) )
				{
					return true;
				}
			}
		}
		Close();
		return false;
	}

	/**
	 * [�Q��] Wave�t�H�[�}�b�g�Q��
	 *	@return Wave�t�H�[�}�b�g
	 */
	const CWaveFormat& RefFormat(void) const
	{
		return m_format;	///< �ێ��f�[�^�t�H�[�}�b�g
	}

	/**
	 * [�擾] �g�`�f�[�^���擾.
	 *	@return �g�`�f�[�^��
	 */
	size_t GetSamples(void) const
	{
		if ( m_format.nBlockAlign == 0 )
		{
			return 0;
		}
		return m_dataSize / m_format.nBlockAlign;
	}

	/**
	 * [�擾] �g�`�f�[�^���Ԏ擾.
	 *	@return �g�`�f�[�^����(ms)
	 */
	DWORD GetTotalTime(void) const
	{
		if ( m_format.nBlockAlign == 0 || m_format.nSamplesPerSec == 0 )
		{
			return 0;
		}
		ULONGLONG r = (m_dataSize / m_format.nBlockAlign);
		r *= 1000;
		return static_cast<DWORD>((r + m_format.nSamplesPerSec - 1) / m_format.nSamplesPerSec);
	}

	/**
	 * [�擾] �g�`�f�[�^�T�C�Y�擾.
	 *	@return �g�`�f�[�^�T�C�Y
	 */
	size_t GetDataSize(void) const
	{
		return m_dataSize;
	}

	/**
	 * [�擾] �g�`�f�[�^�|�C���^�擾.
	 *	@param i �C���f�b�N�X
	 *	@return �g�`�f�[�^�|�C���^
	 */
	const BYTE* RefData(INDEX i = 0) const
	{
		if ( m_refData.IsEmpty() )
		{
			m_refData.Copy(GetByteData());
		}
		return m_refData.ReferBuffer() + i;
	}

	/**
	 * [�擾] 8bit�g�`�f�[�^�擾.
	 *	@attention �߂�l�͈ꎞ�I�ȃI�u�W�F�N�g�ł��B�{�C���X�^���X�̕ʂ̃��\�b�h���g�p����Ɩ��Ӗ��Ȃ��̂ɂȂ�\�����L��܂��B
	 *			�K�v�ɉ����� CByteVector �Ȃǂ� Copy() ���Ă��������B
	 *	@return �g�`�f�[�^
	 */
	const IConstCollectionT<BYTE>& GetByteData(void) const
	{
		m_adapter = CConstOffsetAdapterT<BYTE>(&m_file, m_dataOffset, m_dataSize);
		return m_adapter;
	}

	/**
	 * [�擾] 16bit�g�`�f�[�^�擾.
	 *	@attention �߂�l�͈ꎞ�I�ȃI�u�W�F�N�g�ł��B�{�C���X�^���X�̕ʂ̃��\�b�h���g�p����Ɩ��Ӗ��Ȃ��̂ɂȂ�\�����L��܂��B
	 *			�K�v�ɉ����� CVectorT<short> �Ȃǂ� Copy() ���Ă��������B
	 *	@return �g�`�f�[�^
	 */
	const IConstCollectionT<short>& GetShortData(void) const
	{
		return m_file16;
	}

	/**
	 * [�擾] �`�����N�f�[�^�擾.
	 *	@attention �߂�l�͈ꎞ�I�ȃI�u�W�F�N�g�ł��B�{�C���X�^���X�̕ʂ̃��\�b�h���g�p����Ɩ��Ӗ��Ȃ��̂ɂȂ�\�����L��܂��B
	 *			�K�v�ɉ����� CByteVector �Ȃǂ� Copy() ���Ă��������B
	 *	@param[in] cc �`�����N�R�[�h�B MAKEFOURCC() �}�N�����֗��ł��B \n ex) MAKEFOURCC('n', 'o', 't', 'e')
	 *	@return �`�����N�f�[�^�B �w��`�����N���Ȃ��ꍇ�A�T�C�Y���O�̃I�u�W�F�N�g���Ԃ�܂��B
	 */
	const IConstCollectionT<BYTE>& GetChunk(FOURCC cc) const
	{
		if ( m_chunkMap.HasKey(cc) )
		{
			m_adapter = CConstOffsetAdapterT<BYTE>(&m_file, m_chunkMap[cc].offset, m_chunkMap[cc].size);
			return m_adapter;
		}
		return m_nullCollection;
	}

	/**
	 * [�擾] LIST,INFO �`�����N�f�[�^�擾
	 *	@param[out] _lic �f�[�^���i�[����܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetListInfoChunk(CListInfoChunk& _lic) const
	{
		const IConstCollectionT<BYTE>& c = GetChunk(MAKEFOURCC('L','I','S','T'));
		if ( c.GetSize() > 0 )
		{
			return _lic.Analyze(c);
		}
		return false;
	}

	/**
	 * [�쐬] Wav�t�@�C���w�b�_�쐬.
	 *	@param[out] _head �쐬�����w�b�_���i�[���܂��B
	 *	@param[in] channels �`���l����
	 *	@param[in] rate ���[�g
	 *	@param[in] bit �r�b�g
	 *	@param[in] dataSize �f�[�^�T�C�Y(Byte��)�B
	 *	@param[in] extendSize ���̑��̃T�C�Y(Byte��)�B���̃`�����N��t������Ƃ��Ɏg�p���܂�
	 *	@return �w�b�_�T�C�Y
	 */
	static size_t MakeWaveHeader(CWorkMem& _head, int channels, long rate, WORD bit, size_t dataSize, size_t extendSize = 0)
	{
		_head.Resize(sizeof(TWaveFileHeader));
		TWaveFileHeader* P = reinterpret_cast<TWaveFileHeader*>(_head.Ref());
		P->fileChunk.type				= FC_RIFF;
		P->fileChunk.length				= dataSize + extendSize + sizeof(TWaveFileHeader) - 8;
		P->fileType						= FC_WAVE;
		P->format.wf.wFormatTag			= WAVE_FORMAT_PCM;
		P->format.wf.nChannels			= static_cast<WORD>(channels);
		P->format.wf.nSamplesPerSec		= rate;
		P->format.wBitsPerSample		= bit;
		P->format.wf.nBlockAlign		= static_cast<WORD>(channels * bit / 8);
		P->format.wf.nAvgBytesPerSec	= channels * bit / 8 * rate;
		P->formatChunk.type				= FC_FMT;
		P->formatChunk.length			= sizeof(PCMWAVEFORMAT);
		P->dataChunk.type				= FC_DATA;
		P->dataChunk.length				= dataSize;
		return sizeof(TWaveFileHeader);
	}

private:
	/// �^���
	enum
	{
		FC_RIFF		= MAKEFOURCC('R', 'I', 'F', 'F'),	///< RIFF
		FC_WAVE		= MAKEFOURCC('W', 'A', 'V', 'E'),	///< WAVE
		FC_FMT		= MAKEFOURCC('f', 'm', 't', ' '),	///< fmt
		FC_DATA		= MAKEFOURCC('d', 'a', 't', 'a'),	///< data
	};
	/// wave�t�@�C���w�b�_�^
	struct TWaveFileHeader 
	{
		TChunkHeader	fileChunk;		///< �^�C�v�w�b�_
		FOURCC			fileType;		///< �^�C�v
		TChunkHeader	formatChunk;	///< �t�H�[�}�b�g�w�b�_
		PCMWAVEFORMAT	format;			///< �t�H�[�}�b�g
		TChunkHeader	dataChunk;		///< �f�[�^�w�b�_
	};
	/// �`�����N�}�b�v�^
	struct TChunkMap
	{
		size_t		size;	///< ����
		INDEX		offset;	///< �I�t�Z�b�g
	};

	CWaveFormat							m_format;			///< �t�H�[�}�b�g
	CConstFileMapping					m_file;				///< �t�@�C���A�N�Z�X�i�w�b�_�Abyte�f�[�^�p�j
	CConstFileMappingT<short>			m_file16;			///< �t�@�C���A�N�Z�X�ishort�f�[�^�p�j
	size_t								m_dataSize;			///< �f�[�^Byte�T�C�Y
	INDEX								m_dataOffset;		///< �f�[�^�t�@�C���ʒu
	CMapT<FOURCC, TChunkMap>			m_chunkMap;			///< ���̑��̃`�����N�f�[�^
	mutable CByteVector					m_refData;			///< �߂�l�p�I�u�W�F�N�g
	mutable CConstOffsetAdapterT<BYTE>	m_adapter;			///< �߂�l�p�A�_�v�^
	CDummyCollectionT<BYTE>				m_nullCollection;	///< �߂�l�p�I�u�W�F�N�g
};



}; // TNB

