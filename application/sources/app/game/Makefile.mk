CFLAGS 			+= 	-I./sources/app/game
CPPFLAGS 		+=	-I./sources/app/game

VPATH			+= sources/app/game

SOURCES_CPP 	+= sources/app/game/itd_game_mainsub.cpp
SOURCES_CPP 	+= sources/app/game/itd_game_bomb.cpp
SOURCES_CPP 	+= sources/app/game/itd_game_boom.cpp
SOURCES_CPP 	+= sources/app/game/itd_game_spike.cpp
SOURCES_CPP 	+= sources/app/game/itd_game_coin.cpp
SOURCES_CPP 	+= sources/app/game/itd_game_border.cpp
SOURCES_CPP		+= sources/app/game/itd_game_gift.cpp