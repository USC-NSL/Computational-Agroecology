// Copyright 2019
#ifndef AGROECOLOGY_COMMON_PRODUCE_H_
#define AGROECOLOGY_COMMON_PRODUCE_H_

// base class for produce
class Produce {
public:
	Produce() { ripened = 0; rotten=0; weight=0.0;};
	void updateRipe(bool isRipe) { this->ripened = isRipe; };
	void updateRotten(bool isRotten) { this->rotten = isRotten; };
	void updateWeight(double newWeight) { this->weight = newWeight; };
	/*virtual int getTotal() = 0;*/
/*protected:*/
	bool ripened; // true if ripe, false if not
	bool rotten; // true if rotten, false if not
/*	bool onTree; // true if on tree, false if not*/
	double weight; // WEIGHTS ARE ALWAYS IN POUNDS, POUNDS ARE USED IN YIELD CALCULATIONS
};

// corn is inputted in cobs mostly, but cobs contain kernels
class CornProduce : public Produce {
public:
	CornProduce(bool ripe, bool poison, double Pweight, double moisture)
	{
		ripened=ripe;
		rotten=poison;
		/*this->onTree = locationOnPlant;*/
		kernelMoistureLevels = moisture; // used to calculate the yield for
		weight = Pweight;
		numBushels = this->calculateBushels(Pweight);
	};
	// for corn yield, got info from this site: http://corn.agronomy.wisc.edu/AA/pdfs/A033.pdf
	double calculateBushels(double weights)
	{
		double temp = weights*(1-kernelMoistureLevels);
		temp /= 0.845; // get the weight if 15.5% kernel moisture (i guess its the standard?)
		temp /= 56; // gets you the number of bushels
		return temp; 
	};

private:
	double kernelMoistureLevels;
	double numBushels;
};

// squash class derived from Produce class
class SquashProduce : public Produce {
public:
	SquashProduce(bool ripe, bool poison, double Pweight)
	{
		ripened=ripe;
		rotten=poison;
		/*this->onTree = locationOnPlant;*/
		weight = Pweight;
	};
/*	void addProduce(int amount) { this->numSquash = amount; };
	int getTotal() { return this->numSquash; };*/
private:
};

// bean class derived from Produce class
class BeanProduce : public Produce {
public:
	BeanProduce(bool ripe, bool poison, double Pweight)
	{
		ripened=ripe;
		rotten=poison;
		/*this->onTree = locationOnPlant;*/
		weight = Pweight;
	};
/*	void addProduce(int amount) { this->numBeans = amount; };
	int getTotal() { return this->numBeans; };*/
private:
};

#endif
