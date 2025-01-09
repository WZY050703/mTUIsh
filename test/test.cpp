#include <stdio.h>
#include "../src/bash/mbash.h"

int main()
{
    printf("->%s\n", get_command(true).c_str());
    return 0;
}