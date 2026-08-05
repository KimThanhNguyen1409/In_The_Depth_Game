CFLAGS		+= -I./sources/app/screens
CFLAGS		+= -I./sources/app/game/in_the_depth_game
CPPFLAGS	+= -I./sources/app/screens
CPPFLAGS	+= -I./sources/app/game/in_the_depth_game

VPATH += sources/app/screens

# CPP source files
SOURCES_CPP += sources/app/screens/screens_bitmap.cpp
SOURCES_CPP += sources/app/screens/scr_idle.cpp
SOURCES_CPP += sources/app/screens/scr_qrcode.cpp
SOURCES_CPP += sources/app/screens/scr_startup.cpp
SOURCES_CPP += sources/app/screens/scr_welcome.cpp
SOURCES_CPP += sources/app/screens/scr_game_in_the_depth.cpp
SOURCES_CPP += sources/app/screens/scr_game_over.cpp
SOURCES_CPP += sources/app/screens/scr_game_menu.cpp
SOURCES_CPP += sources/app/screens/scr_game_setting.cpp
SOURCES_CPP += sources/app/screens/scr_game_ranking.cpp
