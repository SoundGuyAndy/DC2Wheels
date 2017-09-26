/*
 UNIVERSIDADE ESTADUAL DO PIAUÍ - UESPI
 Bac. em Ciências da Computação - CTU

 AJ Alves.

 aj.alves@live.com;
 aj.zerokol@gmail.com;

 zerokol.com
 */

/********************************************************************
 * INCLUDES
 ********************************************************************/
#include "DC2Wheels.h"

/*
 speed* must be higher or equal to 0 and lower or equal to 90
 distance** is a value in centimeter
 degree*** is a value positive or negative different of 0, the sigh determines the direction
 degree**** is a value higher then 0
 smooth***** percentage value

 Ps:. Every time you use one of the motion methods the value of _speed and _direction are dynamically changed
 */

/********************************************************************
 * IMPLEMENTATION OF PUBLIC METHODS
 ********************************************************************/
DC2Wheels::DC2Wheels(int *rightWheelPins, int *leftWheelPins,
		double wheelRadius, double turnRadius) {
	//attaching referred pins to associative wheels
	_rightWheelPins = rightWheelPins;
	_leftWheelPins = leftWheelPins;
	stop();
	//Setting the initial values
	_wheelRadius = wheelRadius;
	_turnRadius = turnRadius;
	//Setting the resistance value
	_resistance = RESISTANCE_DEFAULT;
	//Setting the bend smooth
	_turnSmooth = TURN_SMOOTH_DEFAULT;
}

DC2Wheels::DC2Wheels(int *rightWheelPins, int *leftWheelPins,
		double wheelRadius, double turnRadius, double resistance) {
	//attaching referred pins to associative wheels
	_rightWheelPins = rightWheelPins;
	_leftWheelPins = leftWheelPins;
	stop();
	//Setting the initial values
	_wheelRadius = wheelRadius;
	_turnRadius = turnRadius;
	//Setting the resistance value
	_resistance = resistance;
	//Setting the bend smooth
	_turnSmooth = TURN_SMOOTH_DEFAULT;
}

DC2Wheels::DC2Wheels(int *rightWheelPins, int *leftWheelPins,
		double wheelRadius, double turnRadius, double resistance,
		float turnSmooth) {
	//attaching referred pins to associative wheels
	_rightWheelPins = rightWheelPins;
	_leftWheelPins = leftWheelPins;
	stop();
	//Setting the initial values
	_wheelRadius = wheelRadius;
	_turnRadius = turnRadius;
	//Setting the resistance value
	_resistance = resistance;
	//Setting the bend smooth
	_turnSmooth = turnSmooth;
}

//Method to stop the machine
void DC2Wheels::stop() {
	_direction = IDLE;
	for (int i = 0; i < 2; i++) {
		analogWrite(_rightWheelPins[i], MINIMUM_SPEED);
		analogWrite(_leftWheelPins[i], MINIMUM_SPEED);
	}
	_speed = STOPPED;
}

//Method to move the machine forward in a certain speed*
void DC2Wheels::forward(int speed) {
	speed = speedFormatter(speed);
	_direction = FORWARD;
	analogWrite(_rightWheelPins[BACK_POSITION], MINIMUM_SPEED);
	analogWrite(_rightWheelPins[FRONT_POSITION], speed);
	analogWrite(_leftWheelPins[BACK_POSITION], MINIMUM_SPEED);
	analogWrite(_leftWheelPins[FRONT_POSITION], speed);
	_speed = speed;
}

//Method to move the machine forward in a certain speed* to a distance*
void DC2Wheels::forward(int speed, double distance) {
	speed = speedFormatter(speed);
	//get the walk value of wheel displacement
	double displacement = getWalkDisplacement(distance);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//start the motion
	analogWrite(_rightWheelPins[BACK_POSITION], MINIMUM_SPEED);
	analogWrite(_rightWheelPins[FRONT_POSITION], speed);
	analogWrite(_leftWheelPins[BACK_POSITION], MINIMUM_SPEED);
	analogWrite(_leftWheelPins[FRONT_POSITION], speed);
	//wait until to reach the target
	delay(waitValue);
	//stop the machine
	stop();
}

//Method to move the machine backward in a certain speed*
void DC2Wheels::backward(int speed) {
	speed = speedFormatter(speed);
	_direction = REVERSE;
	analogWrite(_rightWheelPins[FRONT_POSITION], MINIMUM_SPEED);
	analogWrite(_rightWheelPins[BACK_POSITION], speed);
	analogWrite(_leftWheelPins[FRONT_POSITION], MINIMUM_SPEED);
	analogWrite(_leftWheelPins[BACK_POSITION], speed);
	_speed = speed;
}

//Method to move the machine backward in a certain speed* to a distance*
void DC2Wheels::backward(int speed, double distance) {
	speed = speedFormatter(speed);
	//get the walk value of wheel displacement
	double displacement = getWalkDisplacement(distance);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//start the motion
	analogWrite(_rightWheelPins[FRONT_POSITION], MINIMUM_SPEED);
	analogWrite(_rightWheelPins[BACK_POSITION], speed);
	analogWrite(_leftWheelPins[FRONT_POSITION], MINIMUM_SPEED);
	analogWrite(_leftWheelPins[BACK_POSITION], speed);
	//wait until to reach the target
	delay(waitValue);
	//stop the machine
	stop();
}

//Method to make a turn with the passed degree*** value, the degree sign will determine the turn side
/*
 Turn does not stop the machine, it gets the current speed value and make a appropriate turn
 */
void DC2Wheels::turn(int degree) {
	if (degree > 0) {
		rightTurn(degree);
	} else if (degree < 0) {
		degree *= -1;
		leftTurn(degree);
	}
}

//Method to make a right turn with the passed degree**** value
/*
 Turn does not stop the machine, it gets the current speed value and make a appropriate turn
 */
void DC2Wheels::rightTurn(int degree) {
	if (degree < 0) {
		degree = 0;
	}
	//Get the radians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement
	double displacement = getCircleDisplacement(radiansVal, _turnRadius);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, _speed);
	//adjust the right wheel for a bend
	analogWrite(_rightWheelPins[0], _speed);
	//wait until to reach the target
	delay(waitValue);
	//restart the motion
	analogWrite(_leftWheelPins[0], _speed);
}

//Method to make a left turn with the passed degree**** value
/*
 
Turn does not stop the machine, it gets the current speed value and make a appropriate turn
 */
void DC2Wheels::leftTurn(int degree) {
	if (degree < 0) {
		degree = 0;
	}
	//Get the radians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement
	double displacement = getCircleDisplacement(radiansVal, _turnRadius);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, _speed);
	//adjust the right wheel for a bend
	analogWrite(_rightWheelPins[0], _speed);
	//wait until to reach the target
	delay(waitValue);
	//restart the motion
	analogWrite(_leftWheelPins[0], _speed);
}

//Method to spin the machine in its own axis, the sign of param will determine the spin direction
void DC2Wheels::spin(int degree) {
	//Get the raidians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement now using the bend radius, it will make the machine spin in itself
	double displacement = getCircleDisplacement(radiansVal, (_turnRadius / 2));
	//by defaul the spin is made with a half maximum speed
	int speed = STOPPED / 2;
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//from the sign of passed param choice the right direction
	if (degree < 0) {
		analogWrite(_rightWheelPins[0], speed);
		analogWrite(_leftWheelPins[0], speed * -1);

	} else {
		analogWrite(_rightWheelPins[0], speed * -1);
		analogWrite(_leftWheelPins[0], speed);
	}

	//wait until to reach the target
	delay(waitValue);
	//STOP THE MACHINE - FOR THE TIME BEING
	stop();
}

//Method to spin the machine in its own axis, the sign of param will determine the spin direction and in a certain speed*
void DC2Wheels::spin(int degree, int speed) {
	speed = speedFormatter(speed);
	spin(degree);
}

//Method to get the speed value
int DC2Wheels::getSpeed() {
	return _speed;
}

//Method to get direction value
int DC2Wheels::getDirection() {
	return _direction;
}

//Method to get the turn smooth value
float DC2Wheels::getTurnSmooth() {
	return _turnSmooth;
}

//Method to set the turn smooth***** value
void DC2Wheels::setTurnSmooth(float smooth) {
	if (smooth < 0.0) {
		smooth = 0.0;
	} else if (smooth > 1.0) {
		smooth = 1.0;
	}
	_turnSmooth = smooth;
}

/********************************************************************
 * IMPLEMENTATION OF PRIVATE METHODS
 ********************************************************************/
//Private method used to calculate the speed value to a specific wheel
int DC2Wheels::getWheelSpeed(int speed, int wheel) {
	switch (_direction) {
	default:
	case IDLE:
		return STOPPED;
		break;
	case FORWARD:
		if (wheel == RIGHTWHEEL) {
			return STOPPED + speed;
		} else {
			return STOPPED - speed;
		}
		break;
	case BACKWARD:
		if (wheel == RIGHTWHEEL) {
			return STOPPED - speed;
		} else {
			return STOPPED + speed;
		}
		break;
	}
}

//Method to convert degrees to radians
double DC2Wheels::degreeToRadian(int degree) {
	return (RAD * degree);
}

//Method to calculate the circle "for wheels" displacement
double DC2Wheels::getCircleDisplacement(double radiansV, double radius) {
	return (radiansV * radius);
}

//Method to calculate the machine distance displacement
double DC2Wheels::getWalkDisplacement(double distance) {
	unsigned int degree = (360 * distance) / circleLength(_wheelRadius);
	double radiansV = degreeToRadian(degree);
	return getCircleDisplacement(radiansV, _wheelRadius);
}

//Method to calculate the length  for an circle
double DC2Wheels::circleLength(double radius) {
	return 2 * PI * radius;
}

//Method to calculate among of time to wait until the displacement is walked
unsigned long DC2Wheels::getWaitValue(double displacement, int speed) {
	return ((((displacement / speed) * MILI) * _resistence) * (1 + _turnSmooth));
}

//Method to format the speed to a valid speed value
int DC2Wheels::speedFormatter(int speed) {
	if (speed < 0) {
		speed = 0;
	} else if (speed > 100) {
		speed = 100;
	}
	return (speed * MAXIMUM_SPEED) / 100;
}
