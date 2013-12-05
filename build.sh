#!/bin/bash
set -e
EM_DIR=~/src/emscripten

pushd sdl1
$EM_DIR/emmake make
popd

mkdir web || true

build_demo () {
    cp sdl1/$1 web/$1.bc
    $EM_DIR/emcc -o web/$1.html web/$1.bc --preload-file pdcfont.bmp
}

# special build for xmas2
# xmas2.html has been prepared
build_xmas2() {
    cp sdl1/xmas2 web/xmas2.bc
    $EM_DIR/emcc -o web/xmas2.old.js web/xmas2.bc --preload-file pdcfont.bmp -s EXPORTED_FUNCTIONS="['_real_main']"
    # replace $_async_context with for streamline
    cat web/xmas2.old.js | sed 's/$_async_context/_/g' > web/xmas2._js
    _node -c web/xmas2._js
}

# special build for xmas_orig
# xmas_orig.html has been prepared
build_xmas_orig() {
    cp sdl1/xmas_orig web/xmas_orig.bc
    $EM_DIR/emcc -o web/xmas_orig.js web/xmas_orig.bc --preload-file pdcfont.bmp
}

build_demo xmas
exit 0


build_xmas2
exit 0


for demo in firework newdemo ptest rain testcurs tuidemo worm xmas sdltest xmas_orig; do
    echo "Building $demo"
    build_demo $demo
done




