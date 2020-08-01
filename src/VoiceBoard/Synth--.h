/*
 *  Synth--.h
 *
 *  Copyright (c) 2001-2012 Nick Dowell
 *
 *  This file is part of amsynth.
 *
 *  amsynth is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  amsynth is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with amsynth.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SYNTH_MM_H
#define _SYNTH_MM_H

#include <algorithm>
#include <cmath>

#ifdef M_E
#undef M_E
#endif
#define M_E		2.7182818284590452354F

#ifdef M_PI_2
#undef M_PI_2
#endif
#define M_PI_2	1.57079632679489661923F

#define TWO_PI 6.28318530717958647692F
#define PI     3.14159265358979323846F

class Lerper
{
public:

	void configure(float startValue, float finalValue, unsigned int numSteps)
	{
		_start = startValue;
		_final = finalValue;
		_steps = numSteps;
		if (0 < _steps) {
			_inc = (_final - _start) / (float)_steps;
		} else {
			_inc = 0.0F;
			_start = finalValue;
		}
		_i = 0;
	}

	inline float getValue() const
	{
		return _start + _inc * static_cast<float>(_i);
	}
	
	inline float nextValue()
	{
		float y = getValue();
		_i = std::min(_i + 1, _steps);
		return y;
	}

	inline float getFinalValue() const
	{
		return _final;
	}
	
private:

	float _start{0.F};
	float _final{0.F};
	float _inc{0.F};
	unsigned int _steps{0};
	unsigned int _i{0};
};

struct IIRFilterFirstOrder
{
	enum class Mode
	{
		kLowPass,
		kHighPass,
	};
	
	void setCoefficients(float sampleRate, float cutoffFreq, Mode mode)
	{
		float x = powf(M_E, -M_PI_2 * std::min(cutoffFreq / sampleRate, 0.5F));
		if (mode == Mode::kLowPass) {
			_a0 = 1.0F - x;
			_a1 = 0.0F;
			_b1 = x;
		} else {
			_a0 =  (1 + x) / 2.0F;
			_a1 = -(1 + x) / 2.0F;
			_b1 = x;
		}
	}
	
	inline float processSample(float x)
	{
		float y = (x * _a0) + _z;
		_z = (x * _a1) + (y * _b1);
		return y;
	}
	
	void processBuffer(float *samples, unsigned numSamples)
	{
		for (unsigned i=0; i<numSamples; i++) {
			samples[i] = processSample(samples[i]);
		}
	}
	
	float _a0{0.F};
	float _a1{0.F};
	float _b1{0.F};
	float _z{0.F};
};

class ParamSmoother
{
public:
	
	inline float processSample(float x)
	{
		return (_z += ((x - _z) * 0.005F));
	}
	
	inline void set(float z)
	{
		_z = z;
	}
	
private:
	float _z{0.F};
};

class SmoothedParam
{
public:
	
	SmoothedParam(float rawValue = 0.F): _rawValue(rawValue) {}
	~SmoothedParam() = default;
	
	SmoothedParam(const SmoothedParam&) = delete;
	SmoothedParam& operator=(const SmoothedParam&) = delete;
	
	SmoothedParam(SmoothedParam&&) = delete;
	SmoothedParam& operator=(SmoothedParam&&) = delete;
	
	SmoothedParam& operator=(float rawValue)
	{
		_rawValue = rawValue;
		return *this;
	}
	
	float getRawValue()
	{
		return _rawValue;
	}
	
	inline float tick()
	{
		return _smoother.processSample(_rawValue);
	}
	
private:
	
	float _rawValue;
	ParamSmoother _smoother;
};

#endif
