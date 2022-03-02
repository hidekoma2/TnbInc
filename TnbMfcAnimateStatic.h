#pragma once
/**
 *@file
 * �A�j���\���֌W�̃w�b�_
 *
 *		�A�j����\�����邽�߂̃R���g���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapAnimater.h"
#include "TnbMfcRgnControl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL ANIMATE
 * �A�j���\���R���g���[���N���X
 *
 *	@note Disable���A�A�j�����ꎞ��~���A Enable���ɍĊJ���܂��B
 *	@note Hide���A�A�j�����~���AShow���A������Đ����܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		// �_�C�A���O�̃w�b�_�A�_�C�A���O�N���X�̃����o�B
 *		CAnimateStatic m_anime;
 *
 *		// OnInitDialog() ��
 *		m_anime.SetFirstFrame("1.bmp", CPenetrateWiper());
 *		m_anime.AddBaseFrame("1.bmp", 5);
 *		m_anime.AddBaseFrame("2.bmp", 5);
 *		m_anime.AddBaseFrame("3.bmp", 5);
 *		m_anime.AddBaseFrame("4.bmp", 5);
 *		ASSERT( anime.GetLength() == 21 ); 
 *		m_anime.SetAnimationParameter(false, 1000 / 21, false, true);
 *
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcAnimateStatic.h
 * 
 *	@date 07/11/14 �V�K�쐬
 *	@date 07/11/30 CBitmapAnimater �g�p����悤�ɕύX�B
 *	@date 07/12/04 HIDE���ADisable���ɁA�A�j����~����悤�ɕύX�B
 *	@date 07/12/12 ���[�W�����w����\�ɂ����B
 *	@date 09/04/15 SetAnimationParameter() �� StartAnimation()�ɉ����B
 */
class CAnimateStatic : public CRgnControlAddinT<CWnd>, public CBitmapAnimater
{
	DEFSUPER(CRgnControlAddinT<CWnd>);
	typedef CBitmapAnimater _animater;
public:

	/// �R���X�g���N�^
	CAnimateStatic(void) 
		: _super(), m_interval(1000), m_nowIndex(0), m_mode(0), m_useCache(false), m_isStopInDrag(false), m_doAnimate(false)
	{
	}

	/**
	 * [�ݒ�] �A�j���ݒ�.
	 *	@note �ݒ肵�A�A�j�����J�n���܂��B
	 *	@param mode true �Ȃ琳���A�~�����J��Ԃ��܂��B false �Ȃ琳�����J��Ԃ��܂��B
	 *	@param interval �w�莞��(ms)���ƂɃt���[�������������܂��B
	 *	@param useCache �ȗ��� false �Ȃ�A�L���b�V���g���܂���Btrue �Ȃ�L���b�V�����g�p���܂��B\n
	 *					���L���b�V���̓������͎g���܂����A�����͌y���Ȃ�܂��B
	 *	@param isStopInDrag �ȗ��� false �Ȃ�A�h���b�N�����A�j���[�V�������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool StartAnimation(bool mode, DWORD interval, bool useCache = false, bool isStopInDrag = false)
	{
		m_Stop();
		if ( _animater::GetLength() > 0 )
		{
			m_mode = mode;
			m_interval = interval;
			m_useCache = useCache;
			m_isStopInDrag = isStopInDrag;
			_animater::FlashCache();
			//
			m_nowIndex = 0;
			m_nowBitmap.Null();
			m_doAnimate = true;
			m_Start();
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �A�j����~
	 */
	void StopAnimation(void)
	{
		m_doAnimate = false;
		m_Stop();
	}

	/**
	 * [�ݒ�] �ŏ��̃t���[���摜�ݒ�
	 *	@note ��������s����ƁA���܂ł̏��̓N���A����܂��B
	 *	@param bmp �r�b�g�}�b�v
	 *	@param defaultWiper AddBaseFrame() �Ń��C�p�[���ȗ����ꂽ���Ɏg�p����郏�C�p�[�B
	 *				�������L�����܂��̂ŁA�j�����Ă��܂��܂���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetFirstFrame(CBitmapHandle bmp, const IBitmapWipable& defaultWiper)
	{
		bool r = _animater::SetFirstFrame(bmp, defaultWiper);
		m_Resize();
		return r;
	}

	/**
	 * [�ݒ�] �ŏ��̃t���[���摜�ݒ�
	 *	@note ��������s����ƁA���܂ł̏��̓N���A����܂��B
	 *	@param bmp �r�b�g�}�b�v
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetFirstFrame(CBitmapHandle bmp)
	{
		bool r = _animater::SetFirstFrame(bmp);
		m_Resize();
		return r;
	}

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/* ����
	 * �A�j���J�n�B StartAnimation() ���g���Ă��������B
	 */
	bool SetAnimationParameter(bool mode, DWORD interval, bool useCache = false, bool isStopInDrag = false)
	{
		return StartAnimation(mode, interval, useCache, isStopInDrag);
	}
	#endif

protected:

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			if ( _animater::GetLength() != 0 )
			{
				return 0;
			}
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDc = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				m_Draw(pDc);
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
				RECT rc;
				GetClientRect(&rc);
				if ( ::RectVisible(dc, &rc) )
				{
					m_Draw(&dc);
				}
			}
			return 0;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			(wParam != 0) ? m_Start() : m_Stop();
			return 0;
		case WM_SHOWWINDOW:
			TTRACE0("WM_SHOWWINDOW\n");
			if ( wParam != 0 )
			{
				m_nowIndex = 0;
				m_nowBitmap.Null();
				m_Start();
			}
			else
			{
				m_Stop();
			}
			return 0;
		case WM_TIMER:
			TTRACE0("WM_TIMER\n");
			if ( wParam == TIMERID_ANIMA )
			{
				if ( ! ::IsWindowEnabled(m_hWnd) )
				{
					m_Stop();
				}
				else if ( m_IsAnimation() )
				{
					if ( m_mode > 0 )
					{
						m_nowIndex++;
						if ( m_nowIndex >= _animater::GetLength() )
						{
							m_nowIndex = _animater::GetLength() - 2;
							m_mode = -1;
						}
					}
					else if ( m_mode < 0 )
					{
						if ( m_nowIndex > 0 )
						{
							m_nowIndex--;
						}
						else
						{
							m_nowIndex = 1;
							m_mode = 1;
						}
					}
					else 
					{
						m_nowIndex++;
						if ( m_nowIndex >= _animater::GetLength() )
						{
							m_nowIndex = 0;
						}
					}
					m_nowBitmap.Null();
//					RedrawWindow(NULL, NULL, RDW_INVALIDATE);
					RedrawWindow();
				}
			}
			break;
		case WM_LBUTTONDOWN:
			return 0;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::ModifyStyle(0, SS_NOTIFY);
		_super::PreSubclassWindow();
		m_Resize();
	}

private:
	enum { TIMERID_ANIMA = 10000 };
	DWORD					m_interval;		///< �\���ؑփ^�C�~���O(ms)
	bool					m_doAnimate;	///< �A�j������
	INDEX					m_nowIndex;		///< �\�����̃C���f�b�N�X
	CBitmapHandle			m_nowBitmap;	///< �\�����̊G
	int						m_mode;			///< 0;�����̂� -1;�~���� 1;������
	bool					m_useCache;		///< true;�L���b�V������
	bool					m_isStopInDrag;	///< true;�h���b�N���A�j����~
	/// ���T�C�Y
	void m_Resize(void)
	{
		if ( _animater::GetLength() == 0 ) { return; }
		SIZE size;
		if ( _animater::FrameAt(0).GetSize(size) )
		{
			MFCLIB::ChangeClientSize(this, size.cx, size.cy);
		}
	}
	/// �`��.
	void m_Draw(CDC* pDC)
	{
		if ( m_nowBitmap.IsNull() && _animater::GetLength() != 0 )
		{
			m_nowBitmap = m_useCache ? _animater::FrameAtWithCache(m_nowIndex): _animater::FrameAt(m_nowIndex);
		}
		if ( m_clientRgn != NULL )
		{
			MFCLIB::SelectClipRgnOrg(pDC, m_clientRgn);
		}
		if ( ! m_nowBitmap.IsNull() )
		{
			m_nowBitmap.Draw(*pDC);
		}
	}
	/// �A�j������H
	bool m_IsAnimation(void)
	{
		if ( m_isStopInDrag && ::GetCapture() != NULL )
		{
			return false;
		}
		return _animater::GetLength() > 1;
	}
	/// �A�j���J�n(�������A�����ɂ���Ă͊J�n���Ȃ�)
	void m_Start(void)
	{
		if ( m_doAnimate && ::IsWindow(m_hWnd) )
		{
			SetTimer(TIMERID_ANIMA, m_interval, NULL);
		}
	}
	/// �A�j����~
	void m_Stop(void)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			KillTimer(TIMERID_ANIMA);
		}
	}

};



}; //MFC
}; //TNB



