#define __IDAOP_Export_C_
#include <Util_LIB.HPP>
/*
*			IDA���Exportģ��
*��Ҫ����.
*	1.��ͨDUMP����
*	2.�ļ����¹���
*/
//���¶���UI��ģʽö��
const char ASK_EXPORT_UI[] = "Export Module\n\n\n"
			"<#��ͨDUMP# ~Dump~:R>\n"
			"<#�ļ�����# ~FileUpdate~:R>"
			"<#�����ڴ�# �����ڴ�DEX:R:32:16:>\n"
			"<#�������ڴ�# �������ڴ�:R:32:16:>\n"
			"<#Helper# ���DEX��Ϣ:R:32:16:>>\n";
enum{
	MODE_Dump,
	MODE_FileUpdate,
	MODE_ConDex,
	MODE_UnConDex
};
//Dump UI����
const char ASK_DUMP_UI[] = "Export Dump\n"
				"<~S~tart Mem:N:32:16::>>\n"
				"<~E~nd Mem:N:32:16::>>\n"
				"<~S~ize:N:32:16::>>\n";
//FileUpdate UI����
const char ASK_FILEDUMP_UI[] = "Export ASK_FILEDUMP_UI\n"
				"<~S~tart Addr:N:32:16::>>\n"
				"<~F~ile Start Addr:N:32:16::>>\n"
				"<~S~ize:N:32:16::>>\n";
/**
* @See	Common_Dump -> ��ͨDump����
* @Return NULL
*/
int Common_Dump(int inFlag){
	//��ʼ��������
	ulong mem_st = get_screen_ea(), mem_ed = getseg(get_screen_ea())->endEA, mszie = mem_ed - mem_st;
	//����UI,��������
	if (AskUsingForm_c(ASK_DUMP_UI, &mem_st, &mem_ed, &mszie) == 0)return -1;
	//�жϳ����Ƿ�ı䣬���ȸı����Գ�����������Ϊ׼
	if (mszie == 0){
		if (mem_ed < mem_st)return -1; 
		mszie = mem_ed - mem_st;
	}
	//��ȡ�����ļ�·��
	char* m_filepath = askfile_c(1, "*.*", "����Ϊ");
	if (m_filepath == NULL)return -1;
	FILE* mFile = fopen(m_filepath, "wb");
	char* m_buf = (char*)malloc(mszie);
	ulong m_i = 0;
	while (m_i < mszie){
		*(m_buf + m_i) = get_full_byte(mem_st + m_i);
		m_i++;
	}
	//д���ļ�
	qfwrite(mFile, m_buf, mszie);
	fclose(mFile);
	//�����ڴ�
	free(m_buf);
	return 0;
}
/**
* @See	File_Dump -> �ļ�Dump���¹���
* @Return NULL
*/
int File_Dump(int inFlag){
	//��ʼ��������
	ulong mem_st = get_screen_ea(), file_st = mem_st, mszie = getseg(get_screen_ea())->endEA - get_screen_ea();
	if (AskUsingForm_c(ASK_FILEDUMP_UI, &mem_st, &file_st, &mszie) == 0)return -1;
	if (mszie == 0)return -1;
	char* m_filepath = askfile_c(0, "*.*", "���ļ�");
	if (m_filepath == NULL)return -1;
	FILE* mFile = fopen(m_filepath, "rb+");
	fseek(mFile, 0, SEEK_END); //��λ���ļ�ĩ 
	ulong fileSzie = qftell(mFile);
	fseek(mFile, 0, SEEK_SET); //��λ���ļ���
	//�ж�ԭ���ļ��󣬻�������Ҫ���µ��ļ�Ҫ��
	ulong _AllocMem = fileSzie;
	if (fileSzie <(file_st + mszie)){
		_AllocMem = file_st + mszie;
	}
	char* m_buf = (char*)malloc(_AllocMem);
	memset(m_buf, 0, _AllocMem);
	qfread(mFile, m_buf, fileSzie);
	ulong m_i = 0;
	while (m_i < mszie){
		*(m_buf + file_st + m_i) = get_full_byte(mem_st + m_i);
		m_i++;
	}
	char* newFilename = (char*)malloc(1024); memset(newFilename, 0, 1024);
	sprintf(newFilename, 1024, "%s.dump", m_filepath);
	FILE* mSaveFile = fopen(newFilename, "wb+");
	//д������
	qfwrite(mSaveFile, m_buf, _AllocMem);
	fclose(mSaveFile);
	fclose(mFile);
	//�����ڴ�
	free(newFilename);
	free(m_buf);
	return 0;
}
/*
*				ģʽ˵��
*	1��MODE_Dump -> ��ͨDUMP
*	2��MODE_FileUpdate -> �ļ�����
*/
int Export_Module(int inFlag){
	static int Main_Mode = 0;
	if (inFlag != Flag_Again)
		if(AskUsingForm_c(ASK_EXPORT_UI, &Main_Mode) == 0)
			return -1;
	if (MODE_Dump == Main_Mode){
		Common_Dump(inFlag);
	}
	else if (MODE_FileUpdate == Main_Mode){
		File_Dump(inFlag);
	}
	return 0;
}
/*
*/