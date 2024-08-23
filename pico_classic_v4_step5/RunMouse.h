/*
 * RUN.h
 *
 *  Created on: Apr 7, 2024
 *      Author: aokimasatake
 */

#ifndef SRC_RUN_H_
#define SRC_RUN_H_

class RUN {
private:

public:
	volatile double accel;
	volatile double speed;
	volatile double max_speed;
	volatile double min_speed;


	RUN();
	virtual ~RUN();
	void Interrupt(void);

	void accelerate(int len, int finish_speed);
	void oneStep(int len, int init_speed);
	void decelerate(int len, float init_speed);

private:
  int step_lr;
};


extern RUN g_run;

#endif /* SRC_RUN_H_ */
