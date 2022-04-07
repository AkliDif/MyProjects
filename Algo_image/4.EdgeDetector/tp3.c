#include <stdio.h>
#include <stdlib.h>

#include <math.h>

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


double get_Y_component_from_RGB(RGB pixel)
{
  double res = 0.299*pixel.R + 0.587*pixel.G + 0.114*pixel.B;

  return res;
}


picture* ppm_to_pgm(picture *image)
{
  if( image == NULL )
    {
      fprintf(stderr,"Erreur ppm_to_pgm : uninitialized parameter!\n");
      exit(1);
    }

  if( image->type != 1 )
    {
      fprintf(stderr,"Erreur ppm_to_pgm : parameter is not a ppm picture!\n");
      exit(1);
    }

  unsigned int largeur = image->largeur;
  unsigned int hauteur = image->hauteur;
  
  picture *res = (picture*) malloc(sizeof(picture));
  
  if(res == NULL)
    {
      fprintf(stderr,"Erreur ppm_to_pgm : malloc picture pgm failed!\n");
      exit(1);
    }

  res->type = 0;
  res->largeur = image->largeur;
  res->hauteur = image->hauteur;
  res->value_max = image->value_max;
  
  
  res->pixels = (unsigned char**) malloc(largeur*sizeof(unsigned char*));
  if (res->pixels == NULL)
    {
      fprintf(stderr,"Erreur ppm_to_pgm : malloc pixels failed!\n");
      exit(1);
    }
  for(int i = 0 ; i < largeur ; i++)
    {
      res->pixels[i] = (unsigned char*) malloc(hauteur*sizeof(unsigned char));
      if (res->pixels[i] == NULL)
	{
	  fprintf(stderr,"Erreur ppm_to_pgm : malloc pixels[%d] failed!\n",i);
	  exit(1);
	}
    }

  for(int i = 0 ; i < image->largeur ; i++)
    for(int j = 0 ; j < image->hauteur ; j++)
      res->pixels[i][j] = (unsigned char) round(get_Y_component_from_RGB(image->pixels_rgb[i][j]));	

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

  
# define N 8

double Q[N][N] = {
  { 16,  12,  14,  14,  18,  24,  49,  72 },
  { 11,  12,  13,  17,  22,  35,  64,  92 },
  { 10,  14,  16,  22,  37,  55,  78,  95 },
  { 16,  19,  24,  29,  56,  64,  87,  98 },
  { 24,  26,  40,  51,  68,  81, 103, 112 },
  { 40,  58,  57,  87, 109, 104, 121, 100 },
  { 51,  60,  69,  80, 103, 113, 120, 103 },
  { 61,  55,  56,  62,  77,  92, 101,  99 }
  };

void extract_bloc(picture *image, double bloc[N][N], unsigned int i, unsigned int j)
{
  for( unsigned int k = 0 ; k < N ; k ++ )
    for( unsigned int l = 0 ; l < N ; l ++ )
      bloc[k][l] = image->pixels[i+k][j+l];
}

void DCT(double bloc[N][N])
{
  double res[N][N];

  for(int i = 0 ; i < N ; i++)
    for(int j = 0 ; j < N ; j++)
      {
	res[i][j] = 0;

	for(int k = 0 ; k < N ; k++)
	  for(int l = 0 ; l < N ; l++)
	    res[i][j] += bloc[k][l] * cos((2*k+1)*i*M_PI/(2*N)) * cos((2*l+1)*j*M_PI/(2*N));

	res[i][j] *= 2./N;
	
	if(i == 0)
	  res[i][j] /= sqrt(2);
	
	if(j == 0)
	  res[i][j] /= sqrt(2);
      }

  for(int i = 0 ; i < N ; i++)
    for(int j = 0 ; j < N ; j++)
      bloc[i][j] = res[i][j];
}

void quantify(double bloc[N][N])
{
  for(int i = 0 ; i < N ; i++)
    for(int j = 0 ; j < N ; j++)
      bloc[i][j] /= Q[i][j];
}

void zigzag_extraction(double bloc[N][N], int zigzag[N*N])
{
  int i = 0, j = 0, cpt = 0;
  zigzag[cpt] = bloc[i][j];
  cpt++;
  /* Tant que l'on atteindra pas le coin en bas à gauche */
  while ( (i != N-1) || (j != N-1) )
    {
      if ( j == 0 && i < N-1 )
	{
	  i++;
	  zigzag[cpt] = (int) round(bloc[i][j]);
	  cpt++;

	  while( i > 0 )
	    {
	      i--;
	      j++;
	      zigzag[cpt] = (int) round(bloc[i][j]);
	      cpt++;
	    }
	}
      else if ( i == N-1 && j < N-1 )
	{
	  j++;
	  zigzag[cpt] = (int) round(bloc[i][j]);
	  cpt++;

	  while( j < N-1 )
	    {
	      i--;
	      j++;
	      zigzag[cpt] = (int) round(bloc[i][j]);
	      cpt++;
	    }
	}
      else if ( i == 0 && j < N-1 )
	{
	  j++;
	  zigzag[cpt] = (int) round(bloc[i][j]);
	  cpt++;
	      
	  while( j > 0 )
	    {
	      i++;
	      j--;
	      zigzag[cpt] = (int) round(bloc[i][j]);
	      cpt++;
	    }
	}
      else if ( j == N-1 && i < N-1 )
	{
	  i++;
	  zigzag[cpt] = (int) round(bloc[i][j]);
	  cpt++;
	      
	  while( i < N-1 )
	    {
	      i++;
	      j--;
	      zigzag[cpt] = (int) round(bloc[i][j]);
	      cpt++;
	    }
	}
    }
}

void compress_RLE(FILE *f, int zigzag[N*N])
{
  int index = 0;
  while (index < N*N)
    {
      while (zigzag[index] != 0)
	{
	  fprintf(f,"%d\n", zigzag[index] );
	  index++;
	}

      int nbZeros = 0;
      while( index < N*N && zigzag[index] == 0)
	{
	  index++;
	  nbZeros++;
	}
      if( nbZeros >=2)
	fprintf(f,"@%d\n", nbZeros);
      else
	fprintf(f,"0\n");
    }
}

void jpeg_compression(picture *image, char *fileName)
{  
  FILE *f = fopen(fileName,"w+");
  if( f == NULL)
    {
      fprintf(stderr,"Error jpegCompression : fopen failed!\n");
      exit(1);
    }
  
  fprintf(f,"JPEG\n");
  fprintf(f,"%u %u\n", image->largeur, image->hauteur);

  double bloc[N][N];
  int zigzag[N*N];
  for(int i = 0 ; i < image->largeur ; i+=N)
    {
      for(int j = 0 ; j < image->hauteur ; j+=N)
	{	  
	  extract_bloc(image,bloc,i,j);
	  
	  DCT(bloc);

	  quantify(bloc);

	  zigzag_extraction(bloc,zigzag);
	  
	  compress_RLE(f,zigzag);
	}
    }
  
  fclose(f);
}

unsigned int file_size(char *fileName)
{
  FILE *f = fopen(fileName,"r+");
  if( f == NULL)
    {
      fprintf(stderr,"Error fileSize : fopen failed!\n");
      exit(1);
    }

  unsigned int res = 0;
  char c = getc(f);
  while(c != EOF)
    {
      res++;
      c = getc(f);
    }
  
  fclose(f);

  return res;
}
/*
int main(int argc, char *argv[])
{
  if(argc < 2)
    {
      printf("Veuillez préciser le nom du fichier ppm à convertir.\nPar exemple : ./main lena.ppm\n");
      return 1;
    }
  
  int indexName = 0;
  while(argv[1][indexName] != '.')
    indexName++;
*/
  /* On récupère le nom du nouveau fichier pgm */
  /*
  char pgmFileName[string_length(argv[1])+1];
  for (int i = 0 ; i <= indexName ; i++)
    pgmFileName[i] = argv[1][i];

  pgmFileName[indexName+1] = 'p';
  pgmFileName[indexName+2] = 'g';
  pgmFileName[indexName+3] = 'm';
  pgmFileName[indexName+4] = '\0';

  picture *imagePPM = get_picture(argv[1]);
  picture *imagePGM = ppm_to_pgm(imagePPM);

  write_picture(imagePGM, pgmFileName,0); // 1 for writing in binary

  unsigned int pgmSize = file_size(pgmFileName);    
  printf("Le fichier %s fait %u octets.\n",pgmFileName,pgmSize);
 
  \* On récupère le nom du nouveau fichier jpeg */
/*
  char jpegFileName[string_length(argv[1])+2];
  for (int i = 0 ; i <= indexName ; i++)
    jpegFileName[i] = argv[1][i];

  jpegFileName[indexName+1] = 'j';
  jpegFileName[indexName+2] = 'p';
  jpegFileName[indexName+3] = 'e';
  jpegFileName[indexName+4] = 'g';
  jpegFileName[indexName+5] = '\0';

  jpeg_compression(imagePGM, jpegFileName);

  unsigned int jpegSize = file_size(jpegFileName);
  
  printf("Le fichier %s fait %u octets.\n",jpegFileName,jpegSize);

  printf("Le taux de compression de ce fichier est de %.2lf\n",((double) jpegSize)/pgmSize);
  
  free_picture(imagePPM);
  free_picture(imagePGM);
  
  return 0;
}
 */