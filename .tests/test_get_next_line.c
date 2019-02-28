#include "../get_next_line.h"
#include "../libft/libft.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_FILES 40
#define FILE_SIZE_CHAR 400

void	test_reading_from_file()
{
	char	*line;
	FILE *f = fopen("example.txt", "r");	
	get_next_line(fileno(f), &line);
	assert(0 == 1);
}

int main()
{
	test_reading_from_file();
}


