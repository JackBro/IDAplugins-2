#include "Util_LIB.H"
class _Base_Segment{
public:
	ulong CheckCode;
	List<_Base_Func> Func;
	List<_Base_Cmt> Cmt;
	List<_Base_Bpt> Bpt;
	segment_t* m_Seg;
/**
* @See		���ҵ�ǰIDAƥ���Segment��
* @Return��	ƥ���segment_t��
*/
	segment_t* FindSegment(){
		int m_i = 0;
		while (getnseg(m_i) != NULL){
			ulong _Code = Util_Char::ReadCheck(getnseg(m_i)->startEA);
			Debug::MSG("FindSegment@ _Code():0x%08x,CheckCode:0x%08x\n",_Code,CheckCode);
			if (_Code == CheckCode){
				return getnseg(m_i);
			}
			m_i++;
		}
		return NULL;
	}
/**
* @See		��ϵͳ�и������¶�
*/	
	void UpSegment(){
		segment_t* _Seg = FindSegment();
		if (_Seg == NULL){
			Debug::MSG("_Base_Segment@ UpSegment() No Find \n");
			return;
		}
		AddSegment(_Seg);
	}
/**
* @See	����ȫ���б�
*	�ڵ�ǰIDA�У���������ӵĶ�
*/
	void To_IDAMem(){
		segment_t* _Seg = FindSegment();
		if (_Seg == NULL){
			Debug::MSG("_Base_Segment@ To_IDAMem() No Find \n");
			return;
		}
		Func.Reset();
		while (Func.Get() != NULL){
			Func.Get()->To_IDAMem(_Seg->startEA);
			Func.Next();
		}
		Cmt.Reset();
		while (Cmt.Get() != NULL){
			Cmt.Get()->To_IDAMem(_Seg->startEA);
			Cmt.Next();
		}
	}
/**
* @See		���µ�ǰinSeg
* @Param��	inSeg -> IDA����
*/
	void AddSegment(segment_t* inSeg){
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
	void To_Ini(INI* inIni,int inPlace){
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
			Cmt.Get()->To_Ini(inIni, _SecName);
			Cmt.Next();
		}
		//���溯��
		Func.Reset();
		while (Func.Get() != NULL){
			memset(_SecName, 0, 1024);
			sprintf(_SecName, 1024, "Seg.%d.Func.%d", inPlace, Func.GetPlace());
			Func.Get()->To_Ini(inIni, _SecName);
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
		char* _SonSecName = (char*)Util_Base::Alloc(1024);
		sprintf(_SecName, 1024, "Seg.%d", inPlace);
		CheckCode = inIni->GetIntValue(_SecName, "CheckCode");
		int _FuncLen = inIni->GetIntValue(_SecName,"FuncLength");
		int _CmtcLen = inIni->GetIntValue(_SecName, "CmtLength");	
		//
		int m_i = 0;
		while (m_i < _FuncLen){
			memset(_SonSecName,0,1024);
			sprintf(_SonSecName, 1024, "Seg.%d.Func.%d", inPlace, m_i);
			_Base_Func* _Func = new _Base_Func(inIni,_SonSecName);
			Func.Inster(_Func);
			m_i++;
		}
		//
		m_i = 0;
		while (m_i < _CmtcLen){
			memset(_SonSecName, 0, 1024);
			sprintf(_SonSecName, 1024, "Seg.%d.Cmt.%d", inPlace, m_i);
			_Base_Cmt* _Cmt = new _Base_Cmt(inIni, _SonSecName);
			Cmt.Inster(_Cmt);
			m_i++;
		}
		//
	}
/**
* @See		��ʼ��_Base_Segment
* @Param��	inSeg -> IDA����
*/
	_Base_Segment(segment_t* inSeg){
		AddSegment(inSeg);
	}
/**/
	_Base_Segment(INI* inIni, int inPlace){
		LoadIni(inIni, inPlace);
	}
};


