#include <Util_LIB.HPP>
/*
..	Class : C_INI_Key
..	Comment : ����ΪINI�������
*/
typedef struct C_INI_Key{
public:
	char* Name = NULL;
	char* Value = NULL;
/*
..	Name : GetValue
..	Function :����ȡValue�����ַ���
*/
	char* GetValue(){
		if (Value == NULL)return NULL;
		char* _OutChat = strdup(Value);
		Util::ReplaceChar(_OutChat, 0x0A, 0x01);
		Util::ReplaceChar(_OutChat, 0x0D, 0x02);
		return _OutChat;
	}
/*
..	Name : C_INI_Key���캯��
..	Function :����ʼ���ַ���ֵ
*/
	C_INI_Key(char* inName, char* inValue){
		Name = strdup(inName);
		Value = strdup(inValue);
	}
}*LP_INIKEY;
/*
..	Class : C_INI_Section
..	Comment : ����ΪINI�����
*/
class C_INI_Section{
public:
typedef std::vector<C_INI_Key> Vec_Key;
public:
	char* SecName = NULL;
	long Leng = NULL;
	Vec_Key Key;
/*
..	Name : AddInt
..	Function :������INT����
*/
	void AddInt(char *in_Key, int in_Value){
		char* m_Value = (char*)Util::Alloc(12);
		sprintf(m_Value, 11, "0x%08x", in_Value);
		Vec_Key::value_type mKey = C_INI_Key(in_Key, m_Value);
		Key.push_back(mKey);
		free(m_Value);
	}
/*
..	Name : AddString
..	Function :�������ַ�������
*/
	void AddString(char *in_Key, char* in_Value){
		Vec_Key::value_type mKey = C_INI_Key(in_Key, in_Value);
		Key.push_back(mKey);
	}
/*
..	Name : GetInt
..	Function :����ȡINT����
*/
	int GetInt(char *in_Key){
		if (in_Key == NULL)return NULL;
		if (Key.size() == 0)return NULL;
		char* m_Str = GetString(in_Key);
		if (m_Str == NULL)return NULL;
		return strtol(m_Str, NULL, 16);
	}
/*
..	Name : GetString
..	Function :����ȡINT����
*/
	char* GetString(char *in_Name){
		if (in_Name == NULL)return NULL;
		if (Key.size() == 0)return NULL;
		for (size_t m_i = 0; m_i < Key.size(); m_i++){
			if (!strcmp(Key.at(m_i).Name, in_Name)){
				return  Key.at(m_i).Value;
			}
		}
		return NULL;
	}
/*
..	Name : C_INI_Section���캯��
..	Function :����ʼ��C_INI_Section��Ļ���ȫ�ֱ�������ʼ������
*/
	C_INI_Section(char* in_name){
		Key.clear();
		SecName = strdup(in_name);
		Leng = 0;
	}
};
/*
..	Class : INI
..	Comment : INI��
*/
class INI{
#define	MAX_SECTION			65535		//Section��󳤶�
#define	MAX_KEY				65535		//KeyValues��󳤶�
#define	MAX_ALLSECTIONS     65535		//����Section����󳤶�
#define	MAX_ALLKEYS			65535		//����KeyValue����󳤶�
#define MAX_FILE_SIZE		10*1024*1024//�ļ���󳤶�
#define LEFT_BRACE			'['			//
#define RIGHT_BRACE			']'			//
public:
typedef std::vector<C_INI_Key> Vec_Key;
typedef std::vector<C_INI_Section> Vec_Sec;
public:
	char* iniPath = NULL;
	char* FileBuf = NULL;
	Vec_Sec Section;
/*-----------------------------------------------------------------------------------*/
/*
..	Name : GetSection
..	Function :  ����<in_SegName>��
..	Input : in_SegName -> ����
..	Ouput : NULL -> ������ , !NULL -> C_INI_Section*
*/
	C_INI_Section* GetSection(char* in_SegName){
		if (Section.size() == 0)return NULL;
		for (size_t m_i = 0; m_i < Section.size(); m_i++){
			if (!strcmp(Section.at(m_i).SecName, in_SegName)){
				return &Section.at(m_i);
			}
		}
		return NULL;
	}
/*
..	Name : GetStrValue
..	Function :  ����<in_SegName>����in_Key��Ӧ��Value(String)
..	Input : in_SegName -> ����
			in_Key ->��Key����
..	Ouput : NULL -> ������ , !NULL -> char*
*/
	char* GetStrValue(char* in_SegName, char* in_Key){
		if (GetSection(in_SegName) == NULL)return NULL;
		return GetSection(in_SegName)->GetString(in_Key);
	}
/*
..	Name : GetIntValue
..	Function :  ����<in_SegName>����in_Key��Ӧ��Value(int)
..	Input : in_SegName -> ����
			in_Key ->��Key����
*/
	int GetIntValue(char* in_SegName, char* in_Key){
		char* out_Value = NULL;
		if (GetSection(in_SegName) == NULL)return NULL;
		return GetSection(in_SegName)->GetInt(in_Key);
	}
/*
..	Name : addSection
..	Function :  �����������<in_SegName>��
*/
	void addSection(char* in_SegName){
		Vec_Sec::value_type mSection = C_INI_Section(in_SegName);
		Section.push_back(mSection);
	}
/*
..	Name : addStrValue
..	Function :  �������<in_SegName>�������KEY��in_Value(String)
*/
	void addStrValue(char* in_SegName, char* in_Key, char* in_Value){
		if (in_Key == NULL)return;
		if (GetSection(in_SegName) == NULL)addSection(in_SegName);
		if (in_Value == NULL)GetSection(in_SegName)->AddString(in_Key, "NULL");
		else GetSection(in_SegName)->AddString(in_Key, in_Value);
	}
/*
..	Name : addIntValue
..	Function :  �������<in_SegName>�������KEY��in_Value(Int)
*/
	void addIntValue(char* in_SegName, char* in_Key, int in_Value){
		if (in_Key == NULL)return;
		if (GetSection(in_SegName) == NULL)addSection(in_SegName);
		if (in_Value == NULL)GetSection(in_SegName)->AddString(in_Key, "NULL");
		else GetSection(in_SegName)->AddInt(in_Key, in_Value);
	}
/*-----------------------------------------------------------------------------------*/
/*�ж����ļ��Ƿ����*/
	int isEndFile(char* In_Buf,long m_point){
		if (In_Buf[m_point] == -1)return 1;
		return 0;
	}
/*�ж���Section*/
	int isLeftSectionMark(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf, in_Po))return 1;
		return LEFT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*�ж���Section*/
	int isRightSectionMark(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf, in_Po))return 1;
		return RIGHT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*�ж��Ƿ�Ϊ����*/
	int isNewLine(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 1;
		if (In_Buf[in_Po] == '\r' || In_Buf[in_Po] == '\n')return 1;
		return 0;
	}
/*�ж��Ƿ����*/
	int isEndKey(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 1;
		if (In_Buf[in_Po] == '\0' || In_Buf[in_Po] == '=')return 1;
		return 0;
	}
/*�ж��Ƿ����*/
	int isEndValue(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 1;
		if (In_Buf[in_Po] == '\0')return 1;
		if ((In_Buf[in_Po] == 0x0D))return 1;
		if (In_Buf[in_Po] == 0x0A)return 1;
		return 0;
	}
/*�ж��ǿո��ַ�*/
	int isSpace(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 0;
		return isspace(In_Buf[in_Po]);
	}
/*
..Name : load
..Comment:��INI�ļ����ݼ��ص�����ṹ
*/
	int load(){
		//��ʼ��������INI�ļ�����
		long po_Find = 0;
		//�ж��ļ��Ƿ����
		Util::MSG("Find INI @ %s\n", iniPath);
		while (!isEndFile(FileBuf,po_Find)){
			//ѭ�����Ҷ�(Section)���ж����кͲ���'[',����в���
			if (isLeftSectionMark(FileBuf, po_Find++)){
				char* p_SecName;
				long mStart_Section = po_Find;
				long mLen_Section;
				//�ַ�������������ҵ�']'
				while (!isRightSectionMark(FileBuf, po_Find)){
					po_Find++;
				}
				mLen_Section = po_Find - mStart_Section;
				//��ȡ��Section�ַ�����ȥ�ո�
				p_SecName = (char*)malloc(mLen_Section + 1);
				memset(p_SecName, 0, mLen_Section + 1);
				memcpy(p_SecName, &FileBuf[mStart_Section], mLen_Section);
				p_SecName = trim(p_SecName);
				addSection(p_SecName);
				//�����ո��ַ�
				while (isSpace(FileBuf, ++po_Find));
				//�ж��Ƿ�Ϊ�µĶο�ʼ�� ���ǿ�ʼ����Key��Value
				while (!isLeftSectionMark(FileBuf, po_Find)){
					//��ʼ����Key��Value
					char* p_Key;
					long mStart_Key = po_Find;
					long mLen_Key;
					//�жϽ������л����ַ���������		
					while (isSpace(FileBuf, po_Find)) {
						po_Find++;
					}
					mStart_Key = po_Find;
					//�ж��Ƿ�Ϊע��,�ǵĻ����²��ҿ�ʼ�ж�
					if (';' != FileBuf[po_Find++]){
						char* p_Value;
						long mStart_Value;
						long mLen_Value;
						//����KEY�ַ���������ȥ�ո�
						while (!isEndKey(FileBuf, po_Find)){
							po_Find++;
						}
						if (isEndFile(FileBuf,po_Find))break;
						mLen_Key = po_Find - mStart_Key;
						p_Key = (char*)malloc(mLen_Key + 1);
						memset(p_Key, 0, mLen_Key + 1);
						memcpy(p_Key, &FileBuf[mStart_Key], mLen_Key);
						p_Key = trim(p_Key);
						//��ȡ�Ⱥ�
						while (FileBuf[po_Find++] != '='){
							if (isEndFile(FileBuf,po_Find))break;
						}
						//��ȡValue
						while (isSpace(FileBuf, po_Find)){
							po_Find++;
						}
						mStart_Value = po_Find;
						while (!isEndValue(FileBuf, po_Find)){
							po_Find++;
						}
						//����Value�ַ���������ȥ�ո�
						if (isEndFile(FileBuf,po_Find))break;
						mLen_Value = po_Find - mStart_Value;
						p_Value = (char*)malloc(mLen_Value + 1);
						memset(p_Value, 0, mLen_Value + 1);
						memcpy(p_Value, &FileBuf[mStart_Value], mLen_Value);
						p_Value = trim(p_Value);
						Util::ReplaceChar(p_Value, 0x01, 0x0A);
						Util::ReplaceChar(p_Value, 0x02, 0x0D);
						//��ӵ��б�
					//	Util::MSG("FindKey:%s,%s\n", p_Key, p_Value);
						addStrValue(p_SecName, p_Key, p_Value);
						while (isSpace(FileBuf, po_Find)){
							po_Find++;
						}
					}
				}
			}
		}
		Util::MSG("Find INI End\n", iniPath);
		return 0;
	}
/*
..
..
*/
	void SaveIni(char* in_Path){
		if (in_Path == NULL)return;
		FILE* m_fd = fopen(in_Path, "wb+");
		char* mBuf = (char*)malloc(10 * 1024);
		for (size_t m_i = 0; m_i < Section.size(); m_i++){
			memset(mBuf, 0, 10 * 1024);
			sprintf(mBuf, 1024, "[%s]\r\n", Section.at(m_i).SecName);
			Util_File::fwrite(m_fd, mBuf);
			Vec_Key mKey = Section.at(m_i).Key;
			for (size_t m_j = 0; m_j < mKey.size(); m_j++){
				memset(mBuf, 0, 10 * 1024);
				sprintf(mBuf, 1024, "%s = %s\r\n",
					mKey.at(m_j).Name,
					mKey.at(m_j).GetValue());
				Util_File::fwrite(m_fd, mBuf);
			}
		}
		fclose(m_fd);
	}
/*
..		�����ļ�·������ȡ�ļ�����
*/
	INI(char* in_path){
		iniPath = strdup(in_path);
		Section.clear();
		FileBuf = Util_File::Read(in_path, "rb+");
		load();
	}
	INI(){
		Section.clear();
	}
};

