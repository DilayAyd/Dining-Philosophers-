#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

//5tane filazof tanımladık
#define NUMBER_OF_PHILOSOPHERS 5

//filazoflar yemek yer, düşünür, çatal alır ve çatal bırakır
void *philosopher(void *);
void think(int);
void pickUp(int);
void eat(int);
void putDown(int);


pthread_mutex_t chopsticks[NUMBER_OF_PHILOSOPHERS];
pthread_t philosophers[NUMBER_OF_PHILOSOPHERS];
pthread_attr_t attributes[NUMBER_OF_PHILOSOPHERS];

int main() {
	int i;
	srand(time(NULL));
	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_mutex_init(&chopsticks[i], NULL);
	}

	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_attr_init(&attributes[i]);
	}
	
	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_create(&philosophers[i], &attributes[i], philosopher, (void *)(i));
	}

	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}

//filazof fonksiyonu içinde yapılacak işlemleri tanımlıyoruz.
void *philosopher(void *philosopherNumber) {
	while (1) {
		think(philosopherNumber);
		pickUp(philosopherNumber);
		eat(philosopherNumber);
		putDown(philosopherNumber);
	}
}

//filazof düşünür
void think(int philosopherNumber) {
	int sleepTime = rand() % 3 + 1;
	printf(" %d . Filazof   dusunuyor\n", philosopherNumber, sleepTime);
	sleep(sleepTime);
}

void pickUp(int philosopherNumber) {

	//sağ çatal numarası filazofun numarasından 1 fazla olan çataldır.
	int right = (philosopherNumber + 1) % NUMBER_OF_PHILOSOPHERS;
	
	//sol çatal numarası filazofun numarası ile aynı olan çataldır.
	int left = (philosopherNumber + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS;
	
	//Eğer filazofun numarası tek sayı ise  önce sol çatalı alarak başlar
	if (philosopherNumber & 1) {
		//filazof solundaki çatalı almak için bekler
		printf("Filazof %d solundaki %d numarali catali almak icin bekliyor \n", philosopherNumber, left);
		pthread_mutex_lock(&chopsticks[left]);
		//filazof solundaki çatalı alır
		printf("Filazof %d solundaki %d numarali catali aliyor\n", philosopherNumber, left);
		
		//filazof sağındaki çatalı almak için bekler
		printf("Filazof %d sagindaki %d numarali catali almak icin bekliyor\n", philosopherNumber, right);
		pthread_mutex_lock(&chopsticks[right]);
		
		//filazof sağındaki çatalı alır
		printf("Filazof %d sagındaki %d numarali catali aliyor\n", philosopherNumber, right);
	}
	
	//Eğer filazofun numarası çift sayı ise  önce sağ çatalı alarak başlar
	else {
	
		//filazof sağındaki çatalı almak için bekler
		printf("Filazof %d sagindaki %d numarali catali almak icin bekliyor\n", philosopherNumber, right);
		pthread_mutex_lock(&chopsticks[right]);
		
		//filazof sağındaki çatalı alır
		printf("Filazof %d sagındaki %d numarali catali aliyor\n", philosopherNumber, right);
		
		//filazof solundaki çatalı almak için bekler
		printf("Filazof %d solundaki %d numarali catali almak icin bekliyor \n", philosopherNumber, left);
		pthread_mutex_lock(&chopsticks[left]);
		//filazof solundaki çatalı alır
		printf("Filazof %d solundaki %d numarali catali aliyor\n", philosopherNumber, left);
	}
}

//filazof yemek yer
void eat(int philosopherNumber) {
	int eatTime = rand() % 3 + 1;
	printf(" %d . Filazof  yemek yiyor\n", philosopherNumber, eatTime);
	sleep(eatTime);
}
 //çatalları bırakır
void putDown(int philosopherNumber) {
	printf("Filazof %d catallarını bırakıyor\n", philosopherNumber);
	
	//sağındaki çatalı bırakır ve böylece diğerlerinin alabilmesi için serbest kalmış olur
	pthread_mutex_unlock(&chopsticks[(philosopherNumber + 1) % NUMBER_OF_PHILOSOPHERS]);
	
	//solundaki çatalı bırakır ve böylece diğerlerinin alabilmesi için serbest kalmış olur
	pthread_mutex_unlock(&chopsticks[(philosopherNumber + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS]);
}
