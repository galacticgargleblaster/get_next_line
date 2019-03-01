#include "../get_next_line.h"
#include "../libft/libft.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	test_reading_from_file()
{
	char	*line;
	FILE *f = fopen("readme.txt", "w");
	char *content = "foo\n";
	write(fileno(f), content, ft_strlen(content));
	fclose(f);

	f = fopen("readme.txt", "r");

	char *buf = ft_strnew(20);
	int	ret = read(fileno(f), buf, 10);
	
	assert(ret > 0);
	assert(ft_strcmp(content, buf) == 0); 

	// read() only returns 0 if there is nothing more to read
	ret = read(fileno(f), buf, 10);
	assert(ret == 0);

	// as is the case with an empty file
	FILE*g = fopen("empty_file.txt", "w");
	fclose(g);
	g = fopen("empty_file.txt", "r");
	ret = read(fileno(g), buf, 10);
	assert(ret == 0);
}

int main()
{
	test_reading_from_file();
}


