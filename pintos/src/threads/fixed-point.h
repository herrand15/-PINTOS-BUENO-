#include <stdint.h>

const static int LAST_ADDRESS = 16384;

int multiplication(int a, int b);
int division(int a, int b);
int addition(int a, int b);
int to_fixed_pointer(int a);
int tointfloor(int a);
int round_to_int(int a);
int fixed_add(int a, int b);
int fixed_sub(int a, int b);
int fixed_mul(int a, int b);
int fixed_div(int a, int b);


/* Aritmetica normal */
int multiplication(int a, int b)
{
	return a * b;
}

int division(int a, int b)
{
	return a / b;
}

int addition(int a, int b)
{
	return a + b;
}

/* Transformacion a puntero fijo */
int to_fixed_pointer(int a)
{
	return a * LAST_ADDRESS;
}

/*  Transformacion de puntero a entero fijo*/
int tointfloor(int a)
{
	return a / LAST_ADDRESS;
}

/* Rendodear entero */
int round_to_int(int a)
{
	return (a >= 0) ?  ((a + LAST_ADDRESS/2) / LAST_ADDRESS) : ((a - LAST_ADDRESS/2) / LAST_ADDRESS);
}

/* Aritmetica de punteros */
int fixed_add(int a, int b)
{
	return a + (b * LAST_ADDRESS);
}

int fixed_sub(int a, int b)
{
	return a - (b * LAST_ADDRESS);
}

/* Multiplicacion */
int fixed_mul(int a, int b)
{
	return ((int64_t) a) * b / LAST_ADDRESS;
}


/* Division */
int fixed_div(int a, int b)
{
	return ((int64_t) a) * LAST_ADDRESS / b;
} 
