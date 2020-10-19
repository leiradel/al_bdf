#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

static void put_pixel(uint16_t* const canvas, int const x, int const y, uint16_t const color, size_t const pitch);

/*---------------------------------------------------------------------------*/
/* al_sfxr config and inclusion */
#define AL_BDF_IMPLEMENTATION
#define AL_BDF_CANVAS_TYPE uint16_t*
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

static void put_pixel(uint16_t* const canvas, int const x, int const y, uint16_t const color, size_t const pitch) {
    canvas[y * pitch / 2 + x] = color;
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

    int x0, y0, width, height;
    al_bdf_size(&font, &x0, &y0, &width, &height, argv[2]);

    size_t const count = width * height;
    uint16_t* const canvas = (uint16_t*)malloc(count * 2);

    if (canvas == NULL) {
        fprintf(stderr, "Out of memory\n");
        al_bdf_unload(&font);
        return EXIT_FAILURE;
    }

    memset(canvas, 0, count * 2);

    al_bdf_render(&font, argv[2], canvas, 0xff00, width * 2);
                  
    if (stbi_write_png("render.png", width, height, 2, canvas, width * 2) == 0) {
        fprintf(stderr, "Error writing image\n");
        free(canvas);
        al_bdf_unload(&font);
        return EXIT_FAILURE;
    }

    free(canvas);
    al_bdf_unload(&font);
    return EXIT_SUCCESS;
}
