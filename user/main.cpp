
#include "ebox.h"
#include "w5500.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "udp.h"
extern void attachSDCardToFat(SD* sd);

USART uart1(USART1,PA9,PA10);

static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
DIR DirObject;       //Ŀ¼�ṹ
FIL fsrc;            // �ļ��ṹ

FRESULT res;
u8 ret;
	
SD sd(PB12,SPI2,PB13,PB14,PB15);

u8 buf[100];
u8 readBuf[6] ;
u32 bw=0;
u32 br=0;

void fileOpt()
{
	for(int i=0;i<100;i++)
		buf[i] = '1';
	res = f_open(&fsrc,"/12345.txt",FA_WRITE | FA_CREATE_ALWAYS| FA_READ);//û������ļ��򴴽����ļ�
	uart1.printf("\r\n");

	if(res==FR_OK)
	{
		uart1.printf("�����ļ�����ļ��ɹ�  O(��_��)O\r\n");
		uart1.printf("���ļ�����:%d\r\n",fsrc.flag);
		uart1.printf("���ļ���С��%d\r\n",fsrc.fsize);
		uart1.printf("���ļ���д��ʼ����%d\r\n",fsrc.fptr);
		uart1.printf("���ļ���ʼ�غ�:%d\r\n",fsrc.org_clust);
		uart1.printf("���ļ���ǰ�غţ�%d\r\n",fsrc.curr_clust);
		uart1.printf("���ļ���ǰ������:%d\r\n",fsrc.dsect);

		f_lseek(&fsrc,0);
		do
		{ 
		 res = f_write(&fsrc, buf, sizeof(buf),&bw); 
		if(res) 
			 { 
				 uart1.printf("write error : %d\r\n",res); 
				 break; 
			 }  
		uart1.printf("write ok!\r\n"); 
		}  
			while (bw < sizeof(buf));  //  �ж��Ƿ�д��(bw > 100����ʾд�����)
	}
	else if(res==FR_EXIST)
		uart1.printf("���ļ��Ѵ���\r\n");
	else
		uart1.printf("�����ļ�����ļ�ʧ��~~~~(>_<)~~~~ \r\n");
	f_close(&fsrc);//�ر��ļ�
	
	/////////////////////////////////////////////////////////////////////////////////////
	u32 readsize;
	u32 buflen;
	buflen = sizeof(readBuf);
	res = f_open(&fsrc,"/12345.txt", FA_READ);//û������ļ��򴴽����ļ�
	if(res==FR_OK)
	{
		uart1.printf("���ļ���С��%d\r\n",fsrc.fsize);	
	}
	readsize = 0;
	do
	{
		res=f_read(&fsrc,readBuf,buflen,&br);
		if(res==FR_OK)
		{
//			 uart1.printf("�ɹ���ȡ���ݣ�%dBytes\r\n",br);
//			 uart1.printf("data:%s\r\n",readBuf);
			 uart1.printfln((const char*)readBuf,sizeof(readBuf));
		}
		else				
			{uart1.printf("��ȡ����ʧ�ܣ�\r\n");}
		readsize+=buflen;
		f_lseek(&fsrc,readsize);
	
	}while(br==buflen);
	uart1.printf("�ļ���ȡ��ĩβ��\r\n");
  f_close(&fsrc);//�ر��ļ�


	

}
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	ret = sd.begin();
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	attachSDCardToFat(&sd);
	
	f_mount(0, &fs);
   
}
u32 count;
int main(void)
{
	setup();
	fileOpt();
	PB8->mode(OUTPUT_PP);
	while(1)
	{
		
		PB8->write(!PB8->read());
//		uart1.printf("\r\nrunning��");
		delay_us(10);
	}


}



