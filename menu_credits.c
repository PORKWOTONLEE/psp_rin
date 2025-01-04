//
// Created by Maciej Barczak on 03.08.2018.
//

#include "menu_credits.h"
static unsigned long get_text_color() {
	return setting.color[3];
}

//  "---------------------------------------------------",
static char* credits[] = {
	"     RIN REWIND MOD CREDITS AND VERSION HISTORY",
	"",
	"Mr. Mirakichi - 2005/08/22",
	"  Original version of RIN 1.32",
	"",
	"DaveX (efengeler@gmail.com) - around 2007",
	"  RIN 1.32 Rewind and Mirror mod",
	"",
	 "Papiex (github.com/mbarczak) - summer 2018",
	 "  Rewind mod v2:",
	 "    - Sources ported to latest PSP SDK,",
	 "    - Dedicated menu for rewind,",
	 "    - Support for extra memory (PSP Slim and higher,",
	 "      PS Vita with Adrenaline)",
	 "    - Adjustable memory and states amount in rewind,",
	 "    - Progress bar for actual rewind status,",
	 "    - Fixed crash when loading game with a different",
	 "      save size than the previous one,",
	 "    - Fixed crash when putting PSP to sleep while",
	 "      Turbo mode is engaged.",
	"",
	"PORKWOTONLEE (github.com/PORKWOTONLEE) - winter 2024",
	"  Rewind mod v2 cn_zh(this version):",
	"    - Sources ported to latest PSP SDK,",
	"    - Add GBK codeset support for chinese.",
};
static int credits_len = sizeof(credits)/ sizeof(char*);

void rin_menu_credits() {
	int i,j, page=0;
	for (;;) {
		readpad();
		if (new_pad & (CTRL_CIRCLE|CTRL_TRIANGLE|CTRL_CROSS|CTRL_SQUARE)) {
			return;
		}
		if (new_pad & (CTRL_UP)) {
			if (page-1 >= 0)
				page--;
		}
		if (new_pad & (CTRL_DOWN)) {
			if (page+1 < credits_len-16)
				page++;
		}
	
		rin_frame("贡献列表","↑↓：翻页 ＯＸ△□：返回到上级菜单");
		unsigned y = 35;
		unsigned x = 110;
		for(i=page, j=0; j<17; ++i,++j){
			mn_printf(x,y,get_text_color(), credits[i]);
			y+=12;
		}
		pgScreenFlipV();
	}
}