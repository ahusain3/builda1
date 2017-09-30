#include "CalendarParser.h"
#include <ctype.h>


/**
*For checking if two properties are the same
*@return 0 if the properties are the same, non-zero integer returned if properties are different 
**/
int compareProperty (Property first, Property second);

/**create a property based on string for propName and a string for propDescr
*@return a pointer to the created property on success or a NULL pointer if function fails
*@param propName - a string name for the property
*@param propDescr - a string description for the property
**/
Property* createProperty(char* propName, char* propDescr);

/**
*Initialize an alarm based on string for action and string for trigger
**/
Alarm* initializeAlarm (char* action, char* trigger);

//replace new line at end of string
void replaceNewLine(char* string);