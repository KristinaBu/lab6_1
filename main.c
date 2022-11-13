#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#define COLUMN_NUMBER 4 // число столбцов таблицы
struct I_print {        // данные для печати результатов интегрирования
    char *name;         // название функции
    double i_sum;       // значение интегральной суммы
    double i_toch;      // точное значение интеграла
    int n;              // число разбиений области интегрирования  при котором достигнута требуемая точность
};
void printDividingLine(const char horizontalSybmol, const char connectorSybmol, const int m, const int *wn) {
    putchar(connectorSybmol);
    for (int line_i = 0; line_i < m; line_i++) {
        for (int line_j = 0; line_j < wn[line_i]; line_j++) {
            putchar(horizontalSybmol);
        }
        putchar(connectorSybmol);
    }
    putchar('\n');
}
void PrintTabl(struct I_print i_prn[], int k) {
    const char SIDE_SYBMOL = '|';
    const char HORIZONTAL_SYBMOL = '-';
    const char CONNECTOR_SYBMOL = '+';

    int wn[COLUMN_NUMBER] = {12, 18, 18, 10};   // ширина столбцов таблицы
    char *title[COLUMN_NUMBER] = {(char *) "Function", (char *) "Integral", (char *) "IntSum", (char *) "N "};
    int size[COLUMN_NUMBER];
    for (int i = 0; i < COLUMN_NUMBER; i++) {
        size[i] = (int) strlen(title[i]);
    }

    // шапка таблицы
    printDividingLine(HORIZONTAL_SYBMOL, CONNECTOR_SYBMOL, COLUMN_NUMBER, wn);

    putchar(SIDE_SYBMOL);
    for (int line_i = 0; line_i < COLUMN_NUMBER; line_i++) {
        int half = (wn[line_i] - size[line_i]) / 2;
        for (int line_j = 0; line_j < half; line_j++) {
            putchar(' ');
        }
        printf("%s", title[line_i]);
        for (int line_j = 0; line_j < half; line_j++) {
            putchar(' ');
        }
        putchar(SIDE_SYBMOL);
    }
    putchar('\n');

    printDividingLine(HORIZONTAL_SYBMOL, CONNECTOR_SYBMOL, COLUMN_NUMBER, wn);

    // заполнение таблицы
    for (int i = 0; i < k; i++) {
        putchar(SIDE_SYBMOL);
        printf("%12s", i_prn[i].name);
        putchar(SIDE_SYBMOL);

        char i_toch[15];
        sprintf(i_toch, "%.10f", i_prn[i].i_toch);
        printf("%18s", i_toch);
        putchar(SIDE_SYBMOL);

        char i_sum[15];
        sprintf(i_sum, "%.10f", i_prn[i].i_sum);
        printf("%18s", i_sum);
        putchar(SIDE_SYBMOL);

        printf("%10d", i_prn[i].n);
        putchar(SIDE_SYBMOL);
        putchar('\n');
        printDividingLine(HORIZONTAL_SYBMOL, CONNECTOR_SYBMOL, COLUMN_NUMBER, wn);
    }
}




int nomn=0;
int n[]={9,0,0,0}; //склад количеств прямоугольников и трапеций при вычислении сумм
//склад точностей

typedef double (*TPF)(double); //указатель на подынтегральную функция для х0
typedef double (*for_eps)(TPF f, double a, double b, double dx, int* n);


//подынтегральные функции для х0:
double f_ot_x1(double x0){
    return x0;
}
double f_ot_x2(double x0){
    return sin(22*x0);
}
double f_ot_x3(double x0){
    return pow(x0,4);
}
double f_ot_x4(double x0){
    return atan(x0);
}

//интегралы по прямоугольнику и трапеции
double IntRect(TPF f, double a, double b,double dx, int* n){

    double sumR=0;
    int nn=0;
    for(double x0=a;x0<b;x0=x0+dx){
        sumR=sumR+f(x0+dx/2);
        nn++;
    }
    sumR=sumR*dx;

    //f(nn);
    nomn=nn;
    n[0]=nomn;
    //printf("%d %d\n",nn,n[0]);
    return sumR; //
}//глобальная перемена N

double IntTrap(TPF f, double a, double b, double dx, int* n){

    double summT=0;
    int nn=0; //количество итераций !!!!!!!!!!!!!!!!!!! замена на фор и вывод ост суммы
    for(double x0=a;x0<b;x0=x0+dx){
        summT=summT+(f_ot_x1(x0)+ f(x0+dx))/2;
        nn++;
    }
    summT=summT*dx;
    n[0]=nomn;
    //printf("%d %d\n",nn,n[0]);
    return summT;
}//глобальная перемена N

//рассчет оределенного интеграла, классическая формула
double integr1(double a,double b){
    return (b*b - a*a)/2.0;
}
double integr2(double a,double b){
    return (cos(a*22.0) - cos(b*22.0))/22.0;
}
double integr3(double a,double b){
    return (b*b*b*b*b - a*a*a*a*a)/5.0;
}
double integr4(double a,double b){
    return b*atan(b) - a*atan(a) - (log(b*b+1) - log(a*a+1))/2.0;

}


//dx=(a-b)/n
//указатель трап/рект, указатель ф1,ф2,ф3 или ф4,  номер точности
double Epsilon(for_eps q,TPF f,int epsnom){ //указатель на функцию суммы прямоугольников или трапеций, epsnom-номер точности в массиве
    double EpsMas[5]={0.01, 0.001, 0.0001, 0.00001, 0.000001};
    double dx1=1;
    //double dx1=EpsMas[epsnom];
    //double dx1=1/(pow(2,epsnom-2));
    double i1=q(f,1,3,dx1,n); // сумма интеграла с дельта
    dx1=dx1/2;
    double i2=q(f,1,3,dx1,n); //сумма интеграла с дельта/2
    //printf("i1 i2 %lf %lf\n",i1,i2);

    int k=0;
    while ((fabs(i1-i2)/3)>EpsMas[epsnom]){

        i1=i2;
        //printf("i1 i2 %lf %lf dx1 %lf\n",i1,i2,dx1);
        dx1=dx1/2;
        i2=q(f,1,3,dx1,n);//призыв нового исчисления интегралов, но с делением дельта на 2
        k=k+1;
        //printf("k %d\n",k);
        //printf("%d %lf %lf dx1 %lf %lf\n",k,i1,i2,dx1,(fabs(i1-i2)/3));

    } //эпсилон в шаге dx/2;
    return i2;//вывод суммы с нужной точностью
}//вывод суммы по трап или рект

void Velikiy_print( ){
    for(int functS=0;functS<5;functS++){ //~epsnom



    }

}



int ItogPrint_Rect(int epsnomm0) {
    setlocale(LC_ALL, "Russian");
    printf("Метод прямоугольников. eps = %lf\n", pow(10,-epsnomm0-2));
    struct I_print a;
    a.name = (char *) "y=x*x";
    a.i_sum = Epsilon(IntRect,f_ot_x1,epsnomm0);
    a.i_toch = integr1(1,3);
    a.n = 3;

    struct I_print b;
    b.name = (char *) "y=sin(22x)";
    b.i_sum = Epsilon(IntRect,f_ot_x2,epsnomm0);;
    b.i_toch = integr2(1,3);
    b.n = 6;

    struct I_print c;
    c.name = (char *) "y=x^4 ";
    c.i_sum = Epsilon(IntRect,f_ot_x3,epsnomm0);;
    c.i_toch = integr3(1,3);
    c.n = 6555;

    struct I_print d;
    d.name = (char *) "y=arctg(x))";
    d.i_sum = Epsilon(IntRect,f_ot_x4,epsnomm0);;
    d.i_toch = integr4(1,3);
    d.n = 6;

    struct I_print f[4];
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;

    PrintTabl(f, 4);
    return 0;
}//!!!!!integr
int ItogPrint_Trap(int epsnomm0) {
    setlocale(LC_ALL, "Russian");
    printf("Метод трапеций. eps = %lf\n", pow(10,-epsnomm0-2));
    struct I_print a;
    a.name = (char *) "y=x*x";
    a.i_sum = Epsilon(IntTrap,f_ot_x1,epsnomm0);
    a.i_toch = integr1(1,3);
    a.n = 3;

    struct I_print b;
    b.name = (char *) "y=sin(22x)";
    b.i_sum = Epsilon(IntTrap,f_ot_x2,epsnomm0);;
    b.i_toch = integr2(1,3);
    b.n = 6;

    struct I_print c;
    c.name = (char *) "y=x^4 ";
    c.i_sum = Epsilon(IntTrap,f_ot_x3,epsnomm0);;
    c.i_toch = integr3(1,3);
    c.n = 6555;

    struct I_print d;
    d.name = (char *) "y=arctg(x))";
    d.i_sum = Epsilon(IntTrap,f_ot_x4,epsnomm0);;
    d.i_toch = integr4(1,3);
    d.n = 6;

    struct I_print f[4];
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;

    PrintTabl(f, 4);
    return 0;
}//!!!!!integr

int main() {
    setlocale(LC_ALL, "Russian");
    for (int Eps=0;Eps<5;Eps++) {
        ItogPrint_Rect(Eps);
        ItogPrint_Trap(Eps);
    }
    printf("\n");
    //вывод



    //printf("классика %lf\n", integr1(1,3));

    //printf("R %lf \n", Epsilon(IntRect,4));
    //printf("T %lf \n", Epsilon(IntTrap,4));


    //printf("%d \n",n[0]);
    return 0;
}
