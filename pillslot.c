#include "pillslot.h"

Pill pills[MAX_SLOT];
int slotSize = 0;

int setSlot(int slotNum, char* pillName, int amount){
  if(pills[slotNum].enabled){
    //Override?
    return 0;
  }
  pills[slotNum].pillName = pillName;
  pills[slotNum].amount = amount;
  pills[slotNum].enabled = 1;
  slotSize++;
  return 1;
}

void removeSlot(int slotNum){
  if(pills[slotNum].enabled){
    pills[slotNum].enabled = 0;
    slotSize--;
  }
}

int compare( const void* a, const void* b){
   int int_a = * ( (int*) a );
   int int_b = * ( (int*) b );

   if ( int_a == int_b ) return 0;
   else if ( int_a < int_b ) return -1;
   else return 1;
}

int addTime(int slotNum, int t){
  if(pills[slotNum].enabled){
    pills[slotNum].sch_mode = SCH_TIMESET;
    int thisTimeSize = pills[slotNum].timeSize;
    if(thisTimeSize >= MAX_TIMESET){
      //print no more alarm allowed
      return 0;
    }
    pills[slotNum].time[thisTimeSize] = t;
    qsort(pills[slotNum].time, thisTimeSize + 1, sizeof(int), compare);
    pills[slotNum].timeSize++;
    return 1;
  }
  return 0;
}

void setPeriod(int slotNum, int period, int startTime, int endTime){
  if(pills[slotNum].enabled){
    pills[slotNum].sch_mode = SCH_PERIODIC;
    pills[slotNum].period = period;
    pills[slotNum].time[0] = startTime;
    pills[slotNum].time[1] = endTime;
  }
}

int dispensePill(int slotNum){
  if(!pills[slotNum].enabled | pills[slotNum].amount <= 0){
    return 0;
  }
  pills[slotNum].amount--;
  return 1;
}

Pill getPill(int slotNum){
  return pills[slotNum];
}

int getNextPill(int nextPill[MAX_SLOT], int* time, int currTime){
  int minTime = 24 * 60;
  int index = 0;
  for(int i = 0; i < MAX_SLOT; i++){
    Pill pill = getPill(i); 
    if(pill.enabled){
      if(!pill.sch_mode){//time set
        for(int j = 0; j < pill.timeSize; j++){
          if(pill.time[j] > currTime && pill.time[j] < minTime){
            nextPill[0] = i;
            minTime = pill.time[j];
            index = 1;
          } else if(pill.time[j] == minTime){
            nextPill[index++] = i;
          }
        }
      } else {//periodic
        for(int j = pill.time[0]; j <= pill.time[1]; j += pill.period){
          if(j > currTime && j < minTime){
            nextPill[0] = i;
            minTime = j;
            index = 1;
          } else if(j == minTime){
            nextPill[index++] = i;
          }
        }
      }
    }
  }
  *time = minTime;
  return index;
  
}

int getSlotSize(){
  return slotSize;
}