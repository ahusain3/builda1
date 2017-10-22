#include "CalendarParser.h"
#include "CalendarHelper.h"

int main (void)
{
	// //create the list 
	// List list = initializeList(&printProp, &deleteProp, &compareProp);

	// //POPULATE THE LIST
	// char* newPName = "Russel";
	// char* newPDescr = "teacher";
	// Property* first = createProperty(newPName, newPDescr);

	// insertBack(&list, (void*)first);

	// char* secPName = "lion";
	// char* secPDescr = "brave";
	// Property* sec = createProperty(secPName, secPDescr);

	// insertBack(&list, (void*)sec);

	// //create alarm
	// Alarm* dummyAlarm = NULL;
	// char action[200];
	// char* trigger = malloc(sizeof(char) * 100);
	
	// strcpy(action, "Ring");
	// strcpy(trigger, "7am");

	// dummyAlarm = createAlarm(action, trigger, list);


	// //create event
	// //declare and initialize UID and creationDateTime
	// Event* dummyEvent;
	// char UID[1000];
	// DateTime creationDateTime;

	// strcpy(UID, "uniqueEvent007");
	// strcpy(creationDateTime.date, "19961019");
	// strcpy(creationDateTime.time, "140703");

	// //create eventPropList
	// //create the list 
	// List eventPropList = initializeList(&printProp, &deleteProp, &compareProp);

	// //POPULATE THE LIST
	// newPName = "Fruit";
	// newPDescr = "apple";
	// first = createProperty(newPName, newPDescr);

	// insertBack(&eventPropList, (void*)first);

	// secPName = "Sound";
	// secPDescr = "Roar";
	// sec = createProperty(secPName, secPDescr);

	// insertBack(&eventPropList, (void*)sec);

	// //end eventPropList

	// //create eventAlarmList
	// //create the list of properties for the second alarm
	// List alarm2PropList = initializeList(&printProp, &deleteProp, &compareProp);

	// //POPULATE THE LIST
	// newPName = "Sport";
	// newPDescr = "swimming";
	// first = createProperty(newPName, newPDescr);

	// insertBack(&alarm2PropList, (void*)first);

	// secPName = "Season";
	// secPDescr = "winter";
	// sec = createProperty(secPName, secPDescr);

	// insertBack(&alarm2PropList, (void*)sec);

	// //create second alarm
	// Alarm* dummyAlarm2 = NULL;
	// trigger = malloc(sizeof(char) * 100);
	
	// strcpy(action, "Beep");
	// strcpy(trigger, "120pm");

	// dummyAlarm2 = createAlarm(action, trigger, alarm2PropList);

	// //initialize alarm list
	// List alarmList = initializeList(&printAlarm, &deleteAlarm, NULL);
	
	// //populate alarm list
	// insertBack(&alarmList, (void*) dummyAlarm);
	// insertBack(&alarmList, (void*) dummyAlarm2);
	// //END eventAlarmList	

	// dummyEvent = createEvent(UID, creationDateTime, eventPropList, alarmList);


	// //initialize a Calendar structure
	// float version = 2.0;
	// char prodID[1000];
	// strcpy(prodID, "uniqueCalendar001");

	// //for the event we use dummyEvent
	// Calendar* dummyCal = initializeCalendar(version, prodID, dummyEvent);



	// //Print Calendar
	// char* returnString = printCalendar(dummyCal);
	// printf("%s\n", returnString);
	// free(returnString);

	// //Print Calendar again
	// char* printAgain = printCalendar(dummyCal);
	// printf("%s\n", printAgain);
	// free(printAgain);


	// deleteCalendar(dummyCal);

//***PARTITION acutal tests files past this point

	//check createCalendar
	char fileName[250];
	strcpy(fileName , "testCalEvtPropAlm.ics");

	Calendar* cal = NULL;


	createCalendar(fileName, &cal);

	// //Print Calendar
	char* returnString = printCalendar(cal);
	printf("%s\n", returnString);
	free(returnString);

	deleteCalendar(cal);

	return 0;
}