#include "sample4.h"
#define burm_stmt_NT 1
#define burm_reg_NT 2
#define burm_disp_NT 3
#define burm_rc_NT 4
#define burm_con_NT 5
#define burm___NT 6
extern int burm_max_nt;


void stmt_action(struct burm_state *_s, 

int indent);


void reg_action(struct burm_state *_s, 

int indent);


void disp_action(struct burm_state *_s, 

int indent);


void rc_action(struct burm_state *_s, 

int indent);


void con_action(struct burm_state *_s, 

int indent);
#ifndef ALLOC
#define ALLOC(n) malloc(n)
#endif

#ifndef burm_assert
#define burm_assert(x,y) if (!(x)) {  y; abort(); }
#endif

static NODEPTR burm_np;

#define burm_stmt_NT 1
#define burm_reg_NT 2
#define burm_disp_NT 3
#define burm_rc_NT 4
#define burm_con_NT 5
#define burm___NT 6
extern int burm_max_nt;
int burm_max_nt = 6;

std::string burm_ntname[] = {
  "",
  "stmt",
  "reg",
  "disp",
  "rc",
  "con",
  "_",
  ""
};

static short burm_nts_0[] = { burm___NT, burm___NT, burm___NT, 0 };
static short burm_nts_1[] = { burm_disp_NT, burm_reg_NT, 0 };
static short burm_nts_2[] = { burm_reg_NT, 0 };
static short burm_nts_3[] = { burm_reg_NT, burm_rc_NT, 0 };
static short burm_nts_4[] = { burm_disp_NT, 0 };
static short burm_nts_5[] = { 0 };
static short burm_nts_6[] = { burm_reg_NT, burm_con_NT, 0 };
static short burm_nts_7[] = { burm_con_NT, 0 };

short *burm_nts[] = {
  burm_nts_0,  /* 0 */
  burm_nts_1,  /* 1 */
  burm_nts_2,  /* 2 */
  burm_nts_3,  /* 3 */
  burm_nts_4,  /* 4 */
  burm_nts_5,  /* 5 */
  burm_nts_4,  /* 6 */
  burm_nts_6,  /* 7 */
  burm_nts_5,  /* 8 */
  burm_nts_7,  /* 9 */
  burm_nts_2,  /* 10 */
  burm_nts_5,  /* 11 */
  burm_nts_5,  /* 12 */
};

char burm_arity[] = {
  3,  /* 0=BURP */
  2,  /* 1=ADDI */
  0,  /* 2=ADDRLP */
  2,  /* 3=ASGNI */
  0,  /* 4=CNSTI */
  1,  /* 5=CVCI */
  0,  /* 6=I0I */
  1,  /* 7=INDIRC */
};

std::string burm_opname[] = {
  /* 0 */  "BURP",
  /* 1 */  "ADDI",
  /* 2 */  "ADDRLP",
  /* 3 */  "ASGNI",
  /* 4 */  "CNSTI",
  /* 5 */  "CVCI",
  /* 6 */  "I0I",
  /* 7 */  "INDIRC",
};


std::string burm_string[] = {
  /* 0 */  "stmt: BURP(_,_,_)",
  /* 1 */  "stmt: ASGNI(disp,reg)",
  /* 2 */  "stmt: reg",
  /* 3 */  "reg: ADDI(reg,rc)",
  /* 4 */  "reg: CVCI(INDIRC(disp))",
  /* 5 */  "reg: I0I",
  /* 6 */  "reg: disp",
  /* 7 */  "disp: ADDI(reg,con)",
  /* 8 */  "disp: ADDRLP",
  /* 9 */  "rc: con",
  /* 10 */  "rc: reg",
  /* 11 */  "con: CNSTI",
  /* 12 */  "con: I0I",
};


std::string burm_files[] = {
"sample4.brg",
};

int burm_file_numbers[] = {
  /* 0 */  0,
  /* 1 */  0,
  /* 2 */  0,
  /* 3 */  0,
  /* 4 */  0,
  /* 5 */  0,
  /* 6 */  0,
  /* 7 */  0,
  /* 8 */  0,
  /* 9 */  0,
  /* 10 */  0,
  /* 11 */  0,
  /* 12 */  0,
};

int burm_line_numbers[] = {
  /* 0 */  70,
  /* 1 */  76,
  /* 2 */  87,
  /* 3 */  97,
  /* 4 */  108,
  /* 5 */  118,
  /* 6 */  127,
  /* 7 */  137,
  /* 8 */  148,
  /* 9 */  157,
  /* 10 */  167,
  /* 11 */  177,
  /* 12 */  186,
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

static short burm_decode_stmt[] = {
   -1,
  0,
  1,
  2,
};

static short burm_decode_reg[] = {
   -1,
  3,
  4,
  5,
  6,
};

static short burm_decode_disp[] = {
   -1,
  7,
  8,
};

static short burm_decode_rc[] = {
   -1,
  9,
  10,
};

static short burm_decode_con[] = {
   -1,
  11,
  12,
};

static short burm_decode__[] = {
   -1,
};

int burm_rule(struct burm_state *state, int goalnt) {
  burm_assert(goalnt >= 1 && goalnt <= 6,
        PANIC("Bad goal nonterminal %d in burm_rule\n", goalnt));
  if (!state)
    return 0;
  switch (goalnt) {
  case burm_stmt_NT:  return burm_decode_stmt[((struct burm_state *)state)->rule.burm_stmt];
  case burm_reg_NT:  return burm_decode_reg[((struct burm_state *)state)->rule.burm_reg];
  case burm_disp_NT:  return burm_decode_disp[((struct burm_state *)state)->rule.burm_disp];
  case burm_rc_NT:  return burm_decode_rc[((struct burm_state *)state)->rule.burm_rc];
  case burm_con_NT:  return burm_decode_con[((struct burm_state *)state)->rule.burm_con];
  case burm___NT:  return burm_decode__[((struct burm_state *)state)->rule.burm__];
  default:
    burm_assert(0, PANIC("Bad goal nonterminal %d in burm_rule\n", goalnt));
  }
  return 0;
}


struct burm_action {
  int nt;
  struct burm_state* st;
};

#ifndef RULE
#define RULE(n,s) \
    (act = (burm_action*) malloc(sizeof(struct burm_action)),act->nt=n,act->st=s,act)
#endif

int burm_cost_code(COST *_c, int _ern,struct burm_state *_s)
{
  NODEPTR *_children;
  struct burm_action *act;
  switch(_ern){
  default:
    DEFAULT_COST;
  case 0:
    _children = GET_KIDS(_s->node);
{


 return 1; 
}
  break;
  case 1:
{


 (*_c).cost=_s->kids[0]->cost[burm_disp_NT].cost+_s->kids[1]->cost[burm_reg_NT].cost+1; 
}
  break;
  case 2:
{


 (*_c).cost=_s->cost[burm_reg_NT].cost; 
}
  break;
  case 3:
{


 (*_c).cost=_s->kids[0]->cost[burm_reg_NT].cost+_s->kids[1]->cost[burm_rc_NT].cost+1; 
}
  break;
  case 4:
{


 (*_c).cost = _s->kids[0]->kids[0]->cost[burm_disp_NT].cost+1; 
}
  break;
  case 5:
{


 (*_c).cost=0; 
}
  break;
  case 6:
{


 (*_c).cost=_s->cost[burm_disp_NT].cost+1; 
}
  break;
  case 7:
{


 (*_c).cost=_s->kids[0]->cost[burm_reg_NT].cost+_s->kids[1]->cost[burm_con_NT].cost; 
}
  break;
  case 8:
{


 (*_c).cost=0; 
}
  break;
  case 9:
{


 (*_c).cost=_s->cost[burm_con_NT].cost; 
}
  break;
  case 10:
{


 (*_c).cost=_s->cost[burm_reg_NT].cost; 
}
  break;
  case 11:
{


 (*_c).cost=0; 
}
  break;
  case 12:
{


 (*_c).cost=0; 
}
  break;
  }
  return 1;
}


void stmt_action(struct burm_state *_s, 

int indent);


void reg_action(struct burm_state *_s, 

int indent);


void disp_action(struct burm_state *_s, 

int indent);


void rc_action(struct burm_state *_s, 

int indent);


void con_action(struct burm_state *_s, 

int indent);


#include <stdarg.h>

void burm_exec(struct burm_state *state, int nterm, ...) 
{
  va_list(ap);
  va_start(ap,nterm);

  burm_assert(nterm >= 1 && nterm <= 6,
        PANIC("Bad nonterminal %d in $exec\n", nterm));

  if (state)
    switch (nterm) {
    case burm_stmt_NT:
      stmt_action(state,va_arg(ap,int));
      break;
    case burm_reg_NT:
      reg_action(state,va_arg(ap,int));
      break;
    case burm_disp_NT:
      disp_action(state,va_arg(ap,int));
      break;
    case burm_rc_NT:
      rc_action(state,va_arg(ap,int));
      break;
    case burm_con_NT:
      con_action(state,va_arg(ap,int));
      break;
    default:
      PANIC("Bad nonterminal %d in $exec\n", nterm);
      break;
    }
  else
    PANIC("Bad state for $exec in nonterminal %d \n",nterm);
  va_end(ap);
}

#define EXEC(s,n,a) ( \
  (n == burm_stmt_NT)? burm_exec(s,n,a): \
  (n == burm_reg_NT)? burm_exec(s,n,a): \
  (n == burm_disp_NT)? burm_exec(s,n,a): \
  (n == burm_rc_NT)? burm_exec(s,n,a): \
  (n == burm_con_NT)? burm_exec(s,n,a): \
  PANIC("Bad nonterminal %d in $exec\n", n))

struct burm_state *burm_immed(struct burm_state *s,int n);
#ifndef NO_ACTION
#define NO_ACTION assert(0)
#endif


void stmt_action(struct burm_state *_s, 

int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_stmt[_s->rule.burm_stmt];
  NODEPTR *_children;
  if(_s->rule.burm_stmt==0)
    NO_ACTION(stmt);
  switch(_ern){
  case 0:
    _children = GET_KIDS(_s->node);
{



		stmt_action(burm_immed(_s,0),10);
	
}
  break;
  case 1:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		disp_action(_s->kids[0],indent+1);
		reg_action(_s->kids[1],indent+1);
	
}
  break;
  case 2:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		reg_action(_s,indent+1);
	
}
  break;
  }
}


void reg_action(struct burm_state *_s, 

int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_reg[_s->rule.burm_reg];
  NODEPTR *_children;
  if(_s->rule.burm_reg==0)
    NO_ACTION(reg);
  switch(_ern){
  case 3:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		reg_action(_s->kids[0],indent+1);
		rc_action(_s->kids[1],indent+1);
	
}
  break;
  case 4:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		disp_action(_s->kids[0]->kids[0],indent+1);
	
}
  break;
  case 5:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
	
}
  break;
  case 6:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		disp_action(_s,indent+1);
	
}
  break;
  }
}


void disp_action(struct burm_state *_s, 

int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_disp[_s->rule.burm_disp];
  NODEPTR *_children;
  if(_s->rule.burm_disp==0)
    NO_ACTION(disp);
  switch(_ern){
  case 7:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		reg_action(_s->kids[0],indent+1);
		con_action(_s->kids[1],indent+1);
	
}
  break;
  case 8:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
	
}
  break;
  }
}


void rc_action(struct burm_state *_s, 

int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_rc[_s->rule.burm_rc];
  NODEPTR *_children;
  if(_s->rule.burm_rc==0)
    NO_ACTION(rc);
  switch(_ern){
  case 9:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		con_action(_s,indent+1);
	
}
  break;
  case 10:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
		reg_action(_s,indent+1);
	
}
  break;
  }
}


void con_action(struct burm_state *_s, 

int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_con[_s->rule.burm_con];
  NODEPTR *_children;
  if(_s->rule.burm_con==0)
    NO_ACTION(con);
  switch(_ern){
  case 11:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
	
}
  break;
  case 12:
{



		int i;
		for (i = 0; i < indent; i++)
			std::cerr << " ";
		std::cerr << burm_string[_ern] << "\n";
	
}
  break;
  }
}
static void burm_closure_reg(struct burm_state *, COST);
static void burm_closure_disp(struct burm_state *, COST);
static void burm_closure_con(struct burm_state *, COST);

static void burm_closure_reg(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,10,s) && COST_LESS(c,s->cost[burm_rc_NT])) {
burm_trace(burm_np, 10, c);     s->cost[burm_rc_NT] = c ;
    s->rule.burm_rc = 2;
  }
  if(burm_cost_code(&c,2,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 2, c);     s->cost[burm_stmt_NT] = c ;
    s->rule.burm_stmt = 3;
  }
}

static void burm_closure_disp(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,6,s) && COST_LESS(c,s->cost[burm_reg_NT])) {
burm_trace(burm_np, 6, c);     s->cost[burm_reg_NT] = c ;
    s->rule.burm_reg = 4;
    burm_closure_reg(s, c );
  }
}

static void burm_closure_con(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,9,s) && COST_LESS(c,s->cost[burm_rc_NT])) {
burm_trace(burm_np, 9, c);     s->cost[burm_rc_NT] = c ;
    s->rule.burm_rc = 1;
  }
}

struct burm_state *burm_alloc_state(NODEPTR u,int op,int arity)
{
  struct burm_state *p, **k;
  p = (struct burm_state *)ALLOC(sizeof *p);
  burm_assert(p, PANIC("1:ALLOC returned NULL in burm_alloc_state\n"));
    burm_np = u;
  p->op = op;
  p->node = u;
  if(arity){
    k=(struct burm_state **)ALLOC(arity*sizeof (struct burm_state *));
    burm_assert(k, PANIC("2:ALLOC returned NULL in burm_alloc_state\n"));
    p->kids=k;
  }else
    p->kids=0;
  p->rule.burm_stmt =
  p->rule.burm_reg =
  p->rule.burm_disp =
  p->rule.burm_rc =
  p->rule.burm_con =
  p->rule.burm__ =
    0;
  p->cost[1] =
  p->cost[2] =
  p->cost[3] =
  p->cost[4] =
  p->cost[5] =
  p->cost[6] =
    COST_INFINITY;
  return p;
}
struct burm_state *burm_label1(NODEPTR u) {
  int op, arity, i, immed_matched=0;
  COST c=COST_ZERO;
  struct burm_state *s,**k;
  NODEPTR *children;
  op=OP_LABEL(u);
  arity=burm_arity[op];
  switch(op){
  case 0:		/* BURP */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
        /*immediate rule: stmt: BURP(_,_,_) */
    if(burm_cost_code(&c,0,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 0, c);       s->cost[burm_stmt_NT] = c ;
      s->rule.burm_stmt = 1;
      immed_matched=1;
    }
    if(immed_matched){
      for(i=0;i<arity;i++)k[i]=0;
      return s;
    }
    break;
  case 1:		/* ADDI */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* disp: ADDI(reg,con) */
      k[0]->rule.burm_reg && 
      k[1]->rule.burm_con
    ) {
      if(burm_cost_code(&c,7,s) && COST_LESS(c,s->cost[burm_disp_NT])) {
burm_trace(burm_np, 7, c);         s->cost[burm_disp_NT] = c ;
        s->rule.burm_disp = 1;
        burm_closure_disp(s, c );
      }
    }
    if (   /* reg: ADDI(reg,rc) */
      k[0]->rule.burm_reg && 
      k[1]->rule.burm_rc
    ) {
      if(burm_cost_code(&c,3,s) && COST_LESS(c,s->cost[burm_reg_NT])) {
burm_trace(burm_np, 3, c);         s->cost[burm_reg_NT] = c ;
        s->rule.burm_reg = 1;
        burm_closure_reg(s, c );
      }
    }
    break;
  case 2:		/* ADDRLP */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* disp: ADDRLP */
      if(burm_cost_code(&c,8,s) && COST_LESS(c,s->cost[burm_disp_NT])) {
burm_trace(burm_np, 8, c);         s->cost[burm_disp_NT] = c ;
        s->rule.burm_disp = 2;
        burm_closure_disp(s, c );
      }
    }
    break;
  case 3:		/* ASGNI */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* stmt: ASGNI(disp,reg) */
      k[0]->rule.burm_disp && 
      k[1]->rule.burm_reg
    ) {
      if(burm_cost_code(&c,1,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 1, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 2;
      }
    }
    break;
  case 4:		/* CNSTI */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* con: CNSTI */
      if(burm_cost_code(&c,11,s) && COST_LESS(c,s->cost[burm_con_NT])) {
burm_trace(burm_np, 11, c);         s->cost[burm_con_NT] = c ;
        s->rule.burm_con = 1;
        burm_closure_con(s, c );
      }
    }
    break;
  case 5:		/* CVCI */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* reg: CVCI(INDIRC(disp)) */
      k[0]->op == 7 && 	/* INDIRC */
      k[0]->kids[0]->rule.burm_disp
    ) {
      if(burm_cost_code(&c,4,s) && COST_LESS(c,s->cost[burm_reg_NT])) {
burm_trace(burm_np, 4, c);         s->cost[burm_reg_NT] = c ;
        s->rule.burm_reg = 2;
        burm_closure_reg(s, c );
      }
    }
    break;
  case 6:		/* I0I */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* con: I0I */
      if(burm_cost_code(&c,12,s) && COST_LESS(c,s->cost[burm_con_NT])) {
burm_trace(burm_np, 12, c);         s->cost[burm_con_NT] = c ;
        s->rule.burm_con = 2;
        burm_closure_con(s, c );
      }
    }
    {  		/* reg: I0I */
      if(burm_cost_code(&c,5,s) && COST_LESS(c,s->cost[burm_reg_NT])) {
burm_trace(burm_np, 5, c);         s->cost[burm_reg_NT] = c ;
        s->rule.burm_reg = 3;
        burm_closure_reg(s, c );
      }
    }
    break;
  case 7:		/* INDIRC */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  default:
    burm_assert(0, PANIC("Bad operator %d in burm_state\n", op));
  }
  return s;
}

struct burm_state *burm_label(NODEPTR p) {
  burm_label1(p);
  return ((struct burm_state *)STATE_LABEL(p))->rule.burm_stmt ? STATE_LABEL(p) : 0;
}

void burm_free(struct burm_state *s)
{
  int i,arity=burm_arity[s->op];
  if(s->kids==0)
    free(s);
  else {
    for(i=0;i<arity;i++)
      burm_free(s->kids[i]);
    free(s->kids);
  }
}
struct burm_state *burm_immed(struct burm_state *s,int n)
{
  NODEPTR *children = GET_KIDS(s->node);
  if(s->kids[n])
    return s->kids[n];
  else
  return s->kids[n]=burm_label1(children[n]);
}
int burm_op_label(NODEPTR p) {
  burm_assert(p, PANIC("NULL tree in burm_op_label\n"));
  return OP_LABEL(p);
}

struct burm_state *burm_state_label(NODEPTR p) {
  burm_assert(p, PANIC("NULL tree in burm_state_label\n"));
  return STATE_LABEL(p);
}

NODEPTR burm_child(NODEPTR p, int index) {
  NODEPTR *kids;
  burm_assert(p, PANIC("NULL tree in burm_child\n"));
  kids=GET_KIDS(p);
  burm_assert((0<=index && index<burm_arity[OP_LABEL(p)]),
    PANIC("Bad index %d in burm_child\n", index));

  return kids[index];
}
NODEPTR *burm_kids(NODEPTR p, int eruleno, NODEPTR kids[]) {
  burm_assert(p, PANIC("NULL tree in burm_kids\n"));
  burm_assert(kids, PANIC("NULL kids in burm_kids\n"));
  switch (eruleno) {
  case 0: /* stmt: BURP(_,_,_) */
    kids[0] = burm_child(p,0);
    kids[1] = burm_child(p,1);
    kids[2] = burm_child(p,2);
    break;
  case 7: /* disp: ADDI(reg,con) */
  case 3: /* reg: ADDI(reg,rc) */
  case 1: /* stmt: ASGNI(disp,reg) */
    kids[0] = burm_child(p,0);
    kids[1] = burm_child(p,1);
    break;
  case 10: /* rc: reg */
  case 9: /* rc: con */
  case 6: /* reg: disp */
  case 2: /* stmt: reg */
    kids[0] = p;
    break;
  case 4: /* reg: CVCI(INDIRC(disp)) */
    kids[0] = burm_child(burm_child(p,0),0);
    break;
  case 12: /* con: I0I */
  case 11: /* con: CNSTI */
  case 8: /* disp: ADDRLP */
  case 5: /* reg: I0I */
    break;
  default:
    burm_assert(0, PANIC("Bad external rule number %d in burm_kids\n", eruleno));
  }
  return kids;
}

void dumpCover(NODEPTR p, int goalnt, int indent)
{
  int eruleno = burm_rule(STATE_LABEL(p), goalnt);
  short *nts = burm_nts[eruleno];
  NODEPTR kids[10];
  int i;

  std::cerr << "\t\t";
  for (i = 0; i < indent; i++)
    std::cerr << " ";
  std::cerr << burm_string[eruleno] << "\n";
  burm_kids(p, eruleno, kids);
  for (i = 0; nts[i]; i++)
    dumpCover(kids[i], nts[i], indent + 1);
}


#pragma GCC diagnostic pop



/* burm_trace - print trace message for matching p */
static void burm_trace(NODEPTR p, int eruleno, COST cost) {
	if (shouldTrace)
		std::cerr << "0x" << p << " matched " << burm_string[eruleno] << " = " << eruleno << " with cost " << cost.cost << "\n";
}

static void gen(NODEPTR p) {
	if (burm_label(p) == 0)
		std::cerr << "no cover\n";
	else {
		stmt_action(p->x.state,0);
		if (shouldCover != 0)
			dumpCover(p, 1, 0);
	}
}

static Tree tree(int op, Tree l, Tree r) {
	Tree t = (Tree) malloc(sizeof *t);

	t->op = op;
	t->kids[0] = l; t->kids[1] = r;
	t->val = 0;
	t->x.state = 0;
	return t;
}

int main(int argc, char *argv[]) {
	Tree t;
	int i;

	for (i = 1; i < argc ; ++i) {
		if (strcmp(argv[i], "--cover") == 0){
			shouldCover = 1;
		}
		if (strcmp(argv[i], "--trace") == 0){
			shouldTrace = 1;
		}
	}
	printf("i = c + 4;\n");
	t = tree(ASGNI,
		tree(ADDRLP, 0, 0),
		tree(ADDI,
			tree(CVCI, tree(INDIRC, tree(ADDRLP, 0, 0), 0), 0),
			(t = tree(CNSTI, 0, 0), t->val = 4, t)
		)
	);
	gen(t);
	return 0;
}
