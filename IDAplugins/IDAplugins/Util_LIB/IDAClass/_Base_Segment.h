#include "Util_LIB.H"
class _Base_Segment{
public:
	ulong CheckCode;
	List<_Base_Func> Func;
	List<_Base_Cmt> Cmt;
	List<_Base_Bpt> Bpt;
	segment_t* m_Seg;
/**
* @See		��ϵͳ�и������¶�
*/	
	void Update(){
		int m_i = 0;
		while (getnseg(m_i) != NULL){
			ulong _Code = Util_Char::ReadCheck(getnseg(m_i)->startEA);
			if (_Code == CheckCode){
				Update(getnseg(m_i));
				return;
			}
			m_i++;
		}
		Debug::MSG("_Base_Segment@ Update() No Find \n");
	}
/**
* @See		���µ�ǰinSeg
* @Param��	inSeg -> IDA����
*/
	void Update(segment_t* inSeg){
		if (inSeg == NULL)return;
		//�������
		Func.Clear();
		Cmt.Clear();
		Bpt.Clear();
		//��ȡУ����
		m_Seg = inSeg;
		CheckCode = Util_Char::ReadCheck(inSeg->startEA);
		//���Һ�����
		func_t* _func = get_func(inSeg->startEA);
		if (_func == NULL)_func = get_next_func(inSeg->startEA);
		while ((_func != NULL) && (inSeg->endEA > _func->startEA)){
			Func.Inster(new _Base_Func(inSeg, _func));
			_func = get_next_func(_func->startEA);
		}
		//����ע��
		ea_t _ThisEA = inSeg->startEA;
		while (_ThisEA <= inSeg->endEA){
			if (_Base_Cmt::isCmt(_ThisEA)){
				Cmt.Inster(new _Base_Cmt(inSeg, _ThisEA));
			}
			_ThisEA++;
		}
		//���Ҷϵ�

	}
/**
* @See		������ת����INI��ģʽ
* @Param��	inName -> ������
* @Return��	Out_Sec -> INI����
*/
	void ToIni(INI* inIni,int inPlace){
		char* _SecName= (char*)Util_Base::Alloc(1024);
		sprintf(_SecName, 1024, "Seg.%d", inPlace);
		inIni->addIntValue(_SecName, "CheckCode", CheckCode);
		inIni->addIntValue(_SecName, "CmtLength", Cmt.GetLength());
		inIni->addIntValue(_SecName, "FuncLength", Func.GetLength());
		//����ע��
		Cmt.Reset();
		while (Cmt.Get() != NULL){
			memset(_SecName,0,1024);
			sprintf(_SecName, 1024, "Seg.%d.Cmt.%d", inPlace, Cmt.GetPlace());
			Cmt.Get()->ToIni(inIni, _SecName);
			Cmt.Next();
		}
		//���溯��
		Func.Reset();
		while (Func.Get() != NULL){
			memset(_SecName, 0, 1024);
			sprintf(_SecName, 1024, "Seg.%d.Func.%d", inPlace, Func.GetPlace());
			Func.Get()->ToIni(inIni, _SecName);
			Func.Next();
		}
		free(_SecName);
	}
/**
* @See		��INI�ļ���
* @Param��	inName -> ������
* @Return��	Out_Sec -> INI����
*/
	void LoadIni(INI* inIni, int inPlace){
		char* _SecName = (char*)Util_Base::Alloc(1024);
		sprintf(_SecName, 1024, "Seg.%d", inPlace);
		int _FuncLen = inIni->GetIntValue(_SecName,"FuncLength");
		int _CmtcLen = inIni->GetIntValue(_SecName, "CmtLength");
		int m_i = 0;
		while (m_i < _FuncLen){




		}


	}
/**
* @See		��ʼ��_Base_Segment
* @Param��	inSeg -> IDA����
*/
	_Base_Segment(segment_t* inSeg){
		Update(inSeg);
	}
	_Base_Segment(INI* inIni, int inPlace){
		LoadIni(inIni, inPlace);
	}
};


