/**
 * Takes 3 arguments from the user and performs add / subtract / multiply / divide operations based on that
 * this is done by opening 4 different char devices that implement the functionality in their read write functions
 * 
 * 
 */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define ADD_DEV_NAME "my_add_dev" //255 5
#define SUB_DEV_NAME "my_sub_dev" //255 6
#define MUL_DEV_NAME "my_mul_dev" //255 7
#define DIV_DEV_NAME "my_div_dev" //255 8

struct oper_container *u_oper_container;
static char result[50];
int showUsageErrorMessage()
{
    fprintf(stderr, "ERROR: Usage :\n./UserApp 1 + 2 \n# that means: ./UserApp operand1 operator operand2\n# operands should be number and operator should be a character +, -, * or /\n");

    return (-1);
}

int main(int argc, char const *argv[])
{
    int retval;
    char *operand;

    if (argc < 4)
    {
        retval = showUsageErrorMessage();
        goto ret;
    }
    else
    {
        int d_fd;
        switch (*argv[2])
        {
        case '+':
            d_fd = open("/dev/" ADD_DEV_NAME, O_RDWR, 0777);
            goto operate;
        case '-':
            d_fd = open("/dev/" SUB_DEV_NAME, O_RDWR, 0777);
            goto operate;
        case '*':
            d_fd = open("/dev/" MUL_DEV_NAME, O_RDWR, 0777);
            goto operate;
        case '/':
            d_fd = open("/dev/" DIV_DEV_NAME, O_RDWR, 0777);
            goto operate;
        default:
            printf("Operation not supported");
            retval = -1;
            goto ret;
        }
    operate:
        if (d_fd < 0)
        {
            fprintf(stderr, "Could not open device\n");
            retval = -1;
            goto ret;
        }
        else
        {
            write(d_fd, argv[1], strlen(argv[1])); //sending first number to driver
            write(d_fd, argv[3], strlen(argv[3])); //sending second number to driver
            read(d_fd, result, 50);                //reading result from the driver
            printf("Result  = %s\n", result);
            retval = 0;
            printf("closing device!\n");
            close(d_fd);
        }
    }
ret:
    return retval;
}
