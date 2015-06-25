#include <Util_LIB.HPP>
//
static int Debug_En = TRUE;
//
namespace Util{
/**
* @See	��ȡ�������״̬
* @Return Debug_En -> ��ȡ�������״̬
*/
	static int GetEnable(){
		return Debug_En;
	}
/**
* @See	���õ������״̬
* @Param inEn -> ���õ������״̬
*/
	static void SetEnable(int inEn){
		Debug_En = inEn;
	}
/**
* @See	��ӡ���ݣ���printf�������
*/
	static void MSG(const char *format, ...){
		if (GetEnable() == 0)return;
		va_list va;
		va_start(va, format);
		vmsg(format, va);
		va_end(va);
	}
/**
* @See	���õ������״̬
* @Param inEn -> ���õ������״̬
*/
	static void MSGHex(void* inData,size_t inSize){
		size_t m_i = 0;
		if (inSize == 0)return;
		if (GetEnable() == 0)return;
		uchar *mdata = (uchar *)inData;
		do{
			_MSG("0x%02X ", *mdata);
			mdata++;
			m_i++;
			if ((m_i % 16) == 0){
				_MSG("\n");
			}
			else if ((m_i % 8) == 0){
				_MSG("\t");
			}
		} while (m_i < inSize);
	}
};

