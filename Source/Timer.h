#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	unsigned int Read();
	unsigned int Stop();


private:
	float timer = 0;
	bool is_enabled = false;

	unsigned int start_time;
	unsigned int elapsed_time;

};

#endif // __TIMER_H__