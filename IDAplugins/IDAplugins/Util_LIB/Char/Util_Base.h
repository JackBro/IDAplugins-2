#include <Util_LIB.HPP>
class Util_Base{
public:
/**
* @See	��ȡ����յ��ڴ�
* @Param��inSize -> �����ڴ��ȡ���
* @Return��out_Mem -> ������ڴ���ʼ��ַ
*/
	static void* Alloc(ulong inSize){
		void* out_Mem = malloc(inSize+1);
		memset(out_Mem, 0, inSize + 1);
		return out_Mem;
	}
/**
* @See 
* @Class 
* @Struct
* @Function 
* @Param 
* @Return 
* @Since
*/
};

