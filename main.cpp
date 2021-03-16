#include <iostream>
#include <string.h>
#include <stdio.h>
//#include <libpmem.h>
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

using namespace std;
using namespace std::chrono;

#define MAP_SYNC 0x080000 /* perform synchronous page faults for the mapping */
#define MAP_SHARED_VALIDATE 0x03    /* share + validate extension flags */

#define TIME_BODY(name, body)                                         \
  do {                                                                \
    auto start = high_resolution_clock::now();                        \
    body                                                              \
    auto end = high_resolution_clock::now();                          \
    std::cout << name << ": "                                         \
              << duration_cast<duration<double>>(end - start).count() * 10e9 / len \
              << std::endl;                                           \
  } while (false)

char filename1[100] = {"/aepmount/test.txt"};
char filename2[200] = {"/home/wangke/Optane/test.txt"};
double *p1 = NULL, *p2 = NULL;
uint64_t size = 1024 * 1024 * 20; //20MB
uint64_t cache_line_size = 64; //64Byte
uint64_t len = size / sizeof(uint64_t);

void clflush_array(double *addr) {
    for (uint64_t i = 0; i < size; i += cache_line_size) {
        _mm_clflush((char *) addr + i);
    }
    _mm_mfence();
}

void flat(double *addr_dram, double *addr_nvm) {
//    for (int i = 0; i < len; i += 4) {
//        __m256d value_nvm = _mm256_load_pd((double const *) (addr_nvm + i));
//        __m256d value_dram = _mm256_load_pd((double const *) (addr_dram + i));
//        _mm256_stream_pd((double *) (addr_nvm + i), value_dram);
//        _mm256_stream_pd((double *) (addr_dram + i), value_nvm);
//    }
    for (int i = 0; i < len; i += 32) {
        __m256d value_nvm[8];
        __m256d value_dram[8];
        for (int j = 0; j < 8; ++j) {
            value_nvm[j] = _mm256_load_pd((double const *) (addr_nvm + i + j * 4));
            value_dram[j] = _mm256_load_pd((double const *) (addr_dram + i + j * 4));
        }
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_dram + i + j * 4), value_nvm[j]);
            _mm256_stream_pd((double *) (addr_nvm + i + j * 4), value_dram[j]);

        }
    }
}

void cache(double *addr_dram, double *addr_nvm) {
//    for (int i = 0; i < len; i += 4) {
//        __m256d value_nvm = _mm256_load_pd((double const *) (addr_nvm + i));
//        _mm256_stream_pd((double *) (addr_dram + i), value_nvm);
//    }
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

void nvm2dram(double *addr_dram, double *addr_nvm) {
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

void dram2nvm(double *addr_dram, double *addr_nvm) {
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

void readdram(double *addr_dram) {
    __m256d value_dram[8];
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            value_dram[j] = _mm256_load_pd((double const *) (addr_dram + i + j * 4));
        }
    }
}

void readnvm(double *addr_nvm) {
    __m256d value_nvm[8];
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            value_nvm[j] = _mm256_load_pd((double const *) (addr_nvm + i + j * 4));
        }
    }
}

void storedram(double *addr_dram) {
    __m256d value = _mm256_load_pd((double const *) (addr_dram));;
    for (int i = 0; i < len; i += 32) {
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_dram + i + j * 4), value);
        }
    }
    _mm_mfence();
}

void storenvm(double *addr_nvm) {
    __m256d value = _mm256_load_pd((double const *) (addr_nvm));;
    for (int i = 0; i < len; i += 32) {
//        _mm256_stream_pd((double *) (addr_nvm + i), value);
        for (int j = 0; j < 8; ++j) {
            _mm256_stream_pd((double *) (addr_nvm + i + j * 4), value);
        }
    }
    _mm_mfence();
}


int main(int argc, char *argv[]) {
    int fd1 = open(filename1, O_CREAT | O_RDWR, 0644);
    int fd2 = open(filename2, O_CREAT | O_RDWR, 0644);
    cout << fd1 << endl << fd2 << endl;
//    for (int i = 0; i < len; ++i) {
//        uint64_t temp = i + 1;
//        write(fd1, &temp, sizeof(uint64_t));
//        write(fd2, &temp, sizeof(uint64_t));
//    }
    p1 = (double *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SYNC | MAP_SHARED_VALIDATE, fd1, 0);
    p2 = (double *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    close(fd1);
    close(fd2);
    if (p1 == (void *) -1) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return -1;
    }

    clflush_array(p1);
    clflush_array(p2);
    TIME_BODY("flat", { flat(p2, p1); });
    clflush_array(p1);
    clflush_array(p2);
    TIME_BODY("cache", { cache(p2, p1); });
    clflush_array(p1);
    clflush_array(p2);
    TIME_BODY("nvm2dram", { nvm2dram(p2, p1); });
    clflush_array(p1);
    clflush_array(p2);
    TIME_BODY("dram2nvm", { dram2nvm(p2, p1); });
    clflush_array(p2);
    TIME_BODY("readdram", { readdram(p2); });
    clflush_array(p1);
    TIME_BODY("readnvm", { readnvm(p1); });
    clflush_array(p2);
    TIME_BODY("storedram", { storedram(p2); });
    clflush_array(p1);
    TIME_BODY("storenvm", { storenvm(p1); });

    return 0;
}