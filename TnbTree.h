#pragma once
/**
 *@file
 * �c���[�^���Ǘ��֌W�̃w�b�_
 *
 *		���傤��PC�̃t�H���_�ƃt�@�C���̂悤�ȏ��̊Ǘ����\�ȁA
 *		Tree�̃e���v���[�g������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"
#include "TnbMap.h"
#include "TnbCollectionAdapter.h"
#include "TnbStr.h"
#include "TnbSerializeAdapter.h"



//TNB Library
namespace TNB
{



/**@ingroup TREE
 * �c���[�^���Ǘ��e���v���[�g�N���X
 *
 *	@note		ICollenctionT �͎������Ă��܂���B
 *	@attention	�{�N���X�̃R�s�[�@�\�͎��̂̎Q�Ƃ̕����ɂȂ�܂��B
 *				�ʂ̎��̂ɂ������ꍇ�A Clone() ���g���K�v������܂��B
 *
 *	@param	KEY �L�[�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\���K�{�B
 *	@param	VAL �l�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\���K�{�B
 *	@param	INK �L�[�Ƃ���^�̓��͌^�B�ȗ��\�B
 *
 *	@par�K�v�t�@�C��
 *			TnbTree.h
 * 
 *	@date 06/04/28 �V�K�쐬
 *	@date 06/09/04 �S�ʉ���
 */
template<typename KEY, typename VAL, typename INK = KEY>
class CTreeT : public ISerializable
{
	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	class COneHandle;										///< �O���錾
	typedef CMapT<KEY,COneHandle,INK> CMapKey;				///< �����pMAP�^��`
	/// �X���^
	struct TOne
	{
		VAL							value;					///< �����̒l
		CPointerHandleT< CMapKey >	hpMap;					///< �}���
		/// �R���X�g���N�^
		TOne(void) : value(VAL()) {}
	};

	/// �X���Ǘ��n���h���N���X
	class COneHandle : public CPointerHandleT<TOne>, public ISerializable
	{
		DEFSUPER(CPointerHandleT<TOne>);
	public:
		/// �R���X�g���N�^ 
		COneHandle(void) {}
		/**
		 * ����R���X�g���N�^.
		 *	@param P new �Ŋm�ۂ��� TOne �|�C���^�B
		 */
		COneHandle(TOne* P) : _super(P) {}
		/**
		 * �R�s�[�R���X�g���N�^.
		 *	@param other �����^�̃C���X�^���X�B
		 */
		COneHandle(const COneHandle& other) : _super(other) {}
		/**
		 * [�ʒm] �V���A���C�Y
		 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
		 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
		 */
		virtual void Serialize(ISerializer& _sr) const
		{
			_sr << ((*this)->value);
			size_t l = 0;
			if ( ! ((*this)->hpMap.IsNull()) )
			{
				l = (*this)->hpMap->GetSize();
			}
			_sr << l;
			if ( l > 0 )
			{
				_sr << *((*this)->hpMap);
			}
		}
		/**
		 * [�ʒm] �f�V���A���C�Y
		 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
		 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
		 */
		virtual void Deserialize(const IDeserializer& ds)
		{
			COneHandle o = new TOne;
			ds >> (o->value);
			size_t l = 0;
			ds >> l;
			if ( l > 0 )
			{
				o->hpMap = new CMapKey;
				ds >> *(o->hpMap);
			}
			*this = o;
		}
	};

	typedef typename CMapKey::CConstKeysAdapter CConstKeysAdapter;	///< �L�[�ꗗ�p�^

	#endif //_TnbDOXYGEN

	COneHandle	m_one;				///< �ێ����Ă���Tree�̑匳

public:

	/**@ingroup TREE
	 * �c���[�^���Ǘ��p �S�L�[�����R�}���h�C���^�[�t�F�[�X
	 *
	 *	@note	�{�C���^�[�t�F�[�X���������A CTreeT::Scan() �ɓn�����ƂŁA
	 *			�S�����X�L�������A�L�[�A�l�������{�C���^�[�t�F�[�X�ɒʒm����܂��B
	 *
	 *	@see CTreeT::Scan
	 */
	struct ICommand
	{
		/// �f�X�g���N�^
		virtual ~ICommand(void) {}

		/**
		 * [�ʒm] �L�[�ƒl����
		 *	@param iNestLevel �[��
		 *	@param key ���������L�[�B
		 *	@param value ���������L�[���̒l�B
		 *	@param tree ���������L�[������TREE�B
		 *	@retval true �����p��
		 *	@retval false ������~
		 */
		virtual bool OnFound(int iNestLevel, const KEY& key, const VAL& value, const CTreeT& tree) = 0;
	};

private:

	/**
	 * [����] �S�v�f����.
	 *		�����������A ICommand �C���^�[�t�F�[�X�ɒʒm���܂��B
	 *	@param[in]	I �R�}���h
	 *	@param[in]	tree �Ώۂ� tree
	 *	@param[out]	_iNestLevel �l�X�g�[��
	 *	@retval true �Ō�܂ő���
	 *	@retval false ���f
	 */
	bool m_Scan(ICommand* I, const CTreeT& tree, int& _iNestLevel)
	{
		_iNestLevel++;
		CTreeT::CConstKeysAdapter vkey = tree.GetKeysAdapter();
		loop ( i, vkey.GetSize() )
		{
			const KEY& k = vkey.At(i);
			ASSERTLIB(tree.HasKey(k));
			if ( ! I->OnFound(_iNestLevel, k, tree.At(k), tree) )
			{
				return false;
			}
			if ( tree.HasLeaf(k) && ! m_Scan(I, tree(k), _iNestLevel) )
			{
				return false;
			}
		}
		_iNestLevel--;
		return true;
	}

public:

	/**
	 * �R���X�g���N�^ 
	 *	@note �}������Ȃ�Tree
	 */
	CTreeT(void) : m_one(new TOne)
	{
		m_one->hpMap = new CMapKey;
	}

	/** 
	 * �R�s�[�R���X�g���N�^
	 *	@note ���̋��L�����܂��B
	 *	@param other �R�s�[��
	 */
	CTreeT(const CTreeT& other) : m_one(other.m_one) 
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CTreeT(void)
	{
		m_one.Null();
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@attention ���̋��L�����܂��B
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CTreeT& operator=(const CTreeT& other)
	{
		m_one = other.m_one;
		return *this;
	}

	/**
	 * [�擾] �����̏��̎Q��
	 *	@return ���(�l�Ǝ})
	 */
	const VAL& AtSelf(void) const
	{
		return m_one->value;
	}

	/**
	 * [�擾] �����̏��̎Q��
	 *	@return ���(�l�Ǝ})
	 */
	VAL& AtSelf(void)
	{
		return m_one->value;
	}

	/**
	 * [�m�F] �L�[�L��
	 *	@param key �L�[
	 *	@retval true ���݂���
	 *	@retval false ���݂��Ȃ�
	 */
	bool HasKey(INK key) const
	{
		return m_one->hpMap->HasKey(key);
	}

	/**
	 * [�擾] �L�[���̒l�̎Q��
	 *	@param key �L�[
	 *	@return �l�̎Q��
	 *	@throw CEmptyException �L�[�����݂��Ȃ����ɃX���[����܂��B 
	 */
	const VAL& At(INK key) const
	{
		if ( HasKey(key) )
		{
			return 	m_one->hpMap->operator[](key)->value;
		}
		throw CEmptyException();
	}

	/**
	 * [�擾] �L�[���̒l�̎Q��
	 *	@param key �L�[
	 *	@return �l�̎Q��
	 *	@throw CEmptyException �L�[�����݂��Ȃ����ɃX���[����܂��B 
	 */
	const VAL& operator[](INK key) const
	{
		return At(key);
	}

	/**
	 * [�擾] �L�[���̒l�̎Q��
	 *	@note �L�[���Ȃ��ꍇ�A�ǉ����܂��B	
	 *	@param key �L�[
	 *	@return �l�̎Q��
	 */
	VAL& At(INK key)
	{
		if ( ! HasKey(key) )
		{
			COneHandle P = new TOne;
			P->value = VAL();
			m_one->hpMap->SetKey(key, P);
		}
		return 	m_one->hpMap->operator[](key)->value;
	}

	/**
	 * [�擾] �L�[���̒l�̎Q��
	 *	@note �L�[���Ȃ��ꍇ�A�ǉ����܂��B	
	 *	@param key �L�[
	 *	@return �l�̎Q��
	 */
	VAL& operator[](INK key)
	{
		return At(key);
	}

	/**
	 * [�擾] �L�[���̒l�擾
	 *	@param key �L�[
	 *	@return �l�̎Q��
	 *	@throw CEmptyException �L�[�����݂��Ȃ����ɃX���[����܂��B 
	 */
	VAL Get(INK key) const
	{
		return At(key);
	}

	/**
	 * [�擾] �L�[�̐�
	 *	@return ��
	 */
	size_t GetSize(void) const
	{
		return m_one->hpMap->GetSize();
	}

	/**
	 * [�擾] �L�[�ꗗ
	 *	@return �ꗗ
	 */
	CVectorT<KEY> EnumKeys(void) const
	{
		return m_one->hpMap->EnumKeys();
	}

	/**
	 * [�ݒ�] �L�[���̒l�ݒ�
	 *	@param key �L�[
	 *	@param v �l
	 *	@retval true �ݒ萬��
	 *	@retval false �L�[�͑��݂��Ȃ�
	 */
	bool Set(INK key, VAL v)
	{
		if ( HasKey(key) )
		{
			m_one->hpMap->operator[](key)->value = v;
			return true;
		}
		return false;
	}

	/**
	 * [�ǉ�] �L�[���̒l�ǉ�
	 *	@param key �L�[
	 *	@param v �l
	 *	@retval true �ǉ�����
	 *	@retval false �L�[�͑��݂��Ă���
	 */
	bool Add(INK key, VAL v)
	{
		if ( HasKey(key) )
		{
			return false;
		}
		COneHandle P = new TOne;
		P->value = v;
		m_one->hpMap->SetKey(key, P);
		return true;
	}

	/**
	 * [�폜] �L�[���̒l�Ǝ}���폜
	 *	@param key �L�[
	 *	@retval true ����
	 *	@retval false ���s�i�L�[�����݂��Ȃ��j
	 */
	bool Remove(INK key)
	{
		if ( ! HasKey(key) )
		{
			return false;
		}
		return m_one->hpMap->RemoveKey(key);
	}
	
	/**
	 * [�m�F] �L�[�͎}������
	 *	@param key �L�[
	 *	@retval true �����Ă���
	 *	@retval false �����Ă��Ȃ�
	 */
	bool HasLeaf(INK key) const
	{
		if ( ! HasKey(key) )
		{
			return false;
		}
		const CPointerHandleT<TOne>& c = m_one->hpMap->operator[](key);
		if ( c->hpMap.IsNull() )
		{
			return false;
		}
		return ! c->hpMap->IsEmpty();
	}

	/**
	 * [�ǉ�] �L�[���Ɏ}��ǉ�
	 *	@note �L�[������Ǝ��s���܂��B
	 *	@attention	�ǉ�����TREE���A�����̎q���A�e�łȂ����Ƃ�ۏႵ�Ă���}���w�肵�Ă��������B
	 *	@param key �L�[
	 *	@param other �}
	 *	@retval true �ǉ�����
	 *	@retval false �L�[�͑��݂��Ă���
	 */
	bool AddLeaf(INK key, CTreeT<KEY,VAL,INK>& other)
	{
		if ( HasKey(key) )
		{
			return false;
		}
		//add �����̎q���A�e�łȂ����Ƃ��m�F�B
		m_one->hpMap->SetKey(key, other.m_one);
		return true;
	}

	/**
	 * [�폜] �l�Ǝ}���폜
	 */
	void RemoveAll(void)
	{
		m_one = new TOne;
		m_one->hpMap = new CMapKey;
	}

	/**
	 * [�擾] �L�[�݂̂̎Q�ƃA�_�v�^�擾. 
	 *		�L�[�̂ݎQ�ƃA�_�v�^�N���X���쐬���܂��B
	 *	@return CConstKeysAdapter
	 */
	CConstKeysAdapter GetKeysAdapter(void) const
	{
		return m_one->hpMap->GetKeysAdapter();
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@note �擾����Tree�͑���\�ł��B���삷��ƌ���Tree�ɉe�����L��܂��B
	 *	@note ���݂��Ȃ��L�[���w�肷��Ɗg������܂��B
	 *	@param key �L�[
	 *	@return Tree
	 */
	CTreeT<KEY,VAL,INK> Refer(INK key)
	{
		if ( ! HasKey(key) )
		{
			COneHandle P = new TOne;
			P->value = VAL();
			m_one->hpMap->SetKey(key, P);
		}
		COneHandle& c = m_one->hpMap->operator[](key);
		if ( c->hpMap.IsNull() )
		{
			c->hpMap = new CMapKey;
		}
		CTreeT t;
		t.m_one = c;
		return t;
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@note �擾����Tree�͑���\�ł��B���삷��ƌ���Tree�ɉe�����L��܂��B
	 *	@note ���݂��Ȃ��L�[���w�肷��Ɗg������܂��B
	 *	@param key �L�[
	 *	@return Tree
	 */
	CTreeT<KEY,VAL,INK> operator()(INK key)
	{
		return Refer(key);
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@param key �L�[
	 *	@return Tree
	 *	@throw CEmptyException �L�[�̉��̎}�����݂��Ȃ����ɃX���[����܂��B 
	 */
	const CTreeT<KEY,VAL,INK> Refer(INK key) const
	{
		if ( ! HasKey(key) )
		{ 
			throw CEmptyException();
		}
		const COneHandle& c = m_one->hpMap->operator[](key);
		if ( c->hpMap.IsNull() )
		{
			throw CEmptyException();
		}
		CTreeT t;
		t.m_one = c;
		return t;
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@param key �L�[
	 *	@return Tree
	 *	@throw CEmptyException �L�[�̉��̎}�����݂��Ȃ����ɃX���[����܂��B 
	 */
	const CTreeT<KEY,VAL,INK> operator()(INK key) const
	{
		return Refer(key);
	}

	/**
	 * [�ʒm] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << m_one;
	}

	/**
	 * [�ʒm] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> m_one;
	}

	/**
	 * [����] �����쐬
	 *	@return �����B�ύX���Ă��{�C���X�^���X�ɉe���L��܂���B
	 *	@todo ���݃V���A���C�Y�@�\���g���ĕ������Ă��܂��B���̂��ߏ������x�͒x���ł��B
	 */
	CTreeT<KEY,VAL,INK> Clone(void) const
	{
		CVectorT<BYTE> vb;
		CSerializeAdapter sr(&vb);
		sr << m_one;
		CTreeT t;
		CDeserializeAdapter ds(&vb);
		ds >> t.m_one;
		return t;
	}

	/**
	 * [����] �S�v�f����.
	 *		�����������A ICommand �C���^�[�t�F�[�X�ɒʒm���܂��B
	 *	@param I �R�}���h
	 *	@retval true �Ō�܂ő���
	 *	@retval false ���f
	 */
	bool Scan(ICommand* I)
	{
		int iNestLevel = 0;
		if ( ! I->OnFound(iNestLevel, KEY(), AtSelf(), *this) )
		{
			return false;
		}
		return m_Scan(I, *this, iNestLevel);
	}

private:
	friend class CTreeTest;
};



/**@ingroup TREE
 * �c���[�^��������Ǘ��N���X.
 *		�L�[=�����A�l=�����̃c���[���Ǘ��ł��܂��B
 *	@par�K�v�t�@�C��
 *			TnbTree.h
 */
class CStrsTree : public CTreeT<CStr,CStr,LPCTSTR>
{
	typedef CTreeT<CStr,CStr,LPCTSTR> _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	class CToStringCommand : public _super::ICommand
	{
		CStr m_str;		///< ����
		/**
		 * [�ʒm] �L�[�ƒl����
		 *	@param iNestLevel �[��
		 *	@param key ���������L�[�B
		 *	@param value ���������L�[���̒l�B
		 *	@param tree ���������L�[������TREE�B
		 *	@retval true �����p��
		 *	@retval false ������~
		 */
		virtual bool OnFound(int iNestLevel, const CStr& key, const CStr& value, const _super& tree)
		{
			m_str += CStr::Fmt(_T("%s[+]%s = %s\n")
							, CStr::Lineup(_T(" |"), iNestLevel)
							, key
							, value);
			return true;
		}
	public:
		CStr GetString(void) { return m_str; }
	};

	#endif //_TnbDOXYGEN

public:
	/** �R���X�g���N�^ */
	CStrsTree(void) : _super()
	{
	}

	/** 
	 * �R�s�[�R���X�g���N�^
	 *	@note ���̋��L�����܂��B
	 *	@param other �R�s�[��
	 */
	CStrsTree(const CTreeT<CStr,CStr,LPCTSTR>& other) : _super(other) 
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@attention ���̋��L�����܂��B
	 *	@param other �R�s�[��
 	 *	@return �����̎Q��
	 */
	CStrsTree& operator=(const CTreeT<CStr,CStr,LPCTSTR>& other)
	{
		_super::operator=(other);
		return *this;
	}

	/**
	 * [�ϊ�] �P�ꕶ����
	 *	@return �ϊ���̕�����
	 */
	CStr ToString(void)
	{
		CToStringCommand c;
		_super::Scan(&c);
		return c.GetString();
	}
	/**
	 * [�擾] �L�[����Tree�擾
	 *	@note �擾����Tree�͑���\�ł��B���삷��ƌ���Tree�ɉe�����L��܂��B
	 *	@note ���݂��Ȃ��L�[���w�肷��Ɗg������܂��B
	 *	@param key �L�[
	 *	@return Tree
	 */
	CStrsTree Refer(LPCTSTR key)
	{
		return _super::Refer(key);
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@note �擾����Tree�͑���\�ł��B���삷��ƌ���Tree�ɉe�����L��܂��B
	 *	@note ���݂��Ȃ��L�[���w�肷��Ɗg������܂��B
	 *	@param key �L�[
	 *	@return Tree
	 */
	CStrsTree operator()(LPCTSTR key)
	{
		return Refer(key);
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@param key �L�[
	 *	@return Tree
	 *	@throw CEmptyException �L�[�̉��̎}�����݂��Ȃ����ɃX���[����܂��B 
	 */
	const CStrsTree Refer(LPCTSTR key) const
	{
		return _super::Refer(key);
	}

	/**
	 * [�擾] �L�[����Tree�擾
	 *	@param key �L�[
	 *	@return Tree
	 *	@throw CEmptyException �L�[�̉��̎}�����݂��Ȃ����ɃX���[����܂��B 
	 */
	const CStrsTree operator()(LPCTSTR key) const
	{
		return Refer(key);
	}

	/**
	 * [����] �����쐬.
	 *	@return �����B�ύX���Ă��{�C���X�^���X�ɉe���L��܂���B
	 *	@todo ���݃V���A���C�Y�@�\���g���ĕ������Ă��܂��B���̂��ߏ������x�͒x���ł��B
	 */
	CStrsTree Clone(void) const
	{
		return _super::Clone();
	}
};



}; // TNB



