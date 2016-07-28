#include "linkedList.h"

struct ds18b20_device * addElementLast(UINT64 ROM , struct ds18b20_device * list){
    struct ds18b20_device * tempList = list;
    if (list == NULL)
        return addElementFirst(ROM,list);
    while(tempList->next != NULL)
        tempList = tempList->next;
    struct ds18b20_device * newDevice;
    newDevice = malloc( sizeof(struct ds18b20_device));
    newDevice->ROM = ROM;
    newDevice->prev = tempList;
    newDevice->next = NULL;
    tempList->next = newDevice;
    return list;
}

struct ds18b20_device * addElementFirst(UINT64 ROM , struct ds18b20_device * list){
    struct ds18b20_device * newDevice;
    newDevice = malloc( sizeof(struct ds18b20_device));
    newDevice->ROM = ROM;
    newDevice->next = list;
    newDevice->prev = NULL;
    if (list != NULL)
        list->prev = newDevice;
    return newDevice;
}

struct ds18b20_device * addElement(UINT64 ROM , struct ds18b20_device * list, UINT index){
    if (list == NULL)
        return addElementFirst(ROM,list);
    if (index < 1)
        return list;
    else if (index == 1)
        return addElementFirst(ROM,list);
    else{
        int i;
        struct ds18b20_device * tempList = list;
        for (i=1; i < index; i++){
            tempList = tempList->next;
            if (tempList == NULL)
                return list;
        }
        struct ds18b20_device * newDevice;
        newDevice = malloc( sizeof(struct ds18b20_device));
        newDevice->ROM = ROM;
        newDevice->prev = tempList->prev;
        newDevice->next = tempList;
        tempList->prev->next = newDevice;
        tempList->prev = newDevice;
    }
    return list;
}

struct ds18b20_device * deleteElementLast(struct ds18b20_device * list){
    if (list == NULL)
        return list;
    struct ds18b20_device * tempList = list;
    while(tempList->next != NULL)
        tempList = tempList->next;
    if(tempList->prev != NULL)   
        tempList->prev->next = NULL;
    free(tempList);
    return list;
}

struct ds18b20_device * deleteElementFirst(struct ds18b20_device * list){
    if (list == NULL)
        return list;
    struct ds18b20_device * tempList = list;
    list = list->next;
    if (list != NULL)
        list->prev = NULL;
    free(tempList);
    return list;
}

struct ds18b20_device * deleteElement(struct ds18b20_device * list, UINT index){
    if (list == NULL)
        return list;
    if (index < 1)
        return list;
    else if (index == 1)
        return deleteElementFirst(list);
    else{
        int i;
        struct ds18b20_device * tempList = list;
        for (i=1; i < index; i++){
            tempList = tempList->next;
            if (tempList == NULL)
                return list;
        }
        tempList->prev->next = tempList->next;
        if (tempList->next != NULL)
            tempList->next->prev = tempList->prev;
        free(tempList);
    }
    return list;
}

void deleteAll(struct ds18b20_device * list){
    if (list == NULL)
        return;
    struct ds18b20_device * temp;
    while(list != NULL){
        temp = list;
        list = list->next;
        free(temp);
    }
}

bool searchElementByRom(UINT64 ROM, struct ds18b20_device * list){
    if (list == NULL)
        return false;
    struct ds18b20_device * slider = list;
    while(1){
        if (slider->ROM == ROM){
            return true;
        }else{
            if (slider->next == NULL)
                break;
            slider = slider->next;
        }
    }
    return false;
}

struct ds18b20_device * searchAndPopElementByRom(UINT64 ROM, struct ds18b20_device * list){
    if (list == NULL)
        return list;
    struct ds18b20_device * slider = list;
    while(1){
        if (slider->ROM == ROM){
            if (slider->prev == NULL)
                return deleteElementFirst(list);
            else if (slider->next == NULL)
                return deleteElementLast(list);
            else{
                slider->prev->next = slider->next;
                slider->next->prev = slider->prev;
            }
            free(slider);
            return list;
        }else{
            if (slider->next == NULL)
                break;
            slider = slider->next;
        }
    }
    return NULL;
}
