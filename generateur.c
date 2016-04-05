#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Genere un entier entre a et b
 * @param a borne inferieure
 * @param b borne superieure
 */
int rand_a_b (int a, int b){
  return rand()%(b-a )+a;
}

int main( int argc, char* argv []){
  int N, xmin , xmax, ymin, ymax; //N=nb  de points et coordonnées du cadre 
  srand(time(NULL));
  if (argc!=6){
    printf ("nb invalide d'arguments\n");
    printf("usage: %s <nbDePoints> <xmin> <xmax> <ymin> <ymax>\n", argv[0]); 
    return 1;
   }
   N=atoi(argv[1]);
   xmin=atoi(argv[2]);
   xmax=atoi(argv[3]);
   ymin=atoi(argv[4]);
   ymax=atoi(argv[5]);
   int i;
   int x, y; 
   for (i=0;i<N;i++){
    x=rand_a_b(xmin,xmax);
    y=rand_a_b(ymin,ymax);
    printf ("%d %d\n", x, y);
   }
}
