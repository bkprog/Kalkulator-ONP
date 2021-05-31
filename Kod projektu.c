#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

typedef struct _stosTranslacja
{
char tablica[16];
size_t poz;
}stosTranslacja;

void push(stosTranslacja *stack,char znak)
{
stack->tablica[stack->poz++]=znak;
}

double pop(stosTranslacja *stack)
{
return stack->tablica[--stack->poz];
}

double peek(stosTranslacja *stack)
{
return stack->tablica[stack->poz-1];
}

int isEmpty(stosTranslacja *stack)
{
if(stack->poz==0)
{
return 2;
}
else
{
return 3;
}
}

typedef double opr(double a,double b);

double dod(double a,double b)
{ return a+b; }
double ode(double a,double b)
{ return a-b; }
double mno(double a,double b)
{ return a*b; }
double dzi(double a,double b)
{ return a/b; }
double mod(double a, double b)
{return (int)a%(int)b; }
double pot(double a, double b)
{return pow(a,b); }

struct { char op; opr *fun; } oprtb[]=
{
   {'+', &dod},
   {'-', &ode},
   {'*', &mno},
   {'/', &dzi},
   {'%', &mod},
   {'^', &pot},
};
const size_t oprtbsize=sizeof(oprtb)/sizeof(*oprtb);


int dzialanie(stosTranslacja *stack,char ch1)
{
   int i;
   for(i=0;i<oprtbsize;++i)
   {
      if(oprtb[i].op==ch1)
      {
         double b=pop(stack);
         double a=pop(stack);
         push(stack,oprtb[i].fun(a,b));
         return 1;
      }
   }
   return 0;
}

double kalk(char *text)
{
   char ch1,*ptr;
   stosTranslacja stack={{0},0};
   while(*text)
   {
      char ch1=*text;
      if(isspace(ch1))
      {
        ++text;
      }

      else
      {
         double value=strtod(text,&ptr);
         if(ptr>text)
         {
            push(&stack,value);
            text=ptr;
         }
         else
         {
            dzialanie(&stack,ch1);
            ++text;
         }
      }
   }
   double wyn=pop(&stack);
   return wyn;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


void translator(char *tekst)
{
stosTranslacja stack1={{0},0};
stosTranslacja stack2={{0},0};
int n=0;
char ch;
char tekst_po_T[25];
memset(tekst_po_T,0,25);
int flaga=0;
char space;

while(*tekst)
{
ch=*tekst;

if(isspace(ch))
{
    space=tekst_po_T[n-1];
    if(isspace(space)){
    ++tekst;
    }
    else{tekst_po_T[n]=ch;
    n++;}
}
else if(ch=='(')
{
    ++tekst;
    flaga=1;
}
else if(ch==')')
{
  while(isEmpty(&stack2)==3)
  {
    tekst_po_T[n]=pop(&stack2);
    n++;
  }

   ++tekst;
   flaga=0;
}

else if(ch=='^' && flaga==1)
{
if(isEmpty(&stack2)==2 || peek(&stack2)!='^')
{
    push(&stack2,ch);
    ++tekst;
}
else if(peek(&stack2)=='^')
{
   while(peek(&stack2)=='^')
   {
      tekst_po_T[n]=pop(&stack2);
      n++;
   }

push(&stack2,ch);
++tekst;
}
}

else if(ch=='^' && flaga==0)
{
if(isEmpty(&stack1)==2 || peek(&stack1)!='^')
{
    push(&stack1,ch);
    ++tekst;
}
else if(peek(&stack1)=='^')
{
   while(peek(&stack1)=='^')
   {
      tekst_po_T[n]=pop(&stack1);
      n++;
   }

push(&stack1,ch);
++tekst;
}
}

else if(ch=='*' || ch=='/')
{

if(flaga==1){

if(isEmpty(&stack2) || peek(&stack2)=='+' || peek(&stack2)=='-' )
{
   push(&stack2,ch);
   ++tekst;
}

else if(peek(&stack2)=='^' || peek(&stack2) == '/' || peek(&stack2)=='*')
{
   while(peek(&stack2)!='+' || peek(&stack2)!='-' ||isEmpty(&stack2)==3)
   {
      tekst_po_T[n]=pop(&stack2);
      n++;
   }

   push(&stack2,ch);
   ++tekst;
}
}
else if(flaga==0){

if(isEmpty(&stack1)==2 || peek(&stack1)=='+' || peek(&stack1)=='-')
{
   push(&stack1,ch);
   ++tekst;
}

else if(peek(&stack1)!='^' || peek(&stack1) != '/' || peek(&stack1)!='*')
{
   while(peek(&stack1)!='+' || peek(&stack1)!='-' ||isEmpty(&stack1)!=2 )
   {
      tekst_po_T[n]=pop(&stack1);
      n++;
   }

   push(&stack1,ch);
   ++tekst;
}
}
}

else if(ch=='+' || ch=='-' ||ch=='%')
{

if (flaga==1)
{
   while(isEmpty(&stack2)==3)
   {
      tekst_po_T[n]=pop(&stack2);
      n++;
   }

   push(&stack2,ch);
   ++tekst;
}
if (flaga==0)
{
   while(isEmpty(&stack1)==3)
   {
      tekst_po_T[n]=pop(&stack1);
      n++;
   }

   push(&stack1,ch);
   ++tekst;
}
}
else
{
    tekst_po_T[n]=ch;
    n++;
    ++tekst;
}

}

while(isEmpty(&stack1)==3)
{
tekst_po_T[n]=pop(&stack1);
n++;
}

int forx;
puts("Po przetlumaczeniu:\n");
for(forx=0; forx<20;forx++)
{
printf("%c", tekst_po_T[forx]);
}
printf("\n\nWynik: \n%lf",kalk(tekst_po_T));
}

int main()
{
int wyb;

puts("Podaj wyrazenie (miedzy kazdym operatorem i liczba nalezy postawic odstep (spacje) )\n");
char tekst_do_translacji[25];
gets(tekst_do_translacji);
puts("Forma wprowadzonych dzialan: 1- arytmetycznie 2- w onp\n");
scanf("%d",&wyb);

switch(wyb)
{
case 1:
translator(tekst_do_translacji);
break;

case 2:
printf("Wynik: \n%lf",kalk(tekst_do_translacji));
break;

default:
puts("Blad, wybierz 1 lub 2");
break;
}

return 0;
}
