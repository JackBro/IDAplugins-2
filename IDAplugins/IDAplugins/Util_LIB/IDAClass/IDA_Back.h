#include "Util_LIB.H"
/*		
..ʵ���ڵ���ģʽ�£����浱ǰ�������ơ�ע�͵����ݣ���ʾ�������ӳ���ʱ����ע�Ͷ�ʧ�ȡ�
..����������INI�ļ���ʽ����,�����ʽ���£���
..__________________________________________________________________________________
..[Config] -�� �������������Ϣ
..	SegLength -> Seg����
..	[Seg.No] -> ָ���No��Segment����
		Name ->Segment ����
		Start_EA ->	�ο�ʼ��ַ
		End_EA ->	�ν�����ַ
		FunLen ->   ��������
		CmtLen ->	ע�͸���
		CheckCode -> Segment У����
..		[Seg.No.Fun.No] ָ���No��Segment�ĵ�No����������
			Name -> ��������
			Start_EA -> ������ʼ��ַ
			End_EA ->	����������ַ
			Cmt -> �����ظ�����
			ReCmt -> ���ظ�����
			Code_S -> ��ʼ��(NoUsed)
			Code_E -> ������(NoUsed)
..		[Seg.No.Cmt.No] ָ���No��Segment�ĵ�No��ע������
			Name -> ��������
			Cmt -> �����ظ�����
			ReCmt -> ���ظ�����
..__________________________________________________________________________________
*/
class IDA_Back{
public:
	Lis_Seg* LisSeg = new Lis_Seg();
/*
..	Name :  Save_BSegm(char* inPath)
..	Function : �����Save_BSegm�ļ�(INI��ʽ)
*/
	void Save_BSegm(char* inPath){
		C_INI* mBSegm = new C_INI(inPath);
		LisSeg->Save_Ini(mBSegm);
		mBSegm->SaveIni(inPath);
	}
/*
..	Name : Form_BSegm(char* inPath)
..	Function : ��BSegm�ļ���(INI��ʽ)����ȡ���ݡ�
*/
	void Form_BSegm(char* inPath){
		C_INI* mBSegm = new C_INI(inPath);
		mBSegm->load();
		LisSeg->Form_Ini(mBSegm);
		LisSeg->UpToIDA();
	}
/*
..	Name : addSegment(ea_t inStart,ea_t inEnd)
..	Function : ��ӵ�ǰ��
*/
	void addSegment(ea_t inStart, ea_t inEnd) {
		LisSeg->addSeg(inStart,inEnd);
	}
/*
..	Name : addSegment(segment_t* inSeg)
..	Function : ��ӵ�ǰ��
*/
	void addSegment(segment_t* inSeg){
		LisSeg->addSeg(inSeg);
	}
/*
..	Name : addSegment(IDA_Seg* inSeg)
..	Function : ��ӵ�ǰ��
*/
	void addSegment(IDA_Seg* inSeg) {
		LisSeg->addSeg(inSeg);
	}
/*
..	Name : IDA_Back
..	Function : ��ʼ������
*/
	IDA_Back(){
		LisSeg = new Lis_Seg();
	}
};