#include "Util_LIB.H"
/*
..			IDA Segment
..
*/
class Segment{
public:
//
	SingLink<Comment>* Cmt = new SingLink<Comment>;
	List<Func_t*>* Func = new List<Func_t*>;
	Lis_Fun* Fun = NULL;
	int Fun_Length = 0; 
	int Cmt_Length = 0;
/*
..Name : IniFileToSegment
..Comment:��INI�ļ���������ݳ�ʼ����Segment�ļ�����
*/
	void SaveToINIFile(char* in_path){	
		_MSG("SaveToINIFile @ %s", in_path);
	//	C_INI* mIni = Fun->toINI(in_path);
	//	mIni->SaveIni(in_path);
	}
/*
..Name : addSegment
..Comment: ��segment_t��
*/
	void addSegment(segment_t* in_Seg){
			
	}
/*
..Name : IDATToSegment
..Comment: ��segment_t��
*/
	void IDATToSegment(segment_t* in_Seg){


	}
/*
..Name : IniFileToSegment
..Comment:��INI�ļ���������ݳ�ʼ����Segment�ļ�����
..	
*/
	void IniFileToSegment(char* in_IniFile){

	}
/*
..Name : Segment(char*)
..Input : in_IniFile -> INI�ļ�·��
..Comment: ����INI�ļ�·������INI�ļ��ж�ȡSegment����
*/
	Segment(char* in_IniFile){
		C_INI* Ini = new C_INI(in_IniFile);
		Ini->load();
		//Fun = new Lis_Fun(Ini);
	}
/*
..Name : Segment(segment_t*)
..Comment:������Ҫ��ʼ����
*/
	Segment(segment_t* in_Seg){
		Segment();
		Fun = new Lis_Fun(in_Seg->startEA, in_Seg->endEA);
		if (Fun == NULL)return;
		_MSG("Segment @Length:%d\n", Fun->ArrayFun->getLength());	
	}
/*
..Name : Segment()
..Comment:��ʼ��Segment����Segment�ڲ�����ȫ�����
*/
	Segment(){
		Cmt = new SingLink<Comment>;
		Func = new List<Func_t*>;
		Fun_Length = 0;
		Cmt_Length = 0;
	}
};
