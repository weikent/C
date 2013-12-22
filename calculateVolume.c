
#include <stdio.h>

#define PI 3.1415926

int main(int argc, char *argv[])
{
  int diameter;

  printf ("input boll's diameter: ");
  scanf ("%d", &diameter);


  float volume;

  volume = 4.0f / 3.0f * PI * (diameter / 2) * (diameter / 2) * (diameter / 2);

  printf ("Volume is %f\n",volume);


 /*  double a; */
/*   a = 44444444.5555; */
/*   printf ("%3.3f\n",a); */
/*   printf ("%10f\n",a); */
/*   printf ("%22f\n",a); */
/*   printf ("%.10f\n",a); */

/* printf ("%20.6f\n",a); */
/*   printf ("%f\n",a); */
/*   printf ("%022f\n",a); */

/*   double db; */
/*   db = 0.0000009979; */
/*   printf ("%-6.2g\n",db); */

/*   float x; */
/*   x = 839.21f; */
/*   printf ("|%10.3f|%10.7e|%-10g\n",x,x,x); */

/*   int b, c, d; */
/*   printf ("input :"); */
/*   scanf ("%d%d%d", &b,&c,&d); */
/*   printf ("%d   %d   %d\n",b, c, d); */

/*   scanf ("%d, %d", &b,&c); */
/*   printf ("%d   %d\n",b, c); */

  /* int ii; */
  /* float xx, yy; */
  /* scanf ("%f%d%f", &xx, &ii, &yy); */
  /* printf ("%f     %d     %f\n",xx, ii,  yy); */

  int ee;
  ee = -4;
  printf ("%d\n",-8 / 5);
  printf ("%d\n",8 / 5);
  printf ("%d\n",8 / -5);
  printf ("%d\n",-8 / -5);
  return 0;
}

