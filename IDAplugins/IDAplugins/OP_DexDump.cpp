#include <Util_LIB.HPP>
/*
*			IDA���Dex Dumpģ��
*��Ҫ����.ʵ��
*	1.
*	2.
*/
//���¶���UI��ģʽö��
const char ASK_MAIN_UI[] = "STARTITEM  0\n\n"
"<#�����ڴ�# �����ڴ�DEX:R:32:16:>\n"
"<#�������ڴ�# �������ڴ�:R:32:16:>\n"
"<#Helper# ���DEX��Ϣ:R:32:16:>>\n";
enum{
	MAIN_ConDex,
	MAIN_UnConDex
};
int Mode = 0;
int Dex_Moudle(){
	if (AskUsingForm_c(ASK_MAIN_UI, &Mode) == 0)return -1;
	ea_t mStart = get_screen_ea();
	sval_t mSize = 0;
	if (asklong(&mSize,"����DEX���") == 0)return -1;
	IDA_Dex::IDA_DunmDex(mStart, mSize);
//




	return 0;
}