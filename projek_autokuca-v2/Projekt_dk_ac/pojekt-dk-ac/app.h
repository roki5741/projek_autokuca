#define APP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STRING 100
#define DESCRIPTION_SIZE 300

typedef enum Role {
	USER,
	ADMIN,
	OWNER
} Role;

typedef struct User {

	char username[MAX_STRING];
	char password[MAX_STRING];

	Role role;

} User;

typedef struct Vehicle {

	int id;

	char brand[MAX_STRING];
	char model[MAX_STRING];

	int year;

	float price;

	int kilometers;

	char fuel[MAX_STRING];

	char description[DESCRIPTION_SIZE];

} Vehicle;

typedef struct Employee {

	char name[MAX_STRING];
	char position[MAX_STRING];

} Employee;

extern const char* REGISTER_FILE;
extern const char* VEHICLE_FILE;
extern const char* MESSAGE_FILE;
extern const char* ERROR_FILE;
extern const char* EMPLOYEE_FILE;

/* POMOCNE */

void pauseProgram();
void clearScreen();
int confirmAction(const char* message);

/* AUTH */

int registerUser();
int loginUser(User* loggedUser);
void deleteAccount(const char* username);

/* VEHICLES */

void addVehicle();
void showVehicles();
void filterVehicles();
void editVehicle();
void deleteVehicle();

int compareByPrice(const void* a, const void* b);

Vehicle* loadVehicles(int* count);

void sortVehiclesByPrice();

int compareById(const void* a, const void* b);

void searchVehicle();

void vehicleStatistics();

/* INBOX */

void sendInterestMessage(const char* username);
void sendErrorMessage(const char* username);

void viewInboxMessages();
void viewErrorInbox();

void clearInbox();

/* EMPLOYEES */

void addEmployee();
void showEmployees();

/* MENUS */

void userMenu(User loggedUser);
void adminMenu();
void ownerMenu();
