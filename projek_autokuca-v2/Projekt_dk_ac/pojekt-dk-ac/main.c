#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "app.h"

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