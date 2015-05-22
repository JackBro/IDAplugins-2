#define ___NR_SysCall_H_
#include <Util_LIB.H>
/*
*			IDA���ARMѡ��
*��Ҫ����.
*	1.ARM Code����
*	2.ARM ϵͳ��������
*/
//���¶���UI��ģʽö��
const char ASK_ARM_UI[] = "ARM Options\n\n\n"
			"<#ARM Code����# ~C~ode:R>\n"
			"<#ARM ϵͳ����ע��# ~S~ysCall:R>>\n";
enum{
	MODE_ARMOP_Code,
	MODE_ARMOP_SysCall,
};
//Code����
const char ASK_CODE_UI[] = "ARM CODE\n\n\n"
			"<#Thumb# Mode 16Bit:R>\n"
			"<#ARM# Mode 32Bit:R>>\n";
//SysCallUI����
const char ASK_SYSCALL_UI[] = "STARTITEM  0\n"
				"����ϵͳ���õĲ���\n"
				"<~S~ys Num(Hex):N:32:16::>\n";

/*
*				ģʽ˵��
*	1��MODE_ARMOP_Code -> ʹ��ARMָ���޸�CODE
*	2��MODE_ARMOP_SysCall -> ע��ϵͳ����
*/
int ModeOption = 0;
int Arm_Moudle(){
	int Mode_Bit = 0;
	if (AskUsingForm_c(ASK_ARM_UI, &ModeOption) == 0)return -1;
	switch (ModeOption){
	case MODE_ARMOP_Code:
		_MSG("Entry ARM Code!\n");
		if (AskUsingForm_c(ASK_CODE_UI, &Mode_Bit) == 0)return 0;

	break;
	case MODE_ARMOP_SysCall:
		_MSG("Entry ARM SysCall!\n");
		//�Զ���ȡ
		ulong Sys_No = get_32bit(get_screen_ea()) & 0xFFF;
		if (AskUsingForm_c(ASK_SYSCALL_UI, &Sys_No) == 0)return 0;
		if (SysCall::getName(Sys_No) != NULL){
			set_cmt(get_screen_ea(), SysCall::getName(Sys_No), 1);
		}
	break;
	}
	return 0;
}
