#include <Util_LIB.H>
/*
*			IDA���Noteģ��
*��Ҫ����.
*	1��MODE_NOTE_UP -> ���Ѽ��صĶ�ȫ������,��������ļ�
*	2��MODE_NOTE_ADD -> ���ص�ǰ��
*	3��MODE_NOTE_FILE -> ���ļ��м���
*	4��MODE_NOTE_SAVE -> ������ļ�
*/
//���¶���UI��ģʽö��
int NoteMode = 0;
HANDLE HandlerNo = 0;
boolean MulThread = FALSE;
boolean WorkIng = FALSE;
char* SaveFile = NULL;
char* LoadFile = NULL;
Online_Save OnSave;
const char ASK_NOTE_UI[] = "Note Options\n\n\n"
						"<#�����еĶ�ȫ������,��������ļ�# ~U~pAll:R>\n"
						"<#���ص�ǰ��# ~A~ddSegment:R>\n"
						"<#���ļ��м���# ~F~orm File:R>\n"
						"<#������ļ�# ~S~ave File:R>\n"
						"<#��ӡ������# ~P~rintf :R>>\n"
						"<##����ѡ��##�Ƿ��������̴߳���:C>>\n";
enum{
	MODE_NOTE_UP,
	MODE_NOTE_ADD,
	MODE_NOTE_FILE,
	MODE_NOTE_SAVE,
	MODE_NOTE_PRINTF,
};
/*

*/
void RunMode(int inMode){
	ea_t _ea = get_screen_ea();
	char* _AutoBuf = NULL;
	switch (inMode){
	case MODE_NOTE_UP:
		if (OnSave.Seg.GetLength() == 0)return;
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
		_AutoBuf = (char*)Util_Base::Alloc(1024);
		OnSave.UpAllSegment();
		sprintf(_AutoBuf, 1024, "%s_Auto", SaveFile);
		OnSave.Save(_AutoBuf);
		free(_AutoBuf);
	break;
	case MODE_NOTE_ADD:
		OnSave.AddSegment(getseg(_ea));
	break;
	case MODE_NOTE_FILE:
		if (!MulThread)	LoadFile = askfile_c(1, "*.ini", "����ע���ļ�");
		if (LoadFile == NULL)return;
		OnSave.Online_Load(LoadFile);
	break;
	case MODE_NOTE_SAVE:
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
		OnSave.Save(SaveFile);
	break;
	case MODE_NOTE_PRINTF:


	break;
	}

}
/**
* @See	WINAPI���߳�
*/
DWORD WINAPI Mul_Hander(LPVOID lpParam){
	Debug::MSG("Mul_Hander(%d)@ is Run \n", NoteMode);
	RunMode(NoteMode);
	WorkIng = FALSE;
	Debug::MSG("Mul_Hander(%d)@ is End \n", NoteMode);
	CloseHandle(HandlerNo);
	return 0;
}
/*
*ģʽ˵��
*	1��MODE_NOTE_UP -> ���Ѽ��صĶ�ȫ������,��������ļ�
*	2��MODE_NOTE_ADD -> ���ص�ǰ��
*	3��MODE_NOTE_FILE -> ���ļ��м���
*	4��MODE_NOTE_SAVE -> ������ļ�
*����˵��
*	0x01 ->	�Ƿ����ö��߳�
*/
int Note_Moudle(){
	int _Config = MulThread;
	if (WorkIng){
		_MSG("֮ǰ����δ������ϣ����Ժ�����\n");
		return 0;
	}
	if (AskUsingForm_c(ASK_NOTE_UI, &NoteMode,&_Config) == 0)return -1;
	MulThread = _Config;
	if (_Config == 0){
		Debug::MSG("�����ö��߳�\n");
		MulThread = FALSE;
		RunMode(NoteMode);
	}
	else{
		Debug::MSG("���ö��߳�\n");
		if (NoteMode == MODE_NOTE_SAVE){
			SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
			if (SaveFile == NULL)return 0;
		}else if (NoteMode == MODE_NOTE_FILE){
			LoadFile = askfile_c(0, "*.ini", "����ע���ļ�");
			if (LoadFile == NULL)return 0;
		}
		else if (NoteMode == MODE_NOTE_UP){
			if (SaveFile == NULL){
				SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
				if (SaveFile == NULL)return 0;
			}
		}
		MulThread = TRUE;
		WorkIng = TRUE;
		HandlerNo = CreateThread(NULL, 0, Mul_Hander, NULL, 0, NULL);
	}
	return 0;
}