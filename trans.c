// Enhanced Bank Account Management System using Random Access Files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

unsigned int enterChoice(void);

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

/* EXTRA FEATURES */
void searchRecord(FILE *fPtr);
void displayAllRecords(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void totalBalance(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File does not exist. Creating file...\n");

        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            exit(1);
        }

        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
        }

        rewind(cfPtr);
    }

    while ((choice = enterChoice()) != 10)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            searchRecord(cfPtr);
            break;

        case 6:
            displayAllRecords(cfPtr);
            break;

        case 7:
            depositMoney(cfPtr);
            break;

        case 8:
            withdrawMoney(cfPtr);
            break;

        case 9:
            totalBalance(cfPtr);
            break;

        default:
            puts("Invalid choice.");
            break;
        }
    }

    fclose(cfPtr);

    printf("\nProgram terminated successfully.\n");

    return 0;
}

/* MENU */
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n==============================");
    printf("\n BANK MANAGEMENT SYSTEM");
    printf("\n==============================");

    printf("\n1  - Store formatted text file");
    printf("\n2  - Update account");
    printf("\n3  - Add new account");
    printf("\n4  - Delete account");
    printf("\n5  - Search account");
    printf("\n6  - Display all accounts");
    printf("\n7  - Deposit money");
    printf("\n8  - Withdraw money");
    printf("\n9  - Display total bank balance");
    printf("\n10 - Exit");

    printf("\n\nEnter your choice: ");
    scanf("%u", &menuChoice);

    return menuChoice;
}

/* CREATE TEXT FILE */
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;

    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        while ((result = fread(&client,
                               sizeof(struct clientData),
                               1,
                               readPtr)) == 1)
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("accounts.txt file created successfully.\n");
    }
}

/* UPDATE RECORD */
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        printf("\nCurrent Balance: %.2f\n", client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Updated Balance: %.2f\n", client.balance);
    }
}

/* ADD NEW RECORD */
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter lastname firstname balance:\n");

        scanf("%14s%9s%lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account created successfully.\n");
    }
}

/* DELETE RECORD */
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

/* SEARCH RECORD */
void searchRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("Enter account number to search: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found\n");

        printf("Account No : %u\n", client.acctNum);
        printf("Last Name : %s\n", client.lastName);
        printf("First Name: %s\n", client.firstName);
        printf("Balance   : %.2f\n", client.balance);
    }
}

/* DISPLAY ALL RECORDS */
void displayAllRecords(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s %-15s %-10s %-10s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-6u %-15s %-10s %-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

/* DEPOSIT MONEY */
void depositMoney(FILE *fPtr)
{
    unsigned int account;
    double amount;

    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Enter deposit amount: ");
        scanf("%lf", &amount);

        client.balance += amount;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Deposit successful.\n");
        printf("New Balance: %.2f\n", client.balance);
    }
}

/* WITHDRAW MONEY */
void withdrawMoney(FILE *fPtr)
{
    unsigned int account;
    double amount;

    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Enter withdraw amount: ");
        scanf("%lf", &amount);

        if (amount > client.balance)
        {
            printf("Insufficient balance.\n");
        }
        else
        {
            client.balance -= amount;

            fseek(fPtr,
                  -sizeof(struct clientData),
                  SEEK_CUR);

            fwrite(&client,
                   sizeof(struct clientData),
                   1,
                   fPtr);

            printf("Withdrawal successful.\n");
            printf("Remaining Balance: %.2f\n",
                   client.balance);
        }
    }
}

/* TOTAL BANK BALANCE */
void totalBalance(FILE *fPtr)
{
    struct clientData client;

    double total = 0.0;

    rewind(fPtr);

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            total += client.balance;
        }
    }

    printf("\nTotal Bank Balance = %.2f\n", total);
}