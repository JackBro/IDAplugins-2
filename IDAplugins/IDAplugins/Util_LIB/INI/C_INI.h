#include<Util_LIB.H>
/*
..	Class : C_INI_Key 
..	Comment : ����ΪINI�������
*/
class C_INI_Key{
public:
	char* Name = NULL;
	char* Value = NULL;
/*
..	Name : C_INI_Key���캯��
..	Function :����ʼ���ַ���ֵ
*/
	C_INI_Key(char *in_Key, char* in_Value){
		Name = (char*)malloc(strlen(in_Key) + 1);
		Value = (char*)malloc(strlen(in_Value) + 1);
		Name = strdup(in_Key);
		Value = strdup(in_Value);
	}
};
/*
..	Class : C_INI_Section
..	Comment : ����ΪINI�����
*/
class C_INI_Section{
public:
	char* SecName = NULL;
	long Leng = NULL;
	Lis<C_INI_Key>* Key = new Lis<C_INI_Key>;
/*
..	Name : AddInt
..	Function :������INT����
*/
	void AddInt(char *in_Key, int in_Value){
		char* m_Value = (char*)malloc(12);
		memset(m_Value, 0, 12);
		sprintf(m_Value, 11, "0x%08x", in_Value);
		Key->add(new C_INI_Key(in_Key, m_Value));
	}
/*
..	Name : AddString
..	Function :�������ַ�������
*/
	void AddString(char *in_Key, char* in_Value){
		Key->add(new C_INI_Key(in_Key, in_Value));
	}
/*
..	Name : GetInt
..	Function :����ȡINT����
*/
	int GetInt(char *in_Key){
		if (in_Key == NULL)return NULL;
		if (Key->First == NULL)return NULL;
		char* m_Str = GetString(in_Key);
		if (m_Str == NULL)return NULL;
		return strtol(m_Str,NULL, 16);
	}
/*
..	Name : GetString
..	Function :����ȡINT����
*/
	char* GetString(char *in_Key){
		if (in_Key == NULL)return NULL;
		if (Key->First == NULL)return NULL;
		Node<C_INI_Key>* m_key = Key->First;
		do{
			if (m_key->Data != NULL){
				if (!strcmp(m_key->Data->Name, in_Key)){
					return  m_key->Data->Value;
				}
			}
			m_key = m_key->next;
		} while (m_key != NULL);
		return NULL;
	}
/*
..	Name : C_INI_Section���캯��
..	Function :����ʼ��C_INI_Section��Ļ���ȫ�ֱ�������ʼ������
*/
	C_INI_Section(char* in_name){
		Key = new Lis<C_INI_Key>;
		SecName = in_name;
		Leng = 0;
	}
};
/*
*/
class C_INI{
#define	MAX_SECTION			65535		//Section��󳤶�
#define	MAX_KEY				65535		//KeyValues��󳤶�
#define	MAX_ALLSECTIONS     65535		//����Section����󳤶�
#define	MAX_ALLKEYS			65535		//����KeyValue����󳤶�
#define MAX_FILE_SIZE		1024*1024 //�ļ���󳤶�
#define LEFT_BRACE			'['			//
#define RIGHT_BRACE			']'			//
public:
	char* iniPath;
	char* IniInfoBuf = NULL;
	long IniInfoBuf_Size = NULL;
	//�ڵ��б�
	Lis<C_INI_Section>* ArraySection = new Lis<C_INI_Section>;
/*-----------------------------------------------------------------------------------*/
/*
..	Name : GetSection
..	Function :  ����<in_SegName>��
..	Input : in_SegName -> ����
..	Ouput : NULL -> ������ , !NULL -> C_INI_Section*
*/
	C_INI_Section* GetSection(char* in_SegName){
		Node<C_INI_Section>* mFind = ArraySection->First;
		while (mFind != NULL){
			if (!strcmp(mFind->Data->SecName, in_SegName)){
				return mFind->Data;
			}
			mFind = mFind->next;
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
		if (ArraySection == NULL)ArraySection = new Lis<C_INI_Section>;
		C_INI_Section* mSec = new C_INI_Section(in_SegName);
		ArraySection->add(mSec);
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
	int isEndFile(long m_point){
		if (IniInfoBuf[m_point] == -1)return 1;
		if (m_point >= IniInfoBuf_Size)return 1;
		return 0;
	}
/*�ж���Section*/
	int isLeftSectionMark(char* In_Buf, long in_Po){
		return LEFT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*�ж���Section*/
	int isRightSectionMark(char* In_Buf, long in_Po){
		return RIGHT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*�ж��Ƿ�Ϊ����*/
	int isNewLine(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 1;
		if (In_Buf[in_Po] == '\r' || In_Buf[in_Po] == '\n')return 1;
		return 0;
	}
/*�ж��Ƿ����*/
	int isEndKey(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 1;
		if (In_Buf[in_Po] == '\0' || In_Buf[in_Po] == '=')return 1;
		return 0;
	}
/*�ж��Ƿ����*/
	int isEndValue(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 1;
		if (In_Buf[in_Po] == '\0')return 1;
		if ((In_Buf[in_Po] == 0x0D))return 1;
//		if (In_Buf[in_Po] == 0x0A)return 1;
		return 0;
	}
/*�ж��ǿո��ַ�*/
	int isSpace(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 0;
		return isspace(In_Buf[in_Po]);
	}

/*
..Name : load_ini_file
..Comment:��INI�ļ����ݼ��ص�����ṹ
..
*/
	int load_ini_file(){
		FILE *in_fd = NULL;
		assert(iniPath != NULL);
		assert(IniInfoBuf != NULL);
		int i = 0;
		IniInfoBuf = (char*)malloc(MAX_FILE_SIZE);
		memset(IniInfoBuf, -1, MAX_FILE_SIZE);
		in_fd = fopen(iniPath, "rb+");
		if (NULL == in_fd){
			return 0;
		}
		IniInfoBuf[i] = fgetc(in_fd);
		while (IniInfoBuf[i] != (char)EOF) {
			i++;
			assert(i < MAX_FILE_SIZE);
			IniInfoBuf[i] = fgetc(in_fd);
		}
		IniInfoBuf[i] = 0x0D;
		IniInfoBuf[i + 1] = 0x0A;
		IniInfoBuf[i + 2] = '\0';
		IniInfoBuf[i + 3] = EOF;
		IniInfoBuf_Size = i-1;
		fclose(in_fd);
		return 1;
	}
/*
..Name : load
..Comment:��INI�ļ����ݼ��ص�����ṹ
..
*/
	int load(){
		load_ini_file();
		//��ʼ��������INI�ļ�����
		long po_Find = 0;
		//�ж��ļ��Ƿ����
		msg("Find INI @ %s\n", iniPath);
		while (!isEndFile(po_Find)){
			//ѭ�����Ҷ�(Section)���ж����кͲ���'[',����в���
			if (isLeftSectionMark(IniInfoBuf, po_Find++)){
				char* p_SecName;
				long mStart_Section = po_Find;
				long mLen_Section;
				//�ַ�������������ҵ�']'
				while (!isRightSectionMark(IniInfoBuf, po_Find)){
					po_Find++;
				}
				mLen_Section = po_Find - mStart_Section;
				//��ȡ��Section�ַ�����ȥ�ո�
				p_SecName = (char*)malloc(mLen_Section + 1);
				memset(p_SecName, 0, mLen_Section + 1);
				memcpy(p_SecName, &IniInfoBuf[mStart_Section], mLen_Section);
				p_SecName = trim(p_SecName);
				addSection(p_SecName);
			//	_MSG("FindSec:%s\n", p_SecName);
				//�����ո��ַ�
				while (isSpace(IniInfoBuf, ++po_Find));
				//�ж��Ƿ�Ϊ�µĶο�ʼ�� ���ǿ�ʼ����Key��Value
				while (!isLeftSectionMark(IniInfoBuf, po_Find)){
					//��ʼ����Key��Value
					char* p_Key;
					long mStart_Key = po_Find;
					long mLen_Key;
					//�жϽ������л����ַ���������		
					while (isSpace(IniInfoBuf, po_Find)) {
						po_Find++;
					}		
					mStart_Key = po_Find;
					//�ж��Ƿ�Ϊע��,�ǵĻ����²��ҿ�ʼ�ж�
					if (';' != IniInfoBuf[po_Find++]){
						char* p_Value;
						long mStart_Value;
						long mLen_Value;
						//����KEY�ַ���������ȥ�ո�
						while (!isEndKey(IniInfoBuf, po_Find)){
							po_Find++;
						}
						if (isEndFile(po_Find))break;
						mLen_Key = po_Find - mStart_Key;
						p_Key = (char*)malloc(mLen_Key + 1);
						memset(p_Key, 0, mLen_Key + 1);
						memcpy(p_Key, &IniInfoBuf[mStart_Key], mLen_Key);
						p_Key = trim(p_Key);
						//��ȡ�Ⱥ�
						while (IniInfoBuf[po_Find++] != '='){
							if (isEndFile(po_Find))break;
						}
						//��ȡValue
						while (isSpace(IniInfoBuf,po_Find)){
							po_Find++;
						}
						mStart_Value = po_Find;
						while (!isEndValue(IniInfoBuf, po_Find)){
							po_Find++;
						}
						//����Value�ַ���������ȥ�ո�
						if (isEndFile(po_Find))break;
						mLen_Value = po_Find - mStart_Value ;
						p_Value = (char*)malloc(mLen_Value + 1);
						memset(p_Value, 0, mLen_Value + 1);
						memcpy(p_Value, &IniInfoBuf[mStart_Value], mLen_Value);
						p_Value = trim(p_Value);
						Util_Char::ReplaceChar(p_Value,0x0D,0x01);
						Util_Char::ReplaceChar(p_Value, 0x0A, 0x02);
						//��ӵ��б�
					//	_MSG("FindKey:%s,%s\n", p_Key, p_Value);
						addStrValue(p_SecName, p_Key, p_Value);			
						while (isSpace(IniInfoBuf, po_Find)){
							po_Find++;
						}
					}
				}
			}
		}
		msg("Find INI End\n", iniPath);
		return 0;
	}
/*
..
..
*/
	void SaveIni(char* in_Path){
		assert(iniPath != NULL);
		int m_EndF = 0;
		FILE* m_fd = fopen(in_Path, "wb+");
		char* mBuf = (char*)malloc(10 * 1024);
		int m_i = 0;
		while (ArraySection->Get(m_i)!= NULL){
			memset(mBuf, 0, 10 * 1024);
			sprintf(mBuf, 1024, "[%s]\r\n", ArraySection->Get(m_i)->SecName);
			Util_File::fwrite(m_fd, mBuf);
			int m_j = 0;
			while (ArraySection->Get(m_i)->Key->Get(m_j) != NULL){
				memset(mBuf, 0, 10 * 1024);
				sprintf(mBuf, 1024, "%s = %s\r\n", ArraySection->Get(m_i)->Key->Get(m_j)->Name,
				ArraySection->Get(m_i)->Key->Get(m_j)->Value);
				Util_File::fwrite(m_fd, mBuf);
				m_j++;
			}
			m_i++;
		}
		fclose(m_fd);
	}
/*
..�����ļ�����,��ʼ������.
..
*/
	C_INI(char* in_path){
		iniPath = strdup(in_path);
		if (IniInfoBuf == NULL){
			IniInfoBuf = (char*)malloc(MAX_FILE_SIZE);
		}
		memset(IniInfoBuf, -1, MAX_FILE_SIZE);
	}
/*-----------------------------------------------------------------------------------*/
};