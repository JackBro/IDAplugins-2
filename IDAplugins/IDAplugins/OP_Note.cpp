#include <Util_LIB.H>
/*
*			IDA���Noteģ��
*��Ҫ����.
*	1.ע�ͱ���
*/
//���¶���UI��ģʽö��
const char ASK_ARM_UI[] = "Note Options\n\n\n"
						"<#ARM Code����# ~C~ode:R>\n"
						"<#ARM ϵͳ����ע��# ~S~ysCall:R>>\n";
enum{
	MODE_ARMOP_Code,
	MODE_ARMOP_SysCall,
};