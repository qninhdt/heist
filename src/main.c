#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harray.h"
#include "hlist.h"
#include "hmap.h"
#include <time.h>
typedef struct {
    char* name;
} Room;

typedef hlist(Room) Floor;

void test_list_and_array();

H_HASH_FUNC(char*, str_hash, str) {
    uint32_t hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; 
    }

    return hash;
}

H_EQUAL_FUNC(char*, str_equal, str1, str2) {
    return strcmp(str1, str2) == 0;
}


int main() {
    hmap(char*, Room) tower = hmap_new(str_hash, str_equal);

    clock_t t;
    t = clock();

    for (int i=0; i<2000000; ++i) {
        char* name = malloc(12);
        char* nickname = malloc(12);
        sprintf(name, "qninh-%d", i);
        sprintf(nickname, "qn-%d", i);
        hmap_find_or_create_entry(tower, nickname)->value = ((Room) { name });
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("Set time: %fs\n", time_taken);

    t = clock();

    for (int i=0; i<2000000; ++i) {
        char* nickname = malloc(32);
        sprintf(nickname, "qn-%d", i);
        hmap_get(tower, nickname);
    }

    hmap_delete(tower, "qn-0");

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("Get time: %fs\n", time_taken);
}

// ----------------------------------------------------


// ----------------------------------------------------
H_COPY_FUNC(Room, room_copy, src) {
    char* name = malloc(strlen(src.name));
    strcpy(name, src.name);

    printf("copy %s\n", src.name);
    return (Room) { name };
}

H_FREE_FUNC(Room, room_free, src) {
    printf("remove %s\n", src.name);
    free(src.name);
}

H_COPY_FUNC(Floor, floor_copy, floor) {
    return hlist_new_copy(floor);
}

H_FREE_FUNC(Floor, floor_free, floor) {
    hlist_free(floor);
}

void test_list_and_array() {
    harray(Floor) tower = harray_new();
    harray_set_copy_func(tower, floor_copy);
    harray_set_free_func(tower, floor_free);

    for (int i=1;i<=5;++i) {
        Floor floor = hlist_new();
        hlist_set_copy_func(floor, room_copy);
        hlist_set_free_func(floor, room_free);
        
        for (int j=1;j<=i;++j) {
            char* name = malloc(32);
            sprintf(name, "[room %d/%d]", i, j);
            hlist_push_back(floor, ((Room) { name }));
        }

        harray_push_back(tower, floor);
    }

    harray(Floor) tower0 = harray_new_copy(tower);
    harray_free(tower);

    harray_foreach(tower0, floor, _) {
        hlist_foreach(floor, room, _) {
            printf("%s ", room.name);
        }
        printf("\n");
    }
}