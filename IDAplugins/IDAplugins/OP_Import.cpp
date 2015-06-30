#define __IDA_Import_C_
#include"IDA_Header.H"
//���¶���UI��ģʽö��
const char ASK_Import_UI[] = "Import Options\n"
				"<IDA START:N:32:16::>\n"
				"<IDA END:N:32:16::>\n"
				"<IDA SIZE:N:32:16::>\n"
				"<FILE START:N:32:16::>\n";
/*
* @See ģʽ˵��
*/
int Import_Module(int inFlag){
	char* m_filepath = askfile_c(0, "*.*", "��Ҫ������ļ�");
	if (m_filepath == NULL)return -1;
	FILE* mFile = fopen(m_filepath, "rb");
//��λ���ļ�ĩ,��ȡ�ļ�����
	fseek(mFile, 0, SEEK_END);
	ulong fileSzie = qftell(mFile);
//��λ���ļ���
	fseek(mFile, 0, SEEK_SET);
//
	ulong Mem_Start = get_screen_ea();
	ulong Mem_End = get_screen_ea() + fileSzie;
	ulong Mem_Size = fileSzie;
	ulong File_Start = 0;
	if (AskUsingForm_c(ASK_Import_UI, &Mem_Start, &Mem_End, &Mem_Size,&File_Start) == 0)return 0;
	if (Mem_Size == 0){
		if (Mem_Start >= Mem_End){
			msg("�ļ���ַ���ô���\n");
			return -1;
		}
		Mem_Size = Mem_End - Mem_Start;
	}
//�����ڴ棬�����
	char* ReadBuf = (char*)malloc(Mem_Size);
	memset(ReadBuf, 0, Mem_Size);
	qfseek(mFile, File_Start, SEEK_SET);
	qfread(mFile, ReadBuf, Mem_Size);
	qfclose(mFile);
//����IDA�ڴ�
	ulong m_i = 0;
	while (m_i < Mem_Size){
		patch_byte(Mem_Start + m_i, ReadBuf[m_i]);
		m_i++;
	}
	return 0;
}