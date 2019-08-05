CC=gcc
# -ffast-math is our emulation of the Wolf3D performance... :-)
CFLAGS=-g -W -Wall -Werror -Wextra -DDEBUG -ffast-math `guile-config compile`
LDFLAGS=-lm -lSDL2 `guile-config link`
SOURCES=main.c game.c map.c render.c player.c util.c
BINARY=game

${BINARY}: ${SOURCES}
	${CC} ${SOURCES} ${CFLAGS} ${LDFLAGS} -o ${BINARY}

clean:
	rm -f ${BINARY}
