#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "structures.h"
#include "fonctions_fischer.h"

POINT first_point;		/* first hull POINT pour le calcul de l'enveloppe convexe*/


/****************************** fonctions print **************************************/

void print_point(POINT* p)
{
	printf(" ........ %f %f\n",p->x,p->y);
}

void print_droite(DROITE* d)
{
	printf(" ------ %f %f\n",d->a,d->b);
}

void print_cercle(CERCLE* c)
{
	printf("centre: %f %f, diametre: %f\n",c->x,c->y,c->d);
}

/*************************************************************************************/



/****************************** fonctions geometriques *******************************/

/**
 * calcule la distance entre les points a et b
**/
double distance(POINT a, POINT b){
	return ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}


/**
 * verifie si a,b,c alignes
**/
int collinear(POINT a, POINT b, POINT c)
{

	return (calculer_determinant3(a,b,c)*calculer_determinant3(a,b,c) <= EPSILON );
}

/**
 * Teste si les points du tableau sont alignes
 * retourne 1 si oui, 0 sinon
**/
int tableau_collinear(POINT T[], int nbPoints){
	POINT a = T[0];
	POINT b = T[1];
	int i=2;
	
	while(i<nbPoints){
		if(!collinear(a,b,T[i]))
			return 0;
		i++;
	}
	
	return 1;
}


/**
 * Teste si deux cercles c1 et c2 sont égaux
**/
int estEgalCercle( CERCLE c1, CERCLE c2 ){
    if( (c1.x == c2.x) && (c1.y == c2.y) && (c1.d == c2.d) )
        return 1;
    else
        return 0;
}

/**
 * Teste si deux points p1 et p2 sont égaux
**/
int estEgalPoint( POINT p1, POINT p2 ){
    return ( (p1.x == p2.x) && (p1.y == p2.y) );
}

/*
 * verifie si le point p == {0,0}
 */
int equals_zero(POINT p){

	if(p.x != 0.0 || p.y != 0.0){
		
		return (0);
	}
	else{
		return (1);
	}
}


/**
 * calcule le determinant (avec la regle du Sarrus):
 * | a.x  b.x  c.x |
 * | a.y  b.y  c.y |
 * |  1    1    1  |
 *
**/
double calculer_determinant3(POINT a, POINT b, POINT c){
	return a.x*b.y + a.y*c.x + b.x*c.y - b.y*c.x - a.y*b.x - a.x*c.y;
}

/**
 * calcule le determinant:
 * | a.x  b.x |
 * | a.y  b.y |
 *
**/
double calculer_determinant2(POINT a, POINT b){
	return a.x*b.y - a.y*b.x;
}


//retourne l'index du element qui a un coefficient negatif dans la representation p = somme ( coeff(i) * T[i])
int coefficients_negatifs(POINT p,POINT T[], int nbPoints){
	if(calculer_determinant3(T[2],T[1],T[0]) * calculer_determinant3(T[2],T[1],p) <0)
		return 0;
	else if(calculer_determinant3(T[2],T[0],T[1]) * calculer_determinant3(T[2],T[0],p) <0)
		return 1;
	else return 2;
}

/**
 * retourne 1 si p appartient a conv(T), 0 sinon
 *
**/
int appartenance_conv(POINT p,POINT T[], int nbPoints)
{
	
    if(nbPoints == 3)
    {
    	POINT tab01[2];
    	tab01[0] = T[0];
    	tab01[1] = T[1];

    	POINT tab02[2];
    	tab02[0] = T[0];
    	tab02[1] = T[2];

    	POINT tab12[2];
    	tab12[0] = T[1];
    	tab12[1] = T[2];

    	if(!tableau_collinear(T,nbPoints)){

	    	//sinon on a 3 points non colineaires, et donc p doit etre dans un triangle
	    	//si pour chaque segment du triangle p et l'autre point sont de meme cote ou si p est sur le segment, alors p est dans le triangle

	    	if(calculer_determinant3(T[0],T[1],p)*calculer_determinant3(T[0],T[1],T[2]) > 0 || appartenance_conv(p,tab01,2))
	    		if(calculer_determinant3(T[0],T[2],p)*calculer_determinant3(T[0],T[2],T[1]) > 0 || appartenance_conv(p,tab02,2))
	    			if(calculer_determinant3(T[2],T[1],p)*calculer_determinant3(T[2],T[1],T[0]) > 0 || appartenance_conv(p,tab12,2))
	    				return 1;
	    			else
	    				return 0;
	    		else 
	    			return 0;
	    	else
	    		return 0;
	    }
	    else{
	    	//si les points sont collineaires, il faut que p soit aussi sur la droite et il faut appartenir a un segment determine par 2 points
	    	return(collinear(T[0],T[1],p) && ( appartenance_conv(p,tab01,2) || appartenance_conv(p,tab02,2) || appartenance_conv(p,tab12,2) ));
	    }
    } 
    else if(nbPoints==2)
    {
    	//si on a 2 points dans T, p doit etre sur le segment qu'elles determinent
    	//les 3 points doivent etre donc colineailres et le produit scalaire pT[0] , pT[1] doit etre negatif ou nul
    	if( collinear(T[0],T[1],p) && (((T[0].x - p.x)*(T[1].x - p.x) + (T[0].y - p.y)*(T[1].y - p.y)) <= 0 ))
    		return 1;
    	else
    		return 0;
    }
    else
    {
    	//si on a 1 seul point, les points doivent etre confondus
    	if(distance(p,T[0])<=EPSILON)
    		return 1;
    	else
    		return 0;
    	
    }
}


/*
 *retourne 1 si p appartient a aff(T), 0 sinon
 */
int appartenance_aff(POINT p,POINT T[], int nbPoints){

	//si on a un seul point, il faut que p et T[0] coincident pour que p appartienne a aff(T)
	if(distance(p,T[0]) == 0){
		return 1;
	}
	else if(nbPoints==1){
		return 0;
	}
	else if(tableau_collinear(T,nbPoints))
	{
		//si les POINT de T sont allignes, il faut que p appartienne a la droite pour qu'il appartienne a aff(T)
		//on peut considerer ici le cas ou dans T il y a que 2 points aussi
		return collinear(p,T[0],T[1]);
	}
	else{ 
		//si les points ne sont pas allignes, l'espace affine c'est tout le plan et donc p y appartienne
		return 1;
	}
}

/*
 * Renvoie le cercle le plus petit passant par deux points p1 et p2
 */
CERCLE cerclePassantParDeuxPoints( POINT p1 , POINT p2){
	
	CERCLE c;
		
	c.x = (p1.x + p2.x)/2;
	c.y = (p1.y + p2.y)/2;
	c.d = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
	
	return c;	
}


/*
 * Renvoie le cercle le plus petit passant par trois points p1, p2 et p3
 */
CERCLE cerclePassantParTroisPoints( POINT p1 , POINT p2 , POINT p3){
	
	CERCLE c;
	double det =  (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x)* (p1.y - p2.y); 

    	if (det == 0) { 
    	
    	c = cerclePassantParDeuxPoints(p1,p2);

    	if(c.d < (cerclePassantParDeuxPoints(p3,p2).d))
    		c = cerclePassantParDeuxPoints(p3,p2);
    	else if(c.d < (cerclePassantParDeuxPoints(p3,p1).d))
    		c = cerclePassantParDeuxPoints(p3,p1);

	} else {
		int offset = pow(p2.x,2) + pow(p2.y,2);
    		int bc =   ( pow(p1.x,2) + pow(p1.y,2) - offset )/2.0;
    		int cd =   (offset - pow(p3.x, 2) - pow(p3.y, 2))/2.0;

	    	c.x =  (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) / det;
	    	c.y =  (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) / det;
	    	c.d = 2 * sqrt( pow(p2.x - c.x,2) + pow(p2.y-c.y,2));
	}
	return c;	
}

DROITE mediatrice(POINT p1, POINT p2)
{
	DROITE res;
	if(p1.y != p2.y){
		res.a = (-1.0)/ ((p1.y - p2.y)/(p1.x - p2.x));
		res.b = ((p1.y + p2.y)/2.0) - res.a * ((p1.x + p2.x)/2.0);
	} else{
		res.a = 0;
		res.b = 0;
	}
	return res;
}

POINT intersection(DROITE d1, DROITE d2)
{
	POINT res;
	if(d2.a - d1.a!=0)
	{
		res.x = (d1.b - d2.b)/(d2.a - d1.a);
		res.y = d1.a * res.x + d1.b;
	}
	else{
		res.x=0;
		res.y=0;
	}
	return res;
}

DROITE droitePassantParPoints(POINT p1, POINT p2)
{
	DROITE res;
	if(p1.x != p2.x){
		res.a = (p1.y - p2.y)/(p1.x - p2.x);
		res.b = p1.y - res.a * p1.x ;
	}
	else{
		res.a = 0;
		res.b = 0;
	}
	return res;
}


/**************************** fonctions pour les tableaux ****************************/




/*
 * initialisation tableau avec {0,0} partout
 */
void init_tab(POINT tab[], int nbPoints){
	int i;
	for(i=0;i<nbPoints;i++){
		tab[i].x=0;
		tab[i].y=0;
	}
}

/*
 * verifie si le POINT element se trouve dans le tableau tab
 * retourne 0 si on trouve l'element dans le tableau, 1 sinon
 */
int not_in(POINT tab[], POINT element, int nbPoints){
	int i;
	for(i=0;i<nbPoints;i++)
		if(estEgalPoint(tab[i], element))
			return 0;
	return 1;
}



/**************************** fonctions specifiques a l'algo *************************/




/*
 *on efface du tableau T un element qui a des coeffs negatifs dans la decomposition c = coeff[i]*T[i]
 */
int dropping(POINT c,POINT T[], int nbPoints){

	int i;
	int index = coefficients_negatifs(c, T, nbPoints);

	for(i=index;i<nbPoints-1;i++)
		T[i]=T[i+1];
	T[nbPoints-1].x = 0;
	T[nbPoints-1].y = 0;

	return 1;
}


/*
 * prend en paramentre le set S et renvoie le cercle minimal contenant S
 */
CERCLE algorithme_fischer(POINT S[], int nbPoints){

	CERCLE resultat;

	POINT centreTemp;
	POINT c,p,cc;

	POINT T[3]; // le set support

	DROITE d1;
	DROITE d2;

	double max=0;
	double det;

	int i,j;
	int nbPointsT=1;
	int compteur = 0;

	//les iterations sont faites pour les paires c,T
	//c est le centre du cercle courant et T le set des points support

	c = S[0]; //au debut, c est pris au hasard
	init_tab(T,3);
	
	//on cherche le point le plus eloigne de c
	for(i=0; i<nbPoints;i++)
    {
    	if(max <= distance(c,S[i])){
    		max = distance(c,S[i]);
    		T[0] = S[i];
    	}
    }

	while(!appartenance_conv(c,T,nbPointsT)){
		compteur++;
						
		if(appartenance_aff(c,T,nbPointsT))
		{
			//si c appartient a aff(T) on sait qu'on peut enlever un point par le dropping
			if(dropping(c,T,nbPointsT)){
				nbPointsT--;
			}			
		}
		
		if(nbPointsT == 1)
		{	
			//on a 1 seul point, faut rajouter encore 1 point dans T
			
			p.x =0;
			p.y =0;
			max=0;


			for(i=0;i<nbPoints-1;i++){
				compteur++;

				if(not_in(T,S[i],nbPointsT)){ 

					//pour toutes les points de S qui sont pas dans T on calcule le nouveau cercle qu'elles determinent

					//calcul du centre du cercle temporaire
					d1 = mediatrice(T[0],S[i]);
					d2 = droitePassantParPoints(T[0],c);

					if(d1.a == 0 && d1.b == 0){
						centreTemp.x = T[0].x;
						centreTemp.y = d2.a * T[0].x + d2.b;
					}
					else if(d2.a == 0 && d2.b == 0){
						centreTemp.x = T[0].x;
						centreTemp.y = d1.a * T[0].x + d1.b;
					}
					

					//si on a un cercle maximal on sait qu'on a tombe sur le point qui a arrete la marche du centre du cercle vers cc(T)
					//on reactualise donc c
					if(!equals_zero(centreTemp)){
						if(distance(centreTemp,S[i]) > max){
							p=S[i];
							c.x = centreTemp.x;
							c.y = centreTemp.y;
							max = distance(centreTemp,S[i]);
						}
					}
				}
			}

			
			if(!equals_zero(p)){
				T[1] = p;
				nbPointsT++;
			}
			else{
				//si on n'a pas trouve aucun point qui arrete la marche, alors c atteint cc(T) qui sera en fait le milieu entre c et T[0]
				if(!not_in(S,c,nbPoints)){
					T[1] = c;
					nbPointsT++;
				}
				c.x = (T[0].x + T[1].x)/2.0;
				c.y = (T[0].y + T[1].y)/2.0;
			}
		}
		else if(nbPointsT==2) 
		{
			// on a 2 points dans T et on cherche le 3eme point qui va arreter la marche

			cc.x = (T[0].x+T[1].x)/2;
			cc.y = (T[0].y+T[1].y)/2;

			det = calculer_determinant3(T[0],T[1],c);
			max = 0;
			for(j=0;j<nbPoints;j++){
				compteur++;

				//on cherche le puint "stopper" entre les points de S/T qui se trouvent de meme cote que c par rapport aux autres points de T
				//on cherche encore une fois un cercle maximal pour etre surs que c'est bien le point qui arrete la marche
				if(not_in(T,S[j],nbPointsT) && det * calculer_determinant3(T[0],T[1],S[j]) > 0 && distance(cc,S[j]) > distance(cc,T[0]))
				{
					//calcul du centre du cercle temporaire grace aux mediatrices
					d1 = mediatrice(T[0],T[1]);
					d2 = mediatrice(T[0],S[j]);
					if(d1.a == 0 && d1.b == 0){
						centreTemp.x = T[0].x;
						centreTemp.y = d2.a * T[0].x + d2.b;
					}
					else if(d2.a == 0 && d2.b == 0){
						centreTemp.x = T[0].x;
						centreTemp.y = d1.a * T[0].x + d1.b;
					}
					else{
						centreTemp = intersection(d1,d2);	
					}
					
					if(distance(centreTemp,S[j])>max && calculer_determinant3(T[0],T[1],c) * calculer_determinant3(T[0],T[1],centreTemp) >= 0){
						T[2]=S[j];
						max=distance(centreTemp,S[j]);
						c=centreTemp;
					}
				}
			}

			if(T[2].x == 0 && T[2].y == 0 )
			{
				//si on n'a pas trouve de point, alors c atteint cc(T) qui est le milieu du segment T[0]T[1]
				c.x = (T[0].x + T[1].x)/2;
				c.y = (T[0].y + T[1].y)/2;
			}
			else{
				nbPointsT++;
			}
		}
	}
    //on renvoye le cercle resultat de centre c et diametre = 2 * distance (c,T[0])
	resultat.x = c.x;
	resultat.y = c.y;
	resultat.d = 2*sqrt(distance(c,T[0]));

	printf("\non a fait %d iterations \n",compteur);			
	return resultat;
}




/**************************** fonctions resolution brute *************************/

//Renvoie 1 si tous les point du tableau tab sont contenus dans le cercle c

int contientTousPoint( CERCLE c , POINT tab[] , int nbPoints ){
	
	int i = 0;	
	int ok = 1;
	
	while(i<nbPoints && ok) {

		if (!contientPoint( c, tab[i] ))
		{
			ok = 0;
		}

	   	i++;
	}
	
	return ok;
}

int contientPoint( CERCLE c , POINT p){

	POINT temp;
	temp.x = c.x;
	temp.y = c.y;

	if(sqrt(distance(temp,p)) * 2 - c.d > EPSILON){
		return 0;
	}
	else
		return 1;

}

/**
 * @param tab Tableau contenant les points
 * @return cFinal CERCLE solution finale
**/
CERCLE brute( POINT tab[] , int nbPoints ){

	CERCLE cFinal , cTemp;
	cFinal.d = 100000000000; cFinal.x = 0; cFinal.y=0;	

	POINT p1 , p2 , p3;
	
	int i , j , k;
	int compteur = 0;

	if(nbPoints == 1){
		cFinal.x = tab[0].x;
		cFinal.y = tab[0].y;
		cFinal.d = 0;
	}
	else
	{
		/* TESTS AVEC DUO DE POINTS */
	
		for(i=0 ; i<nbPoints-1 ; i++){
			for(j=i+1 ; j<nbPoints ; j++){
				compteur ++;
				p1 = tab[i];
				p2 = tab[j];
				cTemp = cerclePassantParDeuxPoints(p1 , p2);
				
				if ( contientTousPoint(cTemp , tab , nbPoints) && cTemp.d < cFinal.d){
					cFinal = cTemp;
				}
			}
		}
	
		/* TESTS AVEC TRIO DE POINTS */   
	
		for(i=0 ; i<nbPoints ; i++){
	        for(j=0 ; j<nbPoints ; j++){
	            for(k=0 ; k<nbPoints ; k++){
	            	compteur ++;
					if(i!=j && i!=k && j!=k){
	                	p1 = tab[i];
	                	p2 = tab[j];
						p3 = tab[k];
	        	        cTemp = cerclePassantParTroisPoints(p1 , p2 , p3);
	              	  	if ( contientTousPoint(cTemp , tab , nbPoints) && cTemp.d < cFinal.d){
	                	        cFinal = cTemp;
	                	}
					}
	            }
	        }
	    }
	    printf("\non a fait %d iterations \n",compteur);
	}


	return cFinal;
}