#include <Util_LIB.HPP>
/*
*			IDA插件Note模块
*主要功能.
*	1、MODE_NOTE_UP -> 将已加载的段全部更新,并保存成文件
*	2、MODE_NOTE_ADD -> 加载当前段
*	3、MODE_NOTE_FILE -> 从文件中加载
*	4、MODE_NOTE_SAVE -> 保存成文件
*/
//以下定义UI和模式枚举
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
						"<#将已有的段全部更新,并保存成文件# ~U~pAll:R>\n"
						"<#加载当前段# ~A~ddSegment:R>\n"
						"<#从文件中加载# ~F~orm File:R>\n"
						"<#保存成文件# ~S~ave File:R>\n"
						"<#输出全部信息# ~H~elper :R>\n"
						"<#清除原有数据# ~C~lean :R>>"
						"<##配置选择##是否启动多线程处理:C>>\n";
const char ASK_Setting_UI[] = "Note Setting\n\n\n"
						"<Mem START:N:32:16::>\n"
						"<Segname Name:A:15:15::>\n";
enum{
	MODE_NOTE_UP,
	MODE_NOTE_ADD,
	MODE_NOTE_FILE,
	MODE_NOTE_SAVE,
	MODE_NOTE_PRINTF,
	MODE_NOTE_CLEAN
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
	if (mSave->Seg.at(n - 1).MemStart == BADADDR){
		ea_t mStart = get_screen_ea(); 
		char mName[MAXSTR];
		sprintf(mName, MAXSTR, "Debug");
		if (AskUsingForm_c(ASK_Setting_UI, &mStart, mName) == 0)return ;
		mSave->Seg.at(n - 1).MemStart = mStart;
		mSave->Seg.at(n - 1).SegName = qstrdup(mName);
		sprintf(mName, MAXSTR, "IsAdd_Name:%s", mSave->Seg.at(n - 1).SegName);
		set_cmt(mStart, mName, 1);
		mSave->Seg.at(n - 1).To_IDAMem(mSave->Seg.at(n - 1).MemStart);
	}
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
		if (mSave->Seg.at(n - 1).MemStart == BADADDR){
			qsnprintf(cells[0], 0x100, "未设置地址");
		}
		else{
			qsnprintf(cells[0], 0x100, "0x%08X", mSave->Seg.at(n - 1).MemStart);
		}
		
		qstrncpy(cells[1], mSave->Seg.at(n - 1).SegName, qstrlen(mSave->Seg.at(n - 1).SegName) + 1);
		qsnprintf(cells[2], 0x100, "FunSize:0x%d,CmtSize:0x%d,BptSize:0x%0d!", mSave->Seg.at(n - 1).Func.size(), 
			mSave->Seg.at(n - 1).Cmt.size(), mSave->Seg.at(n - 1).Bpt.size());
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
	mSave->Seg.at(n - 1).To_IDAMem(mSave->Seg.at(n - 1).MemStart);
	return 1;
}
/*

*/
void RunMode(int inMode){
	ea_t _ea = get_screen_ea();
	char* _AutoBuf = NULL;
	char* _SegName = NULL;
	ea_t _SegStart = 0;
	int choice = 0;
	char* m_cmt = NULL;
	switch (inMode){
	case MODE_NOTE_UP:
		Util::MSG("Have Length:%d\n", OnSave.Seg.size());
		if (OnSave.Seg.size() == 0)return;
		_AutoBuf = (char*)Util::Alloc(1024);
		sprintf(_AutoBuf, 1024, "%s.back", SaveFile);
		Util_File::CPFile(_AutoBuf, SaveFile);
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "保存注释文件");
		Util::MSG("OnSave.UpAllSegment();\n");
		OnSave.UpAllSegment();
		OnSave.Save(SaveFile);
	break;
	case MODE_NOTE_ADD:
		OnSave.Seg.clear();
		_SegStart = getseg(_ea)->startEA;
		_SegName = IDA::GetSegmentName(_SegStart);
		_SegName = askstr(1, _SegName, "请输入添加的段名");
		if (_SegName == NULL){
			OnSave.AddSegment(_ea,NULL);
			return;
		}
		m_cmt = (char*)Util::Alloc(1024);
		sprintf(m_cmt, 1024, "IsAdd_Name:%s", _SegName);
		set_cmt(getseg(_ea)->startEA, m_cmt, 1);
		OnSave.AddSegment(_ea, _SegName);
	break;
	case MODE_NOTE_FILE:
		if (!MulThread)	LoadFile = askfile_c(1, "*.*", "导入注释文件");
		if (LoadFile == NULL)return;
		OnSave.Seg.clear();
		OnSave.Online_Load(LoadFile);
	break;
	case MODE_NOTE_SAVE:
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "保存注释文件");
		OnSave.Save(SaveFile);
	break;
	case MODE_NOTE_PRINTF:
		choice = choose2(CH_MULTI, -1, -1, -1, -1, (void*)&OnSave, 3, (int*)widths,
			idaListComment_sizer, idaListComment_getlien2, "Note List", -1, 1, NULL, NULL, idaListComment_update, NULL, idaListComment_enter,
			NULL, (const char * const *)szBuf, NULL);
		return;
	break;
	case MODE_NOTE_CLEAN:
		OnSave.Seg.clear();
		return;
	break;
	}
}
/**
* @See	WINAPI多线程
*/
DWORD WINAPI Mul_Hander(LPVOID lpParam){
	Util::MSG("启用多线程\n");
	RunMode(NoteMode);
	WorkIng = FALSE;
	CloseHandle(HandlerNo);
	return 0;
}
/*
*模式说明
*	1、MODE_NOTE_UP -> 将已加载的段全部更新,并保存成文件
*	2、MODE_NOTE_ADD -> 加载当前段
*	3、MODE_NOTE_FILE -> 从文件中加载
*	4、MODE_NOTE_SAVE -> 保存成文件
*配置说明
*	0x01 ->	是否启用多线程
*/
int Note_Moudle(int inFlag){
	if (WorkIng){
		_MSG("之前操作未处理完毕，请稍后再试\n");
		return NULL;
	}
	if (inFlag != Flag_Again)
		if (AskUsingForm_c(ASK_NOTE_UI, &NoteMode, &MulThread) == 0)
			return NULL;
	if ((MulThread == 0) || (NoteMode == MODE_NOTE_PRINTF) || (NoteMode == MODE_NOTE_ADD)){
		WorkIng = FALSE;
		RunMode(NoteMode);
	}
	else{
		if (NoteMode == MODE_NOTE_SAVE){
			SaveFile = askfile_c(1, "*.ini", "保存注释文件");
			if (SaveFile == NULL)return 0;
		}else if (NoteMode == MODE_NOTE_FILE){
			LoadFile = askfile_c(0, "*.*", "导入注释文件");
			if (LoadFile == NULL)return 0;
		}
		else if (NoteMode == MODE_NOTE_UP){
			if (SaveFile == NULL){
				SaveFile = askfile_c(1, "*.ini", "保存注释文件");
				if (SaveFile == NULL)return 0;
			}
		}
		MulThread = TRUE;
		WorkIng = TRUE;
		HandlerNo = CreateThread(NULL, 0, Mul_Hander, NULL, 0, NULL);
	}
	return NULL;
}