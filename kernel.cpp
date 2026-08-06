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


extern "C" void kernel_main(void* multiboot_structure, unsigned int magic)
{
  multiboot_info* multi_boot = (multiboot_info* ) multiboot_structure;
  if(multi_boot->flags & (1<<12)){ //if bit 12 is set, it is true.
    uint32_t* fb = (uint32_t*) (uint32_t) multi_boot->framebuffer_addr;
    uint32_t width = multi_boot->framebuffer_width;
    uint32_t height = multi_boot->framebuffer_height;

    for(uint32_t x=0; x<height; ++x){ //this loop is to determine which pixel to be filled.
      for(uint32_t y=0; y<width; ++y){  //x: row, y: col.
        fb[x*width +y] = 0x0000FF;
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
