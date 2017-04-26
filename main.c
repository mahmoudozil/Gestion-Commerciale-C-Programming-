#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"


int choice();
la *load_article();
void affiche(la *list_a);
lf *load_fourn();
void affichef(lf *list_f);
tc* load_tree();
tc* seekNode(tc* racine,int code);
void insertNode(tc** racine,int codeCompose,int codeComposant,int quantite);
void matnecc(tc* tree_c, la* list_a,lf* list_f);
int chercher_fourn(int x,la *list_a);
void cout_composantes(tc *tree_c,la* list_a,int* total);
void calcul_prix(la* list_a,int *tot,tc* tree_c);
void cout_compose(tc* tree_c,la* list_a, int* total);
int nbr_composes_finis(la* list_a);
char *chercher_des(la* list_a, int y);

void Affiche_Matieres_Necc(tc* tree_c,la* list_a, lf* list_f);
void Affiche_Compostion_Cout(tc* tree_c,la* list_a);
void Affiche_Cout_Composes(tc* tree_c,la* list_a);
void Affiche_Production(tc* tree_c,la* list_a);


//*********************************************************************//

void main(int argc, char const *argv[])
{
	int x;
	la *list_a;
	lf *list_f;
	tc *tree_c;
	list_a = load_article();
	list_f = load_fourn();
	tree_c = load_tree();

	while(x=choice())
	{
		switch(x)
			{
				case 1 :
					Affiche_Matieres_Necc(tree_c,list_a,list_f);
					//entrez un code de composé : cette fonction va afficher ses composantes et leurs fournisseurs
					break;
				case 2 :
					Affiche_Compostion_Cout(tree_c,list_a);
					//entrez un code de composé : cette fonction va afficher ses composantes, leurs couts et le total
					break;
				case 3 :
					Affiche_Cout_Composes(tree_c,list_a);
					//cette fontion affiche tous les composés finis de l'entreprise
					break;
				case 4 :
					Affiche_Production(tree_c,list_a);
					//entrez les codes des composés et la quantité de production : cette fonction affiche le cout total de la production
					break;
			}
	}
}

int choice()
{
	int x;
	printf("************************************************************\n");
	printf("1- les materiaux necessaires pour un composé\n");
	printf("2- le cout des materiaux pour un composé\n");
	printf("3- le cout des composés finis\n");
	printf("4- le programme de la production\n");
	printf("0- quitter\n");
	printf("************************************************************\n");
	printf("donnez votre choix\n");
	scanf("%d",&x);
	if(x<0 || x>4)
		printf("svp entrez une choix valide\n");
	return(x);
}
la *load_article()
{
	char line[100];
	la *first , *new ;
	first = NULL ;
	FILE *f ;
	f = fopen("article.txt","r");
	if(!f)
	{
		printf("error while opening the file\n");
		exit(0);
	}
	while(fgets(line,100,f))
	{
		new = (la *)malloc(sizeof(la));
		sscanf(line,"%3d %20s %c %3d %d",&(new->data.code),(new->data.des),&(new->data.type),&(new->data.code_fourn),&(new->data.cout));
		new->nexta=first ;
		first = new ;
	}
	fclose(f);
	return(first);
}

lf *load_fourn()
{
	char line[100];
	lf *first , *new ;
	first = NULL ;
	FILE *f ;
	f = fopen("fournisseur.txt","r");
	if(!f)
	{
		printf("error while opening the file\n");
		exit(0);
	}
	while(fgets(line,100,f))
	{
		new = (lf *)malloc(sizeof(lf));
		sscanf(line,"%3d %s",&(new->dataf.code_fourn),(new->dataf.raison));
		new->nextf=first ;
		first = new ;
	}
	fclose(f);
	return(first);
}
tc* load_tree()
{
    FILE* f;
    char line[500];
    tc* noeud;
    tc* racine;
    racine=NULL;
    int codeCompose,codeComposant;
    int quantite;
    f=fopen("composition.txt","r");
    if (f) {
        while(fgets(line,500,f)){
            sscanf(line,"%3d %3d %d",&codeCompose,&codeComposant,&quantite);
            insertNode(&racine,codeCompose,codeComposant,quantite);
        }
    }
    else
        printf("\nCannot acces composition.txt\n");
    return(racine);
}
tc* seekNode(tc* racine,int code) {
    tc* t;
    if (racine) {
        if ((racine->datac).code==code)
            return(racine);
        if (t=seekNode(racine->frere,code))
            return(t);
        return(t=seekNode(racine->fils,code));
    }
}

void insertNode(tc** racine,int codeCompose,int codeComposant,int quantite) {
    tc *t,*p,*r;
    t=(tc*)malloc(sizeof(tc));
    (t->datac).code=codeComposant;
    (t->datac).qt=quantite;
    t->fils=NULL;
    p=seekNode(*racine,codeCompose);
    if (p){
        t->frere=p->fils;
        p->fils=t;
    }
    else{
        r=(tc*)malloc(sizeof(tc));
        (r->datac).code=codeCompose;
        (r->datac).qt=1;
        r->frere=*racine;
        r->fils=t;
        *racine=r;
    }
}

void matnecc(tc* tree_c, la* list_a,lf* list_f) {
	int x;

	if (tree_c)
	{
		x = chercher_fourn(tree_c->datac.code,list_a);

  		lf *p ;
 		p = list_f;
  		while(p){
  		if(x==p->dataf.code_fourn)
  			break;
 		 p=p->nextf;
  		}

		la *p1;
		p1 = list_a;
		while(p1)
		{
			if(x==p1->data.code)
				break;
			p1=p1->nexta;
		}

        printf("| %3d   |      %s         |    %s       |\n",tree_c->datac.code,p1->data.des,p->dataf.raison);

        matnecc(tree_c->fils,list_a,list_f);
        matnecc(tree_c->frere,list_a,list_f);
    }
}

int chercher_fourn(int a,la *list_a)
{
  la *p ;
  p = list_a;
  while(p)
  {
  	if(a==p->data.code)
	    return(a);
	p=p->nexta;
  }
}
void cout_composantes(tc *tree_c,la* list_a, int* total)
{
	int a,b;
	if (tree_c)
	{
		la *p ;
  		p = list_a;
  		int tot=0,x;
  		while(p)
  		{
  		if(tree_c->datac.code==p->data.code)
	    	break;
		p=p->nexta;
		}
		if(p->data.type=='f')
		{
			calcul_prix(list_a,&tot,tree_c->fils);

			x = (tree_c->datac.qt)*tot; //tot = prix compose
			printf("| %3d   |   %s        |  %d   |    %d     |   %3d   |\n",tree_c->datac.code,p->data.des,tot,tree_c->datac.qt,x);
			*total += x;
		}
		if(p->data.type=='m')
		{
			a = p->data.cout;
			b = tree_c->datac.qt;
			*total += a*b;

			printf("| %3d   |   %s        |  %d   |    %d     |   %3d   |\n",tree_c->datac.code,p->data.des,p->data.cout,tree_c->datac.qt,a*b);

		}

        cout_composantes(tree_c->frere,list_a,total);
    }
}
void calcul_prix(la* list_a,int *tot,tc* tree_c)
{
	int a,b;
	la* p;
	p = list_a;

	if(tree_c)
	{
		while(p)
	{
		if(tree_c->datac.code==p->data.code)
			break;
		p=p->nexta;
	}
	a = p->data.cout;
	b = tree_c->datac.qt;
	*tot += a*b;



	calcul_prix(list_a,tot,tree_c->frere);
	}


}

void cout_compose(tc* tree_c,la* list_a,int *total)
{
	int a,b;
	if (tree_c)
	{
		la *p ;
  		p = list_a;
  		while(p)
  		{
  		if(tree_c->datac.code==p->data.code)
	    	break;
		p=p->nexta;
		}

		a = p->data.cout;
		b = tree_c->datac.qt;
		*total += (a * b);

        cout_compose(tree_c->fils,list_a,total);
        cout_compose(tree_c->frere,list_a,total);
	}
}
char *chercher_des(la* list_a, int y)
{
	la *p ;

  		p = list_a;
  		while(p)
  		{
  		if(y==p->data.code)
	    	break;
		p=p->nexta;
		}
	return(p->data.des);
}
int nbr_composes_finis(la *list_a)
{
	la* p;
	p=list_a;
	int nb=0;
	while(p)
	{
		if(p->data.type=='f')
			nb++;
		p=p->nexta;
	}

	return(nb);
}
void Affiche_Matieres_Necc(tc* tree_c,la* list_a, lf* list_f)
{
	tc* t;
	int y ;
	printf("donner le code d'un composé:\n");
	scanf("%d",&y);
	t=seekNode(tree_c,y);
	printf("les matériaux necessaires pour ce composé numéro %d :\n\n",y);
	printf("| code  |     désignation         |    fournisseur    |\n");
	printf("+-------+-------------------------+-------------------+\n");
	matnecc(t->fils,list_a,list_f);
	printf("+-------+-------------------------+-------------------+\n");
	printf("\n");
	printf("\n");
}
void Affiche_Compostion_Cout(tc* tree_c,la* list_a)
{
	tc* t;
	int y;
	int total=0;
	printf("donner le code d'un composé:\n");
	scanf("%d",&y);
	t=seekNode(tree_c,y);
	printf("le cout des materiaux pour ce composé%d\n",y);
	printf("| code  |     designation     | cout  | quantité | total   |\n");
	printf("+-------+---------------------+-------+----------+---------+\n");
	cout_composantes(t->fils,list_a,&total);
   	printf("+-------+---------------------+-------+----------+---------+\n");
   	printf("                                          TOTAL      %d   \n",total);

	printf("\n");
	printf("\n");
}
void Affiche_Cout_Composes(tc* tree_c,la* list_a)
{
	int total,tot=0,y,nb;
	tc* t;
	char *d;
	d = (char*)malloc(sizeof(char));
	printf("le cout de tous les composés: \n");
	printf("| code  |     designation     | cout   |\n");
	printf("+-------+---------------------+--------+\n");
	nb = nbr_composes_finis(list_a);
	for(y=1;y<=nb;y++)
	{
		total=0;
		t = seekNode(tree_c,y);
		d = chercher_des(list_a,y);
		cout_compose(t->fils,list_a,&total); //comp cout
		printf("| %3d   |   %s          |  %d   |\n",y,d,total);
		printf("+-------+----------------------+-------+\n");
		tot += total;
	}
		printf("+                       TOTAL    %d  +\n",tot);
	printf("\n");
	printf("\n");

}
void Affiche_Production(tc* tree_c,la* list_a)
{
	tc* t;
	char *d;
	d= (char*)malloc(sizeof(char));
	int total,c,q,i=-1,j,tot;
	int *t1 , *t2;
	t1 = (int*)malloc(sizeof(int));
	t2 = (int*)malloc(sizeof(int));


	while(1)
		{
			printf("\n");
			printf("********************************************************\n");
			printf("* si vous avez fini , entrez 0 dans le code du composé *\n");
			printf("********************************************************\n\n");
			printf("code composé\n");
			scanf("%d",&c);
			if(c==0)
				break;

			printf("quantite\n");
			scanf("%d",&q);
			i++;
			t1[i]= c ;
			t2[i]= q ;

		}
	printf("le cout total de la production des composés: \n\n");
	printf("| code  |     designation     | quantite   | cout total |\n");
	printf("+-------+---------------------+------------+------------+\n");
	total=0;tot=0;
	for(j=0;j<=i;j++)
		{
			t = seekNode(tree_c,t1[j]);
			d = chercher_des(list_a,t1[j]);
			cout_compose(t->fils,list_a,&total);
			printf("| %3d   |   %s          |  %d         |    %d     |\n",t1[j],d,t2[j],t2[j]*total);
			printf("+-------+---------------------+------------+------------+\n");
			tot += t2[j]*total;

		}
	printf("+                                  TOTAL    =      %d     +\n",tot);
}
