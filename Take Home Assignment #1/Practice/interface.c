#ifndef INTERFACE_FILE_H
#define INTERFACE_FILE_H
//Defines the ADT of a bank account
typedef struct{
char accountNumber[20];
Int accountBalance;
int atmPin;
int chequeBookRequested:
}bankAccount;
//Returns the account balance of the given bank account number.
extern int getAccountBalance(bankAccount* bA);
/* Requests for a new cheque book for the given bank account number.
 Returns 1 if request is successful; 0 otherwise. */
extern int requestChequeBook(bankAccount* bA);
/* Changes the ATM pin for the given account number from oldATMPin to
newATMPin. Returns newATMPin if successful and oldATMPin otherwise. */
extern int changeATMPin(bankAccount* bA, int oldATMPin, int newATMPin);
#endif /*INTERFACE_FILE_H*