#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	virtual void Start();
	virtual unsigned int Read();
	virtual unsigned int Stop();


private:
	float timer = 0;
	bool is_enabled = false;

	unsigned int start_time;
	unsigned int elapsed_time;

};

#endif // __TIMER_H__