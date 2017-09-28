#include "CalendarParser.h"
#include "CalendarHelper.h"


/**
*For checking if two properties are the same
*@return 0 if the properties are the same, non-zero integer returned if properties are different 
**/
int compareProperty (Property first, Property second)
{
	//move to outside function
	//check for NULL
	// if ( first.propName == NULL || second.propName == NULL )
	// {
	// 	return ;
	// }

	char* propName1;
	char* propName2;

	propName1 = first.propName;
	propName2 = second.propName;

	return strcmp(propName1 , propName2);
}

/**create a property based on string for propName and a string for propDescr
*@return a pointer to the created property on success or a NULL pointer if function fails
*@param propName - a string name for the property
*@param propDescr - a string description for the property
**/
Property* createProperty(char* propName, char* propDescr)
{
	if (propName == NULL || propDescr == NULL)
	{
		return NULL;
	}
	Property* newProp = malloc(sizeof(Property) + ((strlen(propDescr)+1) * sizeof(char)) );
	//malloc fails
	if ( newProp == NULL)
	{
		return NULL;
	}
	//now we initialize the attributes 
	strcpy(newProp->propName , propName);
	strcpy(newProp->propDescr , propDescr);
	return newProp; 
}

/**
*create alarm
**/
Alarm* createAlarm (char* action, char* trigger)
{
	if (action == NULL || trigger == NULL)
	{
		return NULL;
	}
	//malloc
	Alarm* newAlarm = malloc( sizeof(Alarm) );
	//malloc failed
	if (newAlarm == NULL)
	{
		return NULL;
	}

	//initialize action
	//initialize trigger
	newAlarm->trigger = malloc(sizeof(char) * (strlen(trigger)+1) );
	//malloc failed
	if (newAlarm->trigger == NULL)
	{
		return NULL;
	}
	//initialize property List
}

 
