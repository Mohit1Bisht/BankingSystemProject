// Simple Banking System
// A C implementation of a basic banking system using arrays to store account data

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_TRANSACTIONS 500
#define MAX_TRANSACTION_DESC 100

// Structure for bank account
typedef struct {
    int accountNumber;
    char ownerName[MAX_NAME_LENGTH];
    double balance;
    int isActive;
} Account;

// Structure for transaction records
typedef struct {
    int transactionId;
    int accountNumber;
    char type[20]; // "deposit", "withdrawal", "transfer"
    double amount;
    char description[MAX_TRANSACTION_DESC];
    char date[20];
} Transaction;

// Global variables
Account accounts[MAX_ACCOUNTS];
Transaction transactions[MAX_TRANSACTIONS];
int accountCount = 0;
int transactionCount = 0;

// Function prototypes
void initializeSystem();
int createAccount(char *name, double initialDeposit);
int findAccount(int accountNumber);
void displayAccount(int index);
void listAllAccounts();
int deposit(int accountNumber, double amount, char *description);
int withdraw(int accountNumber, double amount, char *description);
int transfer(int fromAccount, int toAccount, double amount);
void displayTransactionHistory(int accountNumber);
void getCurrentDate(char *dateStr);
void saveData();
void loadData();
void displayMenu();

int main() {
    initializeSystem();
    loadData(); // Load any existing data
    
    int choice, accountNumber, targetAccount;
    char name[MAX_NAME_LENGTH];
    double amount;
    char description[MAX_TRANSACTION_DESC];
    
    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear buffer
        
        switch (choice) {
            case 1: // Create account
                printf("Enter name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0; // Remove newline
                
                printf("Enter initial deposit amount: ");
                scanf("%lf", &amount);
                getchar(); // Clear buffer
                
                if (amount < 0) {
                    printf("Initial deposit cannot be negative.\n");
                } else {
                    int newAccountNumber = createAccount(name, amount);
                    printf("Account created successfully! Account Number: %d\n", newAccountNumber);
                }
                break;
                
            case 2: // Check balance
                printf("Enter account number: ");
                scanf("%d", &accountNumber);
                getchar(); // Clear buffer
                
                int index = findAccount(accountNumber);
                if (index != -1) {
                    displayAccount(index);
                } else {
                    printf("Account not found.\n");
                }
                break;
                
            case 3: // Deposit
                printf("Enter account number: ");
                scanf("%d", &accountNumber);
                getchar(); // Clear buffer
                
                printf("Enter deposit amount: ");
                scanf("%lf", &amount);
                getchar(); // Clear buffer
                
                printf("Enter description: ");
                fgets(description, MAX_TRANSACTION_DESC, stdin);
                description[strcspn(description, "\n")] = 0; // Remove newline
                
                if (deposit(accountNumber, amount, description)) {
                    printf("Deposit successful.\n");
                }
                break;
                
            case 4: // Withdraw
                printf("Enter account number: ");
                scanf("%d", &accountNumber);
                getchar(); // Clear buffer
                
                printf("Enter withdrawal amount: ");
                scanf("%lf", &amount);
                getchar(); // Clear buffer
                
                printf("Enter description: ");
                fgets(description, MAX_TRANSACTION_DESC, stdin);
                description[strcspn(description, "\n")] = 0; // Remove newline
                
                if (withdraw(accountNumber, amount, description)) {
                    printf("Withdrawal successful.\n");
                }
                break;
                
            case 5: // Transfer
                printf("Enter your account number: ");
                scanf("%d", &accountNumber);
                getchar(); // Clear buffer
                
                printf("Enter recipient account number: ");
                scanf("%d", &targetAccount);
                getchar(); // Clear buffer
                
                printf("Enter transfer amount: ");
                scanf("%lf", &amount);
                getchar(); // Clear buffer
                
                if (transfer(accountNumber, targetAccount, amount)) {
                    printf("Transfer successful.\n");
                }
                break;
                
            case 6: // Transaction history
                printf("Enter account number: ");
                scanf("%d", &accountNumber);
                getchar(); // Clear buffer
                
                displayTransactionHistory(accountNumber);
                break;
                
            case 7: // List all accounts
                listAllAccounts();
                break;
                
            case 8: // Save and exit
                saveData();
                printf("Thank you for using our banking system. Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
        system("clear"); // Clear screen (use "cls" for Windows)
    }
    
    return 0;
}

// Initialize the banking system
void initializeSystem() {
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        accounts[i].isActive = 0;
    }
    accountCount = 0;
    transactionCount = 0;
    
    printf("Banking System Initialized\n");
}

// Create a new account
int createAccount(char *name, double initialDeposit) {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts. Maximum limit reached.\n");
        return -1;
    }
    
    int newIndex = accountCount;
    accountCount++;
    
    accounts[newIndex].accountNumber = 1000 + newIndex;
    strcpy(accounts[newIndex].ownerName, name);
    accounts[newIndex].balance = initialDeposit;
    accounts[newIndex].isActive = 1;
    
    // Record the initial deposit as a transaction
    if (initialDeposit > 0) {
        char desc[MAX_TRANSACTION_DESC] = "Initial deposit";
        deposit(accounts[newIndex].accountNumber, initialDeposit, desc);
    }
    
    return accounts[newIndex].accountNumber;
}

// Find account by account number
int findAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && accounts[i].isActive) {
            return i;
        }
    }
    return -1; // Not found
}

// Display account details
void displayAccount(int index) {
    printf("\n------ Account Details ------\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Owner Name: %s\n", accounts[index].ownerName);
    printf("Current Balance: $%.2f\n", accounts[index].balance);
    printf("----------------------------\n");
}

// List all active accounts
void listAllAccounts() {
    printf("\n------ All Accounts ------\n");
    int activeAccounts = 0;
    
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].isActive) {
            printf("%d. Account #%d - %s - Balance: $%.2f\n", 
                   i + 1, accounts[i].accountNumber, 
                   accounts[i].ownerName, accounts[i].balance);
            activeAccounts++;
        }
    }
    
    if (activeAccounts == 0) {
        printf("No active accounts found.\n");
    }
    printf("--------------------------\n");
}

// Get current date as string
void getCurrentDate(char *dateStr) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dateStr, 20, "%Y-%m-%d %H:%M:%S", t);
}

// Deposit money into an account
int deposit(int accountNumber, double amount, char *description) {
    int index = findAccount(accountNumber);
    
    if (index == -1) {
        printf("Account not found.\n");
        return 0;
    }
    
    if (amount <= 0) {
        printf("Deposit amount must be positive.\n");
        return 0;
    }
    
    accounts[index].balance += amount;
    
    // Record transaction
    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount].transactionId = transactionCount + 1;
        transactions[transactionCount].accountNumber = accountNumber;
        strcpy(transactions[transactionCount].type, "deposit");
        transactions[transactionCount].amount = amount;
        strcpy(transactions[transactionCount].description, description);
        getCurrentDate(transactions[transactionCount].date);
        
        transactionCount++;
    } else {
        printf("Warning: Transaction log full. Transaction not recorded.\n");
    }
    
    return 1;
}

// Withdraw money from an account
int withdraw(int accountNumber, double amount, char *description) {
    int index = findAccount(accountNumber);
    
    if (index == -1) {
        printf("Account not found.\n");
        return 0;
    }
    
    if (amount <= 0) {
        printf("Withdrawal amount must be positive.\n");
        return 0;
    }
    
    if (accounts[index].balance < amount) {
        printf("Insufficient funds.\n");
        return 0;
    }
    
    accounts[index].balance -= amount;
    
    // Record transaction
    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount].transactionId = transactionCount + 1;
        transactions[transactionCount].accountNumber = accountNumber;
        strcpy(transactions[transactionCount].type, "withdrawal");
        transactions[transactionCount].amount = amount;
        strcpy(transactions[transactionCount].description, description);
        getCurrentDate(transactions[transactionCount].date);
        
        transactionCount++;
    } else {
        printf("Warning: Transaction log full. Transaction not recorded.\n");
    }
    
    return 1;
}

// Transfer money between accounts
int transfer(int fromAccount, int toAccount, double amount) {
    int fromIndex = findAccount(fromAccount);
    int toIndex = findAccount(toAccount);
    
    if (fromIndex == -1) {
        printf("Source account not found.\n");
        return 0;
    }
    
    if (toIndex == -1) {
        printf("Destination account not found.\n");
        return 0;
    }
    
    if (amount <= 0) {
        printf("Transfer amount must be positive.\n");
        return 0;
    }
    
    if (accounts[fromIndex].balance < amount) {
        printf("Insufficient funds for transfer.\n");
        return 0;
    }
    
    // Deduct from source account
    accounts[fromIndex].balance -= amount;
    
    // Add to destination account
    accounts[toIndex].balance += amount;
    
    // Record transactions
    if (transactionCount < MAX_TRANSACTIONS - 1) {
        // Source account transaction
        transactions[transactionCount].transactionId = transactionCount + 1;
        transactions[transactionCount].accountNumber = fromAccount;
        strcpy(transactions[transactionCount].type, "transfer_out");
        transactions[transactionCount].amount = amount;
        sprintf(transactions[transactionCount].description, "Transfer to account #%d", toAccount);
        getCurrentDate(transactions[transactionCount].date);
        
        transactionCount++;
        
        // Destination account transaction
        transactions[transactionCount].transactionId = transactionCount + 1;
        transactions[transactionCount].accountNumber = toAccount;
        strcpy(transactions[transactionCount].type, "transfer_in");
        transactions[transactionCount].amount = amount;
        sprintf(transactions[transactionCount].description, "Transfer from account #%d", fromAccount);
        getCurrentDate(transactions[transactionCount].date);
        
        transactionCount++;
    } else {
        printf("Warning: Transaction log full. Transactions not recorded.\n");
    }
    
    return 1;
}

// Display transaction history for an account
void displayTransactionHistory(int accountNumber) {
    int index = findAccount(accountNumber);
    
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }
    
    printf("\n------ Transaction History for Account #%d ------\n", accountNumber);
    int found = 0;
    
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == accountNumber) {
            printf("ID: %d | Type: %s | Amount: $%.2f | Date: %s | Description: %s\n",
                   transactions[i].transactionId,
                   transactions[i].type,
                   transactions[i].amount,
                   transactions[i].date,
                   transactions[i].description);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No transactions found for this account.\n");
    }
    printf("------------------------------------------------\n");
}

// Save all data to files
void saveData() {
    // Save accounts
    FILE *accountFile = fopen("accounts.dat", "wb");
    if (accountFile == NULL) {
        printf("Error: Could not open file to save accounts.\n");
        return;
    }
    
    fwrite(&accountCount, sizeof(int), 1, accountFile);
    fwrite(accounts, sizeof(Account), accountCount, accountFile);
    fclose(accountFile);
    
    // Save transactions
    FILE *transactionFile = fopen("transactions.dat", "wb");
    if (transactionFile == NULL) {
        printf("Error: Could not open file to save transactions.\n");
        return;
    }
    
    fwrite(&transactionCount, sizeof(int), 1, transactionFile);
    fwrite(transactions, sizeof(Transaction), transactionCount, transactionFile);
    fclose(transactionFile);
    
    printf("Data saved successfully.\n");
}

// Load data from files
void loadData() {
    // Load accounts
    FILE *accountFile = fopen("accounts.dat", "rb");
    if (accountFile == NULL) {
        printf("No previous account data found.\n");
    } else {
        fread(&accountCount, sizeof(int), 1, accountFile);
        fread(accounts, sizeof(Account), accountCount, accountFile);
        fclose(accountFile);
    }
    
    // Load transactions
    FILE *transactionFile = fopen("transactions.dat", "rb");
    if (transactionFile == NULL) {
        printf("No previous transaction data found.\n");
    } else {
        fread(&transactionCount, sizeof(int), 1, transactionFile);
        fread(transactions, sizeof(Transaction), transactionCount, transactionFile);
        fclose(transactionFile);
    }
    
    if (accountFile != NULL || transactionFile != NULL) {
        printf("Data loaded successfully.\n");
    }
}

// Display the main menu
void displayMenu() {
    printf("\n===== Simple Banking System =====\n");
    printf("1. Create a New Account\n");
    printf("2. Check Account Balance\n");
    printf("3. Deposit Money\n");
    printf("4. Withdraw Money\n");
    printf("5. Transfer Money\n");
    printf("6. View Transaction History\n");
    printf("7. List All Accounts\n");
    printf("8. Save and Exit\n");
    printf("================================\n");
}
