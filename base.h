struct article
{
	int code; 
	char des[20];       
	char type;
	int code_fourn;
	int cout;
};
typedef struct article a ;

struct article_list
{
	a data ;
	struct article_list *nexta ;
};
typedef struct article_list la ;



struct fourn
{
	int code_fourn ;
	char raison[20];
};
typedef struct fourn f ;

struct fourn_list
{
	f dataf ;
	struct fourn_list *nextf ;
};
typedef struct fourn_list lf ;

struct comp
{
	int code;
	int qt;
};
typedef struct comp c  ;

struct comp_tree
{
	c datac ;
	struct comp_tree *fils ;
	struct comp_tree *frere ;
	
};
typedef struct comp_tree tc ;
