#ifndef __ELF32_Header_H_
#define __ELF32_Header_H_
#define EI_NIDENT	16
//����ELF Header
struct str_ELF_HEADER{
	uchar		e_ident[EI_NIDENT];			// ��ʶ
	Elf32_Half	e_type;						// object ����
	Elf32_Half	e_machine;					// ��ϵ�ṹ
	Elf32_Half	e_version;					// object �ļ��汾
	Elf32_Word	e_entry;					// ������Ƶ������ַ
	Elf32_Addr	e_phoff;					// PHT�� program header table���ļ��е�ƫ����
	Elf32_Off	e_shoff;					// SHT�� section header table���ļ��е�ƫ����
	Elf32_Word	e_flags;					// �ó�Ա����������ļ����ض���������־��
	Elf32_Half	e_ehsize;					// �ó�Ա������ ELF ͷ��С(���ֽڼ���) 
	Elf32_Half	e_phentsize;				//�ó�Ա���������ļ���PHT��һ����ڵĴ�С(���ֽڼ���)��
	Elf32_Half	e_phnum;					//�ó�Ա�������ڳ���ͷ������ڵĸ���
	Elf32_Half	e_shentsize;				//�ó�Ա������ section ͷ�Ĵ�С(���ֽڼ���)
	Elf32_Half	e_shnum;					//�ó�Ա�������� section header table �е������Ŀ
	Elf32_Half	e_shstrndx;					//�ó�Ա�����Ÿ� section �����ַ��������ڵ� section ͷ��(section header table) ����
}ELF_HEADER, *LP_ELF_HEADER;
//



#endif