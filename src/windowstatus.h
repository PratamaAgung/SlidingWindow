#ifndef __WINDOWSTATUS__
#define __WINDOWSTATUS__

#include <ctime>

class WindowStatus{
	private:
		int status; /*0 not_sent, 1 wait, 2 sent*/
		clock_t time;
	public:
		WindowStatus(){
			status = 0;
			time = clock();
		}

		int getStatus(){
			return status;
		}

		clock_t getTime(){
			return time;
		}

		void setStatus(int status){
			this->status = status;
		}

		void setTime(clock_t time){
			this->time = time;
		}
};

#endif