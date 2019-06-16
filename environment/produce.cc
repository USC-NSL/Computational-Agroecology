#include "produce.h"


namespace environment {

	void Produce::UpdateWeight(double weight)
	{
		this->weight_ = weight;
	}

	void Produce::IncreaseWeight(double weight)
	{
		this->weight_ += weight;
	}

	void Produce::IncrementMaturity()
	{
		if (this->maturity_ == Produce::FLOWER)
	    	this->maturity_ = Produce::JUVENILE;
		else if (this->maturity_ == Produce::JUVENILE)
			this->maturity_ = Produce::RIPE;
		else if (this->maturity_ == Produce::RIPE)
			this->maturity_ = Produce::OLD;
	}

} // namespace environmentdfsdf