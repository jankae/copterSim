
#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>
#include <sys/time.h>
#include <stddef.h>

class Clock {
private:
	long long int starttime;
	Clock();
	static Clock* m_pInstance;
public:
	static Clock* Instance();
	uint64_t getTime_us(void);
	uint32_t getTime_ms(void);
	void waitus(long long int us);
};

#endif /* CLOCK_H_ */
