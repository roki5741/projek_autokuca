#define _CRT_SECURE_NO_WARNINGS

#include "app.h"

const char* REGISTER_FILE = "register.txt";
const char* VEHICLE_FILE = "vozila.txt";
const char* MESSAGE_FILE = "inbox.txt";
const char* ERROR_FILE = "greske.txt";
const char* EMPLOYEE_FILE = "zaposlenici.txt";

/* ============================================================
					POMOĆNE FUNKCIJE
============================================================ */

void pauseProgram() {

	printf("\n");
	system("pause");
}

void clearScreen() {

	system("cls");
}

int confirmAction(const char* message) {

	char choice;

	printf("\n%s (y/n): ", message);
	scanf(" %c", &choice);

	return (choice == 'y' || choice == 'Y');
}

/* ============================================================
					REGISTER FUNKCIJA
============================================================ */

int registerUser() {

	FILE* file = fopen(REGISTER_FILE, "a+");

	if (!file) {

		perror("Greska datoteke");
		return 0;
	}

	User newUser;
	User temp;

	printf("\n===== REGISTRACIJA =====\n");

	printf("Username: ");
	scanf("%s", newUser.username);

	rewind(file);

	while (fscanf(file,
		"%s %s %d",
		temp.username,
		temp.password,
		&temp.role) != EOF) {

		if (strcmp(temp.username, newUser.username) == 0) {

			printf("\nNetko vec koristi ovaj username!\n");

			fclose(file);

			return 0;
		}
	}

	printf("Password: ");
	scanf("%s", newUser.password);

	newUser.role = USER;

	fprintf(file,
		"%s %s %d\n",
		newUser.username,
		newUser.password,
		newUser.role);

	fclose(file);

	printf("\nRegistracija uspjesna!\n");

	return 1;
}

int loginUser(User* loggedUser) {

	if (!loggedUser)
		return 0;

	char username[MAX_STRING];
	char password[MAX_STRING];

	printf("\n===== LOGIN =====\n");

	printf("Username: ");
	scanf("%s", username);

	printf("Password: ");
	scanf("%s", password);

	if (strcmp(username, "admin") == 0 &&
		strcmp(password, "1234") == 0) {

		strcpy(loggedUser->username, username);
		strcpy(loggedUser->password, password);

		loggedUser->role = ADMIN;

		return 1;
	}

	if (strcmp(username, "vlasnik") == 0 &&
		strcmp(password, "1234") == 0) {

		strcpy(loggedUser->username, username);
		strcpy(loggedUser->password, password);

		loggedUser->role = OWNER;

		return 1;
	}

	FILE* file = fopen(REGISTER_FILE, "r");

	if (!file) {

		perror("Greska login");
		return 0;
	}

	User temp;

	while (fscanf(file,
		"%s %s %d",
		temp.username,
		temp.password,
		&temp.role) != EOF) {

		if (strcmp(username, temp.username) == 0 &&
			strcmp(password, temp.password) == 0) {

			*loggedUser = temp;

			fclose(file);

			printf("\nLogin uspjesan!\n");

			return 1;
		}
	}

	fclose(file);

	printf("\nPogresan login!\n");

	return 0;
}