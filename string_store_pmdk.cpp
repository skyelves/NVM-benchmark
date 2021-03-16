//
// Created by 王柯 on 2021-01-25.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>

using namespace std;

#define LAYOUT_NAME "intro_0"
#define MAXBUFLEN 10

struct my_root {
    size_t len;
    char buf[MAXBUFLEN];
};

int main(int argc, char *argv[]) {
    //argv[1] repersents the pool file
    PMEMobjpool *pop = pmemobj_create(argv[1], LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);
    if (pop == NULL) {
        perror("pmemobj_create");
        return 1;
    }
//    PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
//    my_root *rootp = (my_root *) pmemobj_direct(root);
//
    char buf[MAXBUFLEN] = {"hello"};
//    rootp->len = strlen(buf);
//    pmemobj_persist(pop, &rootp->len, sizeof(rootp->len));
//    pmemobj_memcpy_persist(pop, rootp->buf, buf, rootp->len);
//
//    TX_BEGIN(pop) {
//                    pmemobj_tx_add_range(root, 0, sizeof(my_root));
//                    memcpy(rootp->buf, buf, strlen(buf));
//                }
//    TX_END
    POBJ_LAYOUT_BEGIN(string_store);
    POBJ_LAYOUT_ROOT(string_store, struct my_root)
    POBJ_LAYOUT_END(string_store)
    TOID(struct my_root) root = POBJ_ROOT(pop, struct my_root);
    TX_BEGIN(pop) {
                    TX_MEMCPY(D_RW(root)->buf, buf, strlen(buf));
                }
    TX_END
    cout << D_RO(root)->buf << endl;

    pmemobj_close(pop);
    return 0;
}