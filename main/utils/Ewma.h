/* EWMA Filter: exponentially weighted moving average filter which smooths out a dataset
* it does this by averaging previously collected datapoints with actively collected datapoints to yield
* a dataset that is less erratic  
*
*EWMA(t) = alpha * x(t) + (1-alpha)EWMA(t-1)
* 
* alpha : smoothing factor from [0,1] (higher a value causes less smoothing of datapoints)
* x(t) : unfiltered data collection
* EWMA(t-1) : last filtered output
* t : time 
*/ 

#pragma once

class Ewma{
public:	
	/// @brief constructs an Ewma filter
	/// @param newAlpha smoothing factor from [0,1] (higher alpha value causes less smoothing of datapoints)
	Ewma(double newAlpha) {
		alpha = newAlpha;
	} 

	void reset(){
		hasInitial = false; 
	}

	void filter(const double input){
		if (hasInitial) {
			output = alpha * (input - output) + output;
		} else {
			output = input;
			hasInitial = true;
		}
	}

	double getOutput() {
		return output;
	}

private:
	double alpha; //smoothing factor
	bool hasInitial;
	double output; //current output EWMA(t)
};
