#include <reg52.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define uchar unsigned char
#define data  1
#define comm  0
#define f_page  0xb8
#define f_row   0xc0
#define f_line  0x00

sbit rst=P3^0;
sbit e1 =P2^0;
sbit e2 =P2^1;
sbit rw =P3^3;
sbit a0 =P3^4;

sbit bf =P1^7;

void wr_lcd (uchar choe1,uchar choe2,uchar data_comm,uchar content);
/*choe1为1,控制左半屏,choe2为1,控制右半屏*/
void chk_busy (uchar choe1,uchar choe2);
void delay (unsigned int us);
void delay1 (unsigned int ms);
//void init_lcd (void);
void disphz (uchar code *chn);
void dispzf (uchar code *chn);
void disptu (uchar code *img);
void wrlattice (uchar data1,uchar data2);

uchar code tab1[]={
/*--  文字:  中  --*/
0x00,0x00,0xFC,0x08,0x08,0x08,0x08,0xFF,0x08,0x08,0x08,0x08,0xFC,0x08,0x00,0x00,
0x00,0x00,0x07,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x02,0x02,0x07,0x00,0x00,0x00,
/*--  文字:  科  --*/
0x10,0x12,0x92,0x72,0xFE,0x51,0x91,0x00,0x22,0xCC,0x00,0x00,0xFF,0x00,0x00,0x00,
0x04,0x02,0x01,0x00,0xFF,0x00,0x04,0x04,0x04,0x02,0x02,0x02,0xFF,0x01,0x01,0x00,
/*--  文字:  国  --*/
0x00,0xFE,0x02,0x0A,0x8A,0x8A,0x8A,0xFA,0x8A,0x8A,0x8A,0x0A,0x02,0xFE,0x00,0x00,
0x00,0xFF,0x40,0x48,0x48,0x48,0x48,0x4F,0x48,0x49,0x4E,0x48,0x40,0xFF,0x00,0x00,
/*--  文字:  技  --*/
0x08,0x08,0x88,0xFF,0x48,0x28,0x00,0xC8,0x48,0x48,0x7F,0x48,0xC8,0x48,0x08,0x00,
0x01,0x41,0x80,0x7F,0x00,0x40,0x40,0x20,0x13,0x0C,0x0C,0x12,0x21,0x60,0x20,0x00,
/*--  文字:  肇  --*/
0x80,0x60,0x1C,0x95,0x96,0x94,0x9C,0xC8,0xA4,0xAF,0x94,0x94,0x2C,0x44,0x44,0x00,
0x20,0x22,0x2A,0x2A,0x2A,0x2A,0x2A,0xFF,0x2A,0x2A,0x2A,0x2F,0x22,0x22,0x20,0x00,
/*--  文字:  有  --*/
0x00,0x04,0x84,0x44,0xE4,0x34,0x2C,0x27,0x24,0x24,0x24,0xE4,0x04,0x04,0x04,0x00,
0x02,0x01,0x00,0x00,0xFF,0x09,0x09,0x09,0x29,0x49,0xC9,0x7F,0x00,0x00,0x00,0x00,
/*--  文字:  庆  --*/
0x00,0x00,0xFC,0x44,0x44,0x44,0x44,0xC5,0x7E,0xC4,0x44,0x44,0x44,0x44,0x44,0x00,
0x40,0x30,0x0F,0x40,0x20,0x10,0x0C,0x03,0x00,0x01,0x06,0x18,0x30,0x60,0x20,0x00,
/*--  文字:  限  --*/
0xFE,0x02,0x32,0x4E,0x82,0x00,0xFE,0x4A,0xCA,0x4A,0x4A,0x4A,0x7E,0x00,0x00,0x00,
0xFF,0x00,0x02,0x04,0x03,0x00,0xFF,0x40,0x20,0x03,0x0C,0x12,0x21,0x60,0x20,0x00,
/*--  文字:  金  --*/
0x40,0x40,0x20,0x20,0x50,0x48,0x44,0xC3,0x44,0x48,0x50,0x50,0x20,0x60,0x20,0x00,
0x40,0x40,0x42,0x42,0x4A,0x72,0x42,0x7F,0x42,0x62,0x5A,0x42,0x42,0x40,0x40,0x00,
/*--  文字:  公  --*/
0x00,0x00,0x80,0x40,0x30,0x0E,0x84,0x00,0x00,0x0E,0x10,0x60,0xC0,0x80,0x80,0x00,
0x00,0x01,0x20,0x70,0x28,0x24,0x23,0x31,0x10,0x10,0x14,0x78,0x30,0x01,0x00,0x00,
/*--  文字:  鹏  --*/
0x00,0xFE,0x12,0xFE,0x00,0xFE,0x12,0xFE,0x00,0xFC,0x0E,0x35,0x44,0x7C,0x00,0x00,
0x20,0x1F,0x21,0x7F,0x20,0x1F,0x41,0x7F,0x08,0x09,0x09,0x29,0x49,0x21,0x1F,0x00,
/*--  文字:  司  --*/
0x00,0x10,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x12,0x02,0x02,0xFE,0x00,0x00,
0x00,0x00,0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x20,0x40,0x3F,0x00,0x00
};
uchar code tab2[]={
/*--  文字:  8  --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
/*--  文字:  G  --*/
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,
/*--  文字:  6  --*/
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  o  --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
/*--  文字:  -  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
/*--  文字:  l  --*/
0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  7  --*/
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
/*--  文字:  d  --*/
0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,
/*--  文字:  5  --*/
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  e  --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
/*--  文字:  8  --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
/*--  文字:  n  --*/
0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,
/*--  文字:  -  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
/*--  文字:  P  --*/
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,
/*--  文字:  2  --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
/*--  文字:  a  --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,
/*--  文字:  2  --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
/*--  文字:  l  --*/
0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  2  --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
/*--  文字:  m  --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,
/*--  文字:  3  --*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:     --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  1  --*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  L  --*/
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,
/*--  文字:  9  --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,
/*--  文字:  C  --*/
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,
/*--  文字:  6  --*/
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  M  --*/
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00
};
uchar code tab3[]={
/*--  调入了一幅图像：F:\梁\其它\HOCO12232.bmp  --*/
/*--  宽度x高度=122x32  --*/
0x00,0x00,0x00,0x70,0xF8,0xF8,0xFC,0xFC,0xFE,0xFE,0xFE,0xFE,0xFC,0xFC,0xF0,0xE0,
0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x48,0xD4,0xB4,0x62,0xC2,0x82,0x03,0x02,0x02,
0x02,0x84,0xC4,0xE0,0xA0,0xE0,0x60,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xB0,0xF8,0xF0,0xF0,0xF8,0xFC,0x60,0x60,0x30,
0x18,0x00,0x00,0x00,0x80,0xC0,0xE0,0xE0,0xF0,0xF0,0xF0,0xD8,0x58,0x58,0x58,0x58,
0x58,0xD8,0xD8,0x98,0xB0,0x30,0x70,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x07,
0x07,0x07,0x02,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xFC,0xFC,0xFC,
0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xF8,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x01,0x00,0x04,
0x0D,0x1B,0x36,0x6D,0xDB,0xF6,0xED,0xFB,0xFE,0xFE,0xFF,0x7F,0x1F,0x07,0x01,0x00,
0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE,0xFE,
0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xE0,0xC0,0xC0,0xF8,0xFE,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x3E,0xF8,0x01,0x37,
0x0C,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x60,0xFF,0xC7,0x03,0x03,0x03,0x03,0xE1,
0xFF,0x3F,0x0F,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x0F,0x3F,0x7F,0xFF,0xEF,
0xBF,0x7F,0xFF,0xC7,0x07,0x1F,0xFE,0xF8,0xB0,0x60,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x20,0x40,0x40,0x80,0x83,
0xFF,0xFF,0xFF,0xFF,0x83,0x40,0x40,0x20,0x10,0x08,0x04,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x0F,0x1F,0x3F,0x3F,0x7F,0xFF,
0xFF,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xDF,0x3F,0x3F,0x0F,0x61,0x3F,0x1F,0x2F,0x3F,
0x1F,0x0F,0x00,0x0F,0x07,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x1E,0x07,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x40,0x60,0x70,0x3C,0x1F,0x07,0x03,0x01,0x01,0x00,0x00,
0x01,0x07,0x07,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x44,0xEC,0x00,
0x7C,0x44,0x7C,0x00,0x7C,0x40,0x40,0x00,0x7C,0x44,0x38,0x00,0x7C,0x14,0x1C,0x00,
0x7C,0x40,0x40,0x00,0x78,0x24,0x78,0x00,0x7C,0x18,0x18,0x7C,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x20,0x20,0x20,0x20,0x10,0x04,0x07,0x00,0x01,0x00,0x00,0x00,0x01,
0x05,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


/*------------------------------------------*/
void wr_lcd (uchar choe1,uchar choe2,uchar data_comm,uchar content)
{
  chk_busy (choe1,choe2);
  if(data_comm)
   {
    a0=1;   //data
    rw=0;   //write
   }
  else 
   {
    a0=0;   //command
    rw=0;   //write
   }
  if(choe1==1)
  {
    P1=content;   //output data or comm
    e1=1;
    ;
    e1=0;
  }
  if(choe2==1)
  {
    P1=content;   //output data or comm
    e2=1;
    ;
    e2=0;
  }
}
void chk_busy (uchar choe1,uchar choe2)
{
  if(choe1==1)
  {
    a0=0;
    rw=1;
    e1=1;
    while(bf==1)
      delay(2);
    e1=0;
  }
  if(choe2==1)
  {
    a0=0;
    rw=1;
    e2=1;
    while(bf==1)
      delay(2);
    e2=0;
  }
}
void delay (unsigned int us)   //delay time
{
  while(us--);
}
void delay1 (unsigned int ms)
{
  unsigned int i,j;
  for(i=0;i<ms;i++)
  for(j=0;j<1000;j++)
  ;
}
/*------------------初始化-----------------*/
void init_lcd (void)
{
  rst=1;
  ;
  wr_lcd (1,1,comm,0xe2);
  wr_lcd (1,1,comm,0xa4);
  wr_lcd (1,1,comm,0xa9);
  wr_lcd (1,1,comm,0xa0);
  wr_lcd (1,1,comm,0xee);
  wr_lcd (1,1,comm,0xaf);
}
/*--------------显示汉字(16x16)---------------*/
void disphz (uchar code *chn)
{
  uchar i,j,k;
  for(k=0;k<3;k++)
  {
    for(j=0;j<4;j++)
    {
      wr_lcd (1,0,comm,f_page+j);
      wr_lcd (1,0,comm,f_row);
      wr_lcd (1,0,comm,f_line+12+k*16);
      for(i=0;i<16;i++)
        wr_lcd (1,0,data,chn[k*64+j*16+i]);
    }
  }
  for(k=3;k<6;k++)
  {
    for(j=0;j<4;j++)
    {
      wr_lcd (0,1,comm,f_page+j);
      wr_lcd (0,1,comm,f_row);
      wr_lcd (0,1,comm,f_line+k*16-48);
      for(i=0;i<16;i++)
        wr_lcd (0,1,data,chn[k*64+j*16+i]);
    }
  }
}
/*--------------显示字符(8x16)---------------*/
void dispzf (uchar code *eng)
{
  uchar i,j,k;
  for(k=0;k<7;k++)
  {
    for(j=0;j<4;j++)
    {
      wr_lcd (1,0,comm,f_page+j);
      wr_lcd (1,0,comm,f_row);
      wr_lcd (1,0,comm,f_line+5+k*8);
      for(i=0;i<8;i++)
        wr_lcd (1,0,data,eng[k*32+j*8+i]);
    }
  }
  for(k=7;k<14;k++)
  {
    for(j=0;j<4;j++)
    {
      wr_lcd (0,1,comm,f_page+j);
      wr_lcd (0,1,comm,f_row);
      wr_lcd (0,1,comm,f_line+k*8-56);
      for(i=0;i<8;i++)
        wr_lcd (0,1,data,eng[k*32+j*8+i]);
    }
  }
}
/*----------------画图形-----------------*/
void disptu (uchar code *img)
{
  uchar i,j;
  for(j=0;j<4;j++)
  {
    wr_lcd (1,1,comm,f_page+j);
    wr_lcd (1,1,comm,f_row);
    wr_lcd (1,1,comm,f_line);
    for(i=0;i<122;i++)
    {
      if(i<61)
      {wr_lcd (1,0,comm,f_line+i);
       wr_lcd (1,0,data,img[j*122+i]);}
      else
      {wr_lcd (0,1,comm,f_line+i-61);
       wr_lcd (0,1,data,img[j*122+i]);}
    }
  }
}
/*--------------写点阵------------------*/
void wrlattice (uchar data1,uchar data2)
{
  uchar i,j;
  for(j=0;j<4;j++)
  {
    wr_lcd (1,1,comm,f_page+j);
    wr_lcd (1,1,comm,f_row);
    wr_lcd (1,1,comm,f_line);
    for(i=0;i<61;i=i+2)
    {
      wr_lcd (1,1,data,data1);
      wr_lcd (1,1,data,data2);
    }
  }
}
/*------------------主程序--------------------*/
void main ()
{
  SP=0x5f;
  init_lcd ();
  while (1)
  {
    wrlattice (0x00,0x00);
    disphz (tab1);
    delay1 (300);
    wrlattice (0xff,0x00);
    delay1 (300);
    wrlattice (0x00,0x00);
    dispzf (tab2);
    delay1 (300);
    wrlattice (0x33,0x33);
    delay1 (300);
    disptu (tab3);
    delay1 (300);
  }
}
