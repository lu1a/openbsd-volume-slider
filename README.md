# openbsd-volume-slider

GUI volume slider using mixerctl

`gcc -o vol-slider main.c $(pkg-config --cflags raylib --libs raylib)`

or

`gcc -o vol-slider main.c -lraylib -Lmodules/x86_64`

For whatever reason, if you want to run this OpenBSD volume slider on a Mac, there's this:

`gcc -o vol-slider main.c -lraylib -Lmodules/darwin_arm64`
