#pragma once
/**
 *@file
 * Office WORD�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"

#error


#include <comutil.h>
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
//Word�𑀍삷�邽�߂̃^�C�v���C�u������ǂ݂���(Word2000�p)
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
			//Word�̋N��
			pDOC.CreateInstance(L"Word.Application");
			pDOC->Visible = TRUE;

			//�V�K Word document ��ǉ�����
			DocumentsPtr pDocments = pDOC->GetDocuments();
			_DocumentPtr pDocment = pDocments->Add();

//			::MessageBox(NULL,"����m�F�̂��߂Ɉꎞ��~","",MB_OK);

			//Word�̏I��
			pDOC->Quit();
			pDOC = NULL;
		}

		_variant_t v;

		try
		{
			 //Word�̋N��
			pDOC.CreateInstance(L"Word.Application");
			pDOC->Visible = TRUE;
	
			//������ Word document ���J��
			DocumentsPtr pDocuments = pDOC->GetDocuments();
//			oleFName.SetString(_T("c:\\�ȂȂ�.doc"),VT_BSTR);
//			_DocumentPtr pDocment  = pDocments->OpenOld(oleFName);
			v.SetString("c:\\�ȂȂ�.doc");
			_DocumentPtr pDocument  = pDocuments->Open(&v);

//			::MessageBox(NULL,"����m�F�̂��߂Ɉꎞ��~","",MB_OK);


			pDOC->ActiveDocument->Content->Text = "aaaaaaaaaaaaaaaa\nbbbbbbbbbbbbbbb";

			pDocument->Application->Selection->Range->Text = "TEST";

			pDocument->Range(&_variant_t((short)2), &_variant_t((short)6))->Text = "TEST";

			//Table���쐬
			TablePtr pTable = pDOC->ActiveDocument->Tables->Add(pDocument->Range(),5,6);

			//Table�֕������o��
			pTable->Cell(2,2)->Range->Text = "Test Data";

			//Word�̏I��
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