#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double Q[8][8] = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
    };

struct RGB_t
{
    unsigned char R, G, B;
};

typedef struct RGB_t RGB;

struct picture_t
{
    unsigned char type; /* Type de l’image (0 ou 1) */
    unsigned int hauteur, largeur;
    unsigned char value_max; /* Valeur max utilisee pour representer les pixels */
    union
    {
        unsigned char **pixels; // un pointeur vers un tableau 2D d’unsigned char (pour les pgm)
        RGB **pixels_rgb; // un pointeur vers un tableau 2D de pixels_rgb (pour les ppm)
    };
};
typedef struct picture_t picture;



picture* get_picture (char* filename)
{
    int i, j;
    unsigned int ligne = 1, ligne_lecture = 1, temp = 0, temp_R, temp_G, temp_B;
    char c;
    picture* pic = (picture*)malloc(sizeof(picture));
    if (pic == NULL)
    {
        fprintf(stderr,"Error  : malloc failed!\n");
        exit(1);
    }
    FILE *  fic =   NULL;

    fic = fopen (filename, "r");
    if (fic == NULL)
    {
        fprintf(stderr,"Error : fopen failed!\n");
        exit(1);
    }
    while(fgetc(fic) != EOF)
    {
        fseek (fic, -1 , SEEK_CUR);
        c = (char)fgetc(fic);
        if (c == '#')
        {
            ligne_lecture++;
        }
    }

    fseek (fic, 0, SEEK_SET);

    while(fgetc(fic) != EOF)
    {
        fseek (fic, -1 , SEEK_CUR);

        c = (char)fgetc(fic);
        
        
        if (c == '\n')
        {
            ligne++;
        }
        
        if (ligne == 1)
        {
            c = fgetc(fic);
            if (c == '2')
                pic->type = '0'; // pgm
            if (c=='3')
                pic->type = '1';//ppm
        }


        if (ligne == 1+ligne_lecture)
        {
            fscanf (fic, "%d %d", &pic->largeur, &pic->hauteur);
            fscanf (fic, "%d", &temp);
            pic->value_max = (unsigned char)temp;
            if (pic->type == '0')
            {
                printf ("%d %d\n", pic->largeur, pic->hauteur);
                printf ("P2 = %c\n", pic->type);
                pic->pixels = (unsigned char**)malloc (pic->hauteur * sizeof(unsigned int*));
                if (pic->pixels == NULL)
                {
                    fprintf (stderr, "Error ! malloc echoué!\n");
                    exit(1);
                }
                else 
                {
                    for (i=0 ; i < pic->hauteur ; i++)
                    {
                        pic->pixels[i] = (unsigned char*)malloc(pic->largeur*sizeof(unsigned int));
                        if (pic->pixels[i] == NULL)
                        {
                            fprintf (stderr, "Error ! Malloc echoué! \n");
                            exit(1);
                        }
                    }
                    
                }
                for (i = 0; i < pic->hauteur ; i++)
                {
                    for ( j = 0; j < pic->largeur; j++)
                    {
                        fscanf (fic, "%d", &temp);
                        pic->pixels[i][j] = (char)temp;
                    }
                    
                }

            }
            if (pic->type == '1')
            {
                printf ("%d %d\n", pic->largeur, pic->hauteur);
                printf ("P2 = %c\n", pic->type);
                pic->pixels_rgb = (RGB**)malloc (pic->hauteur * sizeof(RGB*));
                if (pic->pixels_rgb == NULL)
                {
                    fprintf (stderr, "Error ! malloc echoué!\n");
                    exit(1);
                }
                else 
                {
                    for (i=0 ; i < pic->largeur ; i++)
                    {
                        pic->pixels_rgb[i] = (RGB*)malloc(pic->largeur*sizeof(RGB));
                        if (pic->pixels_rgb[i] == NULL)
                        {
                            fprintf (stderr, "Error ! Malloc echoué! \n");
                            exit(1);
                        }
                    }
                    
                }
                for (i = 0; i < pic->hauteur ; i++)
                {
                    for ( j = 0; j < pic->largeur; j++)
                    {
                        fscanf (fic, "%d %d %d", &temp_R, &temp_G, &temp_B);
                        pic->pixels_rgb[i][j].R = (char)temp_R;
                        pic->pixels_rgb[i][j].G = (char)temp_G;
                        pic->pixels_rgb[i][j].B = (char)temp_B;
                    }
                }
            }
        }
        
    }
    fclose(fic);
    return pic;
}


void free_picture (picture* image)
{
    if (image->type == '0')
    {
        for(int i=0 ; i < image->largeur ; i++)
            free (image->pixels[i]);
        free(image->pixels);
        image->pixels = NULL;
    }
    if (image->type == '1')
    {
        for(int i=0 ; i < image->largeur ; i++)
            free (image->pixels_rgb[i]);
        free(image->pixels_rgb);
        image->pixels_rgb = NULL;
    }
    free (image);
}

double get_Y_component_from_RGB (RGB pixel)
{
    return 0.299*pixel.R + 0.587*pixel.G + 0.144*pixel.B;
}


picture* ppm_to_pgm (picture *image)
{
    int i, j;
    picture* image_pgm = (picture*)malloc(sizeof(picture));

    image_pgm->type = '0';
    image_pgm->hauteur = image->hauteur;
    image_pgm->largeur = image->largeur;
    image_pgm->value_max = image->value_max;
    image_pgm->pixels = (unsigned char**)malloc (image_pgm->hauteur * sizeof(unsigned int*));
    if (image_pgm->pixels == NULL)
    {
        fprintf (stderr, "Error ! malloc echoué!\n");
        exit(1);
    }
    else 
    {
        for (i=0 ; i < image_pgm->hauteur ; i++)
        {
            image_pgm->pixels[i] = (unsigned char*)malloc(image_pgm->largeur*sizeof(unsigned int));
            if (image_pgm->pixels[i] == NULL)
            {
                fprintf (stderr, "Error ! Malloc echoué! \n");
                exit(1);
            }
        }
                    
    }
    for (i=0 ; i<image_pgm->hauteur ; i++)
    {
        for (j=0 ; j<image_pgm->largeur ; j++)
        {
            image_pgm->pixels[i][j] = (unsigned char)(round(get_Y_component_from_RGB(image->pixels_rgb[i][j])));
        }
    }
    return image_pgm;
}


void write_picture (picture* image, char* filename, int binary)
{   
    int i, j;
    FILE* fic = NULL;
    if (binary == 0)
    {
        fic = fopen (filename, "w");
        if (fic == NULL)
        {
            fprintf(stderr,"Error : fopen failed!\n");
            exit(1);   
        }
        else 
        {
            if (image->type == '0')
                fprintf(fic, "P2\n");
            if (image->type == '1')
                fprintf (fic, "P3\n");
            fprintf (fic, "%d %d\n", image->largeur, image->hauteur);
            fprintf (fic, "%u\n", image->value_max);
            for (i=0; i<image->hauteur; i++)
            {
                for (j=0 ; j<image->largeur ; j++)
                {
                    fprintf(fic, "%u\n", image->pixels[i][j]);
                }
            }

        }
    }
}

void print_mat (double bloc[8][8])
{
    int i, j;
    for (i=0 ; i<8 ; i++)
    {
        for (j=0 ; j<8 ; j++)
        {
            printf ("%.0f  ", bloc[i][j]);
        }
        printf ("\n");
    }
    printf ("-----------------------------------------\n");
}

void extract_bloc(picture *image, double bloc[8][8], int i, int j)
{
    for (int k=0 ; k<8 ; k++)
    {
        for (int t=0 ; t<8 ; t++)
        {
            bloc[k][t] = image->pixels[i+k][j+t];
        }
    }
}

void DCT(double bloc[8][8])
{
    int i, j, t, k;
    // création de la matrice de la transformée de cosinus, C et Ct ça transposée.
    double DCT[8][8];

    double Ci=0, Cj=0;
    for (i=0 ; i<8 ; i++)
    {
        for (j=0 ; j<8 ; j++)
        {
            DCT[i][j] = 0;
            for (k=0 ; k<8 ; k++)
            {
                for (t=0 ; t<8 ; t++)
                {
                    DCT[i][j] += bloc[k][t] * cos((M_PI *(2*t+1)*j)/16) * cos((M_PI *(2*k+1)*i)/16);
                }
            }

            if (i == 0)
                Ci = 1/sqrt(2);
            else
                Ci = 1;
            if (j == 0)
                Cj = 1/sqrt(2);
            else
                Cj = 1;
            DCT[i][j] = DCT[i][j]*0.25*Ci*Cj;
        }
    }
    for (i=0 ; i<8 ; i++)
    {
        for (j=0 ; j<8 ; j++)
        {
            bloc[i][j] = DCT[i][j];
        }
    }
}


void quantify (double bloc[8][8])
{
    int i, j;
    double temp[8][8];
    for (i=0 ; i<8 ; i++)
    {
        for (j=0 ; j<8 ; j++)
        {
            temp[i][j] = bloc[i][j]/Q[i][j];
        }
    }

    for (i=0 ; i<8 ; i++)
    {
        for (j=0 ; j<8 ; j++)
        {
            bloc[i][j] = temp[i][j];
        }
    }
}


void zigzag_extraction(double bloc[8][8], int zigzag[64])
{
    unsigned int k =0, i, j;
    for (i=0 ; i < 2*8 ; i++)
    {
        for (j=(i<8)?0:i-8+1 ; j<=i && j<8 ; j++)
        {
            if (i%2 == 1)
                zigzag[k] = bloc[j][i-j];
            else 
                zigzag[k] = bloc[i-j][j];
            k++;
        }
    }
}


void compress_RLE(FILE *f, int zigzag[64])
{
    int i = 0;
    unsigned int n = 0;
    while (i<64)
    {
        if (zigzag[i] != 0 && i<64)
        {
            fprintf (f, "%d\n", zigzag[i]);
            i++;
        }
        else 
        {
            while ( zigzag[i] == 0  && i<64 )
            {
                n++;
                i++;
            }
        
            if (n>1)
            {
                fprintf (f, "@%d\n", n);
            }
            if (n==1)
            {
                fprintf (f, "0\n");
            }
            n=0;
        }  
    }
}

void jpeg_compression(picture *image, char *filename)
{
    double bloc[8][8];
    int zigzag[64];
    FILE * f = fopen (filename, "w");
    if (f == NULL)
    {
        fprintf (stderr, "Error! Ouverture de fichier echouée\n");
        exit(1);
    }
    fprintf (f, "JPEG\n");
    fprintf (f, "%d %d\n", image->largeur, image->largeur);
    for (int a=0 ; a< image->hauteur ; a+=8)
    {
        for (int b=0 ; b < image->largeur ; b+=8)
        {
            extract_bloc (image, bloc, a, b);
            DCT(bloc);
            quantify (bloc);
            zigzag_extraction (bloc, zigzag);
            compress_RLE (f, zigzag);
        }
    }
    fclose (f);
}


unsigned int file_size(char *filename)
{
    char c;
    unsigned int size = 0;
    FILE * f = fopen (filename, "r");
    if (f == NULL)
    {
        fprintf (stderr, "Error! Ouverture de fichier echouée\n");
        exit(1);
    }

    while (!feof(f))
    {
        c = (char)fgetc(f);
        size += sizeof(c);
    }

    fclose (f);

    return size - 1;
}

int main (void)
{
    clock_t t1, t2, t;
    
    t1 = clock ();
    picture* image = get_picture ("Monarch.ppm");
    
    picture* image_pgm = ppm_to_pgm (image);
    write_picture (image_pgm, "akli.pgm", 0);
    jpeg_compression (image_pgm, "akli.jpeg");
    printf ("size of file %d\n", file_size ("akli.jpeg"));
    free_picture (image_pgm);
    free_picture (image);
    t2 = clock();
    t = (t2 - t1) * 1e-6;
    printf ("Temps d'exécution = %ld\n", t);
    return 0;
}