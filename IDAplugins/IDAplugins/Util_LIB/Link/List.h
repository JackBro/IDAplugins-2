#include "Util_LIB.H"
template <class Type_Node>class ListNode{
public:
	char* name;
	Type_Node Data;
	ListNode* next;
	ListNode* prev;
/*��������*/
	void SetName(char *in_name) {
		name = in_name;
	}
/*����Node����*/
	void SetLinkNode(Type_Node *in_Node) {
		memcpy(&Data, in_Node, sizeof(Type_Node));
	}
	void SetNode(Type_Node in_Node) {
		Data = in_Node;
	}
/*����LIST*/
	void SetNext(ListNode* in_listnode){
		next = in_listnode;
	}
	void SetPrev(ListNode* in_listnode){
		prev = in_listnode;
	}
/*���캯��*/
	ListNode(){
		name = NULL;
		next = prev = NULL;
	}
	ListNode(Type_Node in_Node){
		ListNode();
		memcpy(&Data, &in_Node, sizeof(Type_Node));
	}
	ListNode(Type_Node* in_Node){
		ListNode();
		memcpy(&Data,in_Node, sizeof(Type_Node));
	}
	ListNode(char* in_name, Type_Node* in_Node){
		ListNode(in_Node);
		name = in_name;
	}
};
/*
*/
template <class Type>class List{
public:
	ListNode<Type>*First = NULL, *Last = NULL;
//
	void addNode(Type in_node){
		if (in_node == NULL)return;
		ListNode<Type>* m_node = new ListNode<Type>(in_node);
		if ((First == NULL) || (Last == NULL)){	
			First = Last = m_node;
		}else{
			m_node->prev = Last;
			Last->next = m_node;
			Last = Last->next;
		}
	}
/*���һ���ڵ㵽�б���*/
	void addLink(Type* in_node){
		if (in_node == NULL)return;
		ListNode<Type>* m_node = new ListNode<Type>(in_node);
		if ((First == NULL) || (Last == NULL)){
			First = Last = m_node;
		}
		else{
			m_node->prev = Last;
			Last->next = m_node;
			Last = Last->next;
		}
	}
/*���ݽڵ��ȡ���ڵ�����*/
/*	int GetNo(Type* in_Type){
		int m_no = 0;
		ListNode<Type>* po_Find = First;
		while (po_Find!= NULL){
			if (po_Find->Data == in_Type)return m_no;
			m_no++;
		}
		return -1;	
	}
/*���ݽڵ���λ��ȡ*/
	ListNode<Type>* Get(int in_No){
		ListNode<Type>* po_type = First;
		while (in_No--){
			if (po_type == NULL)return NULL;
			po_type = po_type->next;
		}
		return po_type;
	}
/*
..	Name : GetNameNode
..	Function :����ȡ������
*/
	Type* GetNameNode(char* in_Name){
		if (in_Name == NULL)return NULL;
		ListNode<Type>* po_type = First;
		while (po_type != NULL){
			if (!strcmp(po_type->name, in_Name)){
				return po_type->Data;
			}
			po_type = po_type->next;
		}
		return NULL;
	}
/*
..	Name : GetName()
..	Function :����ȡ������������Ϊ<in_Name>�Ľڵ�
*/
	ListNode<Type>* GetName(char* in_Name){
		if (in_Name == NULL)return NULL;
		ListNode<Type>* po_type = First;
		while (po_type != NULL){
			if (!strcmp(po_type->name, in_Name)){
				return po_type;
			}
			po_type = po_type->next;
		}
		return NULL;
	}
/*
..	Name : getLength
..	Function :����ȡ������
*/
	int getLength(){
		int m_i = 0;
		ListNode<Type>* po_type = First;
		while (po_type != NULL){
			po_type = po_type->next;
			m_i++;
		}
		return m_i;
	}
/*
..	Name : List
..	Function :�����캯��
*/
	List(){
		First = Last = NULL;
	}
};
/**/