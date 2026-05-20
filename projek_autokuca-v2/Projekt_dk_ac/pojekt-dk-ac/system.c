#define _CRT_SECURE_NO_WARNINGS

#include "app.h"

/* ============================================================
					PORUKA ZA VOZILO
============================================================ */

void sendInterestMessage(const char* username) {

	FILE* file = fopen(MESSAGE_FILE, "a");

	if (!file)
		return;

	int vehicleId;

	printf("\nID vozila (-1 za izlaz): ");
	scanf("%d", &vehicleId);

	if (vehicleId == -1) {
		printf("\nIzasli ste iz slanja poruke. Nista nije poslano.\n");
		fclose(file);
		return;
	}

	if (!confirmAction("Poslati poruku?")) {
		printf("\nOtkazano!\n");
		fclose(file);
		return;
	}

	if (confirmAction("Jeste li sigurni?")) {

		fprintf(file,
			"KORISNIK: %s | ZAINTERESIRAN ZA VOZILO ID: %d\n",
			username,
			vehicleId);

		printf("\nPoruka poslana!\n");
	}

	fclose(file);
}

/* ============================================================
					PRIJAVA GRESKE
============================================================ */

void sendErrorMessage(const char* username) {

	FILE* file = fopen(ERROR_FILE, "a");

	if (!file)
		return;

	char message[300];

	getchar();

	printf("\nOpis greske: ");

	fgets(message, 300, stdin);

	if (strncmp(message, "exit", 4) == 0) {
		printf("\nIzasli ste iz prijave greske. Nista nije poslano.\n");
		return;
	}

	fprintf(file,
		"KORISNIK: %s | GRESKA: %s\n",
		username,
		message);

	fclose(file);

	printf("\nGreska prijavljena!\n");
}

/* ============================================================
					PREGLED INBOXA
============================================================ */

void viewInboxMessages() {

	int choice;

	do {

		FILE* file = fopen(MESSAGE_FILE, "r");

		if (!file) {
			perror("Greska inbox");
			return;
		}

		char line[500];

		printf("\n==============================\n");
		printf("     INBOX - ZAINTERESIRANI\n");
		printf("==============================\n");

		while (fgets(line, sizeof(line), file)) {
			printf("%s", line);
		}

		fclose(file);

		printf("\n==============================\n");
		printf("1. Osvjezi inbox\n");
		printf("2. Obrisi inbox\n");
		printf("3. Izlaz\n");
		printf("Odabir: ");

		scanf("%d", &choice);

		if (choice == 2) {

			FILE* clear = fopen(MESSAGE_FILE, "w");
			fclose(clear);

			printf("\nInbox obrisan!\n");
		}

	} while (choice != 3);
}

/* ============================================================
					PREGLED GRESAKA
============================================================ */

void viewErrorInbox() {

	int choice;

	do {

		FILE* file = fopen(ERROR_FILE, "r");

		if (!file) {
			perror("Greska");
			return;
		}

		char line[500];

		printf("\n==============================\n");
		printf("        INBOX - GRESKE\n");
		printf("==============================\n");

		while (fgets(line, sizeof(line), file)) {
			printf("%s", line);
		}

		fclose(file);

		printf("\n==============================\n");
		printf("1. Osvjezi\n");
		printf("2. Obrisi sve greske\n");
		printf("3. Izlaz\n");
		printf("Odabir: ");

		scanf("%d", &choice);

		if (choice == 2) {

			FILE* clear = fopen(ERROR_FILE, "w");
			fclose(clear);

			printf("\nGreske obrisane!\n");
		}

	} while (choice != 3);
}

/* ============================================================
					BRISANJE INBOXA
============================================================ */

void clearInbox() {

	FILE* file = fopen(MESSAGE_FILE, "w");

	if (!file)
		return;

	fclose(file);

	printf("\nInbox obrisan!\n");
}

/* ============================================================
					ZAPOSLENICI
============================================================ */

void addEmployee() {

	FILE* file = fopen(EMPLOYEE_FILE, "a");

	if (!file)
		return;

	Employee e;

	printf("\nIme zaposlenika: ");
	scanf("%s", e.name);

	printf("Pozicija: ");
	scanf("%s", e.position);

	fprintf(file,
		"%s %s\n",
		e.name,
		e.position);

	fclose(file);

	printf("\nZaposlenik dodan!\n");
}

void showEmployees() {

	int choice;

	do {

		FILE* file = fopen(EMPLOYEE_FILE, "r");

		if (!file) {
			perror("Greska");
			return;
		}

		Employee e;

		printf("\n==============================\n");
		printf("        ZAPOSLENICI\n");
		printf("==============================\n");

		while (fscanf(file, "%s %s", e.name, e.position) != EOF) {

			printf("\nIme: %s", e.name);
			printf("\nPozicija: %s\n", e.position);
		}

		fclose(file);

		printf("\n==============================\n");
		printf("1. Osvjezi\n");
		printf("2. Dodaj zaposlenika\n");
		printf("3. Izlaz\n");
		printf("Odabir: ");

		scanf("%d", &choice);

		if (choice == 2) {
			addEmployee();
		}

	} while (choice != 3);
}
/* ============================================================
					BRISANJE ACCOUNTA
============================================================ */

void deleteAccount(const char* username) {

	FILE* file = fopen(REGISTER_FILE, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (!file || !temp)
		return;

	User u;

	while (fscanf(file,
		"%s %s %d",
		u.username,
		u.password,
		&u.role) != EOF) {

		if (strcmp(username, u.username) != 0) {

			fprintf(temp,
				"%s %s %d\n",
				u.username,
				u.password,
				u.role);
		}
	}

	fclose(file);
	fclose(temp);

	remove(REGISTER_FILE);

	rename("temp.txt", REGISTER_FILE);

	printf("\nAccount obrisan!\n");
}

/* ============================================================
					USER MENU
============================================================ */

void userMenu(User loggedUser) {

	int choice;

	do {

		printf("\n===== USER MENU =====\n");

		printf("1. Ponudena vozila\n");
		printf("2. Filtriraj vozila\n");
		printf("3. Zainteresiran sam za vozilo\n");
		printf("4. Prijavi gresku\n");
		printf("5. Obrisi account\n");
		printf("6. Logout\n");

		printf("Odabir: ");

		scanf("%d", &choice);

		switch (choice) {

		case 1:
			showVehicles();
			break;

		case 2:
			filterVehicles();
			break;

		case 3:
			sendInterestMessage(loggedUser.username);
			break;

		case 4:
			sendErrorMessage(loggedUser.username);
			break;

		case 5:

			if (confirmAction("Obrisati account?")) {

				deleteAccount(loggedUser.username);
				return;
			}

			break;

		case 6:

			printf("\nLogout uspjesan!\n");
			break;
		}

	} while (choice != 6);
}

/* ============================================================
					ADMIN MENU
============================================================ */

void adminMenu() {

	int choice;

	do {

		printf("\n===== ADMIN MENU =====\n");

		printf("1. Dodaj vozilo\n");
		printf("2. Pregled vozila\n");
		printf("3. Uredi vozilo\n");
		printf("4. Obrisi vozilo\n");
		printf("5. Sortiraj vozila\n");
		printf("6. Pretrazi vozilo\n");
		printf("7. Inbox zainteresirani\n");
		printf("8. Inbox greske\n");
		printf("9. Obrisi inbox\n");
		printf("10. Statistika\n");
		printf("11. Logout\n");

		printf("Odabir: ");

		scanf("%d", &choice);

		switch (choice) {

		case 1:
			addVehicle();
			break;

		case 2:
			showVehicles();
			break;

		case 3:
			editVehicle();
			break;

		case 4:
			deleteVehicle();
			break;

		case 5:
			sortVehiclesByPrice();
			break;

		case 6:
			searchVehicle();
			break;

		case 7:
			viewInboxMessages();
			break;

		case 8:
			viewErrorInbox();
			break;

		case 9:
			clearInbox();
			break;

		case 10:
			vehicleStatistics();
			break;
		}

	} while (choice != 11);
}

/* ============================================================
					OWNER MENU
============================================================ */

void ownerMenu() {

	int choice;

	do {

		printf("\n===== OWNER MENU =====\n");

		printf("1. Dodaj vozilo\n");
		printf("2. Pregled vozila\n");
		printf("3. Uredi vozilo\n");
		printf("4. Obrisi vozilo\n");

		printf("5. Sortiraj vozila\n");
		printf("6. Pretrazi vozilo\n");

		printf("7. Inbox zainteresirani\n");
		printf("8. Inbox greske\n");

		printf("9. Dodaj zaposlenika\n");
		printf("10. Pregled zaposlenika\n");

		printf("11. Obrisi inbox\n");

		printf("12. Statistika autokuce\n");

		printf("13. Logout\n");

		printf("Odabir: ");

		scanf("%d", &choice);

		switch (choice) {

		case 1:
			addVehicle();
			break;

		case 2:
			showVehicles();
			break;

		case 3:
			editVehicle();
			break;

		case 4:
			deleteVehicle();
			break;

		case 5:
			sortVehiclesByPrice();
			break;

		case 6:
			searchVehicle();
			break;

		case 7:
			viewInboxMessages();
			break;

		case 8:
			viewErrorInbox();
			break;

		case 9:
			addEmployee();
			break;

		case 10:
			showEmployees();
			break;

		case 11:
			clearInbox();
			break;

		case 12:
			vehicleStatistics();
			break;
		}

	} while (choice != 13);
}