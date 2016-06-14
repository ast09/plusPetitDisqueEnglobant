#include <stdio.h>


#include "structures.h"
#include "fonctions_fischer.h"


int main()
{

    CERCLE vraiC; //Solution universelle pour les 3 tests
    vraiC.x = 10.0; vraiC.y=10.0; vraiC.d=10.0; 
    int nbPoints;

    // Test 1 : Points cocirculaires

    
    printf("\n** Test 1 : Points cocirculaires **\n\n");

    POINT tab1[] = {{10.0,15.0},{10.0,5.0},{5.0,10.0},{15.0,10.0}}; 
    nbPoints = sizeof(tab1) / sizeof (tab1[0]);


    CERCLE c1 = algorithme_fischer(tab1,nbPoints);
    printf("Cercle solution: Coordonnées x=%f, y=%f, diamètre=%lf.\n",c1.x,c1.y,c1.d);
    printf("Le résultat devrait être x=10, y=10, d=10\n");

    // Test 2 : Points alignés

    POINT tab2[] = {{10.0,15.0},{10.0,14.0},{10.0,5.0},{10.0,7.0}}; 
    printf("\n** Test 2 : Points alignés **\n\n");
    nbPoints = sizeof(tab2) / sizeof (tab2[0]);

    
    CERCLE c2 = algorithme_fischer(tab2,nbPoints);
    printf("Cercle solution: Coordonnées x=%f, y=%f, diamètre=%lf.\n",c2.x,c2.y,c2.d);
    printf("Le résultat devrait être x=10, y=10, d=10\n");

    // Test 3 : Points confondus

    POINT tab3[] = {{10.0,10.0},{10.0,10.0},{10.0,10.0},{10.0,15.0},{10.0,5.0}};    
    printf("\n** Test 3 : Points confondus **\n\n");

    nbPoints = sizeof(tab3) / sizeof (tab3[0]);

    CERCLE c3 = algorithme_fischer(tab3,nbPoints);
    printf("Cercle solution: Coordonnées x=%f, y=%f, diamètre=%lf.\n",c3.x,c3.y,c3.d);
    printf("Le résultat devrait être x=10, y=10, d=10\n");



    // Résultat

    if ( estEgalCercle(c1, vraiC) && estEgalCercle(c2, vraiC) && estEgalCercle(c3, vraiC) ){
       printf(" \n\n     => Résultat CONFORME! \n"); 
    } else {
       printf(" \n\n     => Résultat NON CONFORME! \n");
    }

    return 0;
}