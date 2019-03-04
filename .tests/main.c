#include "../get_next_line.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
	char *line = NULL;

	printf("ac: %d\n", ac);
	if (ac == 1)
	{
		while (get_next_line(STDIN_FILENO, &line))
			printf("%s\n", line);
	}
	else
		av = 0;

	return (0);
}

// echo "foo\nbar\nbaz" | ./a.out
