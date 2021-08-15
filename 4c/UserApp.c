/**
 * Opens our newly created char device /dev/mychardev3
 * 
 * make sure to execute sudo mknod /dev/mychardev3  with the allocated number shown in dmesg after running insmod chardriver_module.ko
 * before running this program
 */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define DEV_NAME "mychardev3"

int main()
{
    int d_fd;
    d_fd = open("/dev/" DEV_NAME, O_RDWR, 0777);

    if (d_fd < 0)
    {
        printf("Could not open device\n");
        return -1;
    }

    close(d_fd);
    printf("\nDevice /dev/" DEV_NAME " opened and closed successfully\n\n");
    return 0;
}
