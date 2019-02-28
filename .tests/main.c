#include "../get_next_line.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_FILES 40
#define FILE_SIZE_CHAR 400

void	write_random_bytes_to_file(FILE *f)
{
	size_t bytes_written = 0;
	while (bytes_written < FILE_SIZE_CHAR)
	{
		int *rand_mem = malloc(sizeof(int));
		*rand_mem = random();
		write(fileno(f), (char*)rand_mem, sizeof(int));
		bytes_written += sizeof(int);
	}
}

int main()
{

	FILE *fhandles[N_FILES];
	for (int i = 0; i < N_FILES; i++)
	{
		char *fname =ft_strcat(ft_itoa(i), ".txt"); 
		fhandles[i] = fopen(fname, "w");
		write_random_bytes_to_file(fhandles[i]);
		printf("file: %s\tfileno:%d\n",fname, fileno(fhandles[i]));
		free(fname);
	}
	return (0);
}


