#include "user.h"
#include "scopedLock.h"


User *User::m_pInstance = NULL;
MultiThreadMutex User::m_mutexOfInstance;

User *User::GetInstance()
{
    if(NULL == m_pInstance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
        if(NULL == m_pInstance)
        {
            m_pInstance = new User();
            if(DW_OK != m_pInstance->Init())
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    }
    
    return m_pInstance;
}

void User::Destroy()
{
    const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }

}

User::User()
{

}

User::~User()
{

}


int User::Init()
{

	return DW_OK;

}

void User::DisplayList(P_Node head )
{
    if (NULL == head || NULL == head->next )
    {
        printf("链表头部异常或为空 。\n");
        return ;
    }

    for(P_Node tmp = head->next ; tmp != head ;tmp = tmp->next)
    {
        printf("(user.c)当前文件路名：%s 类型：%c \n" , tmp->data.PathName , tmp->data.Type );

    }

    
    return ;



}

P_Node User::NewNode(dataType *newdata)
{
    P_Node new_p = static_cast<P_Node>(calloc(1, sizeof(Node)));  // 显式类型转换

    if(NULL != newdata )
    {   
        memcpy(&new_p->data,newdata,sizeof(dataType));
    }

    new_p->next=new_p->prev= new_p ;

    return new_p ;

}

void User::InsList(P_Node Prev ,P_Node new_p,P_Node Next)
{
     Prev->next = new_p;
     new_p->prev = Prev ;

     new_p->next = Next ;
     Next->prev = new_p ;

     return ;

    
}


int User::SearchForFiles(P_Node head ,const char*SearchTypes, const char*SearchPath)
{

    int count_Num = 0 ;
    //打开指定的目录
    DIR *dp = opendir(SearchPath);
    if(NULL == dp)
    {
         fprintf(stderr,"open dir %s error : %s \n",SearchPath,strerror(errno));
       
        return 0 ;
    }
    
    while (1)
    {
        //读取目录文件 readdir 成功返回下一个目录的指针，失败返回NULL
        struct dirent* dirinfo = readdir(dp);
        if(NULL ==  dirinfo)
        {
            fprintf(stderr,"(user.c)read dir %s error : %s \n",SearchPath,strerror(errno));
            break ; 
        }

        //过滤隐藏文件 
        if ('.' ==  dirinfo->d_name[0])
        {
            continue;
        }
        
        //如果是目录文件 需要深度搜索
        if (dirinfo->d_type == 4)
        {
            //重新整合路径加文件名
            char path[257] = {0} ;
            snprintf(path ,257 ,"%s/%s" ,SearchTypes,dirinfo->d_name) ;
            count_Num += SearchForFiles( head ,SearchTypes , path );

        }

        // 如果不是目录也不是隐藏文件则检查是否为指定类型 文件  SearchTypes 
        // asdf.jpg.mp3
        char *ptr = strrchr(dirinfo->d_name , '.');
        if (NULL == ptr)
        {
            continue;
        }

        if( ! strcmp( ptr , SearchTypes))
        {

            //把文件路径和名字存到链表中

            dataType newData ;
            snprintf(newData.PathName , 257 , "%s/%s",SearchPath ,dirinfo->d_name );
            newData.Type = SearchTypes[1] ;

            
            //使用该数据创建一个新节点
            P_Node new_p = NewNode( &newData ) ;
            //把新节点插入到链表中
            InsList(head ,new_p,head->next);

            //计数值 ++
            count_Num ++;


        }


    }
  return count_Num ;  

}


