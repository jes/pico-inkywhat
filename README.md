# InkyWHAT eInk screen library for Raspberry Pi Pico

This is not exactly complete, but the basics are there. I stopped working on it because it
became apparent that the eInk screen was not performant enough for my application.

If you want to use this library, you'll need to add a function like `inky_setpixel()` that allows you to write
bits into the `inky_pixbuf` array. You'll probably also want to remove the default image that it
comes up with, which is a black-and-white representation of my face.

Copy and paste `src/inky.c` and `src/inky.h` from this project into your project, edit them
to suit your needs, and consult `CMakeLists.txt` for tips on how to build it.

Email james@incoherency.co.uk if you want any help.

![demo photograph](demo.jpg)
