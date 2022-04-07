#ifndef __TP3_H__
#define __TP3_H__

#include <math.h>





void free_picture(picture *image);

picture *get_picture(char *file_name);
void write_picture(picture *image, char *file_name, int binary);

#endif
