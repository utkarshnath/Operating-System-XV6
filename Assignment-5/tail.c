#include "types.h"
#include "stat.h"
#include "user.h"

char buf[1024] = {'\\'};

void tail(int fd, char *name, int num_of_lines)
{
	int i, n; 
	int line;
	int total_lines = 0;
	int start = 0;

	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		for (i = 0; i <= n; i++) // Loop to calculate total number of lines in the file
		{
			if (buf[i] == '\n')
			{
				total_lines++;
				if (strcmp(name, "") == 0)
				{
					printf(1, "\n");
				}
			}
			else
			{
				if (strcmp(name, "") == 0)
				{
					if (buf[i] == '\0') // Check end of file
					{
						exit();
					}
					if (buf[i] != '\n')
					{
						printf(1, "%c", buf[i]);
					}
					else
					{
						printf(1, "\n");
					}
				}
			}
		}
	}

	close(fd);
	start = total_lines - num_of_lines;
	line = 0;
	fd = open(name, 0);

	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		for (i = 0; i <= n; i++)
		{
			if (buf[i] == '\n')
				line++;
			if (line >= start)
			{
				if (buf[i] != '\n' && line >= start)
				{
					printf(1, "%c", buf[i]);
				}
				else
				{
					printf(1, "\n");
					line++;
				}
			}
		}
	}

	close(fd);
	if (n < 0)
	{
		printf(1, "tail: error while reading \n");
		exit();
	}
}

int main(int argc, char *argv[])
{
	int fd, i;

	if (argc <= 1)
	{
		tail(0, "", 10);
		exit();
	}
	else if (argc == 2)
	{
		for (i = 1; i < argc; i++)
		{
			if ((fd = open(argv[i], 0)) < 0)
			{
				printf(1, "tail: error in opening the file");
				exit();
			}
			tail(fd, argv[i], 10);

			close(fd);
		}
	}
	else if (argc == 3)
	{
		char temp[1024];
		strcpy(temp, argv[1]);
		char *num_str = temp;
		num_str = num_str + 1;
		int num_of_lines = atoi(num_str);

		for (i = 2; i < argc; i++)
		{
			if ((fd = open(argv[i], 0)) < 0)
			{
				printf(1, "tail: error opening the %s\n", argv[i]);
				exit();
			}
			tail(fd, argv[i], num_of_lines);
			close(fd);
		}
	}

	else
	{
		printf(1, "tail: syntax error");
	}
	exit();
}
