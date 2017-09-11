///* Fig. 11.16: fig11_16.c
//This program reads a random access file sequentially,
//updates data already written to the file, creates new
//data to be placed in the file, and deletes data
//already in the file.             */
//
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//struct clientData {
//	int acctNum;
//	char name;
//	int month;
//	int day;
//    double account;
//	
//};
//
//int enterChoice(void);
//void textFile(FILE *);
//void updateRecord(FILE *);
//void newRecord(FILE *);
//void deleteRecord(FILE *);
//
//int main()
//{
//	FILE *cfPtr;
//	int choice;
//
//	if ((cfPtr = fopen("credit.dat", "r+")) == NULL)
//		printf("File could not be opened.\n");
//	else {
//
//		while ((choice = enterChoice()) != 5) {
//
//			switch (choice) {
//			case 1:
//				textFile(cfPtr);
//				break;
//			case 2:
//				updateRecord(cfPtr);
//				break;
//			case 3:
//				newRecord(cfPtr);
//				break;
//			case 4:
//				deleteRecord(cfPtr);
//				break;
//			}
//		}
//
//		fclose(cfPtr);
//	}
//
//	return 0;
//}
//
//void textFile(FILE *readPtr)
//{
//	FILE *writePtr;
//	struct clientData client = { 0, 0, 0, 0.0 };
//
//	if ((writePtr = fopen("accounts.txt", "w")) == NULL)
//		printf("File could not be opened.\n");
//	else {
//		rewind(readPtr);
//		fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
//			"Acct", "name", "account", "month.day");
//
//		while (!feof(readPtr)) {
//			fread(&client, sizeof(struct clientData), 1,
//				readPtr);
//
//			if (client.acctNum != 0)
//				fprintf(writePtr, "%-6d%-16s%-11.2d%10d.%d\n",
//					client.acctNum, client.name,
//					client.account, client.month,client.day);
//		}
//
//		fclose(writePtr);
//	}
//
//}
//
//void updateRecord(FILE *fPtr)
//{
//	int account;
//	double transaction;
//	struct clientData client = { 0, 0, 0, 0.0 };
//
//	printf("Enter account to update ( 1 - 100 ): ");
//	scanf("%d", &account);
//	fseek(fPtr,
//		(account - 1) * sizeof(struct clientData),
//		SEEK_SET);
//	fread(&client, sizeof(struct clientData), 1, fPtr);
//
//	if (client.acctNum == 0)
//		printf("Acount #%d has no information.\n", account);
//	else {
//		printf("%-6d%-16s%-11.2s%10d.%d\n\n",
//			client.acctNum, client.name,
//			client.account, client.month,client.day);
//		printf("Enter charge ( + ) or payment ( - ): ");
//		scanf("%lf", &transaction);
//		client.balance += transaction;
//		printf("%-6d%-16s%-11.2s%10d.%d\n\n",
//			client.acctNum, client.name,
//			client.account, client.month, client.day);
//		fseek(fPtr,
//			(account - 1) * sizeof(struct clientData),
//			SEEK_SET);
//		fwrite(&client, sizeof(struct clientData), 1,
//			fPtr);
//	}
//}
//
//void deleteRecord(FILE *fPtr)
//{
//	struct clientData client,
//		blankClient = { 0, 0,0, 0, 0 };
//	int accountNum;
//
//	printf("Enter account number to "
//		"delete ( 1 - 100 ): ");
//	scanf("%d", &accountNum);
//	fseek(fPtr,
//		(accountNum - 1) * sizeof(struct clientData),
//		SEEK_SET);
//	fread(&client, sizeof(struct clientData), 1, fPtr);
//
//	if (client.acctNum == 0)
//		printf("Account %d does not exist.\n", accountNum);
//	else {
//		fseek(fPtr,
//			(accountNum - 1) * sizeof(struct clientData),
//			SEEK_SET);
//		fwrite(&blankClient,
//			sizeof(struct clientData), 1, fPtr);
//	}
//}
//
//void newRecord(FILE *fPtr)
//{
//	struct clientData client = { 0, 0,0, 0,0};
//	int accountNum;
//	printf("Enter new account number ( 1 - 100 ): ");
//	scanf("%d", &accountNum);
//	fseek(fPtr,
//		(accountNum - 1) * sizeof(struct clientData),
//		SEEK_SET);
//	fread(&client, sizeof(struct clientData), 1, fPtr);
//
//	if (client.acctNum != 0)
//		printf("Account #%d already contains information.\n",
//			client.acctNum);
//	else {
//		printf("Enter lastname, firstname, balance\n? ");
//		scanf("%s%s%lf", &client.lastName, &client.firstName,
//			&client.balance);
//		client.acctNum = accountNum;
//		fseek(fPtr, (client.acctNum - 1) *
//			sizeof(struct clientData), SEEK_SET);
//		fwrite(&client,
//			sizeof(struct clientData), 1, fPtr);
//	}
//}
//
//int enterChoice(void)
//{
//	int menuChoice;
//
//	printf("\nEnter your choice\n"
//		"1 - store a formatted text file of acounts called\n"
//		"    \"accounts.txt\" for printing\n"
//		"2 - update an account\n"
//		"3 - add a new account\n"
//		"4 - delete an account\n"
//		"5 - end program\n? ");
//	scanf("%d", &menuChoice);
//	return menuChoice;
//}
//
//
//
///**************************************************************************
//* (C) Copyright 2000 by Deitel & Associates, Inc. and Prentice Hall.     *
//* All Rights Reserved.                                                   *
//*                                                                        *
//* DISCLAIMER: The authors and publisher of this book have used their     *
//* best efforts in preparing the book. These efforts include the          *
//* development, research, and testing of the theories and programs        *
//* to determine their effectiveness. The authors and publisher make       *
//* no warranty of any kind, expressed or implied, with regard to these    *
//* programs or to the documentation contained in these books. The authors *
//* and publisher shall not be liable in any event for incidental or       *
//* consequential damages in connection with, or arising out of, the       *
//* furnishing, performance, or use of these programs.                     *
//*************************************************************************/
