### Conatants ###
bin = game
bin-dir = bin

srcs = game.c
srcs-dir = src

objects = $(patsubst %.c, %.o, $(srcs))
build-dir = build

### SDL Stuff ###
sdl-includes = include/SDL2/
sdl-libs = SDL2-2.0.0 SDL2_image-2.0.0

### Final ###
cflags = -Wall -v

includes = $(sdl-includes)
final-includes = $(addprefix -I , $(includes))

final-objects = $(addprefix $(build-dir)/, $(objects))

lib-dirs = lib
final-lib-dirs = $(addprefix -L , $(lib-dirs))

linked-libs = $(sdl-libs)
final-linked-libs = $(addprefix -l , $(linked-libs))


### Targets ###
bin: build $(objects)
	gcc $(cflags) $(final-objects) -o $(bin-dir)/$(bin) $(final-includes) $(final-lib-dirs) $(final-linked-libs)

build:
	mkdir bin build

$(objects): %.o: %.c
	gcc $(cflags) -c $< && mv $@ $(build-dir)

clean:
	rm -rf bin build
