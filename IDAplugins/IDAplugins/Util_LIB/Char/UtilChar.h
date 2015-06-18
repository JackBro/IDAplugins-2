#include <Util_LIB.HPP>
namespace Util{
/**
* @See		��ȡ�ļ�һ��֪��"\r\n"��ֹͣ
* @Param	in_fd -> �ļ����
* @Return	out_line -> ���ض�ȡ�ַ���
*/
	static char* readline(FILE* in_fd){
		char* out_line = (char*)malloc(1024);
		memset(out_line, 0, 1024);
		char c = 0, old_c = 0;
		int len = 0;
		while (((c = fgetc(in_fd)) != EOF)){
			if ((old_c == '\r') || (c == '\n'))
				break;
			out_line[len++] = c;
			old_c = c;
		}
		if (c == EOF){
			free(out_line);
			return NULL;
		}
		return out_line;
	}
/**
* @See		�ַ����ϲ�
* @Param	dst -> ԭ�ַ���
* @Param	src -> ����ַ���
* @Return	out_line -> ���غϲ��ַ���
*/
	static char* strcat(char * dst, const char * src){
		char * cp = dst;
		while (*cp)
			cp++; /* find end of dst */
		while (*cp++ = *src++); /* Copy src to end of dst */
		return(dst); /* return dst */
	}
/**
* @See		�ַ����ַ��滻����
* @Param	sSrc -> ԭ�ַ���
* @Param	sMatchStr -> ��Ҫ�滻�ַ�
* @Param	sReplaceStr -> �滻�ַ�
*/
	static void ReplaceChar(char *sSrc, char sMatchStr, char sReplaceStr){
		int  StringLen = strlen(sSrc);
		while (StringLen){
			if (*(sSrc + StringLen) == sMatchStr){
				*(sSrc + StringLen) = sReplaceStr;
			}
			StringLen--;
		}
	}
/**
* @See		���Զ�ȡһ���ļ�
* @Param	in_fd -> �ļ����
* @Return	out_line -> ���ض�ȡ����
*/
	static char TryRead(FILE* in_fd){
		char c = fgetc(in_fd);
		fseek(in_fd, -1, SEEK_CUR);
		return c;
	}
/**
* @See		��ȡÿ��Seg��������־(ǰ0x100�ĺ�)
* @Param	inStart -> ��ʵ��ַ
* @Return	OutMark -> ���ؼ�������
*/
	static ulong ReadCheck(ea_t inStart) {
		ulong OutMark = 0;
		ulong m_i = 0;
		while (m_i < 0x100){
			OutMark += get_full_byte(inStart + m_i);
			m_i++;
		}
		return OutMark;
	}

};