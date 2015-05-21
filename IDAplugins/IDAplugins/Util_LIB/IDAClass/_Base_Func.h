#include "Util_LIB.H"
class _Base_Func{
public:
	ea_t StartEA;
	char* Name;
	char* Cmt;
	char* ReCmt;
/**
* @See		��ȡIDA ��������
* @Param��	inFun -> IDA������
* @Return��	Out_Sec -> ��������
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
* @Param��	inIni -> INI��
* @Return��	inSecName -> ����
*/
	void ToIni(INI* inIni, char* inSecName){
		inIni->addIntValue(inSecName, "StartEA", StartEA);
		if (Name != NULL)
			inIni->addStrValue(inSecName, "Name", Name);
		if (Cmt != NULL)
			inIni->addStrValue(inSecName, "Cmt", Cmt);
		if (ReCmt != NULL)
			inIni->addStrValue(inSecName, "ReCmt", ReCmt);
	}
/**
* @See		��ʼ��_Base_Func
* @Param��	inSeg -> IDA ����
* @Param��	inFun -> IDA ������
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
/**
* @See		��ʼ��_Base_Func
* @Param��	inSection -> INI�ļ� ��
*/
	_Base_Func(C_INI_Section* inSection){
		if (inSection == NULL)return;
		StartEA = inSection->GetInt("StartEA");
		Name = inSection->GetString("Name");
		Cmt = inSection->GetString("Cmt");
		ReCmt = inSection->GetString("ReCmt");
	}
};

