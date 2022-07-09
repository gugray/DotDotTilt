



#define bat_empty_LEN  18

const byte  bat_empty[ bat_empty_LEN] __attribute__((section(".progmem.data"))) =
{
   16,  8,
  127, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,127, 28, 28
};




#define sd_ok_LEN  18

const byte sd_ok[sd_ok_LEN] __attribute__((section(".progmem.data"))) =
{
   16,  8,
  127, 81, 85, 85, 69,127, 65, 93, 93, 99,127,127,117, 21, 31,  0
};



#define gps_img_LEN  18

const byte gps_img[gps_img_LEN] __attribute__((section(".progmem.data"))) =
{
   16,  8,
   62, 34, 42, 58,  0, 62, 10, 10, 14,  0, 46, 42, 42, 58,  0,  0
};


#define gpsfixed_LEN  10

const byte gpsfixed[gpsfixed_LEN] __attribute__((section(".progmem.data"))) =
{
     8,  8,
    12, 63, 63, 12,  0, 30,  0, 63
};

