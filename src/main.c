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

H_COPY_FUNC(char*, str_copy, src) {
    char* dst = malloc(strlen(src));
    strcpy(dst, src);
    return dst;
}

H_FREE_FUNC(char*, str_free, str) {
    free(str);
}

H_COPY_FUNC(int, int_copy, num) {
    return num*100;
}

int main() {
    hmap(char*, int) tower = hmap_new(str_hash, str_equal);
    hmap_set_key_func(tower, str_copy, str_free);
    hmap_set_value_func(tower, int_copy, NULL);

    hmap_set(tower, "qninh", 12);
    
    typeof(tower) copy = hmap_new_copy(tower);

    printf("%d %d\n", hmap_get(copy, "qninh"), hmap_get(tower, "qninh"));
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
    harray_set_func(tower, floor_copy, floor_free);

    for (int i=1;i<=5;++i) {
        Floor floor = hlist_new();
        hlist_set_func(floor, room_copy, room_free);
        
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