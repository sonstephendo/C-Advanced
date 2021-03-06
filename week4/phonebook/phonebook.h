#include "../../lib/include/jrb.h"
#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H
typedef struct Phone {
	char name[70];
	char phone[20];
}Phone;
void getMenu(char menu[][40], int len);

void getData(char *filename, JRB b, JRB ptr);

void saveData(char *filename, JRB b, JRB ptr);
#endif /* _PHONEBOOK_H */
