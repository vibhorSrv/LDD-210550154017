#define MAGIC_NUM 'O'

#define IOCT_NUM1 _IOWR(MAGIC_NUM, 21, int)
#define IOCT_NUM2 _IOWR(MAGIC_NUM, 22, int)
#define IOCT_RES _IOR(MAGIC_NUM, 23, int)
#define IOCT_OPR _IOWR(MAGIC_NUM, 24, char)