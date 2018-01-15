#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


#ifndef PICTURE
#define PICTURE
typedef struct {
    char name[256];
    int width, height;
    unsigned short int grey_value;
    unsigned short int **tab;
    unsigned short int standard;	//acceptatble only p2, p5 for pgm
} picture;
#endif

double *picture_histogram_array(picture * const);
unsigned short int **picture_histogram_array_2D(picture * const);
double array_biggest_double(double *, int);
const picture *picture_add(char *);
void picture_save(picture * const, const char *);
void picture_mirror_0Y_axis(picture * const);
void picture_mirror_0X_axis(picture * const);
void picture_negative(picture * const);
void picture_rotate_clockwise(picture * const);
void picture_delete(picture *);
void picture_mediana_filter(picture * const);
void picture_salt_pepper_hum(picture * const);
void picture_proging(picture * const, int new_grey_value);
void picture_resize(picture * const, int);
void picture_generic_filter(picture * const, bool);	//if bool==true is gausian
picture *create_histogram_pgm(picture * picture_ptr, int);
void picture_copy(picture *, picture *);
void array_2D_copy(unsigned short int **, unsigned short int **, double,
		   double);
void array_2D_resize(unsigned short int **, double, double);
