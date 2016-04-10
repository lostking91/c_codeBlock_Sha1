/* �Ľ�sha-1�㷨��ɢ���ܵ��������ֱ䶯ͳ�� */

#include<stdio.h>
#include <string.h>
#include<math.h>
#include<stdlib.h>
// #define i_num 80        //���������Ĵ���
#define r_num 1             //ѭ������
#define tr_i_num 512         //�޸ı���
char ran_input(unsigned char intput[64],int inputlength)
{
    int randi=0;
    int randinput=(rand()%256)+1;
    for(;randi<inputlength;randi++)
    {
        intput[randi]=randinput;
    }
}

void creat_w(unsigned char input[64],unsigned long w[80])
{
   int i,j;
   int wflag;unsigned long k11,k12,k21,k22;
   unsigned long tempw1,tempw2,newtempw1,newtempw2,midnewtempw1,midnewtempw2;
   unsigned long midtempw1,midtempw1_1,midtempw1_2;
   unsigned long midtempw2,midtempw2_1,midtempw2_2;
   for(i=0;i<80;i++)
   {
       wflag=i/40;   //�ֳ�2�����֣�����2��k�������
       switch(wflag)
        {
            case 0: k11=0x428A,k12=0x7137,k21=0xB5c0,k22=0xE9B5;break;
            case 1: k11=0x3956,k12=0x59F1,k21=0x923F,k22=0xAB1C;break;
        }
       if(i<16)   //i<16��w[i]ʵ����Ϊ4��char������ɵ�long���͡�����input[64]ȫ������w[15]����
       {
           j=4*i;
           w[i]=(((long)input[j])<<24 |((long)input[1+j])<<16|((long)input[2+j])<<8|((long)input[3+j])<<0)+0xBEF9A3F7;
       }
       else if(15<i<40)      //��w[16]~w[80]��ֵ:��ѭ�����ó�w[16]~w[80],�ٽ���λ��ѭ������һλ
       {
           tempw1=w[i-16]^w[i-14]^w[i-8]^w[i-3];
           tempw2=w[i-1]^w[i-3]^w[i-7];

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
            w[i]=midnewtempw1|midnewtempw2;

         }
         else      //��w[16]~w[80]��ֵ:��ѭ�����ó�w[16]~w[80],�ٽ���λ��ѭ������һλ
         {
             tempw1=w[i-16]^w[i-14]^w[i-8]^w[i-3];
             tempw2=w[i-1]^w[i-3]^w[i-29];

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
             w[i]=midnewtempw1|midnewtempw2;

         }
   }

}
void creat_errw(unsigned char input[64],unsigned long errw[80],int input_num,int input_bit)       //ָ��λ�ô۸�
{
   int i,j,k;unsigned char inputtemp[64];
   int wflag;unsigned long k11,k12,k21,k22;
   unsigned long tempw1,tempw2,newtempw1,newtempw2,midnewtempw1,midnewtempw2;
   unsigned long midtempw1,midtempw1_1,midtempw1_2;
   unsigned long midtempw2,midtempw2_1,midtempw2_2;
   int arri;

   for(k=0;k<64;k++)
   {
       inputtemp[k]=input[k];
   }
   inputtemp[input_num]=inputtemp[input_num] ^ (1<<(input_bit-1));

   for(i=0;i<80;i++)
   {
       wflag=i/40;   //�ֳ�2�����֣�����2��k�������
       switch(wflag)
        {
            case 0: k11=0x428A,k12=0x7137,k21=0xB5c0,k22=0xE9B5;break;
            case 1: k11=0x3956,k12=0x59F1,k21=0x923F,k22=0xAB1C;break;
        }
       if(i<16)   //i<16��w[i]ʵ����Ϊ4��char������ɵ�long���͡�����input[64]ȫ������w[15]����
       {
           j=4*i;
           errw[i]=(((long)inputtemp[j])<<24 |((long)inputtemp[1+j])<<16|((long)inputtemp[2+j])<<8|((long)inputtemp[3+j])<<0)+0xBEF9A3F7;
       }
       else if(15<i<40)      //��w[16]~w[80]��ֵ:��ѭ�����ó�w[16]~w[80],�ٽ���λ��ѭ������һλ
       {
           tempw1=errw[i-16]^errw[i-14]^errw[i-8]^errw[i-3];
           tempw2=errw[i-1]^errw[i-3]^errw[i-7];

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
            errw[i]=midnewtempw1|midnewtempw2;

         }
         else      //��w[16]~w[80]��ֵ:��ѭ�����ó�w[16]~w[80],�ٽ���λ��ѭ������һλ
         {
             tempw1=errw[i-16]^errw[i-14]^errw[i-8]^errw[i-3];
             tempw2=errw[i-1]^errw[i-3]^errw[i-29];

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
             errw[i]=midnewtempw1|midnewtempw2;

         }
   }

}
char ms_len(long a,char intput[64])     // input��60��61��62��63�������input[64]='\0';
{
    unsigned long temp3,p1;  int i,j;
    temp3=0;            // 0x00000000
    p1=~(~temp3<<8);    // 0x00000000-> 0xffffffff-> 0xffffff00-> 0x000000ff
    for(i=0;i<4;i++)
       {
          j=8*i;
          intput[63-i]=(char)((a&(p1<<j))>>j);  //����inputΪiscbupt��a=56��ת����ascii����Ϊ��char������ ����ֻ��Ϊ0x7f����

       }

}
char input_init(char input[64],int n)     //������ʼ�����룬������������
{
   int i;long x;
   if(n<57)           //n<57,�ȶ�60,61,62,63������䡣n=56,56~59ȫ��ֵΪ0��n<56,input[n]=128,n+1��ȫ��ֵΪ0
          {
                 x=n*8;
                 ms_len(x,input);
                 if(n==56)
                     for(i=n;i<60;i++)
                     input[i]=0;
                 else                 //������ĩλ����128������λ��0
                    {
                     input[n]=128;
                     for(i=n+1;i<60;i++)
                     input[i]=0;
                    }

          }
}
int bit_expand(unsigned long hashtrue[80],unsigned long hashfalse[80],int n,char expand[512][2560],int compute)            //��dc,����dc����
{
    int i,j;
    for(i=0;i<80;i++)
    {
        unsigned long temp=0x000000001;
        for(j=1;j<=32;j++)
        {
           if((hashtrue[i]&temp)!=(hashfalse[i]&temp))       //�ı�ĸ���+1
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

   unsigned long changeT,changeTemp1,changeTemp2,changeTemp3,changeTemp;unsigned long ATemp,ATemp1,ATemp2,ATemp3;
   unsigned long errchangeT,errchangeTemp1,errchangeTemp2,errchangeTemp3,errchangeTemp;
   unsigned long errATemp,errATemp1,errATemp2,errATemp3;

   int dc_i,dc_j,da_i,da_j;int daH4,daH3,daH2,daH1,daH0,da_total_sum;

   unsigned long ARol,ARol7,ARol13,ARol19;unsigned long ARolTemp1,ARolTemp2;
   unsigned long errARol,errARol7,errARol13,errARol19;unsigned long errARolTemp1,errARolTemp2;
   int compute=0;

   repeat_num=r_num;
   srand(time(NULL));		//time��һ����������ȡʱ�䱣������ts��
   char sha1_expand[512][2560];
   for(repeat_num;repeat_num>0;repeat_num--)
    {
        inputlength=rand()%57;
        ran_input(input,inputlength);
        //printf("input message:\n");
        //scanf("%s",input);
        input_len=strlen(input);   //nΪ���������ܳ���
        char_len=sizeof(char)*8;
        //printf("%d\n",char_len);
        input_init(input,input_len);

        creat_w(input,w);

        // ָ��λ�����ı�
        for(traverse_i=1;traverse_i<=tr_i_num ;traverse_i++)
        {
            da_total_sum=0;
            traverse_num=(traverse_i-1)/8;
            traverse_bit=(traverse_i-1)%8+1;
            creat_errw(input,errw,traverse_num,traverse_bit);   //�������hash
            compute=bit_expand(w,errw,traverse_i,sha1_expand,compute);

            unsigned long errw[80]={0};
            int showj=0;
//            for(;showj<2560;showj++){
//                printf("%d,",sha1_expand[traverse_i-1][showj]);
//                //printf(":%d\t%d\t",traverse_i-1,showj);
//            }
            printf("???????? ");
        }
    }

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


