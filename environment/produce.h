#ifndef AGROECOLOGY_PRODUCE_H_
#define AGROECOLOGY_PRODUCE_H_


namespace environment {

	class Produce {
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


#endif