/*
 * SeeYouTask.h
 *
 *  Created on: Mar 27, 2011
 *      Author: seeyou
 */

#ifndef SEEYOUTASK_H_
#define SEEYOUTASK_H_

#include "Task.h"


class SeeYouTask : public Task {

	Q_OBJECT

public:
	int speed;
	QString move;
	int tick;

public:
	SeeYouTask(Create *create, QString move, int speed, TaskPriority priority = Task::Normal);
	virtual ~SeeYouTask();
	virtual void process();
	virtual QString description();
};

#endif /* SEEYOUTASK_H_ */
