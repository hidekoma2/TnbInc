#pragma once
/**
 *@file
 * Office EXCEL処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"
#include "TnbFilePathManager.h"
#include "TnbStr.h"

#include <comutil.h>
#ifndef _TnbDOXYGEN	//Document作成用シンボル
//Excelを操作するためのタイプライブラリを読みこむ(Excel2007用)
//#import "C:\Program Files (x86)\Common Files\Microsoft Shared\OFFICE14\MSO.DLL"
//#import "C:\Program Files (x86)\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
//Excelを操作するためのタイプライブラリを読みこむ(Excel2019用)
#import "C:\Program Files (x86)\Microsoft Office\Root\VFS\ProgramFilesCommonX86\Microsoft Shared\OFFICE16\MSO.DLL"
#import "C:\Program Files (x86)\Microsoft Office\Root\VFS\ProgramFilesCommonX86\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
#import "C:\Program Files (x86)\Microsoft Office\root\Office16\excel.exe" rename("DialogBox", "DialogBoxForExcel") rename( "RGB", "RGBForExcel") 
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * Microsoft Office Excel コントロール補助
 *	@note プロジェクトで ::CoInitialize(NULL); を実行しておいてください。
 */
class CExcelControl
{
public:

	/// デストラクタ
	~CExcelControl(void)
	{
		Close(false);
	}

	/**
	 * [設定] オープン.
	 *	@param lpszXlsxFile xlsファイル名。相対パスの場合EXEからの相対パスになります。
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Open(LPCTSTR lpszXlsxFile)
	{
		CFilePathManager fpm;
		HRESULT r = m_pApp.CreateInstance( "Excel.Application");
		if ( m_pApp == NULL )
		{
			return false;
		}
		try
		{
			CUnicode fn = fpm.GetCanonicalize(lpszXlsxFile);
			m_pWorkbook = m_pApp->Workbooks->Open(fn.ReferBuffer());
		}
		catch( ... )
		{
			m_pApp->Quit();				//非表示の場合、このタイミングではインスタンスは消えない。
			// インスタンスを開放
			m_pApp.Release();				//非表示の場合、ここでexcel.exeインスタンスもなくなる。
			return false;
		}
		return true;
	}

	/**
	 * [設定] クローズ.
	 *	@param isSave true なら保存して終了します。
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Close(bool isSave = true)
	{
		if ( m_pApp != NULL )
		{
			try
			{
				if ( isSave )
				{
					m_pWorkbook->Save();
				}
				m_pWorkbook->Close();
				m_pRange.Release();
				m_pWorkbook.Release();
				m_pSheet.Release();
				m_pApp->Quit();				//非表示の場合、このタイミングではインスタンスは消えない。
				// インスタンスを開放
				m_pApp.Release();				//非表示の場合、ここでexcel.exeインスタンスもなくなる。
			}
			catch( ... )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [設定] カレントワークシート選択.
	 *	@param lpszSheetName シート名
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool SelectWorksheet(LPCTSTR lpszSheetName)
	{
		try
		{
			m_pSheet = m_pWorkbook->Sheets->Item[CUnicode(lpszSheetName).ReferBuffer()];
			return m_pSheet != NULL;
		}
		catch( ... )
		{
			//m_pSheet.Release();
			return false;
		}
	}

	/**
	 * [設定] カレントセル名選択.
	 *	@param lpszCellName セル名
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool SelectCell(LPCTSTR lpszCellName)
	{
		try
		{
			COleVariant varCell;
			varCell = CUnicode(lpszCellName).ReferBuffer();
			m_pRange = m_pSheet->GetRange( varCell);
		}
		catch( ... )
		{
			m_pRange.Release();
			return false;
		}
		return m_pRange != NULL;
	}

	/**
	 * [取得] レンジ取得.
	 *	@return レンジ
	 */
	Excel::RangePtr GetRange(void)
	{
		return m_pRange;
	}

	/**
	 * [取得] ボーダー取得.
	 *	@return ボーダー
	 */
	Excel::BorderPtr GetBorder(Excel::XlBordersIndex index)
	{
		return m_pRange->GetBorders()->GetItem(index);
	}

	/**
	 * [作成] カラム文字生成.
	 *	@param idx カラム番号
	 *	@return カラム文字
	 */
	static CStr ColunmChar(INDEX idx)
	{
		TCHAR buf[3] = {0};
		INDEX i = idx % 26;
		INDEX j = idx / 26;
		if ( j == 0 )
		{
			buf[0] = 'A' + i;
		}
		else
		{
			buf[0] = 'A' + (j - 1);
			buf[1] = 'A' + i;
		}
		return buf;
	}

private:
	Excel::_ApplicationPtr	m_pApp;			///< アプリインスタンス
	Excel::_WorkbookPtr		m_pWorkbook;	///< ワークブック
	Excel::_WorksheetPtr	m_pSheet;		///< シート
	Excel::RangePtr			m_pRange;		///< 範囲ポインタ
public:
	void test(void)
	{
		// COMの初期化
		::CoInitialize(NULL);

		////////////////////
		// Excelを起動
		Excel::_ApplicationPtr pApp;

		// インスタンスを作成
		pApp.CreateInstance( "Excel.Application");
		// Excelを表示する
//		pApp->PutVisible( 0, TRUE);			//TRUEにするとインスタンスが残る

 
		// WorkBookを追加する
//		pApp->Workbooks->Add();

        Excel::_WorkbookPtr pWorkbook;
		try
		{
			pWorkbook = pApp->Workbooks->Open(L"C:\\Home\\Temp\\DDDDD\\Test\\Debug\\test.xlsx");		//フルパスで渡すこと！
		}
		catch( ... )
		{
			pApp->Quit();				//非表示の場合、このタイミングではインスタンスは消えない。
			// インスタンスを開放
			pApp.Release();				//非表示の場合、ここでexcel.exeインスタンスもなくなる。
			::CoUninitialize();
			return;
		}
		// アクティブなSheetを取得
//		Excel::_WorksheetPtr pSheet;
//		pSheet = pApp->GetActiveSheet();

		Excel::_WorksheetPtr pSheet = pWorkbook->Sheets->Item[L"Sheet2"];

		/////////////////////////
		// Cellに文字を書き込む
		COleVariant varCell;
		varCell = L"B2";
		Excel::RangePtr pRang;
		pRang = pSheet->GetRange( varCell);
		pRang->NumberFormat = "@";

		pRang->Value2 = "=A1+B1";


		Excel::BorderPtr border;
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeTop );
		border->LineStyle = (long)Excel::xlContinuous;
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeBottom);
		border->PutLineStyle((long)Excel::xlContinuous);
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeLeft);
		border->LineStyle = (long)Excel::xlContinuous;
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeRight);
		border->LineStyle = (long)Excel::xlContinuous;
		border->PutWeight((long)Excel::xlMedium);

		pRang->Interior->Color = (long)RGB( 255, 0, 0 );

		varCell = pRang->Value;				//計算結果が取得できる。式ではない。
	

		//////////////////////////////////
		// Cellにハイパー
		// リンクを書き込む
		varCell = "C2";
//		pRang = pSheet->GetRange( varCell);
		pRang = pSheet->Range[varCell];
		_bstr_t bstrURL = "http://homepage1.nifty.com/goldfish";
		pSheet->Hyperlinks->Add( pRang, bstrURL);
		

		pWorkbook->Save();
		pWorkbook->Close();
	// インスタンスを開放
		pRang.Release();
		pWorkbook.Release();
		pSheet.Release();

		pApp->Quit();				//非表示の場合、このタイミングではインスタンスは消えない。
		// インスタンスを開放
		pApp.Release();				//非表示の場合、ここでexcel.exeインスタンスもなくなる。

		::CoUninitialize();

	}

};



}; // TNB