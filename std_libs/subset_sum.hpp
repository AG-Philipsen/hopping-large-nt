//Created: 13-05-2014
//Modified: Thu 23 Jun 2016 22:57:34 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef SUBSET_SUM_HPP
#define SUBSET_SUM_HPP

#include"typedefs.hpp"

#include<list>
#include<vector>
#include<algorithm>
#include<numeric>

#include<exception>
#include<stdexcept>


template <
  class IntegerType = int, 
  class Container = std::vector<int>, 
  typename = EnableIf< std::is_integral<IntegerType> > >
class SubsetSum
{
private: 
	IntegerType targetValue;
	Container valuesToPickFrom;

	std::list<IntegerType> currentPick;

private:

	bool targetReached(IntegerType currentSum, IntegerType possibleNewElement){
		return (currentSum + possibleNewElement) == targetValue;
	};

	bool targetNotYetReached(IntegerType currentSum, IntegerType possibleNewElement){
		return (currentSum + possibleNewElement) < targetValue;
	};

	void temporarilyAddNewElementAndAddToResult(std::list<Container> &finalSums, const IntegerType &newElement) {
		currentPick.push_back(newElement);
		finalSums.emplace_back(currentPick.begin(),currentPick.end());
		currentPick.pop_back();
	};

	void pickOneMoreElement(std::list<Container> &finalSums, typename Container::const_iterator startPickAt) {

		IntegerType currentSum = sumCurrentPick();

		for(auto toAddToIt = startPickAt; toAddToIt != valuesToPickFrom.cend(); ++toAddToIt){

			if(targetReached(currentSum, *toAddToIt))
				temporarilyAddNewElementAndAddToResult(finalSums, *toAddToIt);
			else if(targetNotYetReached(currentSum, *toAddToIt)){
				currentPick.push_back(*toAddToIt);
				pickOneMoreElement(finalSums, toAddToIt);
				currentPick.pop_back();
			}
		}
	};

	IntegerType sumCurrentPick() const {
		IntegerType zero(0);
		return std::accumulate(currentPick.begin(), currentPick.end(), zero);
	};

public:
	std::list<Container> calculate() {
		std::list<Container> result;
		pickOneMoreElement(result, valuesToPickFrom.begin());
		return result;
	};

	SubsetSum(IntegerType target, const Container &initialValuesToPickFrom) : 
		targetValue(target), valuesToPickFrom(initialValuesToPickFrom)
	{
		if( std::find_if(
			initialValuesToPickFrom.begin(), 
			initialValuesToPickFrom.end(),
			std_types::LessThanOrEqualToZero<IntegerType>() ) != initialValuesToPickFrom.end() ) {

			throw std::invalid_argument("In construction of SubsetSum-class: All pick-values must be strictly greater than zero.");
		}
	};

	virtual ~SubsetSum() {};
};



#endif /* SUBSET_SUM_HPP */
