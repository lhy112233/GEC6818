#include <Project.h>
int keynum1 = 0;                        //输入密码个数
int key1[7] = {0};                      //输入密码的储存
int truekey[7] = {0, 1, 1, 2, 2, 3, 3}; //正确密码
int panduan = 0;                        //最后的判断

int lock()
{
    int keyshow() //输入时的图片切换
    {
        switch (keynum1)
        {
        case 0:
            ShowBmp(0, 0, "./rec/lock0.bmp");
            break;

        case 1:
            ShowBmp(0, 0, "./rec/lock1.bmp");
            break;

        case 2:
            ShowBmp(0, 0, "./rec/lock2.bmp");
            break;

        case 3:
            ShowBmp(0, 0, "./rec/lock3.bmp");
            break;

        case 4:
            ShowBmp(0, 0, "./rec/lock4.bmp");
            break;

        case 5:
            ShowBmp(0, 0, "./rec/lock5.bmp");
            break;

        case 6:
            ShowBmp(0, 0, "./rec/lock6.bmp");
            break;

        default:
            break;
        }
    }

    while (1) //密码输入
    {
        Get_Xy();
        if (PI.Ts_x > 0 && PI.Ts_x < 265 && PI.Ts_y > 0 && PI.Ts_y < 120) //按钮0
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 0;
        }

        if (PI.Ts_x > 0 && PI.Ts_x < 265 && PI.Ts_y > 120 && PI.Ts_y < 240) //按钮1
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 1;
        }

        if (PI.Ts_x > 266 && PI.Ts_x < 535 && PI.Ts_y > 120 && PI.Ts_y < 240) //按钮2
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 2;
        }

        if (PI.Ts_x > 536 && PI.Ts_x < 800 && PI.Ts_y > 120 && PI.Ts_y < 240) //按钮3
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 3;
        }

        if (PI.Ts_x > 0 && PI.Ts_x < 265 && PI.Ts_y > 240 && PI.Ts_y < 360) //按钮4
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 4;
        }

        if (PI.Ts_x > 266 && PI.Ts_x < 535 && PI.Ts_y > 240 && PI.Ts_y < 360) //按钮5
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 5;
        }

        if (PI.Ts_x > 536 && PI.Ts_x < 800 && PI.Ts_y > 240 && PI.Ts_y < 360) //按6
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 6;
        }

        if (PI.Ts_x > 0 && PI.Ts_x < 265 && PI.Ts_y > 360 && PI.Ts_y < 480) //按钮7
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 7;
        }

        if (PI.Ts_x > 266 && PI.Ts_x < 535 && PI.Ts_y > 360 && PI.Ts_y < 480) //按钮8
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 8;
        }

        if (PI.Ts_x > 536 && PI.Ts_x < 800 && PI.Ts_y > 360 && PI.Ts_y < 480) //按钮9
        {
            printf("key1\n");
            keynum1++;
            keyshow();
            key1[keynum1] = 9;
        }

        if (keynum1 == 6) //退出条件
        {
            printf("密码达到6位\n");
            break;
        }
    }

    for (; panduan < 7; panduan++)//判断密码正确性
    {
        if (key1[panduan] != truekey[panduan])
        {
            break;
        }
    }

    if (panduan == 7)//锁的的判断输出
    {
        return 1;
    }
    else
        return 0;
}