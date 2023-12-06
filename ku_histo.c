#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
int MAX_THREADS;
const char* FILENAME;
int n;
int colors[256] = { 0, };
int thread_num = 0;

typedef struct {
	int colorTableOffset;
	int sizeOfColorTable;
	int fd;
} Info;

int read4bytes(int fd, int pos) {
	unsigned char buf[4];
	pread(fd, buf, 4, pos);
	int ret = (int)buf[3];
	for (int i = 2; i >= 0; i--) {
		ret <<= 8;
		ret += (int)buf[i];
	}
	return ret;
}

int getColorTableOffset(int fd) {
	return read4bytes(fd, 10);
}

int getSizeOfColorTable(int fd) {
	return read4bytes(fd, 34);
}

int getResultString(int num, int value, char* str) {
	int len = 0;
	char temp[20];
	do {
		temp[len++] = value % 10 + '0';
		value /= 10;
	} while (value > 0);

	temp[len++] = ' ';

	do {
		temp[len++] = num % 10 + '0';
		num /= 10;
	} while (num > 0);

	for (int i = 0; i < len; i++) {
		str[i] = temp[len - i - 1];
	}
	return len;
}


void writeResults(const char* name, int i) {
    int fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    int sum;
    char newLine = '\n';
    for (int j = 0; j < 256; j += i) {
        sum = 0;
        char str[20];
        for (int k = j; k < j + i && k < 256; k++) {
            sum += colors[k];
        }
        int len = getResultString(j, sum, str);
        write(fd, str, len);
        if (j + i != 256) write(fd, &newLine, 1);
    }
    close(fd);
}

void* thread_rountine(void* arg) {
	Info* info = (Info*)arg;
	pthread_mutex_lock(&lock1);
	int tnum = thread_num++;
	pthread_mutex_unlock(&lock1);
	int sizeOfColorTable = info->sizeOfColorTable;
	int size = sizeOfColorTable / n;
	int start = info->colorTableOffset + size * tnum;
	if (tnum == n - 1) size = sizeOfColorTable - size * tnum;
	
	int fd = info->fd;
	unsigned char buf;
	for (int i = 0; i < size; i++) {
		pthread_mutex_lock(&lock2);
		lseek(fd, start + i, SEEK_SET);
		read(fd, &buf, 1);
		colors[buf]++;
		pthread_mutex_unlock(&lock2);
	}
}

int main(int argc, char* argv[]) {
	n = atoi(argv[1]);
	FILENAME = argv[3];

	int fd = open(FILENAME, O_RDONLY);
	Info* arg = malloc(sizeof(Info));
	arg->sizeOfColorTable = getSizeOfColorTable(fd);
	arg->colorTableOffset = getColorTableOffset(fd);
	arg->fd = fd;

	pthread_t* tid = malloc(sizeof(pthread_t) * n);
	for (int i = 0; i < n; i++) {
		pthread_create(&tid[i], NULL, thread_rountine, arg);
	}

	for (int i = 0; i < n; i++) {
		pthread_join(tid[i], NULL);
	}

	free(tid);
	close(fd);
	free(arg);

	int i = atoi(argv[2]);
	if (256 % i != 0) return 0;

	const char* RESULT = "results.txt";
	writeResults(RESULT, i);

	return 0;
}
