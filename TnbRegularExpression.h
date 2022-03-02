#pragma once
/**
 *@file
 * ���K�\���֌W�̃w�b�_
 *
 *		���K�\�����������߂̃N���X������܂��B	
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
/// �{�N���X�m�F�p TRACE �}�N��
#if 0
#define _REGEX_TRACE0(S)			TRACE0(S)
#define _REGEX_TRACE1(S,P1)			TRACE1(S, P1)
#define _REGEX_TRACE2(S,P1,P2)		TRACE2(S,P1,P2)
#define _REGEX_TRACE3(S,P1,P2,P3)	TRACE3(S,P1,P2,P3)
#else
#define _REGEX_TRACE0(S)
#define _REGEX_TRACE1(S,P1)
#define _REGEX_TRACE2(S,P1,P2)
#define _REGEX_TRACE3(S,P1,P2,P3)
#endif
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup REGEX
 * ���K�\���Ǘ��N���X�e���v���[�g
 *
 *		��̐��K�\�����R���p�C�����������Ǘ����܂��B
 *		������g���āA�����񓙂Ɣ�r�A�����A�u�����邱�Ƃ��\�ł��B
 *
 *
 *	\par ���K�\���\���̗v��:
 *	\par �E����
 *		x�@			���� x												\n
 *		�����@		�o�b�N�X���b�V������								\n
 *		��0n�@		8 �i�l 0n �������� (0 <= n <= 7)					\n
 *		��0nn�@		8 �i�l 0nn �������� (0 <= n <= 7)					\n
 *		��0mnn�@	8 �i�l 0mnn �������� (0 <= m <= 3�A0 <= n <= 7)	\n
 *		��xhh�@		16 �i�l 0xhh ��������								\n
 *		��uhhhh�@	16 �i�l 0xhhhh ��������							\n
 *		��t�@		�^�u���� ('��x09')									\n
 *		��n�@		���s���� ('��x0A')									\n
 *		��r�@		�L�����b�W���^�[������ ('��x0D')					\n
 *		��f�@		�p�����蕶�� ('��x0C')								\n
 *		��e�@		�G�X�P�[�v���� ('��x1B')
 *
 *	\par �E��`�ς݂̕����N���X
 *		.�@		�C�ӂ̕���											\n
 *		��d�@ 	����: [0-9]											\n
 *		��D�@ 	�����ȊO: [^0-9]									\n
 *		��s�@ 	�󔒕���: [ ��t��n��x0B��f��r]						\n
 *		��S�@ 	��󔒕���: [^��s]									\n
 *		��w�@ 	�P��\������: [a-zA-Z_0-9]							\n
 *		��W�@ 	��P�ꕶ��: [^��w]									
 *
 *	\par �E�����N���X
 *		[abc]�@			a�Ab�A�܂��� c (�P���N���X)					\n
 *		[^abc]�@		a�Ab�Ac �ȊO�̕��� (�ے�)					\n
 *		[a-zA-Z]�@		a �` z �܂��� A �` Z (�͈�)					\n
 *		[a-d[m-p]]�@	a �` d�A�܂��� m �` p: [a-dm-p] (����)		\n
 *		[a-z&&[def]]�@	d�Ae�Af (����) 								\n
 *		[a-z&&[^bc]]�@	b �� c ������ a �` z: [ad-z] (���Z)			\n
 *		[a-z&&[^m-p]]�@	m �` p ������ a �` z: [a-lq-z] (���Z)		\n
 *		��������N���X���ł́A���ʎq,�_�����Z�q,�O���Q�Ƃ́A
 *		  �����̕����Ƃ��ĔF������܂��B
 *
 *	\par �E�Œ���v���ʎq
 *		X?�@		X�A1 ��܂��� 0 ��								\n
 *		X���@		X�A0 ��ȏ�										\n
 *		X�{�@		X�A1 ��ȏ�										\n
 *		X{n}�@		X�An ��											\n
 *		X(n,}�@		X�An ��ȏ�										\n
 *		X{n,m}�@	X�An ��ȏ�Am ��ȉ�						
 *
 *	\par �E�ŒZ��v���ʎq
 *		X??�@		X�A1 ��܂��� 0 ��								\n
 *		X��?�@		X�A0 ��ȏ�										\n
 *		X�{?�@		X�A1 ��ȏ�										\n
 *		X{n}?�@		X�An ��											\n
 *		X(n,}?		X�An ��ȏ�										\n
 *		X{n,m}?�@	X�An ��ȏ�Am ��ȉ�							\n
 *		�������́A X ���C�ӂ̕���(�s���I�h)���w�肵�����̂݁A�L���ł��B
 *
 *	\par �E�_�����Z�q
 *		XY�@		X �̒���� Y									\n
 *		X|Y�@		X �܂��� Y										\n
 *		(X)			�O���[�v�i�O���Q�Ƃ���j						\n
 *		(?:X)		�O���[�v�i�O���Q�ƂȂ��j						\n
 *		X(?=Y)		�m���ǂ�										\n
 *		X(?!Y)		�ے��ǂ�										\n
 *		(?<=X)Y		�m���ǂ�										\n
 *		(?<!X)Y		�ے��ǂ�										\n
 *
 *	\par �E�O���Q��
 *		��n�@	�}�b�`���� n �Ԗڂ̑O���Q�Ƃ��s�����K�\���O���[�v
 *
 *	\par ���K�\���O���[�v�ƑO���Q��:
 *		�O���Q�Ƃ��s�����K�\���O���[�v�ɂ́A������E�����ɍ��ۊ��ʂ𐔂��邱�Ƃɂ���āA
 *		�ԍ����t�����܂��B���Ƃ��΁A�\�� ((A)(B(C))) �́A���� 4 �̃O���[�v�ɕ��ނ�
 *		��܂��B			\n
 *		1�@	((A)(B(C)))		\n
 *		2�@	(A)				\n
 *		3�@	(B(C))			\n
 *		4�@	(C)				\n
 *		�O���[�v 0 �́A��ɕ\���S�̂�\���܂��B
 *
 *	@param TYP char (ASCII/SJIS) �� WCHAR (UNICODE) ���w�肵�܂��B
 *	@param ITE TYP�������C�e���[�^�B�ȗ�����ƁA const TYP* �ɂȂ�܂��B
 *
 *	\par�g�p��
 *		@code
 *		// a,b,�܂���c �ł��邱�Ƃ𒲍�
 *		VERIFY( re.SetPattern("[abc]") );
 *		VERIFY( re.Matches("c") );
 *
 *		// ':'��~���ɂ��ăe�L�X�g�𕪊�
 *		VERIFY( re.SetPattern(":") );
 *		CAsciiVector va = re.Split("boo:and:foo", NULL, 3) );
 *
 *		// �������̃A���t�@�x�b�g������
 *		VERIFY( re.SetPattern("[a-z]") );
 *		VERIFY( re.Find("123r789").foundIndex == 3 );
 *
 *		// �s�̐擪��java�Ƃ��������Ŏn�܂��Ă��邩����
 *		VERIFY( re.SetPattern("^java") );
 *		VERIFY( re.Find("javava").foundIndex, 0 );
 *		VERIFY( re.Find("a java").foundIndex, INVALID_INDEX );
 *		@endcode
 *
 *	@todo ���� �h.��//�h �̂悤�ɁA�Œ���v���ʎq�̌�ɓ�������������ƁA���s���܂��B
 *		 �h.��(?://)�h�̂悤�ɋL�q���K�v�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbRegularExpression.h
 * 
 *	@date 07/09/01 �V�K�쐬
 *	@date 07/09/25 �u�O���[�v�ɐ��ʎq����������ɑO���Q�Ɓv�ɁA�Ή��B
 *	@date 07/09/29 �O���[�v���܂񂾃O���[�v�ɐ��ʎq���������̓����� Java �Ɠ����ɂ����B
 *	@date 08/02/13 (?:X) �����Ή��������B�Ή��B
 */
template<typename TYP, typename ITE = const TYP*>
class CRegularExpressionT
{
public:

	/**
	 * �I�v�V����.
	 *	�}�b�`���O���@���w��ł��܂��B
	 *	@see SetOption
	 */
	enum EOption
	{
		IGNORECASE = _BIT(0),		///< �u�����̑啶���Ə���������ʂ��Ȃ��v�I�v�V����
//		MULTILINE = _BIT(1),		///< �u�����s���[�h��L���v�I�v�V����
	};

private:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	/// �`�F�b�J�[���ۃN���X
	class CAbstractChecker
	{
	public:
		/// �O���[�v�p�C�e���[�^�y�A
		struct TPairIterator
		{
			ITE		top;			///< �J�n�C�e���[�^
			ITE		end;			///< �ԕ��C�e���[�^
			bool	boValidEnd;		///< true �Ȃ�ԕ��L��
			/// �R���X�g���N�^
			TPairIterator(ITE s = ITE(), ITE e = ITE(), bool r = false)
				: top(s), end(e), boValidEnd(r)
			{
			}
			/// �ԕ��Z�b�g
			void SetEnd(ITE e)
			{
				end = e;
				boValidEnd = true;
			}
		};
		/// �`�F�b�N�p�ÓI�p�����[�^
		struct TStaticParam
		{
			ITE						end;				///< �ԕ��C�e���[�^
			CVectorT<TPairIterator>	groupIterators;		///< �O���[�v���
			ITE						tempEnd;			///< �b��ԕ�
			ITE						outsizeEnd;			///< �͈͊O�C�e���[�^
			DWORD					option;				////< �I�v�V����
			/// �R���X�g���N�^
			TStaticParam(void) : end(ITE()), tempEnd(ITE()), option(0)
			{
			}
			size_t GetSize(void) const
			{
				return groupIterators.GetSize();
			}
			TPairIterator At(INDEX index) const
			{
				const TPairIterator& ii = groupIterators.At(index);
				TPairIterator r((index == 0 && outsizeEnd != ITE()) ? outsizeEnd : ii.top);
				r.SetEnd(ii.boValidEnd ? ii.end : tempEnd);
				return r;
			}
			/// �O���[�v���Q�A�擾
			CVectorT< CStrT<TYP> > ToStrings(void) const
			{
				CVectorT< CStrT<TYP> > vs;
				loop ( i, GetSize() )
				{
					const TPairIterator& ii = At(i);
					vs.Add(CStrT<TYP>::FromIterator(ii.top, ii.end));
				}
				return vs;
			}
		};
		/// �`�F�b�N�p�p�����[�^
		struct TParam
		{
			ITE				cur;	///< �J�����g�C�e���[�^
			ITE				top;	///< �J�n�C�e���[�^
			TStaticParam*	pt;		///< �ԕ���t���O�A�O���[�v��
			/// �R���X�g���N�^
			TParam(ITE c, ITE t, ITE e, TStaticParam* p) : cur(c), top(t), pt(p) 
			{
				pt->end = e;
				pt->tempEnd = e;
				pt->outsizeEnd = ITE();
			}
			/// @return �J�����g�C�e���[�^���ԕ������O���w���Ă����� true�B
			bool IsTerminate(void) const
			{
				return (cur == pt->end) ? true : (*cur == 0);
			}
			/// @return �J�����g�C�e���[�^����ꕶ���擾�B
			size_t GetWord(WORD& _ch)
			{
				if ( IsTerminate() ){ return INVALID_SIZE; }
				WORD c = *cur;
				size_t l = STRLIB::GetCharSize(*cur);
				if ( l == 2 )
				{
					ASSERT( sizeof(TYP) == 1 );
					cur++;
					if ( IsTerminate() ){ return INVALID_SIZE; }
					c = static_cast<WORD>(((c & 0xFF) << 8) | ((*cur) & 0xFF));
				}
				_ch = c;
				return l;
			}
			/// �J�����g�C�e���[�^Step
			void StepCur(size_t l)
			{
				if ( l != INVALID_SIZE )
				{
					loop ( i, l )
					{
						ASSERTLIB( cur != pt->end );
						cur++;
					}
				}
			}
			/// ��r
			bool EqualChar(TYP c1, TYP c2)
			{
				if ( (pt->option & IGNORECASE) != 0 )
				{
					if ( c1 >= 'A' && c1 <= 'Z' ) { c1 |= 0x20; }
					if ( c2 >= 'A' && c2 <= 'Z' ) { c2 |= 0x20; }
				}
				return c1 == c2;
			}
		};
		/// �f�X�g���N�^
		virtual ~CAbstractChecker(void){}
		/**
		 * [��r] �`�F�b�N����.
		 *	@param pi �p�����[�^
		 *	@retval INVALID_SIZE ���v�B
		 *	@retval 0�ȏ� ��v�B���l�͈�v�������B
		 */
		virtual size_t Check(TParam ip) const = 0;
	};

	/// IChecker �|�C���^�̃n���h���^�錾
	typedef CPointerHandleT<CAbstractChecker>	CCheckerPtr;

	/// IChecker �|�C���^�̃n���h���^�̔z��^�錾
	typedef CVectorT<CCheckerPtr>		CCheckerPtrsVector;

	//================================================

	/** �u�w�蕶���Ɗ��S��v�v�`�F�b�J�[
	 *	@note �w�肵�������񂪑S�����������v�Ƃ���B
 	 */
	class CEqualChar : public CAbstractChecker
	{
		WORD	m_char;		///< �L������
	public:
		/// �R���X�g���N�^
		CEqualChar(WORD c) : m_char(c)
		{
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; EqualChar [0x%X]\n", m_char );
			WORD c;
			size_t l = ip.GetWord(c);
			if ( l == INVALID_SIZE ){ return INVALID_SIZE; }
			bool r = ip.EqualChar(static_cast<TYP>(c), static_cast<TYP>(m_char));
			return r ? l : INVALID_SIZE;
		}
	};

	/** �u�w�蕶���R�[�h�͈́v�`�F�b�J�[
	 *	@note �w�肵��top,bottm���Ȃ��v�Ƃ���B
	 *	@note ���o�[�X�t���O����B
 	 */
	class CRangeChar : public CAbstractChecker
	{
		WORD	m_top;			///< �L�������R�[�h���
		WORD	m_bottom;		///< �L�������R�[�h����
		bool	m_reverse;		///< �������]�t���O
	public:
		/// �R���X�g���N�^
		CRangeChar(WORD top, WORD bottom, bool boIsReverse = false) 
			: m_top(top), m_bottom(bottom), m_reverse(boIsReverse)
		{
			if ( m_top > m_bottom ) { Swap(m_top, m_bottom); }
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE3( "���K�\�� ��r ; RangeChar ['%c' - '%c' (%d)]\n", m_top, m_bottom, m_reverse );
			WORD c;
			size_t l = ip.GetWord(c);
			if ( l == INVALID_SIZE ){ return INVALID_SIZE; }
			if ( (ip.pt->option & IGNORECASE) != 0 )
			{
				if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
				{
					c |= 0x20;
					return ((c < (m_top|0x20) || c > (m_bottom|0x20)) ^ m_reverse) ? INVALID_SIZE : l;
				}
			}
			return ((c < m_top || c > m_bottom) ^ m_reverse) ? INVALID_SIZE : l;
		}
	};

	/** �u�ǂ�ȕ����ł���v�v�`�F�b�J�[
 	 */
	class CAnythingChar : public CAbstractChecker
	{
	public:
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE0( "���K�\�� ��r ; AnythingChar\n" );
			return ip.IsTerminate() ? INVALID_SIZE : (STRLIB::GetCharSize(*ip.cur));
		}
	};

	/** �u�擪�v�`�F�b�J�[
	 */
	class CTop : public CAbstractChecker
	{
	public:
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE0( "���K�\�� ��r ; Top\n" );
			return (ip.cur == ip.top) ? 0 : INVALID_SIZE;
		}
	};

	/** �u���[�v�`�F�b�J�[
	 */
	class CEnd : public CAbstractChecker
	{
	public:
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE0( "���K�\�� ��r ; End\n" );
			return ip.IsTerminate() ? 0 : INVALID_SIZE;
		}
	};

	/** �u�O���Q�ƈ�v�v�`�F�b�J�[
	*/
	class CFrontWord : public CAbstractChecker
	{
		INDEX		m_index;	///< ���oNo
	public:
		CFrontWord(INDEX i) : m_index(i)
		{
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; FrontWord (index = %d)\n", m_index );
			if ( ip.pt->groupIterators.GetSize() <= m_index )
			{
				return INVALID_SIZE;
			}
			const TPairIterator& ii = ip.pt->groupIterators.At(m_index);
			ITE is = ii.top;
			ITE ie = ii.boValidEnd ? ii.end : ip.pt->tempEnd;
//			if ( ! ii.boValidEnd )
//			{
//				CStrT<TYP> s = CStrT<TYP>::FromIterator(is, ie);
//				TRACE2( "[%s] [%s]\n", CStr(s), CStr(ip.cur) );
//			}
			size_t r = 0;
			while ( is != ie )
			{
				if ( *is == 0 ) { break; }
				if ( ip.cur == ip.pt->end ) { return INVALID_SIZE; }
				if ( ! ip.EqualChar(*is++, *ip.cur++) ){ return INVALID_SIZE; }
				r++;
			}
			return r;
		}
	};

	//================================================

	/** �u�O���[�v�Ǘ��v�`�F�b�J�[
	 *	@note �O���[�v�̂͂��߂ɂ�����͂���ł���
	 *	@note �B��̃`�F�b�J�[�� ����������A���̕������ۑ�����B
 	 */
	class CGroupChecker : public CAbstractChecker
	{
	public:
		enum EType
		{
			ET_Non,
			ET_RefGroup,		///< (regex)			�O���[�v�i�O���Q�Ƃ���j
			ET_NoRefGroup,		///< (?:regex)			�O���[�v�i�O���Q�ƂȂ��j
			ET_AfterEqu,		///< regex(?=regex)		�m���ǂ�
			ET_AfterNeq,		///< regex(?!regex)		�ے��ǂ�
			ET_BeforeEqu,		///< (?<=regex)regex	�m���ǂ�
			ET_BeforeNeq,		///< (?<!regex)regex	�ے��ǂ�
		};
	private:
		EType	m_type;
		CCheckerPtr	m_checker;		///> �`�F�b�J�[
	public:
		/// �R���X�g���N�^
		CGroupChecker(CCheckerPtr cp, EType y = ET_RefGroup) : m_checker(cp), m_type(y)
		{
			ASSERTLIB( y != ET_Non );
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; GroupChecker (%d)\n", m_type );
			if ( m_type == ET_RefGroup )	// (regex) �O���[�v�i�O���Q�Ƃ���j
			{
				INDEX insertIndex = ip.pt->groupIterators.GetSize();
				ip.pt->groupIterators.Add(TPairIterator(ip.cur, ip.pt->end));	//�_�~�[�f�[�^��ǉ����Ă����B
				size_t l = m_checker->Check(ip);
				if ( l != INVALID_SIZE )
				{
					ip.StepCur(l);
					ip.pt->groupIterators.Ref(insertIndex).SetEnd(ip.cur);
				}
				return l;
			}
			size_t l = m_checker->Check(ip);
			switch ( m_type )
			{
			case ET_RefGroup:	// (regex) �O���[�v�i�O���Q�Ƃ���j
				ip.StepCur(l);
				break;
			case ET_NoRefGroup:	// (?:regex) �O���[�v�i�O���Q�ƂȂ��j
				ip.StepCur(l);
				break;
			case ET_AfterEqu:	// regex(?=regex) �m���ǂ�
				l = (l != INVALID_SIZE) ? 0 : INVALID_SIZE;
				break;
			case ET_AfterNeq:	// regex(?!regex) �ے��ǂ�
				l = (l == INVALID_SIZE) ? 0 : INVALID_SIZE;
				break;
			case ET_BeforeEqu:	// (?<=regex)regex �m���ǂ�
			case ET_BeforeNeq:	// (?<!regex)regex �ے��ǂ�
				if ( ip.pt->outsizeEnd != ITE() )
				{
					l = INVALID_SIZE;
				}
				else
				{
					if ( l != INVALID_SIZE )
					{
						ip.StepCur(l);
						ip.pt->outsizeEnd = ip.cur;
					}
					if ( m_type == ET_BeforeNeq )	// (?<!regex)regex �ے��ǂ�
					{
						l = (l == INVALID_SIZE) ? 0 : INVALID_SIZE;
					}
				}
				break;
			default:
				ASSERTLIB( false );
				break;
			}
			return l;
		}
	};
	
	/** �u�`�F�b�J��v�ӏ��v�`�F�b�J�[
	 *	@note �Ō�Ɉ�v�����Ƃ����Ԃ�
	 */
	class CFindChecker : public CAbstractChecker
	{
		CCheckerPtr		m_loopChecker;	///< �J��Ԃ��`�F�b�J�[
		CCheckerPtr		m_markChecker;	///< ���`�F�b�J�[
		int				m_min;			///< �ŏ��A�����B
		int				m_max;			///< �ő�A�����B
		bool			m_isMini;		///< true �Ȃ�ŒP��v
	public:
		/// �R���X�g���N�^
		CFindChecker(CCheckerPtr loopChk, int min, int max, bool mini)
			: m_loopChecker(loopChk), m_min(min), m_max(max), m_isMini(mini)
		{
		}
		void SetMarkChecker(CCheckerPtr cp)
		{
			ASSERTLIB( this != cp.operator->() );
			m_markChecker = cp;
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			CVectorT<TPairIterator> bakGrpItes;
			ITE bakIte = ip.pt->tempEnd;
			size_t ns = ip.pt->groupIterators.GetSize();
			ip.pt->tempEnd = ip.cur;
			if ( m_markChecker.IsNull() )
			{
				_REGEX_TRACE2( "���K�\�� ��r ; FindChecker(%d �` %d) �uEND��MAX�܂Łv\n", m_min, m_max);
				INDEX findingIndex = 0;
				int foundCount = 0;
				while ( true )
				{
					if ( m_max >= 0 && foundCount == m_max ) { break; }
					if ( m_isMini && m_min <= foundCount ) { break; }
					size_t l = m_loopChecker->Check(ip);
					if ( ns < ip.pt->groupIterators.GetSize() )
					{
						if ( l != INVALID_SIZE )
						{
							bakGrpItes = ip.pt->groupIterators;
						}
						ip.pt->groupIterators.SetSize(ns);
					}
					if ( l == INVALID_SIZE ) { break; }
					ip.StepCur(l);
					ip.pt->tempEnd = ip.cur;
					foundCount++;
					findingIndex += l;
				}
				ip.pt->tempEnd = bakIte;
				if ( ! bakGrpItes.IsEmpty() )
				{
					ip.pt->groupIterators = bakGrpItes;
				}
				return (foundCount < m_min) ? INVALID_SIZE : findingIndex;
			}
			_REGEX_TRACE2( "���K�\�� ��r ; FindChecker(%d �` %d) �u����v�v\n", m_min, m_max);
			INDEX findingIndex = 0;
			INDEX foundIndex = INVALID_INDEX;
			int foundCount = 0;
			while ( ! ip.IsTerminate() )
			{
				if ( foundCount >= m_min )
				{
					size_t l = m_markChecker->Check(ip);
					if ( l != INVALID_SIZE )
					{
						foundIndex = findingIndex;
						if ( m_isMini ) { break; }
					}
					ns = ip.pt->groupIterators.GetSize();
				}
				if ( m_max >= 0 && foundCount == m_max ) { break; }
				size_t l = m_loopChecker->Check(ip);
				if ( ns < ip.pt->groupIterators.GetSize() )
				{
					if ( l != INVALID_SIZE )
					{
						bakGrpItes = ip.pt->groupIterators;
					}
					ip.pt->groupIterators.SetSize(ns);
				}
				if ( l == INVALID_SIZE ) { break; }
				ip.StepCur(l);
				ip.pt->tempEnd = ip.cur;
				foundCount++;
				findingIndex += l;
			}
			ip.pt->tempEnd = bakIte;
			if ( ! bakGrpItes.IsEmpty() )
			{
				ip.pt->groupIterators = bakGrpItes;
			}
			return (foundCount < m_min) ? INVALID_SIZE : foundIndex;
		}
	};

	/** �u�`�F�b�J�[�̂ǂꂩ�ƈ�v�v�`�F�b�J�[
	 *	@note m_next�͖��g�p	
 	 */
	class CAnyChecker : public CAbstractChecker
	{
	protected:
		CCheckerPtrsVector	m_checkers;	///< �`�F�b�J�Q
	public:
		/// ���̃`�F�b�J�[�͈Ӗ����邩�H true�Ȃ疳�Ӗ�
		virtual bool IsMeaningless(void) const
		{
			return m_checkers.GetSize() == 1;
		}
		/// �ŏ��̃`�F�b�J�[�擾
		virtual CCheckerPtr& Top(void)
		{
			return m_checkers[0];
		}
		/// �`�F�b�J�[�ǉ�
		virtual void Add(CCheckerPtr chk)
		{
			m_checkers.Add(chk);
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; AnyChecker(%d)\n", m_checkers.GetSize() );
			size_t r = INVALID_SIZE;
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l != INVALID_SIZE )
				{
					if ( r == INVALID_SIZE ) { r = 0; }
					if ( r < l ) { r = l; }
				}
			}
			return r;
		}
	};

	/** �u�`�F�b�J�[�̂ǂ�Ƃ���v���Ȃ��v�`�F�b�J�[
	 *	@note m_next�͖��g�p	
 	 */
	class CNotAnyChecker : public CAnyChecker
	{
	public:
		/// ���̃`�F�b�J�[�͈Ӗ����邩�H true�Ȃ疳�Ӗ�
		virtual bool IsMeaningless(void) const
		{
			return false;	//���̃N���X�͐�΁u�Ӗ�������v
		}
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; NotAnyChecker(%d)\n", m_checkers.GetSize() );
			if ( ip.IsTerminate() ){ return INVALID_SIZE; }
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l != INVALID_SIZE ) { return INVALID_SIZE; }
			}
			return STRLIB::GetCharSize(*ip.cur);
		}
	};

	/** �u�`�F�b�J�[�A�ǂ����v�v�`�F�b�J�[
	 *	@note �����ӏ��𕡐��̃`�F�b�J�Ń`�F�b�N����B���ʒ��������łȂ��ƈ�v�Ƃ��Ȃ��B
	 *	@note m_next�͖��g�p	
	 */
	class CBesidesChecker : public CAnyChecker
	{
	public:
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; BesidesChecker(%d)\n", m_checkers.GetSize() );
			size_t r = INVALID_SIZE;
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l == INVALID_SIZE ) { return INVALID_SIZE; }
				if ( r == INVALID_SIZE )
				{
					r = l;
				}
				else if ( r != l )
				{
					return INVALID_SIZE;
				}
			}
			ip.StepCur(r);
			return r;
		}
	};

	/** �u�A���`�F�b�N�v�`�F�b�J�[
 	 */
	class CLineChecker : public CAnyChecker
	{
	public:
		/// �`�F�b�N
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "���K�\�� ��r ; LineChecker(%d)\n", m_checkers.GetSize() );
			ASSERTLIB( m_checkers.GetSize() != 0 ); 
			size_t r = 0;	//��v�����g�[�^������
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l == INVALID_SIZE ) { return INVALID_SIZE; }
				ip.StepCur(l);
				r += l;
			}
			return r;
		}
	};

	#endif // _TnbDOXYGEN

	//================================================

	CCheckerPtr		m_root;				///< ���C���`�F�b�J
	CStrT<TYP>		m_base;				///< ���K�\��������
	CFindChecker*	m_pLastFindChecker;	///< �Ō�̌����`�F�b�J�[
	DWORD			m_option;			///< �I�v�V����

	/// �������J��
	void m_RemoveAll(void)
	{
		m_root.Null();
		m_base.Empty(); 
		m_pLastFindChecker = NULL;
	}

	/*
	 * _lpsz ��{ �̎��̕������w���Ă���
	 */
	bool m_SubNum(int& _r, const WORD*& _lpsz)
	{
		_r = -1;
		const WORD* P =_lpsz;
		while( *P >= '0' && *P <= '9' )
		{
			if ( _r < 0 ) { _r = 0; }
			_r *= 10;
			_r += (*P - '0');
			P++;
		}
		if ( _r >= 0 )
		{
			_lpsz = P;
			return true;
		}
		return false;
	}

	/*
	 * _lpsz ��{ �̕������w���Ă���
	 *	_checker �ɂ͌J��Ԃ��`�F�b�J�[�������Ă���B
	 */
	bool m_SubCnt(int& _min, int& _max, const WORD*& _lpsz)
	{
		const WORD* P =_lpsz;
		int r;
		if ( *P++ == '{' )
		{
			if ( m_SubNum(r, P) )
			{
				if ( *P != ',' )
				{
					_min = r;
					_max = r;
				}
				else
				{
					_min = r;
					P++;
					if ( m_SubNum(r, P) )
					{
						_max = r;
					}
				}
				if ( *P == '}' )
				{
					_lpsz = P + 1;
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * �w���AnyChecker���Ӗ����邩�`�F�b�N
	 *	�Ӗ������Ȃ炻�̒���TOP��Checker��Ԃ��B
	 */
	CCheckerPtr m_ChkChecker(CAnyChecker* P)
	{
		if ( P->IsMeaningless() )
		{
			CCheckerPtr c = P->Top();
			delete P;
			return c;
		}
		return P;
	}

	/*
	 * isAny �� false �Ȃ�
	 *		" " , ( ) ���̃`�F�b�N�p
	 *		@note �h�� ) �� \0 �܂ł��`�F�b�N
	 * isAny �� true �Ȃ�
	 *		[ ]���̃`�F�b�N�p
	 *		@note ]�܂ł��`�F�b�N
	 *
	 *	@param _lpsz �h���i�� [ �̎��̕����|�C���^�B�������A�I�[�̕����|�C���^�ɂȂ��Ă��܂��B
	 */
	bool m_Sub(CCheckerPtr& _checker, const WORD*& _lpsz, bool isAny)
	{
		CFindChecker* pLastFindChecker = m_pLastFindChecker;
		m_pLastFindChecker = NULL;
		CAnyChecker* pCheckers = NULL;
		const WORD* P = _lpsz;
		if ( isAny )
		{
			if ( *P == '^' )
			{
				P++;
				pCheckers = new CNotAnyChecker();
			}
			else
			{
				pCheckers = new CAnyChecker();
			}
		}
		else
		{
			pCheckers = new CLineChecker();
		}
		bool isError = false;
		bool isFirst = true;
		WORD rangeChar = 0;
		while ( ! isError )
		{
			WORD c = *P++;
			CCheckerPtr chk;
			CGroupChecker::EType groupType = CGroupChecker::ET_Non;
			CFindChecker* pLastFind = NULL;
			if ( ! isAny )
			{
				//Any�ȊO�ł̂ݗL���Ȃ���
				// [�`]�Ȃ��ł͂ł͖����Ȃ���
				switch ( c )
				{
				case '.':
					chk = new CAnythingChar();
					break;
				case '(':
					groupType = CGroupChecker::ET_RefGroup;
					if ( *P == '?' )
					{
						if ( P[1] == ':' )
						{
							groupType = CGroupChecker::ET_NoRefGroup;
							P += 2;
						}
						else if ( P[1] == '=' )
						{
							groupType = CGroupChecker::ET_AfterEqu;
							P += 2;
						}
						else if ( P[1] == '!' )
						{
							groupType = CGroupChecker::ET_AfterNeq;
							P += 2;
						}
						else if ( P[1] == '<' && P[2] == '=' )
						{
							groupType = CGroupChecker::ET_BeforeEqu;
							P += 3;
						}
						else if ( P[1] == '<' && P[2] == '!' )
						{
							groupType = CGroupChecker::ET_BeforeNeq;
							P += 3;
						}
					}
					if ( ! m_Sub(chk, P, false) || *P != ')' )
					{
						groupType = CGroupChecker::ET_Non;
						isError = true;
						break;
					}
					P++;
					pLastFind = m_pLastFindChecker;
					m_pLastFindChecker = NULL;
					break;
				case ')':
					_checker = m_ChkChecker(pCheckers);
					if ( pLastFindChecker != NULL )
					{
						pLastFindChecker->SetMarkChecker(_checker);
					}
					_lpsz = P - 1;
					return true;
				case '|':
					{
						CFindChecker* pCkBack = m_pLastFindChecker;
						m_pLastFindChecker = NULL;
						CCheckerPtr cp;
						if ( m_Sub(cp, P, false) && (*P == 0 || *P == ')') )
						{
							m_pLastFindChecker = pCkBack;
							CAnyChecker* a = new CAnyChecker();
							a->Add(m_ChkChecker(pCheckers));
							a->Add(cp);
							_checker = a;
							if ( pLastFindChecker != NULL )
							{
								pLastFindChecker->SetMarkChecker(_checker);
							}
							_lpsz = P;
							return true;
						}
					}
					isError = true;
					break;
				case '^':
					if ( isFirst )
					{
						chk = new CTop();
					}
					break;
				case '$':
					if ( *P == 0 )
					{
						chk = new CEnd();
					}
					break;
				default:
					break;
				}
			}
			if ( isError ) { break;	}
			//--
			if ( chk.IsNull() )
			{
				switch ( c )
				{
				case '[':
					if ( isFirst && isAny )
					{
						;
					}
					else if ( ! m_Sub(chk, P, true) || *P != ']' )
					{
						isError = true;
					}
					else
					{
						P++;
						pLastFind = m_pLastFindChecker;
						m_pLastFindChecker = NULL;
					}
					break;
				case ']':
					if ( isFirst && isAny )
					{
						break;
					}
					_checker = m_ChkChecker(pCheckers);
					if ( pLastFindChecker != NULL )
					{
						pLastFindChecker->SetMarkChecker(_checker);
					}
					_lpsz = P - 1;
					return true;
				case 0:
					_checker = m_ChkChecker(pCheckers);
					if ( pLastFindChecker != NULL )
					{
						pLastFindChecker->SetMarkChecker(_checker);
					}
					_lpsz = P - 1;
					return true;
				case '&':
					if ( isAny && *P == '&' )
					{
						P++;
						CFindChecker* pCkBack = m_pLastFindChecker;
						m_pLastFindChecker = NULL;
						CCheckerPtr cp;
						if ( m_Sub(cp, P, false) && *P == ']' )
						{
							m_pLastFindChecker = pCkBack;
							CAnyChecker* a = new CBesidesChecker();
							a->Add(m_ChkChecker(pCheckers));
							a->Add(cp);
							_checker = a;
							if ( pLastFindChecker != NULL )
							{
								pLastFindChecker->SetMarkChecker(_checker);
							}
							_lpsz = P;
							return true;
						}
						isError = true;
					}
					break;
				case '\\':
					ASSERTLIB( chk.IsNull() );
					c = *P++;
					switch ( c )
					{
					case 'd':	// ����([0-9])
					case 'D':	//	�����ȊO([~0-9])
						chk = new CRangeChar('0', '9', (c == 'D'));
						break;
					case 's':	//	��(\t\n\x0B\f\r)				���S�p�̊܂ށH
					case 'S':	//	�󔒈ȊO([~\s])
						{
							CAnyChecker* pAny = (c == 's') ? new CAnyChecker() : new CNotAnyChecker();
							const char ap[] = { ' ', '\t', '\n', '\x0B', '\f', '\r' };
							loop ( i, countof(ap) )
							{
								pAny->Add(new CEqualChar(ap[i]));
							}
							chk = pAny;
						}
						break;
					case 'w':	//	�P��([a-zA-Z0-9_])
					case 'W':	//	�P��ȊO([~\W])
						{
							CAnyChecker* pAny = (c == 'w') ? new CAnyChecker() : new CNotAnyChecker();
							pAny->Add(new CRangeChar('a', 'z'));
							pAny->Add(new CRangeChar('A', 'Z'));
							pAny->Add(new CRangeChar('0', '9'));
							pAny->Add(new CEqualChar('_'));
							chk = pAny;
						}
						break;
					default:
						P -= 2;
						int r = STRLIB::EscCharToInt(P);
						if ( r >= 0 )
						{
							c = static_cast<WORD>(r);
						}
						else
						{
							P += 2;
							if ( c >= '0' && c <= '9' )
							{
								chk = new CFrontWord(c - '0');
							}
						}
						break;
					}
					break;
				default:
					break;
				}
			}
			if ( isError ) { break;	}
			//--
			if ( chk.IsNull() )
			{
				if ( *P == '-' )
				{
					P++;
					WORD d = *P++;
					if ( d == '\\' )
					{
						--P;
						int r = STRLIB::EscCharToInt(P);
						if ( r >= 0 )
						{
							d = static_cast<WORD>(r);
						}
						else
						{
							isError = true;
						}
					}
					chk = new CRangeChar(c, d);
				}
				else
				{
					chk = new CEqualChar(c);
				}
			}
			if ( isError ) { break;	}
			//--
			if ( ! chk.IsNull() )
			{
				if ( rangeChar != 0 )
				{
					isError = true;
				}
				else if ( ! isAny )
				{
					int min = -1;
					int max = -1;
					switch ( *P )
					{
					case '*': //0�ȏ�
						min = 0;
						P++;
						break;
					case '+': //1�ȏ�
						min = 1;
						P++;
						break;
					case '?': //0��1��
						min = 0;
						max = 1;
						P++;
						break;
					case '{': // {a} a�̂Ȃ�сA{a,} a�ȏ�A{a,b} a�`b�B
						if ( ! m_SubCnt(min, max, P) )
						{
							isError = true;
							min = -1; //���ʂȏ������Ȃ��悤��
						}
						break;
					default:
						break;
					}
					if ( min >= 0 )
					{
						bool isMini = false;
						if ( *P == '?' )
						{
							P++;
							isMini = true;
						}
						if ( groupType != CGroupChecker::ET_Non )
						{
							chk = new CGroupChecker(chk, groupType);
							groupType = CGroupChecker::ET_Non;
						}
						pLastFind = new CFindChecker(chk, min, max, isMini);
						chk = pLastFind;
					}
				}
				if ( ! chk.IsNull() )
				{
					if ( groupType != CGroupChecker::ET_Non )
					{
						chk = new CGroupChecker(chk, groupType);
						groupType = CGroupChecker::ET_Non;
					}
					if ( m_pLastFindChecker != NULL )
					{
						m_pLastFindChecker->SetMarkChecker(chk);
					}
					m_pLastFindChecker = pLastFind;
					pLastFind = NULL;
					pCheckers->Add(chk);
					chk.Null();
				}
			}
			isFirst = false;
		}
		delete pCheckers;
		return false;
	}

	/**
	 * [��r] �}�b�`
	 *	@param[in] ip �Ώۏ��B
	 *	@param[out] pGroupStr ������̔z����w�肷��ƃO���[�v�̕�����𓾂邱�Ƃ��ł��܂��B
	 *				�s��v�ł���v�����Ƃ���܂ł͊i�[����܂��B
	 *	@retval INVALID_SIZE �s��v�B
	 *	@retval INVALID_SIZE �ȊO ��v�B
	 */
	size_t m_MatchSize(typename CAbstractChecker::TParam ip, CVectorT< CStrT<TYP> >* pGroupStr) const
	{
		if ( m_root.IsNull() )
		{
			return INVALID_SIZE;
		}
		size_t l = m_root->Check(ip);
		if ( ip.pt->outsizeEnd != ITE() )
		{
			l = INVALID_SIZE;
		}
		if ( pGroupStr != NULL )
		{
			*pGroupStr = ip.pt->ToStrings();
		}
		return l;
	}

public:

	/// �R���X�g���N�^
	CRegularExpressionT(void) : m_option(0)
	{
	}

	/**
	 * [�ݒ�] �I�v�V�����ݒ�.
	 *	�{�p�^�[�����g�p���鎞�̃I�v�V�������w�肵�܂��B
	 *	@note ��x�w�肷��ƁA�C���X�^���X���L�����A��Ԃ�ێ����܂��B
	 *	@note �C���X�^���X�쐬���́A �I�v�V����������ݒ肳��Ă��Ȃ���Ԃł��B
	 *	@param dw {@link CRegularExpressionT::EOption �I�v�V����}���w�肵�܂��B�����ɕ����w�肷��ꍇ�� | �ŘA�����܂��B
	 */
	void SetOption(DWORD dw)
	{
		m_option = dw;
	}
	
	/**
	 * [�ݒ�] �p�^�[���ݒ�.
	 *	@param lpsz �p�^�[��
	 *	@retval true �����B
	 *	@retval false ���s�i�p�^�[��������Ɍ�肠��j�B
	 */
	bool SetPattern(const TYP* lpsz)
	{
		m_RemoveAll();
		CWordVector vw = TNB::StringToWordVector(lpsz);
		vw.Add(0);
		vw.Add(0); //�ی�
		const WORD* P = vw.ReferBuffer();
		CCheckerPtr chk;
		if ( ! m_Sub(chk, P, false) )
		{
			m_RemoveAll();
			return false;
		}
		if ( *P != 0 )
		{
			m_RemoveAll();
			return false;
		}
		m_root = new CGroupChecker(chk);
		m_base = lpsz;
		return true;
	}

	/**
	 * [��r] �擪�}�b�`
	 *	@note �{�C���X�^���X�������Ă�������Ɛ擪�}�b�`���Ă��邩�`�F�b�N���܂��B
	 *	@param[in] is �Ώۏ��̊J�n�C�e���[�^�B
	 *	@param[in] ie �Ώۏ��̔ԕ��C�e���[�^�B�ȗ�����ꍇ�A�K�� NIL�I�[����������g�p���Ă��������B
	 *	@param[out] pGroupStr ������̔z����w�肷��ƃO���[�v�̕�����𓾂邱�Ƃ��ł��܂��B
	 *				�s��v�ł���v�����Ƃ���܂ł͊i�[����܂��B�ȗ����邱�Ƃ��o���܂��B
	 *	@retval INVALID_SIZE �s��v�B
	 *	@retval INVALID_SIZE �ȊO ��v�B
	 */
	size_t LookingAt(ITE is, ITE ie = ITE(), CVectorT< CStrT<TYP> >* pGroupStr = NULL) const
	{
		CAbstractChecker::TStaticParam sp;
		sp.option = m_option;
		CAbstractChecker::TParam ip(is, is, ie, &sp);
		return m_MatchSize(ip, pGroupStr);
	}

	/**
	 * [��r] �S�̃}�b�`
	 *	@note �{�C���X�^���X�������Ă�������ƑS�̂��}�b�`���Ă��邩�`�F�b�N���܂��B
	 *	@param[in] is �Ώۏ��̊J�n�C�e���[�^�B
	 *	@param[in] ie �Ώۏ��̔ԕ��C�e���[�^�B�ȗ�����ꍇ�A�K�� NIL�I�[����������g�p���Ă��������B
	 *	@param[out] pGroupStr ������̔z����w�肷��ƃO���[�v�̕�����𓾂邱�Ƃ��ł��܂��B
	 *				�s��v�ł���v�����Ƃ���܂ł͊i�[����܂��B�ȗ����邱�Ƃ��o���܂��B
	 *	@retval ��v�B
	 *	@retval �s��v�B
	 */
	bool Matches(ITE is, ITE ie = ITE(), CVectorT< CStrT<TYP> >* pGroupStr = NULL) const
	{
		CAbstractChecker::TStaticParam sp;
		sp.option = m_option;
		CAbstractChecker::TParam ip(is, is, ie, &sp);
		size_t l = m_MatchSize(ip, pGroupStr);
		if ( l == INVALID_SIZE )
		{
			return false;
		}
		ip.StepCur(l);
		return ip.IsTerminate();
	}

	/**@ingroup REGEX
	 * Find() ���\�b�h�p���U���g
	 *	@see Find �̖߂�l�Ŏg�p����܂��B
	 *	@par�K�v�t�@�C��
	 *			TnbRegularExpression.h
	 *	@date 07/09/01 �V�K�쐬
	 */
	struct TFindResult
	{
		INDEX		foundIndex;	///< �}�b�`�����ʒu
		size_t		foundSize;	///< �}�b�`��������
		/// �R���X�g���N�^
		TFindResult(void) : foundIndex(0), foundSize(0) {}
	};

	/**
	 * [����] ����
	 *	@note �ŏ��Ƀ}�b�`��������Ԃ��܂��B�A���Ō�������ꍇ�A GetFinder() �� CFinder ���擾�����ق����֗��ł��B
	 *	@param[in] is �Ώۏ��̊J�n�C�e���[�^�B
	 *	@param[in] ie �Ώۏ��̔ԕ��C�e���[�^�B�ȗ�����ꍇ�A�K�� NIL�I�[����������g�p���Ă��������B
	 *	@param[out] pGroupStr ������̔z����w�肷��ƃO���[�v�̕�����𓾂邱�Ƃ��ł��܂��B�ȗ��\�B
	 *	@return ���U���g�B foundSize �� INVALID_SIZE �Ȃ猩����Ȃ������B 
	 */
	TFindResult Find(ITE is, ITE ie = ITE(), CVectorT< CStrT<TYP> >* pGroupStr = NULL) const
	{
		CAbstractChecker::TStaticParam sp;
		sp.option = m_option;
		CAbstractChecker::TParam ip(is, is, ie, &sp);
		TFindResult r;
		if ( ! m_root.IsNull() )
		{
			while ( ! ip.IsTerminate() )
			{
				ip.top = is;
				sp.groupIterators.RemoveAll();
				sp.outsizeEnd = ITE();
				r.foundSize = m_root->Check(ip);
				if ( r.foundSize != INVALID_SIZE )
				{
					if ( sp.outsizeEnd != ITE() )
					{
						while ( is != ip.cur )
						{
							is++;
						}
						while ( is != sp.outsizeEnd )
						{
							is++;
							r.foundIndex++;
							r.foundSize--;
						}
					}
					if ( pGroupStr != NULL ) 
					{
						*pGroupStr = ip.pt->ToStrings();
					}
					return r;
				}
				if ( sp.outsizeEnd != ITE() )
				{
					while ( ip.cur != sp.outsizeEnd )
					{
						r.foundIndex++;
						ip.cur++;
					}
				}	
				if ( ! ip.IsTerminate() && STRLIB::GetCharSize(*ip.cur) == 2 )
				{
					r.foundIndex++;
					ip.cur++;
				}
				r.foundIndex++;
				ip.cur++;
			}
		}
		r.foundSize = INVALID_SIZE;
		r.foundIndex = INVALID_INDEX;
		if ( pGroupStr != NULL ) 
		{
			*pGroupStr = ip.pt->ToStrings();
		}
		return r;
	}

	/**
	 * [�擾] ����.
	 *	@note	���̃p�^�[���̃}�b�`�Ɋ�Â��āA�w�肳�ꂽ���̓V�[�P���X�𕪊����܂��B 
	 *			���̃��\�b�h����Ԃ����z��́A���̓V�[�P���X�̕���������ō\������܂��B
	 *			�����̕���������́A���̃p�^�[���ƃ}�b�`����ʂ̕����V�[�P���X�ɂ����
	 *			�������ꂽ�����񂩁A���̓V�[�P���X�̏I���ɂ���ĕ������ꂽ������ł��B
	 *			�z����̕���������́A���̓V�[�P���X���Ŕ����������Ԃɕ���ł��܂��B
	 *			���̃p�^�[�������̓V�[�P���X�̕����V�[�P���X�Ƀ}�b�`���Ȃ��ꍇ�A
	 *			�Ԃ����z��� 1 �̗v�f�A�܂蕶����`���̓��̓V�[�P���X�����ō\������܂��B 
	 *	@note	limit �p�����[�^�́A���̃p�^�[���̓K�p�񐔁A�܂�A�Ԃ����z��̒����𐧌䂵�܂��B
	 *			���� n ���[�����傫���ꍇ�A���̃p�^�[���� n - 1 ��ȉ��̉񐔂��K�p����A
	 *			�z��̒����� n �ȉ��ɂȂ�܂��B
	 *			�z��̍Ō�̃G���g���ɂ́A�Ō�Ƀ}�b�`������؂蕶���ȍ~�̓��̓V�[�P���X�����ׂĊ܂܂�܂��B
	 *			n �����̒l�̏ꍇ�A���̃p�^�[���̓K�p�񐔂Ɣz��̒����͐�������܂���Bn ���[���̏ꍇ�A
	 *			���̃p�^�[���̓K�p�񐔂Ɣz��̒����͐�������܂��񂪁A�㑱�̋�̕�����͔j������܂��B
	 *	@param is �Ώۏ��̊J�n�C�e���[�^�B
	 *	@param ie �Ώۏ��̔ԕ��C�e���[�^�B�ȗ�����ꍇ�A�K�� NIL�I�[����������g�p���Ă��������B
	 *	@param limit �K�p�񐔁B�ȗ�����ƂO���K�p����܂��B
	 *	@return ��������������Q�B 
	 */
	CVectorT< CStrT<TYP> > Split(ITE is, ITE ie = ITE(), int limit = 0) const
	{
		CVectorT< CStrT<TYP> > vs;
		while ( limit <= 0 || static_cast<int>(vs.GetSize()) < limit - 1 )
		{
			TFindResult r = Find(is, ie);
			if ( r.foundIndex == INVALID_INDEX )
			{
				break;
			}
			CStrT<TYP> s; 
			loop ( i, r.foundIndex )
			{
				s += *is++;
			}
			vs.Add(s);
			loop ( i, r.foundSize )
			{
				is++;
			}
		}
		vs.Add(CStrT<TYP>::FromIterator(is, ie));
		if ( limit == 0 )
		{
			for ( INDEX i = vs.GetSize() - 1; i > 0; i-- )
			{
				if ( ! vs[i].IsEmpty() )
				{
					break;
				}
				vs.Remove(i);
			}
		}
		return vs;
	}

	/**@ingroup REGEX
	 * ���K�\���t�@�C���_�N���X
	 *	
	 *	�����������\�B�܂��A�K�v�ɉ����Ēu�����邱�Ƃ��\�ł��B
	 *	
	 *	@par�g�p��
	 *		<table><tr><td><pre>
	 *	
	 *		CRegularExpressionT<char> re;
	 *		re.SetPattern("([ABC])-");
	 *		CRegularExpressionT<char>::CFinder dd = re.GetFinder("A-B-9C!C-!");
	 *		if ( dd.IsFinding() )
	 *		{
	 *			do{
	 *				TRACE2A("index = %d,[%s]\n", dd.GetFoundIndex(), dd.GetFoundString() );			
	 *				dd.Replace("ZZ\\1");
	 *				TRACE1A("[%s]\n", dd.GetString() );
	 *			}
	 *			while( dd.Next() );
	 *			TRACE1A("[%s]\n", dd.GetString() );
	 *		}
	 *	
	 *		</pre></td></tr></table>
	 *
	 *	@see CRegularExpressionT::GetFinder() �ɂ��쐬����܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbRegularExpression.h
	 *
	 *	@date 07/09/01 �V�K�쐬
	 */
	class CFinder
	{
		const CRegularExpressionT*	m_pRegEx;		///< �p�^�[��
		ITE							m_is;			///< �J�n�C�e���[�^
		ITE							m_ie;			///< �ԕ��C�e���[�^
		CStrT<TYP>					m_strDst;		///< �ϊ��㕶����
		INDEX						m_index;		///< ����INDEX
		CStrT<TYP>					m_foundString;	///< ��������������
		INDEX						m_foundIndex;	///< ��������INDEX
		CVectorT< CStrT<TYP> >		m_foundGroupStr;///< ���������Ƃ��̃O���[�v������
	public:

		/**
		 * �R���X�g���N�^
		 *	@note �ʏ�A���ڃC���X�^���X����炸�A CRegularExpressionT::GetFinder() ���g�p���܂��B
		 *	@param P ���K�\���Ǘ��N���X�̃C���X�^���X�B
		 *	@param is �Ώۏ��̊J�n�C�e���[�^�B
		 *	@param ie �Ώۏ��̏I���ԕ��C�e���[�^�B�ȗ�����ƃf�t�H���g�̔ԕ��ɂȂ�܂��B
		 */
		CFinder(const CRegularExpressionT* P = NULL, ITE is = ITE(), ITE ie = ITE()) 
			: m_foundIndex(0), m_index(0), m_pRegEx(P), m_is(is), m_ie(ie)
		{
			Next();
		}

		/**
		 * [�m�F] �����m�F
		 *	@retval true �������iGetFound�n�A Replace() ���\�b�h�L���j
		 *	@retval false �����Ȃ�
		 */
		bool IsFinding(void) const
		{
			return m_pRegEx != NULL;
		}

		/**
		 * [����] ������
		 *	@note ���Ɉ�v����ꏊ���������܂��B
		 *	@retval true �����������iGetFound�n�A Replace() ���\�b�h�L���j
		 *	@retval false ���Ȃ�
		 */
		bool Next(void)
		{
			if ( m_pRegEx != NULL )
			{
				m_strDst += m_foundString;
				m_foundString.Empty();
				TFindResult r = m_pRegEx->Find(m_is, m_ie, &m_foundGroupStr);
				if ( r.foundSize == INVALID_SIZE )
				{
					m_pRegEx = NULL;
				}
				else
				{
					m_strDst += CStrT<TYP>::FromIterator(m_is, m_ie, r.foundIndex);
					loop ( i, r.foundIndex ) { m_is++; }
					m_foundString = CStrT<TYP>::FromIterator(m_is, m_ie, r.foundSize);
					m_foundIndex = m_index + r.foundIndex;
					loop ( i, r.foundSize ) { m_is++; }
					m_index += r.foundIndex + r.foundSize;
				}
			}
			return m_pRegEx != NULL;
		}

		/**
		 * [�u��] �u������.
		 *	@note �}�b�`������������w��̒u��������Œu�����܂��B���̍ہA�ȉ��̃L�[���[�h���g�p�ł��܂��B
		 *	\par �E�L�[���[�h
		 *			���O�`���X�@		�}�b�`����������̃u���b�N���}������܂��B	\n
		 *			�����@�@�@		���������}������܂��B
		 *	@param lpsz �u��������B
		 *	@retval true �����B
		 *	@retval false ���s�B
		 */
		bool Replace(const TYP* lpsz)
		{
			if ( ! IsFinding() ) { return false; }
			CStrT<TYP> s;
			while ( *lpsz != 0 )
			{
				if ( STRLIB::GetCharSize(*lpsz) == 2 )
				{
					s += *lpsz++;
					s += *lpsz++;
				}
				else if ( *lpsz != '\\' )
				{
					s += *lpsz++;
				}
				else
				{
					lpsz++;
					TYP c = *lpsz++;
					if ( c >= '0' && c <= '9' )
					{
						INDEX i = c - '0';
						if ( m_foundGroupStr.GetSize() > i )
						{
							s += m_foundGroupStr[i];
						}
					}
					else
					{
						s += c;
					}
				}
			}
			m_foundString = s;
			return true;
		}

		/**
		 * [�擾] �}�b�`����������擾.
		 *	@attention Replace() ������ƁA�����镶����͒u�����������̂Ȃ�܂��B
		 *	@return ������
		 */
		CStrT<TYP> GetFoundString(void) const
		{
			return m_foundString;
		}

		/**
		 * [�擾] �}�b�`���������񒷎擾.
		 *	@attention Replace() ������ƁA�����镶����͒u�����������̂Ȃ�܂��B
		 *	@return ������
		 */
		size_t GetFoundSize(void) const
		{
			return m_foundString.GetLength();
		}

		/**
		 * [�擾] �}�b�`�����O���[�v������擾.
		 *	@return �O���[�v������
		 */
		const CVectorT< CStrT<TYP> >& GetFoundGroupString(void) const
		{
			return m_foundGroupStr;
		}

		/**
		 * [�擾] �}�b�`�����ʒu�擾.
		 *	@return �ʒu
		 */
		INDEX GetFoundIndex(void) const
		{
			return m_foundIndex;
		}

		/**
		 * [�擾] �u�����ʕ�����擾.
		 *	@note �������u���������ʂ̕�������擾�ł��܂��B
		 *	�Ō�܂Ń`�F�b�N���Ă��Ȃ��Ă��u�������Ƃ���܂ł��擾�ł��܂��B
		 *	@return �u�����ʕ�����
		 */
		CStrT<TYP> GetString(void) const
		{
			return m_strDst + m_foundString + CStrT<TYP>::FromIterator(m_is, m_ie);
		}
	};

	/**
	 * [�擾] �t�@�C���_�[�擾.
	 *	@param is �Ώۏ��̊J�n�C�e���[�^�B
	 *	@param ie �Ώۏ��̏I���ԕ��C�e���[�^�B�ȗ�����ƃf�t�H���g�̔ԕ��ɂȂ�܂��B
	 *	@return �t�@�C���_�[�B 
	 */
	CFinder GetFinder(ITE is, ITE ie = ITE()) const
	{
		return CFinder(this, is, ie);
	}

	/**
	 * [�u��] �u������.
	 *	@note �}�b�`������������w��̒u��������Œu�����܂��B���̍ہA�ȉ��̃L�[���[�h���g�p�ł��܂��B
	 *	\par �E�L�[���[�h
	 *			���O�`���X�@		�}�b�`����������̃u���b�N���}������܂��B	\n
	 *			�����@�@�@		���������}������܂��B
	 *	@note ����m�F���Ȃ���s���ꍇ�A GetFinder() ���g�p���܂��B
	 *	@param lpsz �u��������B
	 *	@param is �Ώۏ��̊J�n�C�e���[�^�B
	 *	@param ie �Ώۏ��̔ԕ��C�e���[�^�B�ȗ�����ꍇ�A�K�� NIL�I�[����������g�p���Ă��������B
	 *	@return �u�����ʕ�����
	 */
	CStrT<TYP> ReplaceAll(const TYP* lpsz, ITE is, ITE ie = ITE()) const
	{
		CFinder f(this, is, ie);
		if ( f.IsFinding() )
		{
			do { f.Replace(lpsz); } while ( f.Next() );
		}
		return f.GetString();
	}

private:
	friend class CRegularExpressionTest;
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")


#if 0

���K�\���O���[�v�ƑO���Q�� 

�O���Q�Ƃ��s�����K�\���O���[�v�ɂ́A������E�����ɍ��ۊ��ʂ𐔂��邱�Ƃɂ���āA
�ԍ����t�����܂��B���Ƃ��΁A�\�� ((A)(B(C))) �́A���� 4 �̃O���[�v�ɕ��ނ�
��܂��B

	1   ((A)(B(C)))
	2	(A)
	3	(B(C))
	4	(C)
�O���[�v 0 �́A��ɕ\���S�̂�\���܂��B


http://oraclesqlpuzzle.hp.infoseek.co.jp/regex/



#endif


#if 0

http://programnet.hp.infoseek.co.jp/practical/regex.html
�X�֔ԍ�	�@\d{3}-\d{4}
�g�єԍ�	�@090-\d{4}-\d{4}
�d�b�ԍ�	�@\d{1,4}?-\d{1,4}?-\d{1,4}
���N����	�@\d{4}-\d{2}-\d{2}
���[���A�h���X	�@[!#-9A-~]+@[a-z0-9-_]+\.+[a-z0-9-_]+\.+[a-z0-9-]



�悭���銨�Ⴂ ����2
�O�q�����ʂ�A �u���P�b�g�Ɉ͂܂�Ă��钆�ł͂قƂ�ǂ̃��^�L����
�N�^�͂��̓���ȈӖ��������܂��B ���������āA [^(foo)]bar �Ƃ���
���K�\�����܂��A�ufoo�ł͂Ȃ�������ɑ�����bar�Ƃ��������񂪑������́v
 �ł͂���܂���B�uf�ł�o�ł�(�ł�)�ł��Ȃ������ɑ����� bar�Ƃ���
 �����񂪑��������́v �ł��B []�̓����ł́A�������(��菬����)
 ���K�\���v�f���܂Ƃ߂�Ƃ����J�b�R�� ���ʂȈӖ��͎����Ă��܂���
 �������Ƃɒ��ӂ��Ă��������B


���K�\���̊m�F�\ HP
http://lab.moyo.biz/tools/regexp/index.jsp
#endif

#if 0
���[�h

�����s���[�h					��	'^' �� '$' ���e�s�̎n�܂�ƏI���Ƀ}�b�`����悤�ɂȂ�܂��B 
.�����s�ɂ��}�b�`���郂�[�h		��	'.' �����s�����ɂ��}�b�`����悤�ɂ��܂��B
�啶���Ə������𓯈ꎋ���[�h	��	�啶���Ə���������ʂ��Ȃ��悤�ɂ��܂��B







#endif
