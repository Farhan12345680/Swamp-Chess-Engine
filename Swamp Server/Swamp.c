#include "../Board Representation/BoardRepresentation.h";
#include <stdio.h>;

int main()
{
    // DO initialization

    // Server Reading

    while (1)
    {
        char *_str = (char *)malloc(sizeof(char) * 200);

        scanf("%s", _str);

        _str[strcspn(_str, "\r\n")] = '\0';

        if (strcmp(_str, "uci") == 0)
        {
        }
        else
        {
            free(_str);
            break;
        }
    }
}