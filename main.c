#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <math.h>

typedef struct {
	unsigned char B, G, R;
}pixel;

typedef struct {
	unsigned char* x;
	pixel* y;
}nr;

typedef struct {
	int *x;
	int *y;
	int z;
}Detectii;

nr openLin(char *numeFisier) {
	FILE *f;
	f = fopen(numeFisier, "rb");
	unsigned char *imagedata;

	imagedata = (unsigned char*)malloc(sizeof(unsigned char) * 54);

	int i = 0, j;
	unsigned char  y;

	for (i = 0; i < 54; i++)
	{
		fread(&y, 1, 1, f);
		imagedata[i] = y;
	}

	int image_width = *(int*)&imagedata[18];
	int image_height = *(int*)&imagedata[22];

	unsigned char padding;
	if (image_width % 4 != 0)
		padding = 4 - (3 * image_width) % 4;
	else padding = 0;

	pixel *data = (pixel*)malloc(sizeof(pixel) * image_width * image_height);

	for (i = image_height - 1; i >= 0; i--) {
		for (j = 0; j < image_width; j++)
			fread(&data[i*image_width + j], sizeof(pixel), 1, f);
		fseek(f, padding, SEEK_CUR);
	}

	nr returnare;
	returnare.x = imagedata;
	returnare.y = data;

	fclose(f);
	free(data);
	free(imagedata);
	return returnare;

}

void closeLin(char *numeFisier, nr result) {
	FILE *g;
	g = fopen(numeFisier, "wb");

	int i;
	int img_w = *(int*)&result.x[18];
	int img_h = *(int*)&result.x[22];
	for (i = 0; i < 54; i++)
		fwrite(&result.x[i], sizeof(unsigned char), 1, g);

	unsigned char padding;
	if (img_w % 4 != 0)
		padding = 4 - ((img_w * 3) % 4);
	else padding = 0;


	int j, k;
	for (i = img_h - 1; i >= 0; i--) {
		for (j = 0; j < img_w; j++)
			fwrite(&result.y[i*img_w + j], sizeof(pixel), 1, g);

		for (k = 1; k <= padding; k++)
			fwrite(&padding, 1, 1, g);
	}

	fclose(g);
}

int xorshift(int seed) {
	int r = seed;
	r = r ^ r << 13;
	r = r ^ r >> 17;
	r = r ^ r << 5;

	return r;
}

void criptare(char *numeFisier, char *numeFisierCriptat, char *Cheie) {
	FILE *f, *h;
	f = fopen(numeFisier, "rb");
	h = fopen(Cheie, "rt");

	int img_w, img_h, i, z;
	unsigned char *imagedata = (unsigned char*)malloc(sizeof(unsigned char) * 54);

	unsigned char  y;

	for (i = 0; i < 54; i++)
	{
		fread(&y, 1, 1, f);
		imagedata[i] = y;
	}

	img_w = *(int*)&imagedata[18];
	img_h = *(int*)&imagedata[22];

	int n = img_w * img_h;
	int *r = (int*)malloc(sizeof(int) * 2 * n);


	int cheie, sv;
	fscanf(h, "%d %d", &cheie, &sv);
	r[0] = cheie;


	for (i = 1; i <= n * 2 - 1; i++)
		r[i] = xorshift(r[i - 1]);


	unsigned int *p = (unsigned int*)malloc(n * sizeof(unsigned int));
	for (i = 0; i < n; i++)
		p[i] = i;


	int d, a = 0, aux;
	for (i = n - 1; i >= 1; i--) {
		d = p[a] % (i + 1);
		aux = p[i];
		p[i] = p[d];
		p[d] = aux;
		a++;
	}


	nr L;
	L.y = malloc(n * sizeof(pixel));
	L = openLin(numeFisier);
	nr M;
	M.y = malloc(n * sizeof(pixel));


	for (i = 0; i < n; i++) {
		M.y[p[i]].B = L.y[i].B;
		M.y[p[i]].G = L.y[i].G;
		M.y[p[i]].R = L.y[i].R;
	}
	nr C;
	C.y = malloc(sizeof(pixel) * n);
	C.x = imagedata;

	for (i = 0; i < n; i++) {
		if (i == 0) {
			C.y[i].B = sv ^ M.y[p[i]].B ^ r[n];
			C.y[i].G = sv ^ M.y[p[i]].G ^ r[n];
			C.y[i].R = sv ^ M.y[p[i]].R ^ r[n];
		}
		else
		{
			C.y[i].B = C.y[i - 1].B ^ M.y[p[i]].B ^ r[n + i];
			C.y[i].G = C.y[i - 1].G ^ M.y[p[i]].G ^ r[n + i];
			C.y[i].R = C.y[i - 1].R ^ M.y[p[i]].R ^ r[n + i];

		}
	}
	free(imagedata);
	free(r);
	free(p);
	fclose(f);
	fclose(h);
	closeLin(numeFisierCriptat, C);


}


void decriptare(char *numeFisierCriptat, char *numeFisier, char *Cheie) {
	FILE *f, *g;
	f = fopen(numeFisierCriptat, "rb");
	g = fopen(Cheie, "rt");

	int cheie, sv;
	fscanf(g, "%d %d", &cheie, &sv);


	int img_w, img_h, i;

	unsigned char *imagedata = (unsigned char*)malloc(sizeof(unsigned char) * 54);

	unsigned char  y;

	for (i = 0; i < 54; i++)
	{
		fread(&y, 1, 1, f);
		imagedata[i] = y;
	}

	img_w = *(int*)&imagedata[18];
	img_h = *(int*)&imagedata[22];
	int n = img_w * img_h;

	int *r = (int*)malloc(sizeof(int) * 2 * n);
	r[0] = cheie;

	for (i = 1; i <= n * 2 - 1; i++)
		r[i] = xorshift(r[i - 1]);

	unsigned int *p = (unsigned int*)malloc(n * sizeof(unsigned int));
	for (i = 0; i < n; i++)
		p[i] = i;

	int d, a = 0, aux;
	for (i = n - 1; i >= 0; i--) {
		d = p[a] % (i + 1);
		aux = p[i];
		p[i] = p[d];
		p[d] = aux;
		a++;
	}
	unsigned int *q = (unsigned int*)malloc(n * sizeof(unsigned int));

	for (i = 0; i < n; i++)
		q[p[i]] = i;

	nr L;
	L.y = malloc(n * sizeof(pixel));
	L = openLin(numeFisierCriptat);
	nr C;
	C.y = malloc(sizeof(pixel) * n);

	for (i = 0; i < n; i++) {
		if (i == 0) {
			C.y[i].B = sv ^ L.y[p[i]].B ^ r[n];
			C.y[i].G = sv ^ L.y[p[i]].G ^ r[n];
			C.y[i].R = sv ^ L.y[p[i]].R ^ r[n];
		}
		else
		{
			C.y[i].B = C.y[i - 1].B ^ L.y[p[i]].B ^ r[n + i];
			C.y[i].G = C.y[i - 1].G ^ L.y[p[i]].G ^ r[n + i];
			C.y[i].R = C.y[i - 1].R ^ L.y[p[i]].R ^ r[n + i];

		}
	}

	nr D;
	D.y = malloc(sizeof(pixel) * n);
	D.x = imagedata;
	for (i = 0; i < n; i++) {
		D.y[q[i]].B = C.y[i].B;
		D.y[q[i]].G = C.y[i].G;
		D.y[q[i]].R = C.y[i].R;

	}
	fclose(f);
	fclose(g);
	free(imagedata);
	free(r);
	free(p);
	free(q);
	closeLin(numeFisier, D);

}

float eq(float estimare, float ap) {

	float E;
	E = pow((ap - estimare), 2) / estimare;
	return E;
}

void chi_test(char *numeFisier) {

	FILE *f;
	f = fopen(numeFisier, "rb");

	fseek(f, 18, SEEK_SET);
	int img_w, img_h;
	fread(&img_w, sizeof(int), 1, f);
	fread(&img_h, sizeof(int), 1, f);
	int row_padded = (img_w * 3 + 3) & (~3);
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * row_padded);
	float* fR = (float*)malloc(sizeof(float) * img_h * img_w);
	float* fG = (float*)malloc(sizeof(float) * img_h * img_w);
	float* fB = (float*)malloc(sizeof(float) * img_h * img_w);

	int i; int j;
	for (i = 0; i < img_h; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		for (j = 0; j < img_w * 3; j += 3) {
			fB[data[j]]++;
			fG[data[j + 1]]++;
			fR[data[j + 2]]++;

		}
	}

	float estimated = (img_w * img_h) / 256;
	float SR = 0, SG = 0, SB = 0;
	for (i = 0; i <= 255; i++)
	{

		SR = SR + eq(estimated, fR[i]);
		SG = SG + eq(estimated, fG[i]);
		SB = SB + eq(estimated, fB[i]);

	}

	printf("R:%f \n G:%f \n B:%f \n\n", SR, SG, SB);
	free(fR); free(fB); free(fG);

	fclose(f);


}

void grayscale_image(char* nume_fisier_sursa, char* nume_fisier_destinatie)
{
	FILE *fin, *fout;
	unsigned int dim_img, latime_img, inaltime_img;
	unsigned char pRGB[3], header[54], aux;

	printf("nume_fisier_sursa = %s \n", nume_fisier_sursa);

	fin = fopen(nume_fisier_sursa, "rb");
	if (fin == NULL)
	{
		printf("nu am gasit imaginea sursa din care citesc");
		return;
	}

	fout = fopen(nume_fisier_destinatie, "wb+");

	fseek(fin, 2, SEEK_SET);
	fread(&dim_img, sizeof(unsigned int), 1, fin);
	printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

	fseek(fin, 18, SEEK_SET);
	fread(&latime_img, sizeof(unsigned int), 1, fin);
	fread(&inaltime_img, sizeof(unsigned int), 1, fin);
	printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n", latime_img, inaltime_img);

	//copiaza octet cu octet imaginea initiala in cea noua
	fseek(fin, 0, SEEK_SET);
	unsigned char c;
	while (fread(&c, 1, 1, fin) == 1)
	{
		fwrite(&c, 1, 1, fout);
		fflush(fout);
	}
	fclose(fin);

	//calculam padding-ul pentru o linie
	int padding;
	if (latime_img % 4 != 0)
		padding = 4 - (3 * latime_img) % 4;
	else
		padding = 0;

	printf("padding = %d \n", padding);

	fseek(fout, 54, SEEK_SET);
	int i, j;
	for (i = 0; i < inaltime_img; i++)
	{
		for (j = 0; j < latime_img; j++)
		{
			//citesc culorile pixelului
			fread(pRGB, 3, 1, fout);
			//fac conversia in pixel gri
			aux = 0.299*pRGB[2] + 0.587*pRGB[1] + 0.114*pRGB[0];
			pRGB[0] = pRGB[1] = pRGB[2] = aux;
			fseek(fout, -3, SEEK_CUR);
			fwrite(pRGB, 3, 1, fout);
			fflush(fout);
		}
		fseek(fout, padding, SEEK_CUR);
	}
	fclose(fout);
}

pixel** matrice(char *numeImagine) {

	FILE *f;
	f = fopen(numeImagine, "rb");

	int img_w, img_h;
	fseek(f, 18, SEEK_SET);
	fread(&img_w, sizeof(int), 1, f);
	fread(&img_h, sizeof(int), 1, f);

	unsigned char padding;
	if (img_w % 4 != 0)
		padding = 4 - (3 * img_w) % 4;
	else padding = 0;

	int i, j;

	pixel **m = (pixel**)malloc(img_h * sizeof(pixel*));
	for (i = 0; i < img_h; i++)
		m[i] = (pixel*)malloc(img_w * sizeof(pixel));

	for (i = 0; i < img_h; i++)
	{
		for (j = 0; j < img_w; j++)
			fread(&m[i][j], sizeof(pixel), 1, f);
		fseek(f, padding, SEEK_CUR);
	}

	fclose(f);

	return m;


}

float corelatie(pixel **Imagine, pixel **Sablon, int img_w, int img_h, int sab_w, int sab_h, int k, int l)
{
	float n = sab_h * sab_w;
	float sig_s = 0, sig_fI = 0;
	float media_S = 0, media_fI = 0;
	int i, j;
	for (i = 0; i < sab_h; i++)
		for (j = 0; j < sab_w; j++) {
			media_S = (float)media_S + Sablon[i][j].R;
			media_fI = (float)media_fI + Imagine[i + k][j + l].R;
		}
	media_S = (float)media_S / n;
	media_fI = (float)media_fI / n;

	for (i = 0; i < sab_h; i++)
		for (j = 0; j < sab_w; j++) {
			sig_s = (float)sig_s + (Sablon[i][j].R - media_S)* (Sablon[i][j].R - media_S);
			sig_fI = (float)sig_fI + (Imagine[i + k][j + l].R - media_fI)*(Imagine[i + k][j + l].R - media_fI);
		}

	float m = n - 1;
	m = (float)1 / m;
	sig_s = (float)m * sig_s;
	sig_fI = (float)m * sig_fI;
	sig_s = sqrt(sig_s);
	sig_fI = sqrt(sig_fI);

	float sig_prod;

	float cor = 0;

	for (i = 0; i < sab_h; i++)
		for (j = 0; j < sab_w; j++) {
			sig_prod = (float)sig_s * sig_fI;
			sig_prod = (float)1 / sig_prod;
			cor = (float)cor + (Imagine[i + k][j + l].R - media_fI) * (Sablon[i][j].R - media_S)*sig_prod;
		}
	cor = (float)cor / n;
	return cor;


}

Detectii template_matching(char *numeImagine, char *numeSablon, float prag) {


	FILE *Img, *Sab, *Result;
	pixel **I;
	I = matrice(numeImagine);
	pixel **S;
	S = matrice(numeSablon);


	Img = fopen(numeImagine, "rb");
	Sab = fopen(numeSablon, "rb");
	Result = fopen("rezultat.bmp", "wb");

	int img_w, img_h;
	fseek(Img, 18, SEEK_SET);
	fread(&img_w, sizeof(int), 1, Img);
	fread(&img_h, sizeof(int), 1, Img);
	fseek(Img, 0, SEEK_SET);

	int x, y, i, j;
	for (i = 0; i < 54; i++) {
		fread(&x, 1, 1, Img);
		fwrite(&x, 1, 1, Result);
	}
	fseek(Img, 54, SEEK_SET);
	while (fread(&y, 1, 1, Img) == 1)
		fwrite(&y, 1, 1, Result);


	int sab_w, sab_h;
	fseek(Sab, 18, SEEK_SET);
	fread(&sab_w, sizeof(int), 1, Sab);
	fread(&sab_h, sizeof(int), 1, Sab);

	unsigned char c[3];
	float crl;
	Detectii d;
	d.x = malloc(sizeof(Detectii) * img_h * img_w);
	d.y = malloc(sizeof(Detectii) * img_h * img_w);
	int k = 0;
	c[0] = 255; c[1] = 0; c[2] = 0;
	for (i = 0; i < img_h; i++)
		for (j = 0; j < img_w; j++)

		{
			crl = corelatie(I, S, img_w, img_h, sab_w, sab_h, i, j);
			if (crl > prag) {
				printf("i: %d   j: %d   crl : %f  \n", i, j, crl);
				d.x[k] = i;
				d.y[k] = j;
				k++;
			}
		}
	for (i = 0; i < img_h; i++)
		free(I[i]);
	free(I);

	for (i = 0; i < sab_h; i++)
		free(S[i]);
	free(S);

	d.z = k;
	return d;

}

int compare(const void *x, const void *y) {
	int zx, zy;
	zx = *(int*)x;
	zy = *(int*)y;
	if (zx < zy)
		return 1;
	if (zx > zy)
		return -1;
	return 0;
}

void Sort(Detectii *D) {
	int n = D->z;
	qsort(D, n, sizeof(Detectii), compare);
}


int main() {
	criptare("peppers.bmp", "fisiercriptat.bmp", "secret_key.txt");
	decriptare("fisiercriptat.bmp", "rezultat.bmp", "secret_key.txt");
	chi_test("peppers.bmp");
	printf("\n\n");
	chi_test("fisiercriptat.bmp");



	FILE *f;
	f = fopen("imagini.txt", "rt");

	char numeImagine[101];
	fscanf(f, "%s", &numeImagine);
	grayscale_image(numeImagine, "test2.bmp");

	float prag;
	char Sablon[101], sablon_gr[101];
	fscanf(f, "%s", &Sablon);

	grayscale_image(Sablon, sablon_gr);
	printf("Prag =  ");
	scanf("%f", &prag);

	Detectii result;
	result = template_matching(numeImagine, Sablon, prag);
	Sort(&result);

	return 1;
}
