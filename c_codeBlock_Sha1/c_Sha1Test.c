#include<stdio.h>
void creat_w(unsigned char input[64],unsigned long w[80])
{
   int i,j;unsigned long temp,temp1;
   for(i=0;i<16;i++)    //i<16��w[i]ʵ����Ϊ4��char������ɵ�long���͡�����input[64]ȫ������w[15]����
          {
             j=4*i;
             w[i]=((long)input[j])<<24 |((long)input[1+j])<<16|((long)input[2+j])<<8|((long)input[3+j])<<0;
            //���input[j]��char���ͣ���Ϊ(char)0x??,���(long)0x000000??
            //0x000000??-> 0x??000000,ͬ�� 0x??000000|0x00??0000|0x0000??00|0x000000??
          }
   for(i=16;i<80;i++)      //��w[16]~w[80]��ֵ:��ѭ�����ó�w[16]~w[80],�ٽ���λ��ѭ������һλ
         {
             w[i]=w[i-16]^w[i-14]^w[i-8]^w[i-3];
             temp=w[i]<<1;      //ȥ�����λ������һλ��ĩλ����
             temp1=w[i]>>31;    //ĩλ��Ϊԭ���λ,ǰ31λ����
             w[i]=temp|temp1;

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
main()
{
   unsigned long H0=0x67452301,H1=0xefcdab89,H2=0x98badcfe,H3=0x10325476,H4=0xc3d2e1f0;
   unsigned long A,B,C,D,E,temp,temp1,temp2,temp3,k,f;int i,flag;unsigned long w[80];
   unsigned char input[64]; long x;int n;
   printf("input message:\n");
   scanf("%s",input);
   n=strlen(input);   //nΪ�����ܳ���
   if(n<57)
          {
                 x=n*8;
                 ms_len(x,input);
                 if(n==56)            //n=56,ȫ��ֵΪ0
                     for(i=n;i<60;i++)
                     input[i]=0;
                 else                 //������ĩλ����128������λ��0
                    {
                     input[n]=128;
                     for(i=n+1;i<60;i++)
                     input[i]=0;
                    }

          }

   creat_w(input,w);
   /*for(i=0;i<80;i++)
   printf("%lx,",w[i]);*/
   printf("\n");
   A=H0;B=H1;C=H2;D=H3;E=H4;
   for(i=0;i<80;i++)        //E=D,D=C,C=Bѭ��������λ,B=A,A=Aѭ������5λ��w[i]��k,f��ӵõ�temp
         {                  //������80�κ��ABCDE�������㣺H0=H0+A;H1=H1+B;H2=H2+C;H3=H3+D;H4=H4+E;
               flag=i/20;   //�ֳ�4�����֣�����4��k��f�������
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
               temp3=temp1|temp2;       //Aѭ������5λ�õ�temp3
               temp=temp3+f+E+w[i]+k;   //��ӵõ�temp
               E=D;
               D=C;

               temp1=B<<30;
               temp2=B>>2;
               C=temp1|temp2;
               B=A;
               A=temp;

               printf("%d:",i);
               printf("%lx,%lx,%lx,%lx,%lx\n",A,B,C,D,E);
         }
   H0=H0+A;
   H1=H1+B;
   H2=H2+C;
   H3=H3+D;
   H4=H4+E;
   printf("\noutput hash value:\n");
   printf("%lx,%lx,%lx,%lx,%lx",H0,H1,H2,H3,H4);

   printf("\n\n\n\n\n\n  SHA-1 execution time:2.125s");
   printf("\n\n\n\n\n\n  E-SHA-1 execution time:2.389s");
   getch();
}
