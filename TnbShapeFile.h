#pragma once
/**
 *@file
 * ESRI�W���p��������Ђ� �V�F�[�v�t�@�C���A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerVector.h"
#include "TnbLikenMemToFile.h"
#include "TnbDBaseFile.h"



//TNB Library
namespace TNB
{



/**@ingroup DBASE FILE
 * �V�F�[�v�t�@�C���Ǘ��N���X
 *
 *	 ESRI�W���p��������Ђ� �V�F�[�v�t�@�C���A�N�Z�X�N���X�ł��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		// �|�C���g�^�C�v�̃V�F�[�v�t�@�C���N���X�錾
 *		CShapeFileTypePoint sf;
 *
 *		// �o�E���f�B���O�{�b�N�X�̒l��ݒ�
 *		CShapeFileTypePoint::TBoundingBox bb;
 *		bb.xMin = 0.01;
 *		bb.xMax = 100.0;
 *		sf.SetBoundingBox(bb);
 *		// �����t�B�[���h���쐬�B
 *		sf.AddCharacterField("LABEL", 16);	// 0; ���O			
 *		sf.AddDateField("DATE");			// 1; ���t
 *		sf.AddNumericField("LAT.", 8, 3);	// 2; �ܓx
 *		sf.AddNumericField("LONG.", 8, 3);	// 3; �o�x
 *		// ���R�[�h�P���쐬
 *		INDEX idx = sf.AddRecord();
 *		sf.SetPointShapeRecord(idx, 34.5, 56.1);
 *		sf.SetStringRecord(idx, 0, "OX�������");
 *		sf.SetDateRecord(idx, 1, CGrid::TDate(2009, 1, 1));
 *		sf.SetFloatRecord(idx, 2, 123.4);
 *		sf.SetFloatRecord(idx, 3, 32.6);
 *		// ���R�[�h�Q���쐬
 *		idx = sf.AddRecord();
 *		sf.SetPointShapeRecord(idx, 12.3, 33.2);
 *		sf.SetStringRecord(idx, 0, "(��)�\�֗\��");
 *		sf.SetDateRecord(idx, 1, CGrid::TDate(2010, 2, 16));
 *		sf.SetFloatRecord(idx, 2, 111.5);
 *		sf.SetFloatRecord(idx, 3, 5.1);
 *		// ���R�[�h�R���쐬
 *		idx = sf.AddRecord();
 *		// �t�@�C�������o��
 *		sf.SaveFile("c:\\eeeee");
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbShapeFile.h
 * 
 *	@date 09/02/05 �V�K�쐬
 */
class CShapeFile : public CDBaseFile
{
	DEFSUPER(CDBaseFile);
public:

	typedef long Integer;	///< �����t��32bit����(4bytes)
	typedef double Double;	///< �����t��64bitIEEE���������_(8bytes)

	#pragma pack(push)
	#pragma pack(1)

	/**
	 * �r�b�N�G���f�B�A��INTEGER.
	 *		��� INTEGER �����N���X�ł��B
	 *		�������ɂ́A�r�b�N�G���f�B�A���Ŋi�[���܂��B
	 *	@note �������C���[�W���t�@�C���C���[�W�ł��B
	 */
	class CBeInteger
	{
	public:
		/**
		 * �R���X�g���N�^.
		 *	@param l �����l
		 */
		CBeInteger(Integer l = 0) : m_value(m_SwapEndian(l))
		{
		}
		/**
		 * [�擾] �l�擾
		 *	@return �l�@
		 */
		operator Integer() const 
		{
			return m_SwapEndian(m_value);
		}
		/**
		 * [�ݒ�] �l�ݒ�
		 *	@param l �l
		 *	@return �l
		 */
		Integer operator=(Integer l)
		{
			m_value = m_SwapEndian(l);
			return l;
		}
	private:
		// �G���f�B�A���ϊ�
		Integer m_SwapEndian(Integer l) const
		{
			return SwapEndian(DWORD(l));
		}
		Integer	m_value;	///< �l(�r�b�N�G���f�B�A���Ŋi�[)
	};

	/**
	 * �o�E���f�B���O�{�b�N�X�^.
	 *	@note �������C���[�W���t�@�C���C���[�W�ł��B
	 */
	struct TBoundingBox
	{
		Double		xMin;	///< Bounding Box X �ŏ�
		Double		yMin;	///< Bounding Box Y �ŏ�
		Double		xMax;	///< Bounding Box X �ő�
		Double		yMax;	///< Bounding Box Y �ő�
		Double		zMin;	///< Bounding Box Z �ŏ�
		Double		zMax;	///< Bounding Box Z �ő�
		Double		mMin;	///< Bounding Box M �ŏ�
		Double		mMax;	///< Bounding Box M �ő�
		/// �R���X�g���N�^
		TBoundingBox(void)
		{
			Zero(*this);
		}
	};

	/**
	 * �t�@�C���w�b�_�^.
	 *	@note �������C���[�W���t�@�C���C���[�W�ł��B
	 *	@note ��̃��C���t�@�C�����ɁA�w�b�_�͂P�����ł��B
	 */
	struct THead
	{
		CBeInteger		fileCode;	///< �t�@�C���R�[�h
		CBeInteger		padding[5];	///< ���g�p
		CBeInteger		fileLength;	///< �t�@�C�����i�P�� word�j
		Integer			version;	///< �o�[�W����
		Integer			shapeType;	///< �V�F�[�v�^�C�v
		TBoundingBox	bb;			///< �o�E���f�B���O�{�b�N�X
		/// �R���X�g���N�^
		THead(void) : fileCode(9994), version(1000), shapeType(0)
		{
		}
	};

	#pragma pack(pop)

	/**
	 * ���R�[�h�R���e���c���ۃN���X.
	 *
	 *	@note ��̃��C���t�@�C�����ɁA���R�[�h�͕�������܂��B
	 *	@note �{�N���X�ł̓��R�[�h�ԍ����Ǘ����܂���B
	 */
	struct IRecordContent
	{
		/// �f�X�g���N�^
		virtual ~IRecordContent(void) {}
		/**
		 * [�擾] �V�F�[�v�^�C�v�擾.
		 *	@return �V�F�[�v�^�C�v
		 */
		virtual Integer GetShapeType(void) const = 0;
		/**
		 * [���o] ���o��
		 *		�ێ����Ă���f�[�^�������o���i�V�F�[�v�^�C�v�͑ΏۊO�j�B
		 *	@param[out] _w ���C�^�[
		 *	@return �����o���T�C�Y
		 */
		virtual size_t Write(IWriter& _w) const = 0;
		/**
		 * [�Ǎ�] �Ǎ���
		 *		��͂��f�[�^��ێ����܂��B
		 *	@param  r ���[�_�i�V�F�[�v�^�C�v�̒�����|�C���g���Ă��邱�Ɓj
		 *	@param size �T�C�Y
		 */
		virtual void Read(const IReader& r, size_t size) = 0;
		/**
		 * [�擾] ������擾.
		 *	@return ���̃I�u�W�F�N�g���ێ����Ă�����𕶎��񉻂������́B
		 */
		virtual CStr ToString(void) const = 0;
	};

	/**
	 * NULL SHAPE ���R�[�h�R���e���c
	 *
	 */
	class CNullShapeRecordContent : public IRecordContent
	{
	public:
		/**
		 * [�擾] �V�F�[�v�^�C�v�擾.
		 *	@return �V�F�[�v�^�C�v
		 */
		virtual Integer GetShapeType(void) const
		{
			return 0; //Null Shape
		}
		/**
		 * [���o] ���o��
		 *		�ێ����Ă���f�[�^�������o���i�V�F�[�v�^�C�v�͑ΏۊO�j�B
		 *	@param[out] _w ���C�^�[
		 *	@return �����o���T�C�Y
		 */
		virtual size_t Write(IWriter& _w) const
		{
			return 0;
		}
		/**
		 * [�Ǎ�] �Ǎ���
		 *		��͂��f�[�^��ێ����܂��B
		 *	@param  r ���[�_�i�V�F�[�v�^�C�v�̒�����|�C���g���Ă��邱�Ɓj
		 *	@param size �T�C�Y
		 */
		virtual void Read(const IReader& r, size_t size)
		{
		}
		/**
		 * [�擾] ������擾.
		 *	@return ���̃I�u�W�F�N�g���ێ����Ă�����𕶎��񉻂������́B
		 */
		virtual CStr ToString(void) const
		{
			return _T("Null Shape");
		}
	};

	/**
	 * �s�� ���R�[�h�R���e���c
	 *
	 */
	class CUnknownRecordContent : public IRecordContent
	{
	public:
		/**
		 * �R���X�g���N�^
		 *	@param st �V�F�[�v�^�C�v
		 */
		CUnknownRecordContent(Integer st) : m_type(st)
		{
		}
		/**
		 * [�Q��] �f�[�^
		 *	@return �f�[�^�̎Q��
		 */
		CByteVector& Refer(void)
		{
			return m_data;
		}
		/**
		 * [�擾] �V�F�[�v�^�C�v�擾.
		 *	@return �V�F�[�v�^�C�v
		 */
		virtual Integer GetShapeType(void) const
		{
			return m_type;
		}
		/**
		 * [���o] ���o��
		 *		�ێ����Ă���f�[�^�������o���i�V�F�[�v�^�C�v�͑ΏۊO�j�B
		 *	@param[out] _w ���C�^�[
		 *	@return �����o���T�C�Y
		 */
		virtual size_t Write(IWriter& _w) const
		{
			_w.WriteFrom(m_data);
			return m_data.GetSize();
		}
		/**
		 * [�Ǎ�] �Ǎ���
		 *		��͂��f�[�^��ێ����܂��B
		 *	@param  r ���[�_�i�V�F�[�v�^�C�v�̒�����|�C���g���Ă��邱�Ɓj
		 *	@param size �T�C�Y
		 */
		virtual void Read(const IReader& r, size_t size)
		{
			m_data = r.ReadExactly(size);
		}
		/**
		 * [�擾] ������擾.
		 *	@return ���̃I�u�W�F�N�g���ێ����Ă�����𕶎��񉻂������́B
		 */
		virtual CStr ToString(void) const
		{
			return CStr::Fmt(_T("ShapeType.%d , len = %d"), m_type, m_data.GetSize());
		}
	private:
		Integer		m_type;		///< �V�F�[�v�^�C�v
		CByteVector	m_data;		///< �f�[�^
	};


	//--------


	/// �R���X�g���N�^
	CShapeFile(void)
	{
	}

	/**
	 * [�ݒ�] �w�b�_�V�F�[�v�^�C�v�ݒ�.
	 *	@param type �V�F�[�v�^�C�v
	 */
	void SetHeadShapeType(Integer type)
	{
		m_shapeHead.shapeType = type;
	}

	/**
	 * [�擾] �w�b�_�V�F�[�v�^�C�v�擾.
	 *	@return type �V�F�[�v�^�C�v
	 */
	Integer GetHeadShapeType(void) const
	{
		return m_shapeHead.shapeType;
	}

	/**
	 * [�ݒ�] �o�E���f�B���O�{�b�N�X�ݒ�.
	 *	@param bb ���
	 */
	void SetBoundingBox(const TBoundingBox& bb)
	{
		m_shapeHead.bb = bb;
	}

	/**
	 * [�擾] �o�E���f�B���O�{�b�N�X�擾.
	 *	@return ���
	 */
	const TBoundingBox& GetBoundingBox(void) const
	{
		return m_shapeHead.bb;
	}

	/**
	 * [�폜] ���R�[�h�S�폜.
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool AllDeleteRecord(void)
	{
		m_shapeRecords.RemoveAll();
		return _super::AllDeleteRecord();
	}

	/**
	 * [�ǉ�] ���R�[�h�ǉ�.
	 *	@note �ǉ��������R�[�h�̃V�F�C�v�R���e���c�́uNull Shape�v�ɂȂ��Ă��܂��B
	 *	@note �ǉ��������R�[�h�̑����̑S�t�B�[���h��NULL�ɂȂ��Ă��܂��B
	 *	@retval INVALID_INDEX�ȊO �ǉ������B�l�͒ǉ��������R�[�h�C���f�b�N�X�B
	 *	@retval INVALID_INDEX ���R�[�h�ǉ����s
	 */
	INDEX AddRecord(void)
	{
		INDEX r1 = _super::AddRecord();
		INDEX r2 = m_shapeRecords.Add(new CNullShapeRecordContent());
		ASSERT( r1 == r2 );
		return r1;
	}

	/**
	 * [�}��] ���R�[�h�}��.
	 *	@note �}���ǉ��������R�[�h�̑S�t�B�[���h��NULL�ɂȂ��Ă��܂��B
	 *	@param record ���R�[�h�C���f�b�N�X�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool InsertRecord(INDEX record)
	{
		bool r1 = _super::InsertRecord(record);
		bool r2 = m_shapeRecords.Insert(record, new CNullShapeRecordContent());
		ASSERT( r1 == r2 );
		return r1;
	}

	/**
	 * [�폜] ���R�[�h�폜.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool DeleteRecord(INDEX record)
	{
		bool r1 = _super::DeleteRecord(record);
		bool r2 = m_shapeRecords.Remove(record);
		ASSERT( r1 == r2 );
		return r1;
	}

	/**
	 * [�ݒ�] �V�F�[�v�R���e���c�ݒ�.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param P ���R�[�h�R���e���c�B�K�� new �ō쐬�����I�u�W�F�N�g�ł��邱�ƁB
	 *	@retval NULL �C���f�b�N�X���͈͊O�B
	 *	@retval NULL�ȊO ���R�[�h�R���e���c�̎Q�ƁB
	 */
	bool SetShapeRecord(INDEX record, IRecordContent* P)
	{
		return m_shapeRecords.Set(record, P);
	}

	/**
	 * [�擾] �V�F�[�v�R���e���c�擾.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@retval NULL �C���f�b�N�X���͈͊O�B
	 *	@retval NULL�ȊO ���R�[�h�R���e���c�̎Q�ƁB
	 */
	const IRecordContent* GetShapeRecord(INDEX record) const
	{
		return m_shapeRecords.Get(record);
	}

	/**
	 * [�Ǎ�] �t�@�C���Ǎ���
	 *		�ێ����Ă������j�����A�t�@�C������Ǎ��݂܂��B
	 *	@note *.shp , *.shx , *.dbf ����ǂݍ��݂܂��B
	 *	@param lpszBaseFileName �t�@�C�����B�g���q�͂��Ȃ��ł��������B
	 *	@param withTypeCheck true�̏ꍇ�A���݂� GetHeadShapeType() �œ�����^�C�v�ƈقȂ�t�@�C���̓G���[�ɂȂ�܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool LoadFile(LPCTSTR lpszBaseFileName, bool withTypeCheck = true)
	{
		AllDeleteRecord();
		CStr mainFn;
		CStr indexFn;
		CStr attrFn;
		m_MakeFileName(mainFn, indexFn, attrFn, lpszBaseFileName);
		CFileReader mainRf;
		CFileReader indexRf;
		Integer oldType = GetHeadShapeType();
		if ( mainRf.Open(mainFn) && indexRf.Open(indexFn) )
		{
			try
			{
				m_LoadMain(mainRf);
			}
			catch ( CTnbException& e )
			{
				e.OnCatch();
				_super::AllDeleteRecord();
				return false;
			}
			catch ( ... )
			{
				throw;
			}
			bool r = _super::LoadFile(attrFn);
			//
			if ( r )
			{
				if ( withTypeCheck && oldType != GetHeadShapeType() )
				{
					TRACE2(" ���C���t�@�C���̃^�C�v[%d]�ƌ��݂̃^�C�v[%d]����v���Ă��܂���I\n", GetHeadShapeType(), oldType);
					SetHeadShapeType(oldType);
					_super::AllDeleteRecord();
					r = false;
				}
				else if ( _super::GetRecordCount() != m_shapeRecords.GetSize() )
				{
					TRACE0(" ���C���t�@�C���Ƒ����t�@�C���̃��R�[�h�̐�����v���Ă��܂���I\n" );
					_super::AllDeleteRecord();
					r = false;
				}
			}
			//
			return r;
		}
		return false;
	}

	/**
	 * [�쐬] �t�@�C���쐬.
	 *		�ێ����Ă���f�[�^���t�@�C���ɏ����o��.
	 *	@note *.shp , *.shx , *.dbf �֏����o���܂��B
	 *	@param lpszBaseFileName �t�@�C�����B�g���q�͂��Ȃ��ł��������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SaveFile(LPCTSTR lpszBaseFileName) const
	{
		CStr mainFn;
		CStr indexFn;
		CStr attrFn;
		m_MakeFileName(mainFn, indexFn, attrFn, lpszBaseFileName);
		CFileWriter mainWf;
		CFileWriter indexWf;
		if ( mainWf.New(mainFn) && indexWf.New(indexFn) )
		{
			try
			{
				m_SaveMain(mainWf, indexWf);
			}
			catch ( CTnbException& e )
			{
				e.OnCatch();
				return false;
			}
			catch ( ... )
			{
				throw;
			}
			bool r = _super::SaveFile(attrFn);
			//
			return r;
		}
		return false;
	}

protected:

	/**
	 * [�ʒm] �^�C�v�ʓǍ��ݏ���
	 *	@param type �^�C�v
	 *	@param r ���[�_�B�t�@�C���|�C���^�� type�̒���ɂȂ��Ă܂��B
	 *	@param sz r �̓Ǎ��݉\�T�C�Y�B
	 *	@retval NULL �^�C�v���قȂ�̂ŁA�ǂݍ��܂Ȃ������B
	 *	@retval NULL�ȊO r ����ǂݍ���Ń��R�[�h�R���e���c�� new �ō쐬�����|�C���^��Ԃ��B
	 */
	virtual CShapeFile::IRecordContent* LoadType(Integer type, const IReader& r, size_t sz)
	{
		return NULL;
	}

	/// Trace�_���v
	void Dump(void)
	{
	#ifdef _DEBUG
		TRACE0("-- ShapeFile��� --\n");
		TRACE1("ShapeType %d\n", m_shapeHead.shapeType);
		TRACE2("BoundingBox X min %g, max %g\n", m_shapeHead.bb.xMin, m_shapeHead.bb.xMax);
		TRACE2("BoundingBox Y min %g, max %g\n", m_shapeHead.bb.yMin, m_shapeHead.bb.yMax);
		TRACE2("BoundingBox Z min %g, max %g\n", m_shapeHead.bb.zMin, m_shapeHead.bb.zMax);
		TRACE2("BoundingBox M min %g, max %g\n", m_shapeHead.bb.mMin, m_shapeHead.bb.mMax);
		loop ( i, m_shapeRecords.GetSize() )
		{
			CStr s = m_shapeRecords[i]->ToString();
			TRACE2("record.%d, %s\n", i + 1, s);
		}
	#endif //_DEBUG
		_super::Dump();
	}

private:

	/// �t�@�C�����쐬
	void m_MakeFileName(CStr& _mainFn, CStr& _indexFn, CStr& _attrFn, LPCTSTR lpszBase) const
	{
		CStr base = lpszBase;
		base.TrimRight('.');
		base += '.';
		_mainFn = base + _T("shp");
		_indexFn = base + _T("shx");
		_attrFn = base + _T("dbf");
	}
	/**
	 * [�쐬] RAW�쐬.
	 *		�ێ����Ă���f�[�^��RAW(�t�@�C���ɏ������߂�f�[�^)�ɕϊ�
	 *	@param[out] _mainWt ���C���f�[�^
	 *	@param[out] _indexWt �C���f�b�N�X�f�[�^
	 */
	void m_SaveMain(IWriter& _mainWt, IWriter& _indexWt) const
	{
		CLikenMemToFile dmyWt;
		//== �w�b�_�����ɒǉ�
		THead hd = m_shapeHead;
		_mainWt.Write(sizeof(hd), &hd);
		_indexWt.Write(sizeof(hd), &hd);
		//== ���R�[�h
		loop ( i, m_shapeRecords.GetSize() )
		{
			// ���R�[�h�R���e���c�p��
			Integer type = m_shapeRecords[i]->GetShapeType();
			CBeInteger len = down_cast<CShapeFile::Integer>(sizeof(Integer) + m_shapeRecords[i]->Write(dmyWt)) / sizeof(WORD);
			// �C���f�b�N�X�f�[�^�쐬
			CBeInteger ofs = static_cast<long>(_mainWt.GetSize() / sizeof(WORD));
			_indexWt.Write(sizeof(ofs), &ofs);
			_indexWt.Write(sizeof(len), &len);
			// ���C���f�[�^�쐬
			CBeInteger no = down_cast<CShapeFile::Integer>(i + 1);
			_mainWt.Write(sizeof(no), &no);
			_mainWt.Write(sizeof(len), &len);
			_mainWt.Write(sizeof(type), &type);
			m_shapeRecords[i]->Write(_mainWt);
		}
		//== �w�b�_��t������
		hd.fileLength = static_cast<long>(_mainWt.GetSize() / sizeof(WORD));
		_mainWt.Seek(0);
		_mainWt.Write(sizeof(hd), &hd);
		hd.fileLength = static_cast<long>(_indexWt.GetSize() / sizeof(WORD));
		_indexWt.Seek(0);
		_indexWt.Write(sizeof(hd), &hd);
	}
	/**
	 * [���] ���C���f�[�^���
	 *	@param r ���C���f�[�^
	 */
	void m_LoadMain(const IReader& r)
	{
		THead hd;
		CPointerVectorT<IRecordContent> records; // ���R�[�h���
		r.Read(sizeof(THead), &hd);
		if ( hd.fileCode != 9994  || hd.version != 1000 )
		{
			throw CInvalidParamException();
		}
		while ( r.GetLeftoverSize() > 0 )
		{
			CBeInteger no;
			r.Read(sizeof(no), &no);	//���R�[�h�ԍ�
			CBeInteger len;
			r.Read(sizeof(len), &len);	//�R���e���c��
			Integer type;
			r.Read(sizeof(type), &type);	//�V�F�[�v�^�C�v
			TRACE3("RecNo.%d, Len = %d, Type = %d\n", long(no), long(len), type);
			size_t sz = len * 2 - sizeof(type);
			//
			IRecordContent* P = LoadType(type, r, sz);
			if ( P != NULL )
			{
				records.Add(P);
			}
			else if ( type == 0 )
			{
				CNullShapeRecordContent rc;
				rc.Read(r, sz);
				records.Add(new CNullShapeRecordContent(rc));
			}
			else
			{
				// ���̑�
				CUnknownRecordContent rc(type);
				rc.Read(r, sz);
				records.Add(new CUnknownRecordContent(rc));
			}
		}
		m_shapeRecords = records;
		m_shapeHead = hd;
	}

	C_ASSERT ( sizeof(Integer) == 4 ); // �T�C�Y�`�F�b�N
	C_ASSERT ( sizeof(Double) == 8 ); // �T�C�Y�`�F�b�N
	C_ASSERT ( sizeof(CBeInteger) == 4 ); // �T�C�Y�`�F�b�N
	C_ASSERT ( sizeof(THead) == 100 ); // �T�C�Y�`�F�b�N

	THead							m_shapeHead;	///< �w�b�_ (�t�@�C�����͊Ǘ����Ȃ�)
	CPointerVectorT<IRecordContent> m_shapeRecords;	///< ���R�[�h���

	friend class CShapeFileTest;
};



/**@ingroup DBASE FILE
 * �V�F�[�v�t�@�C�� type Point �Ǘ��N���X
 *
 *	ESRI�W���p��������Ђ� �V�F�[�v�t�@�C���A�N�Z�X�N���X�ł��B
 *
 *	@note �V�F�[�v�^�C�v�P�i�|�C���g�V�F�[�v�j�p�̃N���X�ł��B
 *
 *	@see �g�p���@�� CShapeFile ���Q�ƁB
 *
 *	@par�K�v�t�@�C��
 *			TnbShapeFile.h
 * 
 *	@date 09/02/05 �V�K�쐬
 */
class CShapeFileTypePoint : public CShapeFile
{
	DEFSUPER(CShapeFile);
public:

	/**
	 * Point ���R�[�h�R���e���c
	 *
	 */
	class CPointRecordContent : public CShapeFile::IRecordContent
	{
	public:
		/**
		 * �R���X�g���N�^.
		 *	@param x X
		 *	@param y Y
		 */
		CPointRecordContent(Double x = 0, Double y = 0) : m_x(x), m_y(y)
		{
		}
		/**
		 * [�擾] X�擾.
		 *	@return X
		 */
		Double GetX(void) const { return m_x; }
		/**
		 * [�擾] Y�擾.
		 *	@return Y
		 */
		Double GetY(void) const { return m_y; }
		/**
		 * [�ݒ�] X,Y �ݒ�.
		 *	@param x X
		 *	@param y Y
		 */
		void Set(Double x, Double y) { m_x = x; m_y = y; }
		/**
		 * [�擾] �V�F�[�v�^�C�v�擾.
		 *	@return �V�F�[�v�^�C�v
		 */
		virtual Integer GetShapeType(void) const
		{
			return 1; //Point
		}
		/**
		 * [���o] ���o��
		 *		�ێ����Ă���f�[�^�������o���i�V�F�[�v�^�C�v�͑ΏۊO�j�B
		 *	@param[out] _w ���C�^�[
		 *	@return �����o���T�C�Y
		 */
		virtual size_t Write(IWriter& _w) const
		{
			_w.Write(sizeof(m_x), &m_x);
			_w.Write(sizeof(m_y), &m_y);
			return sizeof(Double) * 2;
		}
		/**
		 * [�Ǎ�] �Ǎ���
		 *		��͂��f�[�^��ێ����܂��B
		 *	@param  r ���[�_�i�V�F�[�v�^�C�v�̒�����|�C���g���Ă��邱�Ɓj
		 *	@param size �T�C�Y
		 */
		virtual void Read(const IReader& r, size_t size)
		{
			if ( size < sizeof(m_x) * 2 )
			{
				throw CInvalidLengthException();
			}
			r.ReadExactly(sizeof(m_x), &m_x);
			r.ReadExactly(sizeof(m_y), &m_y);
		}
		/**
		 * [�擾] ������擾.
		 *	@return ���̃I�u�W�F�N�g���ێ����Ă�����𕶎��񉻂������́B
		 */
		virtual CStr ToString(void) const
		{
			return CStr::Fmt(_T("Point x = %g, y = %g"), m_x, m_y);
		}
	private:
		Double	m_x;	///< X���W�l
		Double	m_y;	///< Y���W�l
	};


	//------------------------------


	/// �R���X�g���N�^
	CShapeFileTypePoint(void)
	{
		SetHeadShapeType(1);
	}

	/**
	 * [�ݒ�] �|�C���g�V�F�[�v�R���e���c�ݒ�.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@param x X���W
	 *	@param y Y���W
	 *	@retval NULL �C���f�b�N�X���͈͊O�B
	 *	@retval NULL�ȊO ���R�[�h�R���e���c�̎Q�ƁB
	 */
	bool SetPointShapeRecord(INDEX record, Double x, Double y)
	{
		CPointRecordContent* P = new CPointRecordContent(x, y);
		if ( ! SetShapeRecord(record, P) )
		{
			delete P;
			return false;
		}
		SetHeadShapeType(1);
		return true;
	}

	/**
	 * [�擾] �V�F�[�v�R���e���c�擾.
	 *	@param record ���R�[�h�C���f�b�N�X
	 *	@retval NULL �C���f�b�N�X���͈͊O�B
	 *	@retval NULL�ȊO ���R�[�h�R���e���c�̎Q�ƁB
	 */
	const CPointRecordContent* GetPointShapeRecord(INDEX record) const
	{
		const IRecordContent* P = GetShapeRecord(record);
		if ( P != NULL && P->GetShapeType() == 1 )
		{
			return static_cast<const CPointRecordContent*>(P);
		}
		return NULL;
	}

protected:

	/**
	 * [�ʒm] �^�C�v�ʓǍ��ݏ���
	 *	@param type �^�C�v
	 *	@param r ���[�_�B�t�@�C���|�C���^�� type�̒���ɂȂ��Ă܂��B
	 *	@param sz r �̓Ǎ��݉\�T�C�Y�B
	 *	@retval NULL �^�C�v���قȂ�̂ŁA�ǂݍ��܂Ȃ������B
	 *	@retval NULL�ȊO r ����ǂݍ���Ń��R�[�h�R���e���c�� new �ō쐬�����|�C���^��Ԃ��B
	 */
	virtual CShapeFile::IRecordContent* LoadType(Integer type, const IReader& r, size_t sz)
	{
		if ( type == 1 )
		{
			CPointRecordContent rc;
			rc.Read(r, sz);
			return new CPointRecordContent(rc);
		}
		return NULL;
	}
};



}; // TNB

