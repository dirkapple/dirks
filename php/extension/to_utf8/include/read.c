#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}
int main()
{
	int segment_id;

	unsigned short * shared_memory;

	int share_mem_key = 360000;

	struct shmid_ds shmbuffer;

	int segment_size;

	const int shared_segment_size = 0x12800; /* 分配一个共享内存块 */

	printf("%d\n", shared_segment_size);
	segment_id = shmget(share_mem_key, shared_segment_size, IPC_CREAT | S_IRUSR | S_IWUSR); /* 绑定到共享内存块 */

	shared_memory = (unsigned short *)shmat(segment_id, NULL, 0); // 共享内存标识 绑定到进程内存的地址 可读可写控制
	printf("shared memory attached at address %p\n", shared_memory); /* 确定共享内存的大小 */

	int i = 0;
	int length = 48;

	for(i = 0; i < length; i++) {
		printf("item:%d\n", *(shared_memory + i));
	}

	//sprintf(shared_memory, "Hello, world."); /* 在共享内存中写入一个字符串 */
	//shmdt(shared_memory); /* 脱离该共享内存块 */
	//shared_memory = (char*)shmat(segment_id, (void*) 0x500000, 0);/* 重新绑定该内存块 */
	//printf("shared memory reattached at address %p\n", shared_memory);

	//printf("%s\n", shared_memory); /* 输出共享内存中的字符串 */
	shmdt(shared_memory); /* 脱离该共享内存块 */
	//shmctl(segment_id, IPC_RMID, 0);/* 释放这个共享内存块 */
	return 0;
}
