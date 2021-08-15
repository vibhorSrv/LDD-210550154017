/**
 * Opens our newly created char device /dev/mychardev2
 * 
 * make sure to execute sudo mknod /dev/mychardev c 255 1
 * before running this program
 */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEV_NAME "mychardev2"

#define BUFF_SIZ 1024

char buff[BUFF_SIZ];

int showUsageErrorMessage()
{
    printf("ERROR: Usage :\n./UserApp 1 \"DataToWrite To Device\" #to write data to device\nOR\n./UserApp 0 #to read data from device\n");
    return (-1);
}

int main(int argc, char const *argv[])
{
    if (argc < 2 || argc > 3)
    {
       return showUsageErrorMessage();
    }
    else
    {
        int d_fd;
        d_fd = open("/dev/" DEV_NAME, O_RDWR, 0777);
        if (d_fd < 0)
        {
            fprintf(stderr, "Could not open device\n");
            return -1;
        }
        if (atoi(argv[1]) == 0)
        {
            printf("Read from device = %d bytes\n", read(d_fd, buff, BUFF_SIZ));
            printf("Read from device = %s\n", buff);
        }
        else if (atoi(argv[1]) == 1 && argc == 3)
        {
            int bytesWritten;
            bytesWritten = write(d_fd, argv[2], strlen(argv[2]));
            if (bytesWritten > 0)
            {
                printf("Write to device successful %d bytes\n", bytesWritten);
            }
            else
            {
                fprintf(stderr, "Writing to device failed\n");
            }
        }
        else
        {
            close(d_fd);
            return showUsageErrorMessage();
        }
        close(d_fd);
    }
    return 0;
}
