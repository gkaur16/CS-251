#include "hw5.h"
#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include <sched.h>
#include <unistd.h>
#include "uthash.h"

#define NUM_ELEVATORS 4
#define NUM_PASSENGERS 50
#define ENABLE_HASH 1

// used as a hash table
// to skip uneccesary floors
struct Passengers {
  int id;
  int floor;
  UT_hash_handle hh; /*  makes this structure hashable */
} passengers[NUM_PASSENGERS];

struct Passengers *p_struct = NULL;

struct Elevator {
  pthread_mutex_t lock;
  int current_floor;
  int destination_floor;
  int direction;
  int occupancy;
  enum {ELEVATOR_ARRIVED=1, ELEVATOR_OPEN=2, ELEVATOR_CLOSED=3} state;	
} elevators[NUM_ELEVATORS];

void scheduler_init() {	

  for (int i=0; i<NUM_PASSENGERS; i++){
    passengers[i].id = i;
    passengers[i].floor = -1;
  }

  for (int i=0; i<NUM_ELEVATORS; i++){
    elevators[i].current_floor=0;		
    elevators[i].direction=-1;
    elevators[i].occupancy=0;
    elevators[i].state=ELEVATOR_ARRIVED;
    pthread_mutex_init(&elevators[i].lock,0);
  }
}

// used to check if a passenger is at a given floor
int empty_floor(int floor){

  struct Passengers *p;

  HASH_FIND_INT(p_struct, &floor, p);  /*  p: output pointer */

  if (p == NULL)
    return 0; // no passenger on the current floor

  return 1;

}

// add a passenger to a hash table where the 
// floor is the key
void add_passenger(int id, int floor){
  struct Passengers *p = malloc(sizeof(struct Passengers));
  p->id = id;
  p->floor = floor;
  HASH_ADD_INT(p_struct, floor, p);  
}

// delete a passenger from the hash
void delete_passenger(int floor){

  struct Passengers *p;

  HASH_FIND_INT( p_struct, &floor, p);  

  if (p==NULL)
    return;

  HASH_DEL(p_struct, p);  

}


void passenger_request(int passenger, int from_floor, int to_floor, 
    void (*enter)(int, int), 
    void(*exit)(int, int))
{	
  passengers[passenger].id = passenger;
  passengers[passenger].floor = from_floor;

  if(ENABLE_HASH)
    add_passenger(passenger, from_floor);

  int current_elevator;

  // wait for the elevator to arrive at our origin floor, then get in
  int waiting = 1;
  while(waiting){
    // loop through each elevator and allow for passenger to enter
    // that elevator if it's available
    for (current_elevator=0; current_elevator<NUM_ELEVATORS; current_elevator++){
      pthread_mutex_lock(&elevators[current_elevator].lock);

      if(elevators[current_elevator].current_floor == from_floor 
          && elevators[current_elevator].state == ELEVATOR_OPEN 
          && elevators[current_elevator].occupancy==0) {

        enter(passenger, current_elevator);
        elevators[current_elevator].occupancy = 1;
        elevators[current_elevator].destination_floor = to_floor;
        waiting = 0;

        // optimize which direction the elevator takes next
        // after picking up a passegner
        if(elevators[current_elevator].current_floor < to_floor) 
          elevators[current_elevator].direction=1;

        else if (elevators[current_elevator].current_floor == FLOORS-1 
            || elevators[current_elevator].current_floor == 0){
          // do nothing
        }

        else
          elevators[current_elevator].direction=-1;

        pthread_mutex_unlock(&elevators[current_elevator].lock);

        //passenger has been picked up
        pthread_mutex_unlock(&elevators[current_elevator].lock);
        break;
      }
      pthread_mutex_unlock(&elevators[current_elevator].lock);
    }
  }

  // wait for the elevator at our destination floor, then get out
  int riding=1;
  while(riding) {
    pthread_mutex_lock(&elevators[current_elevator].lock);

    // adjust the direction to point at the new floor
    // this isn't perfect but it works fairly well
    if(elevators[current_elevator].current_floor == to_floor 
        && elevators[current_elevator].state == ELEVATOR_OPEN) {
      exit(passenger, current_elevator);
      if (ENABLE_HASH)
        delete_passenger(from_floor);
      elevators[current_elevator].occupancy = 0;
      riding=0;
    }

    pthread_mutex_unlock(&elevators[current_elevator].lock);
    //    usleep(20); //force sleep to allow elevator thread to capture state
  }
}

void elevator_ready(int elevator, int at_floor, 
    void(*move_direction)(int, int), 
    void(*door_open)(int), void(*door_close)(int)) {

  // put each elevator as they come in the back
  // of the thread queue
  usleep(200);

  if (elevator >= NUM_ELEVATORS) return;

  pthread_mutex_lock(&elevators[elevator].lock);

  // dont open door if passenger is inside
  // unless they are at they're destination floor
  if(elevators[elevator].occupancy == 1
      && elevators[elevator].state == ELEVATOR_ARRIVED
      && elevators[elevator].destination_floor != at_floor){

    elevators[elevator].state = ELEVATOR_CLOSED;
  }

  // don't open door if a floor is empty
  else if(elevators[elevator].occupancy == 0
      && elevators[elevator].state == ELEVATOR_ARRIVED 
      && empty_floor(at_floor) == 0
      && ENABLE_HASH){

    elevators[elevator].state = ELEVATOR_CLOSED;
  }

  else if(elevators[elevator].state == ELEVATOR_ARRIVED) {
    door_open(elevator);
    elevators[elevator].state=ELEVATOR_OPEN;
    // allows for enough time to context switch
    usleep(10000); // give time for passenger to enter and/or exit elevator
  }
  else if(elevators[elevator].state == ELEVATOR_OPEN) {
    door_close(elevator);
    elevators[elevator].state=ELEVATOR_CLOSED;
  }
  else {
    if(at_floor==0 || at_floor==FLOORS-1) 
      elevators[elevator].direction*=-1;
    move_direction(elevator,elevators[elevator].direction);
    elevators[elevator].current_floor=at_floor+elevators[elevator].direction;
    elevators[elevator].state=ELEVATOR_ARRIVED;
  }
  pthread_mutex_unlock(&elevators[elevator].lock);
}