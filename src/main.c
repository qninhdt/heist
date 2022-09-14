#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harray.h"
#include "hlist.h"
#include "hmap.h"

typedef struct {
    char* name;
} Room;

typedef hlist(Room) Floor;

void test_list_and_array();

int main() {
    sizeof(struct __hmap_s);
}

Room room_copy(Room src) {
    char* name = malloc(strlen(src.name));
    strcpy(name, src.name);
    printf("copy %s\n", src.name);
    return (Room) { name };
}

void room_free(Room src) {
    printf("remove %s\n", src.name);
    free(src.name);
}

void test_list_and_array() {
    harray(Floor) tower = harray_new();
    harray_set_copy_func(tower, hlist_new_copy);
    harray_set_free_func(tower, hlist_free);
    
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