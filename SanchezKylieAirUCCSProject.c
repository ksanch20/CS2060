//
//  SanchezKylieAirUCCSProject.c
//  BookExample
//
//  Created by Kylie Sanchez on 9/15/23.
//

#include <stdio.h>
#include <stdbool.h>



void printRentalPropertyInfor(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rentalRate, double discount){
    
    printf("Rental Property can be rented for %d and %d nights.\n", minNights, maxNights);
    printf("$%.2f rate a night for the first %d nights.\n", rentalRate,interval1Nights);
    printf("$%.2f discount rate a night for nights %d to %d\n",discount, (interval1Nights+1), interval2Nights);
    printf("$%.2f discount rate a night for each remaining night over %d\n", (discount*2),interval2Nights);
    
}

int getValidInt(int min, int max, int sentinel){
    int numNights=0;
    bool validInput=false;
    
  
   
    do{
        
        puts("Enter the number of nights you want to rent the property");
        int scanned = scanf("%d", &numNights);
        if (scanned==1){
            if ((numNights>=min && numNights<=max)||numNights==sentinel){
                validInput =true;
            }
            else if(numNights<min||numNights>max) {
                puts("Error: Not within 1 and 14. Please enter the value again");
    
            }
            
        }
        
        else  if (scanned==0){
              puts("Error: Not an integer number. Please enter the value again");
    
          }
        
      
        while(getchar()!='\n'){
          
        }
    }
   
    
    while(validInput==false);
    return numNights;
}
    
    
    double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount){
        double charge=0;
        if (nights<=interval1Nights){
            charge = nights*rate;
        }
        
        else if (nights>interval1Nights && nights<=interval2Nights){
            charge= (interval1Nights * rate)+((nights-interval1Nights)*(rate-discount));
        }
        
        else if( nights >interval2Nights){
            charge=(interval1Nights*rate)+ (3*(rate-discount))+((nights-interval2Nights)*(rate-(2*discount)));
        }
        return charge;
    }
    
    void printNightsCharges(unsigned int nights, double charges){
        if (charges==0){
            puts("There were no rentals\n");
        }
        else{
            puts("Nights      Charge");
            printf("%d    $%.2f\n", nights, charges);
        }
    }
    
    
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
            printRentalPropertyInfor(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);
            
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
                
            }
        }
        
        printNightsCharges(totalNights, totalCharges);
        
    }
    

