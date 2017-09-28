 /*
 * CIS2750 F2017
 * Assignment 0
 * Ahsen Husain 0895117
 * This file contains the implementation of the linked List API
 */

#include <stdio.h>
#include "LinkedListAPI.h"

/** Function to initialize the list metadata head with the appropriate function pointers.
*@return the list struct
*@param printFunction function pointer to print a single node of the list
*@param deleteFunction function pointer to delete a single piece of data from the list
*@param compareFunction function pointer to compare two nodes of the list in order to test for equality or order
**/
List initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second))
{
	List list;
	list.head = NULL;
	list.tail = NULL;
	list.deleteData = deleteFunction;
	list.compare = compareFunction;
	list.printData = printFunction;
	return list;
}

/**Function for creating a node for the linked list. 
* This node contains abstracted (void *) data as well as previous and next
* pointers to connect to other nodes in the list
*@pre data should be of same size of void pointer on the users machine to avoid size conflicts. data must be valid.
*data must be cast to void pointer before being added.
*@post data is valid to be added to a linked list
*@return On success returns a node that can be added to a linked list. On failure, returns NULL.
*@param data - is a void * pointer to any data type.  Data must be allocated on the heap.
**/
Node* initializeNode(void* data)
{
	//allocate node on heap
	Node* newNode = malloc(sizeof(Node));
	
	//function fails if malloc fails
	if ( newNode == NULL )
	{
		return NULL;
	}

	//function fails if data is NULL
	if (data == NULL)
	{
		free(newNode);
		return NULL;
	}

	//initialize node
	newNode->data = data;
	newNode->previous = NULL;
	newNode->next = NULL;
	return newNode;
}

/**Inserts a Node at the front of a linked list.  List metadata is updated
* so that head and tail pointers are correct.
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the dummy head of the list
*@param toBeAdded a pointer to data that is to be added to the linked list
**/
void insertFront(List* list, void* toBeAdded)
{
	//make new node toBeAdded
	Node* newNode = initializeNode( toBeAdded );

	//check if head is NULL
	if ( list->head == NULL )
	{
		//set head to newNode
		list->head = newNode;
		//set tail as NULL
		list->tail = NULL;
		return;
	}

	//check if head is not NULL
	if ( list->head != NULL )
	{
		//point new next to head
		newNode->next = list->head;
		//point new previous to NULL
		newNode->previous = NULL;
		//point old head previous to new node
		list->head->previous = newNode;
		//set newNode as head
		list->head = newNode;
		return;
	}

	return;
}

/**Inserts a Node at the back of a linked list. 
*List metadata is updated so that head and tail pointers are correct.
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the dummy head of the list
*@param toBeAdded a pointer to data that is to be added to the linked list
**/
void insertBack(List* list, void* toBeAdded)
{
	//make new node using toBeAdded
	Node* newNode = initializeNode( toBeAdded );

	//check if head is NULL
	if ( list->head == NULL )
	{
		//set new node as head
		list->head = newNode;
		//set tail as NULL
		list->tail = NULL;
		return;
	}

	//check if head is not NULL
	if ( list->head != NULL )
	{
		//find last node
		Node* last = list->head;
		while ( last->next != NULL )
		{
			last = last->next;
		}
		//point new prev to last
		newNode->previous = last;
		//point new next to NULL
		newNode->next = NULL;
		//point last next to new
		last->next = newNode;
		//set list tail to new
		list->tail = newNode;
		return;
	}

	return;
}

/** Clears the contents linked list, freeing all memory asspociated with these contents.
* uses the supplied function pointer to release allocated memory for the data
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the List-type dummy node
**/
void clearList(List* list)
{
	Node* tail = list->head;
	Node* head = list->head;

	//while head is not NULL
	while ( head != NULL )
	{
		//set temp to head
		tail = head->next;
		//free temp
		free(head);
		//move head to head next
		head = tail;
	}

	//set head to NULL
	list->head = NULL;
	//set tail to NULL
	list->tail = NULL;;
	return;
}

/** Uses the comparison function pointer to place the element in the 
* appropriate position in the list.
* should be used as the only insert function if a sorted list is required.  
*@pre List exists and has memory allocated to it. Node to be added is valid.
*@post The node to be added will be placed immediately before or after the first occurrence of a related node
*@param list a pointer to the dummy head of the list containing function pointers for delete and compare, as well 
as a pointer to the first and last element of the list.
*@param toBeAdded a pointer to data that is to be added to the linked list
**/
void insertSorted(List* list, void* toBeAdded)
{
	
	//if head and tail NULL
	if( list->head == NULL && list->tail == NULL )
	{
		//make new node  using toBeAdded
		Node* newNode = initializeNode( toBeAdded);
		//set new as head
		list->head = newNode;
		//set tail as NULL
		list->tail = NULL;
		return;
	}

	//if head not NULL and tail NULL (there is just one node in list)
	if( list->head != NULL && list->tail == NULL )
	{
		int nodeOrder = list->compare(toBeAdded , list->head->data);
		if ( nodeOrder < 0 )
		{
			//insert before head
			insertFront(list , toBeAdded);

		}

		if (nodeOrder >= 0)
		{
			//insert after head
			insertBack(list, toBeAdded);
		}
		return;
	} 
	
	//make new node using toBeAdded
	Node* newNode = initializeNode( toBeAdded);

	//connect new node into the middle of the list
	//find node to insert after
	//start at tail go towards head
	
	Node* curr = list->tail;
	while ( curr->previous != NULL )
	{
		
		int order = list->compare(newNode->data, curr->data); 
		//if new data >= curr data
		if ( order >= 0)
		{	
			//insert after curr
			//point new next to curr->next
			newNode->next = curr->next;
			//point new prev to curr
			newNode->previous = curr;
			curr->next = newNode;			
			Node* oldNext = curr->next;
			//if inserting at end of list
			if ( oldNext != NULL )
			{
				oldNext->previous = newNode;
			}
				
			return;
		}
		
		
		//if new < curr AND curr = head
		if ( order < 0 && curr == list->head )
		{
			insertFront(list, toBeAdded);
			return;
		}
		
		curr = curr->previous;
	}
	
	return;
}

/**Returns a pointer to the data at the front of the list. Does not alter list structure.
 *@pre The list exists and has memory allocated to it
 *@param the list struct
 *@return pointer to the data located at the head of the list
 **/
void* getFromFront(List list)
{
	//check for NULL
	if (list.head->data == NULL)
	{
		printf("Warning : front does not have data.");
	}	

	return list.head->data;
}

/**Returns a pointer to the data at the back of the list. Does not alter list structure.
 *@pre The list exists and has memory allocated to it
 *@param the list struct
 *@return pointer to the data located at the tail of the list
 **/
void* getFromBack(List list)
{
	//check for NULL data
	if (list.tail->data == NULL)
	{
		printf("Warning : front does not have data.");
	}

	return list.tail->data;
}

/** Function for creating an iterator for the linked list. 
 * This node contains abstracted (void *) data as well as previous and next
 * pointers to connect to other nodes in the list
 *@pre List exists and is valid
 *@post List remains unchanged.  The iterator has been allocated and points to the head of the list.
 *@return The newly created iterator object.
 *@param list - a pointer to the list to iterate over.
**/
ListIterator createIterator(List list)
{
	ListIterator iter;
	//point iterator current to list head
	iter.current = list.head;
	//return iterator object
	return iter;
}

/** Function that returns the next element of the list through the iterator. 
* This function returns the head of the list the first time it is called after.
* the iterator was created. Every subsequent call returns the next element.
*@pre List exists and is valid.  Iterator exists and is valid.
*@post List remains unchanged.  The iterator points to the next element on the list.
*@return The data associated with the list element that the iterator pointed to when the function was called.
*@param iter - an iterator to a list.
**/
void* nextElement(ListIterator* iter)
{
	//if we have moved off the end
	if ( iter->current == NULL)
	{
		return NULL;
	}

	// store current data
	void* currData = iter->current->data;
	
	// move iter to next node
	iter->current = iter->current->next;

	//return data
	return currData;
}

/** Removes data from from the list, deletes the node and frees the memory,
 * changes pointer values of surrounding nodes to maintain list structure.
 * returns the data 
 * You can assume that the list contains no duplicates
 *@pre List must exist and have memory allocated to it
 *@post toBeDeleted will have its memory freed if it exists in the list.
 *@param list pointer to the dummy head of the list containing deleteFunction function pointer
 *@param toBeDeleted pointer to data that is to be removed from the list
 *@return on success: void * pointer to data  on failure: NULL
 **/
void* deleteDataFromList(List* list, void* toBeDeleted)
{
	//find node
	Node* curr = list->head;
	while ( curr != NULL )
	{
		
		int match = list->compare(toBeDeleted, curr->data); 

		//if we have found the data to delete
		if ( match == 0)
		{
			//store data
			void* returnData = curr->data;

			//if deleting head
			if( curr == list->head )
			{
				//update head
				list->head = curr->next;
				//if new head exists set new head's prev to NULL
				if ( list->head != NULL )
				{
					list->head->previous = NULL;
				}
			}


			//if deleting tail
			else if(curr == list->tail)
			{
				//free node and reconnect list
				Node* beforeDelete = curr->previous;
				beforeDelete->next = NULL;
				//update tail
				list->tail = beforeDelete;
			}

			//deleting from the middle
			else if ( curr != list->tail && curr != list->head ) 	
			{
				//free node and reconnect list
				Node* beforeDelete = curr->previous;
				Node* afterDelete = curr->next;
				//point beforeDelete's next to curr's next
				beforeDelete->next = curr->next;
				//point afterDelete's prev to curr's prev
				afterDelete->previous = curr->previous;
			}


			//point curr next to NULL
			curr->next = NULL;
			//point curr prev to NULL
			curr->previous = NULL; 			
			//free node
			free(curr);



			return returnData;
		}
		curr = curr->next;
	}
	return NULL;
}

/**Returns a string that contains a string representation of
the list traversed from  head to tail. Utilize the list's printData function pointer to create the string.
returned string must be freed by the calling function.
 *@pre List must exist, but does not have to have elements.
 *@param list Pointer to linked list dummy head.
 *@return on success: char * to string representation of list (must be freed after use).  on failure: NULL
 **/
char* toString(List list)
{
	//start at list head
	Node* curr = list.head;
	//declare returnString
	char* returnString = NULL;
	//moving through list and also handling NULL head case
	while ( curr != NULL)
	{
		//get node's String description  ********FREE IT LATER
		char* currData = list.printData(curr->data);
		//add head description to returnString
		//check NULL because for before adding head descr returnString is NULL
		if (returnString == NULL)
		{
			//instead of keeping a character counter we use strlen everytime
			returnString = malloc( sizeof(char) * strlen(currData + 1) );
			if ( returnString == NULL )
			{
				return NULL;
			}
			//use strcpy DO NOT SET pointers equal to each other
			strcpy(returnString , currData);
		}
		else
		{
			//malloc for the size of the old string plus size of the new string
			char* newReturn = malloc( sizeof(char) * ( strlen(returnString) + strlen(currData) + 2 ) );
			if ( newReturn == NULL )
			{
				return NULL;
			} 
			//put the old and new strings into newReturn
			strcpy(newReturn , returnString);
			strcat(returnString , "; ");
			strcat(newReturn , currData);

			//free old memory
			free(returnString);
			//point to newer bigger place in memory
			returnString = newReturn;
		}

		free(currData);
		//move to next node in list
		curr = curr->next;
	}
	return returnString;
}
