#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define C 2
#define N 2

#define SUEIL_MAX 30
#define SUEIL_MIN 10



struct list_t
{
    int x;
    int y;
    struct list_t* next;
};

typedef struct list_t list;

list* init_list ()
{
    return NULL;
}

double W0(double x)
{
    double y = fabs(x);
    double w = 0;
    if (y > 0.5)
      w=0;
    else if (y < 0.5)
        w=1;
    else
      w = 0.5;
    return w;
}


double W1(double x)
{
    double w = 0;
    if (x >= -1 && x <=0 )
      w = x+1;
    else if (x >= 0 && x <=1)
      w = 1 - x;
    else 
      w = 0;
    return w;
}


double W2(double x)
{
    double w=0;
    if (fabs(x) > 1.5)
        w = 0;
    else if (x >= -1.05 && x <= -0.5)
        w = 0.5 * ((x+1.4) * (x+1.4));
    else if ( x >= -0.5 && x <= 0.5)
        w = 0.75 - (x*x);
    else
      w = 0.5 * ((x-1.5)*(x-1.5));
    return w;
}

double W3(double x)
{
    double y = fabs (x);
    double w=0;
    if (y > 2)
      w = 0;
    else if (y >= 0 && y <= 1)
      w = 0.5 * (y*y*y) - (x*x) + 2/3.;
    else if (y >= 1 && y <= 2)
      w = 1/6. * ((2-y)*(2-y)*(2-y));
    return w;
}



double (*W[4])(double) = {W0, W1, W2, W3};

struct RGB_t
{
  unsigned char R, G, B;
};

typedef struct RGB_t RGB;


struct picture_t
{
  char type; // 0 = PGM, 1 = PPM
  int largeur, hauteur;
  unsigned char value_max;
  union
  {
    unsigned char **pixels; // Image PGM
    RGB ** pixels_rgb; // Image PPM
  };

};

typedef struct picture_t picture; 


picture* copy_image_pgm (picture* image)
{
    picture* image_copy = (picture*)malloc(sizeof(picture));
    
    image_copy->hauteur = image->hauteur;
    image_copy->largeur = image->largeur;
    image_copy->type = image->type;
    image_copy->value_max = image->value_max;
    image_copy->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * image_copy->hauteur);
    if (image_copy->pixels == NULL)
    {
        fprintf (stderr, "Erreur copy_image_pgm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_copy->hauteur ; i++)
    {
        image_copy->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) * image_copy->largeur);
        if (image_copy->pixels[i] == NULL)
        {
            fprintf (stderr, "Erreur copy_image_pgm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }

    for (int i=0 ; i<image_copy->hauteur ; i++)
    {
        for (int j=0 ; j<image_copy->largeur ; j++)
        {
            image_copy->pixels[i][j] = 0;
        }
    }

    return image_copy;
}

picture* copy_image_ppm (picture* image)
{
    picture* image_copy = (picture*)malloc(sizeof(picture));
    
    image_copy->hauteur = image->hauteur;
    image_copy->largeur = image->largeur;
    image_copy->type = image->type;
    image_copy->value_max = image->value_max;
    image_copy->pixels_rgb = (RGB**)malloc(sizeof(RGB*) * image_copy->hauteur);
    if (image_copy->pixels_rgb == NULL)
    {
        fprintf (stderr, "Erreur copy_image_ppm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_copy->hauteur ; i++)
    {
        image_copy->pixels_rgb[i] = (RGB*)malloc(sizeof(RGB) * image_copy->largeur);
        if (image_copy->pixels_rgb[i] == NULL)
        {
            fprintf (stderr, "Erreur copy_image_ppm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }

    for (int i=0 ; i<image_copy->hauteur ; i++)
    {
        for (int j=0 ; j<image_copy->largeur ; j++)
        {
            image_copy->pixels_rgb[i][j].R = 0;
            image_copy->pixels_rgb[i][j].G = 0;
            image_copy->pixels_rgb[i][j].B = 0;
        }
    }

    return image_copy;
}


void free_picture(picture *image)
{
    if(image->type == 0 || image->type == 1)
    {
        for(int i = 0 ; i < image->hauteur ; i++)
	        free(image->pixels_rgb[i]);
        free(image->pixels_rgb);
    }
    else
    {
        for(int i = 0 ; i < image->hauteur ; i++)
	        free(image->pixels[i]);
        free(image->pixels);
    }

    free(image);
}


void extract_pgm_ascii(unsigned char **pixels, int hauteur, int largeur, FILE *f)
{
    int tmp;
    for(int i = 0 ; i < hauteur ; i++)
    {
        for(int j = 0 ; j < largeur ; j++)
        {
	        fscanf(f,"%d",&tmp);
	        pixels[i][j] = (unsigned char) tmp;
        }
    }
}

void extract_pgm_binary(unsigned char **pixels, int hauteur, int largeur, FILE *f)
{
    for(int i = 0 ; i < hauteur ; i++)
        fread((void*)pixels[i],1,largeur, f);
}

void extract_ppm_ascii(RGB **pixels_rgb, int hauteur, int largeur, FILE *f)
{
    int tmp;
    for(int i = 0 ; i < hauteur ; i++)
        for(int j = 0 ; j < largeur ; j++)
        {
	        fscanf(f,"%d",&tmp);
	        pixels_rgb[i][j].R = (unsigned char) tmp;
	        fscanf(f,"%d",&tmp);
	        pixels_rgb[i][j].G = (unsigned char) tmp;
	        fscanf(f,"%d",&tmp);
	        pixels_rgb[i][j].B = (unsigned char) tmp;
        }
}

void extract_ppm_binary(RGB **pixels_rgb, int hauteur, int largeur, FILE *f)
{
    size_t t;
    for(int i = 0 ; i < hauteur ; i++)
    {
        t = fread((void*)pixels_rgb[i], 3, largeur, f);

        if(t != largeur)
	    {
	        fprintf(stderr,"extract_rgb_binary : erreur lors de l'extraction de la ligne %d.\n",i);
	        exit(1);
	    }
    }
}

picture *get_picture(char *file_name)
{
    FILE *f = fopen(file_name, "r");

    if( f == NULL )
    {
        fprintf(stderr, "get_picture : erreur lors de l'ouverture du fichier %s.\n",file_name);
        exit(1);
    }

    char c = getc(f), num = getc(f);
    if( c != 'P' || (num != '2' && num != '3' && num != '5' && num != '6'))
    {
        fprintf(stderr, "get_picture : erreur le fichier doit être de type pgm ou ppm.\n");
        exit(1);
    }

    picture *res = (picture*) malloc(sizeof(picture));

    if( res == NULL)
    {
        fprintf(stderr, "get_picture : erreur lors du malloc pour l'image.\n");
        exit(1);
    }

    if (num == '2' || num == '5')
        res->type = (char) 0;

    if (num == '3' || num == '6')
        res->type = (char) 1;

  
    /* On passe les commentaires*/
    while(c != '\n')
        c = getc(f);
  
    c = getc(f);
    while(c == '#')
    {
        while(c != '\n')
	        c = getc(f);
        c = getc(f);
    }
    fseek(f,-1,SEEK_CUR); /* On recule le curseur d'un cran pour avoir le premier chiffre de la largeur*/
  
    fscanf(f,"%d %d",&(res->largeur), &(res->hauteur));

    /* On extrait la valeur max des pixels */
    int tmp;
    fscanf(f,"%d", &tmp);
    res->value_max = (unsigned char) tmp;
  
    if(res->type == 0) // Image de type pgm
    {
        res->pixels = (unsigned char**) malloc(sizeof(unsigned char*)*res->hauteur);
        for(int i = 0 ; i < res->hauteur ; i++)
	    res->pixels[i] = (unsigned char*) malloc(sizeof(unsigned char)*res->largeur);
      
        if( num == '2' )
	    {
	        extract_pgm_ascii(res->pixels, res->hauteur, res->largeur, f);
	    }
        else
    	{
	        getc(f); // Pour passer le saut de ligne
	        extract_pgm_binary(res->pixels, res->hauteur, res->largeur, f);
	    }
    }
    else // Image de type ppm
    {
        res->pixels_rgb = (RGB**) malloc(sizeof(RGB*)*res->hauteur);
        for(int i = 0 ; i < res->hauteur ; i++)
	        res->pixels_rgb[i] = (RGB*) malloc(sizeof(RGB)*res->largeur);

        if( num == '3' )
	    {
	        extract_ppm_ascii(res->pixels_rgb, res->hauteur, res->largeur, f);
    	}
        else
	    {
	        getc(f); // Pour passer le saut de ligne
	        extract_ppm_binary(res->pixels_rgb, res->hauteur, res->largeur, f);
	    }
    }
  
    fclose(f);

    return res;
}


int string_length(char *string)
{
    int i = 0;
    while(string[i] != '\0')
        i++;
  
    return i;
}

void write_picture(picture *image, char *file_name, int binary)
{
    if( image == NULL )
    {
        fprintf(stderr,"Error write_picture : uninitialized parameter!\n");
        exit(1);
    }
  
    if(image->type != 0 && image->type != 1)
    {
        fprintf(stderr,"write_picture : the image type should be either 0 or 1\n");
        exit(1);
    }
  
    FILE *f = fopen(file_name,"w");
    if( f == NULL )
    {
        fprintf(stderr,"write_picture : erreur lors de l'ouverture de %s.\n",file_name);
        exit(1);
    }

    if(image->type == 0)
    {
        if(binary)
	        fprintf(f,"P5");
        else
	        fprintf(f,"P2");
    }
    else
    {
        if(binary)
	        fprintf(f,"P6");
        else
	        fprintf(f,"P3");
    }

    fprintf(f,"\n%d %d\n",image->largeur,image->hauteur);

    fprintf(f,"%d\n",image->value_max);

    printf("image->value_max = %d\n",image->value_max);

    if(image->type == 0)
    {
        if(binary)
	    {
	        for(int i = 0 ; i < image->hauteur ; i++)
	            fwrite((void*)image->pixels[i], 1, image->largeur, f);
	    }
        else
	    {
	        for(int i = 0 ; i < image->hauteur ; i++)
	        {
	            for(int j = 0 ; j < image->largeur ; j++)
		            fprintf(f,"%d\n",image->pixels[i][j]);
	        }
	    }
    }
    else
    {
        if(binary)
	    {
	        for(int i = 0 ; i < image->hauteur ; i++)
	            fwrite((void*)image->pixels_rgb[i], 3, image->largeur, f);
	    }
        else
    	{
	        for(int i = 0 ; i < image->hauteur ; i++)
	        {
	            for(int j = 0 ; j < image->largeur ; j++)
		            fprintf(f,"%d\n%d\n%d\n",image->pixels_rgb[i][j].R,image->pixels_rgb[i][j].G,image->pixels_rgb[i][j].B);
	        }
	    }
    }
  
    fclose(f);
}


picture* copy_image (picture* image)
{

    if (image->type == 0)
        return copy_image_pgm (image);
    else
        return copy_image_ppm (image);
}


picture *naive_x_pgm(picture *image)
{
    int i=0, j=0;
    picture *Iv = copy_image (image);
    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            Iv->pixels[i][j] = abs(image->pixels[i][j] - image->pixels[i][j-1]);
        }
    }

    return Iv;
}


picture *naive_y_pgm(picture *image)
{
    int i=0, j=0;
    picture *Ih = copy_image (image);

    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            Ih->pixels[i][j] = abs(image->pixels[i][j] - image->pixels[i-1][j]);
        }
    }

    return Ih;
}


picture *naive_edge_detector_pgm(picture *image)
{

    int i=0, j=0;
    int temp=0;

    picture *Iv, *Ih; 
    picture *If = copy_image (image);

    Iv = naive_x_pgm (image);
    Ih = naive_y_pgm (image);
    
    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            temp = sqrt (pow(Ih->pixels[i][j], 2) + pow(Iv->pixels[i][j], 2));
            If->pixels[i][j] = temp/sqrt(2);
        }
    }
    free_picture (Iv);
    free_picture (Ih);

    return If;
}


picture *naive_x_ppm(picture *image)
{
    int i=0, j=0;
    picture *Iv = copy_image (image);
    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            Iv->pixels_rgb[i][j].R = abs(image->pixels_rgb[i][j].R - image->pixels_rgb[i][j-1].R);
            Iv->pixels_rgb[i][j].G = abs(image->pixels_rgb[i][j].G - image->pixels_rgb[i][j-1].G);
            Iv->pixels_rgb[i][j].B = abs(image->pixels_rgb[i][j].B - image->pixels_rgb[i][j-1].B);
        }
    }

    return Iv;
}


picture *naive_y_ppm(picture *image)
{
    int i=0, j=0;
    picture *Ih = copy_image (image);

    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            Ih->pixels_rgb[i][j].R = abs(image->pixels_rgb[i][j].R - image->pixels_rgb[i-1][j].R);
            Ih->pixels_rgb[i][j].G = abs(image->pixels_rgb[i][j].G - image->pixels_rgb[i-1][j].G);
            Ih->pixels_rgb[i][j].B = abs(image->pixels_rgb[i][j].B - image->pixels_rgb[i-1][j].B);
        }
    }

    return Ih;
}


picture *naive_edge_detector_ppm (picture *image)
{

    int i=0, j=0;
    int temp=0;

    picture *Iv, *Ih; 
    picture *If = copy_image (image);

    Iv = naive_x_ppm (image);
    Ih = naive_y_ppm (image);
    
    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            temp = sqrt (pow(Ih->pixels_rgb[i][j].R, 2) + pow(Iv->pixels_rgb[i][j].R, 2));
            If->pixels_rgb[i][j].R = temp/sqrt(2);
            temp = sqrt (pow(Ih->pixels_rgb[i][j].G, 2) + pow(Iv->pixels_rgb[i][j].G, 2));
            If->pixels_rgb[i][j].G = temp/sqrt(2);
            temp = sqrt (pow(Ih->pixels_rgb[i][j].B, 2) + pow(Iv->pixels_rgb[i][j].B, 2));
            If->pixels_rgb[i][j].B = temp/sqrt(2);


        }
    }
    free_picture (Iv);
    free_picture (Ih);

    return If;
}


picture* naive_edge_detector (picture* image)
{
    if (image->type == 0)
        return naive_edge_detector_pgm (image);
    return naive_edge_detector_ppm (image);
}

int Ky[3][3] =
{
    {-1, -C, -1},
    {0, 0, 0},
    {1, C, 1}
};

int Kx[3][3] =
{
    {-1, 0, 1},
    {-C, 0, C},
    {-1, 0, 1}
};


picture *sobel_x(picture *image)
{

    int i=0, j=0, k, t;
    
    int temp = 0;
    
    picture *Ih = copy_image (image);

    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            temp = 0;
            for (k=0 ; k<3 ; k++)
            {
                for (t=0 ; t<3; t++)
                {
                    temp += (image->pixels[i+k-1][j+t-1] * Kx[k][t])/(double)(2+C);
                }
            }
            Ih->pixels[i][j] = abs(temp);
        }
    }

    return Ih;
}

picture *sobel_y(picture *image)
{
    int i=0, j=0, k, t;
    int temp = 0;
    picture *Iv = copy_image (image);

    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            temp = 0;
            for (k=0 ; k<3 ; k++)
            {
                for (t=0 ; t<3; t++)
                {
                    temp += (image->pixels[i+k-1][j+t-1] * Ky[k][t])/(double)(2+C);
                }
            }
            Iv->pixels[i][j] = abs(temp);
        }
    }

    return Iv;
}


picture *sobel_edge_detector(picture *image)
{
    int i=0, j=0;
    unsigned char temp=0;

    picture *Iv, *Ih; 
    picture *If = copy_image (image);

    Iv = sobel_x (image);
    Ih = sobel_y (image);
    
    for (i=1 ; i<image->hauteur-1 ; i++)
    {
        for (j=1 ; j<image->largeur-1 ; j++)
        {
            temp = sqrt (pow((Ih->pixels[i][j]), 2) + pow((Iv->pixels[i][j]), 2));
            If->pixels[i][j] = temp/(sqrt(2));
        }
    }
    free_picture (Iv);
    free_picture (Ih);

    return If;
}


float **gradiant_angle(picture *grad_x, picture *grad_y)
{
    int i, j;

    float **angle = (float**)malloc(grad_x->hauteur * sizeof(float*));
    if (angle == NULL)
    {
        fprintf (stderr, "Error gradiant angle : malloc echoué\n");
        exit(EXIT_FAILURE);
    }

    for (i=0 ; i<grad_x->hauteur ; i++)
    {
        angle[i] = (float*)malloc(grad_x->largeur * sizeof(float));
        if (angle[i] == NULL)
        {
            fprintf (stderr, "Error gradiant angle : malloc echoué\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i=0 ; i<grad_x->hauteur ; i++)
    {
        for (j=0 ; j<grad_x->largeur ; j++)
        {
            angle[i][j] = atan2 ((grad_y->pixels[i][j]), (grad_x->pixels[i][j]));
        }
    }

    return angle;
}



unsigned char interpolation_pgm(float x, float y, picture *image)
{
    int xbegin, xend, ybegin, yend, t, u, u1, t1;
    xbegin = floor (x);
    xend = ceil (x);
    ybegin = floor (y);
    yend = ceil (y);
    t = x-xbegin;
    u = y-ybegin;
    u1 = 1.0-u;
    t1 = 1.0-t;
    if (xbegin >= 0 && xend <= image->largeur && ybegin >= 0 && ybegin <= image->hauteur)
    {
        if (ybegin == image->hauteur - 1)
        {
            if (xbegin == image->largeur - 1)
                return image->pixels[ybegin][xbegin];
            return t * image->pixels[ybegin][xbegin] + t1 *image->pixels[yend][xend];
        }
        if (xbegin == image->largeur-1)
            return u * image->pixels[ybegin][xbegin] + u1 *image->pixels[yend][xend];
        return t1*u1*image->pixels[ybegin][xbegin]+t*u1*image->pixels[ybegin][xend]+t*u*image->pixels[yend][xend]+t1*u*image->pixels[yend][xbegin];
    }
    return 0;
}


void non_maxima_suppression(picture *norm, float **angle)
{
    int i, j;
    float c1, s1, g1, g2;
    for (i=0; i<norm->hauteur; i++)
        for (j=0; j<norm->largeur ; j++)
        {
            if (norm->pixels[i][j] != 0)
            {
                c1 = cos (angle[j][i]);
                s1 = sin (angle[j][i]);
                g1 = interpolation_pgm (j+c1, i+s1, norm);
                g2 = interpolation_pgm (j-c1, i-s1, norm);
                if (norm->pixels[i][j] < g1 && norm->pixels[i][j] < g2)
                {
                    norm->pixels[i][j] = 0;
                }
            }
        }
}


list* push_element (list* L, int x, int y)
{
    list* res = (list*)malloc(sizeof(list));

    if (res == NULL)
    {
        fprintf (stderr, "Error push element : malloc echoué\n");
        exit(EXIT_FAILURE);
    }

    res->x = x;
    res->y = y;
    res->next = L;
    return res;
}


list* suppression_element (list* L)
{
    list* T = L;
    if (T != NULL)
    {
        L = L->next;
        free(T);
    }
    return L;
}



void print_list (list* L)
{
    while (L != NULL)
    {
        printf ("(%d, %d) -> ", L->x, L->y);
        L = L->next;
    }
    printf ("NULL\n");
    
}

void free_list(list *L)
{
    if(L == NULL)
        return;
    free_list(L->next);
    free(L);
}

void hysteresis_thresholding(picture *image)
{
    list* L = init_list();
    int i, j;
    for (i=0 ; i<image->hauteur ; i++)
    {
        for (j=0 ; j<image->largeur ; j++)
        {
            if (image->pixels[i][j] < SUEIL_MIN)
                image->pixels[i][j] = 0;

            else
            {
                if (image->pixels[i][j] > SUEIL_MAX)
                    L = push_element (L, j, i);
                else
                {
                    for (int k=i-1 ; k<i+2 ; k++)
                    {
                        for (int t=j-1 ; t<j+2 ; t++)
                        {
                            if (image->pixels[k][t] > SUEIL_MAX)
                            {
                                L = push_element (L, j, i);
                            }
                            else 
                            {
                                image->pixels[i][j] = 0;
                            }
                            
                        }
                    }
                }
            }
        }
    }


    while (L != NULL)
    {
        image->pixels[L->y][L->x] = 255;
        L = suppression_element (L);
    }

    free_list (L);
    
}

picture *canny_edge_detector(picture *image)
{
    int i;
    picture* grad_x = sobel_x (image);
    picture* grad_y = sobel_y (image);

    image = sobel_edge_detector (image);
    float** angle = gradiant_angle (grad_x, grad_y);

    non_maxima_suppression (image, angle);

    hysteresis_thresholding (image);


    for (i=0 ; i<image->hauteur ; i++)
        free(angle[i]);
    free (angle);

    free_picture (grad_x);
    free_picture (grad_y);

    return image;
}


int main (void)
{
    picture* image = get_picture ("lena.pgm");
    picture* image_sobel;

    image_sobel = naive_edge_detector (image);
    
    write_picture (image_sobel, "lena_naive.pgm", 0);
    

    free_picture (image);
    free_picture (image_sobel);

    return 0;
}