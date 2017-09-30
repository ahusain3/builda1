
#include "CalendarParser.h"
#include "CalendarHelper.h"


/** Function to create a Calendar object based on the contents of an iCalendar file.
 *@pre File name cannot be an empty string or NULL.  File name must have the .ics extension.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid calendar has been created, its address was stored in the variable obj, and OK was returned
		or 
		An error occurred, the calendar was not created, all temporary memory was freed, obj was set to NULL, and the 
		appropriate error code was returned
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param fileName - a string containing the name of the iCalendar file
 *@param a double pointer to a Calendar struct that needs to be allocated
**/
ErrorCode createCalendar(char* fileName, Calendar** obj)
{
	int setProdID = 0;
	int setVersion = 0; 
	//open file
	FILE * file;
	file = fopen(fileName, "r");
	
	//check invalid file
	if (file == NULL)
	{
		return (INV_FILE);
	}

	char line[76];
	char* token;

	//get first line thats not a comment
	while ( fgets(line, 100, file) != NULL)
	{
		//while there are comments go to next line
		while(line[0] == ';')
		{
			fgets(line, 100, file);
		}

		printf("line |%s| \n",line);	
		

		//strtok the line
		token = strtok(line,":");
		replaceNewLine(token);

		//first token is not BEGIN
		if ( strcmp(token,"BEGIN") != 0)
		{
			printf("first return\n");
			return INV_CAL;
		}

		token = strtok(NULL,":");
		//remove new line from token
		replaceNewLine(token);



		printf("second |%s|\n", token);
		
		//if tag is not VCALENDAR
		if ( strcmp(token,"VCALENDAR") != 0 )
		{
			printf("return in second\n");
			return INV_CAL;
		}
	
	}

	//loop through lines
	while ( fgets(line, 100, file) != NULL )
	{
		//if first char is ; it is a comment ca va
		if (line[0] == ';')
		{
			printf("it's a comment\n");
		}
		//***REMOVE print statement***
		printf(line);		 
	}

	fclose(file);
	return OK;
}

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
*Initialize alarm
**/
Alarm* initializeAlarm (char* action, char* trigger)
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

	//***UNIFINISHED***
	return newAlarm;
}

//replace new line at end of string
void replaceNewLine(char* string)
{
	//replace newline with null terminator
	int i;
	for (i = 0; i < strlen(string); i++)
	{
		if (iscntrl((int) string[i]) )
		{
			string[i] = '\0';	
		}
	}	
}

 
