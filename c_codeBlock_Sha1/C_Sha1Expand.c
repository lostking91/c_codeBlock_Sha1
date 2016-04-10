/* sha-1算法扩散性能的逐拍码字变动统计 */
#include<stdio.h>
#include <string.h>
#include<math.h>
#include<stdlib.h>
// #define i_num 80        //迭代操作的次数
#define r_num 1             //循环次数
#define tr_i_num 512         //修改遍历
char ran_input(unsigned char intput[64],int inputlength)
{
    int randi=0;
    int randinput=(rand()%256)+1;
    for(;randi<inputlength;randi++)
    {
        intput[randi]=randinput;
    }
}

void creat_w(unsigned char input[64],unsigned long w[80])       //数据填充函数
{
   int i,j;unsigned long temp,temp1;
   for(i=0;i<16;i++)    //i<16，w[i]实际上为4个char类型组成的long类型。所以input[64]全放入了w[15]以内
          {
             j=4*i;
             w[i]=((long)input[j])<<24 |((long)input[1+j])<<16|((long)input[2+j])<<8|((long)input[3+j])<<0;
            //如果input[j]是char类型，则为(char)0x??,变成(long)0x000000??
            //0x000000??-> 0x??000000,同理 0x??000000|0x00??0000|0x0000??00|0x000000??
          }
   for(i=16;i<80;i++)      //给w[16]~w[80]补值:先循环异或得出w[16]~w[80],再进行位的循环左移一位
         {
             w[i]=w[i-16]^w[i-14]^w[i-8]^w[i-3];
             temp=w[i]<<1;      //去除最高位，左移一位，末位补零
             temp1=w[i]>>31;    //末位设为原最高位,前31位补零
             w[i]=temp|temp1;

         }

}
void creat_errw(unsigned char input[64],unsigned long errw[80],int input_num,int input_bit)       //指定位置篡改
{
   int i,j,k;unsigned long temp,temp1;unsigned char inputtemp[64];
   for(k=0;k<64;k++)
   {
       inputtemp[k]=input[k];
   }
   inputtemp[input_num]=inputtemp[input_num] ^ (1<<(input_bit-1));
   for(i=0;i<16;i++)    //i<16，w[i]实际上为4个char类型组成的long类型。所以input[64]全放入了w[15]以内
          {
             j=4*i;
             errw[i]=((long)inputtemp[j])<<24 |((long)inputtemp[1+j])<<16|((long)inputtemp[2+j])<<8|((long)inputtemp[3+j])<<0;
            //如果input[j]是char类型，则为(char)0x??,变成(long)0x000000??
            //0x000000??-> 0x??000000,同理 0x??000000|0x00??0000|0x0000??00|0x000000??
          }
   for(i=16;i<80;i++)      //给w[16]~w[80]补值:先循环异或得出w[16]~w[80],再进行位的循环左移一位
         {
             errw[i]=errw[i-16]^errw[i-14]^errw[i-8]^errw[i-3];
             temp=errw[i]<<1;      //去除最高位，左移一位，末位补零
             temp1=errw[i]>>31;    //末位设为原最高位,前31位补零
             errw[i]=temp|temp1;

         }

}
char ms_len(long a,char intput[64])     // input的60，61，62，63进行填充input[64]='\0';
{
    unsigned long temp3,p1;  int i,j;
    temp3=0;            // 0x00000000
    p1=~(~temp3<<8);    // 0x00000000-> 0xffffffff-> 0xffffff00-> 0x000000ff
    for(i=0;i<4;i++)
       {
          j=8*i;
          intput[63-i]=(char)((a&(p1<<j))>>j);  //假设input为iscbupt，a=56，转化成ascii，因为有char的限制 输入只能为0x7f以下

       }

}
char input_init(char input[64],int n)     //用来初始化输入，对数组进行填充
{
   int i;long x;
   if(n<57)           //n<57,先对60,61,62,63进行填充。n=56,56~59全赋值为0。n<56,input[n]=128,n+1后全赋值为0
          {
                 x=n*8;
                 ms_len(x,input);
                 if(n==56)
                     for(i=n;i<60;i++)
                     input[i]=0;
                 else                 //在数据末位增加128，其他位赋0
                    {
                     input[n]=128;
                     for(i=n+1;i<60;i++)
                     input[i]=0;
                    }

          }
}

int bit_expand(unsigned long hashtrue[80],unsigned long hashfalse[80],int n,char expand[512][2560],int compute)            //求dc,返回dc矩阵
{
    int i,j;
    for(i=0;i<80;i++)
    {
        unsigned long temp=0x000000001;
        for(j=1;j<=32;j++)
        {
           if((hashtrue[i]&temp)!=(hashfalse[i]&temp))       //改变的个数+1
            {
                expand[n-1][i*32+j-1]=1;
                compute++;
            }else
            {
                expand[n-1][i*32+j-1]=0;
            }
            temp=temp<<1;
        }
    }
    return compute;
}

main()
{
   unsigned long temp,temp1,temp2,temp3,k,f;int i,flag;unsigned long w[80],errw[80];
   int input_len,char_len;int testi;unsigned char input[64];
   int traverse_i,traverse_num,traverse_bit;
   int repeat_num;int inputlength;
   int compute=0;

   repeat_num=r_num;
   srand(time(NULL));		//time是一个函数，获取时间保存结果于ts中

   char sha1_expand[512][2560];

   for(;repeat_num>0;repeat_num--)
    {
        inputlength=rand()%57;
        ran_input(input,inputlength);
        //printf("input message:\n");
        //scanf("%s",input);
        input_len=strlen(input);   //n为输入明文总长度
        char_len=sizeof(char)*8;
        //printf("%d\n",char_len);
        input_init(input,input_len);
        creat_w(input,w);

        for(traverse_i=1;traverse_i<=tr_i_num ;traverse_i++)
        {
            traverse_num=(traverse_i-1)/8;
            traverse_bit=(traverse_i-1)%8+1;
            creat_errw(input,errw,traverse_num,traverse_bit);   //错误输出hash
            compute=bit_expand(w,errw,traverse_i,sha1_expand,compute);

            unsigned long errw[80]={0};
            int showj=0;
 //           for(;showj<2560;showj++){
 //               printf("%d,",sha1_expand[traverse_i-1][showj]);
 //               //printf(":%d\t%d\t",traverse_i-1,showj);
 //           }
            printf("???????? ");
        }
    }
    //显示区域

    printf("hello world!!!\n");
    printf("%d",compute);
    /*
    int showi=0;int showj=0;
    for(;showi<512;showi++){
        for(;showj<2560;showj++){
            printf("%d,",sha1_expand[showi][showj]);
        }
        printf("! ");
    }
    printf("Good night!!!\n");
    */
    getch();
}
