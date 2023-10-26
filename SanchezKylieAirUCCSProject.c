//Kylie Sanchez
//CS 2060 003
//This program is for renting out UCCS properties to vacationers
//It takes input from user for number of days, calculates charge,
//and displays rental information
//macOS
//

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include<stdlib.h>
#include <limits.h>

#define LENGTH 80
#define VACATION_RENTERS 5
#define RENTER_SURVEY_CATEGORIES 3
#define CORRECT_ID "id1"
#define CORRECT_PASSWORD "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1
#define MIN_RENTAL_NIGHTS 1
#define MAX_RENTAL_NIGHTS 14
#define MIN_RATE 1
#define MAX_RATE 1000
#define MIN_RATING 1
#define MAX_RATING 5
#define DISCOUNT_MULTIPLIER 2

typedef struct  property {
    char name[LENGTH];
    char location[LENGTH];
    unsigned int totalNights;
    unsigned int totalCharge;
    unsigned int totalRenters;
    unsigned int interval1Nights;
    unsigned int interval2Nights;
    unsigned int rentalRate;
    unsigned int discount;
    const char *surveyCategories[RENTER_SURVEY_CATEGORIES];
    int ratings[VACATION_RENTERS][RENTER_SURVEY_CATEGORIES];
    double *categoryAverage;

}Property;



//function prototypes
void printRentalPropertyInfo(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rentalRate, double discount);
int getValidInt(int min, int max, int sentinel, int length);
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);
void printNightsCharges(unsigned int nights, double charges);
bool getIdAndPass (const char* correctPassword, const char* correctId,const int maxAttempts, int stringLength);
bool scanInt(const char* str, int *validIntPtr);
char* fgetsTrim(char* str);
void setUpProperty(struct property* propertyPtr,int minNights, int maxNights, int length, int sentinel,int minRate, int maxRate);
void getRenterRatings(struct property* propertyPtr, int max, int min, int categories, int rows,   const char *surveyCategories[RENTER_SURVEY_CATEGORIES], int length, int sentinel);
void rentalMode(struct property* propertyPtr, int sentinelValue, int minNights, int maxNights, int categories, int rows, int length, int minRating, int maxRating);
void printCategoryData(double categoryAverage[],size_t categories,const char *surveyCategories[]);



//function main begins program execution
int main (void){
    
    
  bool correctLogin =  getIdAndPass(CORRECT_PASSWORD, CORRECT_ID, LOGIN_MAX_ATTEMPTS, LENGTH);
    
    if(correctLogin==true){
    Property property1;
    setUpProperty(&property1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, LENGTH, SENTINAL_NEG1,MIN_RATE,MAX_RATE);
        rentalMode(&property1, SENTINAL_NEG1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, RENTER_SURVEY_CATEGORIES, VACATION_RENTERS, LENGTH, MIN_RATING, MAX_RATING);
        bool loginRenterSummary = getIdAndPass(CORRECT_PASSWORD, CORRECT_ID, LOGIN_MAX_ATTEMPTS, LENGTH);
        if(loginRenterSummary==true){
            puts("Rental Property Report");
            printf("Name: %s", property1.name);
            printf("Location, %s", property1.location);
            puts("Rental Property Totals");
            printNightsCharges(property1.totalNights, property1.totalCharge);
            puts("Category Rating Averages");
            printCategoryData(property1.categoryAverage, RENTER_SURVEY_CATEGORIES, property1.surveyCategories);
        }
       }
    
}//end Main


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
int getValidInt(int min, int max, int sentinel, int length){
    
  unsigned int validInt=0; // number to be read from user
    bool validInput=false;
    char userInput[length];
    
    
    do{
        
        fgets(userInput,length,stdin);
        fgetsTrim(userInput);
        
     
        
    }
    
    while(validInput==false);
    return validInt;
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
        charge=(interval1Nights*rate)+ ((interval2Nights-interval1Nights))*((rate-discount))+((nights-interval2Nights)*(rate-(2*discount)));
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

bool scanInt(const char* str, int* validIntPtr){
    char*end;
    errno=0;
    bool validIntFlag=false;
    long intTest=strtol(str, &end, 10);
    if (end == str) {
        puts("Error: not an number");
    }
    else if ('\0' != *end) {
        puts("Error: extra characters at end of input");
    }
    else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
        puts("Error: out of range for type");
    }
    else if (intTest > INT_MAX) {
        puts("Error: greater than max");
    }
    else if (intTest < INT_MIN) {
        puts("Error: less than min");
    }
    else {
        
        validIntFlag=true;
    }
    return validIntFlag;
}

char* fgetsTrim(char* str){
    if(str[strlen(str)-1]=='\n'){
        str[strlen(str)-1]='\0';
    }
    return str;
}


bool getIdAndPass (const char* correctPassword, const char* correctId, const int maxAttempts, int stringLength){
    bool validLogin=false;
    int numAttempts=0;
    char userPass[stringLength];
    char userId[stringLength];
    while(numAttempts<maxAttempts&&validLogin==false){
        puts("Enter your airUCCS id ");
        fgets(userId, stringLength,stdin);
        fgetsTrim(userId);
        
        puts("Enter your airUCCS password ");
        fgets(userPass, stringLength,stdin);
        fgetsTrim(userPass);
        
        if((strcmp(userId,correctId)==0)&&(strcmp(userPass,correctPassword)==0)){
            puts("Login successful");
            validLogin=true;
        }
        else{
            puts("Error: incorrect id or password");
        }
        numAttempts++;
      
    }
    if(validLogin==false){
        puts("Error: you've exceeded max attempts. Try again later.");
    }
    return validLogin;
}



void setUpProperty(struct property* propertyPtr,int minNights, int maxNights, int length, int sentinel, int minRate, int maxRate){
    puts("Enter the number of nights until first discount ");
    propertyPtr->interval1Nights = getValidInt(minNights, maxNights, sentinel, length);
    
    puts("Enter the number of nights until second discount ");
    propertyPtr->interval2Nights=getValidInt(propertyPtr->interval1Nights+1, maxNights, sentinel, length);

    
    puts("Enter nightly rental rate");
propertyPtr->rentalRate=getValidInt(minRate, maxRate, sentinel, length);

    
    puts("Enter the discount ");
 propertyPtr->discount= getValidInt(minRate, propertyPtr->rentalRate, sentinel, length);

    
    puts("Enter rental property name ");
    fgets(propertyPtr->name, LENGTH, stdin);
    fgetsTrim(propertyPtr->name);

    puts("Enter rental property location ");
    fgets(propertyPtr->location, LENGTH, stdin);
    fgetsTrim(propertyPtr->location);

        propertyPtr-> surveyCategories[0] = "Check-in Process";
        propertyPtr-> surveyCategories[1] = "Cleanliness";
        propertyPtr-> surveyCategories[2] = "Amenities";
    
    
}

void rentalMode(struct property* propertyPtr, int sentinelValue, int minNights, int maxNights, int categories, int rows, int length, int minRating, int maxRating){
    bool rentalMode=true;
    int numNights=0;
    int cost=0;
    while(rentalMode==true){
        printRentalPropertyInfo(minNights, maxNights, propertyPtr->interval1Nights, propertyPtr->interval2Nights, propertyPtr->rentalRate, propertyPtr->discount);
        puts("Enter number of nights you want to stay ");
       numNights= getValidInt(minNights, maxNights, sentinelValue, length);
        if(numNights!=sentinelValue){
            cost= calculateCharges(numNights, propertyPtr->interval1Nights, propertyPtr->interval2Nights, propertyPtr->rentalRate, propertyPtr->discount);
            printNightsCharges(numNights, cost);
            getRenterRatings(propertyPtr, maxRating, minRating, categories, rows, propertyPtr->surveyCategories, length, sentinelValue);
        
            propertyPtr->totalCharge+=cost;
            propertyPtr->totalNights+=numNights;
            propertyPtr->totalRenters++;
        }
        else rentalMode=false;
    }
}

void getRenterRatings(struct property* propertyPtr, int max, int min, int categories, int rows,   const char *surveyCategories[RENTER_SURVEY_CATEGORIES], int length, int sentinel ){
    int currentRow = propertyPtr-> totalRenters;
    if(currentRow<rows){
        for(size_t cat=0; cat< categories; cat++ ){
            printf("Enter rating for %s", propertyPtr->surveyCategories[cat]);
            int rating = getValidInt(min, max, sentinel, length);
            propertyPtr->ratings[currentRow][cat]= rating;
        }
    }
    else{
        puts("Error: survey full");
    }
}



//this function calculates the average category rating
//input: number of renters and categories, rental survey and category average array
//return:void
void calculateCategoryAverage(struct property* propertyPtr, size_t renters, size_t categories, int rentalSurvey[renters][categories], double categoryAverage[]){
    
    for(int j=0; j<categories; j++){
        double total=0;
        for(int i=0; i<renters; i++){
            total+=rentalSurvey[i][j];
            
            categoryAverage[j]=total/propertyPtr->totalRenters;
        }
    }
}


//This function prints the rating categories for the customer
//input:category array and number of categories
//returns: void
void printCategories(const char *categories[], size_t totalCategories)
{
    //loop to display each category horizontally
    printf("%s", "Rating Categories:\t");
    for (size_t surveyCategory = 0; surveyCategory < totalCategories; ++surveyCategory)
    {
        printf("\t%zu.%s\t", surveyCategory+1, categories[surveyCategory]);
    }
    puts(""); // start new line of output
}

//this function prints the average ratings
//input:number of categories, category average and survey category arrays
//return: void
void printCategoryData(double categoryAverage[],size_t categories,const char *surveyCategories[]){
    printCategories(surveyCategories, categories);
    printf("Rating averages                ");
    for(int i=0; i<categories; i++){
        printf("%-20.1f ", categoryAverage[i]);
    }
    puts("\n");
}
