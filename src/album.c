#include "Project.h"

int StartAlbum() //启动相册
{
    int next=0;
    //显示相册界面
    ShowBmp(0, 0, "./rec/photo4.bmp");
    while(1)
    {
        Get_Xy();//获取触摸屏的坐标

        if (PI.Ts_x > 0 && PI.Ts_x < 267 && PI.Ts_y > 0 && PI.Ts_y < 240) //手动下一张
        {
            next++;
            next=next%4;
            printf("下一张！\n");
            if(next==2)
            {ShowBmp(0, 0, "./rec/photo2.bmp");}
            
            else if (next==3)
            {
            ShowBmp(0, 0, "./rec/photo3.bmp");}
            
            else if(next==1){
            ShowBmp(0, 0, "./rec/photo1.bmp");}
            
            else if(next==0){
            ShowBmp(0, 0, "./rec/photo4.bmp");}
        }

        if (PI.Ts_x > 267 && PI.Ts_x < 533 && PI.Ts_y > 0 && PI.Ts_y < 240) //自动下一张
        {
            for(int photosize=0;photosize<4;photosize++)
            {
            next++;
            next=next%4;
            printf("下一张！\n");
            if(next==2)
            {ShowBmp(0, 0, "./rec/photo2.bmp");}
            
            else if (next==3)
            {
            ShowBmp(0, 0, "./rec/photo3.bmp");}
            
            else if(next==1){
            ShowBmp(0, 0, "./rec/photo1.bmp");}
            sleep(2);
            }
            ShowBmp(0, 0, "./rec/photo4.bmp");
        }
        
        
        if (PI.Ts_x > 0 && PI.Ts_x < 267 && PI.Ts_y > 240 && PI.Ts_y < 480) //手动下一张渐进
        {
            next++;
            next=next%4;
            printf("下一张！\n");
            if(next==2)
            {ShowBmpslp(0, 0, "./rec/photo2.bmp");}
            
            else if (next==3)
            {
            ShowBmpslp(0, 0, "./rec/photo3.bmp");}
            
            else if(next==1){
            ShowBmpslp(0, 0, "./rec/photo1.bmp");}

            else if(next==0){
            ShowBmpslp(0, 0, "./rec/photo4.bmp");}
        }

         if (PI.Ts_x > 267 && PI.Ts_x < 533 && PI.Ts_y > 240 && PI.Ts_y < 480) //自动下一张渐进
        {
            for(int photosize=0;photosize<4;photosize++)
            {
            next++;
            next=next%4;
            printf("下一张！\n");
            if(next==2)
            {ShowBmpslp(0, 0, "./rec/photo2.bmp");}
            
            else if (next==3)
            {
            ShowBmp(0, 0, "./rec/photo3.bmp");}
            
            else if(next==1){
            ShowBmpslp(0, 0, "./rec/photo1.bmp");}
            sleep(2);
            }
            ShowBmpslp(0, 0, "./rec/photo4.bmp");
        }
        

        if (PI.Ts_x > 533 && PI.Ts_x < 800 && PI.Ts_y > 0 && PI.Ts_y < 480) //退出按钮
        {
            printf("退出按钮！\n");
            ShowBmp(0, 0, "./rec/main.bmp"); //显示主界面

            PI.Ts_x = -1;
            PI.Ts_y = -1;
            break;
        }

        

    }
}