/*
 * RFIDDBase.h
 *
 *  Created on: Apr 28, 2011
 *      Author: seeyou
 */

#ifndef RFIDDBASE_H_
#define RFIDDBASE_H_


#include <iostream>
#include <vector>

using namespace std;

class RFIDDBase {
public:
	 typedef struct
	{
		int id;
		double x;
		double y;
	} RFID_DB;

    vector <RFID_DB> predefinedDB;

    void createDB();

    int getDBX(int tag);
    int getDBY(int tag);

	RFIDDBase();
	virtual ~RFIDDBase();

private:
	vector <int> x;
};

#endif /* RFIDDBASE_H_ */
