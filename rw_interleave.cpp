#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <x86intrin.h>
#include <chrono>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>

using namespace std;

#define MAP_SYNC 0x080000 /* perform synchronous page faults for the mapping */
#define MAP_SHARED_VALIDATE 0x03    /* share + validate extension flags */

#define TESTNUM 10000000
#define NUMTHREAD 16
#define FILE_SIZE 1<<30

string nodeNvmFile = "/aepmount/test0.txt";
string kvNvmFile = "/aepmount1/test1.txt";

int myindex[TESTNUM] = {0};
int o = 0;


struct rw_arg {
    int type;//0 read 1 write 2 mix
    int *counter;
};

void *rw(void *_arg) {
    rw_arg *arg = (rw_arg *) _arg;
    int type = arg->type;
    int *counter = arg->counter;
    for (int i = 0; i < TESTNUM; ++i) {
        if (type == 0) {
            o = counter[myindex[i]];
        } else if (type == 1) {
            _mm_mfence();
            counter[myindex[i]] = i;
            _mm_clflush((char *) (counter + myindex[i]));
        }
    }
}

void test() {
    string nvm[2] = {"/aepmount/test", "/aepmount1/test"};
    for (int i = 0; i < TESTNUM; ++i) {
        myindex[i] = rand() % ((FILE_SIZE) / 8);
    }
    int nvm_fd;
    int *counter[NUMTHREAD];
    rw_arg test_arg[NUMTHREAD];
    for (int i = 0; i < NUMTHREAD; ++i) {
        string nvm_file = nvm[i % 2];
        nvm_file += to_string(i / 2);
        nvm_file += ".txt";
        if (access(nvm_file.c_str(), F_OK) != -1) {
            nvm_fd = open(nvm_file.c_str(), O_CREAT | O_RDWR, 0644);
        } else {
            nvm_fd = open(nvm_file.c_str(), O_CREAT | O_RDWR, 0644);
            if (posix_fallocate(nvm_fd, 0, FILE_SIZE) < 0)
                puts("fallocate fail");
        }
        counter[i] = (int *) mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SYNC | MAP_SHARED_VALIDATE, nvm_fd, 0);
        close(nvm_fd);

        test_arg[i].type = i % 2;
//        test_arg[i].type = (i / 4) % 2;
        test_arg[i].counter = counter[i];
    }
    timeval start, ends;
    gettimeofday(&start, NULL);
//    put();
    pthread_t *tids = new pthread_t[NUMTHREAD];
    for (int i = 0; i < NUMTHREAD; ++i) {
        int ret = pthread_create(&tids[i], NULL, &rw, &(test_arg[i]));
        if (ret != 0) {
            cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    for (int j = 0; j < NUMTHREAD; ++j) {
        pthread_join(tids[j], NULL);
    }
    gettimeofday(&ends, NULL);
    double timeCost = (ends.tv_sec - start.tv_sec) * 1000000 + ends.tv_usec - start.tv_usec;
    double throughPut = (double) TESTNUM * NUMTHREAD / timeCost;
    cout << "Put " << TESTNUM * NUMTHREAD << " kv pais in " << timeCost / 1000000 << " s" << endl;
    cout << "Put ThroughPut: " << throughPut << " Mops" << endl;
    for (int i = 0; i < NUMTHREAD; ++i) {
        munmap(counter[i], FILE_SIZE);
    }
}


int main() {
    test();
    return 0;
}