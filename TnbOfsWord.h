#pragma once
/**
 *@file
 * Office WORD処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"

#error


#include <comutil.h>
#ifndef _TnbDOXYGEN	//Document作成用シンボル
//Wordを操作するためのタイプライブラリを読みこむ(Word2000用)
//#import "C:\Program Files\Microsoft Office\Office\Mso9.dll" no_namespace rename("DocumentProperties", "DocumentPropertiesDOC")   
#import "C:\Program Files\Microsoft Office\Office\MSO9.DLL"
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
#import "C:\Program Files\Microsoft Office\Office\Msword9.olb" rename("ExitWindows", "ExitWindowsDOC")
#endif // _TnbDOXYGEN


//TNB Library
namespace TNB
{



class CWordControl
{
public:
	void Test(void)
	{
		HRESULT hRes = ::CoInitialize(NULL);
		using namespace Word;
		_ApplicationPtr pDOC;

		if (0)
		{
			//Wordの起動
			pDOC.CreateInstance(L"Word.Application");
			pDOC->Visible = TRUE;

			//新規 Word document を追加する
			DocumentsPtr pDocments = pDOC->GetDocuments();
			_DocumentPtr pDocment = pDocments->Add();

//			::MessageBox(NULL,"動作確認のために一時停止","",MB_OK);

			//Wordの終了
			pDOC->Quit();
			pDOC = NULL;
		}

		_variant_t v;

		try
		{
			 //Wordの起動
			pDOC.CreateInstance(L"Word.Application");
			pDOC->Visible = TRUE;
	
			//既存の Word document を開く
			DocumentsPtr pDocuments = pDOC->GetDocuments();
//			oleFName.SetString(_T("c:\\ななな.doc"),VT_BSTR);
//			_DocumentPtr pDocment  = pDocments->OpenOld(oleFName);
			v.SetString("c:\\ななな.doc");
			_DocumentPtr pDocument  = pDocuments->Open(&v);

//			::MessageBox(NULL,"動作確認のために一時停止","",MB_OK);


			pDOC->ActiveDocument->Content->Text = "aaaaaaaaaaaaaaaa\nbbbbbbbbbbbbbbb";

			pDocument->Application->Selection->Range->Text = "TEST";

			pDocument->Range(&_variant_t((short)2), &_variant_t((short)6))->Text = "TEST";

			//Tableを作成
			TablePtr pTable = pDOC->ActiveDocument->Tables->Add(pDocument->Range(),5,6);

			//Tableへ文字を出力
			pTable->Cell(2,2)->Range->Text = "Test Data";

			//Wordの終了
			pDOC->Quit();
		}
		catch(_com_error& e)
		{
			TNB::IgnoreUnusedValue(e);
			CString s;
		}
		catch(...)
		{
			CString s;
		}

	}
};



}; // TNB