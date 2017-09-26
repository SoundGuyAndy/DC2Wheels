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
#ifndef DC2WHELLS_H
#define DC2WHELLS_H
//C headers
#include <inttypes.h>
#include <Arduino.h> // Header to work with native proprietes from Arduino plataform "http://www.arduino.cc" 1.0
// #include <WProgram.h> // Header to work with native proprietes from Arduino plataform "http://www.arduino.cc" later
/********************************************************************
 * CONSTANTS
 ********************************************************************/
// helpers value
#define MILI 1000 //value for 1 second in milliseconds
#define STOPED 90 //Value in which the continuous Servo motors is stopped, its variation is between 0 to 180
#define PI 3.14159265 //Math PI value
#define RAD 0.017453292 //Math RADIANS value
//machine wheels reference
#define RIGHTWHEEL 1 //right wheel
#define LEFTWHEEL 2 //left wheel
//machine motion state
#define IDLE 0 //It sights machine is stopped
#define FORWARD 1 //It sights machine is moving to forward
#define REVERSE 2 //It sights machine is moving to backward
#define RESISTENCE_DEFAULT 0 //Perfect world resistance
#define TURN_SMOOTH_DEFAULT 0.0 //Bend smooth value default does't attenuate nothing
#define MAXIMUM_SPEED 255 // Max duty cycle for PWM
#define MINIMUM_SPEED 0 //Minimum duty cycle for PWM value of *STOPED
#define FRONT_POSITION 0 //Sight the array position to indicate front pin for well
#define BACK_POSITION 1 //Sight the array position to indicate back pin for well
/********************************************************************
 * DECLARATIONS
 ********************************************************************/
class DC2Wheels {
public:
	int _speed; //Holds the actual machine' speed
	int _direction; //Holds the actual motion direction of machine
	double _resistance; //Holds the resistance from middle into the wheels, in a ideal scenery it is zero
	float _bendSmooth; //Holds the smooth value to attenuate bends movies

	/*Both constructors needs: the Arduino's pin in which the right Servo is plugged in,
	 the Arduino's pin in which the left Servo is plugged in,
	 The Radius of machine's wheels,
	 The Turn Radius, Turn radius is the length value from one wheel to another, the machine shaft length

	 The first constructor to work in a ideal scenery, without resistance, the second needs to set a resistance value,
	 and the last sets turn smooth too
	 */
	DC2Wheels(int *rightWhreelPins, int *leftWhreelPins, double wheellRadius,
			double turnRadius);
	DC2Wheels(int *rightWhreelPins, int *leftWhreelPins, double wheellRadius,
			double turnRadius, double resistance);
	DC2Wheels(int *rightWhreelPins, int *leftWhreelPins, double wheellRadius,
			double turnRadius, double resistance, float turnSmooth);

	/*
	 speed* must be higher or equal to 0 and lower or equal to 100, is a percent
	 distance** is a value in centimeter
	 degree*** is a value positive or negative different of 0, the sigh determines the direction
	 degree**** is a value higher then 0
	 smooth***** percentage value

	 Ps:. Every time you use one of the motion methods the value of _speed and _direction are dynamically changed
	 */

	//Motion methods
	void stop(); //Method to stop the machine
	void forward(int speed); //Method to move the machine forward in a certain speed*
	void forward(int speed, double distance); //Method to move the machine forward in a certain speed* to a distance*
	void reverse(int speed); //Method to move the machine backward in a certain speed*
	void reverse(int speed, double distance); //Method to move the machine backward in a certain speed* to a distance*
	void turn(int degree); //Method to make a bend with the passed degree*** value, the degree sigh will determine the bend side
	void rightTurn(int degree); //Method to make a right bend with the passed degree**** value
	void leftTurn(int degree); //Method to make a left bend with the passed degree**** value
	void spin(int degree); //Method to spin the machine in its own axis, the sign of param will determine the spin direction
	void spin(int degree, int speed); //Method to spin the machine in its own axis, the sign of param will determine the spin direction and in a certain speed*
	//Auxiliary methods
	int getSpeed(); //Method to get the speed value
	int getDirection(); //Method to get direction value
	float getTurnSmooth(); //Method to get the bend smooth value
	void setTurnSmooth(float smooth); //Method to set the bend smooth***** value

private:
	int *_rightWheelPins; //Holds the pins which will work with the right wheel
	int *_leftWheelPins; //Holds the pins object which will work with the left wheel
	double _wheelRadius; //Holds the wheel radios value
	double _turnRadius; //Holds the bend radius value

	int getWheelSpeed(int speed, int wheel); //Private method used to calculate the speed value to a specific wheel
	double degreeToRadian(int degree); //Method to convert degrees to radians
	double getCircleDisplacement(double radians, double radius); //Method to calculate the circle "for wheels" displacement
	double getWalkDisplacement(double distance); //Method to calculate the machine distance displacement
	double circleLength(double radius); //Method to calculate the length  for an circle
	unsigned long getWaitValue(double desplacement, int speed); //Method to calculate among of time to wait until the displacement is walked
	int speedFormatter(int speed); //Method to format the speed to a valid speed value
};
#endif
