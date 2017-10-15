#include "CalendarParser.h"
#include <ctype.h>


/**
*For checking if two properties are the same
*@return 0 if the properties are the same, non-zero integer returned if properties are different 
**/
int compareProp (const void* first, const void* second);

/**create a property based on string for propName and a string for propDescr
*@return a pointer to the created property on success or a NULL pointer if function fails
*@param propName - a string name for the property
*@param propDescr - a string description for the property
**/
Property* createProperty(char* propName, char* propDescr);

/**
*parse an alarm section of the icaledar file
**/
Alarm* parseAlarm (FILE* file);

//replace new line at end of string
void replaceNewLine(char* string);

/**
*Function for parsing an event for the calendar
**/
Event* parseEvent(FILE* file, int* eventErr);

/**create an event based on string for UID, a DateTime struc for creationDateTime, 
*a List of properties and a List of alarms
*@return a pointer to the created event on success or a NULL pointer if function fails
**/
Event* createEvent(char* UID, DateTime creationDateTime, List properties, List alarms);

/**
*Free the memory allocated for the Event
*free the list of properties using clearList
*free the list of alarms using clearList
**/
void deleteEvent(void* toBeDeleted);

/**
*Returns a humanly readable copy of the event contents
**/
char* printEvent(void* toBePrinted);

/**
*Returns a string that contains a humanly readable copy of the property contents
**/
char* printProp ( void* toBePrinted );

/**
*Free the memory allocated to the property
**/
void deleteProp (void* toBeDeleted);

/**
*Returns a string that contains a humanly readable copy of the alarm contents
**/
char* printAlarm ( void* toBePrinted );

//free the memory allocated to the property
void deleteAlarm (void* toBeDeleted);

/**create an alarm given a string for action, a string for trigger
*and a List of properties
*NOTE: Careful with the list, might have to clear the list after passing
* it  to this funtion
*@return a pointer to the created alarm on success or a NULL pointer on failure
**/
Alarm* createAlarm(char* action, char* trigger, List properties);
