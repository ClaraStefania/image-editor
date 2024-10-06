// Copyright Stefania-Clara Diaconescu 313CAb 2023-2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100
#define I_MAX 255

typedef struct {
	int r; // rosu
	int v; // verde
	int a; // albastru
} mat_col;

typedef struct {
	mat_col **mat;
	int x1, x2, y1, y2; // coordonate
	int lin, col; // dimensiuni
} imagine;

// Functia aloca memorie pentru matrice.
void aloc_mat(mat_col ***mat, int m, int n)
{
	int i, j;

	(*mat) = malloc(m * sizeof(mat_col *));

	if (!(*mat)) {
		printf("Nu s-a putut aloca memorie\n");
		free(*mat);
		*mat = NULL;
		return;
	}

	// Daca se poate aloca matricea, se aloca fiecare linie in parte.
	// Daca o linie nu se poate aloca, se elibereaza toate cele deja alocate.

	for (i = 0; i < m; i++) {
		(*mat)[i] = malloc(n * sizeof(mat_col));

		if (!(*mat)[i]) {
			printf("Nu s-a putut aloca memorie\n");
			for (j =  0; j < i; j++)
				free((*mat)[j]);
			free(*mat);
			*mat = NULL;
			return;
		}
	}
}

// Functia elibereaza memoria alocata dinamic pentru o matrice.
void eliberare_mat(mat_col **mat, int m)
{
	int i;

	// Se elibereaza liniile, apoi matricea.

	for (i = 0; i < m; i++)
		free(mat[i]);
	free(mat);
}

// Functia afiseaza matricea in format ASCII
void afisare_ascii(mat_col **mat, int m, int n, char tip[4], FILE *out)
{
	int i, j;

	// Daca matricea e color, se afiseaza valorile pentru fiecare culoare.
	// Altfel, se afiseaza doar prima valoare, folosita in cazul in care
	// matricea este grayscale/alb-negru.

	if (strcmp(tip, "P3") == 0) {
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				fprintf(out, "%d ", mat[i][j].r);
				fprintf(out, "%d ", mat[i][j].v);
				fprintf(out, "%d ", mat[i][j].a);
			}
			fprintf(out, "\n");
		}
	} else {
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++)
				fprintf(out, "%d ", mat[i][j].r);
			fprintf(out, "\n");
		}
	}
}

// Functia afiseaza matricea in format binar
void afisare_binar(mat_col **mat, int m, int n, char tip[4], FILE *out)
{
	int i, j;

	// Daca matricea e color, se afiseaza valorile pentru fiecare culoare.
	// Altfel, se afiseaza doar prima valoare, folosita in cazul in care
	// matricea este grayscale/alb-negru.

	if (strcmp(tip, "P6") == 0) {
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++) {
				fwrite(&mat[i][j].r, sizeof(unsigned char), 1, out);
				fwrite(&mat[i][j].v, sizeof(unsigned char), 1, out);
				fwrite(&mat[i][j].a, sizeof(unsigned char), 1, out);
			}
	} else {
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				fwrite(&mat[i][j].r, sizeof(unsigned char), 1, out);
	}
}

// Functia citeste matricea de tip P1/P2
void citire_p1_p2(imagine *imag, FILE *fisier)
{
	int i, j;

	for (i = 0; i < imag->lin; i++)
		for (j = 0; j < imag->col; j++)
			fscanf(fisier, "%d", &imag->mat[i][j].r);
}

// Functia citeste matricea de tip P3
void citire_p3(imagine *imag, FILE *fisier)
{
	int i, j;

	for (i = 0; i < imag->lin; i++) {
		for (j = 0; j < imag->col; j++) {
			fscanf(fisier, "%d", &imag->mat[i][j].r);
			fscanf(fisier, "%d", &imag->mat[i][j].v);
			fscanf(fisier, "%d", &imag->mat[i][j].a);
		}
	}
}

// Functia citeste matricea de tip P4/P5
void citire_p4_p5(imagine *imag, FILE *fisier)
{
	int i, j;
	unsigned char c;

	for (i = 0; i < imag->lin; i++)
		for (j = 0; j < imag->col; j++) {
			fread(&c, sizeof(unsigned char), 1, fisier);
			imag->mat[i][j].r = (int)c;
		}
}

// Functia citeste matricea de tip P6
void citire_p6(imagine *imag, FILE *fisier)
{
	int i, j;
	unsigned char c;

	for (i = 0; i < imag->lin; i++) {
		for (j = 0; j < imag->col; j++) {
			fread(&c, sizeof(unsigned char), 1, fisier);
			imag->mat[i][j].r = (int)c;
			fread(&c, sizeof(unsigned char), 1, fisier);
			imag->mat[i][j].v = (int)c;
			fread(&c, sizeof(unsigned char), 1, fisier);
			imag->mat[i][j].a = (int)c;
		}
	}
}

// Functia selecteaza intreaga imagine
void selectare_intreg(imagine *imag, int ok)
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}

	imag->x1 = 0;
	imag->x2 = imag->col;
	imag->y1 = 0;
	imag->y2 = imag->lin;
	printf("Selected ALL\n");
}

// Functia schimba intre ele valorile a doua numere
void interschimbare(int *x, int *y)
{
	int aux;

	aux = *x;
	*x = *y;
	*y = aux;
}

// Functia selecteaza intervalele pe care vor fi prelucrati pixelii
void selectare_imag(imagine *imag, int ok, int x1)
{
	int x2, y1, y2;
	char *s = (char *)malloc(MAX * sizeof(char));
	if (!s) {
		printf("Nu s-a putut aloca memorie\n");
		return;
	}

	fgets(s, MAX, stdin);

	if (ok == 0) {
		printf("No image loaded\n");
		free(s);
		return;
	}

	if (sscanf(s, "%d%d%d", &y1, &x2, &y2) != 3) {
		// In cazul in care nu sunt inca 3 numere in comanda, afiseaza mesajul
		// si iese din functie.
		printf("Invalid command\n");
		free(s);
		return;
	}

	int n = strlen(s), nr = 0, i;

	for (i = 0; i < n; i++) {
		if (s[i] == ' ')
			nr++; // numara cate spatii se afla in comanda
	}

	if (nr != 3) {
		// Daca sunt mai multe spatii (mai multe cuvinte) in comanda, afiseaaza
		// mesajul si iese din functie.
		printf("Invalid command\n");
		free(s);
		return;
	}

	// Verifica daca sunt valide coordonatele citite.

	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) {
		printf("Invalid set of coordinates\n");
		free(s);
		return;
	}

	if (x1 > imag->col || x2 > imag->col || y1 > imag->lin || y2 > imag->lin) {
		printf("Invalid set of coordinates\n");
		free(s);
		return;
	}

	if (x1 == x2 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		free(s);
		return;
	}

	if (x1 > x2)
		interschimbare(&x1, &x2);
	if (y1 > y2)
		interschimbare(&y1, &y2);

	imag->x1 = x1;
	imag->x2 = x2;
	imag->y1 = y1;
	imag->y2 = y2;

	printf("Selected %d %d %d %d\n", imag->x1, imag->y1, imag->x2, imag->y2);
	free(s);
}

// Functia sare peste comentarii
void verif_com(FILE *fisier)
{
	char *comentariu, c;

	comentariu = (char *)malloc(MAX * sizeof(char));
	if (!comentariu) {
		printf("Nu s-a putut aloca memorie\n");
		return;
	}

	fscanf(fisier, "%c", &c);

	// Cat timp primul caracter este "#", citeste linia si trece la urmatoarea.

	while (c == '#') {
		fgets(comentariu, MAX, fisier);
		fscanf(fisier, " %c", &c);
	}

	fseek(fisier, -1, SEEK_CUR);
	free(comentariu);
}

// Functia incarca matricea citita
void incarcare(char *nume_fisier, imagine *imag, int *ok, char tip[4],
			   int *int_max)
{
	FILE *fisier = fopen(nume_fisier, "rb");

	if (!fisier) {
		if (*ok == 1)
			eliberare_mat(imag->mat, imag->lin);
		printf("Failed to load %s\n", nume_fisier);
		*ok = 0;
		return;
	}

	// Daca era o matrice incarcata, o elibereaza.
	if (*ok == 1)
		eliberare_mat(imag->mat, imag->lin);

	*ok = 1;

	printf("Loaded %s\n", nume_fisier);
	fscanf(fisier, "%s", tip);

	verif_com(fisier); // Sare peste comentarii.

	fscanf(fisier, "%d%d\n", &imag->col, &imag->lin);
	aloc_mat(&imag->mat, imag->lin, imag->col);

	if (strstr(tip, "P2") || strstr(tip, "P3"))
		fscanf(fisier, "%d\n", int_max);
	if (strstr(tip, "P5") || strstr(tip, "P6"))
		fscanf(fisier, "%d\n", int_max);

	// Verifica tipul si apeleaza citirea corespunzatoare.

	if (strstr(tip, "P1") || strstr(tip, "P2"))
		citire_p1_p2(imag, fisier);
	else if (strstr(tip, "P3"))
		citire_p3(imag, fisier);
	else if (strstr(tip, "P4") || strstr(tip, "P5"))
		citire_p4_p5(imag, fisier);
	else if (strstr(tip, "P6"))
		citire_p6(imag, fisier);

	imag->x1 = 0;
	imag->x2 = imag->col;
	imag->y1 = 0;
	imag->y2 = imag->lin;

	fclose(fisier);
}

// Functia construiste histograma.
void histograma(imagine *imag, int ok, char tip[4])
{
	int x, y, i;
	char *s = (char *)malloc(MAX * sizeof(char));
	if (!s) {
		printf("Nu s-a putut aloca memorie\n");
		return;
	}
	fgets(s, MAX, stdin);
	if (ok == 0) {
		printf("No image loaded\n");
		free(s);
		return;
	}
	if (sscanf(s, "%d%d", &x, &y) != 2) {
		printf("Invalid command\n"); // Verifica daca se pot citi inca 2 numere
		free(s);
		return;
	}
	int n = strlen(s), nr = 0;
	for (i = 0; i < n; i++)
		if (s[i] == ' ')
			nr++;
	if (nr != 2) {
		// Verifica daca sunt mai multe spatii (cuvinte) in comanda.
		printf("Invalid command\n");
		free(s);
		return;
	}
	if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {
		printf("Black and white image needed\n");
		free(s);
		return;
	}
	int *frecv = (int *)malloc((I_MAX + 1) * sizeof(int));
	if (!frecv) {
		printf("Nu s-a putut aloca memorie\n");
		return;
	}
	int *nr_stele = (int *)malloc((I_MAX + 1) * sizeof(int));
	if (!nr_stele) {
		printf("Nu s-a putut aloca memorie\n");
		free(frecv);
		return;
	}
	int j, maxim = 0, grup = 256 / y;
	double suma_frecv = 0;
	for (i = 0; i <= I_MAX; i++) {
		frecv[i] = 0;
		nr_stele[i] = 0;
	}
	for (i = 0; i < imag->lin; i++)
		for (j = 0; j < imag->col; j++)
			frecv[imag->mat[i][j].r]++; // Calculeaza frecventele valorilor
	// Afla maximul numarului de frecvente
	for (i = 0; i <= I_MAX; i++) {
		suma_frecv += frecv[i];
		if ((i + 1) % grup == 0) {
			if (suma_frecv > maxim)
				maxim = suma_frecv;
			suma_frecv = 0;
		}
	}
	// Afla numarul de stelute pentru fiecare bin
	j = 0;
	for (i = 0; i <= I_MAX; i++) {
		suma_frecv += frecv[i];
		if ((i + 1) % grup == 0) {
			nr_stele[j] = (int)(suma_frecv * x / maxim);
			j++;
			suma_frecv = 0;
		}
	}
	for (i = 0; i < y; i++) {
		printf("%d | ", nr_stele[i]);
		for (j = 0; j < nr_stele[i]; j++)
			printf("*");
		printf("\n");
	}
	free(s); free(frecv); free(nr_stele);
}

// Functia incadreaza valorile intre un minim si maxim date
double clamp(double x, int minim, int maxim)
{
	if (x < minim)
		return minim;
	else if (x > maxim)
		return maxim;
	return x;
}

// Functia egalizeaza imaginea
void egalizare(imagine *imag, char tip[4], int ok)
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}

	if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}

	double *frecv = (double *)malloc((I_MAX + 1) * sizeof(double));
	if (!frecv) {
		printf("Nu s-a putut aloca memorie\n");
		return;
	}

	double *f = (double *)malloc((I_MAX + 1) * sizeof(double));
	if (!f) {
		printf("Nu s-a putut aloca memorie\n");
		free(frecv);
		return;
	}

	int i, j;

	for (i = 0; i <= I_MAX; i++)
		frecv[i] = 0;

	// Calculeaza frecventele valorilor
	for (i = 0; i < imag->lin; i++)
		for (j = 0; j < imag->col; j++)
			frecv[imag->mat[i][j].r]++;

	// Calculeaza noile valori ale pixelilor
	for (i = 0; i <= I_MAX; i++) {
		double suma = 0;
		for (j = 0; j <= i; j++)
			suma += frecv[j];
		f[i] = I_MAX * suma / (imag->lin * imag->col);
	}

	for (i = 0; i < imag->lin; i++)
		for (j = 0; j < imag->col; j++)
			imag->mat[i][j].r = round(clamp(f[imag->mat[i][j].r], 0, I_MAX));

	printf("Equalize done\n");
	free(frecv);
	free(f);
}

// Functia calculeaza noua matrice dupa aplicarea nucleului
void aplicare_parametru(imagine *imag, double efect[3][3])
{
	mat_col **aux;
	int i, j, k, l;
	double suma_r = 0, suma_v = 0, suma_a = 0;

	int x1 = imag->x1;
	int x2 = imag->x2;
	int y1 = imag->y1;
	int y2 = imag->y2;

	if (x1 == 0)
		x1 = 1;
	if (x2 == imag->col)
		x2--;
	if (y1 == 0)
		y1 = 1;
	if (y2 == imag->lin)
		y2--;

	aloc_mat(&aux, y2 - y1, x2 - x1);

	// Calculeaza noile valori ale pixelilor
	for (i = y1; i < y2; i++)
		for (j = x1; j < x2; j++) {
			suma_r = 0;
			suma_v = 0;
			suma_a = 0;

			for (k = i - 1; k <= i + 1; k++) {
				for (l = j - 1; l <= j + 1; l++) {
					int k2 = k - (i - 1);
					int l2 = l - (j - 1);
					suma_r += efect[k2][l2] * imag->mat[k][l].r;
					suma_v += efect[k2][l2] * imag->mat[k][l].v;
					suma_a += efect[k2][l2] * imag->mat[k][l].a;
				}
			}

			aux[i - y1][j - x1].r = round(clamp(suma_r, 0, I_MAX));
			aux[i - y1][j - x1].v = round(clamp(suma_v, 0, I_MAX));
			aux[i - y1][j - x1].a = round(clamp(suma_a, 0, I_MAX));
		}

	// Inlocuieste valorile in matrice
	for (i = y1; i < y2; i++)
		for (j = x1; j < x2; j++) {
			imag->mat[i][j].r = aux[i - y1][j - x1].r;
			imag->mat[i][j].v = aux[i - y1][j - x1].v;
			imag->mat[i][j].a = aux[i - y1][j - x1].a;
		}

	eliberare_mat(aux, y2 - y1);
}

void edge(imagine *imag)
{
	int i, j;
	double efect[3][3];

	// Matricea specifica filtrului edge.
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			efect[i][j] = -1;
	efect[1][1] = 8;

	aplicare_parametru(imag, efect);
}

void sharpen(imagine *imag)
{
	double efect[3][3];

	// Matricea specifica filtrului sharpen.
	efect[0][0] = 0;
	efect[0][1] = -1;
	efect[0][2] = 0;
	efect[1][0] = -1;
	efect[1][1] = 5;
	efect[1][2] = -1;
	efect[2][0] = 0;
	efect[2][1] = -1;
	efect[2][2] = 0;

	aplicare_parametru(imag, efect);
}

void blur(imagine *imag)
{
	int i, j;
	double efect[3][3];

	// Matricea specifica filtrului blur.
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			efect[i][j] = 1.0 / 9;

	aplicare_parametru(imag, efect);
}

void gaussian_blur(imagine *imag)
{
	double efect[3][3];
	int i, j;

	// Matricea specifica filtrului gaussian_blur.
	efect[0][0] = 1.0;
	efect[0][1] = 2.0;
	efect[0][2] = 1.0;
	efect[1][0] = 2.0;
	efect[1][1] = 4.0;
	efect[1][2] = 2.0;
	efect[2][0] = 1.0;
	efect[2][1] = 2.0;
	efect[2][2] = 1.0;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			efect[i][j] /= 16;

	aplicare_parametru(imag, efect);
}

// Functia aplica nucleele de imagine
void aplicare(imagine *imag, int ok, char tip[4])
{
	char parametru[MAX] = "", parametru2[MAX] = "", c;

	scanf("%c", &c);

	if (ok == 0) {
		printf("No image loaded\n");

		// Daca nu este nicio imagine incarcata, sare peste parametru
		if (c == '\n')
			return;

		parametru[0] = c;
		scanf("%s", parametru2);
		strcat(parametru, parametru2);

		return;
	}

	if (c == '\n') {
		printf("Invalid command\n");
		return;
	}

	parametru[0] = c;
	scanf("%s", parametru2);
	strcat(parametru, parametru2);

	if (strcmp(tip, "P2") == 0 || strcmp(tip, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	// In functie de ce parametru este citit, se apeleaza functia
	// corespunzatoare.
	if (strstr(parametru, "EDGE")) {
		edge(imag);
	} else if (strstr(parametru, "SHARPEN")) {
		sharpen(imag);
	} else if (strstr(parametru, "GAUSSIAN_BLUR")) {
		gaussian_blur(imag);
	} else if (strstr(parametru, "BLUR")) {
		blur(imag);
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}

	printf("APPLY%s done\n", parametru);
}

// Functia decupeaza imaginea
void decupare(imagine *imag, int ok, char tip[4])
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}

	int i, j;
	mat_col **aux;

	aloc_mat(&aux, imag->y2 - imag->y1, imag->x2 - imag->x1);

	// Sunt salvate in matricea auxiliara valorile din intervalele date.
	// Daca matricea este color, se salveaza toate cele 3 valori.

	if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {
		for (i = imag->y1; i < imag->y2; i++) {
			for (j = imag->x1; j < imag->x2; j++) {
				aux[i - imag->y1][j - imag->x1].r = imag->mat[i][j].r;
				aux[i - imag->y1][j - imag->x1].v = imag->mat[i][j].v;
				aux[i - imag->y1][j - imag->x1].a = imag->mat[i][j].a;
			}
		}

		eliberare_mat(imag->mat, imag->lin);
		// Dimensiunile sunt modificate.
		imag->col = imag->x2 - imag->x1;
		imag->lin = imag->y2 - imag->y1;
		aloc_mat(&imag->mat, imag->lin, imag->col);

		for (i = 0; i < imag->lin; i++) {
			for (j = 0; j < imag->col; j++) {
				imag->mat[i][j].r = aux[i][j].r;
				imag->mat[i][j].v = aux[i][j].v;
				imag->mat[i][j].a = aux[i][j].a;
			}
		}
	} else {
		for (i = imag->y1; i < imag->y2; i++)
			for (j = imag->x1; j < imag->x2; j++)
				aux[i - imag->y1][j - imag->x1].r = imag->mat[i][j].r;

		eliberare_mat(imag->mat, imag->lin);
		// Dimensiunile sunt modificate.
		imag->col = imag->x2 - imag->x1;
		imag->lin = imag->y2 - imag->y1;
		aloc_mat(&imag->mat, imag->lin, imag->col);

		for (i = 0; i < imag->lin; i++)
			for (j = 0; j < imag->col; j++)
				imag->mat[i][j].r = aux[i][j].r;
	}

	eliberare_mat(aux, imag->lin);
	imag->x1 = 0;
	imag->y1 = 0;
	imag->x2 = imag->col;
	imag->y2 = imag->lin;
	printf("Image cropped\n");
}

// Functia roteste imaginea color cu 90 de grade spre stanga.
void rot_90_col(imagine *imag)
{
	mat_col **aux;
	int x1, x2, y1, y2, i, j, ok = 0;
	x1 = imag->x1;
	x2 = imag->x2;
	y1 = imag->y1;
	y2 = imag->y2;

	aloc_mat(&aux, x2 - x1, y2 - y1);

	// Verifica daca matricea trebuie rotita in intregime, sau doar
	// o zona selectata.
	if (x1 == 0 && x2 == imag->col && y1 == 0 && y2 == imag->lin)
		ok = 1;

	// Transpune matricea.
	for (i = y1; i < y2; i++)
		for (j = x1; j < x2; j++) {
			aux[j - x1][i - y1].r = imag->mat[i][j].r;
			aux[j - x1][i - y1].v = imag->mat[i][j].v;
			aux[j - x1][i - y1].a = imag->mat[i][j].a;
		}

	// Inversaza liniile.
	for (i = 0; i < (x2 - x1) / 2; i++)
		for (j = 0; j < y2 - y1; j++) {
			interschimbare(&aux[i][j].r, &aux[x2 - x1 - 1 - i][j].r);
			interschimbare(&aux[i][j].v, &aux[x2 - x1 - 1 - i][j].v);
			interschimbare(&aux[i][j].a, &aux[x2 - x1 - 1 - i][j].a);
		}

	// Se modifica matricea initiala.
	if (ok == 1) {
		eliberare_mat(imag->mat, imag->lin);
		aloc_mat(&imag->mat, imag->col, imag->lin);
		for (i = y1; i < y2; i++)
			for (j = x1; j < x2; j++) {
				imag->mat[j][i].r = aux[j - x1][i - y1].r;
				imag->mat[j][i].v = aux[j - x1][i - y1].v;
				imag->mat[j][i].a = aux[j - x1][i - y1].a;
			}

		// Daca matricea este rotita in intregime, se moifica dimensiunile.
		imag->lin = x2;
		imag->col = y2;
		imag->x2 = imag->col;
		imag->y2 = imag->lin;
	} else {
		for (i = y1; i < y2; i++)
			for (j = x1; j < x2; j++) {
				imag->mat[i][j].r = aux[i - y1][j - x1].r;
				imag->mat[i][j].v = aux[i - y1][j - x1].v;
				imag->mat[i][j].a = aux[i - y1][j - x1].a;
			}
	}

	eliberare_mat(aux, x2 - x1);
}

// Functia roteste imaginea grayscale/alb-negru cu 90 de grade spre stanga.
void rot_90(imagine *imag)
{
	mat_col **aux;
	int x1, x2, y1, y2, i, j, ok = 0;
	x1 = imag->x1;
	x2 = imag->x2;
	y1 = imag->y1;
	y2 = imag->y2;

	aloc_mat(&aux, x2 - x1, y2 - y1);

	// Verifica daca matricea trebuie rotita in intregime, sau doar
	// o zona selectata.
	if (x1 == 0 && x2 == imag->col && y1 == 0 && y2 == imag->lin)
		ok = 1;

	// Transpune matricea.
	for (i = y1; i < y2; i++)
		for (j = x1; j < x2; j++)
			aux[j - x1][i - y1].r = imag->mat[i][j].r;

	// Inversaza liniile.
	for (i = 0; i < (x2 - x1) / 2; i++)
		for (j = 0; j < y2 - y1; j++)
			interschimbare(&aux[i][j].r, &aux[x2 - x1 - 1 - i][j].r);

	// Se modifica matricea initiala.
	if (ok == 1) {
		eliberare_mat(imag->mat, imag->lin);
		aloc_mat(&imag->mat, imag->col, imag->lin);

		for (i = y1; i < y2; i++)
			for (j = x1; j < x2; j++)
				imag->mat[j][i].r = aux[j - x1][i - y1].r;

		// Daca matricea este rotita in intregime, se modifica dimensiunile.
		imag->lin = x2;
		imag->col = y2;
		imag->x2 = imag->col;
		imag->y2 = imag->lin;
	} else {
		for (i = y1; i < y2; i++)
			for (j = x1; j < x2; j++)
				imag->mat[i][j].r = aux[i - y1][j - x1].r;
	}

	eliberare_mat(aux, x2 - x1);
}

// Functia verifica daca rotatia poate avea loc si o roteste cu unghiul cerut.
void rotire(imagine *imag, int ok, int unghi, char tip[4])
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}

	// Verifica daca unghiul este valid.
	if (unghi != 0 && unghi != 90 && unghi != 180 && unghi != 270 &&
		unghi != 360 && unghi != -90 && unghi != -180 && unghi != -270 &&
		unghi != -360) {
		printf("Unsupported rotation angle\n");
		return;
	}

	// Verifica daca selectia este patrat.
	if (imag->x2 - imag->x1 != imag->y2 - imag->y1) {
		if (imag->x1 != 0 && imag->y1 != 0 && imag->x2 != imag->col &&
			imag->y2 != imag->lin) {
			printf("The selection must be square\n");
			return;
		}
	}

	// Imaginea este rotita in functie de unghiul din comanda.
	if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {
		if (unghi == 0 || unghi == 360 || unghi == -360) {
			printf("Rotated %d\n", unghi);
			return;
		} else if (unghi == -90 || unghi == 270) {
			rot_90_col(imag);
		} else if (unghi == -180 || unghi == 180) {
			rot_90_col(imag);
			rot_90_col(imag);
		} else {
			rot_90_col(imag);
			rot_90_col(imag);
			rot_90_col(imag);
		}
	} else {
		if (unghi == 0 || unghi == 360 || unghi == -360) {
			printf("Rotated %d\n", unghi);
			return;
		} else if (unghi == -90 || unghi == 270) {
			rot_90(imag);
		} else if (unghi == -180 || unghi == 180) {
			rot_90(imag);
			rot_90(imag);
		} else {
			rot_90(imag);
			rot_90(imag);
			rot_90(imag);
		}
	}

	printf("Rotated %d\n", unghi);
}

// Functia salveaza matricea in fisier.
void salvare(imagine *imag, int ok, char tip[4], int intens_max,
			 char nume_fisier_s[MAX])
{
	char c;
	int i = 1;

	// Verifica daca matricea trebuie salvata in format ASCII.
	scanf("%c", &c);
	while (c != '\n') {
		scanf("%c", &c);
		i++;
	}

	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}

	FILE *out = fopen(nume_fisier_s, "wb");

	if (!out) {
		printf("Nu s-a putut deschide fisierul\n");
		return;
	}

	if (i != 1) {
		// Se modifica tipul matricei.
		if (strcmp(tip, "P6") == 0)
			strcpy(tip, "P3");
		else if (strcmp(tip, "P5") == 0)
			strcpy(tip, "P2");
		else if (strcmp(tip, "P4") == 0)
			strcpy(tip, "P1");

		fprintf(out, "%s\n", tip);
		fprintf(out, "%d %d\n", imag->col, imag->lin);

		if (strcmp(tip, "P2") == 0 || strcmp(tip, "P3") == 0)
			fprintf(out, "%d\n", intens_max);
		afisare_ascii(imag->mat, imag->lin, imag->col, tip, out);
	} else {
		// Se modifica tipul matricei.
		if (strcmp(tip, "P3") == 0)
			strcpy(tip, "P6");
		else if (strcmp(tip, "P2") == 0)
			strcpy(tip, "P5");
		else if (strcmp(tip, "P1") == 0)
			strcpy(tip, "P4");

		fprintf(out, "%s\n", tip);
		fprintf(out, "%d %d\n", imag->col, imag->lin);

		if (strcmp(tip, "P5") == 0 || strcmp(tip, "P6") == 0)
			fprintf(out, "%d\n", intens_max);
		afisare_binar(imag->mat, imag->lin, imag->col, tip, out);
	}

	fclose(out);
	printf("Saved %s\n", nume_fisier_s);
}

int main(void)
{
	char comanda[MAX] = "", nume_fisier[MAX] = "", tip[4] = "", c;
	char nume_fisier_s[MAX] = "";
	int ok = 0, int_max = 0;

	imagine *imag = malloc(1 * sizeof(imagine));
	scanf("%s", comanda);

	while (!strstr(comanda, "EXIT")) {
		if (strstr(comanda, "LOAD")) {
			scanf("%s", nume_fisier);
			incarcare(nume_fisier, imag, &ok, tip, &int_max);
		} else if (strstr(comanda, "SELECT")) {
			scanf("%s", comanda);
			if (strstr(comanda, "ALL")) {
				selectare_intreg(imag, ok);
			} else {
				int x1 = atoi(comanda);
				selectare_imag(imag, ok, x1);
			}
		} else if (strstr(comanda, "SAVE")) {
			scanf("%s", nume_fisier_s);
			salvare(imag, ok, tip, int_max, nume_fisier_s);
		} else if (strstr(comanda, "CROP")) {
			decupare(imag, ok, tip);
		} else if (strstr(comanda, "EQUALIZE")) {
			egalizare(imag, tip, ok);
		} else if (strstr(comanda, "APPLY")) {
			aplicare(imag, ok, tip);
		} else if (strstr(comanda, "HISTOGRAM")) {
			histograma(imag, ok, tip);
		} else if (strstr(comanda, "ROTATE")) {
			int unghi;
			scanf("%d", &unghi);
			rotire(imag, ok, unghi, tip);
		} else {
			printf("Invalid command\n");
			scanf("%c", &c);
			while (c != '\n') {
				scanf("%s", comanda);
				scanf("%c", &c);
			}
		}

		scanf("%s", comanda);
	}

	if (ok == 0)
		printf("No image loaded\n");
	else
		eliberare_mat(imag->mat, imag->lin);

	free(imag);

	return 0;
}
