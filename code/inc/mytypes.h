#ifndef _MY_TYPES_H
#define _MY_TYPES_H

#include "user.h"






typedef short int16_t;
typedef int int32_t;


/* typedef struct Coordinates
{
    int XAxis;
    int YAxis;

}Abscissa , *P_Abscissa; */



typedef struct  data
{
    char PathName[257];
    char Type ;

}dataType ;


typedef struct node
{
    //数据域
    dataType data ;

    //指针域
    struct node *prev ;
    struct node *next ;   


}Node , *P_Node;




// BMP 文件头  14 字节
struct bitmap_header
{
	int16_t type; // 图像类型  BM  2 字节
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));


// 位图 信息头  40 字节
struct bitmap_info
{
	int32_t size;   // 本结构大小	
	int32_t width;  // 图像宽
	int32_t height; // 图像高
	int16_t planes;

	int16_t bit_count; // 色深  一个像素点占用多少个位
	int32_t compression;
	int32_t size_img; // bmp数据大小，必须是4的整数倍
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed));









#endif