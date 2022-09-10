#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <inttypes.h>

#define STRING_MAX_SIZE 257

// here is defined the size of a element where the pointer p points at
#define CURR_ELEMENT_LENGTH (sizeof(head) + *((unsigned int *) (p + 4)))

// Function that stores data into a data_structure variable which
// will later be used to insert data into the main array
data_structure *create(int type, char *command)
{
	int data_size, sum1, sum2;
	size_t bill1_t, bill2_t;
	if (type == 1) {
		bill1_t = sizeof(int8_t);
		bill2_t = sizeof(int8_t);
	} else if (type == 2) {
		bill1_t = sizeof(int16_t);
		bill2_t = sizeof(int32_t);
	} else if (type == 3) {
		bill1_t = sizeof(int32_t);
		bill2_t = sizeof(int32_t);
	}
	command = strtok(NULL, " ");
	// The one who dedicates a song
	char *dedicate = strdup(command);

	command = strtok(NULL, " ");
	sum1 = atoi(command);
	command = strtok(NULL, " ");
	sum2 = atoi(command);
	command = strtok(NULL, " ");
	// The one who the song is dedicated to
	char *dedicated = strdup(command);

	int len1 = strlen(dedicate);
	int len2 = strlen(dedicated);
	data_structure *tmp = malloc(sizeof(*tmp));
	if (!tmp) {
		exit(1);
	}
	tmp->header = (head *) malloc(sizeof(head));
	tmp->header->type = (unsigned char) type;

	data_size = len1 + len2 + 2 + bill1_t + bill2_t;
	tmp->header->len = data_size;
	tmp->data = malloc(data_size);

	// Copy first string into data
	for (int i = 0; i < strlen(dedicate) + 1; i++)
		memcpy(tmp->data + i, &dedicate[i], sizeof(char));

	// Copy first bill into data
	memcpy(tmp->data + len1 + 1, &sum1, bill1_t);

	// Copy second bill into data
	memcpy(tmp->data + len1 + 1 + bill1_t, &sum2, bill2_t);

	// Copy second string into data
	for (int i = 0; i < strlen(dedicated) + 1; i++) {
		int data_length = len1 + 1 + bill1_t + bill2_t + i;
		memcpy(tmp->data + data_length, &dedicated[i], sizeof(char));
	}
	free(dedicate);
	free(dedicated);
	return tmp;
}

// function that prints an element from the arr array
// the function receives a pointer to that element
void print_element(char **element)
{
	size_t bill1_t, bill2_t;
	unsigned char type = *((unsigned char *) *element);
	*element += sizeof(head);
	char *first_name = (char *) *element;
	*element += strlen(first_name) + 1;

	// printing for a type 1 element
	if (type == 1) {
		bill1_t = bill2_t = sizeof(int8_t);
		int8_t bill1 = *((int8_t *) *element);
		int8_t bill2 = *((int8_t *) (*element + bill1_t));
		*element += bill1_t + bill2_t;
		char *second_name = (char *) *element;
		*element += strlen(second_name) + 1;

		printf("Tipul %hhu\n", type);
		printf("%s pentru %s\n", first_name, second_name);
		printf("%"PRId8"\n%"PRId8"\n\n", bill1, bill2);

	// printing for a type 2 element
	} else if (type == 2) {
		bill1_t = sizeof(int16_t);
		bill2_t = sizeof(int32_t);
		int16_t bill1 = *((int16_t *) *element);
		int32_t bill2 = *((int32_t *) (*element + bill1_t));
		*element += bill1_t + bill2_t;
		char *second_name = (char *) *element;
		*element += strlen(second_name) + 1;

		printf("Tipul %hhu\n", type);
		printf("%s pentru %s\n", first_name, second_name);
		printf("%"PRId16"\n%"PRId32"\n\n", bill1, bill2);

	// printing for a type 3 element
	} else if (type == 3) {
		bill1_t = bill2_t = sizeof(int32_t);
		int32_t bill1 = *((int32_t *) *element);
		int32_t bill2 = *((int32_t *) (*element + bill1_t));
		*element += bill1_t + bill2_t;
		char *second_name = (char *) *element;
		*element += strlen(second_name) + 1;

		printf("Tipul %hhu\n", type);
		printf("%s pentru %s\n", first_name, second_name);
		printf("%"PRId32"\n%"PRId32"\n\n", bill1, bill2);
	}
}

int add_last(void **arr, int *len, data_structure *data)
{
	if (*len == 0) {
		*arr = malloc(sizeof(head) + data->header->len);
		if (*arr == NULL) {
			exit(1);
		}
		memcpy(*arr, data->header, sizeof(head));
		memcpy(*arr + sizeof(head), data->data, data->header->len);
		*len = *len + 1;
	} else {
		char *p = (char *) *arr;
		int size_arr = 0;

		// go to the end of arr
		for (int i = 0; i < *len; i++) {
			size_arr += CURR_ELEMENT_LENGTH;
			p += CURR_ELEMENT_LENGTH;
		}

		void *tmp = realloc(*arr, size_arr + sizeof(head) + data->header->len);
		if (!tmp) {
			return 1;
		}
		*arr = tmp;
		p = *arr + size_arr;
		memcpy(p, data->header, sizeof(head));
		memcpy(p + sizeof(head), data->data, data->header->len);
		*len = *len + 1;
	}
	free(data->header);
	free(data->data);
	free(data);
	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	char *p = (char *) *arr;
	int size_arr = 0;
	int size_to_add = 0;
	// go to the index-th element
	for (int i = 0; i < index; i++) {
		size_arr += CURR_ELEMENT_LENGTH;
		p += CURR_ELEMENT_LENGTH;
	}
	// store the pointer into a variable
	size_to_add = size_arr;

	// go to end of arr
	for (int i = index; i < *len; i++) {
		size_arr += CURR_ELEMENT_LENGTH;
		p += CURR_ELEMENT_LENGTH;
	}
	void *tmp = realloc(*arr, size_arr + sizeof(head) + data->header->len);
	if (!tmp) {
		return 1;
	}
	*arr = tmp;
	char *addtmp = malloc(size_arr - size_to_add + 1);
	p = *arr + size_to_add;

	// make space for the new element
	memcpy(addtmp, p, size_arr - size_to_add);
	memcpy(p + sizeof(head) + data->header->len, addtmp, size_arr - size_to_add);

	// and add it inside of arr
	memcpy(p, data->header, sizeof(head));
	memcpy(p + sizeof(head), data->data, data->header->len);

	free(addtmp);
	*len = *len + 1;
	free(data->header);
	free(data->data);
	free(data);
	return 0;
}

void find(void *data_block, int len, int index) 
{
	char *p = (char *) data_block;
	while (index-- && len--)
		p += CURR_ELEMENT_LENGTH;
	if (p != NULL)
		print_element(&p);
}

int delete_at(void **arr, int *len, int index)
{
	char *p = (char *) *arr;
	int size_arr = 0;
	int size_to_delete = 0;

	// go to the index-th element and store its pointer and length
	for (int i = 0; i < index; i++) {
		size_arr += CURR_ELEMENT_LENGTH;
		p += CURR_ELEMENT_LENGTH;
	}
	size_to_delete = size_arr;
	int del_length = CURR_ELEMENT_LENGTH;

	// go to the end of arr
	for (int i = index; i < *len; i++) {
		size_arr += CURR_ELEMENT_LENGTH;
		p += CURR_ELEMENT_LENGTH;
	}

	char *deltmp = malloc(size_arr - size_to_delete - del_length + 1);
	p = *arr + size_to_delete;
	char *pstart = p;

	// write on top of the element we want to delete the rest of the array
	memcpy(deltmp, p + del_length, size_arr - size_to_delete - del_length);
	memcpy(pstart, deltmp, size_arr - size_to_delete - del_length);

	void *tmp = realloc(*arr, size_arr - del_length);
	*arr = tmp;
	free(deltmp);
	*len = *len - 1;
	return 0;
}

void print(void *arr, int len)
{
	char *p = arr;
	for (int i = 0; i < len; i++)
		print_element(&p);
}

int main() {
	void *arr = NULL;
	int len = 0, type, index;
	char *command_line = (char *) malloc(STRING_MAX_SIZE);
	while (1) {
		fgets(command_line, 256, stdin);
		command_line[strlen(command_line) - 1] = '\0';
		char *command = strtok(command_line, " ");

		// insert function implementation
		if (strcmp(command, "insert") == 0) {
			command = strtok(NULL, " ");
			type = atoi(command);
			data_structure *data = create(type, command);
			add_last(&arr, &len, data);

		// insert_at function implementation
		} else if (strcmp(command, "insert_at") == 0) {
			command = strtok(NULL, " ");
			index = atoi(command);
			command = strtok(NULL, " ");
			type = atoi(command);
			data_structure *data = create(type, command);
			if (index < 0)
				printf("Invalid index\n");
			else if (index >= len)
				add_last(&arr, &len, data);
			else
				add_at(&arr, &len, data, index);

		// delete_at function implementation
		} else if (strcmp(command, "delete_at") == 0) {
			command = strtok(NULL, " ");
			index = atoi(command);
			delete_at(&arr, &len, index);

		// find function implementation
		} else if (strcmp(command, "find") == 0) {
			command = strtok(NULL, " ");
			index = atoi(command);
			find(arr, len, index);

		// print function implementation
		} else if (strcmp(command, "print") == 0) {
			print(arr, len);

		// exit function implementation
		} else if (strcmp(command, "exit") == 0) {
			free(command_line);
			free(arr);
			break;
		}
	}

	return 0;
}
