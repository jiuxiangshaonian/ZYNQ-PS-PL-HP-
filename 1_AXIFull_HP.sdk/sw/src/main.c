#include "stdio.h"
#include "xil_io.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "ff.h"

void load_sd_bmp(u8 *frame);

//frame buffer����ʼ��ַ
unsigned int const frame_buffer_addr = 0x10000000;

int main()
{
	Xil_DCacheDisable();
	print("AXI4 PL DDR TEST!\n\r");

	load_sd_bmp((u8*)frame_buffer_addr);

	return 0;
}

//��SD���ж�ȡBMPͼƬ
void load_sd_bmp(u8 *frame)
{
	static 	FATFS fatfs;
	FIL 	fil;
	u8		bmp_head[54];
	UINT 	*bmp_width,*bmp_height,*bmp_size;
	UINT 	br;
	int 	i,j;

	//�����ļ�ϵͳ
	f_mount(&fatfs,"",1);

	//���ļ�
	f_open(&fil,"test.bmp",FA_READ);

	//�ƶ��ļ���дָ�뵽�ļ���ͷ
	f_lseek(&fil,0);

	//��ȡBMP�ļ�ͷ
	f_read(&fil,bmp_head,54,&br);
	xil_printf("fengjing.bmp head: \n\r");
	for(i=0;i<54;i++)
		xil_printf(" %x",bmp_head[i]);

	//��ӡBMPͼƬ�ֱ��ʺʹ�С
	bmp_width  = (UINT *)(bmp_head + 0x12);
	bmp_height = (UINT *)(bmp_head + 0x16);
	bmp_size   = (UINT *)(bmp_head + 0x22);
	xil_printf("\n width = %d, height = %d, size = %d bytes \n\r",
			*bmp_width,*bmp_height,*bmp_size);

	//����ͼƬ��д��DDR
	for(i=*bmp_height-1;i>=0;i--)
	{
		for(j=*bmp_width-1;j>=0;j--)
		{
			f_read(&fil,frame+((i*(*bmp_width)*4) + (j*4)),3,&br);
		}
	}

	//�ر��ļ�
	f_close(&fil);

	Xil_DCacheFlush();     //ˢ��Cache�����ݸ�����DDR3��
	xil_printf("show bmp\n\r");
}