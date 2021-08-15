/**
 * Opens our newly created char device /dev/my_ioctldev
 * for ioctl operations
 * 
 * make sure to execute sudo mknod /dev/my_ioctldev c 255 3
 * before running this program
 */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "module_ioctl.h"

#define DEV_NAME "my_ioctldev"

static int user_num1, user_num2, result;
static char user_oper;

int showUsageErrorMessage()
{
    fprintf(stderr, "ERROR: Usage :\n./UserApp 1 + 2 \n# that means: ./UserApp operand1 operator operand2\n# operands should be number and operator should be a character +, -, * or /\n");
    
    return (-1);
}

int main(int argc, char const *argv[])
{
    int retval;

    if (argc < 4)
    {
        retval = showUsageErrorMessage();

        goto ret;
    }
    else
    {
        int d_fd, ioctl_res;

        printf("trying to open device /dev/" DEV_NAME);

        d_fd = open("/dev/" DEV_NAME, O_RDWR, 0777);

        if (d_fd < 0)
        {
            fprintf(stderr, "Could not open device\n");
            return -1;
        }
        user_num1 = atoi(argv[1]);

        user_oper = *argv[2];

        user_num2 = atoi(argv[3]);

        ioctl_res = ioctl(d_fd, IOCT_NUM1, (int *)&user_num1);

        if (ioctl_res < 0)
            goto reterr;

        ioctl_res = ioctl(d_fd, IOCT_OPR, (char *)&user_oper);

        if (ioctl_res < 0)
            goto reterr;

        ioctl_res = ioctl(d_fd, IOCT_NUM2, (int *)&user_num2);

        if (ioctl_res < 0)
            goto reterr;

        ioctl_res = ioctl(d_fd, IOCT_RES, (int *)&result);

        if (ioctl_res < 0)
            goto reterr;

        printf("\nResult Value = %d\n", result);

        retval = 0;

        goto close;

    reterr:
        retval = -1;

        printf("\nOne of the ioctl operations failed!\n");

    close:
        printf("closing device!\n");

        close(d_fd);
    }
ret:
    return retval;
}
