#include "lcd_init.h"
#include "scopedLock.h"


LcdInit *LcdInit::m_pInstance = NULL;
MultiThreadMutex LcdInit::m_mutexOfInstance;


LcdInit *LcdInit::GetInstance()
{
    if(NULL == m_pInstance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
        if(NULL == m_pInstance)
        {
            m_pInstance = new LcdInit();
            if(DW_OK != m_pInstance->Init())
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    }
    
    return m_pInstance;
}

void LcdInit::Destroy()
{

    const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }


}




LcdInit::LcdInit()
{


}


LcdInit::~LcdInit()
{

    close(LcdFd);
    munmap(map , 800*480*4);

}


int  LcdInit::Init()
{
    // 打开显示器设别文件
    LcdFd = open("/dev/fb0" , O_RDWR );
    if (LcdFd == DW_ERR )
    {
        perror("open lcd error");
        return DW_ERR ;
    }
    
    // 内存映射
    map = static_cast<int*>(mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, LcdFd, 0));
    //int * map = mmap(NULL , 800*480*4 , PROT_READ | PROT_WRITE ,  MAP_SHARED , LcdFd , 0 );
    if (map == MAP_FAILED)
    {
        perror("mmap error");
        return -1 ;
    }

    
    return DW_OK;
}

int LcdInit::DisplayImages( const char * PahtName , int x_sta , int y_sta )
{
    // 打开图像文件
    FILE * fp = fopen(PahtName, "r+");
    if (fp == NULL )
    {
        fprintf( stderr , "open %s error :%s \n" , PahtName , strerror(errno));
        return -1 ;
    }
    

    // 读取图像的头部信息
    struct bitmap_header bmphead ;
    if( 1 != fread( &bmphead , sizeof( bmphead ) ,  1 , fp ))
    {
        perror("read bmp head error");
        return -1 ;
    }

    // 读取图像的信息头
    struct bitmap_info bmpinfo ;
    if( 1 != fread( &bmpinfo , sizeof( bmpinfo ) ,  1 , fp ))
    {
        perror("read bmp head error");
        return -1 ;
    }


    // 为所欲为
    printf("File size: %d \n" , bmphead.size );
    printf("Picture size: %d \n" , bmpinfo.size_img );
    printf("Picture width and height: %dx%d \n" , bmpinfo.width , bmpinfo.height  );
    printf("Picture color : %d \n" , bmpinfo.bit_count );


    // 计算得到BMP图像为了能被4整除而补齐的字节数
    int num = 0 ;
    if (((bmpinfo.width *3) % 4) != 0 )
    {
        num = 4 - ((bmpinfo.width *3) % 4) ;
    }
    

    // 读取图像数据
    int bgr_size =  (bmpinfo.width * 3 + num ) * bmpinfo.height  ;
    char buf_bmp [ bgr_size ] ;
    if(1 != fread( buf_bmp , bgr_size, 1 ,fp ))
    {
        perror ("read file error");
        return -1 ;
    }

    // 把读取到的 BUR 数据转换成 ARGB 
    int buf_lcd[ bmpinfo.height ][ bmpinfo.width ];

    for (int y = 0; y < bmpinfo.height ; y++)
    {
        for (int x = 0; x < bmpinfo.width ; x++)
        {
            buf_lcd[bmpinfo.height - y - 1] [x] =   buf_bmp [ (x+y*bmpinfo.width)*3 + 0 + y*num ] << 0 |    // 蓝色值
                                                    buf_bmp [ (x+y*bmpinfo.width)*3 + 1 + y*num ] << 8 |    // 绿色值
                                                    buf_bmp [ (x+y*bmpinfo.width)*3 + 2 + y*num ] << 16 |   // 红色值
                                                    0x00 << 24 ;            // 灰度值
        }
    }
    
    // 把转换后得到的 ARGB 显示到屏幕上
    for (int y = y_sta ; y < bmpinfo.height + y_sta  ; y++)
    {
        for (int x = x_sta  ; x < bmpinfo.width + x_sta ; x++)
        {
            if ( x < 0 ||  x > 799 || y < 0 ||  y > 479) // 边界检测 防止段错误出现
            {
                continue;
            }
            
            *(map + x + y * 800) = buf_lcd[y-y_sta] [x-x_sta] ;
        }
        
    }

    // 关闭文件    
    fclose(fp);

    return 0;


}



