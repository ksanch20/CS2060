//Kylie Sanchez
//CS 2060 003
//This program is for renting out UCCS properties to vacationers
//It takes input from user for number of days, calculates charge,
//and displays rental information
//



#include <stdio.h>
#include <stdbool.h>

//This function prints the rental information to console
//Input: rental info
//Return: void
void printRentalPropertyInfo(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rentalRate, double discount){
    
    printf("Rental Property can be rented for %d and %d nights.\n", minNights, maxNights);
    printf("$%.2f rate a night for the first %d nights.\n", rentalRate,interval1Nights);
    printf("$%.2f discount rate a night for nights %d to %d\n",discount, (interval1Nights+1), interval2Nights);
    printf("$%.2f discount rate a night for each remaining night over %d\n\n", (discount*2),interval2Nights);
}//end function printRentalPropertyInfo



//This function gets input from user and checks that it is valid
//input: allowed range for input
//returns: valid user input
int getValidInt(int min, int max, int sentinel){
    
    int numNights=0; // number to be read from user
    bool validInput=false;

    do{
        
        puts("Enter the number of nights you want to rent the property");
        int scanned = scanf("%d", &numNights);
        
        if (scanned==1){
            if ((numNights>=min && numNights<=max)||numNights==sentinel){
                validInput =true;
            }//valid input
            
            else if(numNights<min||numNights>max) {
                puts("Error: Not within 1 and 14. Please enter the value again");
            }
        } //invalid int
        
        else  if (scanned==0){
            puts("Error: Not an integer number. Please enter the value again");
        }//invalid non-int input
        
    
  
        while(getchar()!='\n'){
        }//clear buffer
        
    }
    
    while(validInput==false);
    return numNights;
}// end function getValidInt


//This function calculates the charge
//input: rental info and number of nights
//returns: charge amount
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount){
    double charge=0;
    if (nights<=interval1Nights){
        charge = nights*rate;
    }//calculare charge within first interval of nights
    
    else if (nights>interval1Nights && nights<=interval2Nights){
        charge= (interval1Nights * rate)+((nights-interval1Nights)*(rate-discount));
    }//calculate charge within second interval of nights
    
    else if( nights >interval2Nights){
        charge=(interval1Nights*rate)+ (3*(rate-discount))+((nights-interval2Nights)*(rate-(2*discount)));
    }//calculate charge within third interval of nights
    
    return charge;
}//end function calculateCharge


//This function displays rental nights and charges to console
//Input: amount of nights and charges
//return: void
void printNightsCharges(unsigned int nights, double charges){
    if (charges==0){
        puts("\nRental Property Total Summary");
        puts("There were no rentals\n");
    }
    else{
        puts("\nRental Charges");
        puts("Nights      Charge");
        printf("%-11d $%.2f\n\n", nights, charges);
    }
}//end function printNightsCharges


//function main begins program execution
int main (void){
    
    int const SENTINAL_NEG1 = -1;
    unsigned int const MIN_RENTAL_NIGHTS=1;
    unsigned int const MAX_RENTAL_NIGHTS=14;
    unsigned int const INTERVAL_1_NIGHTS=3;
    unsigned int const INTERVAL_2_NIGHTS=6;
    double const RENTAL_RATE=400;
    double const DISCOUNT = 50;
    bool rentalMode=true;
    double totalCharges=0;
    int totalNights=0;
    
    while(rentalMode){
        printRentalPropertyInfo(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);
        
        int numNights=0;
        numNights = getValidInt(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, SENTINAL_NEG1);
        if (numNights==SENTINAL_NEG1){
            rentalMode=false;
        }
        else{
            int charge = calculateCharges(numNights, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);
            totalNights = totalNights+numNights;
            totalCharges=totalCharges+charge;
            
            printNightsCharges(numNights, charge);
        }//calculate and print rental information
        
    }//while loop keeps getting input from user until sentinal value is entered
    
    puts("\nRental Property Owner Total Summary");
    printNightsCharges(totalNights, totalCharges);//display rental summary
}//end Main


