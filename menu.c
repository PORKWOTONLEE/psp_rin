#include "main.h"
#include "image.h"
#include "colbl.c"
#include "tools.h"
#include "menu.h"
#include "rewind.h"
#include "menu_rewind.h"
#include "menu_credits.h"

SETTING setting, tmpsetting;
int bTurbo=0, bBitmap;
unsigned short bgBitmap[480*272];
word thumb_w[160*144];

byte *state_tmp = NULL;
word thumb_tmp[160*144];
ScePspDateTime state_tmp_time;

void init_config()
{
	int i;

	strcpy(setting.vercnf, VERCNF);

	setting.screensize = SCR_X2_FIT;
	setting.gb_type = 0;
	setting.gb_palette = PAL_DARK_GREEN;
	setting.frameskip = 2;
	setting.vsync = 1;
	setting.sound = 1;
	setting.sound_buffer = 0;

	setting.color[0] = DEF_COLOR0;
	setting.color[1] = DEF_COLOR1;
	setting.color[2] = DEF_COLOR2;
	setting.color[3] = DEF_COLOR3;
	setting.bgbright=100;

	for(i=0; i<32; i++){
		setting.skeys[i].buttons = 0;
		setting.skeys[i].n = -1;
	}
	for(i=0; i<=6; i++)
		setting.skeys[i].n = i;
	setting.skeys[0].buttons = CTRL_CIRCLE;
	setting.skeys[1].buttons = CTRL_CROSS;
	setting.skeys[2].buttons = CTRL_TRIANGLE;
	setting.skeys[3].buttons = CTRL_SQUARE;
	setting.skeys[4].buttons = CTRL_SELECT;
	setting.skeys[5].buttons = CTRL_START;
	setting.skeys[6].buttons = CTRL_LTRIGGER;
	setting.skeys[7].buttons = CTRL_RTRIGGER|CTRL_SELECT;
	setting.skeys[7].n = 11;
	setting.skeys[8].buttons = CTRL_RTRIGGER|CTRL_START;
	setting.skeys[8].n = 12;

	//davex: default key for rewinding
	setting.skeys[9].buttons = CTRL_RTRIGGER;
	setting.skeys[9].n = 8;


	setting.analog2dpad=1;
	setting.thumb = 1;
	setting.cpu_clock = 0;
	strcpy(setting.lastpath,RinPath);
	for(i=0; i<16; i++)
		setting.bScreenSizes[i] = 1;
	for(i=0; i<32; i++)
		setting.bGB_Pals[i] = 1;
	setting.compress = 1;
	setting.quickslot = 0;
	setting.rewind_user_max_memory_ammount = max_rewind_memory;
	setting.rewind_user_max_states_ammount = 100;
	setting.rewind_always_use_max_memory = 1;
	setting.rewind_always_use_max_states = 1;
	setting.rewind_limit_mode = REWIND_MODE_LIMIT_MEMORY_AMOUNT;
	setting.rewind_enabled = 1;
}

void check_config()
{
	int i;

	if(strcmp(setting.vercnf, VERCNF)){
		init_config();
		return;
	}

	if(setting.screensize>=SCR_END)
		setting.screensize = SCR_X2_FIT;
	if(setting.gb_type>4)
		setting.gb_type = 0;
	if(setting.gb_palette<=PAL_MONOCHROME || setting.gb_palette>=PAL_SGB)
		setting.gb_palette = PAL_DARK_GREEN;
	if(setting.frameskip > 9)
		setting.frameskip=0;
	if(setting.sound_buffer>2)
		setting.sound_buffer = 0;
	if(setting.bgbright>100)
		setting.bgbright=100;
	if(setting.cpu_clock>2)
		setting.cpu_clock = 0;
	if(strncmp(setting.lastpath, "ms0:/", 5))
		strcpy(setting.lastpath,RinPath);
	for(i=1; i<PAL_SGB; i++)
		if(setting.bGB_Pals[i]) break;
	if(i>=PAL_SGB)
		setting.bGB_Pals[1] = 1;
	for(i=0; i<SCR_END; i++)
		if(setting.bScreenSizes[i]) break;
	if(i>=SCR_END)
		setting.bScreenSizes[0] = 1;
}

void set_gb_type()
{
	if (rom_get_loaded()){
		switch(setting.gb_type){
		case 0:
			if(org_gbtype==1){
				rom_get_info()->gb_type = 2;
				lcd_set_mpal(setting.gb_palette);
			}else if(org_gbtype == 2){
				rom_get_info()->gb_type = 2;
				lcd_set_mpal(PAL_SGB);
			}else if(org_gbtype == 3){
				rom_get_info()->gb_type = 3;
				lcd_set_mpal(PAL_SGB);
			}
			break;
		case 1:
			rom_get_info()->gb_type = 1;
			lcd_set_mpal(PAL_MONOCHROME);
			break;
		case 2:
			rom_get_info()->gb_type = 2;
			if(sgb_mode)
				lcd_set_mpal(PAL_SGB);
			else
				lcd_set_mpal(setting.gb_palette);
			break;
		case 3:
			rom_get_info()->gb_type = 3;
			lcd_set_mpal(setting.gb_palette);
			break;
		case 4:
			rom_get_info()->gb_type = 4;
			lcd_set_mpal(setting.gb_palette);
			break;
		}

		if(rom_get_info()->gb_type>=3 && org_gbtype==3)
			now_gb_mode = 3;
		else if(rom_get_info()->gb_type==2 && sgb_mode)
			now_gb_mode = 2;
		else
			now_gb_mode = 1;
	}
}

int get_nShortcutKey(u32 buttons)
{
	int i;
	for(i=6; i<32; i++){
		if (setting.skeys[i].buttons==0)
			return -1;
		if ((buttons & setting.skeys[i].buttons)==setting.skeys[i].buttons)
			return setting.skeys[i].n;
	}
	return -1;
}

// by kwn
int load_menu_bg()
{
	char path[MAX_PATH], *p;
	strcpy(path, RinPath);

	p = strrchr(path, '/');
	strcpy(p+1, "RINMENU.PNG");
	if(read_png(path,bgBitmap,sizeof(bgBitmap)))
		return 1;

	p = strrchr(path, '.');
	strcpy(p, ".BMP");
	return read_bitmap(path,bgBitmap,sizeof(bgBitmap));
}

// 半透明処理
unsigned short rgbTransp(unsigned short fgRGB, unsigned short bgRGB, int alpha) {
	unsigned short fgR, fgG, fgB;
	unsigned short bgR, bgG, bgB;
	unsigned short R, G, B;
	unsigned short rgb;

	fgB = (fgRGB >> 10) & 0x1F;
	fgG = (fgRGB >> 5) & 0x1F;
	fgR = fgRGB & 0x1F;

	bgB = (bgRGB >> 10) & 0x1F;
	bgG = (bgRGB >> 5) & 0x1F;
	bgR = bgRGB & 0x1F;

	R = coltbl[fgR][bgR][alpha/10];
	G = coltbl[fgG][bgG][alpha/10];
	B = coltbl[fgB][bgB][alpha/10];

	rgb = (((B & 0x1F)<<10)+((G & 0x1F)<<5)+((R & 0x1F)<<0)+0x8000);
	return rgb;
}

void bgbright_change()
{
	unsigned short *vptr=bgBitmap, rgb;
	int i;
	for(i=0; i<272*480; i++){
		rgb = *vptr;
		*vptr++ = rgbTransp(rgb, 0x0000, setting.bgbright);
	}
}

void rin_frame(const char *msg0, const char *msg1)
{
	char tmp[128];

	// 背景
	if(bBitmap)
		pgBitBlt(0,0,480,272,1,bgBitmap);
	else
		pgFillvram(setting.color[0]);

	// バージョンナンバー
	mn_printf(385-strlen(VERRIN)*5, 5, setting.color[1],"■ ＲＩＮ Ver%s ■",VERRIN);

	// メッセージなど
	if(msg0) mn_printf(17, 14, setting.color[2], msg0);
	pgDrawFrame(17,25,463,248,setting.color[1]);
	pgDrawFrame(18,26,462,247,setting.color[1]);

	// 操作説明
	if(msg1) mn_printf(17, 252, setting.color[2], msg1);

	// バッテリーライフ
	if(scePowerIsBatteryExist()){
		sprintf(tmp,"[%d%%]",scePowerGetBatteryLifePercent());
		if(!scePowerIsPowerOnline()){
			int time=scePowerGetBatteryLifeTime();
			if(time >= 0)
				sprintf(&tmp[strlen(tmp)-1],"(%d:%02d)]",time/60,time%60);
		}
		mn_printf(440-strlen(tmp)*5, 252, setting.color[2], "电池 %s", tmp);
	}
}

int rin_MessageBox(const char *msg, int type){
	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE){
			return 1;
		}else if(new_pad & CTRL_CROSS && type){
			return 0;
		}

		if(type)
			rin_frame(0,"Ｏ：确定 Ｘ：取消");
		else
			rin_frame(0,"Ｏ：确定");
		mn_printf(28,32,setting.color[3],msg);
		pgScreenFlipV();
	}
}

void rin_colorconfig(void)
{
	enum
	{
		COLOR0_R,
		COLOR0_G,
		COLOR0_B,
		COLOR1_R,
		COLOR1_G,
		COLOR1_B,
		COLOR2_R,
		COLOR2_G,
		COLOR2_B,
		COLOR3_R,
		COLOR3_G,
		COLOR3_B,
		BG_BRIGHT,
		EXIT,
		INIT,
	};
	char tmp[4], msg[256];
	int color[4][3];
	int sel=0, x, y, i, crs_count=0, bLoop=1;

	memset(color, 0, sizeof(int)*4*3);
	for(i=0; i<4; i++){
		color[i][2] = setting.color[i]>>10 & 0x1F;
		color[i][1] = setting.color[i]>>5 & 0x1F;
		color[i][0] = setting.color[i] & 0x1F;
	}

	for(;;){
		readpad();
		if(now_pad & (CTRL_UP|CTRL_DOWN|CTRL_LEFT|CTRL_RIGHT))
			crs_count=0;
		if(new_pad & CTRL_CIRCLE){
			switch(sel){
			case EXIT:
				bLoop=0;
				break;
			case INIT:
				color[0][2] = DEF_COLOR0>>10 & 0x1F;
				color[0][1] = DEF_COLOR0>>5 & 0x1F;
				color[0][0] = DEF_COLOR0 & 0x1F;
				color[1][2] = DEF_COLOR1>>10 & 0x1F;
				color[1][1] = DEF_COLOR1>>5 & 0x1F;
				color[1][0] = DEF_COLOR1 & 0x1F;
				color[2][2] = DEF_COLOR2>>10 & 0x1F;
				color[2][1] = DEF_COLOR2>>5 & 0x1F;
				color[2][0] = DEF_COLOR2 & 0x1F;
				color[3][2] = DEF_COLOR3>>10 & 0x1F;
				color[3][1] = DEF_COLOR3>>5 & 0x1F;
				color[3][0] = DEF_COLOR3 & 0x1F;
				setting.bgbright = 100;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
				break;
			case BG_BRIGHT:
				//輝度変更
				setting.bgbright += 10;
				if(setting.bgbright > 100) setting.bgbright=0;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
				break;
			default:
				if(color[sel/3][sel%3]<31)
					color[sel/3][sel%3]++;
				break;
			}
		}else if(new_pad & CTRL_SQUARE){
			if(sel == BG_BRIGHT) {
				//輝度変更
				if(setting.bgbright > 10)
					setting.bgbright-=10;
				else
					setting.bgbright=100;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else if(sel>=COLOR0_R && sel<=COLOR3_B){
				if(color[sel/3][sel%3]>0)
					color[sel/3][sel%3]--;
			}
		}else if(new_pad & CTRL_CROSS){
			break;
		}else if(new_pad & CTRL_UP){
			if(sel!=0)	sel--;
			else		sel=INIT;
		}else if(new_pad & CTRL_DOWN){
			if(sel!=INIT)	sel++;
			else			sel=0;
		}else if(new_pad & CTRL_RIGHT){
			if (sel<=COLOR0_G)
				sel+=13;
			else if (sel<=BG_BRIGHT)
				sel=INIT;
		}else if(new_pad & CTRL_LEFT){
			if (sel>=EXIT)
				sel-=13;
		}
		if(!bLoop) break;

		for(i=0; i<4; i++)
			setting.color[i]=color[i][2]<<10|color[i][1]<<5|color[i][0]|0x8000;

		if (crs_count++>=30) crs_count=0;

		x = 35;
		y = 35;

		if(sel>=COLOR0_R && sel<=BG_BRIGHT)
			strcpy(msg, "Ｏ：增加　□：减少 Ｘ：返回");
		else
			strcpy(msg, "Ｏ：确定 Ｘ：返回");

		rin_frame(0, msg);

		mn_printf(x,y,setting.color[3],"颜色0 R:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色0 G:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色0 B:");
		y+=24;
		mn_printf(x,y,setting.color[3],"颜色1 R:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色1 G:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色1 B:");
		y+=24;
		mn_printf(x,y,setting.color[3],"颜色2 R:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色2 G:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色2 B:");
		y+=24;
		mn_printf(x,y,setting.color[3],"颜色3 R:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色3 G:");
		y+=12;
		mn_printf(x,y,setting.color[3],"颜色3 B:");
		y+=24;
		if(setting.bgbright / 100 == 1) {
			mn_printf(x,y,setting.color[3],"背景亮度：100%");
			y+=12;
		}
		else {
			mn_printf(x,y,setting.color[3],"背景亮度：0%");
			y+=12;
		}
		if(setting.bgbright % 100 != 0)			// 10%?90%
			mn_printf((x+13)*8,(y-1)*8,setting.color[3],0,'0'+setting.bgbright/10,1,0,1);
		y+=12;
		mn_printf(285,35,setting.color[3],"返回主菜单");
		y+=12;
		mn_printf(285,47,setting.color[3],"恢复默认值");

		x = 80;
		y = 35;
		for(i=0; i<12; i++){
			if(i!=0 && i%3==0) y+=12;
			sprintf(tmp, "%d", color[i/3][i%3]);
			mn_printf(x,y,setting.color[3],tmp);
			y+=12;
		}

		x = 35;
		y = 35;
		if (crs_count < 15){
			if (sel>=EXIT) {
				x += 245;
				y = y + (sel-EXIT) * 12;
			}
			else
			{
				if (sel>=COLOR1_R) y+=12;
				if (sel>=COLOR2_R) y+=12;
				if (sel>=COLOR3_R) y+=12;
				if (sel>=BG_BRIGHT) y+=12;
				x = x - 5;
				y = y + sel * 12;
			}
			mn_printf(x,y,setting.color[3],">");
		}

		pgScreenFlipV();
	}
}

int cmp_skey(S_BUTTON *a, S_BUTTON *b)
{
	int i, na=0, nb=0;

	for(i=0; i<32; i++){
		if ((a->buttons >> i) & 1) na++;
		if ((b->buttons >> i) & 1) nb++;
	}
	return nb-na;
}

void sort_skeys(S_BUTTON *a, int left, int right) {
	S_BUTTON tmp, pivot;
	int i, p;

	if (left < right) {
		pivot = a[left];
		p = left;
		for (i=left+1; i<=right; i++) {
			if (cmp_skey(&a[i],&pivot)<0){
				p=p+1;
				tmp=a[p];
				a[p]=a[i];
				a[i]=tmp;
			}
		}
		a[left] = a[p];
		a[p] = pivot;
		sort_skeys(a, left, p-1);
		sort_skeys(a, p+1, right);
	}
}

// by kwn
void rin_keyconfig(void)
{
	enum
	{
		CONFIG_A,
		CONFIG_B,
		CONFIG_RAPIDA,
		CONFIG_RAPIDB,
		CONFIG_SELECT,
		CONFIG_START,
		CONFIG_MENU,
		CONFIG_WAIT,
		CONFIG_REWIND,//davex: CONFIG_VSYNC replaced
		CONFIG_SOUND,
		CONFIG_SCREENSIZE,
		CONFIG_QUICKSAVE,
		CONFIG_QUICKLOAD,
		CONFIG_STATE_SLOT,
		CONFIG_GB_COLOR,
		CONFIG_CPU_CLOCK,
		CONFIG_ANALOG2DPAD,
		CONFIG_EXIT,
	};
	char msg[256];
	int key_config[32];
	int sel=0, x, y, i, bPad=0, crs_count=0;

	for(i=0; i<32; i++)
		key_config[i] = 0;
	for(i=0; i<32; i++){
		if(setting.skeys[i].n >= 0)
			key_config[setting.skeys[i].n] = setting.skeys[i].buttons;
	}

	for(;;){
		readpad();
		if(now_pad & (CTRL_UP|CTRL_DOWN|CTRL_LEFT|CTRL_RIGHT))
			crs_count=0;
		if(now_pad==CTRL_LEFT || now_pad==CTRL_RIGHT){
			if(sel!=CONFIG_EXIT && sel!=CONFIG_MENU && sel!=CONFIG_ANALOG2DPAD)
				key_config[sel] = 0;
		}else if(now_pad==CTRL_UP){
			if(bPad==0){
				if(sel!=0)	sel--;
				else		sel=CONFIG_EXIT;
				bPad++;
			}else if(bPad >= 25){
				if(sel!=0)	sel--;
				else		sel=CONFIG_EXIT;
				bPad=20;
			}else
				bPad++;
		}else if(now_pad==CTRL_DOWN){
			if(bPad==0){
				if(sel!=CONFIG_EXIT)sel++;
				else				sel=0;
				bPad++;
			}else if(bPad >= 25){
				if(sel!=CONFIG_EXIT)sel++;
				else				sel=0;
				bPad=20;
			}else
				bPad++;
		}else if(new_pad != 0){
			if(sel==CONFIG_EXIT && new_pad&CTRL_CIRCLE)
				break;
			else if(sel==CONFIG_ANALOG2DPAD && new_pad&CTRL_CIRCLE)
				setting.analog2dpad = !setting.analog2dpad;
			else
				key_config[sel] = now_pad;
		}else{
			bPad=0;
		}

		if (crs_count++>=30) crs_count=0;

		if(sel>=CONFIG_ANALOG2DPAD)
			strcpy(msg,"Ｏ：确定");
		else
			strcpy(msg,"←→：清除键位");

		rin_frame(0, msg);

		x=35; y=35;
		mn_printf(x,y,setting.color[3],"A 键：");
		y+=12;
		mn_printf(x,y,setting.color[3],"B 键：");
		y+=12;
		mn_printf(x,y,setting.color[3],"A 键（连发）：");
		y+=12;
		mn_printf(x,y,setting.color[3],"B 键（连发）：");
		y+=12;
		mn_printf(x,y,setting.color[3],"SELECT键：");
		y+=12;
		mn_printf(x,y,setting.color[3],"START键：");
		y+=12;
		mn_printf(x,y,setting.color[3],"菜单键：");
		y+=12;
		mn_printf(x,y,setting.color[3],"连发开关：");
		y+=12;
		mn_printf(x,y,setting.color[3],"回溯：");
		y+=12;
		//mn_printf(x,y,setting.color[3],"  VSYNC ON/OFF   :");//davex: VSYNC removed as key
		y+=12;
		mn_printf(x,y,setting.color[3],"音频开关：");
		y+=12;
		mn_printf(x,y,setting.color[3],"屏幕尺寸：");
		y+=12;
		mn_printf(x,y,setting.color[3],"即时存档：");
		y+=12;
		mn_printf(x,y,setting.color[3],"即时读档：");
		y+=12;
		mn_printf(x,y,setting.color[3],"快速存档：");
		y+=12;
		mn_printf(x,y,setting.color[3],"GB调色盘：");
		y+=12;
		mn_printf(x,y,setting.color[3],"CPU频率：");
		y+=12;
		if(setting.analog2dpad)
			mn_printf(x+250,35,setting.color[3],"摇杆：开启");
		else
			mn_printf(x+250,35,setting.color[3],"摇杆：关闭");
		y+=12;
		mn_printf(x+250,47,setting.color[3],"返回主菜单");

		for (i=0; i<CONFIG_ANALOG2DPAD; i++){
			y = 35 + i * 12;
			int j = 0;
			msg[0]=0;
			if(key_config[i] == 0){
				strcpy(msg,"未定义");
			}else{
				if (key_config[i] & CTRL_LTRIGGER){
					msg[j++]='L'; msg[j++]='+'; msg[j]=0;
				}
				if (key_config[i] & CTRL_RTRIGGER){
					msg[j++]='R'; msg[j++]='+'; msg[j]=0;
				}
				if (key_config[i] & CTRL_CIRCLE){
					strcat(msg,"Ｏ +"); j+=3;
				}
				if (key_config[i] & CTRL_CROSS){
					strcat(msg,"Ｘ +"); j+=3;
				}
				if (key_config[i] & CTRL_SQUARE){
					strcat(msg,"□ +"); j+=3;
				}
				if (key_config[i] & CTRL_TRIANGLE){
					strcat(msg,"△ +"); j+=3;
				}
				if (key_config[i] & CTRL_START){
					strcat(msg,"START+"); j+=6;
				}
				if (key_config[i] & CTRL_SELECT){
					strcat(msg,"SELECT+"); j+=7;
				}
				if (key_config[i] & CTRL_UP){
					msg[j++]=5; msg[j++]='+'; msg[j]=0;
				}
				if (key_config[i] & CTRL_RIGHT){
					msg[j++]=6; msg[j++]='+'; msg[j]=0;
				}
				if (key_config[i] & CTRL_DOWN){
					msg[j++]=7; msg[j++]='+'; msg[j]=0;
				}
				if (key_config[i] & CTRL_LEFT){
					msg[j++]=8; msg[j++]='+'; msg[j]=0;
				}
				msg[strlen(msg)-1]=0;
			}
			if (i>=CONFIG_SOUND)
			{
				y+=12;
			}
			mn_printf(120,y,setting.color[3],msg);
		}

		x = 35;
		y = 35;
		if (crs_count < 15){
			if (sel>=CONFIG_ANALOG2DPAD) {
				x += 245;
				y = y + (sel-CONFIG_ANALOG2DPAD) * 12;
			}
			else
			{
				if (sel>=CONFIG_SOUND) y+=12;
				x = x - 5;
				y = y + sel * 12;
			}
			mn_printf(x,y,setting.color[3],">");
		}

		pgScreenFlipV();
	}

	for(i=0; i<32; i++){
		if (i!=6 && key_config[i] == key_config[6])
			key_config[i] = 0;
		if(key_config[i]){
			setting.skeys[i].buttons = key_config[i];
			setting.skeys[i].n = i;
		}else{
			setting.skeys[i].buttons = 0;
			setting.skeys[i].n = -1;
		}
	}
	sort_skeys(&setting.skeys[6],0,25);
}

const char *gbtype_names[] = {
	"自动",
	"GB",
	"SGB",
	"GBC",
	"GBA",
};
int rin_gbtype(int n)
{
	int x,y,i,sel=n;

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>4) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=4;
		}else if(new_pad & CTRL_RIGHT){
			sel+=2;
			if(sel>4) sel=4;
		}else if(new_pad & CTRL_LEFT){
			sel-=2;
			if(sel<0) sel=0;
		}

		rin_frame("选择GB类型", "Ｏ：确定 Ｘ：取消");

		x=35, y=35;
		for(i=0; i<=4; i++){
			mn_printf(x,y,setting.color[i==sel?2:3],gbtype_names[i]);
			y+=12;
		}

		pgScreenFlipV();
	}
}

int rin_screensize(int n)
{
	int x,y,i,sel=n;

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_SELECT){
			setting.bScreenSizes[sel] = !setting.bScreenSizes[sel];
			for(i=0; i<SCR_END; i++)
				if(setting.bScreenSizes[i]) break;
			if(i>=SCR_END)
				setting.bScreenSizes[sel] = 1;
		}else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>=SCR_END) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=SCR_END-1;
		}else if(new_pad & CTRL_RIGHT){
			sel+=SCR_END/2;
			if(sel>=SCR_END) sel=SCR_END-1;
		}else if(new_pad & CTRL_LEFT){
			sel-=SCR_END/2;
			if(sel<0) sel=0;
		}

		if(setting.bScreenSizes[sel])
			rin_frame("选择屏幕尺寸", "Ｏ：确定 Ｘ：取消  ＳＥＬＥＣＴ：禁用");
		else
			rin_frame("选择屏幕尺寸", "Ｏ：确定 Ｘ：取消  ＳＥＬＥＣＴ：启用");

		x=35, y=35;
		for(i=0; i<SCR_END; i++){
			if(setting.bScreenSizes[i])
				mn_printf(x-2,y,setting.color[1],"+");
			mn_printf(x,y,setting.color[i==sel?2:3],scr_names[i]);
			y+=12;
		}

		pgScreenFlipV();
	}
}

int rin_gbcolor(int n)
{
	int x,y,i,sel=n;

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_SELECT){
			setting.bGB_Pals[sel] = !setting.bGB_Pals[sel];
			for(i=1; i<PAL_SGB; i++)
				if(setting.bGB_Pals[i]) break;
			if(i>=PAL_SGB)
				setting.bGB_Pals[sel] = 1;
		}else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>=PAL_SGB) sel=1;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<1) sel=PAL_SGB-1;
		}else if(new_pad & CTRL_RIGHT){
			sel+=(PAL_SGB-1)/2;
			if(sel>=PAL_SGB) sel=PAL_SGB-1;
		}else if(new_pad & CTRL_LEFT){
			sel-=(PAL_SGB-1)/2;
			if(sel<1) sel=1;
		}

		if(setting.bGB_Pals[sel])
			rin_frame("选择GB调色盘", "Ｏ：确定 Ｘ：取消  ＳＥＬＥＣＴ：禁用");
		else
			rin_frame("选择GB调色盘", "Ｏ：确定 Ｘ：取消  ＳＥＬＥＣＴ：启用");

		x=35, y=35;
		for(i=1; i<PAL_SGB; i++){
			if(setting.bGB_Pals[i])
				mn_printf(x,y,setting.color[1],"+");
			mn_printf(x,y,setting.color[i==sel?2:3],pal_names[i]);
			y+=12;
		}

		pgScreenFlipV();
	}
}

int rin_frameskip(int sel)
{
	char tmp[8];
	int x,y,i;

	strcpy(tmp,"0");

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>9) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=9;
		}else if(new_pad & CTRL_RIGHT){
			sel+=5;
			if(sel>9) sel=9;
		}else if(new_pad & CTRL_LEFT){
			sel-=5;
			if(sel<0) sel=0;
		}

		rin_frame("选择最大跳帧数", "Ｏ：确定 Ｘ：取消");

		x=35, y=35;
		for(i=0; i<=9; i++){
			tmp[0] = i + '0';
			mn_printf(x,y++,setting.color[i==sel?2:3],tmp);
			y += 12;
		}

		pgScreenFlipV();
	}
}

const char *cpu_clocks[] = {
	"222MHz（默认）",
	"266MHz",
	"333MHz",
};
int rin_cpuclock(int sel)
{
	int x,y,i;

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>2) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=2;
		}

		rin_frame("选择CPU频率（危险！危险！危险！）", "Ｏ：确定 Ｘ：取消");

		x=35, y=35;
		for(i=0; i<3; i++) {
			mn_printf(x,y,setting.color[i==sel?2:3],cpu_clocks[i]);
			y+=12;
		}

		pgScreenFlipV();
	}
}

const char *sound_buffers[] = {
	"快速",
	"普通",
	"慢速",
};
int rin_sound_buffers(int sel)
{
	int x,y,i;

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>2) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=2;
		}

		rin_frame("选择音频缓存方式", "Ｏ：确定 Ｘ：取消");

		x=35, y=35;
		for(i=0; i<3; i++) {
			mn_printf(x,y,setting.color[i==sel?2:3],sound_buffers[i]);
			y+=12;
		}

		pgScreenFlipV();
	}
}

void rin_findState(int nState[], int nThumb[])
{
	char tmp[MAX_PATH], *p;
	int i, j;

	strcpy(tmp,SavePath);
	p = strrchr(tmp,'/') + 1;
	*p = 0;

	nfiles = 0;
	strcpy(path_files, tmp);
	int fd = sceIoDopen(tmp);
	while(nfiles<MAX_ENTRY){
		memset(&files[nfiles], 0x00, sizeof(SceIoDirent));
		if(sceIoDread(fd, &files[nfiles])<=0) break;
		nfiles++;
	}
	sceIoDclose(fd);

	for(i=0; i<=STATE_SLOT_MAX; i++){
		get_state_path(i,tmp);
		nState[i]=-1;
		for(j=0; j<nfiles; j++){
			if(!strcasecmp(p,files[j].d_name)){
				nState[i] = j;
				break;
			}
		}

		if (nState[i]<0){
			strcat(p, ".gz");
			for(j=0; j<nfiles; j++){
				if(!strcasecmp(p,files[j].d_name)){
					nState[i] = j;
					break;
				}
			}
		}

		get_thumb_path(i,tmp);
		nThumb[i]=-1;
		for(j=0; j<nfiles; j++){
			if(!strcasecmp(p,files[j].d_name)){
				nThumb[i] = j;
				break;
			}
		}
		if(nThumb[i]>=0)
			continue;

		*strrchr(tmp, '.') = 0;
		for(j=0; j<nfiles; j++){
			if(!strcasecmp(p,files[j].d_name)){
				nThumb[i] = j;
				break;
			}
		}
	}
}

enum
{
	RIN_STATE_SAVE,
	RIN_STATE_LOAD,
	RIN_QUICK_SLOT,
};
int rin_stateslot(int type)
{
	const int MAX_ITEM = STATE_SLOT_MAX+1;
	char msg[256], *p;
	static int ex_sel=0;
	int nState[STATE_SLOT_MAX+1], nThumb[STATE_SLOT_MAX+1];
	int x, y, i, ret, sel=ex_sel, sel_bak=-1;

	rin_findState(nState, nThumb);

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE){
			if (type != RIN_STATE_LOAD)
				break;
			else{
				if(sel > STATE_SLOT_MAX){
					if(state_tmp)
						break;
				}else if (nState[sel]>=0)
					break;
			}
		}else if(new_pad & CTRL_CROSS){
			return -1;
		}else if((new_pad & CTRL_SELECT)){
			if (sel > STATE_SLOT_MAX){
				free(state_tmp);
				state_tmp = NULL;
			}
			else if (nState[sel]>=0){
				if (delete_state(sel)>=0)
					nState[sel] = nThumb[sel] = -1;
			}
		}else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>MAX_ITEM) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=MAX_ITEM;
		}else if(new_pad & CTRL_RIGHT){
			sel+=(MAX_ITEM+1)/2;
			if(sel>MAX_ITEM) sel=MAX_ITEM;
		}else if(new_pad & CTRL_LEFT){
			sel-=(MAX_ITEM+1)/2;
			if(sel<0) sel=0;
		}

		if (sel!=sel_bak){
			sel_bak = sel;
			if (sel > STATE_SLOT_MAX){
				if (state_tmp)
					memcpy(thumb_w,thumb_tmp,sizeof(thumb_w));
			}
			else if (nState[sel]>=0 && nThumb[sel]>=0){
				p = strrchr(files[nThumb[sel]].d_name, '.');
				if(!strcasecmp(p, ".png"))
					ret = load_thumb(sel,thumb_w,sizeof(thumb_w));
				else
					ret = load_thumb_old(sel,thumb_w,sizeof(thumb_w));
				if(!ret)
					nThumb[sel] = -1;
			}
		}

		switch(type)
		{
		case RIN_STATE_LOAD:
			p = "选择加载存档插槽";
			break;
		case RIN_STATE_SAVE:
			p = "选择保存存档插槽";
			break;
		case RIN_QUICK_SLOT:
			p = "选择快速存档插槽";
			break;
		default:
			p = NULL;
		}
		rin_frame(p,"Ｏ：确定 Ｘ：取消  ＳＥＬＥＣＴ：删除");

		if ((sel>STATE_SLOT_MAX && state_tmp) ||
			(sel<=STATE_SLOT_MAX && nState[sel]>=0 && nThumb[sel]>=0)){
			pgBitBlt(272,50,160,144,1,thumb_w);
			pgDrawFrame(270,48,433,195,setting.color[1]);
			pgDrawFrame(271,49,432,194,setting.color[1]);
		}

		// switch(type)
		// {
		// case RIN_STATE_LOAD:
		// 	p = "加载从：";
		// 	break;
		// case RIN_STATE_SAVE:
		// 	p = "保存到：";
		// 	break;
		// case RIN_QUICK_SLOT:
		// 	p = "快速保存到：";
		// 	break;
		// }
		x=35, y=35;
		// mn_printf(x++,y,setting.color[3],p);
		// y+=12;

		for(i=0; i<=STATE_SLOT_MAX; i++){
			if(nState[i] < 0){
				sprintf(msg,"普通插槽 %d - 暂空", i);
			}else{
				sprintf(msg, "普通插槽 %d - %04d/%02d/%02d %02d:%02d:%02d", i,
					files[nState[i]].d_stat.sce_st_mtime.year,
					files[nState[i]].d_stat.sce_st_mtime.month,
					files[nState[i]].d_stat.sce_st_mtime.day,
					files[nState[i]].d_stat.sce_st_mtime.hour,
					files[nState[i]].d_stat.sce_st_mtime.minute,
					files[nState[i]].d_stat.sce_st_mtime.second);
			}
			mn_printf(x,y,setting.color[i==sel?2:3],msg);
			y += 12;
		}
		y+=12;
		if (state_tmp){
			sprintf(msg, "临时插槽：%04d/%02d/%02d %02d:%02d:%02d",
				state_tmp_time.year,
				state_tmp_time.month,
				state_tmp_time.day,
				state_tmp_time.hour,
				state_tmp_time.minute,
				state_tmp_time.second);
		}else
			strcpy(msg,"临时插槽：暂空");
		mn_printf(x,y++,setting.color[i==sel?2:3],msg);

		pgScreenFlipV();
	}

	ex_sel = sel;
	return sel;
}

void select_cheat(void)
{
	static int sel=0;
	int top=0, rows=21, x, y, h, i;

	cheat_decreate_cheat_map();

	for(;;){
		readpad();
		if(new_pad & CTRL_CIRCLE){
			cheats[sel].enable = !cheats[sel].enable;
		}else if(new_pad & CTRL_SQUARE){
			cheats[0].enable = !cheats[0].enable;
			for (i=0; i<nCheats; i++)
				cheats[i].enable = cheats[0].enable;
		}else if(new_pad & CTRL_CROSS){
			break;
		}else if(new_pad & CTRL_UP){
			sel--;
		}else if(new_pad & CTRL_DOWN){
			sel++;
		}else if(new_pad & CTRL_LEFT){
			sel-=rows/2;
		}else if(new_pad & CTRL_RIGHT){
			sel+=rows/2;
		}

		if(top > nCheats-rows)	top=nCheats-rows;
		if(top < 0)				top=0;
		if(sel >= nCheats)		sel=nCheats-1;
		if(sel < 0)				sel=0;
		if(sel >= top+rows)		top=sel-rows+1;
		if(sel < top)			top=sel;

		rin_frame("","Ｏ：确定 Ｘ：返回  □：所有");

		// スクロールバー
		if(nCheats > rows){
			h = 219;
			pgDrawFrame(445,25,446,248,setting.color[1]);
			pgFillBox(448, h*top/nCheats + 27,
				460, h*(top+rows)/nCheats + 27,setting.color[1]);
		}

		x=30; y=32;
		for (i=0; i<rows; i++){
			if (top+i >= nCheats) break;
			mn_printf(x, y, setting.color[top+i==sel?2:3], cheats[top+i].name);
			if (cheats[top+i].enable)
				mn_printf(x-6, y, setting.color[3], "*");
			y+=10;
		}

		pgScreenFlipV();
	}

	cheat_create_cheat_map();
	return;
}

void rin_menu(void)
{
	enum
	{
		STATE_SAVE,
		STATE_LOAD,
		QUICK_SLOT,
		SAVE_THUMB,
		COMPRESS,
		SCREEN_SIZE,
		GB_TYPE,
		GB_PALETTE,
		TURBO,
		REWIND,
		VSYNC,
		SOUND,
		SOUND_BUFFER,
		MAX_FRAME_SKIP,
		CPU_CLOCK,
		COLOR_CONFIG,
		KEY_CONFIG,
		CODE_PAGE,
		LOAD_CHEAT,
		SELECT_CHEAT,
		LOAD_ROM,
		RESET,
		CONTINUE,
		CREDITS,
	};
	char tmp[MAX_PATH], msg[256]={0};
	static int sel=0;
	int x, y, ret, crs_count=0, bLoop=1;
	int romsize, ramsize;

	old_pad = 0;
	readpad();
	old_pad = paddata.buttons;

	for(;;){
		readpad();
		if(now_pad & (CTRL_UP|CTRL_DOWN|CTRL_LEFT|CTRL_RIGHT))
			crs_count=0;
		if(new_pad)
			msg[0]=0;
		if(new_pad & CTRL_CIRCLE){
			switch(sel)
			{
			case STATE_SAVE:
				ret = rin_stateslot(RIN_STATE_SAVE);
				if(ret>=0){
					strcpy(msg, "即时存档失败");
					if(ret>STATE_SLOT_MAX){
						free(state_tmp);
						state_tmp = save_state_tmp();
						if(state_tmp)
							strcpy(msg, "即时存档成功");
					}else{
						if(save_state(ret))
							strcpy(msg, "即时存档成功");
					}
				}
				crs_count=0;
				break;
			case STATE_LOAD:
				ret = rin_stateslot(RIN_STATE_LOAD);
				if(ret>=0){
					strcpy(msg, "即时读档失败");
					if(ret>STATE_SLOT_MAX){
						if(load_state_tmp(state_tmp))
							// strcpy(msg, "即时读档成功");
							bLoop = 0;
					}else{
						if(load_state(ret))
							// strcpy(msg, "即时读档成功");
							bLoop = 0;
					}
				}
				crs_count=0;
				break;
			case SAVE_THUMB:
				setting.thumb = !setting.thumb;
				break;
			case COMPRESS:
				setting.compress = !setting.compress;
				break;
			case QUICK_SLOT:
				if (++setting.quickslot>STATE_SLOT_MAX+1) setting.quickslot=0;
				break;
			case SCREEN_SIZE:
				ret = rin_screensize(setting.screensize);
				if(ret>=0)
					setting.screensize = ret;
				crs_count=0;
				break;
			case MAX_FRAME_SKIP:
				ret = rin_frameskip(setting.frameskip);
				if(ret>=0)
					setting.frameskip = ret;
				crs_count=0;
				break;
			case TURBO:
				bTurbo = !bTurbo;
				break;
			case REWIND:
				rin_menu_rewind_get_config();
				crs_count=0;
				break;
			case SOUND:
				setting.sound = !setting.sound;
				break;
			case VSYNC:
				setting.vsync = !setting.vsync;
				break;
			case GB_TYPE:
				ret = rin_gbtype(setting.gb_type);
				if(ret>=0){
					setting.gb_type = ret;
					gb_reset();
					bTurbo = 0;
				}
				crs_count=0;
				break;
			case GB_PALETTE:
				ret = rin_gbcolor(setting.gb_palette);
				if(ret>=0){
					setting.gb_palette = ret;
					if(rom_get_info()->gb_type!=1 && now_gb_mode==1)
						lcd_set_mpal(setting.gb_palette);
				}
				crs_count=0;
				break;
			case SOUND_BUFFER:
				ret = rin_sound_buffers(setting.sound_buffer);
				if (ret>=0)
					setting.sound_buffer = ret;
				break;
			case CPU_CLOCK:
				ret = rin_cpuclock(setting.cpu_clock);
				if (ret>=0)
					setting.cpu_clock = ret;
				break;
			case COLOR_CONFIG:
				rin_colorconfig();
				crs_count=0;
				break;
			case KEY_CONFIG:
				rin_keyconfig();
				crs_count=0;
				break;
			case CODE_PAGE:
				setting.utf8_support = !setting.utf8_support;
				break;
			case LOAD_CHEAT:
				if(getFilePath(CheatPath,EXT_TCH)){
					strcpy(msg, "金手指文件加载失败");
					FILE *fp = fopen(CheatPath,"r");
					if (fp){
						if (cheat_load(fp))
							strcpy(msg, "金手指文件加载成功");
						fclose(fp);
					}
				}
				crs_count=0;
				break;
			case SELECT_CHEAT:
				if (nCheats>0){
					select_cheat();
					crs_count=0;
				}
				break;
			case LOAD_ROM:
				for (;;){
					if (!getFilePath(RomPath,EXT_GB|EXT_GZ|EXT_ZIP)){
						if (bLoop)
							break;
						else
							continue;
					}
					strcpy(tmp, RomPath);
					*(strrchr(tmp,'/')+1) = 0;
					strcpy(setting.lastpath, tmp);

					bTurbo = 0;
					bLoop = 0;

					if (rom_get_loaded() && rom_has_battery())
						save_sram(get_sram(), rom_get_info()->ram_size);

					gb_init();

					// 指定したファイルをロードする。 by ruka
					romsize = load_rom(RomPath);
					if (!romsize){
						strcpy(filer_msg,"ROM Load Failed");
						continue;
					}
					ramsize = load_sram(sram_space, sizeof(sram_space));
					if (!gb_load_rom(rom_image, romsize, sram_space, ramsize)){
						strcpy(filer_msg,"ROM Load Failed");
						continue;
					}

					showMsgAboutLoadedRom();

					free(state_tmp);
					state_tmp = NULL;

					break;
				}
				crs_count=0;
				break;
			case CREDITS:
				rin_menu_credits();
				crs_count=0;
				break;
			case RESET:
				gb_reset();
				bTurbo = 0;
				bLoop = 0;
				break;
			case CONTINUE:
				bLoop = 0;
				break;
			}
		}else if(new_pad & CTRL_SQUARE){
			if (sel==QUICK_SLOT){
				if (setting.quickslot>0)
					setting.quickslot--;
				else
					setting.quickslot=STATE_SLOT_MAX+1;
			}
		}else if(new_pad & CTRL_CROSS){
			bLoop = 0;
		}else if(new_pad & CTRL_UP){
			if(sel!=0) sel--;
			else       sel=CONTINUE;
		}else if(new_pad & CTRL_DOWN){
			if(sel!=CREDITS)	sel++;
			else				sel=0;
		}else if(new_pad & CTRL_LEFT){
			if(sel>=COLOR_CONFIG) {
				if (sel >= KEY_CONFIG) sel++;
				sel -= 15;
			}
		}else if(new_pad & CTRL_RIGHT){
			if (sel <= TURBO)
			{
				if (sel >= SAVE_THUMB) sel--;
				sel += 15;
			}
			else if (sel <=CPU_CLOCK)
			{
				sel = CONTINUE;
			}
		}else if(new_pad & CTRL_TRIANGLE){
			rin_menu_credits();
			crs_count=0;
		}else if(get_nShortcutKey(new_pad)==6){
			bLoop = 0;
			break;
		}

		if(!bLoop) break;
		if (crs_count++>=30) crs_count=0;

		rin_frame(msg, "Ｏ：确定 Ｘ：继续  MenuBTN：继续 △: 贡献列表");

		x = 35;
		y = 35;

		mn_printf(x,y,setting.color[3],"即时存档");
		mn_printf(x,y+12,setting.color[3],"即时读档");
		mn_printf(x,y+24,setting.color[3],(setting.quickslot > STATE_SLOT_MAX)?"快速存档：临时插槽":"快速存档：普通插槽 %d",setting.quickslot);
		mn_printf(x,y+36,setting.color[3],"存档缩略图：%s",setting.thumb?"开启":"关闭");
		mn_printf(x,y+48,setting.color[3],"压缩存档：%s",setting.compress?"开启":"关闭");

		mn_printf(x,y+72,setting.color[3],"屏幕尺寸：%s",scr_names[setting.screensize]);
		mn_printf(x,y+84,setting.color[3],"GB类型：%s",gbtype_names[setting.gb_type]);
		mn_printf(x,y+96,setting.color[3],"GB调色盘：%s",pal_names[setting.gb_palette]);
		mn_printf(x,y+108,setting.color[3],"按键连发：%s",bTurbo?"开启":"关闭");
		mn_printf(x,y+120,setting.color[3],"回溯：%s",rin_menu_rewind_get_main_menu_string());
		mn_printf(x,y+132,setting.color[3],"VSYNC：%s",setting.vsync?"开启":"关闭");
		mn_printf(x,y+144,setting.color[3],"声音：%s",setting.sound?"开启":"关闭");
		mn_printf(x,y+156,setting.color[3],"声音缓冲区：%s",sound_buffers[setting.sound_buffer]);
		mn_printf(x,y+168,setting.color[3],"最大跳帧数：%d",setting.frameskip);
		mn_printf(x,y+180,setting.color[3],"CPU频率：%s",cpu_clocks[setting.cpu_clock]);

		mn_printf(x+250,y,setting.color[3],"菜单颜色设置");
        mn_printf(x+250,y+12,setting.color[3],"按键设置");
		mn_printf(x+250,y+24,setting.color[3],"文件系统编码：%s",setting.utf8_support?"UTF-8":"GBK");

		mn_printf(x+250,y+48,setting.color[3],"加载金手指文件");
		mn_printf(x+250,y+60,setting.color[nCheats>0?3:2],"选择作弊码");
		
		mn_printf(x+250,y+84,setting.color[3],"返回ROM列表");
		mn_printf(x+250,y+96,setting.color[3],"重置");
		mn_printf(x+250,y+108,setting.color[3],"继续");
		mn_printf(x+250,y+120,setting.color[3],"贡献列表");
		
		if(crs_count < 15){
			if (sel>=COLOR_CONFIG) {
				x += 245;
				if (sel >= LOAD_ROM)
				{
					y += 24;
				}
				else if (sel >= LOAD_CHEAT)
				{
					y += 12;
				}
				y = y + (sel-COLOR_CONFIG) * 12;
			}
			else
			{
				if (sel >= SCREEN_SIZE)
				{
					y += 12;
				}
				x = x - 5;
				y = y + sel * 12;
			}
			mn_printf(x,y,setting.color[3],">");
		}

		pgScreenFlipV();
	}

	pgFillvram(0);
	pgScreenFlipV();
	pgFillvram(0);
	pgScreenFlipV();
	gb_fill_vframe(0);
	pgWaitVn(10);
	memset(&paddata, 0x00, sizeof(paddata));
	wavoutClear();

	if(render_msg_mode!=6)
		render_msg_mode = 0;
	border_uploaded = 2;
}

static char* gbType2Str(){
	switch(org_gbtype){
		case 1: return "GB";
		case 2: return "SGB";
		case 3: return "GBC";
		default: return "Unknown";
	}
}

void showMsgAboutLoadedRom() {
#define MAX_LEN 70
	char tmp[MAX_LEN]={0};
	if(setting.rewind_enabled){
		snprintf(tmp,MAX_LEN,"ROM TYPE:%s, REWIND: going back %d states available",gbType2Str(),num_rwnd_states);
	}else{
		snprintf(tmp,MAX_LEN,"ROM TYPE:%s, REWIND: disabled",gbType2Str());
	}
	renderer_set_msg(tmp);
}
