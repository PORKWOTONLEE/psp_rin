
extern int wavout_enable;
extern unsigned long cur_play;

//������ɥХåե����Х󥯤���������������Х󥯤��m���˥饦��ɥ�ӥ�
//PGA_SAMPLES�α����ˤ��뤳�ȡ�PGA_SAMPLES��ͬ�����ȶ�֥���ʤΤ�ע�⡣ - LCK
#define MAX_SOUND_BANKLEN 2048

extern short sound_buf[MAX_SOUND_BANKLEN*4*2];

void wavoutClear();
int wavoutInit();

