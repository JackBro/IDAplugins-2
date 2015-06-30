#define __IDA_MUL_DEBUG_C_
#include"IDA_Header.H"
//���������ڴ棬
#pragma data_seg("Debug-Module")
ea_t PageSize = 0;
ea_t Server_StartEA = NULL;
ea_t Server_EndEA = NULL;
ea_t Server_ThisEA = NULL;
ea_t Client_StartEA = NULL;
ea_t Client_EndEA = NULL;
#pragma data_seg() 
//
static bool BFirst_IDA = true;
int DebugMode = 0;
int Handler_Time = 0;
boolean RealTime = FALSE;
boolean ForceZero = FALSE;
//���¶���UI��ģʽö��
const char ASK_DEBUG_UI[] ="Debug Module\n\n\n"
						"<##ģʽѡ��## #���õ�ַ#~S~erver:R:32:16:>\n"
						"<#�ͻ��˵�ַ# ~C~lient:R:32:16::>\n"
						"<#����IDA����# ~I~DA Server:R:32:16::>"
						"<#����IDA Forward# ~I~DA Forward:R:32:16::>>"
						"<##����## #ʵʱ����# ~R~eal-time:C>\n"
						"<#��ַǿ�ж���# ~F~orce:C>>\n"
						"<##�����ַ����#�����С#�����СHex:M:8:10::>>\n";
enum{
	MODE_MULD_SAVE,
	MODE_MULD_READ,
	MODE_MULD_IDA_server,
	MODE_MULD_IDA_Forward,
};
/**
* @See	���÷���������ƫ�Ƶ�ַ
*/
void Set_ServerThis(){
	ea_t _EA = get_screen_ea();
	if ((_EA < Server_StartEA) || (_EA > Server_EndEA))return;
	if ((_EA - Server_StartEA) == Server_ThisEA)return;
	Util::MSG("Set_ServerThis()@ Update\n");
	Server_ThisEA = _EA - Server_StartEA;
}
/**
* @See	���¿ͻ��˵�ַ
*/
void Client_Update(){
	Util::MSG("Client_Update()@ Update\n");
	jumpto(Client_StartEA + Server_ThisEA);
}
/**
* @See	����˶�ʱ��
*/
VOID CALLBACK TIMER_Server(
	HWND hWnd,	/* handle of window for timer messages*/	UINT uMsg,	/* WM_TIMER message*/
	UINT idEvent,/*timer identifier*/	DWORD dwTime /*current system time*/){
	Set_ServerThis();
}
/**
* @See	�ͻ��˶�ʱ��
*/
VOID CALLBACK TIMER_Client(
	HWND hWnd,	/* handle of window for timer messages*/	UINT uMsg,	/* WM_TIMER message*/
	UINT idEvent,/*timer identifier*/	DWORD dwTime /*current system time*/){
	Client_Update();
}
/*
*			IDA�������ģ��
*��Ҫ����.
*		1.���õ�ַ
*		2.��ȡ��ַ
*��Ҫ����
*/
int Debug_Moude(int inFlag){
	//  �ָ��ϴ����õ���������
	ushort _CONFIG = 0;
	if (RealTime)_CONFIG |= 0x01;
	if (ForceZero)_CONFIG |= 0x02;
	Util::MSG("Debug_Moude()\n");
	KillTimer(NULL, Handler_Time);
	if (inFlag != Flag_Again)
		if (AskUsingForm_c(ASK_DEBUG_UI, &DebugMode, &_CONFIG, &PageSize) == 0)
			return NULL;
	Util::MSG("_CONFIG:%d,PageSize:0x%08x\n", _CONFIG, PageSize);
	//����Check��ȡ�����ݲ���
	if (_CONFIG & 0x01) RealTime = TRUE;
	else RealTime = FALSE;
	if (_CONFIG & 0x02) ForceZero = TRUE;
	else ForceZero = FALSE;
	//ģʽѡ��
	ea_t _Size = ~PageSize;
	if (MODE_MULD_SAVE == DebugMode){
		if (_Size == 0)Server_StartEA = 0;
		else{
			Server_StartEA = getseg(get_screen_ea())->startEA & _Size;
		}
		Server_EndEA = getseg(get_screen_ea())->endEA;
		Set_ServerThis();
		if (RealTime){
			Handler_Time = SetTimer(NULL, 0, 1000, TIMER_Server);
		}
	}
	else if (MODE_MULD_SAVE == DebugMode){


	}else if (MODE_MULD_READ == DebugMode){
		if (_Size == 0)Server_StartEA = 0;
		else{
			Server_StartEA = getseg(get_screen_ea())->startEA & _Size;
		}
		Client_Update();
		if (RealTime){
			Handler_Time = SetTimer(NULL, 0, 1000, TIMER_Client);
		}
	}
	else if (MODE_MULD_IDA_server == DebugMode){
		if (BFirst_IDA){
			WinExec("adb shell su -c pkill -9 IDA_server", SW_HIDE);
			Sleep(1000);
			WinExec("adb shell su -c /data/local/Debug/IDA_server", SW_HIDE);
			BFirst_IDA = false;
			Sleep(2000);
			WinExec("adb forward tcp:23946 tcp:23946", SW_HIDE);
		}
		else{
			WinExec("adb shell su -c /data/local/Debug/IDA_server", SW_HIDE);
		}
	}
	else if (MODE_MULD_IDA_Forward == DebugMode){
		WinExec("adb forward tcp:23946 tcp:23946", SW_HIDE);
	}
	return NULL;
}