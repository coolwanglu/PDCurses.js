#!/bin/bash
EM_DIR=~/src/emscripten
PATH=$EM_DIR/system/bin:$PATH

pushd sdl1
$EM_DIR/emmake make -j4 CFLAGS=-Os
popd

mkdir web || true

build_demo () {
    pushd web
    cp ../sdl1/$1 $1.bc
    $EM_DIR/emcc -Oz -o $1.html $1.bc --preload-file pdcfont.bmp
    popd
}

for demo in firework newdemo ptest rain testcurs tuidemo worm xmas sdltest; do
    echo "Building $demo"
    build_demo $demo
done

