#include "stdhdr.h"

void PrintNodeData(topNode *node) {
	printf("PID : %d ", node->element.pid);
	printf("lwp : %d ", node->element.lwp);
	printf("time : %s ", node->element.time);
	printf("size : %d ", node->element.size);
	printf("res : %d ", node->element.res);
	printf("command : %s", node->element.command);
}


