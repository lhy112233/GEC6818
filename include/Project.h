#ifndef _PROJECT_H_
#define _PROJECT_H_ //防止头文件冲突

/*存放头文件*/
#include <stdio.h>   //printf函数、scanf函数
#include <error.h>   //标准出错头文件
#include <string.h>  //memset函数、strlen函数的头文件
#include <stdlib.h>
#include <unistd.h>   //read、write函数,sleep的
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h> //mmap函数
#include <linux/input.h> //Linux下的输入子系
#include <pthread.h>  //线程API接口头文件
#include <unistd.h>

// 宏和全局变量结构体
#define _GEC_LCD_PATH_     "/dev/fb0"              //lcd屏的路径
#define _GEC_TS_PATH_   "/dev/input/event0"      //触摸屏文件的路径
#define _MMAP_LCD_SIZE_     800*480*4              //映射地址大小



struct Pro_Info
{
    int lcd_fd; /*LCD屏*/
    int ts_fd;  /*触摸屏*/
    int *mmap_addr; /*映射内存的首地址*/
    int Ts_x,Ts_y; /*触摸屏的坐标*/
    pthread_t pid1,pid2; /*线程ID号*/
    
}PI;
struct input_event Xy; //存放放触摸屏数据的结构体

int ProjectInit();

int ProjectFree();

int Get_Xy(); //获取触摸屏坐标


int ShowBmp(int s_x, int s_y, const char *picName);


int ControlMianUI(); //控制模块


void* LoadingPic();

int Loadig();


int GameInit(); //游戏模块的初始化


int GameFree(); //游戏结束


int CountGameScore(int score); //积分器


void* MoveBall(void *arg); //画球


int DrawPlate();//画板


int StartGame(); //启动游戏模块的



void* TouchControlPlate(void *arg); //控制木板

int StartMusic();



int StartVideo();


int  VideoInit();


int  VideoFree();

int  SendCmd(char *cmd);


int StartAlbum(); //启动相册

int lock();

int musicnext(int musicnum);

int nextvideo();

int ShowBmpslp(int s_x, int s_y, const char *picName);

#endif