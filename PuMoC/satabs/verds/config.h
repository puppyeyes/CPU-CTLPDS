/************************************************************************/
/* This is the place for deinition of a SAT solver and a QBF solver;	*/
/* The solvers are to be used as the default solvers for BMC;  		*/
/* If they are not defined, internal (slow) solvers are used. 		*/
/************************************************************************/
/* The solvers are required: 						*/
/* to output "UNSAT" to stdout for unsatifiable instances, and 		*/
/* to output "SATISFIABLE" to stdout for satifiable instances.		*/
/************************************************************************/
#define SATSOLVER "/home/bin/satsolver"
#define QBFSOLVER "/home/bin/qbfsolver"
/************************************************************************/
