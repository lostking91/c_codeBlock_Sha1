/* sha-1算法抗碰撞性逐步统计结果 */
#include<stdio.h>
#include <string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define r_num 1     // 循环次数
#define ori_long 2800   // 源信息长度       1 <512-64-1(<57bit),2 <2*512-64-1(<2*64-7)
#define i_num (80*ori_long)      // 迭代操作的次数【最大次数(80*ori_long)】
#define p_l (64*ori_long)       //p_l-->packet_long 数据包长度
#define w_l (80*ori_long)

char ran_input(unsigned char intput[p_l],int inputlength)
{
    int randi=0;
    int randinput=(rand()%256)+1;
    for(;randi<inputlength;randi++)
    {
        intput[randi]=randinput;
    }
    /*
    //int randinput=rand()%257;       //int randinput=(rand()%256)+1;
    // 其实没有必要修改，但是修改后从定义上正确，即rand为多少bit即为多少bit输入
    // 未修改是算法上的正确，因为可能存在rand为多少bit实际后几个bit为0情况下bit数目更少。但是代码上算法上不受影响的！！！
    for(;randi<inputlength;randi++)
    {
        if(randi==inputlength-1)
        {
            intput[randi]=(rand()%256)+1;
        }
        else
        {
            intput[randi]=rand()%257;
        }
    }
    */
}
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
void creat_w(unsigned char input[p_l],unsigned long w[w_l],int a)
{
   int i,j;
   int wflag;unsigned long k11,k12,k21,k22;
   unsigned long tempw1,tempw2,newtempw1,newtempw2,midnewtempw1,midnewtempw2;
   unsigned long midtempw1,midtempw1_1,midtempw1_2;
   unsigned long midtempw2,midtempw2_1,midtempw2_2;
   int t=0;
   for(;t<a;t++)
   {
        for(i=0;i<80;i++)
        {
            wflag=i/40;   //分成2个部分，设置2类k的情况。
            switch(wflag)
            {
                case 0: k11=0x428A,k12=0x7137,k21=0xB5c0,k22=0xE9B5;break;
                case 1: k11=0x3956,k12=0x59F1,k21=0x923F,k22=0xAB1C;break;
            }
            if(i<16)   //i<16，w[i]实际上为4个char类型组成的long类型。所以input[64]全放入了w[15]以内
            {
                j=4*i;
                w[t*80+i]=(((long)input[t*80+j])<<24 |((long)input[t*80+1+j])<<16|((long)input[t*80+2+j])<<8|((long)input[t*80+3+j])<<0)+0xBEF9A3F7;
            }
            else if(15<i<40)      //给w[16]~w[80]补值:先循环异或得出w[16]~w[80],再进行位的循环左移一位
            {
                tempw1=w[t*80+i-16]^w[t*80+i-14]^w[t*80+i-8]^w[t*80+i-3];
                tempw2=w[t*80+i-7]^w[t*80+i-3]^w[t*80+i-1];

                midtempw1=(((tempw1&0x0000ffff) ^k11) <<16 )|((tempw1&0xffff0000 )^(k12<<16 ));
                midtempw1_1=midtempw1<<7;
                midtempw1_2=midtempw1>>25;
                newtempw1=midtempw1_1|midtempw1_2;

                midtempw2=(((tempw1&0x0000ffff) ^k21) <<16 )|((tempw1&0xffff0000 )^(k22<<16 ));
                midtempw2_1=midtempw2<<7;
                midtempw2_2=midtempw2>>25;
                newtempw2=midtempw2_1|midtempw2_2;

                midnewtempw1=(newtempw1^newtempw2)<<3;
                midnewtempw2=(newtempw1^newtempw2)>>29;
                w[t*80+i]=midnewtempw1|midnewtempw2;

            }
            else      //给w[16]~w[80]补值:先循环异或得出w[16]~w[80],再进行位的循环左移一位
            {
                tempw1=w[t*80+i-16]^w[t*80+i-14]^w[t*80+i-8]^w[t*80+i-3];
                tempw2=w[t*80+i-39]^w[t*80+i-37]^w[t*80+i-29];

                midtempw1=(((tempw1&0x0000ffff) ^k11) <<16 )|((tempw1&0xffff0000 )^(k12<<16 ));
                midtempw1_1=midtempw1<<7;
                midtempw1_2=midtempw1>>25;
                newtempw1=midtempw1_1|midtempw1_2;

                midtempw2=(((tempw1&0x0000ffff) ^k21) <<16 )|((tempw1&0xffff0000 )^(k22<<16 ));
                midtempw2_1=midtempw2<<7;
                midtempw2_2=midtempw2>>25;
                newtempw2=midtempw2_1|midtempw2_2;

                midnewtempw1=(newtempw1^newtempw2)<<3;
                midnewtempw2=(newtempw1^newtempw2)>>29;
                w[t*80+i]=midnewtempw1|midnewtempw2;

            }
        }
   }
}
void creat_errw(unsigned char input[p_l],unsigned long errw[w_l],int input_num,int input_bit,int a)       //指定位置篡改
{
   int i,j,k;unsigned char inputtemp[a*64];
   int wflag;unsigned long k11,k12,k21,k22;
   unsigned long tempw1,tempw2,newtempw1,newtempw2,midnewtempw1,midnewtempw2;
   unsigned long midtempw1,midtempw1_1,midtempw1_2;
   unsigned long midtempw2,midtempw2_1,midtempw2_2;
   int arri;

   for(k=0;k<a*64;k++)
   {
       inputtemp[k]=input[k];
   }
   inputtemp[input_num]=inputtemp[input_num] ^ (1<<(input_bit-1));

   int t=0;
   for(;t<a;t++)
   {
        for(i=0;i<80;i++)
        {
            wflag=i/40;   //分成2个部分，设置2类k的情况。
            switch(wflag)
            {
                case 0: k11=0x428A,k12=0x7137,k21=0xB5c0,k22=0xE9B5;break;
                case 1: k11=0x3956,k12=0x59F1,k21=0x923F,k22=0xAB1C;break;
            }
            if(i<16)   //i<16，w[i]实际上为4个char类型组成的long类型。所以input[64]全放入了w[15]以内
            {
                j=4*i;
                errw[t*80+i]=(((long)input[t*80+j])<<24 |((long)input[t*80+1+j])<<16|((long)input[t*80+2+j])<<8|((long)input[t*80+3+j])<<0)+0xBEF9A3F7;
            }
            else if(15<i<40)      //给w[16]~w[80]补值:先循环异或得出w[16]~w[80],再进行位的循环左移一位
            {
                tempw1=errw[t*80+i-16]^errw[t*80+i-14]^errw[t*80+i-8]^errw[t*80+i-3];
                tempw2=errw[t*80+i-7]^errw[t*80+i-3]^errw[t*80+i-1];

                midtempw1=(((tempw1&0x0000ffff) ^k11) <<16 )|((tempw1&0xffff0000 )^(k12<<16 ));
                midtempw1_1=midtempw1<<7;
                midtempw1_2=midtempw1>>25;
                newtempw1=midtempw1_1|midtempw1_2;

                midtempw2=(((tempw1&0x0000ffff) ^k21) <<16 )|((tempw1&0xffff0000 )^(k22<<16 ));
                midtempw2_1=midtempw2<<7;
                midtempw2_2=midtempw2>>25;
                newtempw2=midtempw2_1|midtempw2_2;

                midnewtempw1=(newtempw1^newtempw2)<<3;
                midnewtempw2=(newtempw1^newtempw2)>>29;
                errw[t*80+i]=midnewtempw1|midnewtempw2;

            }
            else      //给w[16]~w[80]补值:先循环异或得出w[16]~w[80],再进行位的循环左移一位
            {
                tempw1=errw[t*80+i-16]^errw[t*80+i-14]^errw[t*80+i-8]^errw[t*80+i-3];
                tempw2=errw[t*80+i-39]^errw[t*80+i-37]^errw[t*80+i-29];

                midtempw1=(((tempw1&0x0000ffff) ^k11) <<16 )|((tempw1&0xffff0000 )^(k12<<16 ));
                midtempw1_1=midtempw1<<7;
                midtempw1_2=midtempw1>>25;
                newtempw1=midtempw1_1|midtempw1_2;

                midtempw2=(((tempw1&0x0000ffff) ^k21) <<16 )|((tempw1&0xffff0000 )^(k22<<16 ));
                midtempw2_1=midtempw2<<7;
                midtempw2_2=midtempw2>>25;
                newtempw2=midtempw2_1|midtempw2_2;

                midnewtempw1=(newtempw1^newtempw2)<<3;
                midnewtempw2=(newtempw1^newtempw2)>>29;
                errw[t*80+i]=midnewtempw1|midnewtempw2;

            }
        }
   }
}
char ms_len(long a,char intput[p_l])     // input的60，61，62，63进行填充input[64]='\0';
{                                       //根据p_l的值进行填充
    unsigned long temp3,p1;
    int i,j;
    temp3=0;            // 0x00000000
    p1=~(~temp3<<8);    // 0x00000000-> 0xffffffff-> 0xffffff00-> 0x000000ff
    for(i=0;i<4;i++)
    {
        j=8*i;
        intput[p_l-1-i]=(char)((a&(p1<<j))>>j);  //假设input为iscbupt，a=56，转化成ascii，因为有char的限制 输入只能为0x7f以下
    }
}
char input_init(char input[p_l],int n)     //用来初始化输入，对数组进行填充
{
   if(n<p_l-7)
   {
        int i;long x;
        x=n*8;
        ms_len(x,input);
        if(n==p_l-8)
            for(i=n;i<p_l-4;i++)
                input[i]=0;
            else                 //在数据末位增加128，其他位赋0
            {
                input[n]=128;   // 填充1个“1”和若干个“0”
                for(i=n+1;i<p_l-4;i++)
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
   clock_t start, finish;
   double  duration;
   start = clock();

   unsigned long H0=0x67452301,H1=0xefcdab89,H2=0x98badcfe,H3=0x10325476,H4=0xc3d2e1f0;//设置初始H0,H1,H2,H3,H4
   unsigned long temp,temp1,temp2,temp3,k,f;int i,flag;unsigned long w[w_l],errw[w_l];
   int input_len,char_len;int testi;unsigned char input[p_l];
   unsigned long A,B,C,D,E;unsigned long errA,errB,errC,errD,errE;
   unsigned long trueH0,trueH1,trueH2,trueH3,trueH4,falseH0,falseH1,falseH2,falseH3,falseH4;
   float totalsumA,totalsumB,totalsumC,totalsumD,totalsumE,total_sum=0;
   int rand_num,rand_bit,ran_depend_i,ran_depend_j;
   int repeat_num;int inputlength;
   repeat_num=r_num;
   srand(time(NULL));		//time是一个函数，获取时间保存结果于ts中

    for(;repeat_num>0;repeat_num--)
    {
        inputlength=rand()%57;
        ran_input(input,inputlength);
        inputlength=ori_long-1+rand()%57;   //根据要求数据包的数量设置数据长度
        /* 64改变成p_l
        input[64]定义处改变，ran_input改变,input_init改变,ms_len改变,creat_w改变  */

        ran_input(input,inputlength);
        input_len=strlen(input);   //n为输入明文总长度
        char_len=sizeof(char)*8;   //输入明文总bit长度
        input_init(input,input_len);    //搞定任意长度数据初始化问题
        creat_w(input,w,ori_long);

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
         }
        trueH0=H0+A;
        trueH1=H1+B;
        trueH2=H2+C;
        trueH3=H3+D;
        trueH4=H4+E;

        rand_num=rand()%(p_l-ori_long);                 //明文随机位
        rand_bit=(rand()%char_len)+1;       //bit随机位
        ran_depend_i=rand_num*8+rand_bit;   //改变的位置
        errA=H0;errB=H1;errC=H2;errD=H3;errE=H4;
        //printf("%8lx,%8lx,%8lx,%8lx,%8lx\n",errA,errB,errC,errD,errE);       //初始化errA...errE
        //printf("%d,%d\t",rand_num,rand_bit);                                 //每次随机
        totalsumA=0;totalsumB=0;totalsumC=0;totalsumD=0;totalsumE=0;
        creat_errw(input,errw,rand_num,rand_bit,ori_long);    //错误输出hash
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

        }

        falseH0=H0+errA;
        falseH1=H1+errB;
        falseH2=H2+errC;
        falseH3=H3+errD;
        falseH4=H4+errE;
        //printf("\noutput hash value with changing one bit:\n");
        //printf("%lx,%lx,%lx,%lx,%lx\n",falseH0,falseH1,falseH2,falseH3,falseH4);

        //求误码前后hash值对应字符见平均距离
        totalsumA=((float)(cu_sum(trueH0,falseH0))/20);
        totalsumB=((float)(cu_sum(trueH1,falseH1))/20);
        totalsumC=((float)(cu_sum(trueH2,falseH2))/20);
        totalsumD=((float)(cu_sum(trueH3,falseH3))/20);
        totalsumE=((float)(cu_sum(trueH4,falseH4))/20);
        total_sum=total_sum+totalsumA+totalsumB+totalsumC+totalsumD+totalsumE;

    }
    total_sum=total_sum/r_num;
    printf("The average absolute distance is %f\n",total_sum);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration );
    system("pause");
    //getch();
}


