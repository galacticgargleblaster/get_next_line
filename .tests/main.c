#include "../get_next_line.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

#define N_FILES 40

int main()
{

	FILE *fhandles[N_FILES];
	for (int i = 0; i < N_FILES; i++)
	{
		char *fname =ft_strcat(ft_itoa(i), ".txt"); 
		fhandles[i] = fopen(fname, "w");
		printf("file: %s\tfileno:%d\n",fname, fileno(fhandles[i]));
		free(fname);
	}

//	get_next_line();
	return (0);
}