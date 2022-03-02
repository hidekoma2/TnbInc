#pragma once
/**
 *@file
 * �V���A���C�U�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbException.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
//�O���錾
struct ISerializer;
struct IDeserializer;
#endif



/**@ingroup SERIALIZE
 * �V���A���C�U�u���C���^�[�t�F�[�X.
 *
 *		�V���A���C�Y�@�\���������邽�߂̃C���^�[�t�F�[�X�ł��B
 *		���̃C���^�[�t�F�[�X����������Ă����� ISerializer , IDeserializer �̃T�u�N���X��
 *		�V���A���C�Y/�f�V���A���C�Y���\�ł��i�R���N�^�̗v�f�Ɏg�p���Ă��\�j�B
 *
 *	@par�K�v�t�@�C��
 *			TnbSerializer.h
 *
 *	@date 06/08/17 �V�K�쐬
 */
struct ISerializable
{
	/// �f�X�g���N�^
	virtual ~ISerializable(void) {}
	/**
	 * [�ʒm] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const = 0;
	/**
	 * [�ʒm] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds) = 0;
};



/**@ingroup SERIALIZE
 * �V���A���C�U�[�C���^�[�t�F�[�X�N���X.
 *
 *		��{�^�A����сA ISerializable �����������N���X
 *		( ICollectionT ��������������Ă��܂�)�A
 *		����сA CStrT �n �̓V���A���C�Y�\�ɂȂ��Ă��܂��B
 *
 *		�V���A���C�Y�����f�[�^�́A�����^�A�������ԂŃf�V���A���C�Y����K�v������܂��B
 *		
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		void Foo(ISerializer& _sr)
 *		{
 *			int i;
 *			CStr str;
 *			_sr << i << str;			// int �ƕ�������V���A���C�Y����B			
 *		}
 *	</pre></td></tr>
 *
 *	@note ���̑��̌^(MFC�N���X�⃆�[�U�N���X�Ȃ�)�͈ȉ��̂悤�ȃI�y���[�^��p�ӂ��邱�ƂŃV���A���C�Y�ɑΉ��\�ł��B
 *	@par
 *	<table><tr><td><pre>
 *
 *		struct TMyType {	//���[�U�N���X
 *		{
 *			CStr strKey;
 *			int  iValue;
 *		}
 *
 *		inline ISerializer& operator<<(ISerializer& _sr, const TMyType& t)		
 *		{
 *			_sr << t.strKey << t.iValue;
 *			return _sr;
 *		}
 *	</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbSerializer.h
 *
 *	@date 06/08/17 �V�K�쐬
 *	@date 08/04/16 �@�\����
 *	@date 14/08/13 ���z�f�X�g���N�^����
 */
struct ISerializer
{
protected:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	template<typename T> void m_AddSub(const ISerializable* P, const T& t)
	{
		P->Serialize(*this);
	}

	template<typename T> void m_AddSub(LPCVOID P, const T& t)
	{
		TRACE1A("NotSupport-Serialize[%s]\n", typeid(T).name());
		throw CNotSupportException();
	}

	#define _SERIDEF(TY) virtual void m_AddSub(const TY* P, const TY& ty) { AddRaw(sizeof(TY), &ty); }
	_SERIDEF(bool)
	_SERIDEF(BYTE)
	_SERIDEF(char)
	_SERIDEF(WORD)
	_SERIDEF(short)
	_SERIDEF(DWORD)
	_SERIDEF(long)
	_SERIDEF(UINT)
	_SERIDEF(int)
	_SERIDEF(ULONGLONG)
	_SERIDEF(LONGLONG)
	_SERIDEF(long double)
	_SERIDEF(double)
	_SERIDEF(float)
	#undef _SERIDEF

	#endif //_TnbDOXYGEN

public:

	/// �f�X�g���N�^
	virtual ~ISerializer(void) 
	{
	}
	
	/**
	 * [�ǉ�] �ǉ�.
	 *		�w��f�[�^���V���A���C�Y���Ēǉ����܂��B
	 *	@param t �f�[�^
	 *	@return �����̎Q��
 	 *	@throw CNotSupportException �V���A���C�Y���T�|�[�g���Ă��Ȃ��^���w�肳���ƁA�X���[����܂��B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	template<typename T> ISerializer& operator<<(const T& t)
	{
		TRACE1A("Serialize[%s]\n", typeid(T).name());
		m_AddSub(&t, t);
		return *this;
	}

	/**
	 * [�ǉ�] �ǉ�.
	 *		�w��f�[�^��ǉ����܂��B
	 *	@note pure�֐��ł��B�T�u�N���X�ł͖{���\�b�h����������K�v������܂��B
	 *	@param size �ǉ�����f�[�^�T�C�Y(�P��BYTE)
	 *	@param P �ǉ�����f�[�^
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void AddRaw(size_t size, LPCVOID P) = 0;
};



/**@ingroup SERIALIZE
 * �f�V���A���C�U�[�C���^�[�t�F�[�X�N���X.
 *
 *		��{�^�A����сA ISerializable �����������N���X
 *		( ICollectionT ��������������Ă��܂�)�A
 *		����сA CStrT �n �̓f�V���A���C�Y�\�ɂȂ��Ă��܂��B
 *
 *		�V���A���C�Y�����f�[�^�́A�����^�A�������ԂŃf�V���A���C�Y����K�v������܂��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		void Foo(IDeserializer& ds)
 *		{
 *			int i;
 *			CStr str;
 *			ds >> i >> str;			// int �ƕ���������o���B		
 *		}
 *	</pre></td></tr>
 *
 *	@note ���̑��̌^(MFC�N���X�⃆�[�U�N���X�Ȃ�)�͈ȉ��̂悤�ȃI�y���[�^��p�ӂ��邱�ƂŃf�V���A���C�Y�ɑΉ��\�ł��B
 *	@par
 *	<table><tr><td><pre>
 *
 *		struct TMyType {	//���[�U�N���X
 *		{
 *			CStr strKey;
 *			int  iValue;
 *		}
 *
 *		inline const IDeserializer& operator>>(const IDeserializer& ds, TMyType& t)		
 *		{
 *			ds >> t.strKey >> t.iValue;
 *			return sr;
 *		}
 *	</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbSerializer.h
 *
 *	@date 06/08/17 �V�K�쐬
 *	@date 08/04/16 �@�\����
 *	@date 14/08/13 ���z�f�X�g���N�^����
 */
struct IDeserializer
{
private:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	template<typename T>
	void m_AssaySub(ISerializable* P, T& t) const
	{
		P->Deserialize(*this);
	}

	template<typename T> void m_AssaySub(LPVOID P, T& t) const
	{
		TRACE1A("NotSupport-Deserialize[%s]\n", typeid(T).name());
		throw CNotSupportException();
	}

	#define _SERIDEF(TY) virtual void m_AssaySub(TY* P, TY& ty) const { GetRaw(sizeof(TY), &ty); }
	_SERIDEF(bool)
	_SERIDEF(BYTE)
	_SERIDEF(char)
	_SERIDEF(WORD)
	_SERIDEF(short)
	_SERIDEF(DWORD)
	_SERIDEF(long)
	_SERIDEF(UINT)
	_SERIDEF(int)
	_SERIDEF(ULONGLONG)
	_SERIDEF(LONGLONG)
	_SERIDEF(long double)
	_SERIDEF(double)
	_SERIDEF(float)
	#undef _SERIDEF

	#endif //_TnbDOXYGEN

public:

	/// �f�X�g���N�^
	virtual ~IDeserializer(void) 
	{
	}

	/**
	 * [�擾] �擾.
	 *		�w��f�[�^���f�V���A���C�Y���Ď擾���܂��B
	 *	@param[out] _t �f�[�^
	 *	@return �����̎Q��
 	 *	@throw CNotSupportException �f�V���A���C�Y���T�|�[�g���Ă��Ȃ��^���w�肳���ƁA�X���[����܂��B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	template<typename T> const IDeserializer& operator>>(T& _t) const
	{
		TRACE1A("Deserialize[%s]\n", typeid(T).name());
		m_AssaySub(&_t, _t);
		return *this;
	}

	/**
	 * [�擾] �擾.
	 *		�w��f�[�^���擾���܂��B
	 *	@note pure�֐��ł��B�T�u�N���X�ł͖{���\�b�h����������K�v������܂��B
	 *	@param[in] size �擾����f�[�^�T�C�Y(�P��BYTE)
	 *	@param[out] _P �擾����f�[�^
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void GetRaw(size_t size, LPVOID _P) const = 0;
};



/**@ingroup SERIALIZE
 * �V���A���C�Y�^�O�N���X.
 *
 *		�V���A���C�Y�@�\�̃T�|�[�g�N���X�ł��B
 *		�V���A���C�Y���ɁA�^�O�i��񖼁j���������{�N���X�̃C���X�^���X�����邱�ƂŁA
 *		�f�V���A���C�Y���ɁA�^�O�i��񖼁j�`�F�b�N���s�����ƂŌ�����f�[�^�̃f�V���A���C�Y���K�[�h�ł��܂�
 *		�i��񖼂ɍ��ق�����ƁA CInvalidParamException ���X���[�����j�B
 *		
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		void CFoo::Serialize(ISerializer& _sr)
 *		{
 *			int i;
 *			CStr str;
 *			_sr << CSerializeTag(_T("CFooData"));	//��񖼂�ǉ�
 *			_sr << i << str;			// int �ƕ�������V���A���C�Y����B			
 *		}
 *
 *		void CFoo::Deserialize(IDeserializer& ds)
 *		{
 *			int i;
 *			CStr str;
 *			ds >> CSerializeTag(_T("CFooData"));	//��񖼂��`�F�b�N
 *			ds >> i >> str;			// int �ƕ���������o���B		
 *		}
 *
 *	</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbSerializer.h
 *
 *	@date 08/04/16 �V�K�쐬
 */
class CSerializeTag : public ISerializable
{
	CArrayPtrHandleT<TCHAR>	m_hpTagName;
public:
	/** 
	 * �R���X�g���N�^
	 *	@param tagName �^�O�l�[��
	 */
	CSerializeTag(LPCTSTR tagName)
	{
		size_t size = STRLIB::GetLen(tagName);
		m_hpTagName = new TCHAR[size + 1];
		STRLIB::Copy(m_hpTagName, tagName);
	}
	/**
	 * [�ʒm] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		LPCTSTR P = m_hpTagName;
		size_t l = STRLIB::GetLen(m_hpTagName);
		_sr.AddRaw(l * sizeof(TCHAR), P);
	}
	/**
	 * [�ʒm] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CInvalidParamException �^�O�����قȂ鎞�A�X���[����܂��B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		size_t l = STRLIB::GetLen(m_hpTagName);
		CWorkMemT<TCHAR> mem(l);
		ds.GetRaw(l * sizeof(TCHAR), mem);
		if ( STRLIB::Compare(mem, m_hpTagName, l) != 0 )
		{
			throw CInvalidParamException();
		}
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
