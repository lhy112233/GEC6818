#include "Project.h"

int StartMusic() //启动音乐
{
    int musicnum = 0;
    int musicnext();

    //显示音乐界面
    ShowBmp(0, 0, "./rec/music.bmp");

    while (1)
    {
        Get_Xy();                                                         //获取触摸屏的坐标
        if (PI.Ts_x > 0 && PI.Ts_x < 248 && PI.Ts_y > 0 && PI.Ts_y < 238) //播放按钮
        {
            printf("播放按钮！\n");
            system("madplay ./music/faded.mp3 &");
        }

        if (PI.Ts_x > 550 && PI.Ts_x < 800 && PI.Ts_y > 241 && PI.Ts_y < 480) //下一首按钮
        {
            printf("下一首按钮！\n");

            musicnum=musicnext(musicnum);
        }

        if (PI.Ts_x > 253 && PI.Ts_x < 545 && PI.Ts_y > 0 && PI.Ts_y < 238) //暂停按钮
        {
            printf("暂停按钮！\n");
            system("killall -19 madplay");
        }

        if (PI.Ts_x > 550 && PI.Ts_x < 800 && PI.Ts_y > 0 && PI.Ts_y < 240) //退出按钮
        {
            printf("退出按钮！\n");
            ShowBmp(0, 0, "./rec/main.bmp"); //显示主界面

            PI.Ts_x = -1;
            PI.Ts_y = -1;
            break;
        }

        if (PI.Ts_x > 0 && PI.Ts_x < 248 && PI.Ts_y > 245 && PI.Ts_y < 480) //停止按钮
        {
            printf("停止按钮！\n");
            system("killall -9 madplay");
        }

        if (PI.Ts_x > 255 && PI.Ts_x < 550 && PI.Ts_y > 245 && PI.Ts_y < 480) //继续按钮
        {
            printf("继续按钮！\n");
            system("killall -18 madplay");
        }
    }

}

int musicnext(int musicnum)
    {
        musicnum++;
        musicnum = musicnum % 4;
        system("killall -9 madplay");
        if (musicnum == 0)
        {
            system("madplay ./music/faded.mp3 &");
        }

        if (musicnum == 1)
        {
            system("madplay ./music/Mojito.mp3 &");
        }

        if (musicnum == 2)
        {
            system("madplay ./music/music3.mp3 &");
        }

        if (musicnum == 3)
        {
            system("madplay ./music/music4.mp3 &");
        }
        return musicnum;
    }
