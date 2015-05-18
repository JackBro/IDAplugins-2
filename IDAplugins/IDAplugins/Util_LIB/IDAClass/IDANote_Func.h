#include "Util_LIB.H"
/*
..		
..								INI�ļ����ݴ洢˵��
..1.��Function_Config���д��ȫ�����ã�����Func��ĳ��ȡ���ʼ��ַ��������ַ�����֡�Code.
..2.��Function_Name���д������Func���������,
..3.
*/
/*
��ṹ��.
*/
class Func_t{
public:
	char* name = NULL;
	char* cmt = NULL;
	char* recmt = NULL;
	int Code = NULL;
	int Code_2 = NULL;
	int off_start = NULL;
	int off_end = NULL;
/*
..Name : isEmpty
..�ж����Ƿ�Ϊ��
.. !0 Ϊ��
.. 0 Ϊ�ǿ�
*/
	int isEmpty(){
		if ((name == NULL) && (cmt == NULL) && (recmt == NULL)
			&& (off_start == NULL) && (off_end == NULL) && (Code == NULL)){
			msg("Func_Node isEmpty \n");
			return -1;
		}
		return 0;
	}
/*
..Name : ExportINI
..[Function:%No]
..name:
..cmt:
..recmt:
..Code:
..Code2:
..start:
..end:
*/
	char* ExportINI(char* m_Section){
		char* mBuf = (char*)malloc(4096);
			
		
		return mBuf;
	}
/*���캯��*/
	Func_t(func_t* in_func){
		if (in_func == NULL)return;
		name = (char*)malloc(1024);
		memset(name, 0, 1024);
		get_func_name(in_func->startEA, name, 1024);
		cmt = strdup(get_func_cmt(in_func, 0));
		recmt = strdup(get_func_cmt(in_func, 1));
		off_start = in_func->startEA;
		off_end = in_func->endEA;	
		Code = get_full_byte(in_func->startEA);
		Code_2 = get_full_byte(in_func->endEA);
	}
/*
..	Name : ReadIni_Funct
..	Function :����in_Ini�ļ�
*/
	Func_t(char* inmark, char* incmt, char* inrecmt){
		name = (char*)malloc(1024);
		cmt = (char*)malloc(1024);
		recmt = (char*)malloc(1024);
		memset(name, 0, 1024);
		memset(cmt, 0, 1024);
		memset(recmt, 0, 1024);
		sscanf(inmark, "FMark: %s %08x %08x %08x",
			name, &off_start, &off_end, &Code);
		sscanf(incmt, "Fcmt: %s", cmt);
		sscanf(inrecmt, "Frecmt: %s", recmt);
	}
};
/*
*/
class Util_Func_t{
public:
#define CFunSec_Mark	"Function_Config"
#define CFunSec_Leng	"Length"
#define CFunSec_Start	"Start_Address"
#define CFunSec_End		"End_Address"
#define CFun_NameCon	"Function_Name"
//
#define CFun_Name		"Name"
#define CFun_Start		"StartAddress"
#define CFun_End		"EndAddress"
#define CFun_Code		"Code"
#define CFun_Code2		"Code2"
/*
..	Name : Find(ea_t,ea_t)
..	Function :����IDA�ڴ��ж�ȡ
..	Input : in_Start ->  ��ʼ��ַ��in_End->������ַ
..	Ouput: NULL -> ��ȡʧ�ܣ� ��NULL -> ����Func_t�ṹ��
*/
	static Lis<Func_t>* ReadIDAFunct(ea_t in_Start, ea_t in_End){
		Lis<Func_t>* out_fun = new Lis<Func_t>;
		ea_t m_Start = in_Start;
		func_t *m_Func = get_func(m_Start);
		if (m_Func == NULL)m_Func = get_next_func(m_Start);
		while ((m_Func != NULL) && (in_End <= m_Func->startEA)){
			Func_t* mFunc_t = new Func_t(m_Func);
			out_fun->add(mFunc_t);
			m_Func = get_next_func(m_Func->startEA);
		}
		return 	out_fun;
	}
/*
..	Name : Find(ea_t,ea_t)
..	Function :����IDA�ڴ��ж�ȡ
..	Input : in_Start ->  ��ʼ��ַ��in_End->������ַ
..	Ouput: NULL -> ��ȡʧ�ܣ� ��NULL -> ����Func_t�ṹ��
*/
	static List<Func_t*>* Find(ea_t in_Start, ea_t in_End){
		List<Func_t*>* out_fun = new List<Func_t*>;
		ea_t m_Start = in_Start;
		func_t *m_Func = get_func(m_Start);
		if (m_Func == NULL)m_Func = get_next_func(m_Start);
		if (m_Func == NULL)	return NULL;
		while ((m_Func != NULL) && (m_Start <= m_Func->startEA)){
			Func_t* mFunc_t = new Func_t(m_Func);
			out_fun->addNode(mFunc_t);
			m_Func = get_next_func(m_Func->startEA);
		}
		return 	out_fun;
	}
/*
..	Name : ReadIni_Funct
..	Function :����in_Ini�ļ���in_no������,����
..	Input : in_Ini->INI·����in_no->����λ��
..	Ouput: NULL -> ��ȡʧ�ܣ� ��NULL -> ����Func_t�ṹ��
*/
	static Func_t* ReadIni_Funct(C_INI* in_Ini,int in_no){
		char* mFun_Name = NULL;
		char* mFunc_Key = (char*)malloc(1024);
		memset(mFunc_Key, 0, 1024);
		char* mBuf_FunSec = (char*)malloc(1024);
		memset(mBuf_FunSec, 0, 1024);
		int m_Start = 0, m_End = 0, mCode = 0, mCode2 = 0;
		if (in_Ini == NULL)goto CLEAR_ERR;
		/*��ȡ��������*/
		sprintf(mFunc_Key, 1024, "%s%d", CFun_Name, in_no);
		mFun_Name = in_Ini->GetStrValue(CFun_NameCon, mFunc_Key);
		if (mFun_Name == NULL)goto CLEAR_ERR;
		/*���ú�����*/
		sprintf(mBuf_FunSec, 1024, CFun_Name"_%s", mFun_Name);
		/*��ȡ������������*/
		m_Start = in_Ini->GetIntValue(mBuf_FunSec, CFun_Start);
		m_End = in_Ini->GetIntValue(mBuf_FunSec, CFun_End);
		mCode = in_Ini->GetIntValue(mBuf_FunSec, CFun_Code);
		mCode2 = in_Ini->GetIntValue(mBuf_FunSec, CFun_Code2);
	CLEAR_ERR:
		msg("Name:%s,Start:0x%08x,End:0x%08x,", mFun_Name, m_Start, m_End);
		msg("mC:0x%08x,mC2:0x%08x\n",  mCode, mCode2);
		free(mFunc_Key);
		free(mBuf_FunSec);
		return NULL;
	}
/*
..	Name : ReadINI
..	Function :����in_Ini�ļ���ȡȫ��Func_t����
..	Input : in_Ini->INI·��
..	Ouput: NULL -> ��ȡʧ��
*/
	static List<Func_t*>* ReadINI(C_INI* in_Ini){
		List<Func_t*>* out_fun = new List<Func_t*>;
		int m_Length = 0, StartAddr = 0, EndAddr = 0;;
		char* FunSec_Name = NULL;
		if (in_Ini->GetSection(CFunSec_Mark) == NULL){
			msg("#Warning The Input in_Ini is No Have Funtion Section!\n");
			return NULL;
		}
		/*��ȡ������Ϣ*/
		FunSec_Name = in_Ini->GetStrValue(CFunSec_Mark, CFun_Name);
		StartAddr = in_Ini->GetIntValue(CFunSec_Mark, CFunSec_Start);
		EndAddr = in_Ini->GetIntValue(CFunSec_Mark, CFunSec_End);
		m_Length = in_Ini->GetIntValue(CFunSec_Mark, CFunSec_Leng);/**/
		msg("Name:%s,Start:0x%08x,End:0x%08x,m_Length:%d\n",
			FunSec_Name, StartAddr, EndAddr, m_Length);
		int m_i = 0;
		while (m_i < m_Length){
			Func_t* mFunct = ReadIni_Funct(in_Ini, m_i);
			if (mFunct != NULL)
				out_fun->addNode(mFunct);
			m_i++;
		}

		return 	NULL;
	}
/*
..	Name : FunToIni
..	Function :����Fun����ת����INI�ļ�����
..	Ouput: NULL -> ��ȡʧ��
*/
	static void FunToIni(C_INI* in_ini, Func_t* in_Fun){
		char* mSection = (char*)malloc(1024);
		memset(mSection, 0, 1024);
	//	sprintf(mSection, 1024, "Function_%s", in_Fun->name);
	/*	in_ini->addStrValue(mSection, "Name", in_Fun->name);
		in_ini->addStrValue(mSection, "Cmt", in_Fun->cmt);
		in_ini->addStrValue(mSection, "Recmt", in_Fun->recmt);/**/
	}
/*
..	Name : ListToIni
..	Function :����List����ת����INI�ļ�����
..	Ouput: NULL -> ��ȡʧ��
*/
	static C_INI* ListToIni(List<Func_t*>* in_List,char* in_File){
		assert(in_File != NULL);
		assert(in_List != NULL);
		C_INI* m_ini = new C_INI(in_File);	
		/*�����ݴ���INI�У�*/
		ListNode<Func_t*>* mFunt = in_List->First;
		while (mFunt != NULL){
			char* mSection = (char*)malloc(1024);
			memset(mSection, 0, 1024);
			sprintf(mSection, 1024, "Fun_%s", mFunt->Data->name);
			_MSG("%s\n", mFunt->Data->name);


		/*	m_ini->addStrValue(mSection, "Name", mFunt->Data->name);
			m_ini->addStrValue(mSection, "Cmt", mFunt->Data->cmt);
			m_ini->addStrValue(mSection, "Recmt", mFunt->Data->recmt);/**/
			mFunt = mFunt->next;
		}
		return m_ini;
	}
/*
..	Name : SaveINI
..	Function :��
..	Ouput: NULL -> ��ȡʧ��
*/
	void SaveINI(char* in_IniPath, List<Func_t*>* in_Func){
		




	}
};
