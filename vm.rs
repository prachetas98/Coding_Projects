//! Grumpy virtual machine.
//!
//! This module contains the Grumpy virtual machine.
#![allow(warnings)]
use std::fmt::{self, Display};
use super::isa::{*, Binop::*, Instr::*, Val::*, Unop::*};

static STK_SIZE: usize = 1024;
static HEAP_SIZE: usize = 1024;

/// GrumpyVM state.
#[derive(Debug)]
struct State {
    /// Program counter.
    pc: u32,
    /// Frame pointer.
    fp: u32,
    /// The stack, with maximum size STK_SIZE.
    stk: Vec<Val>,
    /// The heap, with maximum size HEAP_SIZE.
    heap: Vec<Val>,
    /// The program being executed, a vector of instructions.
    prog: Vec<Instr>
}

/// Display implementation for State (modify as you wish).
impl Display for State {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
	write!(f, "pc: {}\ninstr: {:?}\nfp: {}\nstk: {:?}\nheap: {:?}",
	       self.pc, self.prog[self.pc as usize], self.fp, self.stk, self.heap)?;
	write!(f, "\nheap size: {}", self.heap.len())
    }
}

/// Debug enum (whether to print debug information during execution or not).
#[derive(Clone, Copy)]
pub enum Debug {
    DEBUG,
    NODEBUG
}

/// State methods.
impl State {
    /// Create initial state for given program.
    fn init(prog: Vec<Instr>) -> State {
	State {
	    pc: 0, 
	    fp: 0,
	    stk: Vec::with_capacity(STK_SIZE),
	    heap: Vec::with_capacity(HEAP_SIZE),
	    prog: prog
	}
    }

    /// Push a Val to the stack, checking for overflow.
    fn push(&mut self, v: Val) -> Result<(), String> {
	if self.stk.len() < STK_SIZE {
    	    Ok(self.stk.push(v))
	} else {
	    Err("out of stack space".into())
	}
    }

    /// Pop a Val from the stack, checking for underflow.
    fn pop(&mut self) -> Result<Val, String> {
    	self.stk.pop().ok_or("attempt to pop empty stack".into())
    }

    fn swap(&mut self) -> Result<(), String> {
        let x: Val = match self.pop(){                          // pop the first value
            Ok(x) => {x},
            _ => return Err("Problem with the pop".into())
        };
        let y :Val = match self.pop(){                          // pop the second value
            Ok(y) => {y},
            _ => return Err("Problem with the pop".into())
        };

        match self.push(x){                                         // push the first value, then the second one on the stack
            Ok(()) => {Ok(self.push(y)?)},
            _ => return Err("Problem with the pop".into())
        }
    }

    // This function will set the frame pointer to the value given in its parameter
    fn setframe(&mut self, i: u32) -> Result<(), String> {
        self.push(Vloc(self.fp));
        let stack_length: u32 = self.stk.len() as u32;          // the length of the stack is computed 
        self.fp = stack_length - i - 1;                             // the frame pointer is determined
        Ok(())
    }

    // This function will look for a number at a specific location in a stack
    fn peek(&mut self, i: u32) -> Result<(), String> {
        let num: usize = i as usize;                                // gets the number
        let y = self.stk[num];
        self.push(y);                                               // pushes the number onto the stack
        Ok(())
    }

    // This function negates the boolean value
    fn unary(&mut self, u: Unop) -> Result<(), String> {
        let x: Val = match self.pop(){                          // pop the value from the stack
            Ok(x) => {x},
            _ => return Err("Problem with the pop".into())
        };
        match unop(u,x){                                            // calls the unop function to compute the bool
            Ok(v) => match self.push(v) {
                Ok(()) => Ok(()),
                Err(e) => Err("problem with unary operation".into())
            },
            Err(e) => Err("problem with unary operation".into())
        }
    }

    // This function performs a binary operation on the two values from the stack
    fn binary(&mut self, b: Binop) -> Result<(), String>{
        let v1: Val = match self.pop(){                             // pop the first value
            Ok(v1) => {v1},
            _ => return Err("Problem with the pop".into())
        };
        let v2 :Val = match self.pop(){                             // pop the second value
            Ok(v2) => {v2},
            _ => return Err("Problem with the pop".into())
        };
        
        match binop(b,v1,v2){                                           // calls the binop function to compute the result
            Ok(v) => match self.push(v) {
                        Ok(()) => Ok(()),
                        Err(e) => Err("problem with unary operation".into())
                    },
            Err(e) => Err("problem with unary operation".into())
        }
    }
    
    // pops the 2 top values and pushes an array onto the heap
    fn alloc(&mut self) -> Result<(), String>{
        let mut heap_len: usize = self.heap.len() as usize;
        
        let vinit: Val = match self.pop(){                          // pop the value from the stack
            Ok(vinit) => {vinit},
            _ => return Err("problem with pop operation".into())
        };
        let arr_size :Val = match self.pop(){                       // pop the size of the array
            Ok(arr_size) => {arr_size},
            _ => return Err("problem with pop operation".into())
        };

        let arr_len = match arr_size.to_i32(){
            Some(arr_len) => {arr_len},
            None => return Err("Problem with the pop".into())
        };
        let arr_cap: usize = arr_len as usize;
        if (heap_len + arr_cap) < HEAP_SIZE {                               // allocate the values on the array
            self.heap.push(Vsize(arr_cap));
            for i in 0..arr_len{
                self.heap.push(vinit);
            }
        
            self.push(Vaddr(heap_len));                                 // push the array on the heap
            Ok(())
        }
        else{
            eprintln!("GC start: heap size = {:?} values",self.heap.len());
            run_gc(self);
            eprintln!("GC end: heap size = {:?} values",self.heap.len());
            heap_len = self.heap.len();
            if (heap_len + arr_cap) < HEAP_SIZE {  
                self.heap.push(Vsize(arr_cap));
            for i in 0..arr_len{
                self.heap.push(vinit);
            }
        
            self.push(Vaddr(heap_len));                                 // push the array on the heap
            Ok(())
            }
            else{
                //eprintln!("GC start: heap size = {:?} values",self.heap.len());
                //eprintln!("GC end: heap size = {:?} values",self.heap.len());
                return Err("out of heap space".into())
            }
            
            //Ok(())
            
        }
        //Ok(())
    }
    
    // This sets the value of the heap
    fn set(&mut self) -> Result<(), String>{
        let v: Val = match self.pop(){                                  // get the first value 
            Ok(v) => {v},
            _ => return Err("Problem with the pop".into())
        };
        let idx :Val = match self.pop(){                                // obtain the index
            Ok(idx) => {idx},
            _ => return Err("Problem with the pop".into())
        };
        let base: Val = match self.pop(){                               // obtain the base
            Ok(base) => {base},
            _ => return Err("Problem with the pop".into())
        };

        let idx_value = match idx.to_i32(){
            Some(idx_value) => {idx_value},
            None => return Err("Problem with the pop".into())
        };
        
        let base_value = match base.to_address(){
            Some(z) => {z},
            None=> return Err("Problem with the pop".into())
        };
        let new_idx_value: usize = idx_value as usize;
        if new_idx_value < HEAP_SIZE {

        
        let heap_index = base_value + new_idx_value + 1;
        let got = std::mem::replace(&mut self.heap[heap_index], v);         // it replaces the value in the base + idx + 1 location 
        Ok(())
        }
        else{
            return Err("heap out of bounds".into())
        }
    }

    // This gets the value at a specific location on the stack
    fn get(&mut self) -> Result<(), String>{
        let idx :Val = match self.pop(){                                // pops the value from the stack
            Ok(idx) => {idx},
            _ => return Err("Problem with the pop".into())
        };
        let base: Val = match self.pop(){                               // obtains the base
            Ok(base) => {base},
            _ => return Err("Problem with the pop".into())
        };

        let idx_value = match idx.to_i32(){                             // obtains idx value
            Some(a) => {a},
            None => return Err("Problem with the pop".into())
        };
        let base_value = match base.to_address(){
            Some(z) => {z},
            None=> return Err("Problem with the pop".into())
        };
        let new_idx_value: usize = idx_value as usize;
        if(new_idx_value < HEAP_SIZE){
        let heap_index = base_value + new_idx_value + 1;                // finds heap index
        let v = self.heap[heap_index];
        self.push(v);                                                       // push the value onto the stack
        Ok(())
        }
        else{
            return Err("out of heap space".into())
        }
    }

    // This function copies the value at a specific location on the stack
    fn var(&mut self, i: u32) -> Result<(), String> {
        let index: usize = (self.fp + i) as usize;
        if(index < self.stk.len()){
        let v = self.stk[index];                                        // obtain the value
        self.push(v);                                                       // push on the stack
        Ok(())
        }
        else{
            return Err("out of stack space".into())
        }
    }

    // This function replaces the new value in the stack
    fn store(&mut self, i: u32) -> Result<(), String> {
        let index: usize = (self.fp + i) as usize;
        if index < self.stk.len()-1 {
        let new_value = match self.pop(){                                   // pops the value from the stack
            Ok(new_value) => {new_value},
            _ => return Err("Problem with the pop".into())
        };
        let got = std::mem::replace(&mut self.stk[index], new_value);           // replaces the old value with the new one
        Ok(())
        }
        else{
            return Err("out of stack space".into())
        }
    }

    // This function sets the program counter based on the boolean
    fn branch(&mut self) -> Result<(), String> {
        let mut cur_pc = self.pc;
        let x: Val = match self.pop(){                                  // pop the loc
            Ok(x) => {x},
            _ => return Err("Problem with the pop".into())
        };
        let y :Val = match self.pop(){                                  // pop the bool
            Ok(y) => {y},
            _ => return Err("Problem with the pop".into())
        };
        let target = match x.to_loc(){
            Some(target) => {target},
            None => return Err("Problem with the pop".into())
        }; 
        let b = match y.to_bool(){
            Some(b) => {b},
            None => return Err("Problem with the pop".into())
        };
        if b==true {
            self.pc = target;                                               // resets the pc
        }
        else{
            cur_pc = self.pc + 1;
        }
        Ok(())
    }


    // This function returns the value from the stack
    fn ret(&mut self) -> Result<(), String> {
        let callee_fp: usize = self.fp as usize;
        let callee_pc = self.pc;
        let vret: Val = match self.pop(){
            Ok(vret) => {vret},
            _ => return Err("Problem with the pop".into())
        };
        
        let call_pc: Val = match self.pop(){
            Ok(call_pc) => {call_pc},
            _ => return Err("Problem with the pop".into())
        };
        
        let call_fp: Val = match self.pop(){
            Ok(call_fp) => {call_fp},
            _ => return Err("Problem with the pop".into())
        };
        
        let target = match call_pc.to_loc(){
            Some(target) => {target},
            None => return Err("Problem with the pop".into())
        };

        let target_sec = match call_fp.to_loc(){
            Some(target_sec) => {target_sec},
            None => return Err("Problem with the pop".into())
        }; 

        while self.stk.len() > callee_fp {
            self.pop();
        }
        self.pc = target;
        self.fp = target_sec;
        match self.push(vret){
            Ok(()) => {Ok(())},
            _ => return Err("Problem with the pop".into())
        }
    }

    // This function takes the parameter from the top values of the stack and reassigns the values
    fn call(&mut self) -> Result<(), String>{
        let caller_pc: u32 = self.pc as u32;
        let locks: Val = match self.pop(){
            Ok(locks) => {locks},
            _ => return Err("Problem with the pop".into())
        };
        let target = match locks.to_loc(){
            Some(target) => {target},
            None => return Err("Problem with the pop".into())
        }; 
        self.pc = target;
        match self.push(Vloc(caller_pc)){
            Ok(()) => {Ok(())},
            _ => return Err("Problem with the pop".into())
        }
    }

    // Halts the machine    
    fn halt(&mut self) -> Result<(), String>{
        Ok(())
    }

}

/// Evaluate a unary operation on a value.
fn unop(u: Unop, v: Val) -> Result<Val, String> {
    let x = v.to_bool().ok_or("Expected a bool")?;                          // obtain the first value
    match v{                                
        Vbool(false) => {Ok(Vbool(true))},                                    // negates the boolean value
        Vbool(true) => {Ok(Vbool(false))}
        _ => {Err("Something wrong with unop the given value".into())}
    }
}

/// Evaluate a binary operation on two argument values.
fn binop(b: Binop, v1: Val, v2: Val) -> Result<Val, String> {
    let x = v1.to_i32().ok_or("Expected a number")?;                        // obtain the first value
    let y = v2.to_i32().ok_or("Expected a number")?;                        // obtain the second value
    match b{
        Add => Ok(Vi32(x+y)),
        Sub => Ok(Vi32(x-y)),                                                   // computes the result
        Mul => Ok(Vi32(x*y)),
        Div => Ok(Vi32(x/y)),
        Lt => {if x < y {
            Ok(Vbool(true))
        }
        else{
            Ok(Vbool(false))
        }},
        Eq => {if x == y {
            Ok(Vbool(true))
        }
        else{
            Ok(Vbool(false))
        }},
        _ => Err("Something wrong with the binop given value".into()) 
    }
}

fn forward(s: &mut State, v: &mut Vec<Val>) -> Result<(), String> {
    //let mut counter = 0;
    for i in 0..v.len(){
        match v[i]{
            Vaddr(y) => {
                let mut value = s.heap[y];
                match value{
                    Vsize(a) => {for t in y..(y+a+1){
                        v.push(s.heap[t]);
                    }
                    },
                    _ => {}
                }
            },
            _ => {}
        }
    }
    s.heap = v.to_vec();

    Ok(())
}

/// Run the garbage collector.
fn run_gc(s: &mut State) -> Result<(), String> {
    let mut new_heap: Vec<Val> = Vec::new();
    let v: Vec<usize> = Vec::new();
    for i in 0..s.stk.len(){
        match s.stk[i]{
            Vunit => {},
            Vi32(i) => {},
            Vbool(true) => {},
            Vbool(false) => {},
            Vundef => {},
            Vloc(i) => {},
            Vaddr(addr) => {
                match s.heap[addr]{
                    Vsize(d) => {
                        let len_of_new = new_heap.len();
                        for j in 0..(d+1){
                        new_heap.push(s.heap[addr+j]);
                        }
                        s.stk[i] = Vaddr(len_of_new);
                    },
                    _ => {}
                }
            },
            _ => {}
        }
    }
    forward(s,&mut new_heap)
}

// This funcion determines which instruction is called
fn instr(i: Instr,s: &mut State) -> Result<(), String> {
    //run_gc(s);
    match i {
        Push(v) => Ok(s.push(v)?),
        Pop => match s.pop(){
            Ok(j) => {Ok(())},
            _ => return Err("Something wrong with the instr given value".into())
        },
        Peek(z) => Ok(s.peek(z)?),
        Unary(u) => Ok(s.unary(u)?),
        Binary(b) => Ok(s.binary(b)?),
        Swap => Ok(s.swap()?),
        Alloc => {Ok(s.alloc()?)},
        Set => Ok(s.set()?),
        Get => Ok(s.get()?),
        Var(c) => Ok(s.var(c)?),
        Store(n) => Ok(s.store(n)?),
        SetFrame(d) => Ok(s.setframe(d)?),
        Call => Ok(s.call()?),
        Ret => Ok(s.ret()?),
        Branch => Ok(s.branch()?),
        Halt => Ok(s.halt()?), 
        _ => return Err("Something wrong with the given value".into()) 
    }
}

/// Execute from initial state s.
fn exec(d: Debug, s: &mut State) -> Result<(), String> {
    // Dispatch loop.
    // Check for pc out of bounds.

    // Get next instruction.

    // Increment pc.

    // Dispatch on instruction.

    'mainloop:loop {
        // Print state if debug flag is set.
	    if let Debug::DEBUG = d {
	        println!("{}\n", s)
	    }
        let program_length: u32 = s.prog.len() as u32; 
        if s.pc >= program_length {
            return Err("Something wrong with the given value".into()) 
        }
        let cur_pc: usize = s.pc as usize;
        let i = s.prog[cur_pc].clone();
        s.pc = s.pc + 1;
        if i != Halt {
            instr(i,s)?;
        }
        else{
            break 'mainloop;
        }
        
        // Check for pc out of bounds.

        // Get next instruction.

        // Increment pc.

        // Dispatch on instruction.
    }
    Ok(())
    
}

/// Entry point of the module. Run a given program in the VM.
pub fn run(d: Debug, prog: &[Instr]) -> Result<Val, String> {
    // Create initial state.
    let mut s = State::init(prog.into());

    // Run VM.
    exec(d, &mut s)?;

    // Return the value on top of the stack.
    s.pop()
}
