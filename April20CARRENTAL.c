// ORIGINAL FILE WITH NO ENCRYPTION AND DECRYPTION
#include <stdio.h>
#include <string.h>
#define MAX_USERS 30
#define CATEGORIES 5
#define SLOTS 10

// STRUCTURES
typedef struct {
    int carID; 
    char model[30]; 
    char type[15];
    char fuelType[15]; 
    float mileage; 
    float emissionFactor; 
    char color[15];
    float ratePerDay;
    int isRented;
	char rentedBy[30];	
	
} Car;

typedef struct {
    char name[30];
    char password[30];      
} User;

typedef struct {
    char customerName[30];
    int carID;
    char model[30];
    int days;
    float totalBill;
    float totalCO2;
    char status[15];
    char rentDate[15];
	char returnDate[15];

} RentalRecord;

// FUNCTION PROTOYPES
void registerUser (User users[], int *userCount);
void loginUser(User users[], int userCount, Car garage[CATEGORIES][SLOTS]);
void displayAllCars(Car garage[CATEGORIES][SLOTS]);
void initializeCars(Car garage[CATEGORIES][SLOTS]);
void SortByPrice (Car garage[CATEGORIES][SLOTS]);
void SortByCarbonEmissions (Car garage[CATEGORIES][SLOTS]);
void SortByModel(Car garage[CATEGORIES][SLOTS]);
void rentCar(Car garage[CATEGORIES][SLOTS], char username[]);
void printTable(Car list[], int count);
void ShowRentalRecords (char username[]);
void saveAllCars(Car garage[CATEGORIES][SLOTS]);
void adminSide(Car garage[CATEGORIES][SLOTS]);
void addCar(Car garage[CATEGORIES][SLOTS]);
void deleteCar(Car garage[CATEGORIES][SLOTS]);
void returnCar(Car garage[CATEGORIES][SLOTS], char username[]);
void loadAllCars(Car garage[CATEGORIES][SLOTS]);
int encryptFile(const char inputFile[], const char outputFile[], const char key[]);
int decryptFile(const char inputFile[], const char outputFile[], const char key[]);
int isDataEncrypted();
void setDataEncrypted(int state);
void encryptAllFiles();
void decryptAllFiles();
void showAllRentalHistoryAndIncome();
void showIncomePerCar();
void showMostRentedCar();
void generateReceipt();
void editCar(Car garage[CATEGORIES][SLOTS]);

int main() {
	
    // ===== DECLARATIONS =====
    User users[MAX_USERS];
    int choice;
    char extraChar;
    int userCount = 0;
    int exitProgram = 0;
    FILE *fp;

    int confirm;
    char extraChar2;
    int validConfirm;

	if (isDataEncrypted() == 1) { // If data is currently encrypted, decrypt all files
    	decryptAllFiles();
    	setDataEncrypted(0); // Update state to "not encrypted"
	}
	FILE *check;

check = fopen("all_cars.txt", "r");
if (check == NULL) {
    check = fopen("all_cars.txt", "w");
}
if (check != NULL) fclose(check);

check = fopen("users.txt", "a");
if (check != NULL) fclose(check);

check = fopen("rental_records.txt", "a");
if (check != NULL) fclose(check);

check = fopen("crypto_state.txt", "r");
if (check == NULL) {
    setDataEncrypted(0);
} else {
    fclose(check);
}

	

// Initialize ONLY if empty



// Load rental status
Car garage[CATEGORIES][SLOTS] = {0};

// ? ALWAYS initialize FIRST
initializeCars(garage);

// ? THEN overwrite with file data if valid
loadAllCars(garage);


	
	

    fp = fopen("users.txt", "r");
    if (fp != NULL) {
        while (userCount < MAX_USERS && fscanf(fp, " %[^,],%[^\n]\n", users[userCount].name, users[userCount].password) == 2) { 
            userCount++; 
        }
        fclose(fp);
    }

    do {
        printf("\n==================================\n");
        printf("    CAR RENTAL SYSTEM - MAIN\n");
        printf("==================================\n");
        printf("1. Sign Up\n");
        printf("2. Log In\n");
        printf("3. Exit\n");
        printf("Choose: ");

        // Input validation
        if (scanf("%d%c", &choice, &extraChar) != 2 || extraChar != '\n') {
		    printf("Invalid input! Please enter a number.\n");
		    while (getchar() != '\n');
		    choice = 0;
		}
        
        if (choice == 1) {
            if (userCount >= MAX_USERS) {
                printf("Account limit reached!\n");
            } 
            else {
                registerUser(users, &userCount);
            }
        }

        else if (choice == 2) {
            loginUser(users, userCount, garage); 
        }

        else if (choice == 3) {
            
            validConfirm = 0;

            while (validConfirm == 0) {
                printf("Are you sure you want to exit? (1 = Yes, 2 = No): ");

                if (scanf("%d%c", &confirm, &extraChar2) != 2 || extraChar2 != '\n') {
                    printf("Invalid input!\n");
                    while (getchar() != '\n');
                }
                else if (confirm == 1) {
                    printf("Exiting the program. Goodbye!\n");
                    exitProgram = 1; // allow loop to end
                    validConfirm = 1;
                }
                else if (confirm == 2) {
                    printf("Returning to main menu...\n");
                    validConfirm = 1;
                }
                else {
                    printf("Invalid choice. Try again.\n");
                }
            }
        }

        else {
            printf("Invalid Input!\n");
        }

    } while (exitProgram == 0);
    
     //Encrypt all files before exiting the program
    if (isDataEncrypted() == 0) {
	    encryptAllFiles();
	    setDataEncrypted(1);
	}
    
    return 0;
}

/* 
	<Description of function>
	Initializes the car garage with default values and preset vehicles.
	
	Precondition: 
	The garage array has been properly declared with defined CATEGORIES and SLOTS.
	
	@param Car garage[CATEGORIES][SLOTS]: The 2D array representing the car structure
	
	@return (void)
*/
void initializeCars(Car garage[CATEGORIES][SLOTS]) {

    int i, j;

    // ? SAFE CHECK: if already initialized, skip
    if (garage[0][0].carID == 101) {
        return;
    }

    // ? Clear memory (removes garbage values)
    for (i = 0; i < CATEGORIES; i++) {
        for (j = 0; j < SLOTS; j++) {
            garage[i][j] = (Car){0};
        }
    }

    printf("Initializing default cars...\n");

    // ===== SEDAN =====
    garage[0][0] = (Car){101, "Toyota Vios", "Sedan", "Gasoline", 18.5, 130.0, "Red", 3500, 0, ""};
    garage[0][1] = (Car){102, "Honda Civic", "Sedan", "Gasoline", 17.2, 189.0, "White", 3700, 0, ""};
    garage[0][2] = (Car){103, "BYD Seal 5", "Sedan", "Hybrid", 20.2, 62.0, "Gray", 3800, 0, ""};

    // ===== SUV =====
    garage[1][0] = (Car){201, "Toyota Fortuner", "SUV", "Diesel", 10.5, 209.0, "Black", 4200, 0, ""};
    garage[1][1] = (Car){202, "Mitsubishi Montero", "SUV", "Diesel", 11.0, 250.0, "Silver", 4100, 0, ""};
    garage[1][2] = (Car){203, "Ford Everest", "SUV", "Diesel", 16.0, 187.0, "White", 4500, 0, ""};

    // ===== PICKUP =====
    garage[2][0] = (Car){301, "Toyota Hilux", "PICKUP", "Diesel", 12.0, 210.0, "Black", 4000, 0, ""};
    garage[2][1] = (Car){302, "Ford Ranger", "PICKUP", "Diesel", 11.5, 220.0, "Blue", 4200, 0, ""};
    garage[2][2] = (Car){303, "Isuzu D-Max", "PICKUP", "Diesel", 13.0, 205.0, "White", 3900, 0, ""};

    // ===== CROSSOVER =====
    garage[3][0] = (Car){401, "Toyota Corolla Cross", "CROSSOVER", "Hybrid", 21.0, 90.0, "Gray", 3600, 0, ""};
    garage[3][1] = (Car){402, "Honda HR-V", "CROSSOVER", "Gasoline", 18.0, 140.0, "Black", 3500, 0, ""};
    garage[3][2] = (Car){403, "Nissan Kicks", "CROSSOVER", "Gasoline", 19.0, 120.0, "Red", 3400, 0, ""};

    // ===== ELECTRIC =====
    garage[4][0] = (Car){501, "Tesla Model 3", "ELECTRIC", "Electric", 25.0, 0.0, "White", 5000, 0, ""};
    garage[4][1] = (Car){502, "Nissan Leaf", "ELECTRIC", "Electric", 23.0, 0.0, "Green", 4800, 0, ""};
    garage[4][2] = (Car){503, "BYD Dolphin", "ELECTRIC", "Electric", 24.0, 0.0, "Blue", 4700, 0, ""};

    printf("15 cars initialized successfully!\n");
}
    
	

void registerUser(User users[], int *userCount) {
	
	char tempName[31];
	char tempPass[31];
	char extraChar;
	int proceed = 0;
	int isValid;
	int i;
	int duplicate;
	int cancelSignup = 0;
	FILE *fp;
	
	while (proceed == 0) {
		
		if (*userCount >= MAX_USERS) {
			printf("Maximum number of users reached.\n");
			proceed = 1;
		}
		else {
			
			// Get a valid and unique username
			isValid = 0;
			while (isValid == 0 && cancelSignup == 0) {
				
				printf("Enter username (Max of 30 characters, or 0 to cancel): ");
				
				if (scanf("%30[^\n]%c", tempName, &extraChar) != 2 || extraChar != '\n') {
					printf("Invalid input. Try again.\n");
					while (getchar() != '\n');
				}
				else if (strcmp(tempName, "0") == 0) {
					printf("Sign up cancelled. Returning to menu...\n");
					cancelSignup = 1;
				}
				else if (strlen(tempName) < 5) {
					printf("Error. Username must be atleast 5 characters.\n");
				}
				else {
					duplicate = 0;
					
					for (i = 0; i < *userCount; i++) {
						if (strcasecmp(users[i].name, tempName) == 0) {
							duplicate = 1;
						}
					}
					
					if (duplicate == 1) {
						printf("Error. Username already exists.\n");
					}
					else {
						isValid = 1;
					}
				}
			}
			
			// Get a valid password only if sign up was not cancelled
			if (cancelSignup == 0) {
				isValid = 0;
				while (isValid == 0 && cancelSignup == 0) {
					printf("Enter password (Max of 30 characters, or 0 to cancel): ");
					
					if (scanf("%30[^\n]%c", tempPass, &extraChar) != 2 || extraChar != '\n') {
						printf("Invalid input. Try again.\n");
						while (getchar() != '\n');
					}
					else if (strcmp(tempPass, "0") == 0) {
						printf("Sign up cancelled. Returning to menu...\n");
						cancelSignup = 1;
					}
					else if (strlen(tempPass) < 5) {
						printf("Error. Password must be atleast 5 characters.\n");
					}
					else {
						isValid = 1;
					}
				}
			}
			
			// Save only if not cancelled
			if (cancelSignup == 0) {
				strcpy(users[*userCount].name, tempName);
				strcpy(users[*userCount].password, tempPass);
				
				fp = fopen("users.txt", "a");
				if (fp != NULL) {
					fprintf(fp, "%s,%s\n", users[*userCount].name, users[*userCount].password);
			        fclose(fp);
			        (*userCount)++;
			        printf("Registration successful!\n");
				}
				else {
					printf("Error saving the user.\n");
				}
			}
			
			proceed = 1;
		}
	}
}
/* 
	<Description of function>
	Facilitates the user login and directs the user to either admin or user dashboard
	
	Precondition: 
	
	
	@param User users[]: The array where the new user data will be stored
	@param int userCount : A pointer to the total number of users 
	@param Car garage[CATEGORIES][SLOTS] : The car storage system holding details about all cars
	
	@return (void)
*/
void loginUser(User users[], int userCount, Car garage[CATEGORIES][SLOTS]) {
	
    char inputName[30], inputPass[30];
    char extraChar;
    int found = 0, userIndex = 0, subChoice = 0;
    int validInput = 1;
    int cancelLogin = 0;
    
    if (userCount == 0) {
    	printf("No users registered yet.\n");
	}
	else {
	    printf("\n===== LOGIN =====\n");
	    
	    // Username input
	    validInput = 0;
	    while (validInput == 0 && cancelLogin == 0) {
	    	printf("Username (or 0 to cancel): ");
	    	
	    	if (scanf("%29[^\n]%c", inputName, &extraChar) != 2 || extraChar != '\n') {
			    printf("Invalid username input!\n");
			    while (getchar() != '\n');
			}
			else if (strcmp(inputName, "0") == 0) {
				printf("Login cancelled. Returning to menu...\n");
				cancelLogin = 1;
			}
			else if (strlen(inputName) == 0) {
				printf("Username cannot be empty.\n");
			}
			else {
				validInput = 1;
			}
		}
		
		// Password input
		if (cancelLogin == 0) {
			validInput = 0;
			while (validInput == 0 && cancelLogin == 0) {
				printf("Password (or 0 to cancel): ");
				
				if (scanf("%29[^\n]%c", inputPass, &extraChar) != 2 || extraChar != '\n') {
			        printf("Invalid password input!\n");
			        while (getchar() != '\n');
	    		}
	    		else if (strcmp(inputPass, "0") == 0) {
	    			printf("Login cancelled. Returning to menu...\n");
	    			cancelLogin = 1;
				}
				else if (strlen(inputPass) == 0) {
					printf("Password cannot be empty.\n");
				}
				else {
					validInput = 1;
				}
			}
		}
		
		// Check credentials only if not cancelled
		if (cancelLogin == 0) {
			found = 0;
			userIndex = 0;
			
		    while (userIndex < userCount && found == 0) {
		        if (strcmp(users[userIndex].name, inputName) == 0 &&
		            strcmp(users[userIndex].password, inputPass) == 0) {
		            found = 1;
		        } 
				else {
		            userIndex++;
		        }   
		    }
		    
		    if (found == 1) {
		    	if (strcmp(inputName, "admin") == 0 && strcmp(inputPass, "admin123") == 0) {
		    		adminSide(garage);	
		    	}
		    	else {
			        printf("\nWELCOME, %s!\n", users[userIndex].name);
			        subChoice = 0;
			        
			        while (subChoice != 5) {
			            printf("\n--- USER DASHBOARD ---\n");
			            printf("1. RENT A CAR\n");
			            printf("2. DISPLAY ALL CARS\n");
			            printf("3. DISPLAY ALL RECORDS\n");
			            printf("4. RETURN CAR\n");
			            printf("5. LOG OUT\n");
			            printf("Choose: ");
			            
			            if (scanf("%d%c", &subChoice, &extraChar) != 2 || extraChar != '\n') {
						    printf("Invalid input!\n");
						    while (getchar() != '\n');
		    				subChoice = 0;
						}
						else if (subChoice == 1) {
			                rentCar(garage, inputName);
			            }
			            else if (subChoice == 2)  {
			                displayAllCars(garage); 
			            }
			            else if (subChoice == 3) {
			            	ShowRentalRecords(inputName);
						}
						else if (subChoice == 4) {
							returnCar(garage, inputName);
						}
			            else if (subChoice == 5)  {
			                printf("Logging out...\n"); 
			            }
			            else {
			            	printf("Invalid input!\n");
						}
			        }
		    	}
			}
			else {
		        printf("\nLogin Failed!\n");
		    }
		}
	}
}

/* 
	<Description of function>
	Displays all cars with options to sort by price, carbon emissions, category, or model
	
	Precondition: 
	The garage array contains initialized or loaded car data
	
	@param Car garage[CATEGORIES][SLOTS] : The car storage system holding details about all cars
	
	@return (void)
*/

void displayAllCars(Car garage[CATEGORIES][SLOTS]) {
    
    char *categoryHeaders[] = {"SEDAN", "SUV", "PICKUP", "CROSSOVER", "ELECTRIC"};
    int choice;
    int i, j;
    char extraChar;
    int isValid = 0;
    
    // Menu input validation
    while (isValid == 0) {
        printf("\nHOW WOULD YOU LIKE TO VIEW THE CARS?\n");
        printf("1. View by Category (Default)\n");
        printf("2. Sort by Carbon Emissions (Lowest to Highest)\n");
        printf("3. Sort by Price (Lowest to Highest)\n");
        printf("4. Sort by Model\n");
        printf("Choose: ");

        if (scanf("%d%c", &choice, &extraChar) != 2 || extraChar != '\n') {
            printf("Invalid input.\n");
            while (getchar() != '\n');
        }
        else if (choice < 1 || choice > 4) {
            printf("Invalid input.\n");
        }
        else {
            isValid = 1;
        }
    }

    // Sorting options
    if (choice == 2) {
        SortByCarbonEmissions(garage);
        return;
    }
    else if (choice == 3) {
        SortByPrice(garage);
        return;
    }
    else if (choice == 4) {
        SortByModel(garage);
        return;
    }

    // ===== DEFAULT VIEW =====
    printf("\n%-110s\n", "------------------------------------------------------------------------------------------------------------------------");
    printf("%-5s | %-20s | %-10s | %-10s | %-8s | %-10s | %-10s | %-10s | %-12s\n", 
           "ID", "MODEL", "TYPE", "FUEL", "MILEAGE", "EMISSION", "COLOR", "RATE/DAY", "AVAILABILITY");
    printf("%-110s\n", "------------------------------------------------------------------------------------------------------------------------");

    for (i = 0; i < CATEGORIES; i++) {
        printf(">>> %s\n", categoryHeaders[i]);

        int hasCarInCategory = 0;

        for (j = 0; j < SLOTS; j++) {

            // ? ONLY SHOW VALID CARS (prevents missing + garbage issues)
            if (garage[i][j].carID >= 100 && garage[i][j].carID <= 599) {

                hasCarInCategory = 1;

                printf("%-5d | %-20s | %-10s | %-10s | %-8.1f | %-6.1fg/km | %-10s | P%-9.2f | %-12s\n",
                       garage[i][j].carID,
                       garage[i][j].model,
                       garage[i][j].type,
                       garage[i][j].fuelType,
                       garage[i][j].mileage,
                       garage[i][j].emissionFactor,
                       garage[i][j].color,
                       garage[i][j].ratePerDay,
                       (garage[i][j].isRented == 0 ? "Available" : "Rented"));
            }
        }

        // Optional: show message if category empty
        if (!hasCarInCategory) {
            printf("No cars available in this category.\n");
        }
    }

    printf("%-110s\n", "------------------------------------------------------------------------------------------------------------------------");
}

/* 
	<Description of function>
	Sorts available cars by carbon emissions in ascending order
	
	Precondition: 
	The garage array contains valid car data
	
	@param Car garage[CATEGORIES][SLOTS] : The car storage system holding details about all cars
	
	@return (void)
*/

void SortByCarbonEmissions (Car garage[CATEGORIES][SLOTS]) {
	
	Car flatlist[CATEGORIES * SLOTS]; // Temporary 1D array to store all available cars for sorting
	int count = 0;
	
	int i, j;
	int min_idx;
	Car temp;
	
	// Flatten the 2D garage array into a 1D list of available cars
	for (i = 0; i < CATEGORIES; i++) {
		for (j = 0; j < SLOTS; j++) {
				if (garage[i][j].carID != 0  ) {
					flatlist[count] = garage[i][j];
					count++; // Increment number of cars added
				}
			
		}
		
	}
	
	// SELECTION SORT
	for (i = 0; i < count - 1; i++) {
		
		min_idx = i;
		
		for (j = i + 1; j < count; j++) {
			
			if (flatlist[min_idx].emissionFactor > flatlist[j].emissionFactor) {
				min_idx = j;
			}
				
		}
		
		// Swap the car at i with the car at min_idx
		if (i != min_idx) {	
			temp = flatlist[i];
			flatlist[i] = flatlist[min_idx];
			flatlist[min_idx] = temp;	
		}
	}
	
	printf("\n>>> RESULTS SORTED BY CARBON EMISSIONS <<<\n");
	printTable(flatlist, count);
	
}

/* 
	<Description of function>
	Sorts available cars by price in ascending order
	
	Precondition: 
	The garage array contains valid car data
	
	@param Car garage[CATEGORIES][SLOTS] : The car storage system holding details about all cars
	
	@return (void)
*/

void SortByPrice (Car garage[CATEGORIES][SLOTS]) {
	
	Car flatlist[CATEGORIES * SLOTS]; // Temporary 1D array to store all available cars for sorting
	int count = 0;
	
	int i, j;
	int min_idx;
	Car temp;
	
	// Flatten the 2D garage array into a 1D list of available cars
	for (i = 0; i < CATEGORIES; i++) {
		for (j = 0; j < SLOTS; j++) {
			if (garage[i][j].carID != 0) {
				flatlist[count] = garage[i][j];
				count++; // Increment number of cars added
			}
		}
		
	}
	
	// SELECTION SORT: Sort flatlist based on ratePerDay in ascending order
	for (i = 0; i < count - 1; i++) {
		
		min_idx = i;
		
		for (j = i + 1; j < count; j++) {
			
			if (flatlist[min_idx].ratePerDay > flatlist[j].ratePerDay) {
				min_idx = j;
			}
				
		}
		
		if (i != min_idx) {
			temp = flatlist[i];
			flatlist[i] = flatlist[min_idx];
			flatlist[min_idx] = temp;	
		}
	}
	
	printf("\n>>> RESULTS SORTED BY PRICE <<<\n");
	printTable(flatlist, count);
	
}

/* 
	<Description of function>
	Sorts available cars alphabetically in ascending order
	
	Precondition: 
	The garage array valid car data
	
	@param Car garage[CATEGORIES][SLOTS] : The car storage system holding details about all cars
	
	@return (void)
*/

void SortByModel(Car garage[CATEGORIES][SLOTS]){
	
	Car flatlist[CATEGORIES * SLOTS]; // Temporary 1D array to store all available cars for sorting
	int count = 0;
	
	int i, j;
	int min_idx;
	Car temp;
	
	// Flatten the 2D garage array into a 1D list of available cars
	for (i = 0; i < CATEGORIES; i++) {
		for (j = 0; j < SLOTS; j++) {
			if (garage[i][j].carID != 0) {
				flatlist[count] = garage[i][j];
				count++;
			}
		}
	}
	
	// SELECTION SORT
	for (i = 0; i < count - 1; i++) {
		
		min_idx = i;
		
		for (j = i + 1; j < count; j++) {
			
			if (strcmp(flatlist[min_idx].model, flatlist[j].model) > 0) {
				min_idx = j;
			}
				
		}
		
		if (i != min_idx) {
			temp = flatlist[i];
			flatlist[i] = flatlist[min_idx];
			flatlist[min_idx] = temp;	
		}	
	}
	
	printf("\n>>> RESULTS SORTED BY MODEL <<<\n");
	printTable(flatlist, count);
	

}

/* 
	<Description of function>
	Prints a formatted table of car details 
	
	Precondition: 
	The list array contains valid car entries and count is accurate
	
	@param Car list[] : Array of cars to display
	@param int count: Number of cars in the list
	
	@return (void)
*/

void printTable(Car list[], int count) {
	
	int i;
    printf("\n%-110s\n", "------------------------------------------------------------------------------------------------------------------------");
   printf("%-5s | %-20s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-12s\n", 
"ID", "MODEL", "TYPE", "FUEL", "MILEAGE", "EMISSION", "COLOR", "RATE/DAY", "AVAILABILITY");

   printf("%-110s\n", "------------------------------------------------------------------------------------------------------------------------");
	// Loop through each car in the list and print its details
    for (i = 0; i < count; i++) {
       printf("%-5d | %-20s | %-10s | %-10s | %-10.1f | %-6.1fg/km | %-10s | P%-9.2f | %-10s\n",
list[i].carID, list[i].model, list[i].type,
list[i].fuelType, list[i].mileage, list[i].emissionFactor,
list[i].color,
list[i].ratePerDay,
(list[i].isRented == 0 ? "Available" : "Rented"));
    }
    printf("%-110s\n", "------------------------------------------------------------------------------------------------------------------------");
}

/* 
	<Description of function>
	Handles the process of renting a car, including billing and CO2 calculation.
	
	Precondition: 
	The garage contains available cars and the username is valid.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	@param char username[]: Name of the user renting the car
	
	@return (void)
*/
void rentCar(Car garage[CATEGORIES][SLOTS], char username[]) {

    RentalRecord rec = {0};        // Structure to store rental record details
    int inputID;             // Car ID entered by the user
    int i, j;                
    int targetRow, targetCol; // Indices of the selected car in the garage array
    char cExtraChar;         
    int found = 0;           
    int days;                
    float tripDistance;      
    float totalBill;         
    int daysValid = 0;       
    char extraChar;          
    int isValid;             

    // Loop until a valid car ID is selected or user exits
    while (found == 0) {
        printf("SELECT CAR ID YOU WANT TO RENT (or 0 to go back): ");
        
        // Validate integer input for car ID
        if (scanf("%d%c", &inputID, &cExtraChar) != 2 || cExtraChar != '\n') {
            printf("Invalid input. Try again\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else if (inputID == 0) { // User chooses to go back
            found = -1;
        }
        else {
            // Search the garage for the entered car ID
            for (i = 0; i < CATEGORIES; i++) {
                for (j = 0; j < SLOTS; j++) {
                    if (garage[i][j].carID == inputID && garage[i][j].isRented == 0) {
                        found = 1;            // Car found and available
                        targetRow = i;        // Store row index of car
                        targetCol = j;        // Store column index of car
                    }
                }
            }

            if (found == 0) { // If car ID not found or already rented
                printf("Error: ID not found. Please try again.\n");
            }
        }    

        // If a valid car is found, proceed to rental process
        if (found == 1) {

            // Store basic rental record information
            strcpy(rec.customerName, username); // Set customer name
            rec.carID = garage[targetRow][targetCol].carID; // Set car ID
            strcpy(rec.model, garage[targetRow][targetCol].model); // Set car model

			

			strcpy(rec.status, "In Use");
		printf("Enter rental date (YYYY-MM-DD): ");
		scanf("%14s", rec.rentDate);

		printf("Enter expected return date (YYYY-MM-DD): ");
		scanf("%14s", rec.returnDate);
            // Loop to get valid number of rental days (1-10)
            do {
                printf("\nHow many days do you wish to rent this car? ");
                if (scanf("%d%c", &rec.days, &cExtraChar) != 2 || cExtraChar != '\n') {
                    printf("Invalid input! Please enter a numeric value.\n");
                    while(getchar() != '\n'); 
                    rec.days = 0; // Force loop to repeat 
                }     
                else if (rec.days <= 0) {
                    printf("Error: You must rent for at least 1 day.\n");
                }
                else if (rec.days > 10) {
                    printf("Maximum of 10 days only.\n");
                }

            } while (rec.days <= 0 || rec.days > 10);

            // Loop to get valid estimated distance
            isValid = 0;
            while (isValid == 0) {
                printf("Enter estimated distance you will travel (kilometers): ");
                if (scanf("%f%c", &tripDistance, &extraChar) != 2 || extraChar != '\n') {
                    printf("Invalid input.\n");
                    while (getchar() != '\n'); // Clear input buffer
                }
                else if (tripDistance <= 0) {
                    printf("Invalid distance");
                }
                else {
                    isValid = 1; // Valid distance entered
                }        
            }

            // Calculate total CO2 emissions for the trip
            rec.totalCO2 = tripDistance * garage[targetRow][targetCol].emissionFactor;

            // Calculate total rental bill
            rec.totalBill = garage[targetRow][targetCol].ratePerDay * rec.days;

            // Mark car as rented and record the user
            garage[targetRow][targetCol].isRented = 1;
            strcpy(garage[targetRow][targetCol].rentedBy, username);

            // Open rental_records.txt to append new rental record
            FILE *records;
            records = fopen("rental_records.txt", "a");

            if (records != NULL) {
                // Write rental record to file in pipe-separated format
               fprintf(records, "%s|%d|%s|%d|%.2f|%.2f|%s|%s|%s\n",
   			 rec.customerName,
    		 rec.carID, 
    		rec.model,
    		rec.days,    
   		    rec.totalBill,
   		    rec.totalCO2,
   		   rec.status,
   		   rec.rentDate,
    		rec.returnDate);

                fclose(records); // Close the file
            }

            // Save updated garage and all car states
        
            saveAllCars(garage);

            printf("\nRental successful! Enjoy your trip.\n");
        }
    }
}

/* 
	<Description of function>
	Displays rental history records for a specific user.
	
	Precondition: 
	The rental_records.txt file exists and contains properly formatted data.
	
	@param char username[]: Name of the user
	
	@return (void)
*/

void ShowRentalRecords(char username[]) {
    
    int recordsFound = 0;
    char nameBuf[30]; // Temporary buffer to store username from file
    char modelBuf[30]; // Temporary buffer to store car model from file
    int carID, days;
    float totalBill, totalCO2;
   
	char status[15];
	char rentDate[15], returnDate[15];
    FILE *fp = fopen("rental_records.txt", "r");

    if (fp == NULL) {
        printf("No rental records found in the system\n");
    } 
    else {
        printf("\n--- RENTAL HISTORY FOR %s ---\n", username);
       printf("%-5s | %-18s | %-5s | %-12s | %-10s | %-12s | %-12s\n",
"ID", "MODEL", "DAYS", "TOTAL", "STATUS", "RENT DATE", "RETURN DATE");
        printf("---------------------------------------------------------------------------------------------\n");

        char line[512]; // Buffer to store each line from the file

        while (fgets(line, sizeof(line), fp) != NULL) {

            // Skip empty lines
            if (line[0] != '\n' && line[0] != '\0') {
				
				// Parse the line using sscanf to extract all fields
                int fields = sscanf(line,
				"%29[^|]|%d|%29[^|]|%d|%f|%f|%14[^|]|%14[^|]|%14[^\n]",
				nameBuf, &carID, modelBuf, &days, &totalBill, &totalCO2,
				status, rentDate, returnDate);		
                
                if (fields != 9) {
   	 		continue;
    	
			}
				nameBuf[strcspn(nameBuf, "\n")] = '\0';
				// Check if all 7 fields were successfully scanned
               
                    if (strcasecmp(nameBuf, username) == 0) {
                       printf("%-5d | %-18s | %-5d | P%-11.2f | %-10s | %-12s | %-12s\n",
       carID, modelBuf, days, totalBill, status, rentDate, returnDate);
                        recordsFound++; // Increment counter for records found
                    }
                }
               
            }
        }
			// If no records were found for this user
	        if (recordsFound == 0) {
	            printf("No rental records found for this account.\n");
	        }
	
	        printf("---------------------------------------------------------------------------------------------\n");
	        fclose(fp);
    	}


/* 
	<Description of function>
	Saves the current rental status of all cars into a file.
	
	Precondition: 
	The garage array contains updated rental statuses.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/

/* 
	<Description of function>
	Loads rental status of cars from a file and updates the garage.
	
	Precondition: 
	The garage array is initialized and garage_state.txt exists.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/

/* 
	<Description of function>
	Loads rental status of cars from a file and updates the garage.
	
	Precondition: 
	The garage array is initialized and garage_state.txt exists.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/
void saveAllCars(Car garage[CATEGORIES][SLOTS]) {
    int i, j;
    FILE *fp = fopen("all_cars.txt", "w");

    if (fp == NULL) {
        printf("Error saving cars!\n");
        return;
    }

    // Header (for readability)
    fprintf(fp, "CAR ID | MODEL | TYPE | FUEL | MILEAGE | EMISSION | COLOR | RATE | AVAILABILITY | RENTED BY\n");

    for (i = 0; i < CATEGORIES; i++) {
        for (j = 0; j < SLOTS; j++) {

            // ? ONLY save valid cars (prevents accidental deletion)
            if (garage[i][j].carID >= 100 && garage[i][j].carID <= 599) {

                fprintf(fp, "%d|%s|%s|%s|%.2f|%.2f|%s|%.2f|%s|%s\n",
                        garage[i][j].carID,
                        garage[i][j].model,
                        garage[i][j].type,
                        garage[i][j].fuelType,
                        garage[i][j].mileage,
                        garage[i][j].emissionFactor,
                        garage[i][j].color,
                        garage[i][j].ratePerDay,
                        garage[i][j].isRented == 1 ? "Rented" : "Available",
                        garage[i][j].rentedBy);
            }
        }
    }

    fclose(fp);
}
/* 
	<Description of function>
	Provides administrative controls such as adding, deleting, and returning cars.
	
	Precondition: 
	The garage array is initialized and admin credentials are verified.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/
void adminSide(Car garage[CATEGORIES][SLOTS]) {
	
	int cChoice, cExtraChar;
	int isValid = 1;
	char display;
	while (isValid == 1) {
		
		
	
		printf("1. Add Car\n");
		printf("2. Delete Car\n");
		printf("3. Edit Car\n");
		printf("4. Show All Rental History And Income\n");
		printf("5. Show Income Per Car\n");
		printf("6. Show Most Rented Car\n");
		printf("7. Display All Cars\n");
		printf("8. Show Receipt\n");
		printf("9. Log out\n");
		printf("Choose: ");
		
		// Validate user input for menu selection
		if (scanf("%d%c", &cChoice, &cExtraChar) != 2 || cExtraChar != '\n') {
			printf("Invalid input. Try again.\n");
			while (getchar() != '\n'); // Clear input buffer	
		}
		else {
			
			switch (cChoice) {
				
				case 1:
					addCar(garage);
					break;	
				
				case 2:
					deleteCar(garage);
					break;
				case 3: editCar(garage);
    					break;
				case 4:
				 showAllRentalHistoryAndIncome();
				break;
				case 5:  showIncomePerCar();
   					 break;
   				case 6: showMostRentedCar();
    			break;
				case 7: displayAllCars(garage); 
				break;
				case 78: generateReceipt();
    				break;
				case 9: 
					printf("Exiting Admin Mode.\n");
					isValid = 0;
					break;
					
				default:
					printf("Error! Input valid number.\n");
					break;			
			}	
		}	
	}
}


/* 
	<Description of function>
	Adds a new car into the garage system.
	
	Precondition: 
	There is available space in the selected category.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/
void addCar(Car garage[CATEGORIES][SLOTS]) {
    
    Car newCar;
    int index_carType;
    int isAdded = 0;
    int i, j;
    char extraChar;
    int valid = 1;
    int idValid = 0;
    int duplicate;
    int goback = 0;
    printf("---ADD CAR---\n");
	
	// Input Car ID with validation and duplicate check
    if (valid == 1) {
        while (idValid == 0 && goback == 0) {
            printf("(Enter Car ID or 0 to go back): ");
            if (scanf("%d%c", &newCar.carID, &extraChar) != 2 || extraChar != '\n') {
                printf("Invalid Car ID.\n");
                while (getchar() != '\n');
            }
            else if (newCar.carID == 0){
            	goback = 1;
			}
            else {
                duplicate = 0;
                
                for (i = 0; i < CATEGORIES; i++) {
                    for (j = 0; j < SLOTS; j++) {
                        if (garage[i][j].carID == newCar.carID) {
                            duplicate = 1;
                        }
                    }
                }

                if (duplicate == 1) {
                    printf("ID already exists. Try again.\n");
                }
                else {
                    idValid = 1;
                }
            }
        }
    }
	
	// Input Car model
    if (valid == 1 && goback == 0) {
        printf("Enter Car model: ");
        scanf(" %[^\n]", newCar.model);
        while(getchar() != '\n');
    }
	
	// Input category index with validation
    if (valid == 1 && goback == 0) {
        printf("Enter category (0 - Sedan, 1 - SUV, 2 - PICKUP, 3 - CROSSOVER, 4 - ELECTRIC): ");
        if (scanf("%d%c", &index_carType, &extraChar) != 2 || extraChar != '\n') {
            printf("Invalid category.\n");
            while (getchar() != '\n');
            valid = 0;
        }
    }
	
	// Check if category index is within valid range
    if (valid == 1 && (index_carType < 0 || index_carType >= CATEGORIES) && goback == 0) {
        printf("Invalid category number.\n");
        valid = 0;
    }
    
	// Assign category name and input fuel type
    if (valid == 1 && goback == 0) {
        switch (index_carType) {
            case 0: 
				strcpy(newCar.type, "Sedan"); 
				break;
            case 1: 
				strcpy(newCar.type, "SUV"); 
				break;
            case 2: 
				strcpy(newCar.type, "PICKUP"); 
				break;
            case 3: 
				strcpy(newCar.type, "CROSSOVER"); 
				break;
            case 4: 
				strcpy(newCar.type, "ELECTRIC"); 
				break;
        }

        printf("Enter fuel type: ");
        scanf(" %[^\n]", newCar.fuelType);
    }
	
	// Input mileage with validation
    if (valid == 1 && goback == 0) {
        printf("Enter car mileage: ");
        if (scanf("%f%c", &newCar.mileage, &extraChar) != 2 || extraChar != '\n') {
            printf("Invalid mileage.\n");
            while (getchar() != '\n');
            valid = 0;
        }
    }
	
	// Input emission factor with validation
    if (valid == 1 && goback == 0) {
        printf("Enter Emission Factor: ");
        if (scanf("%f%c", &newCar.emissionFactor, &extraChar) != 2 || extraChar != '\n') {
            printf("Invalid emission.\n");
            while (getchar() != '\n');
            valid = 0;
        }
    }
	
	// Input car color
    if (valid == 1 && goback == 0) {
        printf("Enter car color: ");
        scanf(" %[^\n]", newCar.color);

        printf("Enter Rate per Day: ");
        if (scanf("%f%c", &newCar.ratePerDay, &extraChar) != 2 || extraChar != '\n') {
            printf("Invalid rate.\n");
            while (getchar() != '\n');
            valid = 0;
        }
    }
	
	// Add the car to the first available slot in the selected category
    if (valid == 1 && goback == 0) {
        newCar.isRented = 0;
        strcpy(newCar.rentedBy, "");

        i = 0;
        while (i < SLOTS && isAdded == 0) {
            if (garage[index_carType][i].carID == 0) { // Find empty slot
                garage[index_carType][i] = newCar;
                isAdded = 1;
            } 
            else {
                i++;
            }
        }

        if (isAdded == 1) {
            printf("Car successfully added!\n");
            saveAllCars(garage);
            
        } 
        else {
            printf("No available slot for this category.\n");
        }
    } 
    else if (goback == 1){
    
	}
    else {
        printf("Car was not added due to invalid input.\n");
    }
}
/* 
	<Description of function>
	Deletes a car from the garage based on its ID.
	
	Precondition: 
	The garage contains car data and the ID exists.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/
void deleteCar (Car garage[CATEGORIES][SLOTS]) {
    
    int i, j;
    int targetID;
    char cExtraChar;
    char check;
    int exitLoop = 0;

    do {
        int found = 0;

        printf("---DELETE CAR FROM SYSTEM---\n");
        printf("Enter Car ID you want to delete from the system (or 0 to go back): ");

        if (scanf("%d%c", &targetID, &cExtraChar) != 2 || cExtraChar != '\n') {
            printf("Error! Invalid input.\n");
            while (getchar() != '\n');
        }
        else if (targetID == 0) {
            printf("Returning to menu...\n");
            exitLoop = 1; // ? exit condition
        }
        else {

            // ?? CHECK FIRST (no deletion yet)
            for (i = 0; i < CATEGORIES; i++) {
                for (j = 0; j < SLOTS; j++) {
                    if (garage[i][j].carID == targetID) {
                        found = 1;
                    }
                }
            }

            if (found == 0) {
                printf("Error! Car ID %d not found.\n", targetID);
            }
            else {
                
                printf("Are you sure you want to delete this car (Y/N)? ");
                scanf(" %c", &check);

                if (check == 'y' || check == 'Y') {

                    
                    for (i = 0; i < CATEGORIES; i++) {
                        for (j = 0; j < SLOTS; j++) {
                            if (garage[i][j].carID == targetID) {
                                garage[i][j] = (Car){0};
                            }
                        }
                    }

                    printf("Car ID %d successfully deleted!\n", targetID);
                    saveAllCars(garage);
                    
                }
                else {
                    printf("Deletion cancelled.\n");
                }
            }
        }

    } while (exitLoop == 0);
}
/* 
	<Description of function>
	Handles returning of rented cars by validating ownership.
	
	Precondition: 
	The garage contains rented cars and username is valid.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	@param char username[]: Name of the user returning the car
	
	@return (void)
*/
void returnCar(Car garage[CATEGORIES][SLOTS], char username[]) {
	
	//loadGarageState(garage);
	
	int i, j;
	char cExtraChar;
	int targetID;
	int found = 0;
	int sessionActive = 1;
	int isMatch;
	char rentDate[15];
	char returnDate[15];
	
	printf("---RETURN CAR FROM SYSTEM---\n");
	
	// Loop until a valid car is returned or user exits
	while (found == 0 && sessionActive == 1) {
		
		printf("Enter Car ID you want to return (or 0 to go back): ");
		
		// Validate input
		if (scanf("%d%c", &targetID, &cExtraChar) != 2 || cExtraChar != '\n') {
			printf("Error! Invalid input.\n");
			while (getchar() != '\n');
		}
		
		else if (targetID == 0) {
			sessionActive = 0;
		}
		
		else {
			
			isMatch = 0; // Reset match flag
			for (i = 0; i < CATEGORIES; i++) {
				for (j = 0; j < SLOTS; j++) {
					if (garage[i][j].carID == targetID && garage[i][j].isRented == 1) {
						isMatch = 1;
						if (strcasecmp(garage[i][j].rentedBy, username) == 0) {
							garage[i][j].isRented = 0; // Mark as available
							strcpy(garage[i][j].rentedBy, ""); // Clear renter info
							found = 1;
						}
						
						else {
						printf("Car rented by someone else. Try another ID.\n");
							sessionActive = 0; // End session if user cannot return this car
						}
						
					}
	
				}	
			}
			
			if (found == 1) {
				printf("Car ID %d successfully returned!\n", targetID);	
				
				FILE *fp = fopen("rental_records.txt", "r");
FILE *temp = fopen("temp.txt", "w");

char name[30], model[30], status[15];
int carID, days;
float totalBill, totalCO2;

	if (fp == NULL || temp == NULL) {
    printf("File error!\n");
    return;
}

while (fscanf(fp, "%29[^|]|%d|%29[^|]|%d|%f|%f|%14[^|]|%14[^|]|%14[^\n]\n",
              name, &carID, model, &days, &totalBill, &totalCO2,
        status, rentDate, returnDate) == 9) {

    if (carID == targetID && strcmp(name, username) == 0 && strcmp(status, "In Use") == 0) {
        strcpy(status, "Returned");
    }

   fprintf(temp, "%s|%d|%s|%d|%.2f|%.2f|%s|%s|%s\n",
        name, carID, model, days, totalBill, totalCO2,
        status, rentDate, returnDate);
}

fclose(fp);
fclose(temp);

remove("rental_records.txt");
rename("temp.txt", "rental_records.txt");
				saveAllCars(garage);
				
			}
			else {
				printf("Car ID %d not found in the system or currently rented by someone else\n", targetID);
			}
		}
	}	
}

/* 
	<Description of function>
	Loads all car data from file into the garage array.
	
	Precondition: 
	The all_cars.txt file exists and is properly formatted.
	
	@param Car garage[CATEGORIES][SLOTS]: The car storage system
	
	@return (void)
*/
void loadAllCars(Car garage[CATEGORIES][SLOTS]) {
    FILE *fp = fopen("all_cars.txt", "r");
int i,j;
    if (fp == NULL) {
        return; // no file ? keep initialized cars
    }

    char line[512];

    int carID;
    char model[30], type[15], fuelType[15], color[15];
    char status[15], rentedBy[30];
    float mileage, emissionFactor, ratePerDay;

    int updatedCount = 0;

    // Skip header
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {

        int fields = sscanf(line,
            "%d|%29[^|]|%14[^|]|%14[^|]|%f|%f|%14[^|]|%f|%14[^|]|%29[^\n]",
            &carID, model, type, fuelType,
            &mileage, &emissionFactor,
            color, &ratePerDay,
            status, rentedBy
        );

        // ? Ignore bad lines
        if (fields != 10) continue;

        // ? Ignore invalid IDs
        if (carID < 100 || carID > 599) continue;

        // ?? FIND AND UPDATE EXISTING CAR
        for ( i = 0; i < CATEGORIES; i++) {
            for ( j = 0; j < SLOTS; j++) {

                if (garage[i][j].carID == carID) {

                    // ? ONLY update dynamic fields
                    garage[i][j].isRented = (strcmp(status, "Rented") == 0);
                    strcpy(garage[i][j].rentedBy, rentedBy);

                    updatedCount++;
                }
            }
        }
    }

    fclose(fp);

    // ?? SAFETY CHECK: if file is incomplete, ignore it
    if (updatedCount == 0) {
        printf("Warning: Car data file invalid or empty. Using default data.\n");
    }
}

/* 
	<Description of function>
	Encrypts a file using a simple character-shifting algorithm with a key.
	
	Precondition: 
	Input file exists and key is a valid string.
	
	@param const char inputFile[]: Source file to encrypt
	@param const char outputFile[]: Destination encrypted file
	@param const char key[]: Encryption key
	
	@return int: 1 if successful, 0 otherwise
*/
int encryptFile(const char inputFile[], const char outputFile[], const char key[]) {
    FILE *fpIn = NULL;
    FILE *fpOut = NULL;

    char line[300];
    int i, j, encrypted;
    int success = 0;

    const int ASCII_MIN = 32; // Minimum printable ASCII character
    const int ASCII_MAX = 126; // Maximum printable ASCII character
    const int ASCII_RANGE = (ASCII_MAX - ASCII_MIN + 1);

    fpIn = fopen(inputFile, "r");

    if (fpIn == NULL) {
        
        success = 0;
    }
    else {
    	// Read each line from input file
        fpOut = fopen(outputFile, "w");

        if (fpOut == NULL) {
            printf("Error opening output file: %s\n", outputFile);
            success = 0;
        }
        else {
            while (fgets(line, sizeof(line), fpIn) != NULL) {
                i = 0;
                j = 0;

                while (line[i] != '\0') {
                    if (line[i] == '\n') {
                        i++; /* keep newline */
                    }
                    else {
                        if (key[j] == '\0') {
                            j = 0;
                        }
						
						// Encryption formula using key and ASCII wrap-around
                        encrypted = line[i] + key[j];
                        encrypted = ASCII_MIN + (encrypted - ASCII_MIN) % ASCII_RANGE;
                        line[i] = (char)encrypted;

                        i++;
                        j++;
                    }
                }

                fputs(line, fpOut); // Write encrypted line to output file
            }

            success = 1;
        }
    }
	
	// Close input and output files if they were opened
    if (fpIn != NULL) {
        fclose(fpIn);
    }

    if (fpOut != NULL) {
        fclose(fpOut);
    }

   
    if (success == 0) {
        remove(outputFile);
    }

    return success;
}

/* 
	<Description of function>
	Decrypts a previously encrypted file using the same key.
	
	Precondition: 
	Input file exists and was encrypted using the same key.
	
	@param const char inputFile[]: Encrypted file
	@param const char outputFile[]: Destination decrypted file
	@param const char key[]: Decryption key
	
	@return int: 1 if successful, 0 otherwise
*/
int decryptFile(const char inputFile[], const char outputFile[], const char key[]) {
    FILE *fpIn = NULL;
    FILE *fpOut = NULL;

    char line[300];
    int i, j, decrypted;
    int success = 0;

    const int ASCII_MIN = 32;
    const int ASCII_MAX = 126;
    const int ASCII_RANGE = (ASCII_MAX - ASCII_MIN + 1);

    /* Open input first to avoid creating output when input doesn't exist */
    fpIn = fopen(inputFile, "r");

    if (fpIn == NULL) {
        /* Optional: remove this print if you don't want messages on first run */
        /* printf("Error opening input file: %s\n", inputFile); */
        success = 0;
    }
    else {
        fpOut = fopen(outputFile, "w");

        if (fpOut == NULL) {
            printf("Error opening output file: %s\n", outputFile);
            success = 0;
        }
        else {
        	// Read each line from input file
            while (fgets(line, sizeof(line), fpIn) != NULL) {
                i = 0;
                j = 0;
				
				// Decrypt each character in the line
                while (line[i] != '\0') {
                    if (line[i] == '\n') {
                        i++; 
                    }
                    else {
                        if (key[j] == '\0') {
                            j = 0; // Restart key index if end reached
                        }
						
						// Decryption formula using key and ASCII wrap-around
                        decrypted = line[i] - key[j];

                        while (decrypted < ASCII_MIN) {
                            decrypted += ASCII_RANGE;
                        }

                        line[i] = (char)decrypted;

                        i++;
                        j++;
                    }
                }

                fputs(line, fpOut);
            }

            success = 1;
        }
    }

    if (fpIn != NULL) {
        fclose(fpIn);
    }

    if (fpOut != NULL) {
        fclose(fpOut);
    }

    if (success == 0) {
        remove(outputFile);
    }

    return success;
}

/* 
	<Description of function>
	Encrypts all system data files before program exit.
	
	Precondition: 
	All required files exist or were created during execution.
	
	@return (void)
*/
void encryptAllFiles() {
    FILE *tempCheck;

    /* Encrypt all_cars.txt */
    if (encryptFile("all_cars.txt", "temp_cars.txt", "secretkey") == 1) {
        tempCheck = fopen("temp_cars.txt", "r");
        if (tempCheck != NULL) {
            fclose(tempCheck);
            remove("all_cars.txt");
            rename("temp_cars.txt", "all_cars.txt");
        }
    }

    /* Encrypt users.txt */
    if (encryptFile("users.txt", "temp_users.txt", "secretkey") == 1) {
        tempCheck = fopen("temp_users.txt", "r");
        if (tempCheck != NULL) {
            fclose(tempCheck);
            remove("users.txt");
            rename("temp_users.txt", "users.txt");
        }
    }


    /* Encrypt rental_records.txt */
    if (encryptFile("rental_records.txt", "temp_records.txt", "secretkey") == 1) {
        tempCheck = fopen("temp_records.txt", "r");
        if (tempCheck != NULL) {
            fclose(tempCheck);
            remove("rental_records.txt");
            rename("temp_records.txt", "rental_records.txt");
        }
    }
}

/* 
	<Description of function>
	Decrypts all system data files at program startup.
	
	Precondition: 
	Encrypted files exist and correct key is used.
	
	@return (void)
*/
void decryptAllFiles() {
    FILE *tempCheck;

    /* Decrypt all_cars.txt */
    if (decryptFile("all_cars.txt", "temp_cars.txt", "secretkey") == 1) {
        tempCheck = fopen("temp_cars.txt", "r");
        if (tempCheck != NULL) {
            fclose(tempCheck);
            remove("all_cars.txt");
            rename("temp_cars.txt", "all_cars.txt");
        }
    }

    /* Decrypt users.txt */
    if (decryptFile("users.txt", "temp_users.txt", "secretkey") == 1) {
        tempCheck = fopen("temp_users.txt", "r");
        if (tempCheck != NULL) {
            fclose(tempCheck);
            remove("users.txt");
            rename("temp_users.txt", "users.txt");
        }
    }

   
    

    /* Decrypt rental_records.txt */
    if (decryptFile("rental_records.txt", "temp_records.txt", "secretkey") == 1) {
        tempCheck = fopen("temp_records.txt", "r");
        if (tempCheck != NULL) {
            fclose(tempCheck);
            remove("rental_records.txt");
            rename("temp_records.txt", "rental_records.txt");
        }
    }
}

/* 
	<Description of function>
	Checks whether system data is currently encrypted.
	
	Precondition: 
	The crypto_state.txt file may or may not exist.
	
	@return int: 1 if encrypted, 0 if not
*/
int isDataEncrypted() {
    FILE *fp = fopen("crypto_state.txt", "r");
    int state = 0;  /* default: plaintext */

    if (fp != NULL) {
        fscanf(fp, "%d", &state);  /* 1 = encrypted, 0 = plaintext */
        fclose(fp);
    }

    return state;
}

/* 
	<Description of function>
	Sets the encryption state of the system.
	
	Precondition: 
	The program has permission to write to crypto_state.txt.
	
	@param int state: Encryption state (1 = encrypted, 0 = not)
	
	@return (void)
*/
void setDataEncrypted(int state) {
    FILE *fp = fopen("crypto_state.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", state);
        fclose(fp);
    }
}
void showAllRentalHistoryAndIncome() {
    FILE *fp = fopen("rental_records.txt", "r");

    if (fp == NULL) {
        printf("No rental records found.\n");
        return;
    }
    
	char status[15];
    char name[30], model[30];
    int carID, days;
    float totalBill, totalCO2;
    char rentDate[15];
	char returnDate[15];	
    float totalIncome = 0;
    int recordCount = 0;
   

    printf("\n============================================================\n");
    printf("              RENTAL HISTORY (ALL RECORDS)\n");
    printf("============================================================\n");

   printf("%-20s | %-5s | %-18s | %-5s | %-10s | %-10s | %-12s | %-12s\n",
       "CUSTOMER", "ID", "MODEL", "DAYS", "TOTAL", "STATUS", "RENT DATE", "RETURN DATE");
    printf("------------------------------------------------------------------------------------------------------------------\n");

    char line[512];

    while (fgets(line, sizeof(line), fp) != NULL) {

    int fields = sscanf(line,
"%29[^|]|%d|%29[^|]|%d|%f|%f|%14[^|]|%14[^|]|%14[^\n]",
name, &carID, model, &days, &totalBill, &totalCO2,
status, rentDate, returnDate);

if (fields != 9) {
    continue;
} 


// If car is not yet in list, add it
	

        printf("%-20s | %-5d | %-18s | %-5d | P%-9.2f | %-10s | %-12s | %-12s\n",
               name, carID, model, days, totalBill, status, rentDate, returnDate);

        totalIncome += totalBill;
        recordCount++;
    }


    fclose(fp);
    

    printf("------------------------------------------------------------------------------------------------------------------\n");
    printf("TOTAL RECORDS: %d\n", recordCount);
    printf("TOTAL INCOME : P%.2f\n", totalIncome);
    printf("============================================================\n");
}

void showIncomePerCar() {
    FILE *fp = fopen("rental_records.txt", "r");
	int i;
    if (fp == NULL) {
        printf("No rental records found.\n");
        return;
    }

    int carIDs[100];
float carIncome[100];
char carModels[100][30];  
int count = 0;

    char name[30], model[30], status[15];
    int carID, days;
    float totalBill, totalCO2;
    char rentDate[15], returnDate[15];

    char line[512];

    while (fgets(line, sizeof(line), fp) != NULL) {

        int fields = sscanf(line,
            "%29[^|]|%d|%29[^|]|%d|%f|%f|%14[^|]|%14[^|]|%14[^\n]",
            name, &carID, model, &days, &totalBill, &totalCO2,
            status, rentDate, returnDate);

        if (fields != 9) {
            continue;
        }

        int i, found = 0;

        for (i = 0; i < count; i++) {
            if (carIDs[i] == carID) {
                carIncome[i] += totalBill;
                found = 1;
            }
        }

        if (found == 0) {
    carIDs[count] = carID;
    carIncome[count] = totalBill;
    strcpy(carModels[count], model);  
    count++;
}
    }

    fclose(fp);

    printf("\n===== INCOME PER CAR =====\n");
    for ( i = 0; i < count; i++) {
        printf("Car ID %d (%s) -> Total Income: P%.2f\n",
       carIDs[i], carModels[i], carIncome[i]);
    }
    printf("\n");
}

void showMostRentedCar() {
    FILE *fp = fopen("rental_records.txt", "r");

    if (fp == NULL) {
        printf("No rental records found.\n");
        return;
    }

    int carIDs[100];
    int rentCount[100];
    char carModels[100][30];
    int count = 0;
	int i;
    char name[30], model[30], status[15];
    int carID, days;
    float totalBill, totalCO2;
    char rentDate[15], returnDate[15];

    char line[512];

    while (fgets(line, sizeof(line), fp) != NULL) {

        int fields = sscanf(line,
            "%29[^|]|%d|%29[^|]|%d|%f|%f|%14[^|]|%14[^|]|%14[^\n]",
            name, &carID, model, &days, &totalBill, &totalCO2,
            status, rentDate, returnDate);

        if (fields != 9) continue;

        int i, found = 0;

        for (i = 0; i < count; i++) {
            if (carIDs[i] == carID) {
                rentCount[i]++;
                found = 1;
            }
        }

        if (found == 0) {
            carIDs[count] = carID;
            rentCount[count] = 1;
            strcpy(carModels[count], model);
            count++;
        }
    }

    fclose(fp);

    if (count == 0) {
        printf("No rental data available.\n");
        return;
    }

    // ?? Find most rented
    int maxIndex = 0;
    for ( i = 1; i < count; i++) {
        if (rentCount[i] > rentCount[maxIndex]) {
            maxIndex = i;
        }
    }

    printf("\n===== MOST RENTED CAR =====\n");
    printf("Car ID: %d\n", carIDs[maxIndex]);
    printf("Model : %s\n", carModels[maxIndex]);
    printf("Times Rented: %d\n", rentCount[maxIndex]);
    printf("===========================\n\n");
}
void generateReceipt() {
    FILE *fp = fopen("rental_records.txt", "r");

    if (fp == NULL) {
        printf("No rental records found.\n");
        return;
    }

    char targetName[30];
    printf("Enter customer name: ");
    scanf(" %[^\n]", targetName);

    char name[30], model[30], status[15];
    int carID, days;
    float totalBill, totalCO2;
    char rentDate[15], returnDate[15];

    char line[512];
    int found = 0;
    float grandTotal = 0;

    printf("\n=========================================\n");
    printf("           RENTAL RECEIPT\n");
    printf("=========================================\n");
    printf("Customer: %s\n", targetName);
    printf("-----------------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL) {

        int fields = sscanf(line,
            "%29[^|]|%d|%29[^|]|%d|%f|%f|%14[^|]|%14[^|]|%14[^\n]",
            name, &carID, model, &days, &totalBill, &totalCO2,
            status, rentDate, returnDate);

        if (fields != 9) continue;

        // OPTIONAL: only show completed rentals
        

        if (strcasecmp(name, targetName) == 0) {

            printf("Car ID   : %d\n", carID);
            printf("Model    : %s\n", model);
            printf("Days     : %d\n", days);
            printf("Rent Date: %s\n", rentDate);
            printf("Return   : %s\n", returnDate);
            printf("Amount   : P%.2f\n", totalBill);
            printf("-----------------------------------------\n");

            grandTotal += totalBill;
            found = 1;
        }
    }

    fclose(fp);

    if (found == 0) {
        printf("No completed rentals found for this user.\n");
    } else {
        printf("GRAND TOTAL: P%.2f\n", grandTotal);
    }

    printf("=========================================\n\n");
}

void editCar(Car garage[CATEGORIES][SLOTS]) {
    
    int targetID;
    char extraChar;
    int found = 0;
    int i, j;

    printf("\n--- EDIT CAR ---\n");
    printf("Enter Car ID to edit (or 0 to go back): ");

    if (scanf("%d%c", &targetID, &extraChar) != 2 || extraChar != '\n') {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    if (targetID == 0) {
        printf("Returning to menu...\n");
        return;
    }

    // ?? FIND CAR
    for (i = 0; i < CATEGORIES; i++) {
        for (j = 0; j < SLOTS; j++) {

            if (garage[i][j].carID == targetID) {
                found = 1;

                printf("\nCar Found!\n");
                printf("Model: %s\n", garage[i][j].model);

                // ?? EDIT MENU
                int choice;
                printf("\nWhat do you want to edit?\n");
                printf("1. Model\n");
                printf("2. Fuel Type\n");
                printf("3. Mileage\n");
                printf("4. Emission Factor\n");
                printf("5. Color\n");
                printf("6. Rate Per Day\n");
                printf("Choose: ");

                scanf("%d%c", &choice, &extraChar);

                switch (choice) {

                    case 1:
                        printf("Enter new model: ");
                        scanf(" %[^\n]", garage[i][j].model);
                        break;

                    case 2:
                        printf("Enter new fuel type: ");
                        scanf(" %[^\n]", garage[i][j].fuelType);
                        break;

                    case 3:
                        printf("Enter new mileage: ");
                        scanf("%f", &garage[i][j].mileage);
                        break;

                    case 4:
                        printf("Enter new emission factor: ");
                        scanf("%f", &garage[i][j].emissionFactor);
                        break;

                    case 5:
                        printf("Enter new color: ");
                        scanf(" %[^\n]", garage[i][j].color);
                        break;

                    case 6:
                        printf("Enter new rate per day: ");
                        scanf("%f", &garage[i][j].ratePerDay);
                        break;

                    default:
                        printf("Invalid choice.\n");
                        return;
                }

                printf("Car updated successfully!\n");

                // ?? SAVE CHANGES
                saveAllCars(garage);

                return;
            }
        }
    }

    if (found == 0) {
        printf("Car ID not found.\n");
    }
}
