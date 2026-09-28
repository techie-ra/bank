#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_ACCOUNT 100
#define DATA_FILE "Bank_Data.txt"
#define IFSC_CODE "TOONSCODE"
#define CASESAR_KEY 3
typedef struct
{
    int accountNumber;
    char name[100];
    char aadhar[13];
    char password[50];
    char ifsc[12];
    float balance;
} Account;

Account accounts[MAX_ACCOUNT];
int accountCount = 0;
void encryptPassword(char *password);
void decryptPassword(char *password);
void saveData();
void loadData();
void createAccount();
void userLogin();
void adminMenu();
void deposit(int index);
void withdraw(int index);
void checkBalance(int index);
void viewAllAccounts();
void searchAccount();
void exportData();
int main()
{
    int choice;
    int index = 0;
    while (1)
    {
        printf("\n------ACCOUNT MENU--------\n");
        printf("1. User Id\n");
        printf("2. create User ID\n");
        printf("3. Deposit \n");
        printf("4. withdraw\n");
        printf("5. Check Balance\n");
        printf("6. All Accounts\n");
        printf("7. search Account\n");
        printf("8. Create Password\n");
        printf("9. Exit\n");
        printf("Choose your Option:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            userLogin();
            break;
        case 2:
            createAccount();
            break;
        case 3:
            deposit(index);
            break;
        case 4:
            withdraw(index);
            break;
        case 5:
            checkBalance(index);
            break;
        case 6:
            viewAllAccounts();
            break;
        case 7:
            searchAccount();
            break;
        case 8:
            createAccount();
            break;
        case 9:
            exit(0);
            break;
        default:
            printf("Invalid Input!!!!");
            break;
        }
    }
    return 0;
}
void encryptPassword(char *password) {
    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'a' && password[i] <= 'z')
            password[i] = (password[i] - 'a' + CASESAR_KEY) % 26 + 'a';
        else if (password[i] >= 'A' && password[i] <= 'Z')
            password[i] = (password[i] - 'A' + CASESAR_KEY) % 26 + 'A';
    }
}

void decryptPassword(char *password) {
    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'a' && password[i] <= 'z')
            password[i] = (password[i] - 'a' - CASESAR_KEY + 26) % 26 + 'a';
        else if (password[i] >= 'A' && password[i] <= 'Z')
            password[i] = (password[i] - 'A' - CASESAR_KEY + 26) % 26 + 'A';
    }
}
void saveData()
{
    FILE *ptr = fopen(DATA_FILE, "wb");
    if (ptr == NULL)
    {
        printf("Error!!!!");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, ptr);
    fwrite(accounts, sizeof(Account), accountCount, ptr);
    fclose(ptr);
}
void loadData()
{
    FILE *ptr = fopen(DATA_FILE, "rb");
    if (ptr == NULL)
    {
        printf("Error!!!!");
        return;
    }
    fread(&accountCount, sizeof(int), 1, ptr);
    fread(accounts, sizeof(Account), accountCount, ptr);
    fclose(ptr);
}
void createAccount()
{
    if (accountCount >= MAX_ACCOUNT)
    {
        printf("mera aukat se bahar nhi jana!!!\n");
        return;
    }
    Account NewAccount;
    NewAccount.accountNumber = 100 + accountCount;
    strcpy(NewAccount.ifsc, IFSC_CODE);
    printf("Enter Name:");
    scanf("%s", NewAccount.name);
    printf("Enter AAdhar Number:");
    scanf("%s", NewAccount.aadhar);
    if (strlen(NewAccount.aadhar) != 12)
    {
        printf("Please enter valid aadhar no..!\n");
        return;
    }
    int isValid = 0;
    while (!isValid)
    {
        printf("Set a password (only letters, no numbers or special characters): ");
        scanf("%s", NewAccount.password);

        isValid = 1;
        for (int i = 0; NewAccount.password[i] != '\0'; i++)
        {
            if (!isalpha(NewAccount.password[i]))
            {
                printf("Error:\n");
                isValid = 0;
                break;
            }
        }
    }

    encryptPassword(NewAccount.password);
    NewAccount.balance = 0.0f;
    accounts[accountCount++] = NewAccount;
    saveData();
    printf("Account create successfully!\n");
    printf("Your account number is %d\n", NewAccount.accountNumber);
    printf("Your IFSC code is %s\n", NewAccount.ifsc);
}
void userLogin() {
    int accountNumber;
    char password[50];
    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter password: ");
    scanf("%s", password);
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            char decryptedPassword[50];
            strcpy(decryptedPassword, accounts[i].password);
            decryptPassword(decryptedPassword);
            if (strcmp(decryptedPassword, password) == 0) {
                printf("Login successfully\n");
                int choice;
                do {
                    printf("\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Logout\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            deposit(i);
                            break;
                        case 2:
                            withdraw(i);
                            break;
                        case 3:
                            checkBalance(i);
                            break;
                        case 4:
                            printf("Logged out successfully.\n");
                            return;
                        default:
                            printf("Invalid choice.\n");
                    }
                } while (choice != 4);
                return;
            }
        }
    }
    printf("Invalid account number or password.\n");
}
void deposit(int index)
{
    float amount;

    if (index < 0 || index >= accountCount) {
        printf("Invalid account selection.\n");
        return;
    }

    printf("Enter amount to deposit: ");

    if (scanf("%f", &amount) != 1) {
        printf("Invalid input. Please enter a number.\n");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        return;
    }

    if (amount <= 0.0f) {
        printf("Deposit amount must be greater than zero.\n");
        return;
    }

    accounts[index].balance += amount;
    saveData();

    printf("Amount deposited successfully. Current balance: %.2f\n",
           accounts[index].balance);
}

void withdraw(int index) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    if (amount > accounts[index].balance) {
        printf("Insufficient balance! Your current balance is %.2f.\n", accounts[index].balance);
        printf("Please enter an amount less than or equal to your balance.\n");
        return;
    }
    accounts[index].balance -= amount;
    saveData(); 
    printf("Amount withdrawn successfully. Current balance: %.2f\n", accounts[index].balance);
}

void checkBalance(int index) {
    printf("Current balance: %.2f\n", accounts[index].balance);
}

void adminMenu() {
    char adminUsername[20] = "admin";
    char adminPassword[20] = "admin123";
    char username[20], password[20];

    printf("Enter admin username: ");
    scanf("%s", username);
    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        printf("Admin login successfully!\n");
        int choice;
        do {
            printf("\n1. View All Accounts\n2. Search Account\n3. Export Data\n4. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    viewAllAccounts();
                    break;
                case 2:
                    searchAccount();
                    break;
                case 3:
                    exportData();
                    break;
                case 4:
                    printf("Admin logged out successfully.\n");
                    return;
                default:
                    printf("Invalid choice.\n");
            }
        } while (choice != 4);
    } else {
        printf("Invalid .\n");
    }
}

void viewAllAccounts()
{
    printf("\nAccount Details:\n");
    for (int i = 0; i < accountCount; i++)
    {
        printf("Account Number: %d, Name: %s, Balance: %.2f, Aadhar: %s, IFSC: %s\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].balance,
               accounts[i].aadhar, accounts[i].ifsc);
    }
}

void searchAccount()
{
    int accountNumber;
    printf("Enter account number to search: ");
    scanf("%d", &accountNumber);
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            printf("Account found: Name: %s, Balance: %.2f, Aadhar: %s, IFSC: %s\n",
                   accounts[i].name, accounts[i].balance, accounts[i].aadhar, accounts[i].ifsc);
            return;
        }
    }
    printf("Account not found.\n");
}

void exportData()
{
    FILE *file = fopen("bank_data.csv", "w");
    if (file == NULL)
    {
        printf("Error exporting data.\n");
        return;
    }
    fprintf(file, "Account Number,Name,Encrypted Password,Aadhar,IFSC\n");
    for (int i = 0; i < accountCount; i++)
    {
        fprintf(file, "%d,%s,%s,%s,%s\n",
                accounts[i].accountNumber, accounts[i].name, accounts[i].password,
                accounts[i].aadhar, accounts[i].ifsc);
    }
    fclose(file);
    printf("Data exported successfully to bank_data.csv.\n");
}