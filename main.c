#include "CalendarParser.h"
#include "CalendarHelper.h"

int main (void)
{
	// //check compare property
	// Property first;
	// strcpy(first.propName, "UID");

	// Property second;
	// strcpy(second.propName, "UAE");

	// int compare = compareProperty(first, second);
	// printf("compare result %d\n", compare);

	// //check create property
	// char* newPName = "mew";
	// char* newPDescr = "teacher";
	// Property* new = createProperty(newPName, newPDescr);

	// printf("%s    %s\n", new->propName , new->propDescr);

	//check createCalendar
	char* fileName = "testCalShort.ics";
	Calendar** dummyCal = NULL;

	createCalendar(fileName, dummyCal);

	return 0;
}