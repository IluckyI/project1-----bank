#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdbool>
#include <cstring>
#include <cstdlib>

static const int MAX_ACCOUNT_NUM = 100;
static const char* FILE_PATH = "credit.dat";
static const char* Admin_PATH = "admin.dat";

enum
{
	INVALID_ACCOUNT_NO = 0,
};

typedef struct _admin
{
	char adminName[20];
	int password;
}admin,*adminPtr;

typedef struct _User
{
	int acctNum;
	int password;
	char name[20];
	double balance;
}UserData,*UserPtr;

static UserData s_clients[MAX_ACCOUNT_NUM] = { 0 };
enum loginname
{
	ADMIN=1,
	USER,
	LOGEXIT,
};
enum Userexe
{
	USERWRITE=1,
	USERUPDATE,
	USEROUTPUT,
	USERFIND,
	USEREXIT,
};
enum adminexe
{
	ADMWRITE=1,
	ADMUPDATE,
	ADMNEW,
	ADMDELETE,
	ADMFIND,
	ADMOUTPUT,
	ADMEXIT,
};
enum OperatorType
{
	INSERT,
	DELETE,
	UPDATE,
};

int	 updateData(OperatorType type, UserData* data);
int enterlogin(void);
int  enterChoice(void);
void textFile(FILE *);
void updateRecord(FILE *);
void newRecord(FILE *);
void deleteRecord(FILE *);
void initialize(FILE *);
int  recordIndex(int);
void userUpdateRecord(FILE*file,UserPtr user );
int enterPassword(void);
int enterUserAcc(void);
void outputRecord(UserPtr);
int enterUser(void);
char * enternum(void);
void findRecord(FILE*,int);

void outputInfo(FILE*);

bool fileExist(const char* file)
{
	FILE *pf = fopen(file, "r");
	if (pf == 0)
		return false;
	fclose(pf);
	return true;
}

static FILE *cfPtr = 0;
static FILE *admfPtr = 0;
static FILE *ufPtr = 0;
int main()
{
	admin *adminacc = 0;
	adminacc = (adminPtr) malloc(sizeof(admin));
	if (!fileExist(Admin_PATH))
	{
		adminPtr adm= (adminPtr)malloc(sizeof(admin));
		admfPtr = fopen(Admin_PATH, "w+");
		strcpy(adm->adminName, "admin");
		adm->password = 666666;
		fwrite(adm,sizeof(admin),1,admfPtr);
		free(adm);
	}
	else
	{
		admfPtr= fopen(Admin_PATH, "r+");
		fread(adminacc,sizeof(admin),1,admfPtr);

	}
	//对文件是否存在分情况处理
	if (!fileExist(FILE_PATH))
	{
		cfPtr = fopen(FILE_PATH, "w+");
	}
	else
	{
		cfPtr = fopen(FILE_PATH, "r+");//实验：先覆盖，后改用（r+）
	}

	if (cfPtr == 0)
	{
		perror("create file:");
		return -1;
	}

	initialize(cfPtr);
	fclose(cfPtr);
	int login;
	while ((login = enterlogin()) != LOGEXIT)
	{
		switch (login)
		{
			case ADMIN:
			{
				cfPtr = fopen(FILE_PATH, "r+");//应改为r+
				initialize(cfPtr);
				char *accnump = 0;
				accnump = enternum();
				if (strcmp(accnump, adminacc->adminName))
				{
					printf("|  Don't have this account!\n");
					break;
				}
				int password = enterPassword();
				if (password != adminacc->password)
				{
					printf("|  The password is error!\n");
					break;
				}
				printf("---------------------------------------------------\n");
				int choice;
				while ((choice = enterChoice()) != ADMEXIT)
				{
					int acctnum = 0;
					switch (choice)
					{
					case ADMWRITE:
						textFile(cfPtr);
						break;
					case ADMUPDATE:
						updateRecord(cfPtr);
						break;
					case ADMNEW:
						newRecord(cfPtr);
						break;
					case ADMDELETE:
						deleteRecord(cfPtr);
						break;
					case ADMFIND:
						printf("Please input you find information\n");
						scanf("%d", &acctnum);
						findRecord(cfPtr,acctnum);
						break;
					case ADMOUTPUT:
						outputInfo(cfPtr);
						break;
					}
				}
				fclose(cfPtr);
				break;
			}

			case USER:
			{
				UserPtr user=(UserPtr)malloc(sizeof(UserData));
				int acc = 0;
				ufPtr = fopen(FILE_PATH, "r+");//应改为r+
			//	initialize(ufPtr);
				acc=enterUserAcc();
				int i = 0;
				fseek(ufPtr, 0, SEEK_SET);
				while (!feof(ufPtr))
				{
					size_t ret = fread(user, sizeof(UserData), 1,
						ufPtr);
					if (ret != 1)
					{
						break;
					}
					//fread(user, sizeof(UserData), 1, ufPtr);
					if (acc == user->acctNum)
					{
						break;
					}
					fseek(ufPtr, i * sizeof(UserData), SEEK_SET);
					i++;
				}
				if (feof(ufPtr))
				{
					printf("Don't have this account!\n");
					break;
				}
				int password; //= enterPassword();
				int j;
				for ( j= 0; j < 3; j++)
				{		
					password = enterPassword();
					if (password == user->password)
						break;
					printf("this password is error!\n");
					
					
				}
				if (j== 3)
				{
					printf("input three error password!\n");
					break;
				}
				FILE *file = ufPtr;
				int choice;
				while ((choice = enterUser()) != USEREXIT)
				{

					switch (choice)
					{
					case USERWRITE:
						textFile(ufPtr);
						break;
					case USERUPDATE:
						userUpdateRecord(ufPtr,user);
						break;
					case USEROUTPUT:
						outputRecord(user);
						break;
					case USERFIND:
						int acctnum = 0;
						printf("Please input you find information\n");
						scanf("%d", &acctnum);
						findRecord(file,acctnum);
						break;					
					}
				}
				free(user);
				break;
			}
		}
	}
	fclose(cfPtr);

	fclose(admfPtr);
	free(adminacc);
	return 0;
}

void initialize(FILE *file)
{
	memset(s_clients, 0, MAX_ACCOUNT_NUM * sizeof(UserData));
	int count = 0;
	while (!feof(file) && count < MAX_ACCOUNT_NUM)
	{
		size_t ret = fread(&s_clients[count], sizeof(UserData), 1, file);
		if (ret != 1)
		{
			break;
		}
		count++;
	}
}

int recordIndex(int account)
{
	for (int i = 0; i < MAX_ACCOUNT_NUM; i++)
	{
		if (s_clients[i].acctNum == account)
		{
			return i;
		}
	}
	return  -1;
}

void textFile(FILE *readPtr)
{
	FILE *writePtr = 0;
	UserData client = { 0, 0, "", 0.0 };

	if ((writePtr = fopen("accounts.txt", "w+")) == NULL)//同cfptr一样，后改用w
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
			"Acct", "password", "Name", "Balance");

		while (!feof(readPtr))
		{
			size_t ret = fread(&client, sizeof(UserData), 1,
				readPtr);
			if (ret != 1)
			{
				break;
			}

			if (client.acctNum != INVALID_ACCOUNT_NO)
				fprintf(writePtr, "%-6d%-16d%-11s%10.2lf\n",
					client.acctNum, client.password,
					client.name, client.balance);
		}

		fclose(writePtr);
	}

}

void updateRecord(FILE *fPtr)
{
	int account;

	printf("Enter account to update ( 1 - 100 ): ");
	scanf("%d", &account);

	int index = recordIndex(account);
	if (-1 == index)
	{
		printf("Acount #%d has no information.\n", account);
		return;
	}

	UserData data = s_clients[index];

	printf("%-6d%-25s%10.2f\n\n",
		data.acctNum, data.name,
		data.balance);
	printf("Enter charge ( + ) or payment ( - ): ");
	double transaction;
	scanf("%lf", &transaction);
	data.balance += transaction;
	printf("%-6d%-25s% 10.2f\n",
		data.acctNum, data.name,
		data.balance);

	updateData(UPDATE, &data);
}

void deleteRecord(FILE *fPtr)
{
	int accountNum;

	printf("Enter account number to "
		"delete ( 1 - 100 ): ");
	scanf("%d", &accountNum);

	UserData 	blankClient = { INVALID_ACCOUNT_NO, 0, "", 0 };
	blankClient.acctNum = accountNum;

	updateData(DELETE, &blankClient);

}

void newRecord(FILE *fPtr)
{
	UserData client = { 0, 0, "", 0.0 };
	int accountNum;
	printf("Enter new account number ( 1 - 100 ): ");
	scanf("%d", &accountNum);
	//找到该记录在列表中的索引
	int index = recordIndex(accountNum);
	if (-1 != index)//如果该记录已经存在
	{
		printf("Account #%d already contains information.\n",
			client.acctNum);
		return;
	}
	printf("Enter password,name, balance\n? ");
	scanf("%d%s%lf",&client.password,&client.name,
		&client.balance);
	client.acctNum = accountNum;

	updateData(INSERT, &client);
}

int updateData(OperatorType type, UserData * data)
{
	int index = recordIndex(type == INSERT ? INVALID_ACCOUNT_NO : data->acctNum);
	if (-1 == index)
	{
		printf("Account %d does not valid.\n", data->acctNum);
		return -1;
	}

	switch (type)
	{
	case DELETE:
	{
		//更新文件
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		data->acctNum = INVALID_ACCOUNT_NO;
		fwrite(data,
			sizeof(UserData), 1, cfPtr);
		//更新内存数据
		s_clients[index].acctNum = INVALID_ACCOUNT_NO;
	}
	break;
	case UPDATE:
	{
		//更新数据
		s_clients[index].balance = data->balance;//更新数据缓冲
		fseek(cfPtr,
			index * sizeof(UserData),
			SEEK_SET);
		fwrite(&s_clients[index], sizeof(UserData), 1,
			cfPtr);//更新文件

	}
	break;
	case INSERT:
	{
		//更新数据记录文件
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		fwrite(data, sizeof(UserData), 1, cfPtr);
		//更新缓冲区数据	
		s_clients[index] = *data;
	}
	break;
	default:
		break;
	}

	return 0;
}

int enterChoice(void)
{
	int menuChoice;

	printf("                  ***************************\n"
		   "                  *     Enter your choice   *\n" 
		   "                  * 1 - save information    *\n"
		   "                  * 2 - update an account   *\n"
		   "                  * 3 - add a new account   *\n"
		   "                  * 4 - delete an account   *\n"
		   "                  * 5 - find account        *\n"
		   "                  * 6 - output all info     *\n"
		   "                  * 7 - end program?        *\n"
	       "                  ***************************\n");
	scanf("%d", &menuChoice);
	return menuChoice;
}
int enterlogin(void)
{
	int login;
	printf("---------------------------------------------------\n");
	printf("Enter your log in(admin :1 or User :2 or exit :3\n");
	scanf("%d", &login);
	return login;
}

int enterUserAcc(void)
{
	int accnum;
	printf("Please input your accNum\n");
	scanf("%d", &accnum);
	return accnum;
}
int enterPassword(void)
{ 
	int password;
	printf("Please input password\n");
	scanf("%d", &password);
	return password;
}
int enterUser(void)
{
	int menuChoice;

	printf("                  ***************************\n"
		   "                  *     Enter your choice   *\n" 
		   "                  * 1 - save information    *\n"
		   "                  * 2 - update information  *\n"
		   "                  * 3 - output information  *\n"
		   "                  * 4 - find acctnum info   *\n"
		   "                  * 5 - end program?        *\n"
		   "                  ***************************\n");
	scanf("%d", &menuChoice);
	return menuChoice;
}
char * enternum()
{
	char num[10];
	printf("Please input admin name\n");
	scanf("%s", num);
	return num;
}

void outputRecord(UserPtr user)
{   
	printf("%-6s  %-16s%10s\n", "acctnum",  "name", "balance");
	printf("%-6d  %-16s%10.2lf\n", user->acctNum, user->name, user->balance);

}
void userUpdateRecord(FILE*file,UserPtr user)
{
	getchar();
	char a=0;
	printf("change your password?(Y/N)\n");
	scanf("%c", &a);
	if (a == 'N' || a == 'n')
		return;
    int Newinfo;
	printf("Please input New password:\n");	
	scanf("%d", &Newinfo);
	//data.balance += transaction;
	user->password = Newinfo;
	printf("your New password is %d", user->password);
	fseek(file,-sizeof(UserData),SEEK_CUR);
	fwrite(user,sizeof(UserData),1,file);
//	updateData(UPDATE, user);
}


void findRecord(FILE*file,int acctNum)
{
	UserPtr data = (UserPtr)malloc(sizeof(UserData));
	fseek(file,0,SEEK_SET);

	while (!feof(file))
	{
		fread(data, sizeof(UserData), 1, file);
		if (data->acctNum == acctNum)
		{
			printf("%-6s  %-16s%10s\n", "acctnum", "name", "balance");
			printf("%-6d  %-16s%10.2lf\n", data->acctNum, data->name, data->balance);
			free(data);
			data = 0;
			return;
		}
//		fseek(file, sizeof(UserData), SEEK_CUR);

	}
	printf("Don't have this acctNum!\n");
	free(data);
	data = 0;
}
void outputInfo(FILE*file)
{
	printf("%-6s  %-16s%10s\n", "acctnum", "name", "balance");
	UserPtr data=(UserPtr)malloc(sizeof(UserData));
	fseek(file,0,SEEK_SET);
	while (!feof(file))
	{

		size_t ret =fread(data, sizeof(UserData), 1, file);		
		if (ret != 1)
		{
			break;
		}
		
		printf("%-6d  %-16s%10.2lf\n", data->acctNum, data->name, data->balance);
	}
}