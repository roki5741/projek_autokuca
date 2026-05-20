#define _CRT_SECURE_NO_WARNINGS

#include "app.h"

/* ============================================================
					DODAVANJE VOZILA
============================================================ */
int isVehicleIdTaken(int id) {

	FILE* file = fopen(VEHICLE_FILE, "r");

	if (!file) {
		return 0;
	}

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
		v.description) == 8) {

		if (v.id == id) {
			fclose(file);
			return 1;
		}
	}

	fclose(file);
	return 0;
}
void addVehicle() {

	FILE* file = fopen(VEHICLE_FILE, "a+");

	if (!file) {
		file = fopen(VEHICLE_FILE, "w+");
	}

	if (!file) {
		perror("Greska vozila");
		return;
	}

	Vehicle v;

	printf("\n===== DODAVANJE VOZILA =====\n");

	printf("ID (-1 za izlaz): ");
	scanf("%d", &v.id);

	if (v.id == -1) {
		printf("\nIzasli ste iz dodavanja vozila. Nista nije spremljeno.\n");
		fclose(file);
		return;
	}

	/* ID PROVJERA */
	while (isVehicleIdTaken(v.id)) {
		printf("\n Ovaj ID vec postoji! Unesite novi ID: ");
		scanf("%d", &v.id);

		if (v.id == -1) {
			printf("\nIzasli ste iz dodavanja vozila. Nista nije spremljeno.\n");
			fclose(file);
			return;
		}
	}
	
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
		printf("\nIzasli ste iz dodavanja vozila. Vozilo NIJE objavljeno.\n");
		fclose(file);
		return;
	}

	fprintf(file,
		"%d %s %s %d %.2f %d %s %s\n",
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


	printf("Maksimalna cijena (-1 za izlaz): ");
	scanf("%f", &maxPrice);

	if (minPrice == -1 || maxPrice == -1) {
		printf("\nIzasli ste iz filtera. Nema rezultata.\n");
		fclose(file);
		return;
	}

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