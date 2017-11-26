void SearchNode(topNode *node) {
		
}


void PrintNodeData(topNode *node) {
	printf("PID : %d ", node->element.pid);
	printf("lwp : %d ", node->element.lwp);
	printf("time : %s ", node->element.time);
	printf("size : %d ", node->element.size);
	printf("res : %d ", node->element.res);
	printf("command : %s", node->element.command);
}


void InitNode(topNode *node) {
	InitData(&node->element);
	node->link = NULL;
}
void SortData(topNode *node) {
	while (node->link != NULL) {
		PrintData(&node.element);
	}
}


void InitData(topData *data) {
	data->pid = 0;
	strcpy(data->command, "");
	data->lwp = 0;
	strcpy(data->time, "");
	data->size = 0;
	data->res = 0;
}

