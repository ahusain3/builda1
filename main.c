#include "CalendarParser.h"
#include "CalendarHelper.h"

int main (void)
{
	//create the list 
	List list = initializeList(&printProp, &deleteProp, &compareProp);

	//POPULATE THE LIST
	char* newPName = "Russel";
	char* newPDescr = "teacher";
	Property* first = createProperty(newPName, newPDescr);

	insertBack(&list, (void*)first);

	char* secPName = "lion";
	char* secPDescr = "brave";
	Property* sec = createProperty(secPName, secPDescr);

	insertBack(&list, (void*)sec);

	//create alarm
	Alarm* dummyAlarm = NULL;
	char action[200];
	char* trigger = malloc(sizeof(char) * 100);
	
	strcpy(action, "Ring");
	strcpy(trigger, "7am");

	dummyAlarm = createAlarm(action, trigger, list);


	//create event
	//declare and initialize UID and creationDateTime
	Event* dummyEvent;
	char UID[1000];
	DateTime creationDateTime;

	strcpy(UID, "uniqueEvent007");
	strcpy(creationDateTime.date, "19961019");
	strcpy(creationDateTime.time, "140703");

	//create eventPropList
	//create the list 
	List eventPropList = initializeList(&printProp, &deleteProp, &compareProp);

	//POPULATE THE LIST
	newPName = "Fruit";
	newPDescr = "apple";
	first = createProperty(newPName, newPDescr);

	insertBack(&eventPropList, (void*)first);

	secPName = "Sound";
	secPDescr = "Roar";
	sec = createProperty(secPName, secPDescr);

	insertBack(&eventPropList, (void*)sec);

	//end eventPropList

	//create eventAlarmList
	//create the list of properties for the second alarm
	List alarm2PropList = initializeList(&printProp, &deleteProp, &compareProp);

	//POPULATE THE LIST
	newPName = "Sport";
	newPDescr = "swimming";
	first = createProperty(newPName, newPDescr);

	insertBack(&alarm2PropList, (void*)first);

	secPName = "Season";
	secPDescr = "winter";
	sec = createProperty(secPName, secPDescr);

	insertBack(&alarm2PropList, (void*)sec);

	//create second alarm
	Alarm* dummyAlarm2 = NULL;
	action[200];
	trigger = malloc(sizeof(char) * 100);
	
	strcpy(action, "Beep");
	strcpy(trigger, "120pm");

	dummyAlarm2 = createAlarm(action, trigger, alarm2PropList);

	//initialize alarm list
	List alarmList = initializeList(&printAlarm, &deleteAlarm, NULL);
	
	//populate alarm list
	insertBack(&alarmList, (void*) dummyAlarm);
	insertBack(&alarmList, (void*) dummyAlarm2);
	//END eventAlarmList	

	dummyEvent = createEvent(UID, creationDateTime, eventPropList, alarmList);

	//Print Event
	char* returnString = printEvent((void*) dummyEvent);
	printf("%s\n", returnString);
	free(returnString);

	deleteEvent((void*) dummyEvent);

	//deleteAlarm((void*)dummyAlarm);
	
	// printf("%s    %s\n", new->propName , new->propDescr);

	//check createCalendar
	//char* fileName = "ahsenCalendar.ics";

	//Calendar* cal = NULL;


	//createCalendar(fileName, &cal);
	
	//printf("%s\n", cal->prodID);
	// printf("version %lf\n", cal->version);
	 // printf("%s\n", cal->event->UID);
	// printf("%s\n",cal->event->creationDateTime.date );
	// printf("%s\n",cal->event->creationDateTime.time );
	// char* listDescr = toString(cal->event->properties);
	// printf("%s\n", listDescr);

	//deleteCalendar(cal);

	return 0;
}