/*
  CEC21 Test Function Suite for Single Objective Bound Constrained Numerical Optimization 
 

*/

#include <WINDOWS.H>      
#include <stdio.h>
#include <math.h>
#include <malloc.h>

#define INF 1.0e99
#define EPS 1.0e-14
#define E  2.7182818284590452353602874713526625
#define PI 3.1415926535897932384626433832795029

void ellips_func(double *, double *, int , double *,double *, int, int); /* Ellipsoidal */
void bent_cigar_func(double *, double *, int , double *,double *, int, int); /* Discus */
void discus_func(double *, double *, int , double *,double *, int, int);  /* Bent_Cigar */
void rosenbrock_func (double *, double *, int , double *,double *, int, int); /* Rosenbrock's */
void ackley_func (double *, double *, int , double *,double *, int, int); /* Ackley's */
void rastrigin_func (double *, double *, int , double *,double *, int, int); /* Rastrigin's  */
void griewank_func (double *, double *, int , double *,double *, int, int); /* Griewank's  */
void schwefel_func (double *, double *, int , double *,double *, int, int); /* Schwefel's */
void bi_rastrigin_func (double *, double *, int , double *,double *, int, int); /* Lunacek Bi_rastrigin */
void grie_rosen_func (double *, double *, int , double *,double *, int, int); /* Griewank-Rosenbrock  */
void escaffer6_func (double *, double *, int , double *,double *, int, int); /* Expanded Scaffer��s F6  */
void happycat_func (double *, double *, int , double *,double *, int, int); /* HappyCat */
void hgbat_func (double *, double *, int , double *,double *, int, int); /* HGBat  */


void hf01 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 1 */
void hf05 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 5 */
void hf06 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 6 */


void cf02 (double *, double *, int , double *,double *, int); /* Composition Function 2 */
void cf04 (double *, double *, int , double *,double *, int); /* Composition Function 4 */
void cf05 (double *, double *, int , double *,double *, int); /* Composition Function 5 */


void cf02_s (double *, double *, int , double *,double *, int); /* Composition Function 2 for shift case*/
void cf04_s (double *, double *, int , double *,double *, int); /* Composition Function 4 for shift case*/
void cf05_s (double *, double *, int , double *,double *, int); /* Composition Function 5 for shift case*/


void shiftfunc (double*,double*,int,double*);
void rotatefunc (double*,double*,int, double*);
void sr_func (double *, double *, int, double*, double*, double, int, int); /* shift and rotate */
void asyfunc (double *, double *x, int, double);
void oszfunc (double *, double *, int);
void cf_cal(double *, double *, int, double *,double *,double *,double *,int);

extern double *OShift,*M,*y,*z,*x_bound;
extern int ini_flag,n_flag,func_flag,*SS;


void cec21_basic_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d_nr.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d_nr.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d_ns.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d_ns.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


	for (i = 0; i < mx; i++)
	{
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 8:	
			cf02(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 9:	
			cf04(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 10:	
			cf05(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			break;
		}
		
	}

}

void cec21_bias_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
    
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d_nr.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d_nr.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d_ns.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d_ns.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


   for (i = 0; i < mx; i++)
	  {
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=100.0;
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1100.0;
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=700.0;
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1900.0;
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]=f[i]+1700.0;
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=1600.0;
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=2100.0;
			break;
		case 8:	
			cf02(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2200.0;
			break;
		case 9:	
			cf04(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2400.0;
			break;
		case 10:	
			cf05(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2500.0;
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			f[i] = 0.0;
			break;
		}
		
	}

}


void cec21_bias_rot_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d_ns.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d_ns.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


   for (i = 0; i < mx; i++)
	  {
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=100.0;
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1100.0;
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=700.0;
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1900.0;
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]=f[i]+1700.0;
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=1600.0;
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=2100.0;
			break;
		case 8:	
			cf02(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2200.0;
			break;
		case 9:	
			cf04(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2400.0;
			break;
		case 10:	
			cf05(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2500.0;
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			f[i] = 0.0;
			break;
		}
		
	}

}

void cec21_bias_shift_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d_nr.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d_nr.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


	for (i = 0; i < mx; i++)
	{
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=100.0;
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1100.0;
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=700.0;
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1900.0;
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]=f[i]+1700.0;
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=1600.0;
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=2100.0;
			break;
		case 8:	
			cf02_s(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2200.0;
			break;
		case 9:	
			cf04_s(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2400.0;
			break;
		case 10:	
			cf05_s(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2500.0;
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			f[i] = 0.0;
			break;
		}
		
	}

}

void cec21_bias_shift_rot_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


	for (i = 0; i < mx; i++)
	{
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=100.0;
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1100.0;
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=700.0;
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			f[i]+=1900.0;
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]=f[i]+1700.0;
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=1600.0;
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			f[i]+=2100.0;
			break;
		case 8:	
			cf02_s(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2200.0;
			break;
		case 9:	
			cf04_s(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2400.0;
			break;
		case 10:	
			cf05_s(&x[i*nx],&f[i],nx,OShift,M,1);
			f[i]+=2500.0;
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			f[i] = 0.0;
			break;
		}
		
	}

}

void cec21_rot_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d_ns.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d_ns.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


	for (i = 0; i < mx; i++)
	{
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 8:	
			cf02(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 9:	
			cf04(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 10:	
			cf05(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			break;
		}
		
	}

}

void cec21_shift_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d_nr.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d_nr.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


	for (i = 0; i < mx; i++)
	{
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 8:	
			cf02_s(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 9:	
			cf04_s(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 10:	
			cf05_s(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			break;
		}
		
	}

}

void cec21_shift_rot_func(double *x, double *f, int nx, int mx,int func_num)
{
	
    int cf_num=10,i,j;
    if (func_num<1||func_num>10)
		{
			printf("\nError: Test function %d is not defined.\n", func_num);
		}
	if (ini_flag==1)
	{
		if ((n_flag!=nx)||(func_flag!=func_num))
		{
			ini_flag=0;
		}
	}

	if (ini_flag==0)
	{
		FILE *fpt;
		char FileName[256];
		free(M);
		free(OShift);
		free(y);
		free(z);
		free(x_bound);
		y=(double *)malloc(sizeof(double)  *  nx);
		z=(double *)malloc(sizeof(double)  *  nx);
		x_bound=(double *)malloc(sizeof(double)  *  nx);
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==10||nx==20))
		{
			printf("\nError: Test functions are only defined for D=2,10,20.\n");
		}
		if (nx==2&&(func_num==5||func_num==6||func_num==7))
		{
			printf("\nError:  NOT defined for D=2.\n");
		}

		/* Load Matrix M*/
		sprintf(FileName, "input_data/M_%d_D%d.txt", func_num,nx);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
		    printf("\n Error: Cannot open M_%d_D%d.txt for reading \n",func_num,nx);
		}
		if (func_num<7)
		{
			M=(double*)malloc(nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		else
		{
			M=(double*)malloc(cf_num*nx*nx*sizeof(double));
			if (M==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for (i=0; i<cf_num*nx*nx; i++)
			{
				fscanf(fpt,"%lf",&M[i]);
			}
		}
		fclose(fpt);
		
		/* Load shift_data */
		sprintf(FileName, "input_data/shift_data_%d.txt", func_num);
		fpt = fopen(FileName,"r");
		if (fpt==NULL)
		{
			printf("\n Error: Cannot open shift_data_%d.txt for reading \n",func_num);
		}

		if (func_num<7)
		{
			OShift=(double *)malloc(nx*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%lf",&OShift[i]);
			}
		}
		else
		{
            //OShift=(double *)malloc(nx*sizeof(double));
			OShift=(double *)malloc(nx*cf_num*sizeof(double));
			if (OShift==NULL)
			printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<cf_num-1;i++)
			{
				for (j=0;j<nx;j++)
				{
					fscanf(fpt,"%lf",&OShift[i*nx+j]);
					
				}
				fscanf(fpt,"%*[^\n]%*c"); 
			}
			for (j=0;j<nx;j++)
			{
 				fscanf(fpt,"%lf",&OShift[nx*(cf_num-1)+j]);
				
			}
				
		}
		fclose(fpt);


		/* Load Shuffle_data */
		
		if (func_num>=5&&func_num<=7)
		{
			sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
			fpt = fopen(FileName,"r");
			if (fpt==NULL)
			{
				printf("\n Error: Cannot open shuffle_data_%d_D%d.txt for reading \n", func_num, nx);
			}
			SS=(int *)malloc(nx*sizeof(int));
			if (SS==NULL)
				printf("\nError: there is insufficient memory available!\n");
			for(i=0;i<nx;i++)
			{
				fscanf(fpt,"%d",&SS[i]);
			}
			fclose(fpt);	
		}
		
		n_flag=nx;
		func_flag=func_num;
		ini_flag=1;
		//printf("Function has been initialized!\n");
	}


	for (i = 0; i < mx; i++)
	{
		switch(func_num)
		{
		case 1:	
			bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 2:	
			schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 3:	
			bi_rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 4:	
            grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
			break;
		case 5:	
			hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 6:	
			hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 7:
			hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
			break;
		case 8:	
			cf02_s(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 9:	
			cf04_s(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		case 10:	
			cf05_s(&x[i*nx],&f[i],nx,OShift,M,1);
			break;
		default:
			printf("\nError: There are only 10 test functions in this test suite!\n");
			break;
		}
		
	}

}

void ellips_func (double *x, double *f, int nx, double *Os,double *Mr, int s_flag, int r_flag) /* Ellipsoidal */
{
    int i;
	f[0] = 0.0;
	sr_func (x, z, nx, Os, Mr,1.0, s_flag, r_flag); /* shift and rotate */
	for (i=0; i<nx; i++)
	{
       f[0] += pow(10.0,6.0*i/(nx-1))*z[i]*z[i];
	}
}



void bent_cigar_func (double *x, double *f, int nx, double *Os,double *Mr, int s_flag, int r_flag) /* Bent_Cigar */
{
    int i;
	sr_func (x, z, nx, Os, Mr,1.0, s_flag, r_flag); /* shift and rotate */
	f[0] = z[0]*z[0];
	for (i=1; i<nx; i++)
	{
		f[0] += pow(10.0,6.0)*z[i]*z[i];
	}

}

void discus_func (double *x, double *f, int nx, double *Os,double *Mr, int s_flag, int r_flag) /* Discus */
{
    int i;
	sr_func (x, z, nx, Os, Mr,1.0, s_flag, r_flag); /* shift and rotate */
	f[0] = pow(10.0,6.0)*z[0]*z[0];
	for (i=1; i<nx; i++)
	{
		f[0] += z[i]*z[i];
	}
}



void rosenbrock_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Rosenbrock's */
{
    int i;
	double tmp1,tmp2;
	f[0] = 0.0;
	sr_func (x, z, nx, Os, Mr, 2.048/100.0, s_flag, r_flag); /* shift and rotate */
	z[0] += 1.0;//shift to orgin
	for (i=0; i<nx-1; i++)
	{
		z[i+1] += 1.0;//shift to orgin
		tmp1=z[i]*z[i]-z[i+1];
		tmp2=z[i]-1.0;
		f[0] += 100.0*tmp1*tmp1 +tmp2*tmp2;
	}
}



void ackley_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Ackley's  */
{
    int i;
    double sum1, sum2;
    sum1 = 0.0;
    sum2 = 0.0;

	sr_func (x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		sum1 += z[i]*z[i];
		sum2 += cos(2.0*PI*z[i]);
	}
	sum1 = -0.2*sqrt(sum1/nx);
	sum2 /= nx;
		f[0] =  E - 20.0*exp(sum1) - exp(sum2) +20.0;
}




void griewank_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Griewank's  */
{
    int i;
    double s, p;
    s = 0.0;
    p = 1.0;

	sr_func (x, z, nx, Os, Mr, 600.0/100.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		s += z[i]*z[i];
		p *= cos(z[i]/sqrt(1.0+i));
	}
	f[0] = 1.0 + s/4000.0 - p;
}

void rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Rastrigin's  */
{
    int i;
	f[0] = 0.0;

	sr_func (x, z, nx, Os, Mr, 5.12/100.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		f[0] += (z[i]*z[i] - 10.0*cos(2.0*PI*z[i]) + 10.0);
	}
}



void schwefel_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Schwefel's  */
{
    int i;
	double tmp;
	f[0]=0.0;

	sr_func (x, z, nx, Os, Mr, 1000.0/100.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		z[i] += 4.209687462275036e+002;
		if (z[i]>500)
		{
			f[0]-=(500.0-fmod(z[i],500))*sin(pow(500.0-fmod(z[i],500),0.5));
			tmp=(z[i]-500.0)/100;
			f[0]+= tmp*tmp/nx;
		}
		else if (z[i]<-500)
		{
			f[0]-=(-500.0+fmod(fabs(z[i]),500))*sin(pow(500.0-fmod(fabs(z[i]),500),0.5));
			tmp=(z[i]+500.0)/100;
			f[0]+= tmp*tmp/nx;
		}
		else
			f[0]-=z[i]*sin(pow(fabs(z[i]),0.5));
		}
		f[0] +=4.189828872724338e+002*nx;

}


void bi_rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Lunacek Bi_rastrigin Function */
{
    int i;
	double mu0=2.5,d=1.0,s,mu1,tmp,tmp1,tmp2;
	double *tmpx;
	tmpx=(double *)malloc(sizeof(double)  *  nx);
	s=1.0-1.0/(2.0*pow(nx+20.0,0.5)-8.2);
	mu1=-pow((mu0*mu0-d)/s,0.5);

	if (s_flag==1)
		shiftfunc(x, y, nx, Os);
	else
	{
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i] = x[i];
		}
	}
	for (i=0; i<nx; i++)//shrink to the orginal search range
    {
        y[i] *= 10.0/100.0;
    }

	for (i = 0; i < nx; i++)
    {
		tmpx[i]=2*y[i];
        if (Os[i] < 0.0)
            tmpx[i] *= -1.;
    }
	for (i=0; i<nx; i++)
	{
		z[i]=tmpx[i];
		tmpx[i] += mu0;
	}
    tmp1=0.0;tmp2=0.0;
	for (i=0; i<nx; i++)
	{
		tmp = tmpx[i]-mu0;
		tmp1 += tmp*tmp;
		tmp = tmpx[i]-mu1;
		tmp2 += tmp*tmp;
	}
	tmp2 *= s;
	tmp2 += d*nx;
	tmp=0.0;

	if (r_flag==1)
	{
		rotatefunc(z, y, nx, Mr);
		for (i=0; i<nx; i++)
		{
			tmp+=cos(2.0*PI*y[i]);
		}	
		if(tmp1<tmp2)
			f[0] = tmp1;
		else
			f[0] = tmp2;
		f[0] += 10.0*(nx-tmp);
	}
	else
	{
		for (i=0; i<nx; i++)
		{
			tmp+=cos(2.0*PI*z[i]);
		}	
		if(tmp1<tmp2)
			f[0] = tmp1;
		else
			f[0] = tmp2;
		f[0] += 10.0*(nx-tmp);
	}

	free(tmpx);
}

void grie_rosen_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Griewank-Rosenbrock  */
{
    int i;
    double temp,tmp1,tmp2;
    f[0]=0.0;

	sr_func (x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

	z[0] += 1.0;//shift to orgin
    for (i=0; i<nx-1; i++)
    {
		z[i+1] += 1.0;//shift to orgin
		tmp1 = z[i]*z[i]-z[i+1];
		tmp2 = z[i]-1.0;
        temp = 100.0*tmp1*tmp1 + tmp2*tmp2;
         f[0] += (temp*temp)/4000.0 - cos(temp) + 1.0;
    }
	tmp1 = z[nx-1]*z[nx-1]-z[0];
	tmp2 = z[nx-1]-1.0;
    temp = 100.0*tmp1*tmp1 + tmp2*tmp2;;
    f[0] += (temp*temp)/4000.0 - cos(temp) + 1.0 ;
}


void escaffer6_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* Expanded Scaffer��s F6  */
{
    int i;
    double temp1, temp2;

	sr_func (x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

    f[0] = 0.0;
    for (i=0; i<nx-1; i++)
    {
        temp1 = sin(sqrt(z[i]*z[i]+z[i+1]*z[i+1]));
		temp1 =temp1*temp1;
        temp2 = 1.0 + 0.001*(z[i]*z[i]+z[i+1]*z[i+1]);
        f[0] += 0.5 + (temp1-0.5)/(temp2*temp2);
    }
    temp1 = sin(sqrt(z[nx-1]*z[nx-1]+z[0]*z[0]));
	temp1 =temp1*temp1;
    temp2 = 1.0 + 0.001*(z[nx-1]*z[nx-1]+z[0]*z[0]);
    f[0] += 0.5 + (temp1-0.5)/(temp2*temp2);
}

void happycat_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* HappyCat, provdided by Hans-Georg Beyer (HGB) */
/* original global optimum: [-1,-1,...,-1] */
{
	int i;
	double alpha,r2,sum_z;
	alpha=1.0/8.0;
	
	sr_func (x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

	r2 = 0.0;
	sum_z=0.0;
    for (i=0; i<nx; i++)
    {
		z[i]=z[i]-1.0;//shift to orgin
        r2 += z[i]*z[i];
		sum_z += z[i];
    }
    f[0]=pow(fabs(r2-nx),2*alpha) + (0.5*r2 + sum_z)/nx + 0.5;
}

void hgbat_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) /* HGBat, provdided by Hans-Georg Beyer (HGB)*/
/* original global optimum: [-1,-1,...,-1] */
{
	int i;
	double alpha,r2,sum_z;
	alpha=1.0/4.0;

	sr_func (x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

	r2 = 0.0;
	sum_z=0.0;
    for (i=0; i<nx; i++)
    {
		z[i]=z[i]-1.0;//shift to orgin
        r2 += z[i]*z[i];
		sum_z += z[i];
    }
    f[0]=pow(fabs(pow(r2,2.0)-pow(sum_z,2.0)),2*alpha) + (0.5*r2 + sum_z)/nx + 0.5;
}

void hf01 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) /* Hybrid Function 1  /F17 Hybrid Function 1 in cec2014*/
{
	int i,tmp,cf_num=3;
	double fit[3];
	int G[3],G_nx[3];
	double Gp[3]={0.3,0.3,0.4};

	tmp=0;
	for (i=1; i<cf_num; i++)
	{
		G_nx[i] = ceil(Gp[i]*nx);
		tmp += G_nx[i];
	}
	//G_nx[cf_num-1]=nx-tmp;
    G_nx[0]=nx-tmp;
	G[0]=0;
	for (i=1; i<cf_num; i++)
	{
		G[i] = G[i-1]+G_nx[i-1];
	}

	sr_func (x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		y[i]=z[S[i]-1];
	}
	i=0;
	schwefel_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

	i=1;
	rastrigin_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

	i=2;
	ellips_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

	f[0]=0.0;
	for(i=0;i<cf_num;i++)
	{
		f[0] += fit[i];
	}
     
}



void hf05 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) // hf5 in cec2014 F21 in cec2014
{
	int i,tmp,cf_num=5;
	double fit[5];
	int G[5],G_nx[5];
	double Gp[5]={0.1,0.2,0.2,0.2,0.3};
   
    if (nx==5) // deal with D=6**04/01/2020
    { 
        G_nx[0]=1;
        G_nx[1]=1;
        G_nx[2]=1;
        G_nx[3]=1;
        G_nx[4]=1;
      
    }// deal with D=5**04/01/2020   
    else
    {
        tmp=0;
        for (i=1; i<cf_num; i++)
        {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }
        G_nx[0]=nx-tmp;
    }

	G[0]=0;
	for (i=1; i<cf_num; i++)
	{
		G[i] = G[i-1]+G_nx[i-1];
	}

	sr_func (x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		y[i]=z[S[i]-1];
	}
	i=0;
	escaffer6_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
    // printf("\n when i=0 G_nx[i]= %d  \n", G_nx[i]);
	i=1;
	hgbat_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	i=2;
	rosenbrock_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	i=3;
	schwefel_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	i=4;
	ellips_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	f[0]=0.0;
	for(i=0;i<cf_num;i++)
	{
		f[0] += fit[i];
	}
}
void hf06 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) /* Hybrid Function 5 */
{
	int i,tmp,cf_num=4;
	double fit[4];
	int G[4],G_nx[4];
	double Gp[4]={0.2,0.2,0.3,0.3};

	 if (nx==5) // deal with D=5**04/01/2020
    { 
        
            G_nx[0] = 1;
            G_nx[1] = 1;
            G_nx[2] = 1;
            G_nx[3] = 2;
        
    }// deal with D=6**04/01/2020   
    else
    {
        tmp=0;
        for (i=1; i<cf_num; i++)
        {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }
        G_nx[0]=nx-tmp;
    }

	G[0]=0;
	for (i=1; i<cf_num; i++)
	{
		G[i] = G[i-1]+G_nx[i-1];
	}

	sr_func (x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

	for (i=0; i<nx; i++)
	{
		y[i]=z[S[i]-1];
	}
	i=0;
	escaffer6_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	i=1;
	hgbat_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	i=2;
	rosenbrock_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	i=3;
	schwefel_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
	
	f[0]=0.0;
	for(i=0;i<cf_num;i++)
	{
		f[0] += fit[i];
	}
}



void cf02 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 2 */
{
	int i,cf_num=3;
	double fit[3];
	double delta[3] = {10,20,30};
	double bias[3] = {0, 0, 0};

	i=0;
	rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	i=1;
	griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=2;
	schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
}


void cf04 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 3 */
{
	int i,cf_num=4;
	double fit[4];
	double delta[4] = {10,20,30,40};
	double bias[4] = {0, 0, 0, 0};
	
	i=0;
	ackley_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=1;
	ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=10000*fit[i]/1e+10;
	i=2;
	griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=3;
	rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
}

void cf05 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 4 */
{
	int i,cf_num=5;
	double fit[5];
	double delta[5] = {10,20,30,40,50};
	double bias[5] = {0, 0, 0, 0, 0};
	i=0;
	rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=10000*fit[i]/1e+3;
	i=1;
	happycat_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/1e+3;
	i=2;
	ackley_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=3;
	discus_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=10000*fit[i]/1e+10;	
	i=4;
	rosenbrock_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
}		


void cf02_s (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 2 */
{
	int i,cf_num=3;
	double fit[3];
	double delta[3] = {10,20,30};
	double bias[3] = {0, 100, 200};

	i=0;
	rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	i=1;
	griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=2;
	schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
}


void cf04_s (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 3 */
{
	int i,cf_num=4;
	double fit[4];
	double delta[4] = {10,20,30,40};
	double bias[4] = {0, 100, 200, 300};
    
	
	i=0;
	ackley_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=1;
	ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=10000*fit[i]/1e+10;
	i=2;
	griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=3;
	rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
}

void cf05_s (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 4 */
{
	int i,cf_num=5;
	double fit[5];
	double delta[5] = {10,20,30,40,50};
	double bias[5] = {0, 100, 200, 300, 400};
	i=0;
	rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=10000*fit[i]/1e+3;
	i=1;
	happycat_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/1e+3;
	i=2;
	ackley_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=1000*fit[i]/100;
	i=3;
	discus_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	fit[i]=10000*fit[i]/1e+10;	
	i=4;
	rosenbrock_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
	cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
}		



void shiftfunc (double *x, double *xshift, int nx,double *Os)
{
	int i;
    for (i=0; i<nx; i++)
    {
        xshift[i]=x[i]-Os[i];
    }
}

void rotatefunc (double *x, double *xrot, int nx,double *Mr)
{
	int i,j;
    for (i=0; i<nx; i++)
    {
        xrot[i]=0;
			for (j=0; j<nx; j++)
			{
				xrot[i]=xrot[i]+x[j]*Mr[i*nx+j];
			}
    }
}

void sr_func (double *x, double *sr_x, int nx, double *Os,double *Mr, double sh_rate, int s_flag,int r_flag) /* shift and rotate */
{
	int i;
	if (s_flag==1)
	{
		if (r_flag==1)
		{	
			shiftfunc(x, y, nx, Os);
			for (i=0; i<nx; i++)//shrink to the orginal search range
			{
				y[i]=y[i]*sh_rate;
			}
			rotatefunc(y, sr_x, nx, Mr);
		}
		else
		{
			shiftfunc(x, sr_x, nx, Os);
			for (i=0; i<nx; i++)//shrink to the orginal search range
			{
				sr_x[i]=sr_x[i]*sh_rate;
			}
		}
	}
	else
	{	

		if (r_flag==1)
		{	
			for (i=0; i<nx; i++)//shrink to the orginal search range
			{
				y[i]=x[i]*sh_rate;
			}
			rotatefunc(y, sr_x, nx, Mr);
		}
		else
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			sr_x[i]=x[i]*sh_rate;
		}
	}
}

void asyfunc (double *x, double *xasy, int nx, double beta)
{
	int i;
    for (i=0; i<nx; i++)
    {
		if (x[i]>0)
        xasy[i]=pow(x[i],1.0+beta*i/(nx-1)*pow(x[i],0.5));
    }
}

void oszfunc (double *x, double *xosz, int nx)
{
	int i,sx;
	double c1,c2,xx;
    for (i=0; i<nx; i++)
    {
		if (i==0||i==nx-1)
        {
			if (x[i]!=0)
				xx=log(fabs(x[i]));
			if (x[i]>0)
			{	
				c1=10;
				c2=7.9;
			}
			else
			{
				c1=5.5;
				c2=3.1;
			}	
			if (x[i]>0)
				sx=1;
			else if (x[i]==0)
				sx=0;
			else
				sx=-1;
			xosz[i]=sx*exp(xx+0.049*(sin(c1*xx)+sin(c2*xx)));
		}
		else
			xosz[i]=x[i];
    }
}


void cf_cal(double *x, double *f, int nx, double *Os,double * delta,double * bias,double * fit, int cf_num)
{
	int i,j;
	double *w;
	double w_max=0,w_sum=0;
	w=(double *)malloc(cf_num * sizeof(double));
	for (i=0; i<cf_num; i++)
	{
		fit[i]+=bias[i];
		w[i]=0;
		for (j=0; j<nx; j++)
		{
			w[i]+=pow(x[j]-Os[i*nx+j],2.0);
		}
		if (w[i]!=0)
			w[i]=pow(1.0/w[i],0.5)*exp(-w[i]/2.0/nx/pow(delta[i],2.0));
		else
			w[i]=INF;
		if (w[i]>w_max)
			w_max=w[i];
	}

	for (i=0; i<cf_num; i++)
	{
		w_sum=w_sum+w[i];
	}
	if(w_max==0)
	{
		for (i=0; i<cf_num; i++)
			w[i]=1;
		w_sum=cf_num;
	}
	f[0] = 0.0;
    for (i=0; i<cf_num; i++)
    {
		f[0]=f[0]+w[i]/w_sum*fit[i];
    }
	free(w);
}

