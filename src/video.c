#include "Project.h"

static int fd_fifo; //管道文件



int  VideoInit() //初始化
{
    //判断是否存在管道文件， 不存在才创建
    if (access("/fifo", F_OK)) //默认管道文件创建在根目录 F_OK 判断是否存在
    {
        //如果条件成立，不存在
        mkfifo("/fifo", 777);//创建管道文件
    }

    fd_fifo = open("/fifo", O_RDWR);
    if (fd_fifo == -1)
    {
        printf("打开管道文件失败！\n");
        return -1;
    }

    return 0;
}




int  VideoFree() //资源回收
{
    system("killall -9 mplayer");
    ShowBmp(0, 0, "./rec/main.bmp"); //显示主界面
    PI.Ts_x = -1;
    PI.Ts_y = -1;

    return 0;
}

int  SendCmd(char *cmd)//发送命令
{
    write(fd_fifo, cmd, strlen(cmd));
    return 0;
}

int nextvideo(int nextnum){
    if (nextnum<0)
    {
        nextnum=2;
    }
    
    nextnum=nextnum%3;
    system("killall -9 mplayer");
    if (nextnum==0)
    {
        system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/Faded3.avi &");
    }
    
    else if (nextnum==1)
    {
        system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/video2.avi &");
    }
    
    else if (nextnum==2)
    {
        system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/video3.avi &");
    }
    return nextnum;
}



int StartVideo() //启动视频
{
    int nextnum=0;
    ShowBmp(0, 0, "./rec/video.bmp"); //显示视频界面
    VideoInit(); //初始化
    
    system("mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 450 ./video/Faded3.avi &");
    while(1)
    {
        Get_Xy(); //获取坐标
        if (PI.Ts_x > 0 && PI.Ts_x < 43 && PI.Ts_y > 400 && PI.Ts_y < 480) //+按钮
        {
            printf("+按钮！\n");
            SendCmd("volume +10\n");
        }

        if (PI.Ts_x > 46 && PI.Ts_x < 95 && PI.Ts_y > 400 && PI.Ts_y < 480) //-按钮
        {
            printf("-按钮！\n");
            SendCmd("volume -10\n");
        }

        if (PI.Ts_x > 155 && PI.Ts_x < 251 && PI.Ts_y > 400 && PI.Ts_y < 480) //快退按钮
        {
            printf("快退按钮！\n");
            SendCmd("seek -10\n");
        }

        if (PI.Ts_x > 355 && PI.Ts_x < 451 && PI.Ts_y > 400 && PI.Ts_y < 480) //暂停/播放按钮
        {
            printf("暂停/播放按钮！\n");
            SendCmd("pause\n");
        }

        if (PI.Ts_x > 551 && PI.Ts_x < 646 && PI.Ts_y > 400 && PI.Ts_y < 480) //快进按钮
        {
            printf("快进按钮！\n");
            SendCmd("seek +10\n");
        }


        if (PI.Ts_x > 451 && PI.Ts_x < 551 && PI.Ts_y > 400 && PI.Ts_y < 480) //下一集按钮
        {
            nextnum++;
            nextnum=nextvideo(nextnum);
        }

        if (PI.Ts_x > 251 && PI.Ts_x < 355 && PI.Ts_y > 400 && PI.Ts_y < 480) //下一集按钮
        {
            nextnum--;
            nextnum=nextvideo(nextnum);
        }

        if (PI.Ts_x > 708 && PI.Ts_x < 800 && PI.Ts_y > 400 && PI.Ts_y < 480) //退出按钮
        {
            printf("退出按钮！\n");
            break;
        }
    }

    VideoFree();
    return 0;
}