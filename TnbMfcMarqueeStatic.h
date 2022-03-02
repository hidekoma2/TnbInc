#pragma once
/**
 *@file
 * �`����}�[�L�[�\���֌W�̃w�b�_
 *
 *		�`������}�[�L�[�\�����邽�߂̃R���g���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapDrawer.h"
#include "TnbTextDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����}�[�L�[�\���R���g���[���N���X
 *
 *		�`������ړ������Ȃ���\������R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcMarqueeStatic.h
 * 
 *	@date 10/05/20 �V�K�쐬
 *	@date 10/11/12 ������ IMMOVALE ��ǉ�
 */
class CMarqueeStatic : public CWnd
{
	DEFSUPER(CWnd);
public:

	/// �R���X�g���N�^
	CMarqueeStatic(void) : _super(), m_margin(0, 0), m_mode(Mode_Scroll)
		, m_direction(LEFT), m_amount(6), m_delay(85)
		, m_backColor(RGB(0, 0, 0)), m_withBackDraw(true)
	{
	}

	/**
	 * [�ݒ�] �}�[�W��.
	 *	@param cx ���E�̗]��
	 *	@param cy �㉺�̗]��
	 */
	void SetMargin(size_t cx, size_t cy)
	{
		m_margin.cx = cx;
		m_margin.cy = cy;
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			CRect rc;
			_super::GetClientRect(&rc);
			m_clientSize = rc.Size() - m_margin - m_margin;
			m_Reset();
		}
	}

	/// ���[�h
	enum EMode
	{
		Mode_Scroll,	///< �X�N���[��
		Mode_Slide,		///< �[�Œ�~
	};

	/**
	 * [�ݒ�] ���[�h�ݒ�.
	 *	@param mode ���[�h
	 */
	void SetMode(EMode mode)
	{
		m_mode = mode;
		m_Reset();
	}
	
	/// ����
	enum EDirection
	{
		LEFT,		///< �E���獶��
		RIGHT,		///< ������E��
		UP,			///< ��������
		DOWN,		///< �ォ�牺��
		IMMOVALE,	///< �s��
	};

	/**
	 * [�ݒ�] �����ݒ�.
	 *	@param d ����
	 */
	void SetDirection(EDirection d)
	{
		m_direction = d;
		m_Reset();
	}

	/**
	 * [�ݒ�] �ړ����x�ݒ�.
	 *	@param amount ���̈ړ���.
	 *	@param delay �ړ��Ԋu(ms)
	 */
	void SetSpeed(DWORD amount, DWORD delay)
	{
		m_amount = amount;
		m_delay = delay;
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			SetTimer(TIMERID_MARQUEE, m_delay, NULL);
		}
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@param c �w�i�F.
	 */
	void SetBackColor(COLORREF c)
	{
		m_backColor = c;
		m_Reset();
	}

	/**
	 * [�ݒ�] �`����ݒ�.
	 *	@param draw �`����. �{�I�u�W�F�N�g�ɂ̓N���[�����L�������̂ŁA draw �͔j�����Ă����܂��܂���B
	 *	@param withBackDraw �`���񂪔w�i��S���`�悷��ꍇ�A true ���w�肵�Ă��������B
	 */
	void SetDrawingObject(const IDrawable& draw, bool withBackDraw = true)
	{
		m_withBackDraw = withBackDraw;
		m_pDrawing = draw.Clone();
		if ( ! m_pDrawing.IsNull() )
		{
			m_pDrawing->GetSize(m_drawingSize);
		}
		m_Reset();
	}

	/**
	 * [�ݒ�] �`����ݒ�.
	 *	@param bmp �r�b�g�}�b�v
	 */
	void SetDrawingBitmap(CBitmapHandle bmp)
	{
		SetDrawingObject(CBitmapDrawer(bmp), true);
	}

	/**
	 * [�ݒ�] �`�敶����ݒ�.
	 *	@note �����̔w�i�́A�{���\�b�h����ɁA SetBackColor() ���g���ݒ肷��K�v������܂��B
	 *	@param font �t�H���g.
	 *	@param textColor �����F.
	 *	@param lpszText �e�L�X�g.
	 *	@param style �X�^�C��
	 *	@param margin �}�[�W��.
	 */
	void SetDrawingText(HFONT font, COLORREF textColor, LPCTSTR lpszText, DWORD style = DT_CENTER | DT_VCENTER, int margin = 0)
	{
		CTextBkDrawer d(font, textColor, m_backColor, lpszText, style, margin);
		d.AdjustSize();
		SetDrawingObject(d, true);
	}

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
			return 0;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				m_Draw(pDC);
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
				m_Draw(&dc);
			}
			return 0;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			return 0;
		case WM_TIMER:
			if ( wParam == TIMERID_MARQUEE )
			{
				if ( _super::IsWindowVisible() )
				{
					m_Next();
					_super::RedrawWindow();
				}
			}
			break;
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
		_super::ModifyStyle(0, SS_OWNERDRAW);
		_super::PreSubclassWindow();
		SetTimer(TIMERID_MARQUEE, m_delay, NULL);
		CRect rc;
		_super::GetClientRect(&rc);
		m_clientSize = rc.Size() - m_margin - m_margin;
		m_isStarted = false;
	}

private:

	void m_Reset(void)
	{
		m_isStarted = false;
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			SetTimer(TIMERID_MARQUEE, m_delay, NULL);
		}
	}

	/// �`��.
	void m_Draw(CDC* pDC)
	{
		if ( ! m_pDrawing.IsNull() )
		{
			CRect rc;
			_super::GetClientRect(&rc);
			pDC->IntersectClipRect(&rc);
			CPoint po = m_drawPoint + m_margin;
			if ( m_withBackDraw )
			{
				//�w�i���`�悷��Drawer��
				m_pDrawing->Draw(*pDC, po.x, po.y);
				pDC->ExcludeClipRect(CRect(po, m_drawingSize));
				pDC->FillSolidRect(&rc, m_backColor);
			}
			else
			{
				//�w�i�͏����Ă���Ȃ�Drawer��
				pDC->FillSolidRect(&rc, m_backColor);
				m_pDrawing->Draw(*pDC, po.x, po.y);
			}
		}
	}

	// �����o�̏�Ԃ���A m_drawPoint ���X�V����B
	void m_Next(void)
	{
		if ( ! m_isStarted )
		{
			// �J�n�I
			switch ( m_direction )
			{
			case LEFT: //�E����J�n
				m_drawPoint = CPoint(m_clientSize.cx + m_margin.cx, 0);
				break;
			case RIGHT: //������J�n
				m_drawPoint = CPoint(-m_drawingSize.cx - m_margin.cx, 0);
				break;
			case UP: //������J�n
				m_drawPoint = CPoint(0, m_clientSize.cy + m_margin.cy);
				break;
			case DOWN: //�ォ��J�n
				m_drawPoint = CPoint(0, -m_drawingSize.cy - + m_margin.cy);
				break;
			case IMMOVALE: //�s��
				m_drawPoint = CPoint(0, 0);
				KillTimer(TIMERID_MARQUEE);
				break;
			}
			m_isStarted = true;
		}
		int w = (m_clientSize.cx - m_drawingSize.cx);
		int h = (m_clientSize.cy - m_drawingSize.cy);
		switch ( m_direction )
		{
		case LEFT: //����
			m_drawPoint.x -= m_amount;
			break;
		case RIGHT: //�E��
			m_drawPoint.x += m_amount;
			break;
		case UP: //���
			m_drawPoint.y -= m_amount;
			break;
		case DOWN: //����
			m_drawPoint.y += m_amount;
			break;
		}
		if ( m_mode == Mode_Scroll )
		{
			switch ( m_direction )
			{
			case LEFT: //���𒴂�����E����
				if ( m_drawPoint.x < -m_drawingSize.cx ) 
				{
					m_drawPoint.x = m_clientSize.cx + m_margin.cx; 
				}
				break;
			case RIGHT: //�E�𒴂����獶����
				if ( m_drawPoint.x > m_clientSize.cx )
				{
					m_drawPoint.x = -m_drawingSize.cx - m_margin.cx;
				}
				break;
			case UP: //��𒴂����牺����
				if ( m_drawPoint.y < -m_drawingSize.cy ) 
				{
					m_drawPoint.y = m_clientSize.cy + m_margin.cy;
				}
				break;
			case DOWN: //���𒴂�����ォ��
				if ( m_drawPoint.y > m_clientSize.cy )
				{
					m_drawPoint.y = -m_drawingSize.cy - m_margin.cy;
				}
				break;
			}
		}
		if ( m_mode == Mode_Slide )
		{
			bool isContinue = true;
			switch ( m_direction )
			{
			case LEFT: //���ŏI���
				if ( m_drawPoint.x < 0 ) { m_drawPoint.x = 0; isContinue = false; }
				break;
			case RIGHT: //�E�ŏI���
				if ( m_drawPoint.x > w ) { m_drawPoint.x = w; isContinue = false; }
				break;
			case UP: //��ŏI���
				if ( m_drawPoint.y < 0 ) { m_drawPoint.y = 0; isContinue = false; }
				break;
			case DOWN: //���ŏI���
				if ( m_drawPoint.y > h ) { m_drawPoint.y = h; isContinue = false; }
				break;
			}
			if ( ! isContinue )
			{
				KillTimer(TIMERID_MARQUEE);
			}
		}
	}

	enum
	{
		TIMERID_MARQUEE = 1,	
	};

	IDrawable::Ptr	m_pDrawing;		///< �ێ�����`����
	CSize			m_margin;		///< �}�[�W��
	EMode			m_mode;			///< ���[�h
	EDirection		m_direction;	///< ����
	DWORD			m_amount;		///< ���̈ړ���
	DWORD			m_delay;		///< �ړ��Ԋu(ms)
	COLORREF		m_backColor;	///< �w�i�F
	bool			m_withBackDraw;	///< �w�i���`�悷��`����
	// -- work
	CSize			m_drawingSize;	///< �`��T�C�Y(m_pDrawing �̕`��̑傫��)
	CSize			m_clientSize;	///< �N���C�A���g�T�C�Y
	CPoint			m_drawPoint;	///< ���݂̕\���ʒu
	bool			m_isStarted;	///< �J�n�t���O
};



}; //MFC
}; //TNB



