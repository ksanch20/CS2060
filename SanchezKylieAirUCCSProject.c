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


//Define constants
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


//declare property structure
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
void printRentalPropertyInfo(struct property* propertyPtr, const int MIN_NIGHTS, const int MAX_NIGHTS, const int DISCOUNT_MULTIPLY);
int getValidIntSent(int min, int max, int sentinel, int length);
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);
void printNightsCharges(unsigned int nights, double charges, bool rentalSummary, int renters);
bool getIdAndPass (const char* correctPassword, const char* correctId,const int maxAttempts, int stringLength);
bool scanIntSent(const char* str, int* validIntPtr, int min, int max, int sentinel);
char* fgetsTrim(char* str);
void setUpProperty(struct property* propertyPtr,int minNights, int maxNights, int length, int sentinel, int minRate, int maxRate, int categories, int renters);
void getRenterRatings(struct property* propertyPtr, int max, int min, int categories, int rows,   const char *surveyCategories[RENTER_SURVEY_CATEGORIES], int length, int sentinel);
void rentalMode(struct property* propertyPtr, int sentinelValue, int minNights, int maxNights, int categories, int rows, int length, int minRating, int maxRating);
void printCategoryData(double categoryAverage[],size_t categories,const char *surveyCategories[]);
void printSurveyResults( size_t renters, size_t categories, struct property* propertyPtr);
void calculateCategoryAverage(struct property* propertyPtr, size_t renters, size_t categories);
bool scanInt(const char* str, int* validIntPtr, int min, int max);
int getValidInt(int min, int max, int length);


//function main begins program execution
int main (void){
    
    bool loginRenterSummary=false;
    bool correctLogin=false;
    
  //Get Id and Password from user
   correctLogin =  getIdAndPass(CORRECT_PASSWORD, CORRECT_ID, LOGIN_MAX_ATTEMPTS, LENGTH);
    
    //if login successful
    if(correctLogin==true){
        
        //allow user to set up property info
    Property property1;
    setUpProperty(&property1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, LENGTH, SENTINAL_NEG1,MIN_RATE,MAX_RATE,RENTER_SURVEY_CATEGORIES,VACATION_RENTERS);
        
        //rental mode begins
        rentalMode(&property1, SENTINAL_NEG1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, RENTER_SURVEY_CATEGORIES, VACATION_RENTERS, LENGTH, MIN_RATING,
                   MAX_RATING);
        
        //Get Id and Pass
        loginRenterSummary = getIdAndPass(CORRECT_PASSWORD, CORRECT_ID, LOGIN_MAX_ATTEMPTS, LENGTH);
        
        //if login successful
        if(loginRenterSummary==true){

            //calculate survey averages
            calculateCategoryAverage(&property1, VACATION_RENTERS, RENTER_SURVEY_CATEGORIES);
            puts("\nRental Property Report");
            printf("Name: %s\n", property1.name);
            printf("Location, %s\n", property1.location);
            
            //Print out rental summary
            //Print total nights, charges, and renters
            printNightsCharges(property1.totalNights, property1.totalCharge, loginRenterSummary, property1.totalRenters);
            puts("Category Rating Averages");
            
            //Print survey category averages
            printCategoryData(property1.categoryAverage, RENTER_SURVEY_CATEGORIES, property1.surveyCategories);
        }
       }
    
}//end Main


//This function prints the rental information to console
//Input: rental info
//Return: void
void printRentalPropertyInfo(struct property* propertyPtr, const int MIN_NIGHTS, const int MAX_NIGHTS, const int DISCOUNT_MULTIPLY){
    printf("\nRental Property: %s\n", propertyPtr->name);
    printf("Location: %s\n", propertyPtr->location);
    printf("Rental Property can be rented for %d and %d nights.\n", MIN_NIGHTS, MAX_NIGHTS);
    printf("$%d rate a night for the first %d nights.\n", propertyPtr-> rentalRate, propertyPtr->interval1Nights);
    printf("$%u discount rate a night for nights %d to %d\n",propertyPtr -> discount, (propertyPtr->interval1Nights+1), propertyPtr->interval2Nights);
    printf("$%u discount rate a night for each remaining night over %d\n\n", (propertyPtr->discount*DISCOUNT_MULTIPLY),propertyPtr->interval2Nights);
}//end function printRentalPropertyInfo


//This function gets input from user and checks that it is valid including sentinel value
//input: allowed range for input
//returns: valid user input
int getValidIntSent(int min, int max, int sentinel, int length){
    int validInt=0; // number to be read from user
    bool validInput=false;
    char userInput[length];
    
    do{
        fgets(userInput,length,stdin);
        fgetsTrim(userInput);
      validInput=  scanIntSent(userInput, &validInt, min, max,sentinel);
    }
    
    while(validInput==false);
    return validInt;
}// end function getValidIntSent


//This function gets input from user and checks that it is valid
//input: allowed range for input
//returns: valid user input
int getValidInt(int min, int max, int length){
    
    int validInt=0; // number to be read from user
    bool validInput=false;
    char userInput[length];
    
    do{
        fgets(userInput,length,stdin);
        fgetsTrim(userInput);
      validInput=  scanInt(userInput, &validInt, min, max);
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
void printNightsCharges(unsigned int nights, double charges, bool rentalSummary, int renters){
    if (charges==0){
        puts("\nRental Property Total Summary");
        puts("There were no rentals\n");
    }
    else if(rentalSummary==false){
        puts("\nRental Charges");
        puts("Nights      Charge");
        printf("%-11d $%.2f\n\n", nights, charges);
    }
    else if(rentalSummary==true){
        puts("\nRental Property Totals");
        puts("Renters   Nights      Charge");
        printf("%-11d %-11d $%.2f\n\n", renters, nights, charges);
    }
}//end function printNightsCharges


//this function checks if input is a valid int including sentinel value
//input: user input
//Returns: true or false if user input is valid
bool scanIntSent(const char* str, int* validIntPtr, int min, int max, int sentinel){
    char*end;
    errno=0;
    bool validIntFlag=false;
    long intTest=strtol(str, &end, 10);
    if (end == str) {
        puts("Error: not a number");
    }
    else if ('\0' != *end) {
        puts("Error: extra characters at end of input");
    }
    else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
        puts("Error: out of range for type");
    }
    else if (intTest > max) {
        puts("Error: greater than max");
    }
    else if ((intTest < min) && intTest!=sentinel) {
        puts("Error: less than min");
    }
    else if(intTest==sentinel){
        validIntFlag=true;
        *validIntPtr=(int)intTest;
    }
    else {
        
        validIntFlag=true;
        *validIntPtr=(int)intTest;
    }
    return validIntFlag;
}//end scanIntSent


//this function checks if input is a valid int
//input: user input
//Returns: true or false if user input is valid
bool scanInt(const char* str, int* validIntPtr, int min, int max){
    char*end;
    errno=0;
    bool validIntFlag=false;
    long intTest=strtol(str, &end, 10);
    if (end == str) {
        puts("Error: not a number");
    }
    else if ('\0' != *end) {
        puts("Error: extra characters at end of input");
    }
    else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
        puts("Error: out of range for type");
    }
    else if (intTest > max) {
        puts("Error: greater than max");
    }
    else if ((intTest < min) ) {
        puts("Error: less than min");
    }
    else {
        validIntFlag=true;
        *validIntPtr=(int)intTest;
    }
    return validIntFlag;
}//endScanInt


//This function removes newline from input
//input: string
//output: string with removed newline
char* fgetsTrim(char* str){
    if(str[strlen(str)-1]=='\n'){
        str[strlen(str)-1]='\0';
    }
    return str;
}


//This function gets id and password from user and checks if they are correct
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
}// end getIdAndPass


//This function allows owner to set up property info
void setUpProperty(struct property* propertyPtr,int minNights, int maxNights, int length, int sentinel, int minRate, int maxRate, int categories, int renters){
    puts("Enter the number of nights until first discount ");
    propertyPtr->interval1Nights = getValidInt(minNights, maxNights, length);
    
    puts("Enter the number of nights until second discount ");
    propertyPtr->interval2Nights=getValidInt(propertyPtr->interval1Nights+1, maxNights, length);
    
    
    puts("Enter nightly rental rate");
    propertyPtr->rentalRate=getValidInt(minRate, maxRate, length);
    
    
    puts("Enter the discount ");
    propertyPtr->discount= getValidInt(minRate, propertyPtr->rentalRate, length);
    
    
    puts("Enter rental property name ");
    fgets(propertyPtr->name, LENGTH, stdin);
    fgetsTrim(propertyPtr->name);
    
    puts("Enter rental property location ");
    fgets(propertyPtr->location, LENGTH, stdin);
    fgetsTrim(propertyPtr->location);
    
    propertyPtr-> surveyCategories[0] = "Check-in Process";
    propertyPtr-> surveyCategories[1] = "Cleanliness";
    propertyPtr-> surveyCategories[2] = "Amenities";
    

    propertyPtr->totalRenters=0;
    for(size_t i=0; i<renters;i++){
        for(size_t j=0; j<categories;j++){
            propertyPtr->ratings[i][j]=0;
        }
    }
}//end setUpProperty


//This function puts prpogram into rental mode until the sentinel value is entered
void rentalMode(struct property* propertyPtr, int sentinelValue, int minNights, int maxNights, int categories, int rows, int length, int minRating, 
    int maxRating){
    
    bool rentalMode=true;
    bool rentalSummary=false;
    int numNights=0;
    int cost=0;
    
    while(rentalMode==true){
        printRentalPropertyInfo(propertyPtr,MIN_RENTAL_NIGHTS,MAX_RENTAL_NIGHTS,DISCOUNT_MULTIPLIER );
        printSurveyResults(VACATION_RENTERS, RENTER_SURVEY_CATEGORIES, propertyPtr);
        puts("Enter number of nights you want to stay ");
       numNights= getValidIntSent(minNights, maxNights, sentinelValue, length);
        if(numNights!=sentinelValue){
            cost= calculateCharges(numNights, propertyPtr->interval1Nights, propertyPtr->interval2Nights, propertyPtr->rentalRate, propertyPtr->discount);
            printNightsCharges(numNights, cost, rentalSummary, propertyPtr->totalRenters);
            
            propertyPtr->totalCharge+=cost;
            propertyPtr->totalNights+=numNights;
            propertyPtr->totalRenters++;
            
            getRenterRatings(propertyPtr, maxRating, minRating, categories, rows, propertyPtr->surveyCategories, length, sentinelValue);
        }
        else rentalMode=false;
    }
}// end rentalMode


//This function gets ratings from renter
//input: renter enters rating for each category
//Output: Prompts user for rating and states when survey is full
void getRenterRatings(struct property* propertyPtr, int max, int min, int categories, int rows,   const char *surveyCategories[RENTER_SURVEY_CATEGORIES], int length, int sentinel ){
    int currentRow = propertyPtr-> totalRenters-1;
    if(currentRow<rows){
        for(size_t cat=0; cat< categories; cat++ ){
            printf("Enter rating for %s ", propertyPtr->surveyCategories[cat]);
            int rating = getValidInt(min, max, length);
            if(currentRow<rows&&cat<categories){
                propertyPtr->ratings[currentRow][cat]= rating;
            }
        }
    }
    else{
        puts("Error: survey full");
    }
}//end getRenterRatings


//this function calculates the average category rating
//input: number of renters and categories, rental survey and category average array
//return:void
void calculateCategoryAverage(struct property* propertyPtr, size_t renters, size_t categories){
    
    for(size_t j=0; j<categories; j++){
        double total=0;
        for(size_t i=0; i<renters; i++){
            total+=propertyPtr->ratings[i][j];
            
            propertyPtr-> categoryAverage[j]=total/propertyPtr->totalRenters;
        }
    }
}//end calculateCategoryAverage


//This function prints the rating categories for the customer
//input:category array and number of categories
//returns: void
//Output: prints categories
void printCategories(const char *categories[], size_t totalCategories)
{
    //loop to display each category horizontally
    printf("%s", "Rating Categories:\t");
    for (size_t surveyCategory = 0; surveyCategory < totalCategories; ++surveyCategory)
    {
        printf("\t%zu.%s\t", surveyCategory+1, categories[surveyCategory]);
    }
    puts(""); // start new line of output
}//end printCategories


//this function prints the average ratings
//input:number of categories, category average and survey category arrays
//return: void
//Output: prints survey averages
void printCategoryData(double categoryAverage[],size_t categories,const char *surveyCategories[]){
    printCategories(surveyCategories, categories);
    printf("Rating averages                ");
    for(size_t i=0; i<categories; i++){
        printf("%-20.1f ", categoryAverage[i]);
    }
    puts("\n");
}//end printCategoryData


//This function prints the survey results
//input: number of renters, number of categories, and rental survey //array
//return: void
//output: prints survey results
void printSurveyResults( size_t renters, size_t categories, struct property* propertyPtr){
    if(propertyPtr->ratings[0][0]==0){
        puts("Currently no ratings");
    }
    else{
        if(propertyPtr->totalRenters<renters){
            for(size_t i=0; i<propertyPtr->totalRenters; i++){
                printf("Survey %d: ", i+1);
                for(size_t j=0; j<categories; j++){
                    printf("%-3d",propertyPtr->ratings[i][j]);
                }
                puts("\n");
            }
        }
        else{
            for(size_t i=0; i<renters; i++){
                printf("Survey %d: ", i+1);
                for(size_t j=0; j<categories; j++){
                    printf("%-3d",propertyPtr->ratings[i][j]);
                }
                puts("\n");
            }
        }
    }
}// end printSurveyResults
