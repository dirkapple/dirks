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
	const int shared_segment_size = 0x6400; /* 分配一个共享内存块 */

	segment_id = shmget(share_mem_key, shared_segment_size, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR ); /* 绑定到共享内存块 */

	shared_memory = (unsigned short *)shmat(segment_id, NULL, SHM_RDONLY); // 共享内存标识 绑定到进程内存的地址 可读可写控制
	printf("shared memory attached at address %p\n", shared_memory); /* 确定共享内存的大小 */
	shmctl(segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf("segment size: %d\n", segment_size);

	unsigned short map[] = {
		0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000a,0x000b,0x000c,0x000d,0x000e,0x000f,
	    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,0x0018,0x0019,0x001a,0x001b,0x001c,0x001d,0x001e,0x001f,
	    0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x002a,0x002b,0x002c,0x002d,0x002e,0x002f,
	};

	int i = 0;
	int length = 0;
	GET_ARRAY_LEN(map, length);
	printf("%d\n", length);
	for(i = 0; i < length; i++) {
		printf("%d\n", i);
		printf("%x\n", map[i]);
		shared_memory[i] = map[i];
	}

	printf("shared memory attached at address %p\n", shared_memory); /* 确定共享内存的大小 */

	for(i = 0; i < length; i++) {
		printf("item:%d\n", *(shared_memory + i));
	}

	//sprintf(shared_memory, "Hello, world."); /* 在共享内存中写入一个字符串 */
	//shmdt(shared_memory); /* 脱离该共享内存块 */

	//shared_memory = (char*)shmat(segment_id, (void*) 0x500000, 0);/* 重新绑定该内存块 */
	printf("shared memory reattached at address %p\n", shared_memory);
	printf("%s\n", shared_memory); /* 输出共享内存中的字符串 */
	shmdt(shared_memory); /* 脱离该共享内存块 */
	shmctl(segment_id, IPC_RMID, 0);/* 释放这个共享内存块 */
	return 0;
}
