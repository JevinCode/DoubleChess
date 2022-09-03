#pragma once
#include <limits>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "FrameTimer.h"

class Bencher
{
public:
	Bencher()
		:
		outFile("Results.txt")
	{}
	Bencher(std::string outFile)
		:
		outFile(outFile)
	{}
	void Start()
	{
		ft.Mark();
	}
	void OutPut() const
	{
		std::ofstream output;
		output.open(outFile);
		output << "Generate Moves Benchmark Set Results:" << std::endl
			<< "Sample size: " << nSamples << std::endl
			<< "Mean: " << prevMean * 1000.0f << "ms" << std::endl
			<< "Min: " << prevMin * 1000.0f << "ms" << std::endl
			<< "Max: " << prevMax * 1000.0f << "ms" << std::endl;
		output.close();
	}
	float Get() const
	{
		return cur;
	}
	bool End()
	{
		const float dt = ft.Mark();
		cur = dt;
		min = std::min( dt,min );
		max = std::max( dt,max );
		mean += dt / nSamples;

		if( ++count >= nSamples )
		{
			OutPut();
			// store results
			prevMin = min;
			prevMax = max;
			prevMean = mean;
			// reset bench state
			count = 0;
			min = std::numeric_limits<float>::max();
			max = 0.0f;
			mean = 0.0f;
			// signal results update
			return true;
		}
		return false;
	}

private:
	FrameTimer ft;
	int count = 0;
	int nSamples = 5;
	float min = std::numeric_limits<float>::max();
	float max = 0.0f;
	float mean = 0.0f;
	float cur = 0.0f;
	float prevMin = std::numeric_limits<float>::signaling_NaN();
	float prevMax = std::numeric_limits<float>::signaling_NaN();
	float prevMean = std::numeric_limits<float>::signaling_NaN();
	const std::string outFile = "Benchmarks.txt";
};