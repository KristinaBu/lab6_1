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

int ItogPrint() {
    setlocale(LC_ALL, "Russian");
    struct I_print a;
    a.name = (char *) "y=x";
    a.i_sum = 1;
    a.i_toch = 2;
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


typedef double (*TPF)(double); //подынтегр функция

double f_ot_x(double x0){
    return x0*x0;
}

//интеграл прямоугольников
// f - ловушка-шаблон для вызываемой в intrect функции, которая
//будет использовать полученные тут значения. Принимает дабл, выдает дабл
//вывод суммы для шаблона
int IntRect(TPF f, double a, double b, double eps, int* n){

    double sumR=0;
    int nn=0;
    for(double x0=a;x0<b;x0=x0+eps){
        sumR=sumR+f_ot_x(x0+eps/2);
        nn++;
    }
    sumR=sumR*eps;

    f(nn);
    return sumR; //
}

double for_IntRectrap(double nn){
    printf("%lf\n",nn);
}

//рассчет оределенного интеграла, классика
double integr(double a,double b){
    return (double)(fabs(pow(b,3)-pow(a,3))/(double)(3));
}
double EpsMas[]={0.01, 0.001, 0.0001, 0.00001, 0.000001};


//dx=(a-b)/n
//Рассчет шага, i1,i2 - уже полученные интегралы со своими шагами
double Epsilon(double i1, double i2,int epsnom){
    int r;
    i1= IntRect();
    while ((fabs(i1-i2)/3)>EpsMas[epsnom]){
        i1=i2;
        //призыв нового исчисления интегралов, но с делением дельта на 2
    } //эпсилон в шаге dx/2; epsnom - номер точности в массиве

}


int IntTrap(TPF f, double a, double b, double eps, int* n){
    double summT=0;
    int nn=0;
    for(double x0=a;x0<b;x0=x0+eps){
        summT=summT+(f_ot_x(x0)+ f_ot_x(x0+eps))/2;
        nn++;
    }
    summT=summT*eps;
    f(summT);
    return nn;
}
int main() {
    setlocale(LC_ALL, "Russian");
    ItogPrint();
    printf("\n");

    int n[]={0,0,0,0};

    int Nmain=1;

    printf("классика %lf\n", integr(1,3));
    printf("pr %d\n", IntRect(for_IntRectrap,1,3,0.1,n));
    printf("tr %d\n", IntTrap(for_IntRectrap,1,3,0.1,n));


    return 0;
}
