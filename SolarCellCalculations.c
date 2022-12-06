/*************************************************************
Intended Purpose:

Calculate shading loss of solar cell using 
the series of equations that were provided
------------------------------------------

Author: Zakary Watts
--------------------

All functions, libraries, and types 
are defined/ called in the header header file.
************************************************/

#include "SolarCellCalc.h"

int main()
{
    double inputJMP, inputVMP = 0.5; //Default VMP to 1/2, JMP data input
    BBar userBB = {0.0};                     //For bus bar data set
    Finger userFngr = {0.0};                 //For finger data set
    bool bbSpaceCheck = 1;           //Will be true if a value is not needed from user 

    printf("Solar Cell Calculation Program\n");
    printf("The intended purpose of this program is to calculate the fractional loss of a solar cell sample with given information.\n\n");
    printf("This program was designed and created by Zakary Watts. \nThis work is licensed under a Creative Commons Attribution - NonCommercial 4.0 International License."); 
    printf("\n\n\n");

    printf("Please enter your cell current's denity at max power (A/cm^2) to continue... ");
    scanf_s("%lf", &inputJMP); printf("\n");
   
    char inputChar;                 //Input character
    printf("Cell max power voltage is set to %0.1lf by default. Do you want to change it? (y/n): ", inputVMP);
    
    do {                            //Remove whitespace from input stream to prevent error
        inputChar = getchar();
    } while (inputChar != EOF && inputChar != '\n'); 
    
    inputChar = fgetc(stdin);       //Character input
    
    inputChar = tolower(inputChar); //Standardized lowercase for comparision

    inputVMP = VMPBufferInput(inputChar, inputVMP);

    int cNumber;                    //Choice number

    printf("Enter a number to select and option:\n---------------------\n");

    printf("1) Input finger data first\n");
    printf("2) Input bus bar data first\n");

    do {
        scanf_s(" %d", &cNumber);
    } while (cNumber != 1 && cNumber != 2); //Only accept 1 or 2 from user

    if (cNumber == 1) {
       userFngr = FingerInput(userFngr, userBB, bbSpaceCheck);
       userBB.space = (userFngr.length * 2);  //Busbar spacing is 2x finger length so it's longer needed
        bbSpaceCheck = 0;
        printf("\n------------------\n");
       userBB = BusBarInput(userBB, userFngr, bbSpaceCheck);
    }
    if (cNumber == 2) {
        userBB = BusBarInput(userBB, userFngr, bbSpaceCheck);
        userFngr.length = (userBB.space / 2); //Alternative, finger length is 1/2 of busbar spacing so its input is not needed
        bbSpaceCheck = 0;
        printf("\n------------------\n");
        userFngr = FingerInput(userFngr, userBB, bbSpaceCheck);
    }

    double sLoss, eLoss, fLoss, bbLoss, tLoss; //For corresponding return values

    sLoss = ShadingLoss(userBB, userFngr);
    eLoss = EmitterLoss(userBB, userFngr, inputJMP, inputVMP);
    fLoss = FingerLoss(inputJMP, inputVMP, userFngr);
    bbLoss = BusBarLoss(inputJMP, inputVMP, userBB);

    tLoss = TotalLoss(sLoss, eLoss, fLoss, bbLoss);

    cNumber = 0;
    while (cNumber != 6) {
        cNumber = MenuSelect(cNumber);
         
        switch (cNumber) {
        case 1:
            printf("\nShading loss: %lf%%", sLoss);
            continue;
        case 2:
            printf("\nEmitter loss: %lf%%", eLoss);
            continue;
        case 3:
            printf("\nFinger loss: %lf%%", fLoss);
            continue;
        case 4:
            printf("\nBusbar loss: %lf%%", bbLoss);
            continue;
        case 5:
            printf("\nTotal loss: %lf%%", tLoss);
            continue;
        case 6:
            printf("Thank you, goodbye!");
            break;

        }

    }

    /*Possibly expand/ finish later*/
    FILE *celldata = "celldata.txt";                      //File that program will write calculation data too
    errno_t errt;
    
    errt = fopen_s(&celldata, "celldata.txt", "r");       //Working directory should eventually be implemented
    if (celldata != NULL) {                               //Will not execute with NULL return
        printf("\nSending data to a file...\n");

        fprintf(celldata, "Shade loss: %0.8lf\n", sLoss); //Writes output calculations to file
        fprintf(celldata, "Emitter loss: %0.8lf\n", eLoss);
        fprintf(celldata, "Finger loss: %0.8lf\n", fLoss);
        fprintf(celldata, "Busbar loss: 0.8%lf\n", bbLoss);
        fprintf(celldata, "Total loss: 0.8%lf\n", tLoss);
        
        fprintf(celldata, "\n------------\n");

        fprintf(celldata, "BUSBAR INPUTS\n");             //Writes original data inputs to file       
        fprintf(celldata, "Spacing: %0.4lf\n Width: %0.4lf\n Length: %0.4lf\n\n", userBB.space, userBB.width, userBB.length);

        fprintf(celldata, "FINGER INPUTS\n");
        fprintf(celldata, "Spacing: %0.4lf\n Width: %0.4lf\n Length: %0.4lf\n\n", userFngr.space, userFngr.width, userFngr.length);

        printf("\nAll data can be found in 'celldata.txt'\n");
        fclose(celldata);                                //Possibly expand by reading file data into python program to plot results                             
    }
}
