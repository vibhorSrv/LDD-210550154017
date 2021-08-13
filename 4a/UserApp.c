/**
 * Opens our newly created char device /dev/mychardevice1
 * 
 * make sure to execute sudo mknod /dev/mychardevice1 c 255 0
 * before running this code
 */ 
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int d_fd;
    d_fd = open("/dev/mychardevice1", O_RDWR, 0777);

    if(d_fd<0)
    {
        printf("Could not open device\n");
        return -1;
    }

    close(d_fd);
    return 0;
}
