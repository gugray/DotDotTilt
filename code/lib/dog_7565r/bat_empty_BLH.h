/* File 'C:\Users\Jonathan\Desktop\bat_empty.BLV' as include

 the array starts with a 2 byte header:
  1th Byte: Width of image in dots
  2th Byte: Height of image in dots
  After that image data will follow */

#define bat_empty_LEN  14

const byte bat_empty[bat_empty_LEN]   __attribute__((section(".progmem.data")))=
{
    6, 11,
    0,252,  6,  6,252,  0,  0,  3,  2,  2,  3,  0
};
