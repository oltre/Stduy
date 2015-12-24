#include <stdio.h>

using namespace std;

typedef unsigned int u_int;

int main()
{
    int loop, i;
    u_int tmp1, tmp2, tmp3, tmp4;
    u_int input;
    u_int output;

    scanf("%d", &loop);
    for (i = 0; i < loop; i++) {
        scanf("%u", &input);
        output = 0xFFFFFFFF;
        tmp1 = input & 0x000000FF;
        tmp1 = tmp1 << 24;
        tmp2 = input & 0x0000FF00;
        tmp2 = tmp2 << 8;
        tmp3 = input & 0x00FF0000;
        tmp3 = tmp3 >> 8;
        tmp4 = input & 0xFF000000;
        tmp4 = tmp4 >> 24;

        output = output & (tmp1 | tmp2 | tmp3 | tmp4);
        printf("%u ", output);
    }

    return 0;
}
