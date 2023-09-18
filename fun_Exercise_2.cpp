//#define EIGENLIB			// uncomment to use Eigen linear algebra library

#include "fun_head_fast.h"

// do not add Equations in this area

MODELBEGIN

// insert your equations here, ONLY between the MODELBEGIN and MODELEND words

/////////////////////////
///// AVERAGE PRICE /////
/////////////////////////

EQUATION("ap")

v[0] = 0;
v[2] = 0;

CYCLE(cur, "FIRM")
	{
	v[1] = VS(cur, "p");
	v[0] = v[0] + v[1]; //sum of prices
	v[2] = v[2] + 1;	//count firms
	}

RESULT(v[0]/v[2])

/*
ou
*/

EQUATION("ap_2")
RESULT(AVE("p"))

/////////////////////////
////////// PRICE ////////
/////////////////////////

EQUATION("p")
// p_(i,t) = theta_i*dp_(i,t) + (1 - theta_i)*ap_(t-1)
/*
 where:
 p_(i,t) is the price of firm i in period t;
 theta_i is the degree of monopoly of firm i(parameter);
 dp_(i,t) is the desired price of firm i in period t;
 ap_(t-1) is the average price in period t-1; 
*/

v[0] = V("theta"); // theta is the degree of monopoly of firm i (parameter);
v[1] = V("dp"); // dp is the desired price;
v[2] = AVEL("p", 1);

RESULT(v[0]*v[1] + (1-v[0])*v[2])


/////////////////////////
///// DESIRED PRICE /////
/////////////////////////

EQUATION("dp")
// dp_(i,t) = (1 + �_i)*c_(i,t)
/*
 where:
 �_i is the markup of firm i (parameter);
 c_(i,t) is the cost of firm i in period t;
*/
	
v[0] = V("mu"); // mu is the markup of firm i (parameter);
v[1] = V("c");

RESULT((1 + v[0])*v[1])


/////////////////////////
////////// COST /////////
/////////////////////////

EQUATION("c")
//	 c_(i,t) ~ N(avg, sd)

v[0] = V("avg");
v[1] = V("sd");

RESULT(norm(v[0], v[1]))


/////////////////////////
///// MAXIMUM PRICE /////
/////////////////////////

EQUATION("mp"); //mp is maximum price;

/*
Compute the maximum price among firms in each period
*/

v[0] = 0; // Initialize v[0] to store the maximum price

CYCLE(cur, "FIRM")
	{
	v[1] = VS(cur, "p"); // Find the maximum price among firms in the current sector
	
    // Check if v[1] is greater than the current maximum stored in v[0]
    if (v[1] > v[0])
    	{
		v[0] = v[1]; // Update v[0] with the new maximum price
    	}
	}

RESULT(v[0]) // Return the maximum price for the current period

/*
ou
*/

EQUATION("mp_2");

RESULT(MAX("p"))


/////////////////////////
////////// EXIT /////////
/////////////////////////

EQUATION("exit")

	v[1] = COUNT("FIRM");
	
	if(v[1] > 1)
		{
		v[0] = V("mp");
		cur = SEARCH_CND("p", v[0]);
		DELETE(cur);
		}
		
RESULT(0)


/////////////////////////
///////// ENTRY /////////
/////////////////////////

EQUATION("entry")

	v[1] = V("switch_entry"); // 0 = no entry; 1 = minimum price; 2 = random price.
	if(v[1] == 1)
		{
		v[0] = MIN("p");
		cur = SEARCH_CND("p", v[0]);
		cur2 = ADDOBJ_EX("FIRM", cur);
		WRITES(cur2, "theta", 0.1);
		}
	if(v[1] == 2)
		{
		cur = RNDDRAW_FAIR("FIRM");
		ADDOBJ_EX("FIRM", cur);
		}
		
RESULT(0)



MODELEND

// do not add Equations in this area

void close_sim( void )
{
	// close simulation special commands go here
}