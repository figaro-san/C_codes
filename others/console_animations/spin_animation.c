#include <stdio.h>
#include <unistd.h>

//Lower limit of x : 0.0000001
#define wait_sec(x) usleep((x) * 1000000)

int main(void)
{
        char parts[5] = {'|', '/', '-', '\\', '\0'};
        float interval = 0.1;

        while (1)
        {
                for (int i = 0; i < 4; i++) {
                        printf("\b%c", parts[i]);
                        fflush(stdout);
                        wait_sec(0.1);
                }  
        }
        return 0;
}
