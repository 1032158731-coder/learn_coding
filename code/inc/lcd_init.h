#ifndef __LCD_INIT__
#define __LCD_INIT__

#include <fcntl.h>  
#include <sys/mman.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>

#include "user.h"
#include "public.h"
#include "multithreadmutex.h"
#include "mytypes.h"


class LcdInit
{
    
public:
    
    static LcdInit *GetInstance();
    static void Destroy();   
    
    int DisplayImages( const char * PahtName , int x_sta , int y_sta );             //显示照片


private:

    static  LcdInit *m_pInstance ;
    static MultiThreadMutex m_mutexOfInstance;

    int Init();                                                                     //初始化屏幕

    LcdInit();
    ~LcdInit();

private:
    UInt32 LcdFd ;
    int *map;


};




#endif


