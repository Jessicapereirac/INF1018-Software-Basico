/*JESSICA PEREIRA CESARIO 3WA 1711179*/
/* NOTA: 9,5 */

#include "bigint.h"
/* funcão copia, para não perder os valores */

void cpy (BigInt res, BigInt a)					
{																
	for(int i=0;i<NUM_BYTES;i++)
	{
		*res = *a;
		a++;res++;
	}
	return;
}
/* Atribuição (com extensão) */

void big_val(BigInt res, long val)
{
  unsigned char *bigI = res;				/*variaveL auxiliar para guardar o valor de res*/
																
  unsigned char *aux =(unsigned char*)&val;
  int i=0;
  while(i<16)						/* 16, pois é o tamanho do BigInt*/
  {
    if(i>=8)						/*i>= a 8 pois um long possui 8 bytes*/	
    {
        if(val>=0)					/*verifica se e positivo ou negativo*/					
            *bigI=0x00;					
           						/*atribui o valor referente ate o fim, 00's ou ff's*/
        if(val<0)
            *bigI=0xFF;
    }
    else
        *bigI=*aux;
           
    bigI++;
    aux++;
    i++;
 
  }
 
}

/* res = -a */

void big_comp2(BigInt res, BigInt a)
{
    unsigned char a1[NUM_BYTES];			/*fazendo uma copia de a para não perder o valor*/
    cpy(a1,a);

    int i=0;
    int c=0;
    while(c<16)
    {
      res[c]= ~(a[c]);					/*pego o complemento de todos as numeros*/
      c++;
    }
    while(i<16)
    {
      if(res[i]==0xFF)					/* se for 0xFF,ou seja, vai pro caso de somar um fora*/
        res[i]=~(res[i]);
      else
      {
        res[i]+=0x01;					/*acha algum lugar que não é o caso acima e soma*/
        break;
      }
      i++;
    }  
} 

/* res = a + b */

void big_sum(BigInt res, BigInt a, BigInt b)
{
    unsigned char a1[NUM_BYTES],b1[NUM_BYTES];
    cpy(a1,a);
    cpy(b1,b);
   
    int i=0;
    short somaA=0;					/*variaveis auxiliares para a soma */
    short somaB=0;
    short aux=0;
    unsigned char *aux2,*aux3;
    while(i<16)
    {
        aux2=(unsigned char*)&somaA;			/*coloco variaveis auxiliares para pegar o endereço */
        aux3=(unsigned char*)&somaB;
       
        *aux2=a1[i];					/*e vou colocando bit bit dos valores de a e b que recebo*/
        *aux3=b1[i];
       
        somaA=somaA+somaB+aux;				/*somo tudo e aux que seria o 1 que sai na hora da soma 1+1*/
        somaB=somaA&0x0100;
      
        if(somaB==0x0100)				/*onde aux que vai depender se a somaB é 0 ou não*/
            aux=1;
       
        else
            aux=0;
       
        *res=*aux2;
        somaA=0; somaB=0; res++; i++;			/*ando com os arrays*/
    }
   
    return;
}

/* res = a - b */
void big_sub(BigInt res, BigInt a, BigInt b)
{	
    unsigned char a1[NUM_BYTES],b1[NUM_BYTES];
    cpy(a1,a);
    cpy(b1,b);

    big_comp2(b1,b1);					/*pego o complemento do valor a subtrair*/			
    big_sum(res,a1,b1);					/*e somo */
    return;
}

/* res = a << n */
void big_shl(BigInt res, BigInt a, int n)
{
    unsigned char a1[NUM_BYTES];
    cpy(a1,a);

    unsigned char ass,ass2,zero;

    BigInt aux;

    int passos,i,c;					/*variaveis auxiliares que precisarei ao decorrer da função*/
    
    c=0;i=0;
   
    passos=n%8;						/*vejo quantas vezes n é multipla de 8*/
  
    if(passos==0)					/*e 'ando' com os elementos do array (1 byte),quantas vezes n*/			
    {							/*é multiplo de 8*/
        c=n/8;									
        while(i<16)
        {
            if(i<c)
                aux[i]=0x00;
           
            else
                aux[i]=a[i-c];				/*depois de andar (colocar zeros), coloco os valores do array*/
            i++;
        }
    }

    else
    {
       
        ass=0xFF << (8-passos);				/*se n não é multiplo de 8, em ass eu ando quantas vezes*/ 
	zero=0x00;					/*for preciso, para poder salvar o valor em ass2*/
              
        for(i=0;i<16;i++)
        {
            ass2=a[i]&ass;
            aux[i]=(a[i] << passos)|(zero >> (8-passos));/*operação que me devolvera o valor que quero */
            zero=ass2;					/*quando do o shift*/	
        }
        big_shl(aux,aux,(n-passos));			/*chamo a função novamente para poder voltar com um valor */
           						/*multiplo de 8 e entrar no if acima*/
    }
 
    for(i=0;i<16;i++)
            res[i]=aux[i];				/*passando o valor de aux, depois de dar o shift, para res*/

    return;
}

/* res = a >> n */
void big_shr (BigInt res, BigInt a, int n)
{
    unsigned char a1[NUM_BYTES];
    cpy(a1,a);

    unsigned char ass,ass2,zero;

    int passos,i,c;					/*variaveis auxiliares que precisarei ao decorrer da função*/

    BigInt aux;
    
    passos=n;
    c=0;i=16;
   
    passos=passos%8;					/*vejo quantas vezes n é multipla de 8*/
    
    if(passos==0)					/*e 'ando' com os elementos do array (1 byte),quantas vezes n*/	
    {							/*é multiplo de 8*/
        c=n/8;
       
        for(i=16;i>=0;i--)				/*anda de traz para frente no array para realizar o shift*/
        {
       
            if(i>(15-c))				/*coloca zero em quantas 15-c posições */
                aux[i]=0x00;
            else
                aux[i]=a[i+c];				/*volta a colocar o valor de a na posição i+c*/ 
        }
    }
    else
    {
        ass=0xFF >> (8-passos);				/*se n não é multiplo de 8, em ass eu ando quantas vezes*/ 
        zero=0x00;             				/*for preciso, para poder salvar o valor em ass2*/
        for(i=16;i>=0;i--)
        {
            ass2=a[i]&ass;
            aux[i]=(a[i] >> passos)|(zero<< (8-passos));/*operação que me devolvera o valor que quero */
            zero=ass2;					/*quando do o shift*/
        }
        big_shr(aux,aux,(n-passos));    		/*chamo a função novamente para poder voltar com um valor */
    }							/*multiplo de 8 e entrar no if acima*/
  
    for(i=0;i<16;i++)
            res[i]=aux[i];				/*passando o valor de aux, depois de dar o shift, para res*/
    return;
}


int verificaZero (BigInt n)
{
    for (int i=0;i<16;i++)				/*funçao para verificar se ainda ha zero ou não no numero*/
        if(n[i]!=0x00)
            return 1;
    return 0;
}

/* res = a * b */
void big_mul (BigInt res, BigInt a, BigInt b)
{
    unsigned char a1[NUM_BYTES],b1[NUM_BYTES];
    cpy(a1,a);
    cpy(b1,b);
    unsigned char *aux=a1,*aux2=b1,k,l;			/*variaveis auxiliares que precisarei ao decorrer da função*/
    long n=0x00;
  
    big_val(res,n);
   
    aux+=15;						/*ando com o array*/
    aux2+=15;
   
    k=0x80;
    l=0x80;
    k=k&*aux;						/*k,l são variaveis auxiliares que verificam se a ou b*/
    l=l&*aux2;						/*são negativos*/
   
    if(k==0x80)
    {
        big_comp2(a1,a1);
        n++;
    }
   							/*caso algum seja, faço o complemento a 2 e soma no n */
    if(l==0x80)
    {
        big_comp2(b1,b1);
        n++;
    }
    aux2=b1;
    l=0x01;
    while(verificaZero(b1)==1)				/*enquanto b não é todo zerado, multiplico*/
    {
        l=l&*aux2;
        if(l==0x01)
            big_sum(res,res,a1);			/*faço a operação soma e em seguida do o shift*/
        big_shl(a1,a1,1);				/*seguindo o raciocinio apresentado em sala*/
        big_shr(b1,b1,1);
        l=0x01;
   }
   if(n==1)
        big_comp2(res,res);				/*se fiz complemento a 2,volto para o numero de origem*/
   return;
}

