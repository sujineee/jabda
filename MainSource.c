#include <pthread.h>
#include "SharedData.h"
#include "BankManage.h"
#include "Customer.h"
#include "Queue.h"
#include "MyRand.h"
#include <time.h>
#include <stdlib.h>

void * genCustomer(void * arg);
void * genCustomerTwo(void * arg);
void * DepositWithdrawOne(void * arg);
void * DepositWithdrawTwo(void * arg);
void * Transfer(void * arg);

int waitCheck(int wait);

// 뮤텍스 변수 생성

// thread design
// 입출금 스레드 2개
// 계좌 이체 스레드
//
// queue design

// myrand 함수 추가
QueueType qCust;
BankOp bOp;
pthread_mutex_t sMutex;
int timeVal = 0;
int customerNo = 0;

int main(void)
{
	int i;
	pthread_t threadId[5];
	struct timeval start, end;
	double opTime;

	gettimeofday(&start, NULL);
	bankOPInit(&bOp);

	pthread_mutex_init(&sMutex, NULL);
    //멀티스레딩-5개 스레드 생성
	pthread_create(&(threadId[0]), NULL, genCustomer, NULL);
	pthread_create(&(threadId[1]), NULL, genCustomerTwo, NULL);

	pthread_create(&(threadId[2]), NULL, DepositWithdrawOne, NULL);
	pthread_create(&(threadId[3]), NULL, DepositWithdrawTwo, NULL);

	pthread_create(&(threadId[4]), NULL, Transfer, NULL);

	for(i = 0; i < 5; i++)
		pthread_join(threadId[i], NULL);

	pthread_mutex_destroy(&sMutex);

	printf("\nTotal Customer Count : %d\n", bOp.totCustomerCount);
	printf("On Service Customer Count : %d\n", bOp.serviceCompleteCustomer);
	printf("Rejected Service Customer Count : %d\n", bOp.totCustomerCount - bOp.serviceCompleteCustomer);
	printf("Average wait time : %d\n", bOp.averageWaitTime / bOp.serviceCompleteCustomer);
	printf("Average service time : %d\n", bOp.averageServiceTime / bOp.serviceCompleteCustomer);
	printf("stampler Count: %d\n", bOp.stamplerCount);
	printf("copy machine Count : %d\n", bOp.copyMachineCount);
	printf("stapler Count: %d\n", bOp.staplerCount);

	gettimeofday(&end, NULL);
	opTime = (double)(end.tv_sec) + (double)(end.tv_usec)/1000000.0 - (double)(start.tv_sec) - (double)(start.tv_usec)/1000000.0;
	printf("simul time : %f\n", opTime);
	printf("Service End\n");
	return 0;
}

int waitCheck(int wait)
{
	Cust temp = peek(&qCust);
	if(temp.flag == 0)
	{
		return 1; // 큐가 비었을 때
	}

	if(wait > temp.takeServiceTime)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

// 서비스 받은 고객 수가 십만명이면 스레드 종료
// 고객 대기 시간 wait에 정하고
// 입장 하는 고객 수 enterCount에 정함
// 입장 하는 고객들의 서비스 받는 시간을 service에 정해줌
// 고객 구조체의 각 필드에 해당하는 값을 할당
void * genCustomer(void * arg) // 입장 쓰레드
{
	int wait = 0;
	int enterCount = 0;
	int service = 0;
	int ret = 0;
	int i = 0;

	while(1)
	{

		if(bOp.serviceCompleteCustomer >= 100000)
		{
			printf("serviceCom customer : %d\n", bOp.serviceCompleteCustomer);
			break;
		}

		wait = (int)((MyRand() * 3) + 1);       //
		enterCount = (int)((MyRand() * 6) + 1); //
		service = (int)((MyRand() * 7) + 4);    //

		Cust * customer[enterCount];

		pthread_mutex_lock(&sMutex);


		for(i = 0; i < enterCount; i++)
		{
			customer[i] = (Cust *)malloc(sizeof(Cust));
			customer[i]->waitTime = wait;
			customer[i]->takeServiceTime = service;
			customer[i]->cusNumber = ++customerNo;
			customer[i]->flag = 1;
		}

		ret = waitCheck(wait);

		if(ret < 0)//time-out 지나면 고객삭제
		{
			for(i = 0; i < enterCount; i++)
				free(customer[i]);
			bOp.rejectedServiceCustomer += enterCount;
			bOp.totCustomerCount += enterCount;

		}
		else
		{
			for(i = 0; i < enterCount; i++)
				enqueue(&qCust, *(customer[i]));

			bOp.enterCustomerCount += enterCount;
			bOp.totCustomerCount += enterCount;

		}
		pthread_mutex_unlock(&sMutex);
	}
	return NULL;
}



void * genCustomerTwo(void * arg)
{
	int wait = 0;
	int enterCount = 0;
	int service = 0;
	int ret = 0;
	int i = 0;

	while(1)
	{
		if(bOp.serviceCompleteCustomer >= 100000)
		{
			printf("serviceCom customer : %d\n", bOp.serviceCompleteCustomer);
			break;
		}

		wait = (int)((MyRand() * 5) + 1);       // 1 ~ 4
		enterCount = (int)((MyRand() * 6) + 1); // 1 ~ 6
		service = (int)((MyRand() * 4) + 1);    // 1 ~ 3

		Cust * customer[enterCount];

		pthread_mutex_lock(&sMutex);

		for(i = 0; i < enterCount; i++)
		{
			customer[i] = (Cust *)malloc(sizeof(Cust));
			customer[i]->waitTime = wait;
			customer[i]->takeServiceTime = service;
			customer[i]->cusNumber = ++customerNo;
			customer[i]->flag = 1;
		}

		ret = waitCheck(wait);
		if(ret < 0)
		{
			for(i = 0; i < enterCount; i++)
				free(customer[i]);
			bOp.rejectedServiceCustomer += enterCount;
			bOp.totCustomerCount += enterCount;

		}
		else
		{
			for(i = 0; i < enterCount; i++)
				enqueue(&qCust, *(customer[i]));

			bOp.enterCustomerCount += enterCount;
			bOp.totCustomerCount += enterCount;
		}
		pthread_mutex_unlock(&sMutex);
	}
	return NULL;
}

// 십만명이 서비스받으면 스레드 종료
// 공유 자원 사용 시 뮤텍스 락 설정
void * DepositWithdrawOne(void * arg) // 서비스
{
	Cust temp;
	int randVal = 0;

	while(1)
	{
		if(bOp.serviceCompleteCustomer >= 100000)
		{
			printf("serviceCom customer : %d\n", bOp.serviceCompleteCustomer);
			break;
		}

		pthread_mutex_lock(&sMutex);//mutex-사무용품 분배

		temp = dequeue(&qCust);//FCFS 대기큐에서 창구로 순서대로 넘기기

		if(temp.cusNumber == 0)
		{
			// printf("waiting customer..\n");
		}
		else
		{
			randVal = rand() % 3;
			switch(randVal)//사무용품처리
			{
				case 0:
					bOp.copyMachineCount++;
					break;
				case 1:
					bOp.stamplerCount++;
					break;
				case 2:
					bOp.staplerCount++;
					break;
			}
			printf("at %d customer %d arrived \n", timeVal, temp.cusNumber);
			printf("at %d customer %d service start \n", timeVal, temp.cusNumber);
			BankMoney += 100;
			timeVal += temp.takeServiceTime;
			bOp.averageServiceTime += temp.takeServiceTime;
			bOp.averageWaitTime += temp.waitTime;
			printf("at %d customer %d service complete \n", timeVal, temp.cusNumber);
			bOp.serviceCompleteCustomer++;

		}
		pthread_mutex_unlock(&sMutex);
	}
	return NULL;
}

void * DepositWithdrawTwo(void * arg)
{
	Cust temp;
	int randVal = 0;
		while(1)
		{
			if(bOp.serviceCompleteCustomer >= 100000)
			{
				printf("serviceCom customer : %d\n", bOp.serviceCompleteCustomer);
				break;
			}

			pthread_mutex_lock(&sMutex);

			temp = dequeue(&qCust);

			if(temp.cusNumber == 0)
			{
				// printf("waiting customer..\n");
			}
			else
			{
				randVal = rand() % 3;
				switch(randVal)
				{
					case 0:
						bOp.copyMachineCount++;
						break;
					case 1:
						bOp.stamplerCount++;
						break;
					case 2:
						bOp.staplerCount++;
						break;
				}

				printf("at %d customer %d arrived \n", timeVal, temp.cusNumber);
				printf("at %d customer %d service start \n", timeVal, temp.cusNumber);
				BankMoney += 100;
				timeVal += temp.takeServiceTime;
				printf("at %d customer %d service complete \n", timeVal, temp.cusNumber);
				bOp.averageServiceTime += temp.takeServiceTime;
				bOp.averageWaitTime += temp.waitTime;
				bOp.serviceCompleteCustomer++;
			}
			pthread_mutex_unlock(&sMutex);
		}
		return NULL;
}

void * Transfer(void * arg)
{
	Cust temp;
	int randVal = 0;
		while(1)
		{
			if(bOp.serviceCompleteCustomer >= 100000)
			{
				printf("serviceCom customer : %d\n", bOp.serviceCompleteCustomer);
				break;
			}

			pthread_mutex_lock(&sMutex);

			temp = dequeue(&qCust);
			if(temp.cusNumber == 0)
			{
				// printf("waiting customer..\n");
			}
			else
			{
				randVal = rand() % 3;
				switch(randVal)
				{
					case 0:
						bOp.copyMachineCount++;
						break;
					case 1:
						bOp.stamplerCount++;
						break;
					case 2:
						bOp.staplerCount++;
						break;
				}
				printf("at %d customer %d arrived \n", timeVal, temp.cusNumber);
				printf("at %d customer %d service start \n", timeVal, temp.cusNumber);
				BankMoney += 100;
				timeVal += temp.takeServiceTime;
				printf("at %d customer %d service complete \n", timeVal, temp.cusNumber);
				bOp.averageServiceTime += temp.takeServiceTime;
				bOp.averageWaitTime += temp.waitTime;
				bOp.serviceCompleteCustomer++;
			}
			pthread_mutex_unlock(&sMutex);
		}
		return NULL;
}
