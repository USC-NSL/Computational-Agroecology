#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PRODUCE_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PRODUCE_H_

namespace environment {

struct Produce {
  // enum to describe plant's individual fruit/produce maturities
  enum Maturity { FLOWER = 0, YOUNG = 1, RIPE = 2, OLD = 3};
  // initialize weight at 0
  double weight_ = 0.0;
  // initialize maturity to flower
  Maturity maturity_ = Produce::FLOWER;
};

} // namespace environment


#endif



/*


	public:
		Produce() {
			this->weight_=0;
			this->maturity_ = FLOWER;
		}
		enum Maturity { FLOWER = 0, JUVENILE, RIPE, OLD };
		void UpdateWeight(double weight);
		void IncreaseWeight(double weight);
		void IncrementMaturity();
		Maturity getMaturity() { return this->maturity_; }
		double getWeight() { return this->weight_; }
	protected:
		double weight_;
		Maturity maturity_;

};

} // namespace environment
*/
