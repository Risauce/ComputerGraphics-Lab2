#pragma once

#include <string>
#include <sstream>
#include <math.h>


class Matrix3 {
	void initIdentity() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				values[i * 3 + j] = i == j;
			}
		}
	}

public:
	float values[9];
	Matrix3() {
		initIdentity();
	}

	Matrix3(float one, float two, float three, float four, float five, float six, float seven, float eight, float nine)
	{
		values[0] = one;
		values[1] = two;
		values[2] = three;
		values[3] = four;
		values[4] = five;
		values[5] = six;
		values[6] = seven;
		values[7] = eight;
		values[8] = nine;
		
	}

	Matrix3(float tx, float ty) //Translation Matrix Con
	{
		initIdentity();
		values[6] = tx;
		values[7] = ty;
	}

	Matrix3(float Sx, float Sy, float nope) //Scaling Matrix Con
	{
		initIdentity();
		values[0] = Sx;
		values[4] = Sy;
	}

	Matrix3(float theta) //Rotation Matrix
	{
		initIdentity();
		values[0] = cos(theta);
		values[1] = -sin(theta);
		values[3] = sin(theta);
		values[4] = cos(theta);

	}


	std::string to_string() const {
		std::ostringstream os;
		for (int i = 0; i < 9; ++i) {
			os << values[i] << " ";
		}
		return os.str();
	}
	
	Matrix3 operator + (Matrix3 const addition)
	{
		Matrix3 sum;
		for (int i = 0; i < 9; i++)
		{
			sum.values[i] = values[i] + addition.values[i];
		}

		return sum;
	}
	
	Matrix3 operator - (Matrix3 const subtraction)
	{
		Matrix3 difference;
		for (int i = 0; i < 9; i++)
		{
			difference.values[i] = values[i] - subtraction.values[i];
		}

		return difference;
	}

	float multHelper(float oneFirst, float oneSecond, float oneThird, float twoFirst, float twoSecond, float twoThird)
	{
		float result;
		result = oneFirst * twoFirst + oneSecond * twoSecond + oneThird * twoThird;
		return result;
	}

	Matrix3 operator * (Matrix3 const multiplier)
	{
		Matrix3 result;

		result.values[0] = multHelper(values[0], values[1], values[3], multiplier.values[0], multiplier.values[3], multiplier.values[6]);
		result.values[1] = multHelper(values[0], values[1], values[3], multiplier.values[1], multiplier.values[4], multiplier.values[7]);
		result.values[2] = multHelper(values[0], values[1], values[3], multiplier.values[2], multiplier.values[5], multiplier.values[8]);

		result.values[3] = multHelper(values[3], values[4], values[5], multiplier.values[0], multiplier.values[3], multiplier.values[6]);
		result.values[4] = multHelper(values[3], values[4], values[5], multiplier.values[1], multiplier.values[4], multiplier.values[7]);
		result.values[5] = multHelper(values[3], values[4], values[5], multiplier.values[2], multiplier.values[5], multiplier.values[8]);

		result.values[6] = multHelper(values[6], values[7], values[8], multiplier.values[0], multiplier.values[3], multiplier.values[6]);
		result.values[7] = multHelper(values[6], values[7], values[8], multiplier.values[1], multiplier.values[4], multiplier.values[7]);
		result.values[8] = multHelper(values[6], values[7], values[8], multiplier.values[2], multiplier.values[5], multiplier.values[8]);

		return result;

	}

	Matrix3 operator * (float scalar)
	{
		Matrix3 result;

		for (int i = 0; i < 9; i++)
		{
			result.values[i] = values[i] * scalar;
		}
		return result;
	}

};