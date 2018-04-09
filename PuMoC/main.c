/****************************************************************************/
/* main.c                                                                   */
/*                                                                          */
/* Main function of the model checker.					    */
/*									    */
/* Stefan Schwoon, February 2000					    */
/*                                             */
/* Fu Song 2011       */
/*                     */                                                      
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <signal.h> 
#include <sys/time.h>

#include "ba.h"
#include "fatrans.h"
#include "heads.h"
#include "common.h"
#include "reorder.h"
#include "netconv.h"
#include "cycle.h"
#include "expr.h"
#include "abpds.h"
#include "afa.h"



#define isdigit(c) ((c >= '0') && (c <= '9'))
#define isupper(c) ((c >= 'A') && (c <= 'Z'))

#define INTERVAL   1800  
#define product_buffer_size 256
/****************************************************************************/
int isdebug=0;
int mc_result=0;
BuechiAutomaton	mc_ba;
FATransTable	mc_trans, mc_index;
HeadTable	mc_heads;
PushdownSystem	mc_pds;
AlternatingFiniteAutomata afa;
AlternatingBuchiPushdownSystem	mc_abpds;
block_t		mc_blocks;
NameTree	mc_localtree;
NameTreeList	mc_atoms;
Head		mc_starthead;
Name		mc_reach_target_label;
extern ValList abpdsValList;
extern FILE *file;

extern int line;
int		mc_displayfa = 0;
int		mc_stats = 1;
int		mc_verbose = 0;
int		mc_trace = 0;
int		mc_poststar = 1;
int		mc_demand = 1;
int		mc_sstate;
int		mc_restrict = 0;
int		mc_bp = 0;
int		mc_reach = 0;
int		mc_sepvar = 0;
int		mc_elimdead = 0;
int		mc_globals = 0;
int		mc_stop_after_post = 0;
int		mc_queue = 0;
int		mc_elimstuff = 1;
int		mc_encoding = 0;
int		mc_use_ba = 0;
int   mc_ctl_read_from_file = 0;
int   mc_logic_lang_CTL = 0;
int   mc_check_emptiness = 0;
int		mc_CTPL = 0;

float	mctime_read = 0, mctime_post = 0, mctime_pre2 = 0;
float	mctime_head = 0, mctime_pre3 = 0, mctime_total = 0;
float mctime_fixpoint =0, mctime_product=0;  


char* bp_trace_fileprefix;

extern const_t	constlist;

extern void compute_poststar ();
extern void compute_prestar ();
extern void build_ba (char*);
extern void parse_pds (char*);
extern void parse_bp (char*);
extern void define_value (char*);
extern void build_abpds(char*);
extern void build_ctl_formula(char*);
extern void ABPDSPrepareForModelChecking(AlternatingBuchiPushdownSystem*);
extern void compute_fixpoint(AlternatingBuchiPushdownSystem*);
extern void parse_abpds(char*);

extern Name sharp;
extern FormulaTree ctlTree;
extern int state_num;
extern int stack_num;
extern int abpds_trans_num;
extern int afa_trans_num_max;

extern int ctlQuantVarNum;
extern int abpdsValNum;
extern char* product_buffer;
/****************************************************************************/

void usage(char *myname)
{
  char *progname = myname;
  char *base = strrchr(progname,'/');
  if (base) progname = base+1;


  fprintf(stderr,
    "\n%s -- model checker for pushdown systems\n\n"

    "Usage: %s [options] <modelfile> <formula>\n\n"

    "    Options:\n"
    "    -a\t	enable automatic reordering (-aa: only during build)\n"
    "    -b\t	read a boolean program (sets -m2v2e0)\n"
    "    -c<..>	choose method for detecting cycles\n"
    "    -e<n>	choose encoding method for boolean programs (n=0,1)\n"
    "    -f\t	read <formula> as name of file containing Buchi automaton (LTL) or CTL formula\n"
    "    -m<n>	choose initial ordering (n=0..2)\n"
    "    -o<n>	enable live variable/mod-ref analysis\n"
    "    -p<..>	computation method (B=pre*, F=post*, D=demand, E=exhaustive)\n"
    "    -r<n>	compute reachability (n=1,2,3: print automaton)\n"
    "    -s<n>	set verbosity\n"
    "    -t\t	print counterexample (-tP = PDS, -tN = Newton, -tX = XML)\n"
    "    -v<n>	choose bdd variable allocation (n=0..2)\n"
    "    -Dx=n:	define a value\n"
    "    -d\t print debug information\n"
    "    -l<n>:	specific specification logic (n=0,1,2) 0=ltl, 1=ctl, 2=check emptiness of the ABPDS, defalut is 0.\n",

    progname, progname);  

  fprintf(stderr,
    "\nCopyright (c) Fu SONG <songfu1983@gmail.com>, parts of copyright (c) Stefan Schwoon <schwoon@in.tum.de>\n"
    "See Copying for legal information.\n"
    "\nVersion 1.0.0 (10.10.2011)\n\n");

  exit(1);
}

 
/*****************************************************************************/
/* Main program.                                                             */
/*****************************************************************************/



int main(int argc, char **argv)
{
	file= fopen("abpds.txt","w");
	int		i, mc_print_cudd = 0;
	char		*src = NULL, *formfile = NULL;
	char    	**dptr = &src;
	/* no arguments? display usage */
	if (argc == 1) usage(argv[0]);
	nc_rundir = nc_get_path(argv[0]);

	common_resource_init();
	common_resource_setmark(1);
	
  product_buffer=common_malloc(product_buffer_size*sizeof(char));

	name_global_empty();
	constlist = NULL;
	mc_reorderproc = reorder_lgbitvarcopy;
	mc_cyclemethod = owcty_method;
	mc_edgemethod = find_direct;

	/* traverse arguments, get filenames */
	for (i = 1; i < argc; i++)
	{
	  if (*argv[i] == '-')
	  {
			while (*++argv[i])
		    switch (*argv[i])
				{
		    case 'b': mc_bp++; mc_sepvar = 2;
			      mc_reorderproc = bp_reorder_dep;
			      break;
		    case 'r': mc_reach = 1;
			      if (isdigit(argv[i][1]))
				      { argv[i]++; mc_reach = *argv[i] - '0' + 1; }
			      break;
		    case 'D': define_value(argv[i]);
			      argv[i][1] = 0; break;
			  case 'd':
			  	    isdebug=1;
			  	break;
		    case 't': mc_trace++;
			      if (argv[i][1] == 'P')
				{ mc_trace = 1; argv[i]++; }
			      else if (argv[i][1] == 'S')
				{ mc_trace = mc_stop_after_post = 1;argv[i]++; }
			      else if (argv[i][1] == 'N')
				{ mc_trace = 2; argv[i]++; }
			      else if (argv[i][1] == 'X')
				{ mc_trace = 3; argv[i]++; }
			      break;

		    case 'p': while (isupper(argv[i][1]))
			      {
				switch (argv[i][1])
				{
				  case 'F': mc_poststar = 1; break;
				  case 'B': mc_poststar = 0; break;
				  case 'D': mc_demand = 1; break;
				  case 'E': mc_demand = 0; break;
				  default:  usage(argv[0]);
				}
				argv[i]++;
			      }
			      break;

		    case 'e': mc_encoding = argv[i][1] - '0';
			      argv[i]++; break;

		    case 'q': mc_queue = argv[i][1] - '0';
			      argv[i]++; break;
		    case 'c': while (isupper(argv[i][1]))
			      {
				switch (argv[i][1])
				{
				  case 'T': mc_elimstuff = 1; break;
				  case 'N': mc_elimstuff = 0; break;
				  case 'S': mc_edgemethod = find_summary; break;
				  case 'D': mc_edgemethod = find_direct; break;
				  case 'C': mc_cyclemethod = compute_closure;
					    break;
				  case 'X': mc_cyclemethod = xb_decomp;
					    break;
				  case 'E': mc_cyclemethod = el_method;
					    break;
				  case 'O': mc_cyclemethod = owcty_method;
					    break;
				  default:  usage(argv[0]);
				}
				argv[i]++;
			      }
			      break;

		    case 'v': mc_sepvar = argv[i][1] - '0';
			      argv[i]++; break;
		    case 'm': if (!isdigit(argv[i][1])) break;
			      if (argv[i][1] == '0' && !mc_bp)
				mc_reorderproc = reorder_lgbitvarcopy;
			      else if (argv[i][1] == '1')
				mc_reorderproc = reorder_lgvarbitcopy;
			      else if (argv[i][1] == '2' && mc_bp)
				mc_reorderproc = bp_reorder_dep;
			      else
				usage(argv[0]);
			      argv[i]++; break;
		    case 'a': mc_autoreorder++; break;

		    case 'f': mc_use_ba++; mc_ctl_read_from_file++; break;

		    case 'o': { int lvl = 3;
				if (isdigit((int)argv[i][1]))
				  { lvl = argv[i][1]-'0'; argv[i]++; }
				mc_elimdead = (lvl & 2);
				mc_globals = (lvl & 1);
				break;
			      }

		    case 's': { int lvl = 1;
				if (isdigit((int)argv[i][1]))
				  { lvl = argv[i][1]-'0'; argv[i]++; }
				mc_stats = (lvl > 0);
				mc_print_cudd = (lvl > 1);
				mc_verbose = (lvl > 1)? lvl-2 : 0;
				break; }
        // Song: added begin
			  case 'l':
			  {
			  	if (isdigit((int)argv[i][1]))
			  	{
			  		switch(argv[i][1]-'0')
			  		{
			  			case 0: break;
			  			case 1: mc_logic_lang_CTL =1;  break;
			  			case 2: mc_check_emptiness =1;  break;
			  			default: usage(argv[0]);
			  		}
			   		argv[i]++;
			   	}
			  	break;
			  }			  //Song: added end
		    default:  usage(argv[0]);
		}
	    }
	    else	/* filenames */
	    {
		if (!dptr)
		{
			common_error("more than two filenames "
				 "on the command line");
			exit(1);
		}
		*dptr = argv[i];

		if (dptr == &src) dptr = &formfile;
		else dptr = NULL;
	    }
	}

	/* check correctness of arguments */
	if (!src || (!formfile && !mc_check_emptiness) ) usage(argv[0]);
	if (mc_bp && mc_reach && mc_trace > 1)
	{
		char *tmp;
		bp_trace_fileprefix = strdup(src);
		tmp = strrchr(bp_trace_fileprefix,'.');
		if (tmp) *tmp = 0;
	}

	mc_sstate = name_create("__s__");

#ifdef YYDEBUG
	{
	  extern int badebug, pdsdebug, bpdebug, ltldebug;
	  badebug = pdsdebug = bpdebug = ltldebug = 1;
	}
#endif

	if (mc_reach)
	{		 
		mc_reach_target_label = name_create(formfile);
		(mc_bp? parse_bp : parse_pds)(src);
		pds_add_reach_target(&mc_pds,formfile);
		if (mc_autoreorder > 1) Cudd_AutodynDisable(mc_mgr);
		mc_poststar? compute_poststar() : compute_prestar();		
	}
	else if (mc_logic_lang_CTL)            //Song: added begin
	{
		build_ctl_formula(formfile);
		(mc_bp? parse_bp : build_abpds)(src); 

		ABPDSPrepareForModelChecking(&mc_abpds);
		compute_fixpoint(&mc_abpds);
		DdNode* bdd =mc_CTPL? CTLFullset : NULL;
		if (mc_abpds.startState && mc_abpds.startStack)
		{	
			
			if(AFACheckMembership(&afa, mc_abpds.startState, ctlTree,0, bdd, mc_abpds.startStack))
			{
				if(mc_bp) printf("\n Y(^o^)Y The Program satisfies the CTL specification. Y(^o^)Y\n\n");
				else
					printf("\n Y(^o^)Y The PDS satisfies the CTL specification. Y(^o^)Y\n\n");
			}
			else
			{
				if(mc_bp) printf("\n (?) The Program does not satisfy the CTL specification. (?)\n\n");		
				else
					printf("\n (?) The PDS does not satisfy the CTL specification. (?)\n\n");		
				if(mc_CTPL&&mc_print_cudd) Cudd_PrintInfo(CTLBDDMgr,stdout);
			}
		}
		else
		{
			AFAPrint(&afa) ;
		}
	//	printf("|P|=%d, |Gamma|=%d, |Delta|=%d, |delta|=%d, |X|=%d,|D|=%d.\n\n", state_num,stack_num,abpds_trans_num,afa_trans_num_max,ctlQuantVarNum,abpdsValNum);
	}
	else 
	{
  	if (mc_check_emptiness)
  	{
  		parse_abpds(src);
  		compute_fixpoint(&mc_abpds);
  		if (mc_abpds.startState && mc_abpds.startStack)
  		{
    		if(AFACheckMembership(&afa, mc_abpds.startState,ctlTree,0, NULL, mc_abpds.startStack))
    		{  			
    			printf("\n Y(^o^)Y The ABPDS has an accepting run from the initial configuration ( %s, < ", name_cstring(mc_abpds.startState));
    		}
    		else
    			printf("\n(?) The ABPDS has not an accepting run from the initial configuration ( %s, < ", name_cstring(mc_abpds.startState));	
    		
    		NameStack tmp_name = NULL;
    		for (tmp_name = mc_abpds.startStack; tmp_name ; tmp_name = tmp_name->next)
    		{
    			if (tmp_name->name == sharp) break;
    			printf("%s ",name_cstring(tmp_name->name));
    		}
    		printf("> )\n\n");
    	}
    	else
    	{
    		AFAPrint(&afa) ;
    	}
  	}                     //Song: added end
  	else
  	{ 
  		build_ba(formfile);
  		(mc_bp? parse_bp : parse_pds)(src);
  			if (mc_autoreorder > 1) Cudd_AutodynDisable(mc_mgr);
  		mc_poststar? compute_poststar() : compute_prestar();
    }
  }

	if (mc_stats || mc_verbose) printf("\n");

	common_resource_displaymark(1, "overall", &mctime_total);

	if (mc_verbose || mc_print_cudd) Cudd_PrintInfo(mc_mgr,stdout);
	if (mc_verbose > 2) print_shuffle();
	if (mc_verbose > 1)
		printf("\n %7.2f %7.2f %7.2f %7.2f %7.2f %8ld %5d %9.2f\n",
			mctime_read, mctime_post, mctime_pre2,
			mctime_head, mctime_pre3,
			Cudd_ReadMemoryInUse(mc_mgr)/1024,
			Cudd_ReadPeakLiveNodeCount(mc_mgr)/1024,
			mctime_total
			);
  common_free(product_buffer,product_buffer_size*sizeof(char));
	return 0;
}
