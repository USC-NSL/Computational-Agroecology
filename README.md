# Computational AgroEcology Project

## Organization of Plant / Soil Modeling

## Data Structures

### action_adapter

contains different functions that can be runned, from adding a crop to a location, 
to harvesting its produce, to incrementing the number of produce it has.

### plant_type

contains the data structure for the crop that is growing and its produce obtained.
other classes, such as Beans, Corn, Squash are all derived from this class.
This class also contains the threshold information on what weather, etc. is tolerable
for the crop

### produce

data structure for the different types of fruit / vegetable produce that grows,
with data members such as weight, ripe status, poisoned status, etc. 

### soil

stores the information of the soil, from the nutrient levels, to the water content,
so that this information can be communicated with the plants to see if tolerable 
and how this affects the GDD

### plant_index

stores more information for a certain index, such as maturity, health, GDD

### location

tracks the longitude and latitudes of the farm / crops

## Tests

a variety of tests are being implemented to ensure that all the data structures communicate
properly with one another, and that calculations are accurate to our expectations.

### config_tests

config_tests is a broader test that makes sure the other data structures operate correctly
with one another.

### plant.cpp

tests for ability for plant to exist, grow produce, harvest, and gather information on the crop,
including its produce yield / weight

### other tests

the other tests check for more specific parts of the simulator
