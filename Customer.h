#ifndef CUSTOMER_H_
#define CUSTOMER_H_


#define DEPOSIT 10
#define WITHDRAW 11
#define TRANSFER 12

#define YES 12
#define NO 13

typedef struct Cust
{
	int waitTime;
	int takeServiceTime;
	int cusNumber;
	int flag;
}Cust;


#endif /* CUSTOMER_H_ */
