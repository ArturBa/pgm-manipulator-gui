#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#pragma pack(1)

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

const picture *picture_add(char *name) {
    picture *const picture_ptr = malloc(sizeof *picture_ptr);;
    FILE *file_ptr;
    int i, j;
    unsigned short int **picture_tab;
    strcpy(picture_ptr->name, name);
    if (picture_ptr == NULL) {
	exit(1);
    }
    file_ptr = fopen(picture_ptr->name, "r");
    if (file_ptr == NULL)
	fprintf(stderr, "\nERROR. Cannot open file. Check file name.\n");

    while (1) {			//reading standard of file
	char buffor_c = fgetc(file_ptr);

	if (buffor_c == '#') {
	    while (fgetc(file_ptr) == '\n');
	    continue;
	}
	if (buffor_c == 'P') {
	    buffor_c = fgetc(file_ptr);
	    picture_ptr->standard = (unsigned short int) atoi(&buffor_c);
	    break;
	}
    }
    while (1) {			//reading width of picture
	int buffor_i;
	if (fscanf(file_ptr, "%d", &buffor_i) == 1) {
	    picture_ptr->width = buffor_i;
	    break;
	} else {
	    while (fgetc(file_ptr) != '\n')
		continue;
	}
    }

    while (1) {			//reading height of picture
	int buffor_i;
	if (fscanf(file_ptr, "%d", &buffor_i) == 1) {
	    picture_ptr->height = buffor_i;
	    break;
	} else {
	    while (fgetc(file_ptr) != '\n')
		continue;
	}
    }

    while (1) {			//reading greyscale max
	int buffor_i;
	if (fscanf(file_ptr, "%d", &buffor_i) == 1) {
	    picture_ptr->grey_value = buffor_i;
	    break;
	} else {
	    while (fgetc(file_ptr) != '\n')
		continue;
	}
    }
    picture_tab = malloc(picture_ptr->height * sizeof(*picture_tab));
    if (picture_tab == NULL) {
	fprintf(stderr, "ERROR. Allocation memory problem\n");
	exit(1);
    }
    for (i = 0; i < picture_ptr->height; i++) {
	*(picture_tab + i) =
	    malloc(picture_ptr->width * (sizeof(**picture_tab)));
	if (*(picture_tab + i) == NULL) {
	    for (; i >= 0; i--) {
		free(picture_tab + i);
	    }
	    fprintf(stderr, "ERROR. Allocation memory problem\n");
	    exit(1);
	}
    }

    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width;) {
	    if (fscanf(file_ptr, "%hd", (*(picture_tab + i) + j)) == 1) {
		j++;
	    } else {
		while (fgetc(file_ptr) != '\n');
	    }
	}
    }

    fclose(file_ptr);
    picture_ptr->tab = picture_tab;
    return picture_ptr;
}

void picture_delete(picture * picture_ptr) {
    int j;
    for (j = 0; j < (picture_ptr->height); j++) {
	free(*(picture_ptr->tab + j));
    }
    free(picture_ptr->tab);
    free(picture_ptr);
}

void picture_save(picture * picture_ptr, const char *file_name) {
    int i, j;
    FILE *file_ptr = NULL;

    file_ptr = fopen(file_name, "w");
    if (file_ptr == NULL) {
	fprintf(stderr, "ERROR. File open problem\n");
	exit(1);
    }

    fprintf(file_ptr, "P%hd\n#Edited w/ A.B.graph\n%hd %hd\n%hd\n",
	    picture_ptr->standard, picture_ptr->width, picture_ptr->height,
	    picture_ptr->grey_value);

    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width; j++) {
	    fprintf(file_ptr, "%hd ", picture_ptr->tab[i][j]);
	}
	fprintf(file_ptr, "\n");
    }
    fclose(file_ptr);
}

void picture_mirror_0Y_axis(picture * const picture_ptr) {
    short int buffor, i, j;
    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width / 2; j++) {
	    buffor = *(*(picture_ptr->tab + i) + j);
	    *(*(picture_ptr->tab + i) + j) =
		*(*(picture_ptr->tab + i) + picture_ptr->width - j - 1);
	    *(*(picture_ptr->tab + i) + picture_ptr->width - j - 1) =
		buffor;
	}
    }
}

void picture_mirror_0X_axis(picture * const picture_ptr) {
    short int buffor, i, j;
    for (i = 0; i < picture_ptr->height / 2; i++) {
	for (j = 0; j < picture_ptr->width; j++) {
	    buffor = *(*(picture_ptr->tab + i) + j);
	    *(*(picture_ptr->tab + i) + j) =
		*(*(picture_ptr->tab + picture_ptr->height - i - 1) + j);
	    *(*(picture_ptr->tab + picture_ptr->height - i - 1) + j) =
		buffor;
	}
    }
}

void picture_negative(picture * const picture_ptr) {
    short int i, j;
    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width; j++) {
	    *(*(picture_ptr->tab + i) + j) =
		abs((*(*(picture_ptr->tab + i) + j) >
		     picture_ptr->grey_value) ? (picture_ptr->grey_value)
		    : (*(*(picture_ptr->tab + i) + j)) -
		    picture_ptr->grey_value);
	}
    }
}

void picture_proging(picture * const picture_ptr, int grey_value_new) {
    short int i, j;

    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width; j++) {
	    picture_ptr->tab[i][j] =
		picture_ptr->tab[i][j] >
		grey_value_new ? grey_value_new : picture_ptr->tab[i][j];
	}
    }
    picture_ptr->grey_value = grey_value_new;
}

void picture_rotate_clockwise(picture * const picture_ptr) {
    unsigned short int **const picture_tab =
	malloc(picture_ptr->width * sizeof(*picture_tab));
    unsigned short int i, j;
    int buffor;

    if (picture_tab == NULL) {
	fprintf(stderr, "ERROR. Allocation memory problem\n");
	getchar();
	exit(1);
    }
    for (i = 0; i < picture_ptr->width; i++) {
	*(picture_tab + i) =
	    malloc(picture_ptr->height * (sizeof(**picture_tab)));
	if (*(picture_tab + i) == NULL) {
	    fprintf(stderr, "ERROR. Allocation memory problem\n");
	    getchar();
	    exit(1);
	}
    }

    for (i = 0; i < picture_ptr->width; i++) {
	for (j = 0; j < picture_ptr->height; j++) {
	    *(*(picture_tab + i) + j) =
		*(*(picture_ptr->tab + picture_ptr->height - j - 1) + i);
	}
    }

    for (i = 0; i < picture_ptr->height; i++)
	free(*(picture_ptr->tab + i));
    free(picture_ptr->tab);

    picture_ptr->tab = picture_tab;
    buffor = picture_ptr->width;
    picture_ptr->width = picture_ptr->height;
    picture_ptr->height = buffor;
}

int compare_short_int(const void *a, const void *b) {
    if (*(short unsigned int *) a > *(short unsigned int *) b)
	return 1;
    else if (*(short unsigned int *) a < *(short unsigned int *) b)
	return -1;
    else
	return 0;
}

void picture_mediana_filter(picture * const picture_ptr) {
    unsigned short int **picture_tab;
    unsigned short int i, j, k, l, *mediana_tab, mediana_filter_size;

    while (1) {
	printf
	    ("Type in mediana filter size (odd and more than 1)\nType here: ");
	if (scanf("%hd", &mediana_filter_size) == 1) {
	    if (mediana_filter_size % 2 && mediana_filter_size > 1)
		break;
	}
	while (getchar() != '\n');
    }
    mediana_tab =
	malloc(mediana_filter_size * mediana_filter_size *
	       sizeof *mediana_tab);


    picture_tab =
	malloc((picture_ptr->height -
		2 * (mediana_filter_size / 2)) * sizeof(*picture_tab));
    if (picture_tab == NULL) {
	fprintf(stderr, "ERROR. Allocation memory problem\n");
	getchar();
	exit(1);
    }
    for (i = 0; i < picture_ptr->height - 2 * (mediana_filter_size / 2);
	 i++) {
	*(picture_tab + i) =
	    malloc((picture_ptr->width -
		    2 * (mediana_filter_size / 2)) *
		   (sizeof(**picture_tab)));
	if (*(picture_tab + i) == NULL) {
	    fprintf(stderr, "ERROR. Allocation memory problem\n");
	    getchar();
	    exit(1);
	}
    }

    for (i = 0; i < picture_ptr->height - 2 * (mediana_filter_size / 2);
	 i++) {
	for (j = 0; j < picture_ptr->width - 2 * (mediana_filter_size / 2);
	     j++) {
	    for (k = 0; k < mediana_filter_size; k++) {
		for (l = 0; l < mediana_filter_size; l++) {
		    *(mediana_tab + mediana_filter_size * k + l) =
			*(*(picture_ptr->tab + i + k) + j + k);
		}
	    }
	    qsort(mediana_tab, mediana_filter_size * mediana_filter_size,
		  sizeof *mediana_tab, compare_short_int);
	    *(*(picture_tab + i) + j) =
		mediana_tab[mediana_filter_size * mediana_filter_size / 2];
	}
    }

    for (i = 0; i < picture_ptr->height; i++)
	free(*(picture_ptr->tab + i));
    free(picture_ptr->tab);

    picture_ptr->tab = picture_tab;
    picture_ptr->width -= 2 * (mediana_filter_size / 2);
    picture_ptr->height -= 2 * (mediana_filter_size / 2);
}


void picture_salt_pepper_hum(picture * const picture_ptr) {
    short unsigned int buffor, i, j;
    srand(time(NULL));

    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width; j++) {
	    buffor = rand() % 101;
	    if (buffor == 0)
		*(*(picture_ptr->tab + i) + j) = 0;
	    else if (buffor == 100)
		*(*(picture_ptr->tab + i) + j) = picture_ptr->grey_value;
	    else
		continue;
	}
    }
}

void picture_resize(picture * const picture_ptr, int new_value) {
    int new_width = new_value, new_height = 0, i, j, x, y;
    double width_factor, height_factor;
    unsigned short int **picture_new_tab;

    if ((double) new_width / picture_ptr->width * picture_ptr->height >
	new_value) {
	new_height = new_value;
	new_width = (double)new_value / picture_ptr->height * picture_ptr->width;
    } else {
	new_height =
	    (double) new_width / picture_ptr->width * picture_ptr->height;
    }
    picture_new_tab = malloc(sizeof(*picture_new_tab) * new_height);
    if (picture_new_tab == NULL) {
	fprintf(stderr, "ERROR. Memory allocation problem.");
	getchar();
	exit(1);
    }

    for (i = 0; i < new_height; i++) {
	*(picture_new_tab + i) =
	    malloc(sizeof(**picture_new_tab) * new_width);
	if (*(picture_new_tab + i) == NULL) {
	    fprintf(stderr, "ERROR. Memory allocation problem.\n");
	    getchar();
	    exit(1);
	}
    }

    width_factor = (double) picture_ptr->width / new_width;
    height_factor = (double) picture_ptr->height / new_height;

    for (i = 0; i < new_height; i++) {
	for (j = 0; j < new_width; j++) {
	    y = floor(i * height_factor);
	    x = floor(j * width_factor);
	    double x_part_floor =
		j * width_factor - floor(j * width_factor), y_part_floor =
		i * height_factor - floor(i * height_factor);
	    if (x < 0)
		x = 0;
	    if (y < 0)
		y = 0;
	    if (y > (picture_ptr->height - 2))
		y = picture_ptr->height - 2;
	    if (x > (picture_ptr->width - 2))
		x = picture_ptr->width - 2;

	    picture_new_tab[i][j] =
		picture_ptr->tab[y][x] * x_part_floor * y_part_floor +
		picture_ptr->tab[y][x + 1] * (1 -
					      x_part_floor) *
		y_part_floor + picture_ptr->tab[y + 1][x] * (1 -
							     y_part_floor)
		* x_part_floor + picture_ptr->tab[y + 1][x + 1] * (1 -
								   x_part_floor)
		* (1 - y_part_floor);
	}
    }

    for (i = 0; i < picture_ptr->height; i++) {
	free(picture_ptr->tab[i]);
    }
    free(picture_ptr->tab);

    picture_ptr->height = new_height;
    picture_ptr->width = new_width;
    picture_ptr->tab = picture_new_tab;
}

double *picture_histogram_array(picture * const picture_ptr) {
    int i, j;
    double *const histogram_array =
	calloc(picture_ptr->grey_value + 1, sizeof(*histogram_array));

    for (i = 0; i < picture_ptr->height; i++) {
	for (j = 0; j < picture_ptr->width; j++) {
	    histogram_array[(*(*(picture_ptr->tab + i) + j)) >
			    (picture_ptr->grey_value +
			     1) ? picture_ptr->grey_value +
			    1 : *(*(picture_ptr->tab + i) + j)]++;
	}
    }
    return histogram_array;
}

void picture_generic_filter(picture * const picture_ptr, bool is_gausian) {
    short int filter[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 }, i, j;
    int buffor;
    unsigned short int **const picture_tab =
	malloc((picture_ptr->height - 2) * sizeof(*picture_tab));

    if (picture_tab == NULL) {
	fprintf(stderr, "ERROR. Allocation memory problem\n");
	getchar();
	exit(1);
    }
    for (i = 0; i < picture_ptr->height - 2; i++) {
	*(picture_tab + i) =
	    malloc((picture_ptr->width - 2) * (sizeof(**picture_tab)));
	if (*(picture_tab + i) == NULL) {
	    fprintf(stderr, "ERROR. Allocation memory problem\n");
	    getchar();
	    exit(1);
	}
    }
    if (is_gausian == false) {
	do {
	    for (i = 0; i < 9; i++) {
		printf("Type in %d:%d filter value: ", i / 3 + 1,
		       i % 3 + 1);
		if (scanf("%hd", &filter[i]) == 1)
		    continue;
		else {
		    i--;
		    continue;
		}
	    }
	}
	while (filter[0] + filter[1] + filter[2] + filter[3] + filter[4] +
	       filter[5] + filter[6] + filter[7] + filter[8] == 0);
    }

    for (i = 1; i < picture_ptr->height - 1; i++) {
	for (j = 1; j < picture_ptr->width - 1; j++) {
	    buffor = 0;
	    buffor += picture_ptr->tab[i - 1][j - 1] * filter[0];
	    buffor += picture_ptr->tab[i][j - 1] * filter[1];
	    buffor += picture_ptr->tab[i + 1][j - 1] * filter[2];
	    buffor += picture_ptr->tab[i - 1][j] * filter[3];
	    buffor += picture_ptr->tab[i][j] * filter[4];
	    buffor += picture_ptr->tab[i + 1][j] * filter[5];
	    buffor += picture_ptr->tab[i - 1][j + 1] * filter[6];
	    buffor += picture_ptr->tab[i][j + 1] * filter[7];
	    buffor += picture_ptr->tab[i + 1][j + 1] * filter[8];
	    *(*(picture_tab + i - 1) + j - 1) =
		buffor / (filter[0] + filter[1] + filter[2] + filter[3] +
			  filter[4] + filter[5] + filter[6] + filter[7] +
			  filter[8]);
	}
    }

    for (i = 0; i < picture_ptr->height; i++)
	free(*(picture_ptr->tab + i));
    free(picture_ptr->tab);

    picture_ptr->tab = picture_tab;
    picture_ptr->width -= 2;
    picture_ptr->height -= 2;
}
double array_biggest_double(const double *const array, int array_size) {
    unsigned int i;
    double max_value = 0;

    for (i = 0; i < array_size; i++) {
	if (array[i] > max_value) {
	    max_value = array[i];
	    continue;
	} else
	    continue;
    }
    return max_value;
}
unsigned short int **picture_histogram_array_2D(picture *
						const picture_ptr,
						int height) {
    unsigned int i = 0, j = 0;
    const double *const histogram_array =
	picture_histogram_array(picture_ptr);
    const double max_hist_value =
	array_biggest_double(histogram_array, picture_ptr->grey_value + 1);
    unsigned short int **const array_1 = malloc(sizeof(*array_1) * height);
    if (array_1 == NULL) {
	fprintf(stderr, "ERROR. Memory allocation failed.");
	exit(1);
    }

    for (i = 0; i < height; i++) {
	*(array_1 + i) = calloc(sizeof(**array_1), 256);
	if (array_1[i] == NULL) {
	    fprintf(stderr, "ERROR. Memory allocation problem\n.");
	    exit(1);
	}
    }

    for (i = 0; i <= picture_ptr->grey_value; i++) {
	for (j = 0; j < height; j++) {
	    if ((double) j / height <
		(double) histogram_array[i] / max_hist_value) {
		array_1[height - 1 - j][i] = 1;
	    } else {
		break;
	    }
	}
    }
    return array_1;
}

picture *create_histogram_pgm(picture * picture_ptr, int height) {
    picture *pic_ptr = calloc(1, sizeof *pic_ptr);
    strcpy(pic_ptr->name, "histogram.pgm");
    pic_ptr->width = 256;
    pic_ptr->height = height;
    pic_ptr->grey_value = 1;
    pic_ptr->tab = picture_histogram_array_2D(picture_ptr, height);
    pic_ptr->standard = 2;
    return pic_ptr;
}

void picture_array_2D_resize(picture * dest, double height, double width) {
    int i = 0;
    if (dest->tab != NULL) {
	for (i = 0; i < dest->height; i++)
	    free(dest->tab[i]);
	free(dest->tab);
    }
    dest->tab = calloc(height, sizeof *(dest->tab));
    for (i = 0; i < height; i++)
	dest->tab[i] = calloc(width, sizeof **(dest->tab));
}

void array_2D_copy(unsigned short int **dest, unsigned short int **src,
		   double height, double width) {
    int i, j;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++)
	    dest[i][j] = src[i][j];
    }
}

void picture_copy(picture * dest, picture * src) {
    picture_array_2D_resize(dest, src->height, src->width);
    dest->grey_value = src->grey_value;
    dest->width = src->width;
    dest->height = src->height;
    strcpy(dest->name, src->name);
    dest->standard = src->standard;
    array_2D_copy(dest->tab, src->tab, src->height, src->width);
}
