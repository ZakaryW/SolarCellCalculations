#pragma once

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define SRC 5E-6  //Silver conductor resistivity in (Ohm-cm)
#define S_R 95.0  //Sheet resistivity in (Ohms/square area)


typedef struct busbar {

	double width;
	double length;
	double space;

} BBar;

typedef struct finger {

	double width;
	double length;
	double space;

} Finger;

//Rule followed: Busbar spacing = 2 * Finger length

/*Data input functions*/
Finger FingerInput(Finger userFData, BBar userBData, bool bbCheck) { //(1)
	
	printf("|Start entering finger data below (in cm)|\n\n\n");

	printf("Enter finger width: ");                   //Prompt for finger width                           
	scanf_s("%lf", &userFData.width); printf("\n"); //Assigns input to corresponding variable
	
	if (bbCheck == 1) {                                   //Checks if input is needed
		printf("Enter finger length: ");                  //Gets finger length if needed
		scanf_s("%lf", &userFData.length); printf("\n");
	}
	
	printf("Enter finger spacing: ");                 //Gets finger spacing
	scanf_s("%lf", &userFData.space); printf("\n");

	return userFData;
}


BBar BusBarInput(BBar userBData, Finger userFData, bool bbCheck) { //(2)

	printf("|Start entering bus bar data below (in cm)|\n\n\n");

	printf("Enter bus bar width: ");
	fflush(stdin);
	scanf_s("%lf", &userBData.width); printf("\n");

	printf("Enter bus bar length: ");
	fflush(stdin);
	scanf_s("%lf", &userBData.length); printf("\n");

	if (bbCheck == 1) {
		printf("Enter your bus bar spacing: ");
		fflush(stdin);
		scanf_s("%lf", &userBData.space); printf("\n"); //Gets busbar space if needed
	}
	return userBData;
}

/*Calculation functions translated from formulas*/
double ShadingLoss(BBar userBData, Finger userFData) { //(3)

	double shadeLoss;

	shadeLoss = (userBData.width / userBData.space) + (userFData.width / userFData.space);
	shadeLoss *= 100; //Format for percent
	return shadeLoss;
}

double EmitterLoss(BBar userBData, Finger userFData, double userJmp, double userVmp) { //(4)
	
	double emittLoss;

	emittLoss = (S_R * pow(userFData.space, 2) * userJmp) / (12 * userVmp);
	emittLoss *= 100; //Format for percent
	return emittLoss;
}

double FingerLoss(double JMP, double VMP, Finger userFData) { //(5)

	double fingLoss;

	fingLoss = (SRC * userFData.space * pow(userFData.length, 2) * JMP) / (3 * VMP * userFData.width);
	fingLoss *= 100; //Format for percent
	return fingLoss;
}

double BusBarLoss(double JMP, double VMP, BBar userBData) { //(6)

	double bbarLoss;

	bbarLoss = (SRC * userBData.space * pow(userBData.length, 2) * JMP) / (3 * VMP * userBData.width);
	bbarLoss *= 100;
	return bbarLoss;
}

double TotalLoss(double shadeLoss, double emittLoss, double fingLoss, double bbarLoss) { //(7)
	
	double tLoss;

	tLoss = shadeLoss + emittLoss + fingLoss + bbarLoss;
	return tLoss;
}

/*Handles interaction for VMP*/
double VMPBufferInput(char input, double VMP) { //(8)
	
		while (input != 'y' && input != 'n') {
			do {                            //Removes whitespace from buffer
				input = getchar();
			} while (input != EOF && input != '\n');
			printf("\nPlease enter a valid input(y/n): ");
			input = tolower(fgetc(stdin));
		}
		if (input == 'y') {
			printf("Enter change: ");
			fflush(stdin);              //Flush input preceding scanf for stability
			scanf_s("%lf", &VMP);

			printf("\nCell max voltage has been changed to: %0.2lf\n", VMP);
			return VMP;
		}
		else if (input == 'n') {
			printf("\nProceeding with %0.1lf\n", VMP);
			return VMP;
		}


}

/*Menu display and input, returns corresponding number for the loop. Defaults to 1*/
int MenuSelect(int select) { //(9)
	
	printf("\nPlease select an option below\n");
	printf("\n------------------\n");
	printf("1) View shading loss\n");
	printf("2) View emitter loss\n");
	printf("3) View finger loss\n");
	printf("4) View bus bar loss\n");
	printf("5) View total loss\n");
	printf("6) Exit\n");

	fflush(stdin);
	scanf_s(" %d", &select);

	switch (select) {

	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;
	case 4:
		return 4;
		break;
	case 5:
		return 5;
		break;
	case 6:
		return 6;
		break;
	default:
		return 1;
		break;
	}
}
