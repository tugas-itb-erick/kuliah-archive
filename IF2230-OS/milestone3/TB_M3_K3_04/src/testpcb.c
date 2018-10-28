#define MAIN

#include "stdio.h"
#include "assert.h"
#include <string.h>
#include "pcb.h"

void testInit() {
	initializeProcStructures();

	assert(running == &idleProc);
	assert(readyHead == NULL);
	assert(readyTail == NULL);
	int i=0;
	for (i=0; i<8; i++) {
		assert(memoryMap[i] == FREE);
		assert(pcbPool[i].name[0] == 0x00);
		assert(pcbPool[i].state == DEFUNCT);
		assert(pcbPool[i].segment == 0x00);
		assert(pcbPool[i].stackPointer == 0x00);
	}

	assert(strcmp(idleProc.name, "IDLE\0") == 0);
	assert(idleProc.segment == 0x0000);
	assert(idleProc.stackPointer == 0x0000);
}

void testGetFreeMemorySegment() {
	initializeProcStructures();

	int i=0;
	for (i=0; i<8; i++) {
		int x = getFreeMemorySegment();
		assert(x == i);
		assert(memoryMap[i] == USED);
	}

	int x = getFreeMemorySegment();
	assert(x == -1);
}

void testReleaseMemorySegment() {
	initializeProcStructures();
	testGetFreeMemorySegment();

	int i=0;
	for (i=0; i<8; i++) {
		releaseMemorySegment(i);
		assert(memoryMap[i] == FREE);
	}
}

void testGetFreePCB() {
	initializeProcStructures();

	int i=0;
	for (i=0; i<8; i++) {
		struct PCB *pcb = getFreePCB();
		assert(pcbPool[i].state = STARTING);
		assert(pcb == &pcbPool[i]);
	}

	struct PCB *pcb = getFreePCB();
	assert(pcb == NULL);
}

void testReleasePCB() {
	initializeProcStructures();

	struct PCB *pcbs[8];
	int i=0;
	for(i=0; i<8; i++) {
		pcbs[i] = getFreePCB();
	}

	for (i=0; i<8; i++) {
		releasePCB(pcbs[i]);
		assert(pcbPool[i].state == DEFUNCT);
		assert(pcbPool[i].next == NULL);
		assert(pcbPool[i].prev == NULL);
		assert(pcbPool[i].name[0] == 0x00);
	}
}

void testAddToReady() {
	struct PCB pcb1;
	struct PCB pcb2;
	struct PCB pcb3;

	addToReady(&pcb1);
	assert(readyHead == &pcb1);
	assert(readyTail == &pcb1);

	addToReady(&pcb2);
	assert(readyHead == &pcb1);
	assert(readyTail == &pcb2);
	assert(readyHead->next == &pcb2);
	assert(readyTail->prev == &pcb1);

	addToReady(&pcb3);
	assert(readyHead == &pcb1);
	assert(readyTail == &pcb3);
	assert(readyHead->next == &pcb2);
	assert(readyTail->prev == &pcb2);
	assert(readyHead->next->next == &pcb3);
	assert(readyTail->prev->prev == &pcb1);
}

void testRemoveFromReady() {
	initializeProcStructures();

	struct PCB pcb1;
	struct PCB pcb2;
	struct PCB pcb3;

	struct PCB *pcbNull = removeFromReady();
	assert(pcbNull == NULL);

	addToReady(&pcb1);
	struct PCB *pcbA = removeFromReady();
	assert(pcbA == &pcb1);
	assert(readyHead == NULL);
	assert(readyTail == NULL);

	addToReady(&pcb1);
	addToReady(&pcb2);
	struct PCB *pcbB = removeFromReady();
	assert(pcbB == &pcb1);
	assert(readyHead == &pcb2);
	assert(readyTail == &pcb2);
	assert(readyHead->next == NULL);
	assert(readyHead->prev == NULL);

	struct PCB *pcbC = removeFromReady();
	assert(pcbC == &pcb2);
	assert(readyHead == NULL);
	assert(readyTail == NULL);

	addToReady(&pcb1);
	addToReady(&pcb2);
	addToReady(&pcb3);
	struct PCB *pcbD = removeFromReady();
	assert(pcbD == &pcb1);
	assert(readyHead == &pcb2);
	assert(readyTail == &pcb3);
	assert(readyHead->next == &pcb3);
	assert(readyTail->prev == &pcb2);
	assert(readyHead->prev == NULL);
}

int main() {
	printf("Test 1 initializeProcStructures\n");
	testInit();
	printf("Success\n");

	printf("Test 2 getFreeMemorySegment\n");
	testGetFreeMemorySegment();
	printf("Success\n");

	printf("Test 3 releaseMemorySegment\n");
	testReleaseMemorySegment();
	printf("Success\n");

	printf("Test 4 getFreePCB\n");
	testGetFreePCB();
	printf("Success\n");

	printf("Test 5 releasePCB\n");
	testReleasePCB();
	printf("Success\n");

	printf("Test 6 addToReady\n");
	testAddToReady();
	printf("Success\n");

	printf("Test 7 removeFromReady\n");
	testRemoveFromReady();
	printf("Success\n");
}
