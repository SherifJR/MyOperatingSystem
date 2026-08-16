#include<stdint.h>
#include<cstdint>

struct multiboot_info{
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  uint32_t cmdline;
  uint32_t mods_count;
  uint32_t mods_addr;
  uint32_t syms[4]; // allocates 16 bytes
  uint32_t mmap_length;
  uint32_t mmap_addr;
  uint32_t drivers_length;
  uint32_t drivers_addr;
  uint32_t config_table;
  uint32_t boot_loader_name;
  uint32_t apm_table;
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
  uint8_t framebuffer_type;
  union{
    struct{ //index pallete
      uint32_t framebuffer_pallete_addr;
      uint16_t framebuffer_pallete_num_colors;
    };
    struct{ //RGB
      uint8_t framebuffer_red_field_position;
      uint8_t framebuffer_red_mask_size;
      uint8_t framebuffer_green_field_position;
      uint8_t framebuffer_green_mask_size;
      uint8_t framebuffer_blue_field_position;
      uint8_t framebuffer_blue_mask_size;
   };
  };
} __attribute__((packed));

void render_a_pixel(void *multiboot_structure, int x, int y, int color)
{
  multiboot_info *multi_boot = (multiboot_info *) multiboot_structure;
  uint8_t *fb = (uint8_t *) multi_boot->framebuffer_addr;
  uint8_t *address =(uint8_t *) fb + y*multi_boot->framebuffer_pitch + x*(multi_boot->framebuffer_bpp/8);
  *(uint32_t *) address = color;
}

void draw_rectangle(void *multiboot_structure, int color)
{
  multiboot_info *multi_boot = (multiboot_info *) multiboot_structure;
  for(int row=5; row<(int)multi_boot->framebuffer_height-5; row++)
  {
    for(int col=5; col<(int)multi_boot->framebuffer_width-5; col++)
    {
      render_a_pixel(multiboot_structure, col, row, color);
    }
  }
}


//draw line going from left to right using Bresenham algorothm
void draw_bresenham_line(void *multiboot_structure, int x0, int y0, int x1, int y1, int color){
  int d_x = x1 - x0;
  int d_y = y1 - y0;
  int d = 2*d_y - d_x;
  int _x = x0;
  int _y = y0;

  while(_x<=x1){
    render_a_pixel(multiboot_structure, _x, _y, color);
    _x += 1;

    if(d<0)
    {
      d += 2*d_y;
    }else{
      d += 2 * (d_y - d_x);
      _y += 1;
    }
  }
}

extern "C" void kernel_main(void* multiboot_structure, unsigned int magic)
{
  multiboot_info* multi_boot = (multiboot_info* ) multiboot_structure;
  if(multi_boot->flags & (1<<12)){ //if bit 12 is set, it is true.
    //uint32_t* fb = (uint32_t*) (uint32_t) multi_boot->framebuffer_addr;
    uint32_t width = multi_boot->framebuffer_width;
    uint32_t height = multi_boot->framebuffer_height;

    for(uint32_t y=0; y<height; ++y){ //this loop is to determine which pixel to be filled.
      for(uint32_t x=0; x<width; ++x){  //y: row, x: col.
       //fb[x*width +y] = 0x0000FF;
       if(x % 25 == 0 and y % 30 == 0){
         draw_bresenham_line(multiboot_structure, x, y, x+15, y+15, 0xFFFFFF);
       }
      }
    }
  }
  while(1);
}




/*
void print(const char* str)
{
  unsigned short* VideoMemory = (unsigned short*) 0xb8000;
  for(int i=0; str[i] != '\0'; i++)
    VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

extern "C" void kernel_main(void* multiboot_structure, unsigned int magic_number){
  print("Hello, sherif ");

  while(1);
}
*/
