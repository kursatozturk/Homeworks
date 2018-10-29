#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

char* dijkstra(char*);
char* get_formula(char*,int*);
double evaluate(char *,double*,int,char*);
double* get_histogram_of_variable(int);
void generate_value_set(double*,double**,int,int,double*,double*);

int main(){

	char *array_of_formula,*infix_formula,*p,*dijkstra_postfix;
	char x;
	int count_of_intervals;
	long int count_of_experiments;
	int  i,j;
	int *pnumber_of_variables,number_of_variables;
	double *values_of_variables;
	char *index_of_variables;
	double result;
	double **intervals_probabilities;
	double result_upper=LONG_MIN,result_lower=LONG_MAX,avg;
	int *result_probability;
	int count_of_results=0;
	double *btw,*lower_array;

	srand(time(NULL));
	infix_formula=malloc(200);
	pnumber_of_variables=&number_of_variables;
	p=infix_formula;
	while(1){
		x=getchar();
		if(x=='\n'){*p=0; break;}
		*p++=x;
	}

	array_of_formula=get_formula(infix_formula,pnumber_of_variables);

	values_of_variables = malloc(number_of_variables*sizeof(double));
	index_of_variables = malloc(number_of_variables*sizeof(double));
	
	dijkstra_postfix=dijkstra(array_of_formula);
	
	btw = malloc(number_of_variables*sizeof(double));
	lower_array=malloc(number_of_variables*sizeof(double));
	/*
		postfix in dijkstra_postfix
	*/
	free(infix_formula);
	free(array_of_formula);

		scanf("%d %ld ",&count_of_intervals,&count_of_experiments);
		intervals_probabilities=malloc(number_of_variables*sizeof(double*));
		for(i=0;i<number_of_variables;i++){
			double *read;
			char* q;
			double lower,upper,length;
			*(intervals_probabilities+i)=get_histogram_of_variable(count_of_intervals);
			q=(char *)*(intervals_probabilities+i);
			*(index_of_variables+i) = *q++;
			read=(double*)q;
			upper=*(read+1); lower=*(read); length=(upper-lower)/count_of_intervals; *(btw+i)=length; *(lower_array+i)=lower;
		}

 /*
 //		First to find the min/max values
 */
	for(i=0;i<count_of_experiments;i++){
		generate_value_set(lower_array,intervals_probabilities,count_of_intervals,number_of_variables,btw,values_of_variables);
		result=evaluate(dijkstra_postfix,values_of_variables,number_of_variables,index_of_variables);
		if (result<result_lower)result_lower=result;
		if (result>result_upper)result_upper=result;
	}

	result_probability=calloc(count_of_intervals,sizeof(double));

	avg=(result_upper - result_lower)/count_of_intervals;

	for(i=0;i<count_of_experiments;i++){	
		generate_value_set(lower_array,intervals_probabilities,count_of_intervals,number_of_variables,btw,values_of_variables);
		result=evaluate(dijkstra_postfix,values_of_variables,number_of_variables,index_of_variables);
		for(j=0;j<count_of_intervals;j++){
			if(result<=result_lower+(j+1)*avg){(*(result_probability+j))++;count_of_results++;break;}
		}

	}

		printf("%f %f ",result_lower ,result_upper );
	for(i=0;i<count_of_intervals;i++){
		printf("%f ",(float)*(result_probability+i)/count_of_results );
	}

 	return 0;
}
/* 
//		parametres:  infix ; return: array which is in the same order, just floats are in type float, others are char ; side effects: number_of_variables gets the number of variables. 
//		a== ~ ; b== sin ; c== cos ; d== sqrt ; e==ln
//		k== - ; l== + ; m== / ; n== * ; o==^ 
*/

char* get_formula(char* infix_formula_array,int* number_of_variables){
	char *formula_array,*p,*get_array;
	short int integer_ctrl,double_ctrl;
	int variable_detecter;
	double value;
	char variable_control[28];

	variable_detecter=0;
	double_ctrl=0;
	integer_ctrl=0;
	p=infix_formula_array;

	formula_array=malloc(200*sizeof(double));

	get_array=formula_array;

	while(*p){

		if(*p==' '){
			if(double_ctrl>0 || integer_ctrl>0){
				double *write;
				write=(double*) get_array;
				*write=value;

			get_array+=sizeof(double);


			double_ctrl=0;integer_ctrl=0;}
			p++;
			goto cont_stat;
		}
		if(*p>='0' && *p<='9'){
			if(double_ctrl>0){
				value=value +(*p-'0')/pow(10,double_ctrl);				
				double_ctrl++;
				p++;
				goto cont_stat;
			}
			if(integer_ctrl==1 && double_ctrl==0){
				value=10*value+*p-'0';
				p++;
				goto cont_stat;
			}
			*get_array++='i';
			integer_ctrl=1;
			value=(double) *p-'0';
			p++;
			goto cont_stat;
		}

		else if(*p!='.' && (integer_ctrl>0 || double_ctrl>0)){
			double *write;
			integer_ctrl=0; double_ctrl=0;
			write=(double*)get_array;
			*write=value;
			get_array+=sizeof(double);
		}

		if(integer_ctrl==1 && *p =='.'){
			double_ctrl=1;
			p++;
			goto cont_stat;
		}
		else;

		if(integer_ctrl!=1 && *p=='.'){
			*get_array++='i';
			double_ctrl=1;
			p++;
			goto cont_stat;
		}
		else ;

		if(*p=='~'){*get_array++='a'; p++; goto cont_stat;}
		if(*p=='s' && *(p+1) =='i' && *(p+2) == 'n'){*get_array++='b'; p+=3; goto cont_stat;}
		if(*p=='c' && *(p+1)=='o'&& *(p+2)=='s'){*get_array++='c'; p+=3; goto cont_stat;}
		if(*p=='s'&& *(p+1)=='q' && *(p+2)=='r' && *(p+3)=='t'){*get_array++='d'; p+=4; goto cont_stat;}
		if(*p=='l' && *(p+1)=='n'){*get_array++='e'; p+=2; goto cont_stat;}
		if(*p<='Z' && *p>='A'){
			int nn,check=0;
			for(nn=0;nn<variable_detecter;nn++)if(variable_control[nn]==*p)check=1; 
			if(!check){
			variable_control[variable_detecter]=*p;
			variable_detecter++;}}

		switch(*p){
			case '-': *get_array++='k' ; break; 
			case '+': *get_array++='l' ; break; 
			case '/': *get_array++='m' ; break; 
			case '*': *get_array++='n' ; break; 
			case '^': *get_array++='o' ; break;
			default : *get_array++=*p  ; break;
		}	
		p++;
		cont_stat: continue;}
	*get_array=0;
	*number_of_variables=variable_detecter;
	return formula_array;
}


/*
//gets the array where the floats are in type float,return: postfix version of formula
*/

char* dijkstra(char*array_of_formula){
	char *postfix,*queue;
	char *parray,*ppostfix,*pqueue;
	double *write,*read;
	parray=array_of_formula;
	postfix=malloc(200*sizeof(double));
	queue=malloc(200);
	ppostfix=postfix;
	pqueue=queue;
	while(*parray){
		char x;
		x=*parray++;
xxxx:	
		switch(x){
			case 'i':
						*ppostfix++='i';
						read=(double*) parray;
						write=(double*) ppostfix;
						*write=*read;

						parray+=sizeof(double);
						ppostfix+=sizeof(double);
						break;
			case 'k':	if(*(pqueue-1)<'k' && *(pqueue-1)!='l') *(pqueue++)='k';
						else {
							*ppostfix=*(pqueue-1);
							ppostfix++;
							*(pqueue-1)=0;
							pqueue--;
							goto xxxx;
						} break;

			case 'l':   if(*(pqueue-1)<'l' && *(pqueue-1)!='k') *(pqueue++)='l';
						else {
							*ppostfix=*(pqueue-1);
							ppostfix++;
							*(pqueue-1)=0;
							pqueue--;
							goto xxxx;
						}break;

			case 'm':   if(*(pqueue-1)<'m' && *(pqueue-1)!='n') *(pqueue++)='m';
						else {
							*ppostfix=*(pqueue-1);
							ppostfix++;
							*(pqueue-1)=0;
							pqueue--;
							goto xxxx;
						}break;

			case 'n':   
						if(*(pqueue-1)<'n' && *(pqueue-1)!='m') *(pqueue++)='n';
						else {
							*ppostfix=*(pqueue-1);
							ppostfix++;
							*(pqueue-1)=0;
							pqueue--;
							goto xxxx;
						}break;

			case 'o':  if(*(pqueue-1)<='o') *(pqueue++)='o';
						else {
							*ppostfix=*(pqueue-1);
							ppostfix++;
							*(pqueue-1)=0;
							pqueue--;
							goto xxxx;
						}break;

			case '(': *(pqueue++)='(';
					  break;

			case ')':	while(*(pqueue-1)!='('){
							*pqueue--=0;
							*ppostfix++=*pqueue;
						}
						*--pqueue=0;

						if((*(pqueue-1)<='e' && *(pqueue-1)>='a')){
							*ppostfix++=*--pqueue;
						}
							*pqueue=0;
							break;
						
			case 'a':
						*pqueue++=x;
						break;
			case 'b':
						*pqueue++=x;
						break;
			case 'c':
						*pqueue++=x;
						break;
			case 'd':
						*pqueue++=x;
						break;
			case 'e':
						*pqueue++=x;
						break;
			default: 	*ppostfix++=x;
						break;

		}
}

			while(pqueue>queue){
				pqueue--;
				*ppostfix++=*pqueue;
			}
	free(queue);
	return postfix;}
/*
//	gets postfix, values of variables ,number of variables  and  index of variables to evaluate formula;return the result of the formula for the given value_set for variables;
*/
	

double evaluate(char *postfix,double*values_of_variables,int number_of_variables,char* index_of_variables){
	double *stack,*read,*sstack;
	double x,y,result;
	char *p,holder;
	int double_ctrl;
	double_ctrl=0;
	sstack=malloc(100*sizeof(double));
	stack=sstack;
	p=postfix;
	while(*p){
		holder=*p;
		if(holder<='Z' && holder>='A' ){
			double *values;
			int i;
			p++;
			values=(double*)stack;
			for(i=0;i<4;i++){
				if(holder==*(index_of_variables+i)){
					*values= *(values_of_variables+i);
					break;
				}
			}
			stack++;
				continue;
		}
			switch(*p){
			case 'l':
					x=*(stack-1);
					*--stack=0;
					y=*(stack-1);
					*(stack-1)=y+x;
					break;
			case 'k':
					x=*(stack-1);
					*--stack=0;
					y=*(stack-1);
					*(stack-1)=y-x;
					break;
			case 'n':
					x=*(stack-1);
					*--stack=0;
					y=*(stack-1);
					*(stack-1)=y*x;
					break;
			case 'm':
					x=*(stack-1);
					*--stack=0;
					y=*(stack-1);
					*(stack-1)=y/x;
					break;
			case 'o':
					x=*(stack-1);
					*--stack=0;
					y=*(stack-1);
					*(stack-1)=pow(y,x);
					break;
			case 'a':
					*(stack-1)= -(*(stack-1));
					break;
			case 'b':
					*(stack-1)= sin(*(stack-1));
					break;
			case 'c':
					*(stack-1)= cos(*(stack-1));
					break;
			case 'd':						
					*(stack-1)= sqrt(*(stack-1));
					break;
			case 'e':
					*(stack-1)= log(*(stack-1));
					break;
			case 'i':
					double_ctrl=1;
					p++;
					read=(double*)p;
					*stack++=*read;
					p+=sizeof(double);
					break;

			}
		if(double_ctrl==0)	p++;
		else double_ctrl=0;
	}
	result= *(stack-1);
	free(sstack);
	return result;
		}

double* get_histogram_of_variable(int count_of_intervals){
	double *histogram;
	double *p;
	char *ch;
	int i;
	histogram = malloc((count_of_intervals+2)*sizeof(double)+sizeof(char));
	ch =(char*) histogram;
	scanf(" %c", ch);
	ch++;
	p=(double*)ch;
	for(i=0;i<count_of_intervals+2;i++){
		scanf("%lf",p);
		p++;
	}
	return histogram;
		}

void generate_value_set(double* lowers,double**intervals_probabilities,int count_of_intervals,int number_of_variables,double *a,double *value_set){
	double *freq, *prefix,*result,random1;
	int k,i,j;
	result=malloc(count_of_intervals*sizeof(double));
	prefix=malloc(count_of_intervals*sizeof(double));

	for(k=0;k<number_of_variables;k++){

		freq=(double*) *(intervals_probabilities+k);

		freq=(double*)((char*)freq+1);
		freq+=2;

		for(i=0;i<count_of_intervals;i++){
			*(prefix+i)=0;
			for(j=0;j<=i;j++)
				*(prefix+i)+=*(freq+j);			
		}


		random1 = ((float)rand() /(float)RAND_MAX );
			for(j=0;j<count_of_intervals;j++){
				if(random1<=*(prefix+j)){
					*(result+k)=j;
					goto baba;}
				
			}
baba: 	continue;
	}

	for(k=0;k<number_of_variables;k++){
		double randvar;
		randvar=(float)rand()/(float)RAND_MAX;
		*(value_set+k)= ((randvar+result[k])* *(a+k) + *(lowers+k));
	}
	free(prefix);
	free(result);}
