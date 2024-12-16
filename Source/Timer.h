#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	float Read();
	float Stop();	



private:
	float timer = 0;
	bool is_enabled = false;

};

#endif // __TIMER_H__