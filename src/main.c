#include "Project.h"

int ControlMianUI() //控制模块
{
    ShowBmp(0, 0, "./rec/lock0.bmp");

    int keysign=lock();

    if(keysign==1)
    {
    ShowBmp(0, 0, "./rec/main.bmp"); //显示主界面

    //启动触摸屏
    while(1)
    {
        Get_Xy();
        if (PI.Ts_x > 25 && PI.Ts_x < 225 && PI.Ts_y > 300 && PI.Ts_y < 440) //视频按钮
        {
            printf("视频按钮!\n");
            StartVideo(); //启动视频
        }

        if (PI.Ts_x > 300 && PI.Ts_x < 475 && PI.Ts_y > 300 && PI.Ts_y < 440) //视频按钮
        {
            printf("音乐按钮！\n");
            StartMusic();//启动音乐
        }
        
        if (PI.Ts_x > 585 && PI.Ts_x < 770 && PI.Ts_y > 300 && PI.Ts_y < 440) //游戏按钮
        {
            printf("游戏按钮！\n");
            StartGame(); //启动游戏
        }

        if (PI.Ts_x > 25 && PI.Ts_x < 225 && PI.Ts_y > 40 && PI.Ts_y < 180) //视频按钮
        {
            printf("相册!\n");
            StartAlbum(); //启动相册
        }

        if (PI.Ts_x > 340 && PI.Ts_x < 440 && PI.Ts_y > 180 && PI.Ts_y < 277) //退出按钮
        {
            printf("主界面退出按钮！\n");

            ShowBmp(0, 0, "./rec/exit.bmp"); //显示退出界面
            break;
        }
    }

    }
    else{
        printf("主界面退出按钮！\n");

            ShowBmp(0, 0, "./rec/exit.bmp"); //显示退出界面
            
    }

}

int main()
{
    
    
    //初始化
    int ProjectInit_ret =  ProjectInit();
    if (ProjectInit_ret == -1)
    {
        printf("初始化失败！\n");
        return -1;
    }
    else
    {
        printf("初始化成功！\n");
    }

    ControlMianUI(); //启动程序
    //释放资源
    int ProjectFree_ret = ProjectFree();
    if (ProjectFree_ret == -1)
    {
        printf("资源回收失败！\n");
        return -1;
    }
    else
    {
        printf("资源回收成功！\n");
    }
    return 0;
}



//arm-linux-gcc ./src/*c -I./include -o project -lpthread