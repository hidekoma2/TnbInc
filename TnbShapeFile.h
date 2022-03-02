#pragma once
/**
 *@file
 * ESRIジャパン株式会社の シェープファイルアクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerVector.h"
#include "TnbLikenMemToFile.h"
#include "TnbDBaseFile.h"



//TNB Library
namespace TNB
{



/**@ingroup DBASE FILE
 * シェープファイル管理クラス
 *
 *	 ESRIジャパン株式会社の シェープファイルアクセスクラスです。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		// ポイントタイプのシェープファイルクラス宣言
 *		CShapeFileTypePoint sf;
 *
 *		// バウンディングボックスの値を設定
 *		CShapeFileTypePoint::TBoundingBox bb;
 *		bb.xMin = 0.01;
 *		bb.xMax = 100.0;
 *		sf.SetBoundingBox(bb);
 *		// 属性フィールドを作成。
 *		sf.AddCharacterField("LABEL", 16);	// 0; 名前			
 *		sf.AddDateField("DATE");			// 1; 日付
 *		sf.AddNumericField("LAT.", 8, 3);	// 2; 緯度
 *		sf.AddNumericField("LONG.", 8, 3);	// 3; 経度
 *		// レコード１を作成
 *		INDEX idx = sf.AddRecord();
 *		sf.SetPointShapeRecord(idx, 34.5, 56.1);
 *		sf.SetStringRecord(idx, 0, "OX株式会社");
 *		sf.SetDateRecord(idx, 1, CGrid::TDate(2009, 1, 1));
 *		sf.SetFloatRecord(idx, 2, 123.4);
 *		sf.SetFloatRecord(idx, 3, 32.6);
 *		// レコード２を作成
 *		idx = sf.AddRecord();
 *		sf.SetPointShapeRecord(idx, 12.3, 33.2);
 *		sf.SetStringRecord(idx, 0, "(株)予輪予輪");
 *		sf.SetDateRecord(idx, 1, CGrid::TDate(2010, 2, 16));
 *		sf.SetFloatRecord(idx, 2, 111.5);
 *		sf.SetFloatRecord(idx, 3, 5.1);
 *		// レコード３を作成
 *		idx = sf.AddRecord();
 *		// ファイル書き出し
 *		sf.SaveFile("c:\\eeeee");
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbShapeFile.h
 * 
 *	@date 09/02/05 新規作成
 */
class CShapeFile : public CDBaseFile
{
	DEFSUPER(CDBaseFile);
public:

	typedef long Integer;	///< 符号付き32bit整数(4bytes)
	typedef double Double;	///< 符号付き64bitIEEE浮動小数点(8bytes)

	#pragma pack(push)
	#pragma pack(1)

	/**
	 * ビックエンディアンINTEGER.
	 *		一つの INTEGER を持つクラスです。
	 *		メモリには、ビックエンディアンで格納します。
	 *	@note メモリイメージ＝ファイルイメージです。
	 */
	class CBeInteger
	{
	public:
		/**
		 * コンストラクタ.
		 *	@param l 初期値
		 */
		CBeInteger(Integer l = 0) : m_value(m_SwapEndian(l))
		{
		}
		/**
		 * [取得] 値取得
		 *	@return 値　
		 */
		operator Integer() const 
		{
			return m_SwapEndian(m_value);
		}
		/**
		 * [設定] 値設定
		 *	@param l 値
		 *	@return 値
		 */
		Integer operator=(Integer l)
		{
			m_value = m_SwapEndian(l);
			return l;
		}
	private:
		// エンディアン変換
		Integer m_SwapEndian(Integer l) const
		{
			return SwapEndian(DWORD(l));
		}
		Integer	m_value;	///< 値(ビックエンディアンで格納)
	};

	/**
	 * バウンディングボックス型.
	 *	@note メモリイメージ＝ファイルイメージです。
	 */
	struct TBoundingBox
	{
		Double		xMin;	///< Bounding Box X 最小
		Double		yMin;	///< Bounding Box Y 最小
		Double		xMax;	///< Bounding Box X 最大
		Double		yMax;	///< Bounding Box Y 最大
		Double		zMin;	///< Bounding Box Z 最小
		Double		zMax;	///< Bounding Box Z 最大
		Double		mMin;	///< Bounding Box M 最小
		Double		mMax;	///< Bounding Box M 最大
		/// コンストラクタ
		TBoundingBox(void)
		{
			Zero(*this);
		}
	};

	/**
	 * ファイルヘッダ型.
	 *	@note メモリイメージ＝ファイルイメージです。
	 *	@note 一つのメインファイル内に、ヘッダは１つだけです。
	 */
	struct THead
	{
		CBeInteger		fileCode;	///< ファイルコード
		CBeInteger		padding[5];	///< 未使用
		CBeInteger		fileLength;	///< ファイル長（単位 word）
		Integer			version;	///< バージョン
		Integer			shapeType;	///< シェープタイプ
		TBoundingBox	bb;			///< バウンディングボックス
		/// コンストラクタ
		THead(void) : fileCode(9994), version(1000), shapeType(0)
		{
		}
	};

	#pragma pack(pop)

	/**
	 * レコードコンテンツ抽象クラス.
	 *
	 *	@note 一つのメインファイル内に、レコードは複数あります。
	 *	@note 本クラスではレコード番号を管理しません。
	 */
	struct IRecordContent
	{
		/// デストラクタ
		virtual ~IRecordContent(void) {}
		/**
		 * [取得] シェープタイプ取得.
		 *	@return シェープタイプ
		 */
		virtual Integer GetShapeType(void) const = 0;
		/**
		 * [書出] 書出し
		 *		保持しているデータを書き出し（シェープタイプは対象外）。
		 *	@param[out] _w ライター
		 *	@return 書き出しサイズ
		 */
		virtual size_t Write(IWriter& _w) const = 0;
		/**
		 * [読込] 読込み
		 *		解析しデータを保持します。
		 *	@param  r リーダ（シェープタイプの直後をポイントしていること）
		 *	@param size サイズ
		 */
		virtual void Read(const IReader& r, size_t size) = 0;
		/**
		 * [取得] 文字列取得.
		 *	@return このオブジェクトが保持している情報を文字列化したもの。
		 */
		virtual CStr ToString(void) const = 0;
	};

	/**
	 * NULL SHAPE レコードコンテンツ
	 *
	 */
	class CNullShapeRecordContent : public IRecordContent
	{
	public:
		/**
		 * [取得] シェープタイプ取得.
		 *	@return シェープタイプ
		 */
		virtual Integer GetShapeType(void) const
		{
			return 0; //Null Shape
		}
		/**
		 * [書出] 書出し
		 *		保持しているデータを書き出し（シェープタイプは対象外）。
		 *	@param[out] _w ライター
		 *	@return 書き出しサイズ
		 */
		virtual size_t Write(IWriter& _w) const
		{
			return 0;
		}
		/**
		 * [読込] 読込み
		 *		解析しデータを保持します。
		 *	@param  r リーダ（シェープタイプの直後をポイントしていること）
		 *	@param size サイズ
		 */
		virtual void Read(const IReader& r, size_t size)
		{
		}
		/**
		 * [取得] 文字列取得.
		 *	@return このオブジェクトが保持している情報を文字列化したもの。
		 */
		virtual CStr ToString(void) const
		{
			return _T("Null Shape");
		}
	};

	/**
	 * 不明 レコードコンテンツ
	 *
	 */
	class CUnknownRecordContent : public IRecordContent
	{
	public:
		/**
		 * コンストラクタ
		 *	@param st シェープタイプ
		 */
		CUnknownRecordContent(Integer st) : m_type(st)
		{
		}
		/**
		 * [参照] データ
		 *	@return データの参照
		 */
		CByteVector& Refer(void)
		{
			return m_data;
		}
		/**
		 * [取得] シェープタイプ取得.
		 *	@return シェープタイプ
		 */
		virtual Integer GetShapeType(void) const
		{
			return m_type;
		}
		/**
		 * [書出] 書出し
		 *		保持しているデータを書き出し（シェープタイプは対象外）。
		 *	@param[out] _w ライター
		 *	@return 書き出しサイズ
		 */
		virtual size_t Write(IWriter& _w) const
		{
			_w.WriteFrom(m_data);
			return m_data.GetSize();
		}
		/**
		 * [読込] 読込み
		 *		解析しデータを保持します。
		 *	@param  r リーダ（シェープタイプの直後をポイントしていること）
		 *	@param size サイズ
		 */
		virtual void Read(const IReader& r, size_t size)
		{
			m_data = r.ReadExactly(size);
		}
		/**
		 * [取得] 文字列取得.
		 *	@return このオブジェクトが保持している情報を文字列化したもの。
		 */
		virtual CStr ToString(void) const
		{
			return CStr::Fmt(_T("ShapeType.%d , len = %d"), m_type, m_data.GetSize());
		}
	private:
		Integer		m_type;		///< シェープタイプ
		CByteVector	m_data;		///< データ
	};


	//--------


	/// コンストラクタ
	CShapeFile(void)
	{
	}

	/**
	 * [設定] ヘッダシェープタイプ設定.
	 *	@param type シェープタイプ
	 */
	void SetHeadShapeType(Integer type)
	{
		m_shapeHead.shapeType = type;
	}

	/**
	 * [取得] ヘッダシェープタイプ取得.
	 *	@return type シェープタイプ
	 */
	Integer GetHeadShapeType(void) const
	{
		return m_shapeHead.shapeType;
	}

	/**
	 * [設定] バウンディングボックス設定.
	 *	@param bb 情報
	 */
	void SetBoundingBox(const TBoundingBox& bb)
	{
		m_shapeHead.bb = bb;
	}

	/**
	 * [取得] バウンディングボックス取得.
	 *	@return 情報
	 */
	const TBoundingBox& GetBoundingBox(void) const
	{
		return m_shapeHead.bb;
	}

	/**
	 * [削除] レコード全削除.
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool AllDeleteRecord(void)
	{
		m_shapeRecords.RemoveAll();
		return _super::AllDeleteRecord();
	}

	/**
	 * [追加] レコード追加.
	 *	@note 追加したレコードのシェイプコンテンツは「Null Shape」になっています。
	 *	@note 追加したレコードの属性の全フィールドはNULLになっています。
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したレコードインデックス。
	 *	@retval INVALID_INDEX レコード追加失敗
	 */
	INDEX AddRecord(void)
	{
		INDEX r1 = _super::AddRecord();
		INDEX r2 = m_shapeRecords.Add(new CNullShapeRecordContent());
		ASSERT( r1 == r2 );
		return r1;
	}

	/**
	 * [挿入] レコード挿入.
	 *	@note 挿入追加したレコードの全フィールドはNULLになっています。
	 *	@param record レコードインデックス。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool InsertRecord(INDEX record)
	{
		bool r1 = _super::InsertRecord(record);
		bool r2 = m_shapeRecords.Insert(record, new CNullShapeRecordContent());
		ASSERT( r1 == r2 );
		return r1;
	}

	/**
	 * [削除] レコード削除.
	 *	@param record レコードインデックス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool DeleteRecord(INDEX record)
	{
		bool r1 = _super::DeleteRecord(record);
		bool r2 = m_shapeRecords.Remove(record);
		ASSERT( r1 == r2 );
		return r1;
	}

	/**
	 * [設定] シェープコンテンツ設定.
	 *	@param record レコードインデックス
	 *	@param P レコードコンテンツ。必ず new で作成したオブジェクトであること。
	 *	@retval NULL インデックスが範囲外。
	 *	@retval NULL以外 レコードコンテンツの参照。
	 */
	bool SetShapeRecord(INDEX record, IRecordContent* P)
	{
		return m_shapeRecords.Set(record, P);
	}

	/**
	 * [取得] シェープコンテンツ取得.
	 *	@param record レコードインデックス
	 *	@retval NULL インデックスが範囲外。
	 *	@retval NULL以外 レコードコンテンツの参照。
	 */
	const IRecordContent* GetShapeRecord(INDEX record) const
	{
		return m_shapeRecords.Get(record);
	}

	/**
	 * [読込] ファイル読込み
	 *		保持している情報を破棄し、ファイルから読込みます。
	 *	@note *.shp , *.shx , *.dbf から読み込みます。
	 *	@param lpszBaseFileName ファイル名。拡張子はつけないでください。
	 *	@param withTypeCheck trueの場合、現在の GetHeadShapeType() で得られるタイプと異なるファイルはエラーになります。
	 *	@retval true 成功
	 *	@retval false 失敗
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
					TRACE2(" メインファイルのタイプ[%d]と現在のタイプ[%d]が一致していません！\n", GetHeadShapeType(), oldType);
					SetHeadShapeType(oldType);
					_super::AllDeleteRecord();
					r = false;
				}
				else if ( _super::GetRecordCount() != m_shapeRecords.GetSize() )
				{
					TRACE0(" メインファイルと属性ファイルのレコードの数が一致していません！\n" );
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
	 * [作成] ファイル作成.
	 *		保持しているデータをファイルに書き出し.
	 *	@note *.shp , *.shx , *.dbf へ書き出します。
	 *	@param lpszBaseFileName ファイル名。拡張子はつけないでください。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [通知] タイプ別読込み処理
	 *	@param type タイプ
	 *	@param r リーダ。ファイルポインタは typeの直後になってます。
	 *	@param sz r の読込み可能サイズ。
	 *	@retval NULL タイプが異なるので、読み込まなかった。
	 *	@retval NULL以外 r から読み込んでレコードコンテンツを new で作成したポインタを返す。
	 */
	virtual CShapeFile::IRecordContent* LoadType(Integer type, const IReader& r, size_t sz)
	{
		return NULL;
	}

	/// Traceダンプ
	void Dump(void)
	{
	#ifdef _DEBUG
		TRACE0("-- ShapeFile情報 --\n");
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

	/// ファイル名作成
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
	 * [作成] RAW作成.
	 *		保持しているデータをRAW(ファイルに書き込めるデータ)に変換
	 *	@param[out] _mainWt メインデータ
	 *	@param[out] _indexWt インデックスデータ
	 */
	void m_SaveMain(IWriter& _mainWt, IWriter& _indexWt) const
	{
		CLikenMemToFile dmyWt;
		//== ヘッダを仮に追加
		THead hd = m_shapeHead;
		_mainWt.Write(sizeof(hd), &hd);
		_indexWt.Write(sizeof(hd), &hd);
		//== レコード
		loop ( i, m_shapeRecords.GetSize() )
		{
			// レコードコンテンツ用意
			Integer type = m_shapeRecords[i]->GetShapeType();
			CBeInteger len = down_cast<CShapeFile::Integer>(sizeof(Integer) + m_shapeRecords[i]->Write(dmyWt)) / sizeof(WORD);
			// インデックスデータ作成
			CBeInteger ofs = static_cast<long>(_mainWt.GetSize() / sizeof(WORD));
			_indexWt.Write(sizeof(ofs), &ofs);
			_indexWt.Write(sizeof(len), &len);
			// メインデータ作成
			CBeInteger no = down_cast<CShapeFile::Integer>(i + 1);
			_mainWt.Write(sizeof(no), &no);
			_mainWt.Write(sizeof(len), &len);
			_mainWt.Write(sizeof(type), &type);
			m_shapeRecords[i]->Write(_mainWt);
		}
		//== ヘッダを付加する
		hd.fileLength = static_cast<long>(_mainWt.GetSize() / sizeof(WORD));
		_mainWt.Seek(0);
		_mainWt.Write(sizeof(hd), &hd);
		hd.fileLength = static_cast<long>(_indexWt.GetSize() / sizeof(WORD));
		_indexWt.Seek(0);
		_indexWt.Write(sizeof(hd), &hd);
	}
	/**
	 * [解析] メインデータ解析
	 *	@param r メインデータ
	 */
	void m_LoadMain(const IReader& r)
	{
		THead hd;
		CPointerVectorT<IRecordContent> records; // レコード情報
		r.Read(sizeof(THead), &hd);
		if ( hd.fileCode != 9994  || hd.version != 1000 )
		{
			throw CInvalidParamException();
		}
		while ( r.GetLeftoverSize() > 0 )
		{
			CBeInteger no;
			r.Read(sizeof(no), &no);	//レコード番号
			CBeInteger len;
			r.Read(sizeof(len), &len);	//コンテンツ長
			Integer type;
			r.Read(sizeof(type), &type);	//シェープタイプ
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
				// その他
				CUnknownRecordContent rc(type);
				rc.Read(r, sz);
				records.Add(new CUnknownRecordContent(rc));
			}
		}
		m_shapeRecords = records;
		m_shapeHead = hd;
	}

	C_ASSERT ( sizeof(Integer) == 4 ); // サイズチェック
	C_ASSERT ( sizeof(Double) == 8 ); // サイズチェック
	C_ASSERT ( sizeof(CBeInteger) == 4 ); // サイズチェック
	C_ASSERT ( sizeof(THead) == 100 ); // サイズチェック

	THead							m_shapeHead;	///< ヘッダ (ファイル長は管理しない)
	CPointerVectorT<IRecordContent> m_shapeRecords;	///< レコード情報

	friend class CShapeFileTest;
};



/**@ingroup DBASE FILE
 * シェープファイル type Point 管理クラス
 *
 *	ESRIジャパン株式会社の シェープファイルアクセスクラスです。
 *
 *	@note シェープタイプ１（ポイントシェープ）用のクラスです。
 *
 *	@see 使用方法は CShapeFile を参照。
 *
 *	@par必要ファイル
 *			TnbShapeFile.h
 * 
 *	@date 09/02/05 新規作成
 */
class CShapeFileTypePoint : public CShapeFile
{
	DEFSUPER(CShapeFile);
public:

	/**
	 * Point レコードコンテンツ
	 *
	 */
	class CPointRecordContent : public CShapeFile::IRecordContent
	{
	public:
		/**
		 * コンストラクタ.
		 *	@param x X
		 *	@param y Y
		 */
		CPointRecordContent(Double x = 0, Double y = 0) : m_x(x), m_y(y)
		{
		}
		/**
		 * [取得] X取得.
		 *	@return X
		 */
		Double GetX(void) const { return m_x; }
		/**
		 * [取得] Y取得.
		 *	@return Y
		 */
		Double GetY(void) const { return m_y; }
		/**
		 * [設定] X,Y 設定.
		 *	@param x X
		 *	@param y Y
		 */
		void Set(Double x, Double y) { m_x = x; m_y = y; }
		/**
		 * [取得] シェープタイプ取得.
		 *	@return シェープタイプ
		 */
		virtual Integer GetShapeType(void) const
		{
			return 1; //Point
		}
		/**
		 * [書出] 書出し
		 *		保持しているデータを書き出し（シェープタイプは対象外）。
		 *	@param[out] _w ライター
		 *	@return 書き出しサイズ
		 */
		virtual size_t Write(IWriter& _w) const
		{
			_w.Write(sizeof(m_x), &m_x);
			_w.Write(sizeof(m_y), &m_y);
			return sizeof(Double) * 2;
		}
		/**
		 * [読込] 読込み
		 *		解析しデータを保持します。
		 *	@param  r リーダ（シェープタイプの直後をポイントしていること）
		 *	@param size サイズ
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
		 * [取得] 文字列取得.
		 *	@return このオブジェクトが保持している情報を文字列化したもの。
		 */
		virtual CStr ToString(void) const
		{
			return CStr::Fmt(_T("Point x = %g, y = %g"), m_x, m_y);
		}
	private:
		Double	m_x;	///< X座標値
		Double	m_y;	///< Y座標値
	};


	//------------------------------


	/// コンストラクタ
	CShapeFileTypePoint(void)
	{
		SetHeadShapeType(1);
	}

	/**
	 * [設定] ポイントシェープコンテンツ設定.
	 *	@param record レコードインデックス
	 *	@param x X座標
	 *	@param y Y座標
	 *	@retval NULL インデックスが範囲外。
	 *	@retval NULL以外 レコードコンテンツの参照。
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
	 * [取得] シェープコンテンツ取得.
	 *	@param record レコードインデックス
	 *	@retval NULL インデックスが範囲外。
	 *	@retval NULL以外 レコードコンテンツの参照。
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
	 * [通知] タイプ別読込み処理
	 *	@param type タイプ
	 *	@param r リーダ。ファイルポインタは typeの直後になってます。
	 *	@param sz r の読込み可能サイズ。
	 *	@retval NULL タイプが異なるので、読み込まなかった。
	 *	@retval NULL以外 r から読み込んでレコードコンテンツを new で作成したポインタを返す。
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

