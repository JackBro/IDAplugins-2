#include "Util_LIB.H"
class Online_Save{
public:
	List<_Base_Segment> Seg;
/**
* @See	����ȫ���б�
*	�ڵ�ǰIDA�У���������ӵĶ�
*/
	void UpAllSegment(){
		Seg.Reset();
		while (Seg.Get() != NULL){
			//���Ҳ����¶�
			Seg.Get()->UpSegment();
			Seg.Next();
		}
	}
/**
* @See	����ǰ����Ϣȫ�����µ�IDA��
*/
	void To_IDAMem(){
		Seg.Reset();
		while (Seg.Get() != NULL){
			Seg.Get()->To_IDAMem();
			Seg.Next();
		}
	}
/**
* @See		��inSeg���ص�Seg�б�
* @Param��	inSeg -> IDA����
*/
	void AddSegment(_Base_Segment* inBSeg){
		if (inBSeg == NULL)return;
		Debug::MSG("CheckCode:0x%08x\n", inBSeg->CheckCode);
		//֮ǰ����Ϊ�գ�ֱ�Ӽ���
		if (Seg.GetLength() == 0){
			Seg.Inster(inBSeg);
			return;
		}
		//�����Ƿ���ƥ���
		Seg.Reset();
		while (Seg.Get() != NULL){
			if (Seg.Get()->CheckCode == inBSeg->CheckCode){
				//���ڹ�ͬ��,�뽫���ݼ��ص�IDAMem��
				//��IDAMem��������Σ��Ͳ��ڶ�Seg����
				inBSeg->To_IDAMem();
				if (inBSeg->FindSegment() != NULL)
					Seg.Get()->AddSegment(inBSeg->FindSegment());
				return;
			}
			Seg.Next();
		}
		//��ƥ��Σ������
		Seg.Inster(inBSeg);
		Debug::MSG("Seg Length:%d\n", Seg.GetLength());
	}
/**
* @See		��inSeg���ص�Seg�б�
* @Param��	inSeg -> IDA����
*/
	void AddSegment(segment_t* inSeg){
		if (inSeg == NULL)return;
		_Base_Segment* _seg = new _Base_Segment(inSeg);
		AddSegment(_seg);
	}
/**
* @See		��ʼ��Online_Save
*/
	void Save(char* in_path){
		INI* _ini = new INI();
		_ini->iniPath = in_path;
		_ini->addIntValue("CONFIG", "SegLength", Seg.Length);
		Seg.Reset();
		while (Seg.Get()!= NULL){
			Seg.Get()->To_Ini(_ini, Seg.GetPlace());
			Seg.Next();
		}
		_ini->SaveIni(in_path);
	}
/**
* @See		��ʼ��Online_Save
*/
	void Online_Load(char* inPath){
		INI* _ini = new INI(inPath);
		char* SecName = (char*)Util_Base::Alloc(1024);
		int SegLength = _ini->GetIntValue("CONFIG", "SegLength");
		int m_i = 0;
		while (m_i < SegLength){
			_Base_Segment* _BSeg = new _Base_Segment(_ini, m_i);
			AddSegment(_BSeg);
			m_i++;
		}
		To_IDAMem();
	}
};