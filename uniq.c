#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

char tolower(unsigned char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        ch = 'a' + (ch - 'A');
    return ch;
}

int strcasecmp(char const *a, char const *b)
{
    for (;; a++, b++)
    {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

void uniq(int fd, int cflag, int dflag, int iflag)
{
    int n, l = 0, i = 0;
    char prev[1024];
    char curr[1024];
    int countOccurence = 1;
    int duplicate = 0;

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        for (i = 0; i < n; i++)
        {

            while (buf[i] != '\n' && i < n) // read line by line
            {
                curr[l] = buf[i];
                l++;
                i++;
            }
            if (i == 512) //continue reading the line
                break;
            curr[l] = '\0';
            l = 0;

            if (iflag)
            {
                if (cflag)
                {
                    if (!strcasecmp(prev, curr))
                        countOccurence++;
                    else if (strcmp(prev, ""))
                    {
                        printf(1, "%d %s\n", countOccurence, prev);
                        countOccurence = 1;
                    }
                    if (i == n) //if curr line is the last line
                    {
                        if (countOccurence > 1)
                            printf(1, "%d %s\n", countOccurence, prev);
                        else
                            printf(1, "%d %s\n", countOccurence, curr);
                    }
                }
                else if (dflag)
                {
                    if (!strcasecmp(prev, curr))
                        duplicate = 1;
                    else if (duplicate)
                    {
                        printf(1, "%s\n", prev);
                        duplicate = 0;
                    }
                    if (i == n && duplicate) //if curr line is the last line and is equal to prev
                        printf(1, "%s\n", prev);
                }
                else
                {
                    if (strcasecmp(prev, curr) && strcmp(prev, ""))
                        printf(1, "%s\n", prev);
                    if (i == n) //if curr line is the last line
                        printf(1, "%s\n", prev);
                }
            }
            else if (cflag)
            {
                if (!strcmp(prev, curr))
                    countOccurence++;
                else if (strcmp(prev, ""))
                {
                    printf(1, "%d %s\n", countOccurence, prev);
                    countOccurence = 1;
                }
                if (i == n) //if curr line is the last line
                {
                    if (countOccurence > 1)
                        printf(1, "%d %s\n", countOccurence, prev);
                    else
                        printf(1, "%d %s\n", countOccurence, curr);
                }
            }
            else if (dflag)
            {
                if (!strcmp(prev, curr))
                    duplicate = 1;
                else if (duplicate)
                {
                    printf(1, "%s\n", prev);
                    duplicate = 0;
                }
                if (i == n && duplicate) //if curr line is the last line and is equal to prev
                    printf(1, "%s\n", prev);
            }
            else
            {
                if (strcmp(prev, curr) && strcmp(prev, ""))
                    printf(1, "%s\n", prev);
                if (i == n) //if curr line is the last line
                    printf(1, "%s\n", curr);
            }
            strcpy(prev, curr);
        }
    }
}

int main(int argc, char *argv[])
{
    int fd = 0, i;

    for (i = 0; i < argc; ++i)
    {
    }
    char *filename = 0;

    int cflag = 0, dflag = 0, iflag = 0;

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            cflag = 1;
        }
        else if (!strcmp(argv[i], "-d"))
        {
            dflag = 1;
        }
        else if (!strcmp(argv[i], "-i"))
        {
            iflag = 1;
        }
        else
        {
            filename = argv[i];
        }
    }

    if (filename == 0) //no filename read through pipe
        uniq(0, cflag, dflag, iflag);
    else
    {
        if ((fd = open(filename, 0)) < 0)
        {
            printf(1, "cat: cannot open %s\n", argv[i]);
            exit();
        }
        uniq(fd, cflag, dflag, iflag);
        close(fd);
    }
    exit();
}