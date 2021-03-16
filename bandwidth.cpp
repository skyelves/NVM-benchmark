#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>
#include <immintrin.h>
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <sys/timeb.h>

using namespace std;
using namespace std::chrono;

#define MAP_SYNC 0x080000 /* perform synchronous page faults for the mapping */
#define MAP_SHARED_VALIDATE 0x03    /* share + validate extension flags */
#define NUM_THREADS 5
pthread_t tids[NUM_THREADS];

struct thread_data {
    double *addr_dram0;
    double *addr_nvm0;
    double *addr_nvm1;
};

string nvm0 = "/aepmount/test";
string nvm1 = "/aepmount1/test";
string dram0 = "/home/wangke/Optane/test";
double *addr_nvm0[NUM_THREADS] = {nullptr}, *addr_nvm1[NUM_THREADS] = {nullptr}, *addr_dram0[NUM_THREADS] = {nullptr};
uint64_t size = 1 << 30; //1GB
uint64_t cache_line_size = 64; //64Byte
uint64_t len = size / sizeof(uint64_t);

void clflush_array(double *addr[]) {
    for (int j = 0; j < NUM_THREADS; ++j) {
        for (uint64_t i = 0; i < size; i += cache_line_size) {
            _mm_clflush((char *) addr[j] + i);
        }
        _mm_mfence();
    }
}

void *flat(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram0 = arg->addr_dram0;
    double *addr_nvm0 = arg->addr_nvm0;
    double *addr_nvm1 = arg->addr_nvm1;
    for (int i = 0; i < len; i += 32) {
        __m256d value_nvm[8];
        __m256d value_dram[8];
        for (int j = 0; j < 8; ++j) {
            value_nvm[j] = _mm256_load_pd((double const *) (addr_nvm0 + i + j * 4));
            value_dram[j] = _mm256_load_pd((double const *) (addr_dram0 + i + j * 4));
        }
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_dram0 + i + j * 4), value_nvm[j]);
            _mm256_stream_pd((double *) (addr_nvm1 + i + j * 4), value_dram[j]);

        }
    }
}

void *cache(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    for (int i = 0; i < len; i += 32) {
        __m256d value_nvm[8];
        for (int j = 0; j < 8; ++j) {
            value_nvm[j] = _mm256_load_pd((double const *) (addr_nvm + i + j * 4));
        }
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_dram + i + j * 4), value_nvm[j]);
        }
    }
}

void *nvm2dram(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    for (int i = 0; i < len; i += 32) {
        __m256d value_nvm[8];
        for (int j = 0; j < 8; ++j) {
            value_nvm[j] = _mm256_load_pd((double const *) (addr_nvm + i + j * 4));
        }
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_dram + i + j * 4), value_nvm[j]);
        }
    }
}

void *dram2nvm(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    for (int i = 0; i < len; i += 32) {
        __m256d value_dram[8];
        for (int j = 0; j < 8; ++j) {
            value_dram[j] = _mm256_load_pd((double const *) (addr_dram + i + j * 4));
        }
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_nvm + i + j * 4), value_dram[j]);
        }
    }
}

void *readnvm(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    __m256d value;
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            value = _mm256_load_pd((double const *) (addr_nvm + i + j * 4));
        }
    }
    _mm_mfence();
}

void *readdram(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    __m256d value;
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            value = _mm256_load_pd((double const *) (addr_dram + i + j * 4));
        }
    }
    _mm_mfence();
}

void *writenvm(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    __m256d value_dram = _mm256_load_pd((double const *) addr_dram);
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_nvm + i + j * 4), value_dram);
        }
    }
    _mm_mfence();
}

void *writedram(void *threadarg) {
    thread_data *arg = (thread_data *) threadarg;
    double *addr_dram = arg->addr_dram0;
    double *addr_nvm = arg->addr_nvm0;
    __m256d value_dram = _mm256_load_pd((double const *) addr_dram);
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_dram + i + j * 4), value_dram);
        }
    }
    _mm_mfence();
}

void timing(string name, void *func(void *), double *addr_dram[], double *addr_nvm0[], double *addr_nvm1[] = NULL) {
    thread_data td[NUM_THREADS];
    auto start = high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        td[i].addr_dram0 = addr_dram[i];
        td[i].addr_nvm0 = addr_nvm0[i];
        td[i].addr_nvm1 = addr_nvm1[i];
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, func, &td[i]);
        if (ret != 0) {
            cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
//    func(&td);
    for (int j = 0; j < NUM_THREADS; ++j) {
        pthread_join(tids[j], NULL);
    }
    auto end = high_resolution_clock::now();
    std::cout << name << ": "
              << duration_cast<duration<double>>(end - start).count() * 10e9 / len
              << std::endl;
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < NUM_THREADS; ++i) {
        string nvmfile0 = nvm0 + to_string(i) + ".txt";
        string nvmfile1 = nvm1 + to_string(i) + ".txt";
//        string nvmfile1 = nvm1 + to_string(i +  NUM_THREADS) + ".txt";
        string dramfile0 = dram0 + to_string(i) + ".txt";
        int nvm0fd = open(nvmfile0.c_str(), O_CREAT | O_RDWR, 0644);
        int nvm1fd = open(nvmfile1.c_str(), O_CREAT | O_RDWR, 0644);
        int dram0fd = open(dramfile0.c_str(), O_CREAT | O_RDWR, 0644);
//        cout << nvm0fd << endl << nvm1fd << endl << dram0fd << endl;
//        for (int i = 0; i < len; ++i) {
//            double temp = i + 1;
//            write(nvm0fd, &temp, sizeof(double));
//            write(dram0fd, &temp, sizeof(double));
//        }
        addr_nvm0[i] = (double *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SYNC | MAP_SHARED_VALIDATE, nvm0fd,
                                       0);
        addr_nvm1[i] = (double *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SYNC | MAP_SHARED_VALIDATE, nvm1fd,
                                       0);
        addr_dram0[i] = (double *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SHARED, dram0fd, 0);
        close(nvm0fd);
        close(nvm1fd);
        close(dram0fd);
    }

    clflush_array(addr_nvm0);
    clflush_array(addr_nvm1);
    clflush_array(addr_dram0);
    timing("flat", &flat, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_nvm0);
    clflush_array(addr_nvm1);
    clflush_array(addr_dram0);
    timing("cache", &cache, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_nvm0);
    clflush_array(addr_nvm1);
    clflush_array(addr_dram0);
    timing("nvm2dram", &nvm2dram, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_nvm0);
    clflush_array(addr_nvm1);
    clflush_array(addr_dram0);
    timing("dram2nvm", &dram2nvm, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_dram0);
    timing("readdram", &readdram, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_nvm0);
    timing("readnvm", &readnvm, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_dram0);
    timing("writedram", &writedram, addr_dram0, addr_nvm0, addr_nvm1);
    clflush_array(addr_nvm0);
    timing("writenvm", &writenvm, addr_dram0, addr_nvm0, addr_nvm1);

    return 0;
}