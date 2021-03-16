//
// Created by 王柯 on 2021-01-26.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>

using namespace std;

#define LAYOUT_NAME "rect_area"

struct rectangle {
    int a;
    int b;
};

struct my_root {
    TOID(struct rectangle) rect;
};

POBJ_LAYOUT_BEGIN(rect_area);
POBJ_LAYOUT_ROOT(rect_area, my_root)

POBJ_LAYOUT_TOID(rect_area, rectangle)
POBJ_LAYOUT_END(rect_area)

int area_calc(const TOID(struct rectangle) rect) {
    return D_RO(rect)->a * D_RO(rect)->b;
}

int main(int argc, char *argv[]) {
    PMEMobjpool *pop = pmemobj_create(argv[1], LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);
    if (pop == NULL) {
        perror("pmemobj_create");
        return 1;
    }
    TOID(my_root) root = POBJ_ROOT(pop, my_root);
    TX_BEGIN(pop) {
                    TX_ADD(root);
                    TOID(rectangle) rect = TX_NEW(rectangle);
                            D_RW(rect)->a = 5;
                            D_RW(rect)->b = 10;
                            D_RW(root)->rect = rect;
                }
    TX_END

    int p = area_calc(D_RO(root)->rect);
    cout<<p<<endl;

    TX_BEGIN(pop) {
        TX_ADD(root);
        TX_FREE(D_RW(root)->rect);
        D_RW(root)->rect = TOID_NULL(rectangle);
    } TX_END


    return 0;
}