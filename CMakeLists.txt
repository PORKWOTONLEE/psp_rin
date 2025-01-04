cmake_minimum_required(VERSION 3.5)

project(rin_gb)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_FIND_DEBUG_MODE ON)

include_directories(. gbcore)

add_executable(${PROJECT_NAME} main.c saveload.c filer.c rewind.c pg.c gz.c
                renderer.c menu_rewind.c menu_credits.c tools.c sound.c menu.c
                image.c gbcore/gb.c gbcore/lcd.c gbcore/cheat.c gbcore/apu.c gbcore/mbc.c 
                gbcore/rom.c gbcore/sgb.c gbcore/cpu.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libpng.a
    ${CMAKE_SOURCE_DIR}/lib/unziplib.a
    z
    pspge
    pspctrl
    pspdisplay
    psppower
    pspaudio
    pspkubridge
)

target_compile_options(rin_gb PRIVATE -Wall -fomit-frame-pointer -mgp32 -mlong32 -msingle-float -mabi=eabi -G3 -Wdouble-promotion -Wno-incompatible-pointer-types -O3)

# Create an EBOOT.PBP file
create_pbp_file(
    TARGET rin_gb
    ICON_PATH ../ICON0.PNG
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE "Rin 1.32 RM v2 cn_zh"
    VERSION 01.00
)
