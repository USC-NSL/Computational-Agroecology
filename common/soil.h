// Copyright 2019

#ifndef AGROECOLOGY_SOIL_H_
#define AGROECOLOGY_SOIL_H_

class Soil {
	public:
		enum class Texture {
			CLAY = 0,
			SILT = 1,
			SAND = 2
		};
		
		Soil(Texture texture, int pH, int salinity, int orgMat, int water)
			: texture_(texture), pH_(pH), salinity_(salinity), organic_matter_(orgMat), water_content_(water)
			{}
		// with fix soil type and other information
		Soil() : texture_(Texture::CLAY), pH_(7), water_content_(0.03) {}
		Texture texture() const { return texture_; }
		double pH() const { return pH_; }
		double salinity() const { return salinity_; }
		double organic_matter() const { return organic_matter_; }
        double water_content() const { return water_content_; }
		const double* nutrients() const { return nutrients_; }
		
		void addWater(double quantity) { water_content_ += quantity; }
		void addNitrogen(double quantity) { nutrients_[0] += quantity; }
		void addPhosphorus(double quantity) { nutrients_[1] += quantity; }
		void addPotassium(double quantity) { nutrients_[2] += quantity; }
		void addCalcium(double quantity) { nutrients_[3] += quantity; }
		void addMagnesium(double quantity) { nutrients_[4] += quantity; }
		void addSulfur(double quantity) { nutrients_[5] += quantity; }
		
	private:
		Texture texture_;
		double pH_;
		double salinity_;
		double organic_matter_;
		double water_content_;
		
		//contains nitrogen, phosphorus, potassium, calcium, magnesium, and sulfur 
		//information for the soil, contained in the corresponding indices
		double nutrients_[6];
		
};

#endif
