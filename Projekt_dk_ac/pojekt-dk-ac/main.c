/*
================================================================
						AUTOKUCA PROJECT
================================================================

AUTOR: David Keleman
JEZIK: C
IDE: Visual Studio

OPIS:
Kompletan sustav za autokuću:
- Registracija
- Login
- Admin panel
- Owner panel
- CRUD vozila
- Inbox poruke
- Filtriranje vozila
- Zaposlenici
- Sortiranje
- Pretraga
- TXT datoteke
- Dinamička memorija

================================================================
						LOGIN PODACI
================================================================

ADMIN:
username: admin
password: 1234

VLASNIK:
username: vlasnik
password: 1234

================================================================
						DATOTEKE
================================================================

register.txt
vozila.txt
inbox.txt
greske.txt
zaposlenici.txt

================================================================
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* ============================================================
						KONSTANTE
============================================================ */

#define MAX_STRING 100
#define DESCRIPTION_SIZE 300

/* ============================================================
						ENUMI
============================================================ */

typedef enum Role {
	USER,
	ADMIN,
	OWNER
} Role;

/* ============================================================
						STRUKTURE
============================================================ */

/* USER */

typedef struct User {

	char username[MAX_STRING];
	char password[MAX_STRING];

	Role role;

} User;

/* VEHICLE */

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

/* EMPLOYEE */

typedef struct Employee {

	char name[MAX_STRING];
	char position[MAX_STRING];

} Employee;

/* ============================================================
					GLOBALNE DATOTEKE
============================================================ */

static const char* REGISTER_FILE = "register.txt";
static const char* VEHICLE_FILE = "vozila.txt";
static const char* MESSAGE_FILE = "inbox.txt";
static const char* ERROR_FILE = "greske.txt";
static const char* EMPLOYEE_FILE = "zaposlenici.txt";

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

	char answer;

	printf("\n%s (y/n, x za izlaz): ", message);

	scanf(" %c", &answer);

	if (answer == 'x' || answer == 'X')
		return -1;

	return answer == 'y' || answer == 'Y';
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

	/* PROVJERA POSTOJI LI USERNAME */

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

/* ============================================================
					LOGIN FUNKCIJA
============================================================ */

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

	/* ADMIN LOGIN */

	if (strcmp(username, "admin") == 0 &&
		strcmp(password, "1234") == 0) {

		strcpy(loggedUser->username, username);
		strcpy(loggedUser->password, password);

		loggedUser->role = ADMIN;

		return 1;
	}

	/* OWNER LOGIN */

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

/* ============================================================
					DODAVANJE VOZILA
============================================================ */

void addVehicle() {

	FILE* file = fopen(VEHICLE_FILE, "a");

	if (!file) {

		perror("Greska vozila");
		return;
	}

	Vehicle v;

	printf("\n===== DODAVANJE VOZILA =====\n");

	printf("ID: ");
	scanf("%d", &v.id);

	printf("Marka: ");
	scanf("%s", v.brand);

	printf("Model: ");
	scanf("%s", v.model);

	printf("Godina: ");
	scanf("%d", &v.year);

	printf("Cijena: ");
	scanf("%f", &v.price);

	printf("Kilometri: ");
	scanf("%d", &v.kilometers);

	printf("Gorivo: ");
	scanf("%s", v.fuel);

	getchar();

	printf("Opis vozila: ");
	fgets(v.description, DESCRIPTION_SIZE, stdin);

	if (!confirmAction("Spremiti vozilo?")) {
		printf("\nDodavanje otkazano!\n");
		return;
	}

	fprintf(file,
		"%d %s %s %d %.2f %d %s %s",
		v.id,
		v.brand,
		v.model,
		v.year,
		v.price,
		v.kilometers,
		v.fuel,
		v.description);

	fclose(file);

	printf("\nVozilo uspjesno dodano!\n");
}

/* ============================================================
					PRIKAZ VOZILA
============================================================ */

void showVehicles() {

	int choice;

	do {

		FILE* file = fopen(VEHICLE_FILE, "r");

		if (!file) {
			perror("Greska vozila");
			return;
		}

		Vehicle v;

		printf("\n==============================\n");
		printf("        VOZILA\n");
		printf("==============================\n");

		while (fscanf(file,
			"%d %s %s %d %f %d %s %[^\n]",
			&v.id,
			v.brand,
			v.model,
			&v.year,
			&v.price,
			&v.kilometers,
			v.fuel,
			v.description) != EOF) {

			printf("\n=======================");
			printf("\nID: %d", v.id);
			printf("\nMarka: %s", v.brand);
			printf("\nModel: %s", v.model);
			printf("\nGodina: %d", v.year);
			printf("\nCijena: %.2f EUR", v.price);
			printf("\nKilometri: %d", v.kilometers);
			printf("\nGorivo: %s", v.fuel);
			printf("\nOpis: %s", v.description);
			printf("\n=======================\n");
		}

		fclose(file);

		printf("\n1. Osvjezi\n");
		printf("2. Izlaz\n");
		printf("Odabir: ");

		scanf("%d", &choice);

	} while (choice != 2);
}
/* ============================================================
					FILTRIRANJE VOZILA
============================================================ */

void filterVehicles() {

	FILE* file = fopen(VEHICLE_FILE, "r");

	if (!file)
		return;

	float minPrice;
	float maxPrice;

	printf("\n===== FILTER =====\n");

	printf("Minimalna cijena (-1 za izlaz): ");
	scanf("%f", &minPrice);

	if (minPrice == -1)
		return;

	printf("Maksimalna cijena (-1 za izlaz): ");
	scanf("%f", &maxPrice);

	if (maxPrice == -1)
		return;

	Vehicle v;

	while (fscanf(file,
		"%d %s %s %d %f %d %s %[^\n]",
		&v.id,
		v.brand,
		v.model,
		&v.year,
		&v.price,
		&v.kilometers,
		v.fuel,
		v.description) != EOF) {

		if (v.price >= minPrice &&
			v.price <= maxPrice) {

			printf("\n%s %s | %.2f EUR\n",
				v.brand,
				v.model,
				v.price);
		}
	}

	fclose(file);
}

/* ============================================================
					EDIT VOZILA
============================================================ */

void editVehicle() {

	FILE* file = fopen(VEHICLE_FILE, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (!file || !temp)
		return;

	int id;
	int found = 0;

	Vehicle v;

	printf("\nID vozila za uređivanje: ");
	scanf("%d", &id);

	if (!confirmAction("Urediti ovo vozilo?")) {
		printf("\nOtkazano!\n");
		fclose(file);
		fclose(temp);
		return;
	}

	while (fscanf(file,
		"%d %s %s %d %f %d %s %[^\n]",
		&v.id,
		v.brand,
		v.model,
		&v.year,
		&v.price,
		&v.kilometers,
		v.fuel,
		v.description) != EOF) {

		if (v.id == id) {

			found = 1;

			printf("Nova marka: ");
			scanf("%s", v.brand);

			printf("Novi model: ");
			scanf("%s", v.model);

			printf("Nova cijena: ");
			scanf("%f", &v.price);

			printf("Nova kilometraza: ");
			scanf("%d", &v.kilometers);
		}

		fprintf(temp,
			"%d %s %s %d %.2f %d %s %s\n",
			v.id,
			v.brand,
			v.model,
			v.year,
			v.price,
			v.kilometers,
			v.fuel,
			v.description);
	}

	fclose(file);
	fclose(temp);

	remove(VEHICLE_FILE);
	rename("temp.txt", VEHICLE_FILE);

	if (found)
		printf("\nVozilo uređeno!\n");
	else
		printf("\nVozilo nije pronađeno!\n");
}

/* ============================================================
					BRISANJE VOZILA
============================================================ */

void deleteVehicle() {

	FILE* file = fopen(VEHICLE_FILE, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (!file || !temp)
		return;

	int id;
	int found = 0;

	Vehicle v;

	printf("\nID vozila za brisanje: ");
	scanf("%d", &id);

	if (!confirmAction("Obrisati vozilo?")) {
		printf("\nOtkazano!\n");
		fclose(file);
		fclose(temp);
		return;
	}

	while (fscanf(file,
		"%d %s %s %d %f %d %s %[^\n]",
		&v.id,
		v.brand,
		v.model,
		&v.year,
		&v.price,
		&v.kilometers,
		v.fuel,
		v.description) != EOF) {

		if (v.id != id) {

			fprintf(temp,
				"%d %s %s %d %.2f %d %s %s\n",
				v.id,
				v.brand,
				v.model,
				v.year,
				v.price,
				v.kilometers,
				v.fuel,
				v.description);
		}
		else {

			found = 1;
		}
	}

	fclose(file);
	fclose(temp);

	remove(VEHICLE_FILE);
	rename("temp.txt", VEHICLE_FILE);

	if (found)
		printf("\nVozilo obrisano!\n");
	else
		printf("\nVozilo nije pronađeno!\n");
}

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

	if (strncmp(message, "exit", 4) == 0)
		return;

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
					SORTIRANJE
============================================================ */

int compareByPrice(const void* a, const void* b) {

	Vehicle* v1 = (Vehicle*)a;
	Vehicle* v2 = (Vehicle*)b;

	if (v1->price > v2->price)
		return 1;

	if (v1->price < v2->price)
		return -1;

	return 0;
}

/* ============================================================
					UCITAVANJE VOZILA
============================================================ */

Vehicle* loadVehicles(int* count) {

	FILE* file = fopen(VEHICLE_FILE, "r");

	if (!file)
		return NULL;

	*count = 0;

	Vehicle temp;

	while (fscanf(file,
		"%d %s %s %d %f %d %s %[^\n]",
		&temp.id,
		temp.brand,
		temp.model,
		&temp.year,
		&temp.price,
		&temp.kilometers,
		temp.fuel,
		temp.description) != EOF) {

		(*count)++;
	}

	rewind(file);

	Vehicle* vehicles =
		(Vehicle*)calloc(*count, sizeof(Vehicle));

	for (int i = 0; i < *count; i++) {

		fscanf(file,
			"%d %s %s %d %f %d %s %[^\n]",
			&vehicles[i].id,
			vehicles[i].brand,
			vehicles[i].model,
			&vehicles[i].year,
			&vehicles[i].price,
			&vehicles[i].kilometers,
			vehicles[i].fuel,
			vehicles[i].description);
	}

	fclose(file);

	return vehicles;
}

/* ============================================================
					SORTIRANJE PO CIJENI
============================================================ */

void sortVehiclesByPrice() {

	int count = 0;

	Vehicle* vehicles = loadVehicles(&count);

	if (!vehicles)
		return;

	qsort(vehicles,
		count,
		sizeof(Vehicle),
		compareByPrice);

	printf("\n===== SORTIRANA VOZILA =====\n");

	for (int i = 0; i < count; i++) {

		printf("\n%s %s | %.2f EUR\n",
			vehicles[i].brand,
			vehicles[i].model,
			vehicles[i].price);
	}

	free(vehicles);
	vehicles = NULL;
}

/* ============================================================
					PRETRAGA VOZILA
============================================================ */

int compareById(const void* a, const void* b) {

	Vehicle* v1 = (Vehicle*)a;
	Vehicle* v2 = (Vehicle*)b;

	return v1->id - v2->id;
}

void searchVehicle() {

	int count = 0;

	Vehicle* vehicles = loadVehicles(&count);

	if (!vehicles)
		return;

	qsort(vehicles,
		count,
		sizeof(Vehicle),
		compareById);

	Vehicle key;

	printf("\nUnesite ID vozila: ");
	scanf("%d", &key.id);

	Vehicle* found =
		(Vehicle*)bsearch(
			&key,
			vehicles,
			count,
			sizeof(Vehicle),
			compareById);

	if (found) {

		printf("\nVozilo pronađeno!\n");

		printf("Marka: %s\n", found->brand);
		printf("Model: %s\n", found->model);
		printf("Cijena: %.2f EUR\n", found->price);
	}
	else {

		printf("\nVozilo nije pronađeno!\n");
	}

	free(vehicles);
	vehicles = NULL;
}

/* ============================================================
					STATISTIKA
============================================================ */

void vehicleStatistics() {

	FILE* file = fopen(VEHICLE_FILE, "r");

	if (!file)
		return;

	int count = 0;

	float totalPrice = 0;

	Vehicle v;

	while (fscanf(file,
		"%d %s %s %d %f %d %s %[^\n]",
		&v.id,
		v.brand,
		v.model,
		&v.year,
		&v.price,
		&v.kilometers,
		v.fuel,
		v.description) != EOF) {

		count++;

		totalPrice += v.price;
	}

	fclose(file);

	printf("\n===== STATISTIKA =====\n");

	printf("Ukupno vozila: %d\n", count);

	if (count > 0) {

		printf("Prosjecna cijena: %.2f EUR\n",
			totalPrice / count);
	}
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

/* ============================================================
						MAIN
============================================================ */

int main() {

	int choice;

	User loggedUser;

	do {

		printf("\n====================================\n");
		printf("            AUTOKUCA APP\n");
		printf("====================================\n");

		printf("1. Registracija\n");
		printf("2. Login\n");
		printf("0. Izlaz\n");

		printf("Odabir: ");

		scanf("%d", &choice);

		switch (choice) {

		case 1:

			registerUser();
			break;

		case 2:

			if (loginUser(&loggedUser)) {

				if (loggedUser.role == USER) {

					userMenu(loggedUser);
				}
				else if (loggedUser.role == ADMIN) {

					adminMenu();
				}
				else if (loggedUser.role == OWNER) {

					ownerMenu();
				}
			}

			break;

		case 0:

			printf("\nGasenje programa...\n");
			break;

		default:

			printf("\nNevazeci odabir!\n");
		}

	} while (choice != 0);

	return 0;
}