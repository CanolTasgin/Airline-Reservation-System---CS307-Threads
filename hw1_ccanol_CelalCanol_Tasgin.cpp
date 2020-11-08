#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>

//global variables
bool firstOneLeft = false; 
bool planeChecker = false;
int rnd = 0;
int count = 100;
std::vector<std::vector<int> > plane(2,std::vector<int>(50));

void * travelAgency(void * p)
{
  srand(time(NULL)); // In order to make random generator generate different seeds at each execution
  int * pPointer = (int*) p;
  while(!planeChecker){ // check if plane is full
    while(!planeChecker && rnd != *pPointer); // Wait for other agency
    bool seatIsEmpty = false, out = false;
    while(!planeChecker && !seatIsEmpty){ // Loop until finding an empty seat
      if(!out){
        std::cout << "Agency "<< *pPointer + 1 << " Entered Critical Region"<<std::endl;
        out = true;
      }

      int row = rand() % 2;
      int col = rand() % 50;
      if(plane[row][col] == 0){ // Check if the seat is empty
        seatIsEmpty = true;
        plane[row][col] = *pPointer+1; // Tell which agency took the seat
        count--;
        int seat;
        if(!((row)%2)){
          seat = (row+2)*(col+1);
          seat--;
        }
        else{
          seat = (row+1)*(col+1);
        }
        std::cout << "Seat Number " << seat << " is reserved by Agency " << *pPointer + 1 <<std::endl;
        std::cout << "Agency "<< *pPointer+1 <<" Exit Critical Region\n\n\n";
        if(rnd == 0){ //For making sure changing agencies through the loop in a proper way.
          rnd = 1;
        }
        else{
          rnd = 0;
        }
      }
    }
  }
  if (firstOneLeft){ //if one of the threads couldn't find an empty seat: (don't repeat for the second thread if one of them has already left.)
    std::cout << "Agency "<< *pPointer << " Entered Critical Region"<<std::endl; // other thread entered and exit critical region.
    std::cout << "Agency "<< *pPointer <<" Exit Critical Region"<<std::endl;
    std::cout << "No Seats Left\n" <<std::endl;
  }
  firstOneLeft = true;
  
  return 0;
}


int main()
{
	pthread_t thread1, thread2;
    int TA_1 = 0, TA_2 = 1;
	pthread_create( &thread1, NULL, travelAgency, (void *) & TA_1);
	pthread_create( &thread2, NULL, travelAgency, (void *) & TA_2);
  

  while(!planeChecker)
    if(count==0)
      planeChecker=true;

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
	

  std::cout << "Plane is full:\n"<<std::endl;
  for (size_t i = 0; i < 2; i++) {
    std::cout << "Seat "<<i+1<<": ";
    for (size_t j = 0; j < 50; j++) {
      std::cout << plane[i][j] << " ";
    }
    std::cout << std::endl;
  }
	return 0;
}