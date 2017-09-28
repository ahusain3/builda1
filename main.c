#include "CalendarParser.h"
#include "CalendarHelper.h"

int main (void)
{
	Property first;
	strcpy(first.propName, "UID");

	Property second;
	strcpy(second.propName, "UAE");

	int compare = compareProperty(first, second);
	printf("compare result %d\n", compare);

	char* newPName = "mew";
	char* newPDescr = "teacher";
	Property* new = createProperty(newPName, newPDescr);

	printf("%s    %s\n", new->propName , new->propDescr);

	return 0;
}