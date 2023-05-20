#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 8
#define BIT_VERSION

static unsigned char x[N] = {2,7,4,1,5,6,0,3};
static unsigned char y[N] = {4,0,7,1,6,3,5,2};
static unsigned char inv_x[N];

unsigned int xor (void)
{
	static unsigned int y = 2463534242;
	y ^= rand();
	y = y ^ (y << 13);
	y = y ^ (y >> 17);
	return y = y ^ (y << 15);
}

	unsigned long long int xor64(void)
{
	static unsigned long long int x = 88172645463325252ULL;
	x = x ^ (x << 13);
	x = x ^ (x >> 7);
	return x = x ^ (x << 17);
}

void random_permutation(unsigned char *a)
{
	int i, j, x;
	for (i = 0; i < N; i++)
	{
		a[i] = i;
	}
	for (i = 0; i < N - 2; i++)
	{
		// rand from i+1 to N-1
		j = (rand() % (N - 1 - i)) + i + 1;

		// swap a[i] and a[j]
		x = a[j];
		a[j] = a[i];
		a[i] = x;
	}
	if (a[N - 1] == N - 1)
	{
		a[N - 1] = a[N - 2];
		a[N - 2] = N - 1;
	}
}

unsigned long long int toInt(unsigned char *a)
{
	unsigned long long int i = 0, s = 1, ret = 0;
	for (i = 0; i < N; i++)
	{

		ret += s * a[i];
		s *= 2;
		ret << 1;
	}
	//ret >>= 1;

	return ret;
}

unsigned char A[N]={0};

void init_m(){
int i;

for(i=0;i<N/2;i++)
  A[i]=1;

}

unsigned long long int p_rand()
{
	/*
#ifdef BIT_VERSION
	static unsigned char a[N] = {
		1, 1, 1, 1, 0, 0, 0, 0,
		//1,1,1,1,1,1,1,1,
		//0,0,0,0,0,0,0,0,
		//0,0,0,0,0,0,0,0,
		//1,1,1,1,1,1,1,1,
		//1,1,1,1,1,1,1,1,
		//0,0,0,0,0,0,0,0,
		//0,0,0,0,0,0,0,0,
	};
#else
	static unsigned char a[N] = {
		1,
		2,
		3,
		5,
		7,
		11,
		13,
		17,
		19,
		23,
		29,
		31,
		33,
		37,
		39,
		41,
		143,
		151,
		157,
		159,
		161,
		167,
		171,
		175,
		212,
		85,
		1,
		0,
		0,
		0,
		0,
		0,
	};
#endif
*/
	//static unsigned char x[N];
	//static unsigned char y[N];
	//static unsigned char inv_x[N];
	static unsigned char first = 1;

	static unsigned char tmp[N];
	int i;

	if (first)
	{
		/*
		first = 0;
		//random_permutation(x);
		//random_permutation(y);
		for (i = 0; i < N; i++)
		{
			//	printf("%d,",x[i]);
			inv_x[x[i]] = i;
		}
		//printf("\n");
		*/
	}

	//for(i=0;i<N;i++)
	//printf("%d,",tmp[i]);
	//printf("\n");

	// a ^= a * y
	memcpy(tmp, A, sizeof(tmp)); 	// tmp = a;
	for (i = 0; i < N; i++)
	{
		A[i] ^= A[y[i]];
	}

	// y = x * y * ~x
	for (i = 0; i < N; i++)
	{
		tmp[i] = x[y[inv_x[i]]];
	}
	memcpy(y, tmp, sizeof(tmp));

	return toInt(A);
}

int main()
{
	FILE *fp = fopen("out.txt", "wb");
	unsigned int i, j = 0, count = 0, nn = 0;
	unsigned short a, b, c, d, v = 0;
	//unsigned char a[4]={0,1,0,1};
	//unsigned long long int

	time_t t;

	srand(time(&t));

	//printf("%d\n",a);
	
	ku:
	//random_permutation(x);
	//random_permutation(y);

	init_m();
	for(i=0;i<N;i++){
			inv_x[x[i]]=i;
	}
	printf("\n");
	
	j=0;
	count=0;
	nn=0;
	v=0;
	a = p_rand();
	b = p_rand();
	while (1)
	{
		c = p_rand();
		d = p_rand();
		printf(" c=%d d=%d %d\n", c, d, j);
		if (a == c && b == d)
		{		
			nn++;
			printf(" f1=%d %d count=%d\n", a, b,(j+1)/nn);
		}
		if (nn > 10)
			break;

		if (a % 2 == 0)
			count++;
		if (j == 0xffff)
		{
			a = p_rand();
			printf("f2=%d\n", a);
			b = p_rand();
			nn = 0;
			v++;
			j = 0;
		}
		if (v == 3)
		{
			for (i = 0; i < N; i++)
				printf("%d,", x[i]);
			printf("\n");
			exit(1);
		}
		/*
		if(fp){
			fprintf(fp, "%u\n", a);
		}else{
			printf("%u\n", a);
		}
	*/
		j++;
	}
	if((j+1)/nn>3000){
		for(i=0;i<N;i++)
		printf("%d,",x[i]);
		printf(" count x\n");
		for(i=0;i<N;i++)
		printf("%d,",y[i]);
		printf(" count y\n");
	} else if(a >0 || b > 0){
		goto ku;
	}
	printf("county %d %d\n",a,b);
	printf("count f1=%d %d %d\n", count, (j+1)/nn, nn);

	return 0;
}
