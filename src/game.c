#include "Project.h"

static int stopFlag; //0表示暂停、 1：开始
static int isExit; //判断游戏是否是输了，是否需要退出： 0：没有输，退出。1：表示游戏输了，要退出
static int isReset; //0：表示不重新开始， 1：表示重新开始

static char *score_buf[] = {
    "./rec/0.bmp", "./rec/1.bmp", "./rec/3.bmp", "./rec/4.bmp",
    "./rec/5.bmp", "./rec/6.bmp", "./rec/7.bmp", "./rec/8.bmp",
    "./rec/9.bmp"
};

int GameInit() //游戏模块的初始化
{

    ShowBmp(0, 0, "./rec/p_c.bmp"); //游戏界面
    //创建线程
    int ret = pthread_create(&PI.pid1, NULL, MoveBall, NULL);
    if (ret != 0)
    {
        perror("创建线程1失败！\n");
        return -1;
    }
    ret = pthread_create(&PI.pid2, NULL, TouchControlPlate, NULL);
    if (ret != 0)
    {
        perror("创建线程2失败！");
        return -1;
    }
    
    isExit = 0;
    stopFlag = 0;
    isReset = 0;
    return 0;
}


int GameFree() //游戏结束
{
    ShowBmp(0, 0, "./rec/main.bmp");
    return 0;
}

int CountGameScore(int score) //积分器
{
   int i = 0;
    while (1)
    {
       if (score /10 != 0)
       {
           ShowBmp(767-(i*32), 10, score_buf[score % 10]);
           score /= 10;
       }
       else
       {
           ShowBmp(767-(i*32), 10, score_buf[score % 10]);
           break;
       }

       i++;
    }
    
    return 0;

}

void* MoveBall(void *arg)//画球
{
     //定义圆的基本信息
    int score = 0;
    int x0 = 400, y0 = 240; //圆心
    int r = 50;             //半径
    int x_mask = 0, y_mask = 0; //0 表示-- 1表示++
    int x, y;

    while (1)
    {
        for (y = y0 - r; y <= y0 + r; y++)
        {
            for (x = x0 -r; x <= x0 + r; x++)
            {
                //判断是否是球的位置
                if ((x - x0) * (x - x0) + (y - y0) * (y - y0) < r * r)
                {
                    PI.mmap_addr[800 * y + x] = 0x0000ff; //蓝色像素点
                }
                else
                {
                    PI.mmap_addr[800 * y + x] = 0xffffff; //白色像素点
                }
            }
        }
        
        usleep(3000); //延时
        
        while (1) //游戏开始
        {
            if (stopFlag == 1 || isReset == 1)
            {
                break;
            }
        }

        if (isReset == 1)
        {
            x0 = 400;
            y0 = 240;
            x_mask = 0;
            y_mask = 0;

            stopFlag = 0;
            isReset = 0;
            score = 0;
            ShowBmp(0, 0, "./rec/p_c.bmp");//显示游戏界面
        }

        //改变圆心的位置
        if (y0 - r == 0)   y_mask = 1;  //判断是否碰到上方
        if (x0 - r == 0)   x_mask = 1;  //判断是否碰到左边

        if (y0 + r == 399 && x0 > PI.Ts_x - 50 && x0 < PI.Ts_x + 50)
        {
            y_mask = 0; //判断是否能接到球
            score ++;
            CountGameScore(score);
        }

        if (y0 + r >= 410) isExit = 1; //没有接到球，意味着游戏结束了

        if (x0 + r == 696) x_mask = 0;  //判断是否碰到右边

        if (y_mask == 1)   y0++;
        if (y_mask == 0)   y0--;
        if (x_mask == 1)   x0++;
        if (x_mask == 0)   x0--;

    }

    return (void *)0;

}

int DrawPlate()//画板
{
    int x, y;
    for(y = 400; y < 430; y++)
    {
        for(x = 0; x < 696; x++)
        {
            if (x > PI.Ts_x - 50 && x < PI.Ts_x + 50)
            {
                PI.mmap_addr[800*y+x] = 0x0000ff;
            }
            else //大木板，清除轨迹
            {
                PI.mmap_addr[800*y+x] = 0xffffff;
            }
        }
    }
    return 0;
}

void* TouchControlPlate(void *arg) //控制木板
{
   
   while(1)
   {
        Get_Xy(); //获取坐标
        if (PI.Ts_y > 400 && PI.Ts_y < 430) DrawPlate(PI.Ts_x); //画板

        if (PI.Ts_x > 700 && PI.Ts_x < 800 && PI.Ts_y > 45 && PI.Ts_y < 147) //开始/暂停按钮
        {
            printf("开始/暂停按钮!\n");
            if (stopFlag == 1)
            {
                stopFlag = 0;
            }
            else
            {
                stopFlag = 1;
            }
            
        }

        if (PI.Ts_x > 700 && PI.Ts_x < 800 && PI.Ts_y > 150 && PI.Ts_y < 300) //重新开始按钮
        {
            printf("重新开始按钮!\n");
            isReset = 1;
            
        }

        if (PI.Ts_x > 700 && PI.Ts_x < 800 && PI.Ts_y > 302 && PI.Ts_y < 480) //退出按钮
        {
            printf("退出按钮!\n");
            isExit = 1;
            break;
        }
     
   }
}

int StartGame() //启动游戏模块的
{

    GameInit(); //初始化
    while(1)
    {
        if (isExit == 1)
        {
            pthread_cancel(PI.pid1); //取消两条线程
            pthread_cancel(PI.pid2); 

            ShowBmp(0, 0, "./rec/p_c.bmp");
            ShowBmp(250, 168, "./rec/GV.bmp");
            sleep(3); //延时3秒
            break;
        }

    }
    GameFree(); //回收游戏资源

    return 0;
}