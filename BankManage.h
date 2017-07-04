#ifndef BANKMANAGE_H_
#define BANKMANAGE_H_

int BankMoney = 100000000;

typedef struct BankOp
{
	int totCustomerCount;
	int onServiceCustomer;
	int serviceCompleteCustomer;
	int rejectedServiceCustomer;
	int averageWaitTime;
	int averageServiceTime;
	int simulServiceTime;
	int enterCustomerCount;
	int stamplerCount;
	int copyMachineCount;
	int staplerCount;
}BankOp;

void bankOPInit(BankOp * op)
{
	op->totCustomerCount = 0;
	op->onServiceCustomer = 0;
	op->rejectedServiceCustomer = 0;
	op->averageWaitTime = 0;
	op->averageServiceTime = 0;
	op->simulServiceTime = 0;
	op->enterCustomerCount = 0;
	op->stamplerCount = 0;
	op->copyMachineCount = 0;
	op->staplerCount = 0;
	op->serviceCompleteCustomer = 0;
}

#endif /* BANKMANAGE_H_ */
