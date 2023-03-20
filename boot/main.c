typedef struct VBEInfoBlock {
    unsigned short mode_attribute;
    unsigned char win_a_attribute;
    unsigned char win_b_attribute;
    unsigned short win_granuality;
    unsigned short win_size;
    unsigned short win_a_segment;
    unsigned short win_b_segment;
    unsigned int win_func_ptr;
    unsigned short bytes_per_scan_line;
    unsigned short x_resolution;
    unsigned short y_resolution;
    unsigned char char_x_size;
    unsigned char char_y_size;
    unsigned char number_of_planes;
    unsigned char bits_per_pixel;
    unsigned char number_of_banks;
    unsigned char memory_model;
    unsigned char bank_size;
    unsigned char number_of_image_pages;
    unsigned char b_reserved;
    unsigned char red_mask_size;
    unsigned char red_field_position;
    unsigned char green_mask_size;
    unsigned char green_field_position;
    unsigned char blue_mask_size;
    unsigned char blue_field_position;
    unsigned char reserved_mask_size;
    unsigned char reserved_field_position;
    unsigned char direct_color_info;
    unsigned int screen_ptr;
} VBEInfoBlock;

#define VBEInfoAddress 0x8000         // Coresponds with what we put in the assembly, but with an extra 0 (no idea why)
VBEInfoBlock* gVBE;
/*
Produce an RGB value for a VBE render
    R - 4 bits
    B - 5 bits
    G - 4 bits
*/
int rgb(int r, int g, int b) {
    r = (int)(r / 3);
    g = (int)(g / 2);
    b = (int)(b / 3);

    return r << 11 | g << 5 | b;
}


//char* video_memory;

int start() {
    gVBE = (VBEInfoBlock*) VBEInfoAddress;

    for (int i = 0; i < gVBE->x_resolution * gVBE->y_resolution; i++){
        *((unsigned int*)gVBE->screen_ptr + i) = rgb(255,0,0);
    }
    // video_memory = (char *) 0xb8000;

    // for(int i = 0; i < 2  * 25 * 80; i += 2){
    //     *(video_memory + i) = 0;
    //     *(video_memory + i + 1) = 0x02;
    // }
}