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
	FILE *f = fopen("example.txt", "w");
	write(fileno(f), "foo\n", 4);
	fclose(f);

	f = fopen("example.txt", "r");

	// Should stop at '\n'
	int r = get_next_line(fileno(f), &line);
	assert(r == 1);
	assert(strcmp(line, "foo") == 0);

	// Should return that reading has been completed.
	// str should be null
	r = get_next_line(fileno(f), &line);
	assert(r == 0);
	assert(line == NULL);
}

int main()
{
	test_reading_from_file();
}


