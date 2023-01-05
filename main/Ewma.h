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

#ifndef EWMA_H_
#define EWMA_H_

class Ewma{
public:
	
	double output = 0; //current output EWMA(t)
	double alpha = 0; //smoothing factor

	
	Ewma(double alpha) { // constructs an Ewma filter with smoothing factor a
		this->alpha = alpha;
		} 

	void reset(){
		this->hasInitial = false; 
	}

	double filter(double input){
		if (hasInitial) {
			output = alpha * (input - output) + output;
		} else {
			output = input;
			hasInitial = true;
		}
		return output;
	}

private:
	bool hasInitial = false;

};

#endif