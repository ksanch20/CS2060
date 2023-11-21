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
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>


//Define constants
#define LENGTH 80
#define VACATION_RENTERS 5
#define RENTER_SURVEY_CATEGORIES 4
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
    char surveyCategories[RENTER_SURVEY_CATEGORIES][LENGTH];
    int ratings[VACATION_RENTERS][RENTER_SURVEY_CATEGORIES];
    double categoryAverage[RENTER_SURVEY_CATEGORIES];
    struct property* nextPropertyPtr;
}Property;


//function prototypes
void printRentalPropertyInfo(struct property* propertyPtr, const int MIN_NIGHTS, const int MAX_NIGHTS, const int DISCOUNT_MULTIPLY);
int getValidIntSent(int min, int max, int sentinel, int length);
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);
void printNightsCharges(unsigned int nights, double charges, bool rentalSummary, int renters);
bool getIdAndPass (const char* correctPassword, const char* correctId,unsigned int maxAttempts, int stringLength);
bool scanIntSent(const char* str, int* validIntPtr, int min, int max, int sentinel);
char* fgetsTrim(char* str);
void setUpProperty(struct property* propertyPtr,int minNights, int maxNights, int length, int minRate, int maxRate, int categories, int renters);
void getRenterRatings(struct property* propertyPtr, int max, int min, int categories, int rows,   char surveyCategories[RENTER_SURVEY_CATEGORIES][LENGTH], int length, int sentinel );
void rentalMode(struct property* propertyPtr, int sentinelValue, int minNights, int maxNights, int categories, int rows, int length, int minRating, int maxRating);
void printSurveyResults( size_t renters, size_t categories, struct property* propertyPtr);
void calculateCategoryAverage(struct property* propertyPtr, size_t renters, size_t categories);
bool scanInt(const char* str, int* validIntPtr, int min, int max);
int getValidInt(int min, int max, int length);
void printCategories(const char categories[], size_t totalCategories);
void printCategoryData(struct property* propertyPtr, size_t categories);
void displayRentalSummary(struct property* propertyPtr, bool loginRenterSummary, int renterCategories);


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
    setUpProperty(&property1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, LENGTH, MIN_RATE,MAX_RATE,RENTER_SURVEY_CATEGORIES,VACATION_RENTERS);
        
        while(loginRenterSummary==false){
            //rental mode begins
            rentalMode(&property1, SENTINAL_NEG1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, RENTER_SURVEY_CATEGORIES, VACATION_RENTERS, LENGTH, MIN_RATING,
                       MAX_RATING);
            
            //Get Id and Pass
            loginRenterSummary = getIdAndPass(CORRECT_PASSWORD, CORRECT_ID, LOGIN_MAX_ATTEMPTS, LENGTH);
        }
        
        //if login successful
        if(loginRenterSummary==true){

            //calculate survey averages
            calculateCategoryAverage(&property1, VACATION_RENTERS, RENTER_SURVEY_CATEGORIES);
            displayRentalSummary(&property1, loginRenterSummary, RENTER_SURVEY_CATEGORIES);
        }
       }
    
}//end Main


//This function gets id and password from user and checks if they are correct
bool getIdAndPass (const char* correctPassword, const char* correctId, unsigned int maxAttempts, int stringLength){
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
void setUpProperty(struct property* propertyPtr,int minNights, int maxNights, int length, int minRate, int maxRate, int categories, int renters){
    
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
    
    strcpy(propertyPtr->surveyCategories[0],"Check-in Process");
    strcpy(propertyPtr->surveyCategories[1],"Cleanliness");
    strcpy(propertyPtr->surveyCategories[2],"Amenitites");


    propertyPtr->totalRenters=0;
    propertyPtr->totalCharge=0;
    propertyPtr->totalNights=0;
    
    for(size_t i=0; i<renters;i++){
        for(size_t j=0; j<categories;j++){
            propertyPtr->ratings[i][j]=0;
        }
    }
    
    for(size_t j=0; j<categories;j++){
        propertyPtr->categoryAverage[j]=0;
    }
    
    
}//end setUpProperty


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


//This function gets ratings from renter
//input: renter enters rating for each category
//Output: Prompts user for rating and states when survey is full
void getRenterRatings(struct property* propertyPtr, int max, int min, int categories, int rows,   char surveyCategories[RENTER_SURVEY_CATEGORIES][LENGTH], int length, int sentinel ){
    int currentRow = propertyPtr-> totalRenters-1;
    puts("We want to know how your experience was renting our property. Using the rating system 1 to 5 enter your rating for each category:");
    for(size_t cat=0; cat< categories; cat++ ){
        printf("%d. %s\n",cat+1, propertyPtr->surveyCategories[cat]);
    }
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

//this function calculates the average category rating
//input: number of renters and categories, rental survey and category average array
//return:void
void calculateCategoryAverage(struct property* propertyPtr, size_t renters, size_t categories){
    
    for(size_t j=0; j<categories; j++){
        double total=0;
        for(size_t i=0; i<renters; i++){
            total+=propertyPtr->ratings[i][j];
            
        }
        propertyPtr-> categoryAverage[j]=total/propertyPtr->totalRenters;
    }
}//end calculateCategoryAverage



//this function prints the average ratings
//input:number of categories, category average and survey category arrays
//return: void
//Output: prints survey averages
void printCategoryData(struct property* propertyPtr, size_t categories){
    puts("Category Rating Averages");
    
    if(propertyPtr->totalRenters==0){
        puts("There are currently no ratings");
    }
    else{
        for(size_t cat=0; cat< categories; cat++ ){
            printf("%s: %-20.1f\n", propertyPtr->surveyCategories[cat], propertyPtr->categoryAverage[cat]);
        }
    }

}//end printCategoryData

//This function displays the total renters and charges and prints out the survey average results.
void displayRentalSummary(struct property* propertyPtr, bool loginRenterSummary, int renterCategories){
  
    puts("\nRental Property Report");
    printf("Name: %s\n", propertyPtr->name);
    printf("Location: %s\n", propertyPtr->location);
    
    //Print out rental summary
    //Print total nights, charges, and renters
    printNightsCharges(propertyPtr->totalNights, propertyPtr->totalCharge, loginRenterSummary, propertyPtr->totalRenters);
    
    //Print survey category averages
    printCategoryData(propertyPtr, renterCategories);

}

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

//this function checks if input is a valid int
//input: user input
//Returns: true or false if user input is valid
bool scanInt(const char* str, int* validIntPtr, int min, int max){
    char*end;
    errno=0;
    bool validIntFlag=false;
    long intTest=strtol(str, &end, 10);
    if (end == str) {
        puts("Error: not a number. Please try again.");
    }
    else if ('\0' != *end) {
        puts("Error: extra characters at end of input. Please try again.");
    }
    else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
        puts("Error: out of range for type. Please try again.");
    }
    else if (intTest > max) {
        puts("Error: greater than max. Please try again.");
    }
    else if ((intTest < min) ) {
        puts("Error: less than min. Please try again.");
    }
    else {
        validIntFlag=true;
        *validIntPtr=(int)intTest;
    }
    return validIntFlag;
}//endScanInt

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

//this function checks if input is a valid int including sentinel value
//input: user input
//Returns: true or false if user input is valid
bool scanIntSent(const char* str, int* validIntPtr, int min, int max, int sentinel){
    char*end;
    errno=0;
    bool validIntFlag=false;
    long intTest=strtol(str, &end, 10);
    if (end == str) {
        puts("Error: not a number. Please try again. ");
    }
    else if ('\0' != *end) {
        puts("Error: extra characters at end of input. Please try again. ");
    }
    else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
        puts("Error: out of range for type. Please try again.");
    }
    else if (intTest > max) {
        puts("Error: greater than max. Please try again.");
    }
    else if ((intTest < min) && intTest!=sentinel) {
        puts("Error: less than min. Please try again.");
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


//This function removes newline from input
//input: string
//output: string with removed newline
char* fgetsTrim(char* str){
    if(str[strlen(str)-1]=='\n'){
        str[strlen(str)-1]='\0';
    }
    return str;
}

//strcmp that ignores cases
//This function changes input to all lowercase before calling strcmp
//returns 0 if strings are equal, >0 if first string is greater than second, and <0 if second string is bigger than first
int strcmpIgnoreCase(const char* str1,const char* str2, int stringLength ){
    
    //changes str1 to all lowercase temporarily
    char tempString[stringLength];
    size_t str1Length = strlen(str1);
    for(size_t i=0; i< str1Length; i++){
        tempString[i]=tolower(str1[i]);
    }
    //changes str2 to all lowercase temporarily
    char tempString2[stringLength];
    size_t str2Length = strlen(str2);
    for(size_t i=0; i<str2Length; i++){
        tempString2[i]=tolower(str2[i]);
    }
    //return result of strcmp on the two temporary, lowercase strings
    return strcmp(tempString, tempString2);
}//end strcmpIgnoreCase

//validate yes or no from user
char validateYesNo(void) {
    char validYesNo;
    
    do {
        puts("Please enter (y)es or (n)o:");
        validYesNo = getchar();
        while (getchar() != '\n');
        
        validYesNo = tolower(validYesNo);
        
    } while (validYesNo != 'y' && validYesNo != 'n');
    
    return  validYesNo;
} //End validateYesNo



//This function adds property to linked list
void insertProperty(Property** headPtr, int stringLength, int minNights, int maxNights, int length, int minRate, int maxRate, int categories, int rental){
    //allocates memory for new property and returns an address
    Property* newPropertyPtr = malloc(sizeof(Property));
    setUpProperty(newPropertyPtr, minNights, maxNights, length, minRate, maxRate, categories, rental);
    
    //if memory was allocated
    if(newPropertyPtr!=NULL){
    
        //set previouse pointer to null and currentPtr to head
        Property* previousPtr=NULL;
        Property* currentPtr= *headPtr;
        
        //iterate through linked list until match is found or currentPtr is null
        while(currentPtr!=NULL && strcmpIgnoreCase(currentPtr->name, newPropertyPtr->name, stringLength)<0){
            previousPtr=currentPtr;
            currentPtr=currentPtr->nextPropertyPtr;
        }
        //if list is empty set head to new pointer
        if(previousPtr==NULL){
            *headPtr=newPropertyPtr;
        }
        //set previous pointer next to new pointer
        else{
            previousPtr->nextPropertyPtr= newPropertyPtr;
        }
        //set new property next to current pointer
        newPropertyPtr-> nextPropertyPtr=currentPtr;
    }
    //if memory was not allocated print message stating so
    else{
        puts("No memory to add property");
    }
}//end insertProperty
