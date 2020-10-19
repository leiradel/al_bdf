# al_bdf

**al_bdf** is a header-only library that renders text using
[Adobe BDF Fonts](https://wwwimages2.adobe.com/content/dam/acom/en/devnet/font/pdfs/5005.BDF_Spec.pdf).

## Instructions

Define `AL_BDF_IMPLEMENTATION` and include this file in one C or C++ file to
create the implementation. If you need to declare the functions somewhere
else, include this file without defining `AL_BDF_IMPLEMENTATION`; the same
configuration used in the implementation must be used.

**al_bdf** needs some user defined configuration to build:

* `AL_BDF_CANVAS_TYPE`: The type of the canvas where text will be rendered to.
  I.e. this would be a `SDL_Surface` when rendering to a SDL surface.
* `AL_BDF_COLOR_TYPE`: The type of the color to use when rendering. I.e. for a
  32 bits per pixel canvas, this would be an `uint32_t`.
* `AL_BDF_PUT_PIXEL`: A macro that will be called to render each individual
  pixel in the canvas. It'll receive the following parameters:
    1. `AL_BDF_CANVAS_TYPE canvas`: Where to render the text to
    1. `int x`: The horizontal coordinate of the pixel to render (0 is left)
    1. `int y`: The vertical coordinate of the pixel to render (0 is top)
    1. `AL_BDF_COLOR_TYPE color`: The color to set the pixel
* `AL_BDF_PUT_PIXELS`: A macro to be used instead of `AL_BDF_PUT_PIXEL`. This
  one will receive a bit pattern for eight horizontal pixels to be drawn
  starting at `x` and `y`. It takes the same parameters, plus `uint8_t pattern` with each bit set corresponding to a pixel that must be rendered, from most
  significant bit to less significant.

In addition to the mandatory macros, the following macros can be defined to
further configure **al_bdf**:

* `AL_BDF_MALLOC`: A function that allocates memory, compatible with `malloc`.
* `AL_BDF_REALLOC`: A function that realocates memory, compatible with
  `realloc`.
* `AL_BDF_FREE`: A function that frees memory, compatible with `free`

The memory management macros must either be all defined or not defined. If not
defined, they will default to the `libc` functions.

The API is fully documented in the `al_bdf.h` file.

## TL;DR

```cpp
// Load a BDF font
FILE* const fp = fopen("b10.bdf", "rb");

if (al_bdf_load(font, fpreader, fp) != 0) {
    // error
}

// Evaluate the size of the "al_bdf" text
int x0, y0, width, height;
al_bdf_size(&font, &x0, &y0, &width, &height, "al_bdf");

// Allocate a canvas big enough to render the text
size_t const count = width * height;
uint16_t* const pixels = (uint16_t*)malloc(count * 2);

// Clear the canvas, as al_bdf_render only sets pixels
memset(pixels, 0, count * 2);

// Render the text
al_bdf_render(&font, "al_bdf", pixels, 0xff00);
```

## License

The MIT License (MIT)

Copyright (c) 2020 Andre Leiradella

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
