/* sha-1算法主动输入测试 */
#include<stdio.h>
#include <string.h>
#include<math.h>
#include<stdlib.h>
#define i_num 1        //迭代操作的次数
#define r_num 700  //循环次数
//ran_input(unsigned char intput[64],int inputlength);
int cu_sum(unsigned long hashtrue,unsigned long hashfalse)            //求累加和
{
    int total=0;
    int i=0;
    unsigned long sumtemp=0xff000000;
    for(;i<4;i++)
    {
        total=total+abs(((hashtrue&sumtemp)>>(24-i*8))-((hashfalse&sumtemp)>>(24-i*8)));
        //printf("\naverage distance :%d\n",total);
        sumtemp=sumtemp>>8;
        //printf("\naverage :%x\n",sumtemp);
    }
    return total;
}
void creat_w(unsigned char input[64],unsigned long w[80])       //数据填充函数
{
   int i,j;unsigned long temp,temp1;
   int arri;
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
//    printf("\nthis is w[].\n");
//    for(arri=0;arri<80;arri++)
//        {
//            printf("%lx,",w[arri]);
//            if((arri+1)%8==0)
//                {
//                    printf("\n");
//                }
//         }
}
void creat_errw(unsigned char input[64],unsigned long errw[80],int input_num,int input_bit)       //指定位置篡改
{
   int i,j,k;unsigned long temp,temp1;unsigned char inputtemp[64];
   int arri;
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
//    printf("\nthis is errw[].\n");        //显示errw[]
//    for(arri=0;arri<80;arri++)
//        {
//            printf("%lx,",errw[arri]);
//            if((arri+1)%8==0)
//                {
//                    printf("\n");
//                }
//        }
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
/*          ////酷炫迭代处理函数
void data_iterative(unsigned long *A,unsigned long *B,unsigned long *C,unsigned long *D,unsigned long *E,unsigned long w[80])
{
    unsigned long temp,temp1,temp2,temp3,k,f;int i,flag;
    for(i=0;i<i_num;i++)        //E=D,D=C,C=B循环右移两位,B=A,A=A循环左移5位与w[i]及k,f相加得到temp
         {                  //最后迭代80次后的ABCDE进行运算：H0=H0+A;H1=H1+B;H2=H2+C;H3=H3+D;H4=H4+E;
               flag=i/20;   //分成4个部分，设置4类k和f的情况。
               switch(flag)
                  {
                   case 0: k=0x5a827999;f=((*B)&(*C))|(~(*B)&(*D));break;
                   case 1: k=0x6ed9eba1;f=(*B)^(*C)^(*D);break;
                   case 2: k=0x8f1bbcdc;f=((*B)&(*C))|((*B)&(*D))|((*C)&(*D));break;
                   case 3: k=0xca62c1d6;f=(*B)^(*C)^(*D);break;
                  }
               //printf("%lx,%lx\n",k,f);
               temp1=(*A)<<5;
               temp2=(*A)>>27;
               temp3=temp1|temp2;       //A循环左移5位得到temp3
               temp=temp3+f+(*E)+w[i]+k;   //相加得到temp
               *E=*D;
               *D=*C;

               temp1=(*B)<<30;
               temp2=(*B)>>2;
               *C=temp1|temp2;
               *B=*A;
               *A=temp;

               printf("%d:",i);
               printf("%lx,%lx,%lx,%lx,%lx\n",A,B,C,D,E);

         }
}
*/
main()
{
   unsigned long H0=0x67452301,H1=0xefcdab89,H2=0x98badcfe,H3=0x10325476,H4=0xc3d2e1f0;//设置初始H0,H1,H2,H3,H4
   unsigned long temp,temp1,temp2,temp3,k,f;int i,flag;unsigned long w[80],errw[80];
   int input_len,char_len;int testi;unsigned char input[64];
   unsigned long A,B,C,D,E;unsigned long errA,errB,errC,errD,errE;
   unsigned long trueH0,trueH1,trueH2,trueH3,trueH4,falseH0,falseH1,falseH2,falseH3,falseH4;
   float totalsumA,totalsumB,totalsumC,totalsumD,totalsumE,total_sum=0;
   int rand_num;int rand_bit;
   int repeat_num;
   repeat_num=r_num;
   srand(time(NULL));		//time是一个函数，获取时间保存结果于ts中

   printf("input message:\n");
   scanf("%s",input);
   input_len=strlen(input);   //n为输入明文总长度
   char_len=sizeof(char)*8;
   //printf("%d\n",char_len);
   input_init(input,input_len);
   //for(testi=0;testi<64;testi++)
   //{
   //       printf("%d",input[testi]);
   //}
   //正确输出hash
   creat_w(input,w);
   /*for(i=0;i<80;i++)
   printf("%lx,",w[i]);*/
   printf("\n");
   A=H0;B=H1;C=H2;D=H3;E=H4;
   for(i=0;i<i_num;i++)     //E=D,D=C,C=B循环右移两位,B=A,A=A循环左移5位与w[i]及k,f相加得到temp
         {                  //最后迭代80次后的ABCDE进行运算：H0=H0+A;H1=H1+B;H2=H2+C;H3=H3+D;H4=H4+E;
               flag=i/20;   //分成4个部分，设置4类k和f的情况。
               switch(flag)
                  {
                   case 0: k=0x5a827999;f=(B&C)|(~B&D);break;
                   case 1: k=0x6ed9eba1;f=B^C^D;break;
                   case 2: k=0x8f1bbcdc;f=(B&C)|(B&D)|(C&D);break;
                   case 3: k=0xca62c1d6;f=B^C^D;break;
                  }
               /*printf("%lx,%lx\n",k,f); */
               temp1=A<<5;
               temp2=A>>27;
               temp3=temp1|temp2;       //A循环左移5位得到temp3
               temp=temp3+f+E+w[i]+k;   //相加得到temp
               E=D;
               D=C;

               temp1=B<<30;
               temp2=B>>2;
               C=temp1|temp2;
               B=A;
               A=temp;

               printf("%d:",i);
               printf("%8lx,%8lx,%8lx,%8lx,%8lx\n",A,B,C,D,E);
         }
   trueH0=H0+A;
   trueH1=H1+B;
   trueH2=H2+C;
   trueH3=H3+D;
   trueH4=H4+E;
   //printf("\noutput hash value:\n");
   //printf("%lx,%lx,%lx,%lx,%lx",trueH0,trueH1,trueH2,trueH3,trueH4);
   for(;repeat_num>0;repeat_num--)
    {
        //unsigned char * copyinput=NULL;
        //strcpy(*copyinput,input);

        rand_num=rand()%63;      //明文随机位
        rand_bit=(rand()%char_len)+1;   //bit随机位
        errA=H0;errB=H1;errC=H2;errD=H3;errE=H4;
        //printf("%8lx,%8lx,%8lx,%8lx,%8lx\n",errA,errB,errC,errD,errE);       //初始化errA...errE
        printf("%d,%d\t",rand_num,rand_bit);                                 //每次随机
        totalsumA=0;totalsumB=0;totalsumC=0;totalsumD=0;totalsumE=0;
        creat_errw(input,errw,rand_num,rand_bit);   //错误输出hash
        //printf("\n");
        //printf("\n");
        for(i=0;i<i_num;i++)        //E=D,D=C,C=B循环右移两位,B=A,A=A循环左移5位与w[i]及k,f相加得到temp
            {                  //最后迭代80次后的ABCDE进行运算：H0=H0+A;H1=H1+B;H2=H2+C;H3=H3+D;H4=H4+E;
                    totalsumA=0;totalsumB=0;totalsumC=0;totalsumD=0;totalsumE=0;
                    flag=i/20;   //分成4个部分，设置4类k和f的情况。
                    switch(flag)
                        {
                        case 0: k=0x5a827999;f=(errB&errC)|(~errB&errD);break;
                        case 1: k=0x6ed9eba1;f=errB^errC^errD;break;
                        case 2: k=0x8f1bbcdc;f=(errB&errC)|(errB&errD)|(errC&errD);break;
                        case 3: k=0xca62c1d6;f=errB^errC^errD;break;
                        }
                        /*printf("%lx,%lx\n",k,f); */
                    temp1=errA<<5;
                    temp2=errA>>27;
                    temp3=temp1|temp2;       //A循环左移5位得到temp3
                    temp=temp3+f+errE+errw[i]+k;   //相加得到temp
                    errE=errD;
                    errD=errC;

                    temp1=errB<<30;
                    temp2=errB>>2;
                    errC=temp1|temp2;
                    errB=errA;
                    errA=temp;

                    //printf("%d:",i);
                    //printf("%8lx,%8lx,%8lx,%8lx,%8lx\n",errA,errB,errC,errD,errE);
            }
        falseH0=H0+errA;
        falseH1=H1+errB;
        falseH2=H2+errC;
        falseH3=H3+errD;
        falseH4=H4+errE;
        printf("\noutput hash value with changing one bit:\n");
        printf("%lx,%lx,%lx,%lx,%lx\n",falseH0,falseH1,falseH2,falseH3,falseH4);

        totalsumA=((float)(cu_sum(trueH0,falseH0))/20);
        totalsumB=((float)(cu_sum(trueH1,falseH1))/20);
        totalsumC=((float)(cu_sum(trueH2,falseH2))/20);
        totalsumD=((float)(cu_sum(trueH3,falseH3))/20);
        totalsumE=((float)(cu_sum(trueH4,falseH4))/20);
        total_sum=total_sum+totalsumA+totalsumB+totalsumC+totalsumD+totalsumE;
        printf("\naverage distance :%f\n",totalsumA+totalsumB+totalsumC+totalsumD+totalsumE);
    }
    total_sum=total_sum/r_num;
    //printf("\noutput hash value:\n");
    //printf("%#lx,%#lx,%#lx,%#lx,%#lx",trueH0,trueH1,trueH2,trueH3,trueH4);
    //printf("\noutput hash value with changing one bit:\n");
    //printf("%#lx,%#lx,%#lx,%#lx,%#lx\n",falseH0,falseH1,falseH2,falseH3,falseH4);
    printf("The average absolute distance is %f\n",total_sum);
    getch();
}
/*
ran_input(unsigned char intput[64],int inputlength)
{
    int randi=0;
    int randinput=(rand()%256)+1;
    for(;randi<inputlength;randi++)
    {
        intput[randi]=randinput;
    }
}
*/
