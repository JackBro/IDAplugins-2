#include "Util_LIB.H"

class _Base_Func{
public:
	ea_t StartEA;
	char* Name;
	char* Cmt;
	char* ReCmt;
/*
*/
	static char* GetFuncName(func_t* inFun){
		char* Out = NULL;
		char* Buf = (char*)Util_Base::Alloc(10240);
		if (get_func_name(inFun->startEA, Buf, 10240) != NULL){
			Out = strdup(Buf);
		}
		free(Buf);
		return Out;
	}
/**
* @See		������ת����INI��ģʽ
* @Param��	inName -> ������
* @Return��	Out_Sec -> INI����
*/
	C_INI_Section* ToIniSection(char* inName){
		C_INI_Section* Out_Sec = new C_INI_Section(inName);
		Out_Sec->AddInt("StartEA", StartEA);
		Out_Sec->AddString("Name", Name);
		if (Cmt != NULL)
			Out_Sec->AddString("Cmt", Cmt);
		if (ReCmt != NULL)
			Out_Sec->AddString("ReCmt", ReCmt);
		return Out_Sec;
	}
/**
* @See		��ʼ��_Base_Func
*/
	_Base_Func(segment_t* inSeg,func_t* inFun){
		StartEA = 0;
		Name = Cmt = ReCmt = NULL;
		if (inFun == NULL)return;
		//��ȡƫ��
		StartEA = inFun->startEA - inSeg->startEA;
		//��ȡ���֡�ע��
		Name = GetFuncName(inFun);
		Cmt = _Base_Cmt::GetCmt(inFun->startEA,0);
		ReCmt = _Base_Cmt::GetCmt(inFun->startEA, 0);
	}
};

