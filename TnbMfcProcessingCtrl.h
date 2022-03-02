#pragma once
/**
 *@file
 * �������\�L�R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbThread.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �������R���g���[��
 *
 *		inline �ȃR���g���[���ł��B
 *		���\�[�XEDITOR�ŃX�^�e�B�b�N�R���g���[���𒣂�ACStatic �Ɋ��蓖�Č�A
 *		�{�N���X�ɒu�������邱�Ƃ�z�肵�Ă��܂��B
 *
 *		�w��̃^�C�~���O�ŁA�}�[�J�[���E���������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcProcessingCtrl.h
 * 
 *	@date 06/07/20	�V�K�쐬
 *	@date 07/10/31	WM_PRINTCLIENT �Ή��B
 *	@date 07/11/16	IDrawable �Ή��B
 *	@date 11/08/31	�\�������B
 *	@date 12/03/27	ReferThreadStatus() �V�K�B
 */
class CProcessingCtrl : public CWnd , TNB::CThread::IRunner
{
	DEFSUPER(CWnd);
public:

	/// ���[�h
	enum EMode
	{
		Mode_GotoRight,	///< ������E��
		Mode_RoundTrip,	///< �E������
	};

	/// �R���X�g���N�^
	CProcessingCtrl(void) 
		: _super(), m_size(0, 0), m_colorMark(::GetSysColor(COLOR_HIGHLIGHT))
		, m_isActing(false), m_interval(10), m_step(1), m_colorBack(::GetSysColor(COLOR_BTNFACE))
		, m_markWidth(6), m_pos(0), m_mode(1)
	{
		m_thread.SetRunner(this);
	}

	/// �f�X�g���N�^
	~CProcessingCtrl(void)
	{
		m_thread.Stop();
		m_DeleteDrawer();
	}

	/**
	 * [�ݒ�] �����w��
 	 *	@param mode true �Ȃ�E���������܂��B false �Ȃ獶����E�ւ̈ړ��݂̂��J��Ԃ��܂��B
	 *	@param interval �ړ����x(ms)�B�w�莞�Ԗ��Ɉړ����������܂��B
	 *	@param step �}�[�N�̈ړ����B�ȗ�����ƂU�ł��B
	 *	@param colorBack �w�i�̐F
	 */
	void SetParameter(EMode mode, DWORD interval, int step = 6, COLORREF colorBack = CLR_INVALID)
	{
		m_mode = mode;
		m_interval = interval;
		m_step = step;
		m_colorBack = (colorBack != CLR_INVALID) ? colorBack : ::GetSysColor(COLOR_BTNFACE);
	}

	/**
	 * [�ݒ�] �}�[�J�[�w��.
	 *		�P���F�̃}�[�J�[���w�肵�܂��B
	 *	@param colorMark �}�[�N�̐F
	 *	@param width �}�[�N�̕�
	 */
	void SetMarker(COLORREF colorMark, int width)
	{
		Action(false);
		m_DeleteDrawer();
		m_colorMark = (colorMark != CLR_INVALID) ? colorMark : ::GetSysColor(COLOR_HIGHLIGHT);
		m_markWidth = width;
	}

	/**
	 * [�ݒ�] �}�[�J�[�w��.
	 *		�`������}�[�J�[�Ƃ��Ďw�肵�܂��B
	 *	@param draw �`����. �{�I�u�W�F�N�g�ɂ͕\����񂪋L�������̂ŁA draw �͔j�����Ă����܂��܂���B
	 */
	void SetMarker(const IDrawable& draw)
	{
		Action(false);
		m_DeleteDrawer();
		SIZE s;
		if ( draw.GetSize(s) )
		{
			m_piMarkerDrawer = draw.Clone();
			m_markWidth = s.cx;
		}
	}

	/**
	 * [�ݒ�] ����.
	 *		�}�[�J�[�̓�����J�n/��~���܂��B
	 *		�쐬���͒�~���Ă��܂��B
	 *	@param isAction true �œ���J�n / false�œ����~���܂��B
	 */
	void Action(bool isAction)
	{
		m_isActing = isAction;
		m_thread.Stop();
		if ( isAction )
		{
			m_pos = 0;
			m_thread.Start(_T("ProcessingCtrl"));
		}
		RedrawWindow();
	}

	/**
	 * [�m�F] ����m�F.
	 *		�}�[�J�[�̓���m�F�~�܂��B
	 *	@retval true ����
	 *	@retval false ��~
	 */
	bool IsActing(void) const
	{
		return m_isActing;
	}

	/**
	 * [�ݒ�] �X���b�h��Ԏ�擾
	 *	@return �X���b�h���
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		CRect rc;
		_super::GetClientRect(rc);
		m_size = rc.Size();
	}

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
		if ( message == WM_PAINT )
		{
			CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
			m_Draw(&dc);
			return 0;
		}
		else if ( message == WM_PRINTCLIENT )
		{
			CDC* pDc = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
			m_Draw(pDc);
			return 0;
		}
		else if ( message == WM_DESTROY )
		{
			m_isActing = false;
			m_thread.Stop();
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * �X���b�h�����֐�
	 *	@retval �O�i�Œ�)�B
	 */
	DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			::Sleep(m_interval);
			if ( m_isActing )
			{
				if ( m_mode > 0 )
				{
					m_pos += m_step;
					if ( m_pos + m_markWidth > m_size.cx )
					{
						m_pos = m_size.cx - m_markWidth;
						m_mode = -1;
					}
				}
				else if ( m_mode < 0 )
				{
					if ( m_pos >= m_step )
					{
						m_pos -= m_step;
					}
					else
					{
						m_mode = 1;
						m_pos = 0;
					}
				}
				else 
				{
					m_pos += m_step;
					if ( m_pos > m_size.cx )
					{
						m_pos = -m_markWidth;
					}
				}
				if ( IsRunnable() )
				{
					_super::Invalidate();
				}
			}
		}
		return 0;
	}

private:
	/// �}�[�J�[�`��폜
	void m_DeleteDrawer(void)
	{
		m_piMarkerDrawer.Null();
	}
	/**
	 * �`��
	 *	@param pDC �`����CDC�B
	 */
	void m_Draw(CDC* pDC)
	{
		CRect cliRect(CPoint(0, 0), m_size);
		HRGN rgn = ::CreateRectRgnIndirect(&cliRect);
		MFCLIB::SelectClipRgnOrg(pDC, rgn);
		_DeleteObject(rgn);
		if ( m_isActing )
		{
			CRect markRect(m_pos, 0, m_pos + m_markWidth, m_size.cy);
			if ( m_piMarkerDrawer.IsNull() )
			{
				pDC->FillSolidRect(markRect, m_colorMark);
			}
			else
			{
				m_piMarkerDrawer->Draw(*pDC, m_pos, 0);
			}
			HRGN rgn = ::CreateRectRgnIndirect(&markRect);
			MFCLIB::SelectClipRgnOrg(pDC, rgn, RGN_DIFF);
			_DeleteObject(rgn);
		}
		pDC->FillSolidRect(cliRect, m_colorBack);
		pDC->SelectClipRgn(NULL);
	}
	CThread			m_thread;			///< �X���b�h
	CSize			m_size;				///< �N���C�A���g�̑傫��
	COLORREF		m_colorBack;		///< �w�i�F
	bool			m_isActing;			///< �������H
	DWORD			m_interval;			///< ���x
	int				m_step;				///< �ړ���
	COLORREF		m_colorMark;		///< �}�[�N�J���[
	IDrawable::Ptr	m_piMarkerDrawer;	///< �}�[�J�[
	int				m_markWidth;		///< �}�[�J�[�̕�
	int				m_pos;				///< ���݂̍��W
	int				m_mode;				///< �ړ����[�h 0;�����̂� -1;�~���� 1;������
};



}; //MFC
}; //TNB
