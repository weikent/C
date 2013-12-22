
/* Converts a Fahrenheit temperature to Celsius */

#include <stdio.h>

#define FREEZING_PT 32.0f
#define SCALE_FACTOR (5.0f / 9.0f)


int main(int argc, char *argv[])
{
  float fahrenheit, celsius;

  printf ("Enter Fahrenheit tempreature: ");

  scanf ("%f", &fahrenheit);

  celsius = (fahrenheit - FREEZING_PT) * SCALE_FACTOR;

  printf ("Celsius equivalent: %.1f\n",celsius);

  return 0;
}

