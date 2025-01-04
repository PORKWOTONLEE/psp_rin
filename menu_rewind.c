//
// Created by Papiex on 18.07.2018.
//

#include "menu_rewind.h"
#include "tools.h"
#include "rewind.h"

#define MAX_MENU_ENTRY_LENGTH 200

static int rin_menu_rewind_get_config_decrease_row(const int MAX_ROW, int sel);
static int rin_menu_rewind_get_config_increase_row(const int MAX_ROW, int sel);
static void rin_menu_rewind_get_config_save_value(SETTING* localSettings);
static void rin_menu_rewind_get_config_increase_value(SETTING *local, short longStep);
static void rin_menu_rewind_get_config_decrease_value(SETTING *local, short longStep);
static void rin_menu_rewind_get_config_show_current(long sel,int* crs_count,SETTING* local);
static void rin_menu_rewind_get_config_toogle_max(SETTING *local);
static void change_value(u32 *baseValue, int lowerBound, int upperBound, int step, int direction);
static void rin_frame_rewind_use_max();
static void rin_frame_rewind_no_max();
static void rin_frame_rewind_change_mode();
static void rin_frame_rewind_disabled();
static void change_selected_value(SETTING *local, int direction, short longStep);
static void print_rewind_memory_limit_line(unsigned long *x, unsigned long *y, const SETTING *local, const long sel);
static void print_rewind_states_limit_line(unsigned long *x, unsigned long *y, const SETTING *local, const long sel);
static void print_rewind_states_help(unsigned long *x, unsigned long *y, const SETTING *local, const long sel);
static void rin_frame_rewind(SETTING *local, long sel);

/*
 *
 * 0 : Limit mode : memory amount
 * 1 : Preferred memory amount designed for rewind purposes (current max: 23mb): 9mb | Always use MAX
 *    rin_frame("□：不使用默认值 Ｘ：取消 Ｏ：保存 ");
 *    rin_frame("□：使用默认值 <-：减少 ->：增加 Ｘ：取消 Ｏ：保存 ");
 *
 * 0 : Limit mode : number of states
 * 1 : Preferred number of rewind states : 10 states | Always use MAX
 *    rin_frame("□：不使用默认值 Ｘ：取消 Ｏ：保存 ");
 *    rin_frame("□：使用默认值 <-：减少 ->：增加 Ｘ：取消 Ｏ：保存 ");
 */

static void rin_menu_rewind_get_config_toogle_max(SETTING *local) {
	if(local->rewind_limit_mode == REWIND_MODE_LIMIT_MEMORY_AMOUNT){
		local->rewind_always_use_max_memory = (u8)!local->rewind_always_use_max_memory;
	}else{
		local->rewind_always_use_max_states = (u8)!local->rewind_always_use_max_states;
	}
}

#define REWIND_MEMORY_STEP ((int)(1 * 1024 *1024))
#define REWIND_STATES_STEP 10
#define REWIND_MEMORY_STEP_SLOW ((int)(0.1 * 1024 *1024))
#define REWIND_STATES_STEP_SLOW 1

static void rin_menu_rewind_get_config_decrease_value(SETTING *local, short longStep) {
	change_selected_value(local, -1, longStep);
}
static void rin_menu_rewind_get_config_increase_value(SETTING *local, short longStep) {
	change_selected_value(local, 1, longStep);
}

static void change_selected_value(SETTING *local, int direction, short longStep) {
	if(local->rewind_limit_mode == REWIND_MODE_LIMIT_MEMORY_AMOUNT){
		if(!local->rewind_always_use_max_memory){
			change_value(&local->rewind_user_max_memory_ammount,REWIND_MIN_USER_MEMORY,
			             REWIND_MAX_USER_MEMORY,longStep?REWIND_MEMORY_STEP:REWIND_MEMORY_STEP_SLOW,direction);
		}
	}else{
		if(!local->rewind_always_use_max_states){
			change_value(&local->rewind_user_max_states_ammount,REWIND_MIN_USER_STATES,
			             REWIND_MAX_USER_STATES,longStep?REWIND_STATES_STEP:REWIND_STATES_STEP_SLOW,direction);
		}
	}
}

static void change_value(u32 *baseValue, int lowerBound, int upperBound, int step, int direction){
	int bv = (*baseValue);
	bv += (step*direction);
	if(bv > upperBound){
		bv = lowerBound;
	} else if(bv < lowerBound){
		bv = upperBound;
	}
	(*baseValue) = (u32)bv;
}

static void rin_menu_rewind_get_config_save_value(SETTING* localSettings) {
	memcpy(&setting,localSettings, sizeof(SETTING));
}

static int rin_menu_rewind_get_config_increase_row(const int MAX_ROW, int sel) {
	sel--;
	if(sel<0) sel=MAX_ROW;
	return sel;
}

static int rin_menu_rewind_get_config_decrease_row(const int MAX_ROW, int sel) {
	sel++;
	if(sel>MAX_ROW) sel=0;
	return sel;
}

static unsigned long rewind_get_text_color(const long sel,const int element) {
//	return setting.color[sel==element?2:3];
	return setting.color[3];
}

void rin_menu_rewind_get_config_show_current(long sel,int* crs_count,SETTING* local) {

	unsigned long x=35,y=35;
	rin_frame_rewind(local, sel);
	if(local->rewind_enabled) {
		mn_printf(x, y, rewind_get_text_color(sel, 0), "回溯模式: %s",
		         local->rewind_limit_mode == REWIND_MODE_LIMIT_MEMORY_AMOUNT ? "根据设备内存大小"
		                                                                     : "根据回溯时间点个数");
		y+=24;
		if (local->rewind_limit_mode == REWIND_MODE_LIMIT_MEMORY_AMOUNT) {
			print_rewind_memory_limit_line(&x, &y, local, sel);
		} else {
			print_rewind_states_limit_line(&x, &y, local, sel);
		}
		print_rewind_states_help(&x, &y,local,sel);
	}else{
		mn_printf(x, y, rewind_get_text_color(sel, 0), "禁用回溯（按△键启用）");
	}
	if ((*crs_count)++>=30) (*crs_count)=0;
	if ((*crs_count) < 15  && local->rewind_enabled) {
		x-=5;
		if (sel == 0) {
			y = 35;
		} else if (sel == 1) {
			y = 59;
		}
		mn_printf(x,y,setting.color[3],">");
	}
	pgScreenFlipV();
}

void rin_frame_rewind(SETTING *local, long sel) {
	if(local->rewind_enabled){
		if(sel == 0){
			rin_frame_rewind_change_mode();
		}else{
			if(local->rewind_limit_mode == REWIND_MODE_LIMIT_MEMORY_AMOUNT){
				if(local->rewind_always_use_max_memory){
					rin_frame_rewind_use_max();
				}else{
					rin_frame_rewind_no_max();
				}
			}else{
				if(local->rewind_always_use_max_states){
					rin_frame_rewind_use_max();
				}else{
					rin_frame_rewind_no_max();
				}
			}
		}
	}else{
		rin_frame_rewind_disabled();
	}
}

static void print_rewind_states_limit_line(unsigned long *x, unsigned long *y, const SETTING *local, const long sel) {
	char tmpString[MAX_MENU_ENTRY_LENGTH] = {0};
	if(local->rewind_always_use_max_states){
		snprintf(tmpString,MAX_MENU_ENTRY_LENGTH,"默认值");
	}else{
		snprintf(tmpString,MAX_MENU_ENTRY_LENGTH,"%d 个时间点",local->rewind_user_max_states_ammount);
	}
	mn_printf(*x,*y,rewind_get_text_color(sel,1),
	         "自定义时间点个数: %s",tmpString);
}

static void print_rewind_states_help(unsigned long *x, unsigned long *y, const SETTING *local, const long sel) {
#define HELP_COLOR RGB(26, 163, 255)
	unsigned help_x = 35;
	unsigned help_y = 120;
	char tmpString[MAX_MENU_ENTRY_LENGTH] = {0};
	if(local->rewind_always_use_max_states){
		snprintf(tmpString,MAX_MENU_ENTRY_LENGTH,"默认值");
	}else{
		snprintf(tmpString,MAX_MENU_ENTRY_LENGTH,"%d 个时间点",local->rewind_user_max_states_ammount);
	}

	mn_printf(help_x,help_y,HELP_COLOR, "---------------------------------------------------");
	help_y += 12;
	mn_printf(help_x,help_y,HELP_COLOR, "在新款PSP上（如PSP Slim或后续机型），通过自定义固件");
	help_y += 12;
	mn_printf(help_x,help_y,HELP_COLOR, "可以开启额外的内存用于回溯");
	help_y += 24;
	mn_printf(help_x,help_y,HELP_COLOR, "在Recovery菜单中，可以通过以下方式开启额外内存：");
	help_y += 12;
	mn_printf(help_x,help_y,HELP_COLOR, "\"Advanced/Force High Memory Layout\".");
	help_y += 12;
	mn_printf(help_x,help_y,HELP_COLOR, "以上方法同样适用于PSVita的肾上腺素模拟器");
	help_y += 12;
	mn_printf(help_x,help_y,HELP_COLOR, "----------------------------------------------------");
	help_y += 12;
	mn_printf(help_x,help_y,HELP_COLOR, "");
}

static void print_rewind_memory_limit_line(unsigned long *x, unsigned long *y, const SETTING *local, const long sel) {
	char tmpString[MAX_MENU_ENTRY_LENGTH] = {0};

	if(local->rewind_always_use_max_memory){
		snprintf(tmpString,MAX_MENU_ENTRY_LENGTH,"使用默认值");
	}else{
		snprintf(tmpString,MAX_MENU_ENTRY_LENGTH,"%.1f MB",byte2mb_asFloat(local->rewind_user_max_memory_ammount));
	}
	mn_printf(*x,*y,rewind_get_text_color(sel,1),
	         "自定义内存大小");
	(*y)+=12;
	mn_printf(*x,*y,rewind_get_text_color(sel,1),
	         "（当前值： %.1f MB）: %s",byte2mb_asFloat(max_rewind_memory),tmpString);
}

static char* rin_frame_get_title(){
	return "回溯设置";
}

static void rin_frame_rewind_use_max() {
	rin_frame(rin_frame_get_title(),"□：不使用默认值 Ｘ：取消 Ｏ：保存 ");
}

static void rin_frame_rewind_no_max(){
	rin_frame(rin_frame_get_title(),"□：使用默认值 ←：减少 →：增加 △：（按住）微调 Ｘ：取消 Ｏ：保存 ");
}

static void rin_frame_rewind_disabled(){
	rin_frame(rin_frame_get_title(),"△：启用回溯 Ｘ：取消 Ｏ：保存 ");
}

static void rin_frame_rewind_change_mode(){
	rin_frame(rin_frame_get_title(),"←→：改变回溯模式 △: 禁用回溯 Ｘ：取消 Ｏ：保存 ");
}

char *rin_menu_rewind_get_main_menu_string() {
	static char buf[MAX_MENU_ENTRY_LENGTH] = {0};
#if 0
	if(setting.rewind_limit_mode == REWIND_MODE_LIMIT_MEMORY_AMOUNT) {
		if(setting.rewind_always_use_max_memory){
			snprintf(buf,MAX_MENU_ENTRY_LENGTH,"RAM占用：%.1fMB（可回溯 %d 个时间点）",
					get_current_rewind_memory_string(setting),num_rwnd_states);
		} else{
			snprintf(buf,MAX_MENU_ENTRY_LENGTH,"RAM占用：用户自定义，已使用%.1fMB",get_current_rewind_memory_string(setting));
		}
	}else{
		if(setting.rewind_always_use_max_states){
			snprintf(buf,MAX_MENU_ENTRY_LENGTH,"时间点个数：无限制，已使用 %d 个时间点",num_rwnd_states);
		}else{
			snprintf(buf,MAX_MENU_ENTRY_LENGTH,"时间点个数：用户自定义，已使用 %d 个时间点",num_rwnd_states);
		}
	}
#endif
	if(setting.rewind_enabled){
		snprintf(buf,MAX_MENU_ENTRY_LENGTH,"RAM占用：%.1fMB（可回溯 %d 个时间点）",
		         byte2mb_asFloat(get_current_rewind_memory(setting)),num_rwnd_states);
	}else{
		snprintf(buf,MAX_MENU_ENTRY_LENGTH,"禁用");
	}

	return buf;
}
void rin_menu_rewind_get_config(void) {
	const int MAX_ROW = 1;
	SETTING localSettings;
	memcpy(&localSettings,&setting, sizeof(SETTING));
	long sel=0;
	int crs_count=0;
	short longStep = 1;
	for(;;){
		readpad();
		if(now_pad & (CTRL_UP|CTRL_DOWN|CTRL_LEFT|CTRL_RIGHT))
			crs_count=0;
		if(now_pad & CTRL_TRIANGLE) {
			longStep = 0;
		}else{
			longStep = 1;
		}
		if(new_pad & CTRL_TRIANGLE){
			if(sel == 0){
				localSettings.rewind_enabled = (u8)!localSettings.rewind_enabled;
			}
		}
		if(new_pad & CTRL_CIRCLE){
			rin_menu_rewind_get_config_save_value(&localSettings);
			free_rewind_states();
			allocate_rewind_states();
			break;
		}else if(new_pad & CTRL_CROSS){
			break;
		}else if(new_pad & CTRL_SQUARE && localSettings.rewind_enabled){
			if(sel == 1 ) {
				rin_menu_rewind_get_config_toogle_max(&localSettings);
			}
		}else if(new_pad & CTRL_DOWN && localSettings.rewind_enabled){
			sel = rin_menu_rewind_get_config_decrease_row(MAX_ROW, sel);
		}else if(new_pad & CTRL_UP && localSettings.rewind_enabled){
			sel = rin_menu_rewind_get_config_increase_row(MAX_ROW, sel);
		}else if(new_pad & CTRL_RIGHT && localSettings.rewind_enabled){
			if(sel == 0){
				localSettings.rewind_limit_mode = (u8)!localSettings.rewind_limit_mode;
			}else if (sel == 1){
				rin_menu_rewind_get_config_increase_value(&localSettings, longStep);
			}
		}else if(new_pad & CTRL_LEFT && localSettings.rewind_enabled){
			if(sel == 0){
				localSettings.rewind_limit_mode = (u8)!localSettings.rewind_limit_mode;
			}else if (sel == 1){
				rin_menu_rewind_get_config_decrease_value(&localSettings, longStep);
			}
		}
		rin_menu_rewind_get_config_show_current(sel,&crs_count,&localSettings);
	}
}
