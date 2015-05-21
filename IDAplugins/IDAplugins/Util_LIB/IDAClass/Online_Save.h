#include "Util_LIB.H"
class Online_Save{
public:
	List<_Base_Segment> Seg;
/**
* @See		����ȫ���б�
*/
	void Update(){
		Seg.Reset();
		while (Seg.Get() != NULL){
			Seg.Get()->Update();
			Seg.Next();
		}
	}
/**
* @See		��inSeg���ص�Seg�б�
* @Param��	inSeg -> IDA����
*/
	void AddSegment(segment_t* inSeg){
		if (inSeg == NULL)return;
		_Base_Segment* _seg = new _Base_Segment(inSeg);
		Debug::MSG("CheckCode:0x%08x\n", _seg->CheckCode);
		if (Seg.GetLength() == 0){
			Seg.Inster(_seg);
		}
		else{
			//�����Ƿ���ƥ���
			Seg.Reset();
			while (Seg.Get() != NULL){
				if (Seg.Get()->CheckCode == _seg->CheckCode){
					Seg.Get()->Update(inSeg);
					delete _seg;
					return;
				}
				Seg.Next();
			}
			//��ƥ��Σ������
			Seg.Inster(_seg);
		}
		Debug::MSG("Seg Length:%d\n", Seg.GetLength());
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
			Seg.Get()->ToIni(_ini, Seg.GetPlace());
			Seg.Next();
		}
		_ini->SaveIni(in_path);
	}
/**
* @See		��ʼ��Online_Save
*/
	Online_Save(){

	}
};