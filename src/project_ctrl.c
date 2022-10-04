#include "Project.h"
int usleep(useconds_t usec);

int ProjectInit()
{
    //1.打开lcd、触摸屏
    PI.lcd_fd = open(_GEC_LCD_PATH_, O_RDWR);
    PI.ts_fd  = open(_GEC_TS_PATH_, O_RDONLY);
    if (PI.lcd_fd == PI.ts_fd == -1)
    {
        perror("open lcd or ts failed!");
        return -1;
    }

    //2.映射LCD内存
    PI.mmap_addr = mmap(NULL, _MMAP_LCD_SIZE_, 
                        PROT_READ|PROT_WRITE,
                        MAP_SHARED, PI.lcd_fd, 0);
    if (PI.mmap_addr == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    //3.初始化其他成员
    PI.Ts_x = 0;
    PI.Ts_y = 0;
    PI.pid1 = 0;
    PI.pid2 = 0;
    return 0;
}

int ProjectFree()
{
    //关闭文件、解除内存映射
   int close_ret1 = close(PI.lcd_fd);
   int close_ret2 = close(PI.ts_fd);
   int munmap_ret = munmap(PI.mmap_addr, _MMAP_LCD_SIZE_);
   if (close_ret1 == close_ret2 == munmap_ret == -1)
   {
       perror("ProjectFree failed");
       return -1;
   }
   
   return 0;
}

int Get_Xy()
{

   int flag = 0;//标志位
   memset(&Xy, 0, sizeof(Xy));
   while(1)
   {
       //读取触摸屏文件的数据
       read(PI.ts_fd, &Xy, sizeof(struct input_event));
     
       if(EV_ABS == Xy.type && Xy.code == ABS_X && flag == 0)//获取X轴坐标
       {
           PI.Ts_x = Xy.value *800/1024; 
       
           flag = 1;
       }

       if (Xy.type == EV_ABS && Xy.code == ABS_Y && flag == 1)//获取Y轴坐标
       {
           
           PI.Ts_y = Xy.value *480/600;
           flag = 2;
       }

       if (flag == 2)
       {
           //printf("坐标(%d,%d)\n", PI.Ts_x, PI.Ts_y);
           break;
       }
     
   }
    return 0;
}

int ShowBmp(int s_x, int s_y, const char *picName)
{
    //1.打开图片
    int bmp_fd = open(picName, O_RDONLY);
    if (bmp_fd == -1)
    {
        perror("open bmp failed!");
        return -1;
    }

    char head[54]; //存放54个字节的头信息（包含图片的宽度和高度）
                   //图片的宽度18-21字节，想要的得到int wide需要进行合并和移位操作
                   //图片的宽度22-25字节，想要的得到int height需要进行合并和移位操作
    memset(head, 0, sizeof(head));
    //2.读取图片的宽和高
    //读取头信息
    read(bmp_fd, head, 54);

    int wide = 0;//图片的宽度
    int height = 0;//图片的高度
    wide =   head[21] << 24 | head[20] << 16 |head[19] << 8 | head[18];
    height = head[25] << 24 | head[24] << 16| head[23] << 8 | head[22];
    printf("图片的宽度 %d  图片的高度%d\n", wide, height);
    //3.将图片的RGB（24位图片）转化成开发板 ARGB的像素（32图片）
    unsigned char buf[wide*height*3]; //存放图片像素
    memset(buf, 0, sizeof(buf));
    read(bmp_fd, buf, sizeof(buf));
    unsigned char A, R, G, B; // 存放图片像素点的A，R, G, B数据
    int x = 0, y = 0, n = 0;
    unsigned int bmp_buf[wide*height]; //存放转化后的像素点
    unsigned int tmp;

    for(n = 0; n < wide*height; n++)
    {
        A = 0x00;  //透明度
        B = buf[3*n]; //蓝色
        G = buf[3*n+1]; //绿色
        R = buf[3*n+2]; //红色

        // A R G B 逐个的字节 合并成 ARGB int型的像素点
        bmp_buf[n] = A << 24 | R << 16 | G << 8 | B;
    }
    //4.实现反转(关于图片高度的一半进行交换)
    for (y = 0; y < height/2; y++)
    {
        for (x = 0; x < wide; x++)
        {
            //将第x行, 第y列像素点和 第height -y行，第x列的像素点进行交换
            tmp = bmp_buf[wide *y +x];
            bmp_buf[wide*y +x] = bmp_buf[wide*(height-y) +x];
            bmp_buf[wide*(height-y) +x] = tmp;
        }
    }
    //5.将转化后的像素写入到映射内存中（赋值）
    for (y = s_y; y < height+s_y && y < 480; y++)
    {
        for (x = s_x; x < wide + s_x && x < 800; x++)
        {
            //LC显示图片的第一个像素点            图片的第一个像素点
            PI.mmap_addr[800*y+x] = bmp_buf[wide*(y - s_y) + (x - s_x)];
        }
    }
    //6.关闭图片
    close(bmp_fd);
    return 0;
}

void* LoadingPic() //加载进度条的图片
{
     while(1)
    {
        ShowBmp(500, 250, "./rec/lod1.bmp");
        usleep(200000);
		ShowBmp(500, 250, "./rec/lod2.bmp");
        usleep(200000);
        ShowBmp(500, 250, "./rec/lod3.bmp");
        usleep(200000);
    }

    return NULL;
}

int Loadig() //加载进度条
{
    int x,y;
    for(y=0; y<480; y++)
    {
      for(x=0; x<800; x++)
      {
        *(PI.mmap_addr+800*y+x) = 0x00ffffff;
      }
    }

    int * new_p = PI.mmap_addr;

    pthread_t load_id;

    int ret = pthread_create(&load_id,NULL,LoadingPic,NULL);
    if(ret !=0)printf("create pthread for loading_pic failed\n");

    for(y=150; y<201; y++)
    {
        for(x=160; x<641;x++)
        {
            if(x==160) *(new_p+800*y+x) = 0x00000000;
            if(x==640) *(new_p+800*y+x) = 0x00000000;
            if(y==150) *(new_p+800*y+x) = 0x00000000;
            if(y==200) *(new_p+800*y+x) = 0x00000000;
        }
    }

    for(x=161; x<640; x++)
    {
        for(y=151; y<200; y++)
        {
            *(new_p+800*y+x) = 0x0000ff00;

        }
        usleep(2000);


    }

    sleep(1);
    ret = pthread_cancel(load_id);
    if(ret != 0)printf("cancel loading_pic pailed\n");
    return 0;
}



int ShowBmpslp(int s_x, int s_y, const char *picName) //渐进打开图片
{
    //1.打开图片
    int bmp_fd = open(picName, O_RDONLY);
    if (bmp_fd == -1)
    {
        perror("open bmp failed!");
        return -1;
    }

    char head[54]; //存放54个字节的头信息（包含图片的宽度和高度）
                   //图片的宽度18-21字节，想要的得到int wide需要进行合并和移位操作
                   //图片的宽度22-25字节，想要的得到int height需要进行合并和移位操作
    memset(head, 0, sizeof(head));
    //2.读取图片的宽和高
    //读取头信息
    read(bmp_fd, head, 54);

    int wide = 0;//图片的宽度
    int height = 0;//图片的高度
    wide =   head[21] << 24 | head[20] << 16 |head[19] << 8 | head[18];
    height = head[25] << 24 | head[24] << 16| head[23] << 8 | head[22];
    printf("图片的宽度 %d  图片的高度%d\n", wide, height);
    //3.将图片的RGB（24位图片）转化成开发板 ARGB的像素（32图片）
    unsigned char buf[wide*height*3]; //存放图片像素
    memset(buf, 0, sizeof(buf));
    read(bmp_fd, buf, sizeof(buf));
    unsigned char A, R, G, B; // 存放图片像素点的A，R, G, B数据
    int x = 0, y = 0, n = 0;
    unsigned int bmp_buf[wide*height]; //存放转化后的像素点
    unsigned int tmp;

    for(n = 0; n < wide*height; n++)
    {
        A = 0x00;  //透明度
        B = buf[3*n]; //蓝色
        G = buf[3*n+1]; //绿色
        R = buf[3*n+2]; //红色

        // A R G B 逐个的字节 合并成 ARGB int型的像素点
        bmp_buf[n] = A << 24 | R << 16 | G << 8 | B;
    }
    //4.实现反转(关于图片高度的一半进行交换)
    for (y = 0; y < height/2; y++)
    {
        for (x = 0; x < wide; x++)
        {
            //将第x行, 第y列像素点和 第height -y行，第x列的像素点进行交换
            tmp = bmp_buf[wide *y +x];
            bmp_buf[wide*y +x] = bmp_buf[wide*(height-y) +x];
            bmp_buf[wide*(height-y) +x] = tmp;
        }
    }
    //5.将转化后的像素写入到映射内存中（赋值）
    for (y = s_y; y < height+s_y && y < 480; y++)
    {
        for (x = s_x; x < wide + s_x && x < 800; x++)
        {
            //LC显示图片的第一个像素点            图片的第一个像素点
            PI.mmap_addr[800*y+x] = bmp_buf[wide*(y - s_y) + (x - s_x)];
        }
        usleep(2000);
    }
    //6.关闭图片
    close(bmp_fd);
    return 0;
}
