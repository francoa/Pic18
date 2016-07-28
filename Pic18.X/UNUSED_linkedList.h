/* 
 * File:   linkedList.h
 * Author: Franco
 *
 * Created on 27 de julio de 2016, 20:15
 */

#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

/* Necessary include files */
#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>
#include <stdlib.h>

#endif

struct ds18b20_device{
    UINT64 ROM;
    struct ds18b20_device *next;// = NULL;
    struct ds18b20_device *prev;// = NULL;
};

struct ds18b20_device * addElementLast(UINT64 ROM , struct ds18b20_device * list);
struct ds18b20_device * addElementFirst(UINT64 ROM , struct ds18b20_device * list);
struct ds18b20_device * addElement(UINT64 ROM , struct ds18b20_device * list, UINT index);

struct ds18b20_device * deleteElementLast(struct ds18b20_device * list);
struct ds18b20_device * deleteElementFirst(struct ds18b20_device * list);
struct ds18b20_device * deleteElement(struct ds18b20_device * list, UINT index);
void deleteAll(struct ds18b20_device * list);

bool searchElementByRom(UINT64 ROM, struct ds18b20_device * list);

struct ds18b20_device * searchAndPopElementByRom(UINT64 ROM, struct ds18b20_device * list);

#endif	/* LINKEDLIST_H */

