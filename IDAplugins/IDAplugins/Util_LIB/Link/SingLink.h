#include "Util_LIB.H"
/*
..����������ģ��,���ڱ���ģ�����ݺ�ָ����һ������
*/
template <class Type>class LinkNode {
public:
	char* IDName;
	Type Data;				//���ݱ���
	LinkNode* next;
/*��������*/
	void SetIDName(char *in_name) {
		IDName = strdup(in_name);
	}
/*���ýڵ�����*/
	void SetType(Type in_Type) {
		memcpy(&Data, &in_Value, sizeof(Type));
	}
	void SetType(Type* in_Type) {
		memcpy(&Data,in_Value, sizeof(Type));
	}
/*����Ϊ��*/
	void Empty() {
		next = NULL;
		TypeName = NULL;
		Data = NULL;
	};
/*��ȡ��һ����������*/
	Type* GetNext(){
		return next;
	}
/*ɾ����һ���ڵ�*/
	void RemoveAfter(){
		if (nextLink == NULL)return;
		next = next->next;
	}
/*��ʼ��ģ������*/
	LinkNode(Type in_Value) {
		memcpy(&Data, &in_Value, sizeof(Type));
		IDName = NULL;
		next = NULL;
	}
	/*��ʼ��ģ������*/
	LinkNode(char* in_name,Type in_Value){
		memcpy(&Data, &in_Value, sizeof(Type));
		IDName = strdup(in_name);
		next = NULL;
	}
};
/*
//			����������ģ��
//	1.���ڶ�ȡ���������롢�Ƴ��ȵ�
*/
template <class Type>class SingLink{
public:
	int Length = 0;
	LinkNode<Type> *FirstList,*LastList;
/*��ĩβ����һ���ڵ�*/
	void add(Type value) {
		if (value == NULL)return;
		if (FirstList == NULL) {
			LastList = FirstList = new LinkNode<Type>(value);
			Length = 1;
			return;
		}
		Length++;
		LastList->next = new LinkNode<Type>(value);
		LastList = LastList->next;
	}
/*���һ�����������ֽ�,*/
	void add(char * in_Name,Type in_Value) {
		/*�Ȳ����Ƿ���������ͬ��NODE���еĻ�����NODE*/
		if (Get(in_Name) != NULL){
			Get(in_Name)->Data = new LinkNode<Type>(in_Value);
			return;
		}
		Length++;
		if (in_Value == NULL)return;
		if (FirstList == NULL) {
			LastList = FirstList = new LinkNode<Type>(in_Value);
			return;
		}
		LastList->next = new LinkNode<Type>(in_Value);
		LastList = LastList->next;
	}
/*��ȡ��N������ṹ�е�����*/
	LinkNode<Type>* Get(int in_no){
		LinkNode<Type>* retLink = FirstList;
		while (in_no--){
			/*��Ҫ��ȡ�ĳ��Ȳ�û���ڻ�ȡ���ݵĳ�����*/
			if (retLink == NULL)return NULL;
			retLink = retLink->next;
		}
		return retLink;
	}
/*��ȡ��N������ṹ����*/
	Type* GetData(char* in_Name){
		if (Get(in_Name) != NULL)return &Get(in_Name)->Data;
		return NULL;
	}
	Type* GetData(int in_No){
		if (Get(in_Name) != NULL)return &Get(in_Name)->Data;
		return NULL;
	}
/*�Ƴ���(in_no)���ڵ�*/
	void Remove(int in_no){
		LinkNode<Type>* mLink  = Get(in_no);
		if (mLink == NULL) return;
		Get(in_no - 1)->next = Get(in_no + 1);
		delete  mLink;
	}
/*�ڣ�in_no���ڵ����in_Link����*/
	int Insert(int in_no,Type in_Link){
		SingLinkNode<Type>* mAgo = Get(in_no-1);
		SingLinkNode<Type>* mLast = Get(in_no);
		if (mAgo == NULL)return -1;
		if (in_no == 0){
			LinkNode<Type>* mBack = FirstList;
			FirstList = new LinkNode<Type>(in_Link);
			FirstList->next = mBack;
			return 0;
		}
		mAgo->next = new LinkNode<Type>(in_Link);
		mAgo->next->next = mLast;
		return 0;
	}
/*���б�����*/
	void MarkEmpty() {
		FirstList = LastList = NULL;
		Length = 0;
	}
/*��ʼ������*/
	SingLink(){
		MarkEmpty();
	}
};



