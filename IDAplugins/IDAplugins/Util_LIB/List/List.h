#include "Util_LIB.H"
/**
* @See		�������������λ�ṹ��
* @Struct	LinkNode
*/
template <class Type>

struct LinkNode{
	Type*	info;
	struct LinkNode*	_N_next;
	struct LinkNode*	_N_prev;
};
/**
* @See		LinkNode����������
* @Class	_LinkNode_Base
*/
template <class Type>
class _LinkNode_Base {
public:
typedef	LinkNode<Type>*		LP_Node;
/**
* @See		����һ�������ݵĻ�����λ�ṹ��
* @Return	���ػ�����λ�ṹ��ָ��
*/
	static LP_Node CreatVoidNode(){
		return(LP_Node)Util_Base::Alloc(sizeof(struct LinkNode<Type>));
	}
/**
* @See		����������λ�ṹ��
* @Return	���ػ�����λ�ṹ��ָ��
*/
	static LP_Node CreatNode(Type* inTy){
		LP_Node _out = CreatVoidNode();
		_out->info = inTy;
		return _out;
	}
};
/**
* @See		�б����������
* @Class	_List_Base
*/
template <class Type>
class _List_Base :public _LinkNode_Base<Type>{
public:
	LP_Node		First,Last;
	size_t		Length;
/**
* @See		���������������
*/
	void Clear(){
// ��������
/*		if ((First != NULL) && (Last != NULL)){
			LP_Node _m_point = Last;
			while (_m_point != NULL){
				_m_point->info = NULL;
				_m_point = _m_point->_N_prev;
				free(_m_point->_N_next);
				_m_point->_N_next = NULL;
			}
		}**/
		_List_Base();
	}
/**
* @See	��ʼ���б�
*/
	_List_Base(){
		First = Last = NULL;
		Length = 0;
	}
};
/**
* @See		�б���
* @Class	List
*/
template <class Type>
class List :public _List_Base<Type>{
public:
	LP_Node _LinkThis = NULL;
	ulong _NoThis = 0;
/**
* @See		������ݣ���ӵ��б�β��
* @Param	��������
*/
	void Inster(Type* _inTy){
		//�ж��б��Ƿ�Ϊ��
		if (First == NULL){
			Last = First = CreatNode(_inTy);
			Length = 1;
			return;
		}
		Last->_N_next = CreatNode(_inTy);
		Last->_N_next->_N_prev = Last;
		Last = Last->_N_next;
		Length++;
	}
/**
* @See		��ȡ�б�����
* @Param	��������
*/
	Type* Get(int inNo){
		LP_Node _OutTyre = First;
		int m_i = 0;
		while ((m_i < inNo) && (_OutTyre != NULL)){
			_OutTyre = _OutTyre->_N_next;
			m_i++;
		}
		return _OutTyre->info;
	}
/**
* @See		��ȡ������
* @Return	����������
*/
	size_t GetLength(){
		return Length;
	}
/*-----------------------------------------------------------------------------------*/
/*						���º���Ϊ�˲����������õ�									*/
/**
* @See		��λThisָ��
*/
	void Reset(){
		_LinkThis = First;
		_NoThis = 0;
	}
/**
* @See		��ȡThisָ�������
* @Return	This������
*/
	Type* Get(){
		if (_LinkThis == NULL)return NULL;
		return _LinkThis->info;
	}
/**
* @See		��ȡThis��λ��
* @Return	This������
*/
	ulong GetPlace(){
		return _NoThis;
	}
/**
* @See		��ȡThisָ��ָ����һ������
*/
	void Next(){
		if (_LinkThis == NULL)return ;
		_LinkThis = _LinkThis->_N_next;
		_NoThis++;
	}
/**
* @See		��ʼ���б�
*/
	List(){
		_List_Base();
	}
};

