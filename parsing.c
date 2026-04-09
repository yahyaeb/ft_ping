#include "lib.h"

static int validate_count(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (atoi(str) >= 1);
}

char    *parse_args(int argc, char **argv, int *count)
{
    if (argc == 2 && strcmp(argv[1], "-?") == 0)
    {
        print_options();
        exit(0);
    }
    else if (argc == 2 && strcmp(argv[1], "-V") == 0)
    {
        print_version();
        exit(0);
    }
    else if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        g_verbose = 1;
        return (argv[2]);
    }
    else if (argc == 2)
        return (argv[1]);
    else if (argc == 4 && strcmp(argv[1], "-c") == 0)
    {
        if (!validate_count(argv[2]))
        {
            printf("ft_ping: invalid count value\n");
            exit(1);
        }
        *count = atoi(argv[2]);
        return (argv[3]);
    }
    printf("Usage: ./ft_ping [-v] [-c count] <hostname>\n");
    exit(1);
}
