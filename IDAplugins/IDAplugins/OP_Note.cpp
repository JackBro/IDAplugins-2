#include <Util_LIB.HPP>
/*
*			IDA���Noteģ��
*��Ҫ����.
*	1��MODE_NOTE_UP -> ���Ѽ��صĶ�ȫ������,��������ļ�
*	2��MODE_NOTE_ADD -> ���ص�ǰ��
*	3��MODE_NOTE_FILE -> ���ļ��м���
*	4��MODE_NOTE_SAVE -> ������ļ�
*/
//���¶���UI��ģʽö��
static const char *szBuf[] = { "pop1", "pop2", "pop3", "Add Filter List" };
int NoteMode = 0;
HANDLE HandlerNo = 0;
boolean MulThread = TRUE;
boolean WorkIng = FALSE;
char* SaveFile = NULL;
char* LoadFile = NULL;
Online_Save OnSave;
char headers[][256] = { "Addr", "SegName","STATUS"};
int widths[] = { 16, 16, 32 };
const char ASK_NOTE_UI[] = "Note Options\n\n\n"
						"<#�����еĶ�ȫ������,��������ļ�# ~U~pAll:R>\n"
						"<#���ص�ǰ��# ~A~ddSegment:R>\n"
						"<#���ļ��м���# ~F~orm File:R>\n"
						"<#������ļ�# ~S~ave File:R>\n"
						"<#���ȫ����Ϣ# ~H~elper :R>>\n"
						"<##����ѡ��##�Ƿ��������̴߳���:C>>\n";
const char ASK_Setting_UI[] = "Note Setting\n\n\n"
						"<Mem START:N:32:16::>\n"
						"<Segname Name:A:15:15::>\n";



enum{
	MODE_NOTE_UP,
	MODE_NOTE_ADD,
	MODE_NOTE_FILE,
	MODE_NOTE_SAVE,
	MODE_NOTE_PRINTF,
};
/*
*/
ulong idaapi idaListComment_sizer(void *obj)
{
	return OnSave.Seg.size();
}
void idaapi idaListComment_enter(void * obj, uint32 n)
{
	_MSG("idaListComment_enter:%d,Size:%d\n", n, OnSave.Seg.size());
	Online_Save* mSave = (Online_Save*)obj;
	if (n > OnSave.Seg.size())
	{
		return;
	}
	if (mSave->Seg.at(n - 1).MemStart == 0xFFFFFFFF){
		ea_t mStart = get_screen_ea(); 
		char mName[MAXSTR];
		sprintf(mName, MAXSTR, "Debug");
		if (AskUsingForm_c(ASK_Setting_UI, &mStart, mName) == 0)return ;
		mSave->Seg.at(n - 1).MemStart = mStart;
		mSave->Seg.at(n - 1).SegName = qstrdup(mName);
		sprintf(mName, MAXSTR, "IsAdd_Name:%s", mSave->Seg.at(n - 1).SegName);
		set_cmt(mStart, mName, 1);
	}
	jumpto(mSave->Seg.at(n - 1).MemStart);

}
void idaapi idaListComment_getlien2(void *obj, ulong n, char* const *cells)
{
	int nCells = sizeof(headers) / sizeof(headers[0]);
	Online_Save* mSave = (Online_Save*)obj;
	if (n > OnSave.Seg.size())
	{
		return;
	}
	if (n == 0)
	{
		for (int i = 0; i < nCells; i++)
		{
			qstrncpy(cells[i], headers[i], qstrlen(headers[i]) + 1);
		}
	}
	else
	{
		if (mSave->Seg.at(n - 1).MemStart == 0xFFFFFFFF){
			qsnprintf(cells[0], 0x100, "δ���õ�ַ");
		}
		else{
			qsnprintf(cells[0], 0x100, "0x%08X", mSave->Seg.at(n - 1).MemStart);
		}
		
		qstrncpy(cells[1], mSave->Seg.at(0).SegName, qstrlen(mSave->Seg.at(n - 1).SegName) + 1);
		qstrncpy(cells[2], mSave->Seg.at(0).SegName, qstrlen(mSave->Seg.at(n - 1).SegName) + 1);
	}
}
uint32 idaapi idaListComment_update(void *obj, uint32 n){
	_MSG("idaListComment_update:%d\n", n);
	Online_Save* mSave = (Online_Save*)obj;
	if (n > OnSave.Seg.size())
	{
		return 1;
	}
	ea_t mStart = mSave->Seg.at(n - 1).MemStart;
	char mName[MAXSTR] = { 0 };
	sprintf(mName, 1024, "%s", mSave->Seg.at(n - 1).SegName);
	if (AskUsingForm_c(ASK_Setting_UI, &mStart, mName) == 0)return -1;
	_MSG("%s", mName);
	mSave->Seg.at(n - 1).MemStart = mStart;
	mSave->Seg.at(n - 1).SegName = qstrdup(mName);
	sprintf(mName, MAXSTR, "IsAdd_Name:%s", mSave->Seg.at(n - 1).SegName);
	set_cmt(mStart, mName, 1);
	return 1;
}
/*

*/
void RunMode(int inMode){
	ea_t _ea = get_screen_ea();
	char* _AutoBuf = NULL;
	char* _SegName = NULL;
	ea_t _SegStart = 0;
	char* m_cmt = NULL;
	switch (inMode){
	case MODE_NOTE_UP:
		Util::MSG("Have Length:%d\n", OnSave.Seg.size());
		if (OnSave.Seg.size() == 0)return;
		_AutoBuf = (char*)Util::Alloc(1024);
		sprintf(_AutoBuf, 1024, "%s.back", SaveFile);
		Util_File::CPFile(_AutoBuf, SaveFile);
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
		Util::MSG("OnSave.UpAllSegment();\n");
		OnSave.UpAllSegment();
		OnSave.Save(SaveFile);
	break;
	case MODE_NOTE_ADD:
		_SegStart = getseg(_ea)->startEA;
		m_cmt = _Base_Cmt::GetCmt(_SegStart, 1);
		if (m_cmt  != NULL){
			if (strncmp(m_cmt, "IsAdd_Name:", strlen("IsAdd_Name:")) == 0){
				return;
			}
		}
		free(m_cmt);
		_SegName = askstr(1, "Debug", "��������ӵĶ���");
		if (_SegName == NULL){
			_MSG("�����������Ч\n");
			return;
		}
		m_cmt = (char*)Util::Alloc(1024);
		sprintf(m_cmt, 1024, "IsAdd_Name:%s", _SegName);
		set_cmt(getseg(_ea)->startEA, m_cmt, 1);
		OnSave.AddSegment(getseg(_ea), _SegName);
	break;
	case MODE_NOTE_FILE:
		if (!MulThread)	LoadFile = askfile_c(1, "*.*", "����ע���ļ�");
		if (LoadFile == NULL)return;
		OnSave.Online_Load(LoadFile);
	break;
	case MODE_NOTE_SAVE:
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
		OnSave.Save(SaveFile);
	break;
	case MODE_NOTE_PRINTF:
		int choice = choose2(CH_MULTI, -1, -1, -1, -1, (void*)&OnSave, 3, (int*)widths,
			idaListComment_sizer, idaListComment_getlien2, "Note List", -1, 1, NULL, NULL, idaListComment_update, NULL, idaListComment_enter,
			NULL, (const char * const *)szBuf, NULL);
		return;
	break;
	}
}
/**
* @See	WINAPI���߳�
*/
DWORD WINAPI Mul_Hander(LPVOID lpParam){
	Util::MSG("Mul_Hander(%d)@ is Run \n", NoteMode);
	RunMode(NoteMode);
	WorkIng = FALSE;
	Util::MSG("Mul_Hander(%d)@ is End\n", NoteMode);
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
	if ((_Config == 0) || (NoteMode == MODE_NOTE_PRINTF) || (NoteMode == MODE_NOTE_ADD)){
		Util::MSG("�����ö��߳�\n");
		WorkIng = FALSE;
		RunMode(NoteMode);
	}
	else{
		Util::MSG("���ö��߳�\n");
		if (NoteMode == MODE_NOTE_SAVE){
			SaveFile = askfile_c(1, "*.ini", "����ע���ļ�");
			if (SaveFile == NULL)return 0;
		}else if (NoteMode == MODE_NOTE_FILE){
			LoadFile = askfile_c(0, "*.*", "����ע���ļ�");
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