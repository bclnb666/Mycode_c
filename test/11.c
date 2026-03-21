#include <stdio.h>

int main()
{
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n-i;j++){
			printf("* ");
		}
		printf("\n");
	}
	printf("\n");
//---------------------------------------
	for(int i=0;i<n;i++){
		for(int j=0;j<=i;j++){
			printf("* ");
		}
		printf("\n");
	}
	printf("\n");
//---------------------------------------
	for(int i=0;i<n;i++){
	
		for(int j=0;j<i;j++){
			printf("  ");
		}
        for(int z=0;z<n-i;z++){
			printf("* ");
		}

		printf("\n");
	}
	printf("\n");
//---------------------------------------
		for(int i=0;i<n;i++){
			for(int j=0;j<n-i-1;j++){
				printf("  ");
			}
        	for(int z=0;z<=i;z++){
				printf("* ");
			}
		printf("\n");
	}
	printf("\n");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
for(int i=0;i<n;i++){
		for(int j=0;j<n-i;j++){
			printf("* ");
		}
		printf("\n");
	}
	printf("\n");
//---------------------------------------
	for(int i=0;i<n;i++){
		for(int z=0;z<n-i;z++){
			printf(" * ");
		}

		for(int j=0;j<i;j++){
			printf("  ");
		}
		printf("\n");
	}
	printf("\n");
//---------------------------------------
	for(int i=0;i<n;i++){
	
		for(int j=0;j<i;j++){
			printf(" ");
		}
        for(int z=0;z<n-i;z++){
			printf("* ");
		}

		printf("\n");
	}
	printf("\n");
//---------------------------------------
		for(int i=0;i<n;i++){
	
		for(int j=0;j<n-i-1;j++){
			printf(" ");
		}
        for(int z=0;z<=i;z++){
			printf("* ");
		}

		printf("\n");
	}

		return 0;
}
