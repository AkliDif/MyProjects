#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define N 1 //interpolation d'ordre 1


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

void extract_pgm_ascii(unsigned char **pixels, int hauteur, int largeur, FILE *f)
{
  int tmp;
  for(int i = 0 ; i < hauteur ; i++)
    for(int j = 0 ; j < largeur ; j++)
      {
	fscanf(f,"%d",&tmp);
	pixels[i][j] = (unsigned char) tmp;
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

unsigned char interpolation_pgm(picture *image, double x, double y)
{
    unsigned char res = 0;
    int xbegin, xend, ybegin, yend;
    xbegin = floor (x - (N+1)/2.);
    xend = ceil (x + (N+1)/2.);
    ybegin = floor (y - (N+1)/2.);
    yend = ceil (y + (N+1)/2.);


    if (xbegin > 0 && xend < image->largeur && ybegin > 0 && yend < image->hauteur)
    {
      for (int i =xbegin ; i<xend ; i++)
      {
          for (int j=ybegin ; j<yend ; j++)
          {
              res += (*W[N])(y-j) * (*W[N])(x-i) * image->pixels[j][i];
          }
      }
    }
    else
    {
      res = image->value_max;
    }
    
    return round (res);
}


RGB interpolation_ppm(picture *image, double x, double y)
{
    RGB res = {0, 0, 0};

    int xbegin, xend, ybegin, yend;
    
    xbegin = floor (x - (N+1)/2.);
    xend = ceil (x + (N+1)/2.);
    ybegin = floor (y - (N+1)/2.);
    yend = ceil (y + (N+1)/2.);


    if (xbegin > 0 && xend < image->largeur && ybegin > 0 && yend < image->hauteur)
    {
      for (int i =xbegin ; i<xend ; i++)
      {
          for (int j=ybegin ; j<yend ; j++)
          {
              res.R += (*W[N])(y-j) * (*W[N])(x-i) * image->pixels_rgb[j][i].R;
              res.G += (*W[N])(y-j) * (*W[N])(x-i) * image->pixels_rgb[j][i].G;
              res.B += (*W[N])(y-j) * (*W[N])(x-i) * image->pixels_rgb[j][i].B;
          }
      }
    }
    else
    {
      res.R = image->value_max;
      res.G = image->value_max;
      res.B = image->value_max;
    }
    
    return res;
}


picture *rotation_pgm(picture *image, double theta, int x0, int y0)
{
    picture* image_mod = (picture*)malloc(sizeof(picture));

    double x_i, y_i;
    
    image_mod->hauteur = image->hauteur;
    image_mod->largeur = image->largeur;
    image_mod->type = image->type;
    image_mod->value_max = image->value_max;
    image_mod->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * image_mod->hauteur);
    if (image_mod->pixels == NULL)
    {
        fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_mod->hauteur ; i++)
    {
        image_mod->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) * image_mod->largeur);
        if (image_mod->pixels[i] == NULL)
        {
            fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }

    for (int y=0 ; y<image->hauteur ; y++)
    {
        for (int x=0; x<image->largeur ; x++)
        {
            x_i = x0 + (x - x0)*cos(-theta) + (y - y0)*sin(-theta);
            y_i = y0 - (x - x0)*sin(-theta) + (y - y0)*cos(-theta);
            
            image_mod->pixels[y][x] = interpolation_pgm (image, x_i, y_i);
        }
    }

    return image_mod;
}



picture *rotation_ppm(picture *image, double theta, int x0, int y0)
{
    picture* image_mod = (picture*)malloc(sizeof(picture));

    double x_i, y_i;
    
    image_mod->hauteur = image->hauteur;
    image_mod->largeur = image->largeur;
    image_mod->type = image->type;
    image_mod->value_max = image->value_max;
    image_mod->pixels_rgb = (RGB**)malloc(sizeof(RGB*) * image_mod->hauteur);
    if (image_mod->pixels_rgb == NULL)
    {
        fprintf (stderr, "Erreur rotation_ppm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_mod->hauteur ; i++)
    {
        image_mod->pixels_rgb[i] = (RGB*)malloc(sizeof(RGB) * image_mod->largeur);
        if (image_mod->pixels_rgb[i] == NULL)
        {
            fprintf (stderr, "Erreur rotation_ppm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }

    for (int y=0 ; y<image->hauteur ; y++)
    {
        for (int x=0; x<image->largeur ; x++)
        {
            x_i = x0 + (x - x0)*cos(-theta) + (y - y0)*sin(-theta);
            y_i = y0 - (x - x0)*sin(-theta) + (y - y0)*cos(-theta);

                    image_mod->pixels_rgb[y][x] = interpolation_ppm (image, x_i, y_i);
        }
    }

    return image_mod;
}

picture *rotation(picture *image, double theta, int x0, int y0)
{
    if (image->type == 0)
      image = rotation_pgm (image, theta, x0, y0);
    else
      image = rotation_ppm (image, theta, x0, y0);
    return image;
}


picture *zoom_pgm(picture *image, double lambda, int x0, int y0, int Dx, int Dy)
{
  picture* image_mod = (picture*)malloc(sizeof(picture));

    double x_i, y_i;
    int x, y;
    
    image_mod->hauteur =  Dy;
    image_mod->largeur =  Dx;
    image_mod->type = image->type;
    image_mod->value_max = image->value_max;
    image_mod->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * image_mod->hauteur);
    if (image_mod->pixels == NULL)
    {
        fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_mod->hauteur ; i++)
    {
        image_mod->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) * image_mod->largeur);
        if (image_mod->pixels[i] == NULL)
        {
            fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }


    for (y=0 ; y<image_mod->hauteur; y++)
    {
        for (x=0 ; x<image_mod->largeur; x++)
        {
                  x_i = x/lambda;
                  y_i = y/lambda;
                  image_mod->pixels[y][x] = interpolation_pgm (image, x_i, y_i);
        }
    }

    return image_mod;
}


picture *zoom_ppm(picture *image, double lambda, int x0, int y0, int Dx, int Dy)
{
  picture* image_mod = (picture*)malloc(sizeof(picture));

    double x_i, y_i;
    int x, y;
    
    image_mod->hauteur =  Dy;
    image_mod->largeur =  Dx;
    image_mod->type = image->type;
    image_mod->value_max = image->value_max;
    image_mod->pixels_rgb = (RGB**)malloc(sizeof(RGB*) * image_mod->hauteur);
    if (image_mod->pixels_rgb == NULL)
    {
        fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_mod->hauteur ; i++)
    {
        image_mod->pixels_rgb[i] = (RGB*)malloc(sizeof(RGB) * image_mod->largeur);
        if (image_mod->pixels_rgb[i] == NULL)
        {
            fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }


    for (y=0 ; y<image_mod->hauteur; y++)
    {
        for (x=0; x<image_mod->largeur; x++)
        {
                  x_i = x/lambda;
                  y_i = y/lambda;
                  image_mod->pixels_rgb[y][x] = interpolation_ppm (image, x_i, y_i);
        }
    }

    return image_mod;
}


picture *zoom(picture *image, double lambda, int x0, int y0, int Dx, int Dy)
{ 
    if (image->type == 0)
      image = zoom_pgm (image, lambda, x0, y0, Dx, Dy);
    else
      image = zoom_ppm (image, lambda, x0, y0, Dx, Dy);
    return image;
}



picture *shear_pgm(picture *image, double cx, double cy, int Dx, int Dy)
{
  picture* image_mod = (picture*)malloc(sizeof(picture));

    double x_i, y_i;
    int x, y;
    
    image_mod->hauteur =  Dy;
    image_mod->largeur =  Dx;
    image_mod->type = image->type;
    image_mod->value_max = image->value_max;
    image_mod->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * image_mod->hauteur);
    if (image_mod->pixels == NULL)
    {
        fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_mod->hauteur ; i++)
    {
        image_mod->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) * image_mod->largeur);
        if (image_mod->pixels[i] == NULL)
        {
            fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }

    for (y=0 ; y<image_mod->hauteur; y++)
    {
        for (x=0; x<image_mod->largeur; x++)
        {
                  x_i =(x-(cx*y))/(1-(cx*cy));
                  y_i =(y-(cy*x))/(1-(cx*cy));
                  image_mod->pixels[y][x] = interpolation_pgm(image, x_i, y_i);
        }
    }

    return image_mod;
}


picture *shear_ppm(picture *image, double cx, double cy, int Dx, int Dy)
{
  picture* image_mod = (picture*)malloc(sizeof(picture));

    double x_i, y_i;
    int x, y;
    
    image_mod->hauteur =  Dy;
    image_mod->largeur =  Dx;
    image_mod->type = image->type;
    image_mod->value_max = image->value_max;
    image_mod->pixels_rgb = (RGB**)malloc(sizeof(RGB*) * image_mod->hauteur);
    if (image_mod->pixels_rgb == NULL)
    {
        fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
        exit (EXIT_FAILURE);
    }
    for (int i=0 ; i<image_mod->hauteur ; i++)
    {
        image_mod->pixels_rgb[i] = (RGB*)malloc(sizeof(RGB) * image_mod->largeur);
        if (image_mod->pixels_rgb[i] == NULL)
        {
            fprintf (stderr, "Erreur rotation_pgm : malloc echouée!");
            exit (EXIT_FAILURE);
        }
    }


    for (y=0 ; y<image_mod->hauteur; y++)
    {
        for (x=0; x<image_mod->largeur; x++)
        {
                  x_i =(x-(cx*y))/(1-(cx*cy));
                  y_i =(y-(cy*x))/(1-(cx*cy));
                  image_mod->pixels_rgb[y][x] = interpolation_ppm(image, x_i, y_i);
        }
    }

    return image_mod;
}

picture *shear(picture *image, double cx, double cy, int Dx, int Dy)
{
  if (image->type == 0)
    image = shear_pgm(image, cx, cy, Dx, Dy);
  else 
    image = shear_ppm(image, cx, cy, Dx, Dy);
  return image;
}


int main (void)
{
    double lambda = 1.5;
    double cx = 0.4;
    double cy = 0.4;
    picture *image_pgm = get_picture("shanon.pgm");

    picture *image_ppm = get_picture("lena.ppm");

/*------------------------------PGM----------------------------------------*/

    picture * image_mod_pgm = rotation (image_pgm, M_PI/9, image_pgm->largeur/2, image_pgm->hauteur/2);
    write_picture(image_mod_pgm,"shanon_rotate.pgm" , 0);
    free_picture (image_mod_pgm);

    image_mod_pgm = zoom(image_pgm, lambda, image_pgm->largeur/2, image_pgm->hauteur/2, image_pgm->largeur * lambda, image_pgm->hauteur * lambda);
    write_picture(image_mod_pgm,"shanon_zoom.pgm" , 0);
    free_picture (image_mod_pgm);

    image_mod_pgm = shear(image_pgm, cx, cy, image_pgm->largeur * cx + image_pgm->largeur, image_pgm->hauteur * cy + image_pgm->hauteur);
    write_picture(image_mod_pgm,"shanon_shear.pgm" , 0);
    free_picture (image_mod_pgm);
/*------------------------------PPM----------------------------------------*/

    picture* image_mod_ppm = rotation (image_ppm, M_PI/9, image_ppm->largeur/2, image_ppm->hauteur/2);
    write_picture(image_mod_ppm,"lena_rotate.ppm" , 0);
    free_picture (image_mod_ppm);

    image_mod_ppm = zoom(image_ppm, lambda, image_ppm->largeur/2, image_ppm->hauteur/2, image_ppm->largeur * lambda, image_ppm->hauteur * lambda);
    write_picture(image_mod_ppm,"lena_zoom.ppm" , 0);
    free_picture (image_mod_ppm);

    image_mod_ppm = shear(image_ppm, cx, cy, image_ppm->largeur * cx + image_ppm->largeur, image_ppm->hauteur * cy + image_ppm->hauteur);
    write_picture(image_mod_ppm,"lena_shear.ppm" , 0);
    free_picture (image_mod_ppm);

    free_picture (image_pgm);
    free_picture (image_ppm);
    return 0;
}