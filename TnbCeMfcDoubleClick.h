#pragma once
/**
 *@file
 * CE �_�u���N���b�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeMfcDoubleClick.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbMfcCommon.h"
#include "TnbTickCount.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#ifndef STN_DBLCLK
 #define STN_DBLCLK 1
#endif
#ifndef BN_DOUBLECLICKED
 #define BN_DOUBLECLICKED 1
#endif
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
_SELECTANY DWORD s_dcaDoubleClickTime = 0;
_SELECTANY DWORD s_toleranceClickRange = TNB::CalcScaleX(10);
#endif // _TnbDOXYGEN



/**@ingroup MFCCONTROL WINCE 
 * �_�u���N���b�N�A�h�C��(CE��p).
 *	
 *		CE �ł̓_�u���N���b�N�C�x���g���ȗ�����Ă��܂��B
 *		����𐧌��t���őΉ�����A�h�C���ł��B�C�x���g�n���h���͕��ʂ� IDE �ō쐬�ł��܂��B
 *	
 *	@note VC++2005�̏ꍇ�A Wizard �� �_�u���N���b�N�̃n���h�����쐬����Ɓu�V���{��( STN_DBLCLK �� BN_DOUBLECLICKED )��
 *		�錾����Ă��܂���v�G���[���������܂����A���̃w�b�_�ŁA�錾���Ă���܂��B
 *
 *	@param WND CWnd �̃T�u�N���X�ł���K�v������܂��B�ʏ�A �X�^�e�B�b�N�R���g���[���A�{�^���R���g���[�����w�肵�܂��B
 *
 *	@note �X�^�e�B�b�N�̏ꍇ�A SS_NOTIFY �����ĂȂ��ƁA�_�u���N���b�N�̃C�x���g�͔������܂���B
 *
 *	\code
 *		//= �^�錾 (typedef���Ă����Ȃ��� Wizard�ŃG���[�ɐ���܂�)
 *		typedef CDoubleClickAddinT<CStatic> CDcStatic;
 *
 *		//= �_�C�A���O�N���X�錾
 *		CDcStatic   m_text;     // <- ���X CStatic�����������o������������
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbCeMfcDoubleClick.h
 * 
 *	@date 11/06/06	�V�K�쐬
 */
template<typename WND>
class CDoubleClickAddinT : public WND
{
	DEFSUPER(WND);
public:

	/// �R���X�g���N�^
	CDoubleClickAddinT(void) : m_phase(STANDBY)
	{
		GetDoubleClickTime();
	}


	/**
	 * [�ݒ�] ���e�N���b�N�͈͐ݒ�.
	 *		�^�b�`�p�l���Ń_�u���N���b�N������ꍇ�A�����ꏊ����^�b�v���邱�Ƃ͂قڏo���܂���B
	 *		�����ŏ�������Ă��_�u���N���b�N�Ƃ��邽�߂͈̔͂��w��ł���悤�ɂ��Ă���܂��B
	 *		�f�t�H���g�� 96dpi �� 10 pixel(192 dpi �� 20 pixel)�ł��B
	 *	@attention ���̃��\�b�h�́A���� CDoubleClickAddinT �̃C���X�^���X�ɂ��e����^���܂��B
	 *	@param d ���e�͈�(pixel) 
	 */
	static void SetToleranceClickRange(DWORD d)
	{
		s_toleranceClickRange = d;
	}

	/**
	 * [�ݒ�] �_�u���N���b�N���Ԑݒ�.
	 *		�_�u���N���b�N�Ƃ́A�}�E�X�{�^���� 2 �񑱂��ăN���b�N���邱�Ƃł��B
	 *		2 ��ڂ̃N���b�N�́A1 ��ڂ̃N���b�N�̌�A�w�肳�ꂽ���ԓ��ɍs��Ȃ���΂Ȃ�܂���B
	 *		���̎��ԊԊu�̂��Ƃ��_�u���N���b�N���ԂƂ����܂��B
	 *	@attention ���̃��\�b�h�́A���� CDoubleClickAddinT �̃C���X�^���X�ɂ��e����^���܂��B
	 *	@note �f�t�H���g�� WINAPI �� �F�FGetDoubleClickTime() �œ����鎞�Ԃł��B
	 *	@param t �_�u���N���b�N����(ms) 
	 */
	static void SetDoubleClickTime(DWORD t)
	{
		s_dcaDoubleClickTime = t;
	}

	/**
	 * [�擾] �_�u���N���b�N���Ԏ擾.
	 *	@note �f�t�H���g�� WINAPI �� �F�FGetDoubleClickTime() �œ����鎞�Ԃł��B
	 *	@see SetDoubleClickTime()
	 *	@return �_�u���N���b�N����(ms) 
	 */
	static DWORD GetDoubleClickTime(void)
	{
		if ( s_dcaDoubleClickTime == 0 )
		{
			s_dcaDoubleClickTime = ::GetDoubleClickTime();
		}
		return s_dcaDoubleClickTime;
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		m_phase = STANDBY;
		LRESULT r = SendMessage(WM_GETDLGCODE);
		if ( (r & DLGC_STATIC) != 0 && (GetStyle() & SS_NOTIFY) == 0 )
		{
			m_phase = DISABLE;
		}
		_super::PreSubclassWindow();
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
		switch ( message )
		{
		case WM_LBUTTONDOWN:
			if ( m_phase > STANDBY && m_IsPassed() )
			{
				m_phase = STANDBY;
			}
			if ( m_phase == STANDBY )
			{
				m_phase = UPWAIT;
//				m_point = CPoint(LOWORD(lParam), HIWORD(lParam));
				m_point = CPoint(lParam);
				m_tick.Reset();
			}
			else if ( m_phase == DOWNWAIT )
			{
				m_phase = m_Check(lParam) ? UPWAIT2 : STANDBY;
			}
			break;
		case WM_LBUTTONUP:
			if ( m_phase == UPWAIT )
			{
				m_phase = m_Check(lParam) ? DOWNWAIT : STANDBY;
			}
			else if ( m_phase == UPWAIT2 )
			{
				if ( m_Check(lParam) )
				{
					MFCLIB::SendCommandMessage(this, 1); //�e�֒ʒm
				}
				m_phase = STANDBY;
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_phase > STANDBY && ! m_Check(lParam) )
			{
				m_phase = STANDBY;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// �t�F�[�Y
	enum EPhase
	{
		DISABLE,	///< ����
		STANDBY,	///< �{�^��DOWN�҂�
		UPWAIT,		///< �{�^��UP�҂�
		DOWNWAIT,	///< ���ڂ̃{�^��DOWN�҂�
		UPWAIT2,	///< ���ڂ̃{�^��UP�҂�
	};
	// �G���A�����`�F�b�N
	bool m_IsArea(const POINT& p) const
	{
		DWORD x = abs(p.x - m_point.x);
		DWORD y = abs(p.y - m_point.y);
		return (x <= s_toleranceClickRange && y <= s_toleranceClickRange);
	}
	// ���ԓ��`�F�b�N
	bool m_IsPassed(void) const
	{
		return m_tick.IsPassed(s_dcaDoubleClickTime);
	}
	// �`�F�b�N
	bool m_Check(LPARAM lParam) const
	{
//		return (! m_IsPassed() && m_IsArea(CPoint(LOWORD(lParam), HIWORD(lParam))));
		return (! m_IsPassed() && m_IsArea(CPoint(lParam)));
	}
	CTickCount	m_tick;		///< ���ԊǗ�
	CPoint		m_point;	///< ���W
	int			m_phase;	///< �t�F�[�Y
};



}; // MFC
}; // TNB


