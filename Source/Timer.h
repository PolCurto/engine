#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	virtual void Start();
	virtual float Read();
	virtual float Stop();
	virtual float TicksSinceStartup() const;

	float GetDeltaTime() const { return delta_time; }

	void SetTimeScale(const float new_scale) { time_scale = new_scale; }


private:
	float timer = 0;
	bool is_enabled = false;

	float start_time = 0;
	float delta_time = 0;
	float elapsed_time = 0;
	float real_elapsed_time = 0;
	float time_scale = 1.0f;

};

#endif // __TIMER_H__