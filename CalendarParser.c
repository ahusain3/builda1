
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
	Calendar* cal = malloc(sizeof(Calendar));
	
	// ARG 2 check obj for NULL
	if ( cal == NULL )
	{
		return INV_CAL;
	}

	*obj = cal;
	int eventErr = 0;
	int setProdID = 0;
	int setVersion = 0;
	int setEvent = 0;
	int setEndCal = 0; 
	//open file
	FILE * file;
	file = fopen(fileName, "r");
	
	// ARG 1 check filename for invalid filename
	if (file == NULL)
	{
		return (INV_FILE);
	}

	char line[76];
	char* token;

	//get BEGIN:VCALENDAR line
	if ( fgets(line, 100, file) != NULL)
	{
		//while there are comments go to next line
		while(line[0] == ';')
		{
			fgets(line, 100, file);
		}
			
		//strtok the line
		token = strtok(line,":");
		replaceNewLine(token);

		//first token is not BEGIN
		if ( strcmp(token,"BEGIN") != 0)
		{
			return INV_CAL;
		}

		token = strtok(NULL,":");
		replaceNewLine(token);

		//if tag is not VCALENDAR
		if ( strcmp(token,"VCALENDAR") != 0 )
		{
			return INV_CAL;
		}
	
	}

	//loop through lines
	while ( fgets(line, 100, file) != NULL )
	{
		replaceNewLine(line);

		//if blank line
		if ( strlen(line) == 0 )
		{
			//skip line
			continue;
		}

		char* prodid = "PRODID";
		char* version = "VERSION";
		char* event = "BEGIN:VEVENT";
		char* endCal = "END:VCALENDAR";
		
		//if line has end calendar tag stop reading in calendar content
		if ( strstr(line, endCal) != NULL)
		{
			//set end of calendar
			setEndCal = 1;
			break;
		}

		//if first char is ; it is a comment ca va
		if (line[0] == ';')
		{
			//go to next line
			fgets(line, 100, file);
		}
		
		//if line is PRODID
		else if ( strstr(line, prodid) != NULL )
		{
			if ( setProdID == 1 )
			{
				return DUP_PRODID;
			}
			//get first half of line
			token = strtok(line,":");
			
			if (token == NULL)
			{
				return INV_CAL;
			}
			
			replaceNewLine(token);

			if ( strcmp(token,"PRODID") != 0)
			{
				return INV_CAL;
			}

			//get second half of line
			token = strtok(NULL,":");

			if (token == NULL)
			{
				return INV_CAL;
			}

			replaceNewLine(token);

			//set prodid
			strcpy(cal->prodID, token);
			setProdID = 1;
		}

		//if line is VERSION
		else if ( strstr(line,version) != NULL )
		{
			if ( setVersion == 1 )
			{
				return DUP_VER;
			}

			//get first half of line
			token = strtok(line, ":");
			if ( token == NULL )
			{
				return INV_CAL;
			}

			replaceNewLine(token);

			if ( strcmp(token,"VERSION") != 0)
			{
				return INV_CAL;
			}

			//get second half of line
			token = strtok(NULL, ":");

			if ( token == NULL )
			{
				return INV_CAL;
			}

			replaceNewLine(token);

			//set version
			cal->version = atof(token);
			setVersion = 1;
		}


		//if line is BEGIN:VEVENT
		else if ( strstr(line, event) != NULL )
		{
			//call to create event
			cal->event = parseEvent(file, &eventErr);
			if ( cal->event == NULL )
			{
				return INV_EVENT;
			}
			setEvent = 1;
		}
	}

	if ( setEndCal == 0)
	{
		return INV_CAL;
	}

	if ( setProdID == 0 )
	{
		return INV_PRODID;
	}

	if ( setVersion == 0)
	{
		return INV_VER;
	}

	if ( setEvent == 0 )
	{
		return INV_EVENT;
	}

	fclose(file);
	return OK;
}

/**Initialize the attributes of a Calendar give a float for version, string for product ID
*and a pointer to an Event for the event attribute 
*@return a pointer to the created Calendar on success or a NULL pointer if function fails
**/
Calendar* initializeCalendar(float version, char* prodID, Event* event)
{
	//Arg1 check? no we do the float check in parser
	//Arg2 check prodID for NULL pointer
	if (prodID == NULL)
	{
		return NULL;
	}

	//Arg 3 check event for NULL pointer
	if (event == NULL)
	{
		return NULL;
	}

	//malloc
	Calendar* tempCalendar = malloc(sizeof(Calendar) + sizeof(Event));
	//if malloc fails
	if (tempCalendar == NULL)
	{
		return NULL;
	}

	//initialize attributes

	//version
	tempCalendar->version = version;

	//prodID
	strcpy(tempCalendar->prodID, prodID);

	//event
	tempCalendar->event = event;
	// //we must malloc a new event and copy over
	// //the contents of the argument
	// tempCalendar->event = malloc(sizeof(Event));
	
	// //if malloc fails
	// if (tempCalendar->event == NULL)
	// {
	// 	return NULL;
	// } 

	// //copy over the contents
	// Event* tempEvent = tempCalendar->event;
	// //UID
	// strcpy(tempEvent->UID , event->UID);
	// //creationDateTime
	// tempEvent->creationDateTime = event->creationDateTime;
	// //List of properties
	// tempEvent->properties = event->properties;
	// //List of alarms
	// tempEvent->alarms = event->alarms;

	// //NOTE: Should I free argument now that I have a copy of it?
	// deleteEvent((void*)event);

	//END
	return tempCalendar;
}

/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/
void deleteCalendar(Calendar* obj)
{
	// ARG 1 check obj for NULL
	if ( obj == NULL )
	{
		return;
	}

	//free contents
	//free event
	Event* tempEvent = obj->event;
	deleteEvent((void*) tempEvent);
	
	//free the calendar structure
	free(obj);

	//END
	return;
}
/**create an event based on string for UID, a DateTime struc for creationDateTime, 
*a List of properties and a List of alarms
*@return a pointer to the created event on success or a NULL pointer if function fails
**/
Event* createEvent(char* UID, DateTime creationDateTime, List properties, List alarms)
{
	//Arg 1 check for NULL pointer
	if (UID == NULL)
	{
		return NULL;
	}

	//malloc
	Event* tempEvent = malloc(sizeof(Event));
	//if malloc fails
	if (tempEvent == NULL)
	{
		return NULL;
	}
	//initialize attributes
	strcpy(tempEvent->UID, UID);
	tempEvent->creationDateTime = creationDateTime;
	tempEvent->properties = properties;
	tempEvent->alarms = alarms;
}

/**
*Free the memory allocated for the Event
*free the list of properties using clearList
*free the list of alarms using clearList
**/
void deleteEvent(void* toBeDeleted)
{
	//Arg 1 check for NULL
	if (toBeDeleted == NULL)
	{
		return;
	}

	//cast void pointer to Event pointer
	Event* tempEvent = (Event*)toBeDeleted;


	//free contents
		//free properties
		List tempProps = tempEvent->properties;
		clearList(&tempProps);
		//free alarms
		List tempAlarms = tempEvent->alarms;
		clearList(&tempAlarms);

	//free structure
	free(tempEvent);

	//END
	return;
}

/**
*Returns a humanly readable copy of the event contents
**/
char* printEvent(void* toBePrinted)
{
	//Arg1 check for NULL pointer
	if (toBePrinted == NULL)
	{
		return NULL;
	}

	char* tmpStr = NULL;
	char creationDateTime[19];
	char* stringProp = NULL;
	char* stringAlarms = NULL;
	Event* tempEvent = NULL;
	int length = 0;

	tempEvent = (Event*) toBePrinted;	

	//need 1000 chars worth for UID + 19 for creationDateTime + strlen for properties
	//+ strlen for alarms + 1 for null terminator 
	stringAlarms = toString(tempEvent->alarms);
	stringProp = toString(tempEvent->properties);
	strcpy(creationDateTime, tempEvent->creationDateTime.date);
	strcat(creationDateTime, "T");
	strcat(creationDateTime, tempEvent->creationDateTime.time);
	strcat(creationDateTime, "Z");

	length = 1000 + 19 + (strlen(stringProp)) + (strlen(stringAlarms)) + 1 ;
	tmpStr = malloc(sizeof(char)*length);


	sprintf(tmpStr, "%s : %s : %s : %s\n", tempEvent->UID, creationDateTime, stringProp, stringAlarms);

	free(stringProp);
	free(stringAlarms);

	//END
	return tmpStr;
}

/**
*Function for parsing an event for the calendar
**/
Event* parseEvent(FILE* file, int* eventErr)
{
	//initialize
	Event* event = malloc(sizeof(Event));
	if ( event == NULL )
	{
		return NULL;
	}
	//UID
	strcpy(event->UID,"");
	//properties
	event->properties = initializeList( &printProp, &deleteProp, NULL );
	//alarms
	event->alarms = initializeList(NULL,NULL,NULL);

	char line[76];
	char* token;


	//boo!
	int setDTSTAMP = 0;
	int setUID = 0;
	int setDTSTART = 0;
	int setDTEND = 0;
	int setDURATION = 0;


	while ( fgets(line, 100, file) != NULL )
	{
		replaceNewLine(line);

		//if blank line
		if ( strlen(line) == 0 )
		{
			//skip line
			continue;
		}

		char* UID = "UID";
		char* DTSTAMP = "DTSTAMP";
		char* alarm = "BEGIN:VALARM";
		char* endEvent = "END:VEVENT";

		//if line is end event tag stop adding event content
		if (strstr(line,endEvent) != NULL)
		{
			break;
		}

		//if first char is ; it is a comment 
		if (line[0] == ';')
		{
			//go to next line
			continue;
		}		
		//get UID
		else if ( strstr(line,UID) != NULL )
		{
			//get first half of line
			token = strtok(line,":");
			
			if (token == NULL)
			{
				return NULL;
			}
			
			replaceNewLine(token);

			if ( strcmp(token,"UID") != 0)
			{
				return NULL;
			}

			//get second half of line
			token = strtok(NULL,":");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//set UID
			strcpy(event->UID, token);			
			//set UID
			setUID = 1;
		}
		//get creationTime
		else if ( strstr(line,DTSTAMP) != NULL )
		{
			//get first part of line
			token = strtok(line,":");
			
			if (token == NULL)
			{
				return NULL;
			}
			
			replaceNewLine(token);

			if ( strcmp(token,"DTSTAMP") != 0)
			{
				return NULL;
			}

			//get second part of line
			token = strtok(NULL,"T");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//set date
			strcpy(event->creationDateTime.date, token);

			//get third part of line
			token = strtok(NULL,"Z");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//set time
			strcpy(event->creationDateTime.time, token);						
			//set creation time
			setDTSTAMP = 1;
		}
		//alarm
		else if ( strstr(line,alarm) != NULL ) 
		{
			//create alarm
			Alarm* newAlarm = parseAlarm(file);
			
			if ( newAlarm == NULL )
			{
				return NULL;
			}
			//add to alarm list
			insertBack( &(event->alarms),(void*) newAlarm);
		}
		//else its a property
		else
		{
			char* dtstart = "DTSTART";
			char* dtend = "DTEND";
			char* duration = "DURATION";
			//create property
			//get first half of line
			token = strtok(line,":;");
			
			if (token == NULL)
			{
				return NULL;
			}
			
			replaceNewLine(token);

			// //store name
			char name[100];
			strcpy(name, token);

			//in creating properties we can check if DTSTART occurs
			if ( strstr(name,dtstart) != NULL )
			{
				//setDTSTART
				setDTSTART = 1;
			}
			//if DTEND occurs
			if ( strstr(name,dtend) != NULL )
			{
				//setDTEND
				setDTEND = 1;
			}
			//if DURATION occurs
			if ( strstr(name,duration) != NULL )
			{
				//setDURATION
				setDURATION = 1;
			}			

			//get second half of line
			token = strtok(NULL,"");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//store descr
			char descr[100];
			strcpy(descr, token);			
			//create property
			Property* newProp = createProperty(name,descr);
			//add property
			insertBack(&(event->properties),(void*) newProp);
		}

	}

	if ( setUID == 0 || setDTSTAMP == 0 )
	{
		return NULL;
	}

	//start with no end and no duration
	if ( setDTSTART == 1 && setDTEND == 0 && setDURATION == 0)
	{
		return NULL;
	}

	// end and duration both occur
	if (setDTEND == 1 && setDURATION == 1)
	{
		return NULL;
	}
	*(eventErr) = OK;
	return event;

}


/**
*For checking if two properties are the same
*Check is done by comparing the property name
*@return 0 if the properties are the same, non-zero integer returned if properties are different 
* return -1 if arguments are NULL (problem strcmp may also return -1)
**/
int compareProp (const void* first, const void* second)
{
	//Arg 1 check if ptr is NULL
	if ( first == NULL )
	{
		return -1;
	}
	//Arg 2 check if ptr is NULL
	if (second == NULL)
	{
		return -1;
	}

	//cast void* to property*
	Property* firstProp = (Property*) first;
	Property* secondProp = (Property*) second;

	char* propName1;
	char* propName2;

	propName1 = firstProp->propName;
	propName2 = secondProp->propName;

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
*Returns a string that contains a humanly readable copy of the property contents
**/
char* printProp ( void* toBePrinted )
{
	char* tmpStr;
	Property* tmpProp;
	int length;

	if ( toBePrinted == NULL )
	{
		return NULL;
	}

	tmpProp = (Property*) toBePrinted;
	//need 200 chars worth for propName ? for propDescr
	length = 200 + strlen(tmpProp->propDescr) + 1;
	tmpStr = malloc(sizeof(char)*length);

	sprintf(tmpStr, "%s : %s\n", tmpProp->propName, tmpProp->propDescr);

	return tmpStr;
}

/**
*Returns a string that contains a humanly readable copy of the alarm contents
**/
char* printAlarm ( void* toBePrinted )
{
	if ( toBePrinted == NULL )
	{
		return NULL;
	}

	char* tmpStr = NULL;
	char* stringProp = NULL;
	Alarm* tmpAlarm = NULL;
	int length;

	tmpAlarm = (Alarm*) toBePrinted;

	//need 200 chars worth for action + strlen for trigger + strlen for properties + 1 for null terminator
	stringProp = toString(tmpAlarm->properties);
	length = 200 + strlen(tmpAlarm->trigger) + (strlen(stringProp)) + 1 ;
	tmpStr = malloc(sizeof(char)*length);

	sprintf(tmpStr, "%s : %s : %s\n", tmpAlarm->action, tmpAlarm->trigger, stringProp);

	free(stringProp);

	//END
	return tmpStr;
}


/**
*parse an alarm section of the icaledar file
**/
Alarm* parseAlarm (FILE* file)
{
	//malloc
	Alarm* newAlarm = malloc( sizeof(Alarm) );
	//malloc failed
	if (newAlarm == NULL)
	{
		return NULL;
	}

	//initialize action
	strcpy(newAlarm->action, "");
	//initialize trigger
	newAlarm->trigger = NULL;
	//initialize property List
	newAlarm->properties = initializeList( &printProp, &deleteProp, NULL );

	char line[76];
	char* token;

	//booleans
	int setAction = 0;
	int setTrigger = 0;
	int setRepeat = 0;
	int setDuration = 0;	

	while ( fgets(line, 100, file) != NULL )
	{
		replaceNewLine(line);

		//if blank line
		if ( strlen(line) == 0 )
		{
			//skip line
			continue;
		}

		char* action = "ACTION";
		char* trigger = "TRIGGER";		
		char* endAlarm = "END:VALARM";

		//if line is end alarm tag stop adding alarm content
		if (strstr(line,endAlarm) != NULL)
		{
			break;
		}

		//if first char is ; it is a comment 
		if (line[0] == ';')
		{
			//go to next line
			continue;
		}

		//if action
		if ( strstr(line,action) != NULL)
		{
			//get first half of line
			token = strtok(line,":");
			
			if (token == NULL)
			{
				return NULL;
			}
			
			replaceNewLine(token);

			if ( strcmp(token,action) != 0)
			{
				return NULL;
			}

			//get second half of line
			token = strtok(NULL,":");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//set UID
			strcpy(newAlarm->action, token);			
			//set UID
			setAction = 1;			
		}
		//else if trigger
		else if ( strstr(line,trigger) != NULL )
		{
			//get first half of line
			token = strtok(line,";");
			
			if (token == NULL)
			{
				return NULL;
			}
			
			replaceNewLine(token);

			if ( strcmp(token,trigger) != 0)
			{
				return NULL;
			}

			//get second half of line
			token = strtok(NULL,"");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//malloc for trigger **** FREE IT IN DELETE
			newAlarm->trigger = malloc(sizeof( char ) * (strlen(token) + 1) );
			if (newAlarm->trigger == NULL)
			{
				//malloc failed
				//change function this should set error to OTHER_ERR
				return NULL;
			}
			//set UID
			strcpy(newAlarm->trigger, token);			
			//set UID
			setTrigger = 1;			
		}
		//else property
		else
		{
			char* repeat = "REPEAT";
			char* duration = "DURATION";
			//create property
			//get first half of line
			token = strtok(line,":;");
			
			if (token == NULL)
			{
				return NULL;
			}
			
			replaceNewLine(token);

			// //store name
			char name[100];
			strcpy(name, token);

			
			//in creating properties we can check if repeat tag occurs
			if ( strstr(name,repeat) != NULL )
			{
				//setRepeat
				setRepeat = 1;
			}
			//if DURATION occurs
			if ( strstr(name,duration) != NULL )
			{
				//setDURATION
				setDuration = 1;
			}			

			//get second half of line
			token = strtok(NULL,"");

			if (token == NULL)
			{
				return NULL;
			}

			replaceNewLine(token);

			//store descr
			char descr[100];
			strcpy(descr, token);			
			//create property
			Property* newProp = createProperty(name,descr);
			//add property
			insertBack(&(newAlarm->properties),(void*) newProp);
		}

	}	

	if ( setAction == 0 || setTrigger == 0)
	{
		return NULL;
	}

	if ( (setRepeat == 0 && setDuration == 1) || (setRepeat == 1 && setDuration == 0) )
	{
		return NULL;
	}

	return newAlarm;
}

/**create an alarm given a string for action, a string for trigger
*and a List of properties
*NOTE: Careful with the list, might have to clear the list after in the deleteAlarm
* Careful with the trigger have to malloc befoer and free it in deleteAlarm 
*@return a pointer to the created alarm on success or a NULL pointer on failure
**/
Alarm* createAlarm(char* action, char* trigger, List properties)
{
	//Arg 1 check action for NULL
	if (action == NULL)
	{
		return NULL;
	}	
	//Arg 2 check trigger for NULL
	if (trigger == NULL)
	{
		return NULL;
	}

	//malloc
	int triggerLen = strlen(trigger);
	Alarm* newAlarm = malloc(sizeof(Alarm) + (sizeof(char) * triggerLen) );

	//if malloc fails
	if (newAlarm == NULL)
	{
		return NULL;
	}
	//initialize attributes
	strcpy(newAlarm->action, action);
	newAlarm->trigger = trigger;
	newAlarm->properties = properties;
	return newAlarm;
}

//free the memory allocated to the alarm
void deleteAlarm (void* toBeDeleted)
{
	//Arg 1 check for NULL pointer
	if (toBeDeleted == NULL)
	{
		return;
	}

	//cast void pointer to Alarm pointer
	Alarm* tmpAlarm = (Alarm*) toBeDeleted;

	//free trigger
	free(tmpAlarm->trigger);

	//clear the list of properties?
	List tmpProp = tmpAlarm->properties;
	clearList(&tmpProp);

	//free alarm struct
	free(tmpAlarm);
	
	//END
	return;
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

//free the memory allocated to the property
void deleteProp (void* toBeDeleted)
{
	//ARG 1 check for NULL
	if ( toBeDeleted == NULL )
	{
		return;
	}

	//cast to void* to property*
	Property* tmpProperty = (Property*) toBeDeleted;
	//free the property struct
	free(tmpProperty);
	//END
	return;
}

/** Function to "convert" the ErrorCode into a humanly redabale string.
 *@return a string contaning a humanly readable representation of the error code by indexing into 
          the descr array using rhe error code enum value as an index
 *@param err - an error code
**/
char* printError(ErrorCode err)
{
	char* returnString = malloc(sizeof(char) * 250);
	if ( returnString == NULL )
	{
		return NULL;
	}

	if (err == INV_FILE)
	{
		returnString = "invalid file";
	}

	else if ( err == INV_VER )
	{
		returnString = "invalid version";
	}

	else if ( err == DUP_VER )
	{
		returnString = "duplicate version";
	}

	else if ( err == INV_PRODID )
	{
		returnString = "invalid product ID";
	}

	else if ( err == DUP_PRODID )
	{
		returnString = "duplicate product ID";
	}

	else if ( err == INV_CAL )
	{
		returnString = "invalid calendar";
	}

	else if ( err == INV_CREATEDT )
	{
		returnString =  "invalid event creation date-time property";
	}

	else if (INV_EVENT)
	{
		returnString = "invalid event";
	}

	else if (OTHER_ERROR)
	{
		returnString = "other error : possibly ran out of memory";
	}

	return NULL;
}

