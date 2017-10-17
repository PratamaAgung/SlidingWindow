#ifndef __WINDOWSTATUS__
#define __WINDOWSTATUS__

#include <ctime>

class WindowStatus{
	private:
		char status; /*0 not_sent, 1 wait, 2 sent*/
		clock_t time;
	public:
		WindowStatus(){
			status = 0;
			time = clock();
		}

		char getStatus(){
			return status;
		}

		clock_t getTime(){
			return time;
		}

		void setStatus(char status){
			this->status = status;
		}

		void setTime(clock_t time){
			this->time = time;
		}
};

#endif