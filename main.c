#include "main.h"
#include "rewind.h" //davex
//https://ameblo.jp/pspdevblog/theme8-10001513763.html

#define VERS    1
#define REVS    0

PSP_MODULE_INFO("Rin 1.32 RM", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
//PSP_HEAP_SIZE_MAX();

volatile int bSleep=0;
int bMenu=0;

char RinPath[MAX_PATH];
char RomPath[MAX_PATH];
char RomName[MAX_NAME];
char SavePath[MAX_PATH];
char CheatPath[MAX_PATH];

//davex
int save_period_frames = 0;
const int WAIT_MILIS = (70+70)*1000; 

void set_cpu_clock(int n)
{
	if(n==0)
		scePowerSetClockFrequency(222,222,111);
	else if(n==1)
		scePowerSetClockFrequency(266,266,133);
	else if(n==2)
		scePowerSetClockFrequency(333,333,166);
}

// -----------------------------------------------------------------------------

// �۩`��ܥ���K�˕r�˥��`��Хå�
int exit_callback(void)
{
	bSleep=1;
	set_cpu_clock(0);
	save_config();
	if (rom_get_loaded() && rom_has_battery())
		save_sram(get_sram(), rom_get_info()->ram_size);
	
	free_rewind_states();//davex
	
	sceKernelExitGame();
	return 0;
}

// �Դ�����å������r�䲻���ڤ˥��`��Хå���
// �����v�����ޤ��g���ФǤ⥵���ڥ��?������Х����������Ԥ����롣
int power_callback(int unknown, int pwrflags)
{
	//if(pwrflags & (POWER_CB_SUSPEND|POWER_CB_STANDBY)){
	if(pwrflags & PSP_POWER_CB_POWER_SWITCH){
		if (!bSleep){
			bSleep=1;

			// �ե����륢�������Ф˥����ڥ��?������Х������
			// 0byte�Υ��`�֥ե����뤬�Ǥ��Ƥ��ޤ����Ȥ�����Τǡ�
			// �����z���Фϥ����ڥ��?������Х���o������
			sceKernelPowerLock(0);
			set_cpu_clock(0);
			save_config();
			if (rom_get_loaded() && rom_has_battery())
				save_sram(get_sram(), rom_get_info()->ram_size);
			sceKernelPowerUnlock(0);
		}
	}
	if(pwrflags & PSP_POWER_CB_BATTERY_LOW){
		//renderer_set_msg("��ص����ͣ�����");
		if (!bSleep){
			bSleep=1;

			sceKernelPowerLock(0);
			set_cpu_clock(0);
			save_config();
			if (rom_get_loaded() && rom_has_battery())
				save_sram(get_sram(), rom_get_info()->ram_size);
			sceKernelPowerUnlock(0);
			
			// ���ƥ����ڥ�ɡ�
			// �Хåƥ꤬�s10%���Ф�ѥ�`���פ�����ʼ���ȡ�
			// �������O�ˤ��W���ʤ�ե�`�������ꥻ�`�֤Ǥ��ʤ��ʤä��ꤹ�롣
			// ��؜���`��Ǥ�0%�ޤ�ʹ���Ƥ�褦�ʤΤ��i��
			scePowerRequestSuspend(); 
		}
	}
	if(pwrflags & PSP_POWER_CB_RESUME_COMPLETE){
		bSleep=0;
	}

	// ���`��Хå��v�����ٵ��h
	// ��һ�Ⱥ��Ф줿���ٵ��h���Ƥ����ʤ��ȴΤ˥��`��Хå�����ʤ���
//	int cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
//	scePowerRegisterCallback(0, cbid);
    return 0;
}

// �ݩ`����å���å�
int CallbackThread(int args, void *argp)
{
	int cbid;
	
	// ���`��Хå��v���ε��h
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
	scePowerRegisterCallback(0, cbid);
	
	// �ݩ`���
	sceKernelSleepThreadCB();

	return 0;
}

int SetupCallbacks(void)
{
	int thid = 0;
	
	// �ݩ`����å���åɤ�����
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	
	return thid;
}

// -----------------------------------------------------------------------------

void mainloop(void)
{
#ifdef DEBUG	
	unsigned long lastclock=sceKernelLibcClock();
	unsigned long lasttick=lastclock;
#endif

	unsigned long framecount=0;
	const unsigned int sync_time=16666;
	unsigned long cur_time = sceKernelLibcClock();
	unsigned long cur_time_bak = cur_time;
	unsigned long prev_time = cur_time;
	unsigned long next_time = cur_time + sync_time;
	unsigned long waitfc=0;
	int line, turbo_bak=0;
	
	ctrl_data_t paddata;// davex: for rewind checking
//	allocate_rewind_states();
	save_period_frames = 10;

	for(;;) {
		for(line=0; line<154; line++)
			gb_run();
		
		cur_time = sceKernelLibcClock();
#ifdef DEBUG
		
		if (framecount>=60) {
			unsigned long l;

			//�ե�`���`�Ȥ�ָ�ˡ�60�ե�`��Ǥ����ä��r�g(usec)�򣱣��M�Ǳ�ʾ��
			//�ե�ե�`���0x000f4240�Ȥʤꡢ�󤭤����W�����Ȥˤʤ롣����Ȥ������ΤϿ��͡� - LCK
			framecount=0;
			
			pgcLocate(50,0);
			pgcPuthex8(cur_time-lasttick);
			lasttick=cur_time;
			
			pgcLocate(50,2);
			pgcPuthex8(c_regs_PC);
			l=(cpu_read(c_regs_PC)<<24)+(cpu_read(c_regs_PC+1)<<16)+(cpu_read(c_regs_PC+2)<<8)+(cpu_read(c_regs_PC+3));
			pgcLocate(50,3);
			pgcPuthex8(l);

			pgcLocate(50,5);
			pgcPuthex8(cur_time-lastclock);
			lastclock=cur_time;

			pgcLocate(50,25);
			pgcPuthex8(g_regs.IF);
			pgcLocate(50,26);
			pgcPuthex8(g_regs.IE);
			extern byte c_regs_I;
			pgcLocate(50,27);
			pgcPuthex8(c_regs_I);
			
			// kmg
			pgcLocate(2, 2);
			pgcPuthex8(paddata.analog[CTRL_ANALOG_X]);
			pgcLocate(2, 3);
			pgcPuthex8(paddata.analog[CTRL_ANALOG_Y]);
		}
		pgScreenFlip();
#else
		if(setting.rewind_enabled){
			framecount++;

			//davex
			if( framecount ==  save_period_frames){
				save_rewind_state();
				framecount = 0;
			}
		}


		if (bTurbo){
			turbo_bak = 1;
			skip++;
			if (skip > 9){
				skip = 0;
				prev_time = cur_time;
			}
		}else if (cur_time < cur_time_bak){
			prev_time = cur_time;
			skip=0;
		}else if (cur_time > next_time){
			skip++;
			if(skip > setting.frameskip){
				skip=0;
				if(setting.vsync){
					sceDisplayWaitVblank();
					cur_time = sceKernelLibcClock();
				}
				prev_time = cur_time;
			}
		}else{
			if(setting.vsync){
				sceDisplayWaitVblank();
				cur_time = sceKernelLibcClock();
				prev_time = cur_time;
			}else{
				waitfc++;
				while(cur_time < prev_time+10000*(skip+1))
					cur_time = sceKernelLibcClock();
				if (!(waitfc&3)){
					while(cur_time < next_time)
						cur_time = sceKernelLibcClock();
				}
				prev_time = next_time;
			}
			skip=0;
		}
		cur_time_bak = cur_time;
		next_time = prev_time + sync_time * (skip+1);
		if (setting.vsync && !bTurbo){
			if (turbo_bak)
				turbo_bak = 0;
			else if (skip==0) 
				pgScreenFlip();
		}else{
			if (now_frame==0) pgScreenFlip();
		}
#endif
	
		if(setting.rewind_enabled){
			//>>>davex: rewind check
			sceCtrlPeekBufferPositive(&paddata, 1);
			if( get_nShortcutKey(paddata.buttons) == 8 ){ // 8 == REWIND_SHORTCUT KEY

				wavout_enable=0;
				int refreshProgress = 0;
				while(1){
					renderer_set_msg(get_rewind_progress_bar());
					//begin rewinds
					if( read_rewind_state() > 0 ){
						refreshProgress = 1;
						for(line=0; line<154; line++) //emulate a frame
							gb_run();

						pgScreenFlip();
					}else{
						if(refreshProgress){
							for(line=0; line<154; line++) //emulate a frame
								gb_run();

							pgScreenFlip();
							refreshProgress = 0;
						}
					}
					sceKernelDelayThread(WAIT_MILIS);


					sceCtrlPeekBufferPositive(&paddata, 1);
					if( get_nShortcutKey(paddata.buttons) != 8 )
						break;

				}

				//continue normal emulation
				if(setting.sound) wavout_enable=1;
				cur_time = sceKernelLibcClock();
				prev_time = cur_time;
				next_time = cur_time + sync_time;
				skip=0;
			}
			//<<<
		}
		// ��˥�`
		if(bMenu){
			wavout_enable=0;
			set_cpu_clock(0);
			rin_menu();
			set_cpu_clock(setting.cpu_clock);
			if(setting.sound) wavout_enable=1;

			cur_time = sceKernelLibcClock();
			prev_time = cur_time;
			next_time = cur_time + sync_time;
			skip=0;
			bMenu = 0;
		}
		
		// ����`��
		if(bSleep){
			wavout_enable=0;
			while(bSleep)
				pgWaitV();
			set_cpu_clock(setting.cpu_clock);
			if(setting.sound) wavout_enable=1;

			cur_time = sceKernelLibcClock();
			prev_time = cur_time;
			next_time = cur_time + sync_time;
			skip=0;
		}
	}
}

int main(int argc, char *argv[])
{
	int romsize, ramsize;
	char *p, tmp[MAX_PATH];
	
	pgInit();

	strcpy(RinPath, argv[0]);
	p = strrchr(RinPath, '/');
	*++p = 0;
	sprintf(CheatPath, "%sCHEAT/", RinPath);

	SetupCallbacks();
	pgScreenFrame(2,0);
	wavoutInit();
    max_rewind_memory = establish_max_rewind_memory();

	load_config();
	bBitmap = load_menu_bg();
	if(bBitmap) bgbright_change();
	if(setting.sound) wavout_enable=1;

	strcpy(tmp,RinPath);
	strcat(tmp,"SAVE");
	sceIoMkdir(tmp,0777);
	strcpy(tmp,RinPath);
	strcat(tmp,"CHEAT");
	sceIoMkdir(tmp,0777);

	gb_init();
	strcpy(RomPath,setting.lastpath);
	for(;;){
		if (!getFilePath(RomPath,EXT_GB|EXT_GZ|EXT_ZIP))
			continue;
		strcpy(tmp, RomPath);
		*(strrchr(tmp,'/')+1) = 0;
		strcpy(setting.lastpath, tmp);

		// ָ�������ե�������`�ɤ��롣 by ruka
		romsize = load_rom(RomPath);
		if (!romsize){
			strcpy(filer_msg,"ROM����ʧ��");
			continue;
		}
		ramsize = load_sram(sram_space, sizeof(sram_space));
		if (!gb_load_rom(rom_image, romsize, sram_space, ramsize)){
			strcpy(filer_msg,"ROM����ʧ��");
			continue;
		}

		showMsgAboutLoadedRom();

		break;
	}

	pgFillvram(0);
	pgScreenFlipV();
	pgFillvram(0);
	pgScreenFlipV();

	set_cpu_clock(setting.cpu_clock);

	mainloop();

	return 0;
}