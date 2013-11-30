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

build_demo xmas
exit 0

for demo in firework newdemo ptest rain testcurs tuidemo worm xmas sdltest; do
    echo "Building $demo"
    build_demo $demo
done




