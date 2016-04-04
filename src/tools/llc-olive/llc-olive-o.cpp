#include "llc-olive-o.h"
#define burm_stmt_NT 1
#define burm_op_NT 2
#define burm_var_NT 3
#define burm_const_NT 4
extern int burm_max_nt;


void stmt_action(struct burm_state *_s, 

int indent);


void op_action(struct burm_state *_s, 

int indent);


void var_action(struct burm_state *_s);


void const_action(struct burm_state *_s);
#ifndef ALLOC
#define ALLOC(n) malloc(n)
#endif

#ifndef burm_assert
#define burm_assert(x,y) if (!(x)) {  y; abort(); }
#endif

static NODEPTR burm_np;

#define burm_stmt_NT 1
#define burm_op_NT 2
#define burm_var_NT 3
#define burm_const_NT 4
extern int burm_max_nt;
int burm_max_nt = 4;

std::string burm_ntname[] = {
  "",
  "stmt",
  "op",
  "var",
  "const",
  ""
};

static short burm_nts_0[] = { burm_op_NT, 0 };
static short burm_nts_1[] = { burm_var_NT, 0 };
static short burm_nts_2[] = { burm_const_NT, 0 };
static short burm_nts_3[] = { burm_var_NT, burm_const_NT, 0 };
static short burm_nts_4[] = { burm_var_NT, burm_var_NT, 0 };
static short burm_nts_5[] = { burm_var_NT, burm_op_NT, 0 };
static short burm_nts_6[] = { 0 };

short *burm_nts[] = {
  burm_nts_0,  /* 0 */
  burm_nts_1,  /* 1 */
  burm_nts_2,  /* 2 */
  burm_nts_0,  /* 3 */
  burm_nts_3,  /* 4 */
  burm_nts_4,  /* 5 */
  burm_nts_5,  /* 6 */
  burm_nts_3,  /* 7 */
  burm_nts_4,  /* 8 */
  burm_nts_5,  /* 9 */
  burm_nts_6,  /* 10 */
  burm_nts_6,  /* 11 */
};

char burm_arity[] = {
  0,  /* 0=CONST */
  0,  /* 1=VAR */
  1,  /* 2=RET */
  2,  /* 3=STORE */
  2,  /* 4=OP */
};

std::string burm_opname[] = {
  /* 0 */  "CONST",
  /* 1 */  "VAR",
  /* 2 */  "RET",
  /* 3 */  "STORE",
  /* 4 */  "OP",
};


std::string burm_string[] = {
  /* 0 */  "stmt: op",
  /* 1 */  "stmt: RET(var)",
  /* 2 */  "stmt: RET(const)",
  /* 3 */  "stmt: RET(op)",
  /* 4 */  "stmt: STORE(var,const)",
  /* 5 */  "stmt: STORE(var,var)",
  /* 6 */  "stmt: STORE(var,op)",
  /* 7 */  "op: OP(var,const)",
  /* 8 */  "op: OP(var,var)",
  /* 9 */  "op: OP(var,op)",
  /* 10 */  "var: VAR",
  /* 11 */  "const: CONST",
};


std::string burm_files[] = {
"llc-olive-o.brg",
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
};

int burm_line_numbers[] = {
  /* 0 */  409,
  /* 1 */  415,
  /* 2 */  427,
  /* 3 */  439,
  /* 4 */  451,
  /* 5 */  466,
  /* 6 */  484,
  /* 7 */  499,
  /* 8 */  523,
  /* 9 */  549,
  /* 10 */  575,
  /* 11 */  581,
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

static short burm_decode_stmt[] = {
   -1,
  0,
  1,
  2,
  3,
  4,
  5,
  6,
};

static short burm_decode_op[] = {
   -1,
  7,
  8,
  9,
};

static short burm_decode_var[] = {
   -1,
  10,
};

static short burm_decode_const[] = {
   -1,
  11,
};

int burm_rule(struct burm_state *state, int goalnt) {
  burm_assert(goalnt >= 1 && goalnt <= 4,
        PANIC("Bad goal nonterminal %d in burm_rule\n", goalnt));
  if (!state)
    return 0;
  switch (goalnt) {
  case burm_stmt_NT:  return burm_decode_stmt[((struct burm_state *)state)->rule.burm_stmt];
  case burm_op_NT:  return burm_decode_op[((struct burm_state *)state)->rule.burm_op];
  case burm_var_NT:  return burm_decode_var[((struct burm_state *)state)->rule.burm_var];
  case burm_const_NT:  return burm_decode_const[((struct burm_state *)state)->rule.burm_const];
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
{


 (*_c).setCost(_s->cost[burm_op_NT].getCost()); 
}
  break;
  case 1:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()); 
}
  break;
  case 2:
{


 (*_c).setCost(_s->kids[0]->cost[burm_const_NT].getCost()); 
}
  break;
  case 3:
{


 (*_c).setCost(_s->kids[0]->cost[burm_op_NT].getCost()); 
}
  break;
  case 4:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()+_s->kids[1]->cost[burm_const_NT].getCost()); 
}
  break;
  case 5:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()+_s->kids[1]->cost[burm_var_NT].getCost()); 
}
  break;
  case 6:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()+_s->kids[1]->cost[burm_op_NT].getCost()); 
}
  break;
  case 7:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()+_s->kids[1]->cost[burm_const_NT].getCost()); 
}
  break;
  case 8:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()+_s->kids[1]->cost[burm_var_NT].getCost()); 
}
  break;
  case 9:
{


 (*_c).setCost(_s->kids[0]->cost[burm_var_NT].getCost()+_s->kids[1]->cost[burm_op_NT].getCost()); 
}
  break;
  case 10:
{


 (*_c).setCost(1); 
}
  break;
  case 11:
{


 (*_c).setCost(1); 
}
  break;
  }
  return 1;
}


void stmt_action(struct burm_state *_s, 

int indent);


void op_action(struct burm_state *_s, 

int indent);


void var_action(struct burm_state *_s);


void const_action(struct burm_state *_s);


#include <stdarg.h>

void burm_exec(struct burm_state *state, int nterm, ...) 
{
  va_list(ap);
  va_start(ap,nterm);

  burm_assert(nterm >= 1 && nterm <= 4,
        PANIC("Bad nonterminal %d in $exec\n", nterm));

  if (state)
    switch (nterm) {
    case burm_stmt_NT:
      stmt_action(state,va_arg(ap,int));
      break;
    case burm_op_NT:
      op_action(state,va_arg(ap,int));
      break;
    case burm_var_NT:
      var_action(state);
      break;
    case burm_const_NT:
      const_action(state);
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
  (n == burm_op_NT)? burm_exec(s,n,a): \
  (n == burm_var_NT)? burm_exec(s,n): \
  (n == burm_const_NT)? burm_exec(s,n): \
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
{



        op_action(_s,indent);
      
}
  break;
  case 1:
{



        var_action(_s->kids[0]);
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  ";
        std::cout << realIndent << "mov %rax, " << _s->kids[0]->node->getMemVal() << "\n";
        
        std::cout << realIndent << "ret\n";
      
}
  break;
  case 2:
{



        const_action(_s->kids[0]);
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  ";
        std::cout << realIndent << "mov %rax, " << _s->kids[0]->node->getMemVal() << "\n";
        
        std::cout << realIndent << "ret\n";
      
}
  break;
  case 3:
{



        op_action(_s->kids[0],indent);
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  ";
        
        std::cout << realIndent << "mov %rax, " << _s->kids[0]->node->getMemVal() << "\n";
        std::cout << realIndent << "ret\n";
      
}
  break;
  case 4:
{



        var_action(_s->kids[0]);
        const_action(_s->kids[1]);

        std::string firstVal = _s->kids[0]->node->getMemVal();
        std::string secondVal = _s->kids[1]->node->getMemVal();
        _s->node->setMemVal(false, firstVal);        

        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  "; 
        std::cout << realIndent << "mov " << firstVal << ", " << secondVal << "\n";
      
}
  break;
  case 5:
{



        var_action(_s->kids[0]);
        var_action(_s->kids[1]);
        _s->node->setMemVal(true);        

        std::string newReg = _s->node->getMemVal();
        std::string firstVal = _s->kids[0]->node->getMemVal();
        std::string secondVal = _s->kids[1]->node->getMemVal();
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  "; 

        std::cout << realIndent << "mov " << newReg << ", " << secondVal << "\n";
        std::cout << realIndent << "mov " << firstVal << ", " << newReg << "\n";
      
}
  break;
  case 6:
{



        var_action(_s->kids[0]);
        op_action(_s->kids[1],indent);

        std::string firstVal = _s->kids[0]->node->getMemVal();
        std::string secondVal = _s->kids[1]->node->getMemVal();
        _s->node->setMemVal(false, firstVal);        
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  "; 
        std::cout << realIndent << "mov " << firstVal << ", " << secondVal << "\n";
      
}
  break;
  }
}


void op_action(struct burm_state *_s, 

int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_op[_s->rule.burm_op];
  NODEPTR *_children;
  if(_s->rule.burm_op==0)
    NO_ACTION(op);
  switch(_ern){
  case 7:
{



        var_action(_s->kids[0]);
        const_action(_s->kids[1]);

        std::string firstVal = _s->kids[0]->node->getMemVal();
        std::string secondVal = _s->kids[1]->node->getMemVal();
        _s->node->setMemVal(false, firstVal);        
        
        std::string operation = "";
        switch (_s->node->getOpType()) {
          case ADD: operation = "add"; break;
          case MUL: operation = "mul"; break; 
          case DIV: operation = "div"; break; 
          case REM: operation = "mod"; break; 
        }
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  "; 

        std::cout << realIndent << operation << " " << firstVal << ", " << secondVal << "\n";
      
}
  break;
  case 8:
{



        var_action(_s->kids[0]);
        var_action(_s->kids[1]);
        _s->node->setMemVal(true);        

        std::string newReg = _s->node->getMemVal();
        std::string firstVal = _s->kids[0]->node->getMemVal();
        std::string secondVal = _s->kids[1]->node->getMemVal();
        
        std::string operation = "";
        switch (_s->node->getOpType()) {
          case ADD: operation = "add"; break;
          case MUL: operation = "mul"; break; 
          case DIV: operation = "div"; break; 
          case REM: operation = "mod"; break; 
        }
        
        std::string realIndent = "";
        for (int i=0; i<indent; i++) realIndent += "  "; 

        std::cout << realIndent << "mov " << newReg << ", " << firstVal << "\n";
        std::cout << realIndent << operation << " " << newReg << ", " << secondVal << "\n";
      
}
  break;
  case 9:
{



      var_action(_s->kids[0]);
      op_action(_s->kids[1],indent);
      _s->node->setMemVal(true);   

      std::string realIndent = "";
      for (int i=0; i<indent; i++) realIndent += "  ";

      std::string newReg = _s->node->getMemVal();
      std::string firstVal = _s->kids[0]->node->getMemVal();
      std::string secondVal = _s->kids[1]->node->getMemVal();
           
      std::string operation = "";
      switch (_s->node->getOpType()) {
        case ADD: operation = "add"; break;
        case MUL: operation = "mul"; break; 
        case DIV: operation = "div"; break; 
        case REM: operation = "mod"; break; 
      }
      
      std::cout << realIndent << "mov " << newReg << ", " << firstVal << "\n";
      std::cout << realIndent << operation << " " << newReg << ", " << secondVal << "\n";
    
}
  break;
  }
}


void var_action(struct burm_state *_s)
{
  struct burm_state *_t;
  int _ern=burm_decode_var[_s->rule.burm_var];
  NODEPTR *_children;
  if(_s->rule.burm_var==0)
    NO_ACTION(var);
  switch(_ern){
  case 10:
{



      _s->node->setMemVal(true);
    
}
  break;
  }
}


void const_action(struct burm_state *_s)
{
  struct burm_state *_t;
  int _ern=burm_decode_const[_s->rule.burm_const];
  NODEPTR *_children;
  if(_s->rule.burm_const==0)
    NO_ACTION(const);
  switch(_ern){
  case 11:
{



         if (llvm::ConstantInt::classof(_s->node->getValue())) {
           const llvm::ConstantInt *CI = llvm::cast<llvm::ConstantInt>(_s->node->getValue());
           _s->node->setMemVal(false, std::to_string(CI->getValue().getSExtValue()));
         }
       
}
  break;
  }
}
static void burm_closure_op(struct burm_state *, COST);

static void burm_closure_op(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,0,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 0, c);     s->cost[burm_stmt_NT] = c ;
    s->rule.burm_stmt = 1;
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
  p->rule.burm_op =
  p->rule.burm_var =
  p->rule.burm_const =
    0;
  p->cost[1] =
  p->cost[2] =
  p->cost[3] =
  p->cost[4] =
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
  case 0:		/* CONST */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* const: CONST */
      if(burm_cost_code(&c,11,s) && COST_LESS(c,s->cost[burm_const_NT])) {
burm_trace(burm_np, 11, c);         s->cost[burm_const_NT] = c ;
        s->rule.burm_const = 1;
      }
    }
    break;
  case 1:		/* VAR */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* var: VAR */
      if(burm_cost_code(&c,10,s) && COST_LESS(c,s->cost[burm_var_NT])) {
burm_trace(burm_np, 10, c);         s->cost[burm_var_NT] = c ;
        s->rule.burm_var = 1;
      }
    }
    break;
  case 2:		/* RET */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* stmt: RET(op) */
      k[0]->rule.burm_op
    ) {
      if(burm_cost_code(&c,3,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 3, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 4;
      }
    }
    if (   /* stmt: RET(const) */
      k[0]->rule.burm_const
    ) {
      if(burm_cost_code(&c,2,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 2, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 3;
      }
    }
    if (   /* stmt: RET(var) */
      k[0]->rule.burm_var
    ) {
      if(burm_cost_code(&c,1,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 1, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 2;
      }
    }
    break;
  case 3:		/* STORE */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* stmt: STORE(var,op) */
      k[0]->rule.burm_var && 
      k[1]->rule.burm_op
    ) {
      if(burm_cost_code(&c,6,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 6, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 7;
      }
    }
    if (   /* stmt: STORE(var,var) */
      k[0]->rule.burm_var && 
      k[1]->rule.burm_var
    ) {
      if(burm_cost_code(&c,5,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 5, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 6;
      }
    }
    if (   /* stmt: STORE(var,const) */
      k[0]->rule.burm_var && 
      k[1]->rule.burm_const
    ) {
      if(burm_cost_code(&c,4,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
burm_trace(burm_np, 4, c);         s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 5;
      }
    }
    break;
  case 4:		/* OP */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* op: OP(var,op) */
      k[0]->rule.burm_var && 
      k[1]->rule.burm_op
    ) {
      if(burm_cost_code(&c,9,s) && COST_LESS(c,s->cost[burm_op_NT])) {
burm_trace(burm_np, 9, c);         s->cost[burm_op_NT] = c ;
        s->rule.burm_op = 3;
        burm_closure_op(s, c );
      }
    }
    if (   /* op: OP(var,var) */
      k[0]->rule.burm_var && 
      k[1]->rule.burm_var
    ) {
      if(burm_cost_code(&c,8,s) && COST_LESS(c,s->cost[burm_op_NT])) {
burm_trace(burm_np, 8, c);         s->cost[burm_op_NT] = c ;
        s->rule.burm_op = 2;
        burm_closure_op(s, c );
      }
    }
    if (   /* op: OP(var,const) */
      k[0]->rule.burm_var && 
      k[1]->rule.burm_const
    ) {
      if(burm_cost_code(&c,7,s) && COST_LESS(c,s->cost[burm_op_NT])) {
burm_trace(burm_np, 7, c);         s->cost[burm_op_NT] = c ;
        s->rule.burm_op = 1;
        burm_closure_op(s, c );
      }
    }
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
  case 0: /* stmt: op */
    kids[0] = p;
    break;
  case 3: /* stmt: RET(op) */
  case 2: /* stmt: RET(const) */
  case 1: /* stmt: RET(var) */
    kids[0] = burm_child(p,0);
    break;
  case 9: /* op: OP(var,op) */
  case 8: /* op: OP(var,var) */
  case 7: /* op: OP(var,const) */
  case 6: /* stmt: STORE(var,op) */
  case 5: /* stmt: STORE(var,var) */
  case 4: /* stmt: STORE(var,const) */
    kids[0] = burm_child(p,0);
    kids[1] = burm_child(p,1);
    break;
  case 11: /* const: CONST */
  case 10: /* var: VAR */
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



//The action can be anything; in this case, it is to generate assembly code.
//The action we have to write, from what I think you are implying,
// can/should do register allocation as well as code generation.

void CodeGenerator::generateCode(NODEPTR p) {
	if (p != nullptr && burm_label(p) == 0)
		std::cerr << "No match found for the node provided.\n";
	else if (p != nullptr) {
		stmt_action(p->getState(), 1);
	}
}

int OP_LABEL(NODEPTR node) {
  switch (node->getOp()) {
    case CONST:  if (node->getValue() == 0) return 661 /* I0I */;
    default:     return node->getOp();
  }
}

void burm_trace(NODEPTR node, int eRuleNo, COST cost){
}
