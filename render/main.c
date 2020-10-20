#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

typedef struct {
    uint16_t* pixels;
    int width, height;
}
canvas_t;

static void put_pixel(canvas_t* const userdata, int const x, int const y, uint16_t color);

/*---------------------------------------------------------------------------*/
/* al_sfxr config and inclusion */
#define AL_BDF_IMPLEMENTATION
#define AL_BDF_CANVAS_TYPE canvas_t*
#define AL_BDF_COLOR_TYPE uint16_t
#define AL_BDF_PUT_PIXEL put_pixel
#include <al_bdf.h>
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* stb_image_write config and inclusion */
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT(x)
#define STB_IMAGE_WRITE_STATIC
#include <stb_image_write.h>
/*---------------------------------------------------------------------------*/

static int fpreader(void* const userdata, void* const buffer, size_t const count) {
    FILE* const fp = (FILE*)userdata;
    return fread(buffer, 1, count, fp);
}

static int load_font(char const* const filename, al_bdf_Font* const font) {
    FILE* const fp = fopen(filename, "rb");

    if (fp == NULL) {
        fprintf(stderr, "Error opening \"%s\": %s\n", filename, strerror(errno));
        return -1;
    }

    int const res = al_bdf_load(font, fpreader, fp);
    fclose(fp);

    if (res != 0) {
        fprintf(stderr, "Error loading \"%s\": %s\n", filename, strerror(errno));
    }

    return res;
}

static void put_pixel(canvas_t* const canvas, int const x, int const y, uint16_t color) {
    canvas->pixels[y * canvas->width + x] = color;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("USAGE: render <font.bdf> text\n");
        return EXIT_FAILURE;
    }

    al_bdf_Font font;

    if (load_font(argv[1], &font) != 0) {
        return EXIT_FAILURE;
    }

    canvas_t canvas;
    int x0, y0;
    al_bdf_size(&font, &x0, &y0, &canvas.width, &canvas.height, argv[2]);

    size_t const count = canvas.width * canvas.height;
    canvas.pixels = (uint16_t*)malloc(count * 2);

    if (canvas.pixels == NULL) {
        fprintf(stderr, "Out of memory\n");
        al_bdf_unload(&font);
        return EXIT_FAILURE;
    }

    memset(canvas.pixels, 0, count * 2);
    al_bdf_render(&font, argv[2], &canvas, 0xff00);
                  
    if (stbi_write_png("render.png", canvas.width, canvas.height, 2, canvas.pixels, canvas.width * 2) == 0) {
        fprintf(stderr, "Error writing image\n");
        free(canvas.pixels);
        al_bdf_unload(&font);
        return EXIT_FAILURE;
    }

    free(canvas.pixels);
    al_bdf_unload(&font);
    return EXIT_SUCCESS;
}
