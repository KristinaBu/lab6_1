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
double EpsMas[]={0.01, 0.001, 0.0001, 0.00001, 0.000001};//склад точностей

typedef double (*TPF)(double); //указатель на подынтегральную функция для х0
typedef double (*for_eps)(TPF f, double a, double b, double dx, int* n);

//подынтегральные функции для х0:
double f_ot_x(double x0){
    return x0*x0;
}

// f - ловушка-шаблон для вызываемой в intrect функции, которая
//будет использовать полученные тут значения. Принимает дабл, выдает дабл
double IntRect(TPF f, double a, double b, double dx, int* n){

    double sumR=0;
    int nn=0;
    for(double x0=a;x0<b;x0=x0+dx){
        sumR=sumR+f_ot_x(x0+dx/2);
        nn++;
    }
    sumR=sumR*dx;

    //f(nn);
    nomn=nn;
    n[0]=nomn;
    //printf("%d %d\n",nn,n[0]);
    return sumR; //
}

double IntTrap(TPF f, double a, double b, double dx, int* n){
    double summT=0;
    int nn=0; //количество итераций !!!!!!!!!!!!!!!!!!! замена на фор и вывод ост суммы
    for(double x0=a;x0<b;x0=x0+dx){
        summT=summT+(f_ot_x(x0)+ f_ot_x(x0+dx))/2;
        nn++;
    }
    summT=summT*dx;
    //f(nn);
    n[0]=nomn;
    //printf("%d %d\n",nn,n[0]);
    return summT;
}


double for_IntRectrap(double nn){
    printf("nn %lf\n",nn);
}//Убрать на Ф от х0

//рассчет оределенного интеграла, классическая формула
double integr(double a,double b){
    return (double)(fabs(pow(b,3)-pow(a,3))/(double)(3));
}




//dx=(a-b)/n
double Epsilon(for_eps q,int epsnom){ //указатель на функцию суммы прямоугольников или трапеций, epsnom-номер точности в массиве
    double i1=0,i2=0,dx1=1;
    i1=q(for_IntRectrap,1,3,dx1,n); // сумма интеграла с дельта
    //i1=q(for_IntRectrap,1,3,dx1/2,n); //сумма интеграла с дельта/2

    //int k=0;
    while ((fabs(i1-i2)/3)>EpsMas[epsnom]){
        i1=i2;
        dx1=dx1/2;
        i2=q(for_IntRectrap,1,3,dx1,n);//призыв нового исчисления интегралов, но с делением дельта на 2
        //k=k+1;
        //printf("k %d\n",k);

    } //эпсилон в шаге dx/2;
    return i2;//вывод суммы с нужной точностью
}//рассчет точности




int ItogPrint_Rect() {
    setlocale(LC_ALL, "Russian");
    printf("Прямоугольники\n");
    struct I_print a;
    a.name = (char *) "y=x*x";
    a.i_sum = 1;
    a.i_toch = integr(1,3);
    a.n = 3;

    struct I_print b;
    b.name = (char *) "y=sin(22x)";
    b.i_sum = 4;
    b.i_toch = 5;
    b.n = 6;

    struct I_print c;
    c.name = (char *) "y=x^4 ";
    c.i_sum = 4;
    c.i_toch = 5;
    c.n = 6555;

    struct I_print d;
    d.name = (char *) "y=arctg(x))";
    d.i_sum = 4;
    d.i_toch = 5;
    d.n = 6;

    struct I_print f[4];
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;

    PrintTabl(f, 4);
    return 0;
}
int ItogPrint_Trap() {
    setlocale(LC_ALL, "Russian");
    printf("Трапеции\n");
    struct I_print a;
    a.name = (char *) "y=x*x";
    a.i_sum = 1;
    a.i_toch = integr(1,3);
    a.n = 3;

    struct I_print b;
    b.name = (char *) "y=sin(22x)";
    b.i_sum = 4;
    b.i_toch = 5;
    b.n = 6;

    struct I_print c;
    c.name = (char *) "y=x^4 ";
    c.i_sum = 4;
    c.i_toch = 5;
    c.n = 6555;

    struct I_print d;
    d.name = (char *) "y=arctg(x))";
    d.i_sum = 4;
    d.i_toch = 5;
    d.n = 6;

    struct I_print f[4];
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;

    PrintTabl(f, 4);
    return 0;
}
int main() {
    setlocale(LC_ALL, "Russian");
    ItogPrint_Rect();
    ItogPrint_Trap();
    printf("\n");

    printf("классика %lf\n", integr(1,3));
    //printf("pr %d\n", IntRect(for_IntRectrap,1,3,0.1,n));
    //printf("tr %d\n", IntTrap(for_IntRectrap,1,3,0.1,n));
    for(int i;i<4;i++){
        printf("R %lf \n", Epsilon(IntRect,i));
        printf("T %lf \n", Epsilon(IntTrap,i));
    }

    printf("%d \n",n[0]);
    return 0;
}
