
#include "fonction_megiddo.h"
#include "structure.h"
#include "utils_table.h"

int main(int argc, char* argv []){
  
	POINT tab[NB_POINTS] = {{2,1},{3,-3},{5,4},{6,2},{7,-1},{7,-2},{8,2},{8,5},{9,-4},{10,-1},{10,3},{5,-1}};	
	//afficherTableau(tab);
	// int resultat;
	int new_point=NB_POINTS;
	int compteur=1;
	// printf("size of resultat %d\n",sizeof(tab)/sizeof(POINT));
	// printf("size of POINT %d\n",sizeof(POINT));
	//POINT resultat[9]=pruning(tab,NB_POINTS);
	//new_point=pruning(tab,40);
	while(new_point>3){
		new_point=pruning(tab,new_point,0);
		printf("après élagage %d\n",compteur);
		compteur+=1;
		afficherTableauPoint(tab,new_point);
	}
	printf("new_point= %d\n",new_point );
	printf("---------------------\n");
	return 1;
	
	

	


	//quickSortPoint(tab,0,9);
	//afficherTableauPoint(tab,NB_POINTS);
	// resultat[nbPoints];

	//DOUBLET *critique=malloc((NB_POINTS/2)*sizeof(DOUBLET));
	//printf("---------------\n");
	//critique=tableauValeurCritique(resultat,NB_POINTS);
	//afficherTableauDoublet(critique,NB_POINTS/2);
	//quickSortDoublet(critique,0,NB_POINTS/2-1);
	//printf("trie du tableau: \n");
	//afficherTableauDoublet(critique,NB_POINTS/2);
	//printf("valeur médiane:%lf \n", mediane(critique,NB_POINTS/2));
	//printf("pruning...\n");
	//prune=pruning(prune,critique,NB_POINTS);
	//afficherTableauPoint(prune,NB_POINTS*(3/4));


}



