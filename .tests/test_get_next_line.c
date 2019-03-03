#include "../get_next_line.h"
#include "../get_next_line.c" // To test module-private functions
#include "../libft/libft.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_FILES 40
#define FILE_SIZE_CHAR 400

FILE *make_file(char *name, char *contents)
{
	FILE *f = fopen(name, "w");
	write(fileno(f), contents, ft_strlen(contents));
	fclose(f);
	f = fopen(name, "r");
	return (f);
}

/*
**  TIIHN:
**	debuffering "foo\n"
**	to what should line_start point?
**		to next_nl + 1, unless that's out of range.
**
**	The breaking case is BUFF_SIZE = 4
**	what should line_start signify in this case?
*/

void	test_reading_from_file()
{
	char	*line;

	char *name = "example.txt";
	FILE *f = make_file(name, "foo\n");

	// Should stop at '\n'
	int r = get_next_line(fileno(f), &line);
	assert(r == 1);
	assert(strcmp(line, "foo") == 0);

	// Should return that reading has been completed.
	// str should be null
	r = get_next_line(fileno(f), &line);
	assert(r == 0);
	assert(line == NULL);

	remove(name);
}

void	test_reading_from_many_files()
{
	FILE *files[N_FILES];
	char *lines[N_FILES];
	char *names[N_FILES];

	for (size_t i = 0; i < N_FILES; i++)
	{
		names[i] = ft_strcat(ft_itoa((int)i), ".txt");
		files[i] = make_file(names[i], ft_strcat(ft_strdup(names[i]), "\n"));
	}	
	for (size_t i = 0; i < N_FILES; i++)
	{
		int r = get_next_line(fileno(files[i]), &(lines[i]));
		assert(r == 1);
		assert(strcmp(lines[i], names[i]) == 0);	
	}
	for (size_t i = 0; i < N_FILES; i++)
	{
		remove(names[i]);
	}
}

void	test_gnl_1_2()
{
	FILE *f = fopen("gnl1_2.txt", "r");
	char *line;
	int r = get_next_line(fileno(f), &line);
	assert(r == 1);
	assert(strcmp(line, "1234567") == 0);

	r = get_next_line(fileno(f), &line);
	assert(r == 1);
	assert(strcmp(line, "abcdefg") == 0);
	
	r = get_next_line(fileno(f), &line);
	assert(r == 0);
	assert(line == NULL);
}

/*
**	This case breaks my logic by having no newline
**	The debufferer remains hungry
**	The debufferer should detect when line_size matches the read_return_value,
**	and when those are less than the BUFF_SIZE.
**
**	But this still breaks in the case that there are multiple newlines within a buffer
*/

void	test_gnl_7_1()
{
	FILE *f = fopen("gnl7_1.txt", "r");
	char *line;
	int r = get_next_line(fileno(f), &line);
	assert(r == 1);
	assert(strcmp(line, "12345678") == 0);

	r = get_next_line(fileno(f), &line);
	assert(r == 0);
	assert(line == NULL);
}

int main()
{
	test_reading_from_file();
	test_reading_from_many_files();
	test_gnl_1_2();
	test_gnl_7_1();
}


