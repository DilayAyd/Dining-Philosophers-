#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
  int pozisyon;
  
  //loopta indislerin modunu alıp 
  //sağ sol çatalı belirlemek için kullanılır.
  int count;
  
  //semaphorelarımızı tanımladık
  //her indiste 1 semaphore var ve değeri 1 
  sem_t *catal;
  
  //Masaya oturma için kullandığımız semaphore
  //n-1 filazof otursun istediğimiz için 5-1  4 tanımlıyoruz
  sem_t *lock;
  
  
} params_t;

// semaphorelara değerleri veriliyor
void initialize_semaphores(sem_t *lock, sem_t *catal, int num_catal);
void run_all_threads(pthread_t *threads, sem_t *catal, sem_t *lock, int num_philosophers);


void *filazof(void *params);
void dusun(int pozisyon);
void yemek(int pozisyon);


int main(int argc, char *args[])
{
 //5tane filazofumuz var
  int num_filazof = 5;

  sem_t lock;
  
  //çatalları dizide tutuyoruz
  sem_t catal[num_filazof];
  
  //5 filazof olduğu için 5 tane threadimiz oluyor
  pthread_t filazof[num_filazof];

  initialize_semaphores(&lock, catal, num_filazof);
  run_all_threads(filazof, catal, &lock, num_filazof);
  pthread_exit(NULL);
}

void initialize_semaphores(sem_t *lock, sem_t *catal, int num_catal)
{
  int i;
  
  // çatallar bir dizide tutuluyordu
  //for döngüsü ile çatalların değerini 1 atıyor
  for(i = 0; i < num_catal; i++) {
    sem_init(&catal[i], 0, 1);
  }

  //lock masaya oturma semaphory idi 
  //çözümde n-1 filazof masaya otursun istendiği için
  //biz de n-1 tanımlıyoruz.
  sem_init(lock, 0, num_catal - 1);
}

void run_all_threads(pthread_t *threads, sem_t *catal, sem_t *lock, int num_filazof)
{
  int i;
  
  //döngü 5 kere döner her iterasyon için
  //for döngüsü ile 5 filazof için eş zamanlı çalışıyor
  for(i = 0; i < num_filazof; i++) {
    params_t *arg = malloc(sizeof(params_t));
	
	//pozisyon değeri atanır yani filazofun oturduğu koltuk 
	//aynı zamanda kaçıncı filazof olduğu anlaşılır.
    arg->pozisyon = i;
	
	//sağ ve soldaki çatalları belirlerken son filazofun sağındaki çatalı
	//belirleyebilmek için mod almamız gerekir.
	//count ile filazof sayısını mod sayısı olarak belirliyoruz
    arg->count = num_filazof;
	
    arg->lock = lock;
    arg->catal = catal;

    pthread_create(&threads[i], NULL, filazof, (void *)arg);
  }
}

//filazof fonksiyonunu yazıyoruz
//pozisyon,count,çatal ve lock'un kullanılabilmesi için
//daha önce tanımladığımız params'ı parametre olarak koyuyoruz
void *filazof (void *params)
{
  int i;
  params_t self = *(params_t *)params;

//Her prosesin 3 kere işlem yaptığı bir for döngüsü tanımlıyoruz
//her filazof 3 kere düşünür ve 3 kere yemek yer ardından odayı terkeder
  for(i = 0; i < 3; i++) {
    dusun(self.pozisyon);
     
	//filazof çatalları almak için bekler 
    sem_wait(self.lock);
	
	//solundaki çatalı alır
    sem_wait(&self.catal[self.pozisyon]);
	sol(self.pozisyon); //yapılan işlemler ekrana yazdırılır
	
	//sağındaki çatalı alır ve aldığı çatalların değeri 0 olur
    sem_wait(&self.catal[(self.pozisyon + 1) % self.count]);
    sag(self.pozisyon);
	
	
	//filazof yemeğini yer
    yemek(self.pozisyon);
	
	//çatallarını bırakır böylece çatalların semaphorelarının değerleri 1 olur
	//değerleri 1 olunca diğer filazof çatalları alabilir hale gelir.
    sem_post(&self.catal[self.pozisyon]);
    solbirak(self.pozisyon);
	
    sem_post(&self.catal[(self.pozisyon + 1) % self.count]);
    sagbirak(self.pozisyon);
    sem_post(self.lock);
    
    
	// her filazof için 3 kere yeme düşünme tanımladığımız için 
	//döngü o filazof için 2 kere daha yeniden döner
  }
  // döngüsünü bitiren filazof odayı terk eder.
  ayril(self.pozisyon);
 
  
  printf("-------------------------------------\n");
  pthread_exit(NULL);
}

//yapılan işlemlerin ekrana yazdılması için tanımlanan fonksiyonlar.

void dusun(int pozisyon)
{
  printf("Filazof %d dusunuyor...\n", pozisyon);
}

void sol (int pozisyon)
{
    	printf("Filazof %d solundaki catali aldi...\n", pozisyon);
}

void sag (int pozisyon)
{
    	printf("Filazof %d sagindaki catali aldi...\n", pozisyon);
}


void yemek(int pozisyon)
{
  printf("Filazof %d yemek yiyor...\n", pozisyon);
}

void solbirak (int pozisyon)
{
    	printf("Filazof %d solundaki catali birakti...\n", pozisyon);
}

void sagbirak (int pozisyon)
{
    	printf("Filazof %d sagindaki catali birakti...\n", pozisyon);
}

void ayril (int pozisyon)
{
  printf("Filazof %d odadan ayrildi!!!\n", pozisyon);
}
