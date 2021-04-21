//! GrumpyIR compiler.
//! Name : Prachetas Deshpande
//! This file will generate assembly code from the given GrumpyIR file
//! This module contains the compiler from GrumpyIR to GrumpyVM
//! assembly code. Compilation is performed by `compile` methods on
//! GrumpyIR types.
#![allow(warnings)]
use std::collections::HashMap;
use std::sync::atomic::{AtomicUsize, Ordering};
use crate::CompileError;
use crate::isa::{Instr, Label, PInstr, PInstr::*, Val::*, Unop, Unop::*, Binop, Binop::*};
use crate::ir::*;

/// Global gensym counter for generating fresh names.
fn gensym() -> usize {
    static GENSYM: AtomicUsize = AtomicUsize::new(0);
    GENSYM.fetch_add(1, Ordering::SeqCst);
    GENSYM.load(Ordering::SeqCst)
}

/// Generate a fresh label using gensym.
fn fresh_label() -> Label {
    format!("_L{}", gensym())
}

/// The type of compilation environments.
struct Env {
    locals: HashMap<String, u32>
}

impl Env {
    fn mk(locals: HashMap<String, u32>) -> Env {
	Env { locals: locals }
    }
}

impl Val {
    /// Compile a value to a vector of pseudo-instructions (assembly code).
    fn compile(&self) -> Result<Vec<PInstr>, CompileError> {
        let mut instruct: Vec<PInstr> = Vec::new();
        match self{
            Val::Num(i) => {instruct.push(PI(Instr::Push(Vi32(*i))));
                Ok(instruct)
            },
            Val::Bool(b) => {instruct.push(PI(Instr::Push(Vbool(*b))));
                Ok(instruct)
            },
            Val::Unit => {
                instruct.push(PI(Instr::Push(Vunit)));
                Ok(instruct)
            }
        }
    }
}

impl Exp {
    /// Compile an expression to a vector of pseudo-instructions (assembly code).
    fn compile(&self, rho: &Env) -> Result<Vec<PInstr>, CompileError> {
	use Exp::*;
        let mut instruct: Vec<PInstr> = Vec::new();
        match self{
            Val(v) => {
                v.compile()
            },
            Var(r) => {
                for(contact,num) in rho.locals.iter(){
                    if(contact == r){
                        instruct.push(PI(Instr::Var(*num)));
                        break;
                    }
                }
                Ok(instruct)
            },
            Unary(u, b) => {
                match b.compile(&rho){
                    Ok(p) => {
                        let mut wg = p;
                        instruct.append(&mut wg);
                    },
                    Err(e) =>  return Err(format!("problem with the code").into())
                }
                instruct.push(PI(Instr::Unary(*u)));
                Ok(instruct)
            },
            Binary(b, b1, b2) =>{
                match b2.compile(&rho){
                    Ok(c) => {
                        let mut wg = c;
                        instruct.append(&mut wg);
                        
                        match b1.compile(&rho){
                            Ok(u) => {
                                let mut wg = u;
                                instruct.append(&mut wg);
                            },
                            Err(e) => return Err(format!("problem with the code").into())
                        }
                        
                    }
                    Err(e) => return Err(format!("problem with the code").into())
                }
                instruct.push(PI(Instr::Binary(*b)));
                Ok(instruct)
            },
            Let(n, e1, e2) => {
                
                let mut y="";
                let mut cap: u32 = rho.locals.len() as u32;
                for (contact, num) in rho.locals.iter(){
                    y = contact;
                }
                match e1.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                for (contact, num) in rho.locals.iter(){
                    if(contact == &(n.to_string())){
                        instruct.push(PI(Instr::Store(*num)));
                        break;
                    }
                }
                match e2.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                
                Ok(instruct)

            },
            Seq(e1, e2) => {
                let mut y;
                let mut z;
                match e1.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        y = wg[wg.len()-1].to_string();
                        z = y;
                        instruct.append(&mut wg);
                        
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                if(z != "set"){
                    instruct.push(PI(Instr::Pop));
                }
                
                match e2.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                Ok(instruct)
            },
            Alloc(e1, e2) => {
                match e1.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                match e2.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                instruct.push(PI(Instr::Alloc));
                //instruct.push(PI(Instr::Pop));
                Ok(instruct)
            },
            Set(e1, e2, e3) => {
                match e1.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                match e2.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                match e3.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                instruct.push(PI(Instr::Set));
                Ok(instruct)
            },
            Get(e1, e2) => {
                //println!("{:?}",e1);
                match e1.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                match e2.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                instruct.push(PI(Instr::Get));
                Ok(instruct)
            },
            Cond(e1, e2, e3) => {
                let mut instruct_cond: Vec<PInstr> = Vec::new();
                let mut instruct1: Vec<PInstr> = Vec::new();
                let mut instruct2: Vec<PInstr> = Vec::new();
                let mut _L1 = fresh_label();
                let mut _L2 = fresh_label();
                match e1.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                instruct.push(PPush(_L1.to_string()));
                instruct.push(PI(Instr::Branch));
                match e3.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                instruct.push(PI(Instr::Push(Vbool(true))));
                instruct.push(PPush(_L2.to_string()));
                instruct.push(PI(Instr::Branch));
                instruct.push(PLabel(_L1.to_string()));
                match e2.compile(&rho){
                    Ok(w) =>{
                        let mut wg = w;
                        instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                }
                instruct.push(PLabel(_L2.to_string()));
                Ok(instruct)
            },
            Funptr(n) => {
                let mut beg_lab = "L".to_owned();
                let mut label_str = beg_lab + n;
                instruct.push(PPush(label_str));
                Ok(instruct)
            },
            Call(f_name, ve) => {
                for i in 0..ve.len(){
                    match ve[i].compile(&rho){
                        Ok(w) =>{
                            let mut wg = w;
                            instruct.append(&mut wg);
                        }
                        Err(e) => {return Err(format!("problem with the code").into())}
                    }
                }
                match f_name.compile(&rho){
                    Ok(w) => {
                        let mut wg = w;
                    instruct.append(&mut wg);
                    }
                    Err(e) => {return Err(format!("problem with the code").into())}
                    }
                instruct.push(PI(Instr::SetFrame((ve.len()+1) as u32)));
                instruct.push(PI(Instr::Swap));
                instruct.push(PI(Instr::Call));
                Ok(instruct)
            }
        }
    }
}

impl Fun {
    /// Compile a function to a vector of pseudo-instructions (assembly code).
    fn compile(&self) -> Result<Vec<PInstr>, CompileError> {
        let mut final_instruct: Vec<PInstr> = Vec::new();
        let mut beg_lab;
        beg_lab = "L".to_owned();
        let mut label_str = beg_lab + &self.name;
        final_instruct.push(PLabel(label_str));
        let mut rho = Env::mk(self.params.iter().enumerate().map(|(i,x)|(x.0.clone(), i as u32)).collect());
        let mut y = self.body.bound_vars();
        rho.locals.extend(y.iter().enumerate().map(|(i,x)|(x.clone(), (self.params.len() + 2 + i) as u32)));
        //let mut pinstrs = vec![PLabel(format!("L{}",self.name))];
        final_instruct.extend(y.iter().map(|_| PI(Instr::Push(Vundef))));
        match self.body.compile(&rho){
            Ok(t) => {
                let mut wg = t;
                final_instruct.append(&mut wg);
            },
            Err(e) => return Err(format!("problem with the code").into())
        }
        if(y.len() > 0){
            final_instruct.push(PI(Instr::Store(self.params.len() as u32 + 2)));
        }
        final_instruct.push(PI(Instr::Ret));
        Ok(final_instruct)
    }
}

impl Prog {
    /// Compile a program to a vector of pseudo-instructions (assembly code).
    pub fn compile(&self) -> Result<Vec<PInstr>, CompileError> {
	let main = Fun::new("main".into(), vec![], Type::I32, self.main.clone());
	Ok([vec![PI(Instr::SetFrame(0)),
		 PPush("Lmain".into()),
		 PI(Instr::Call),
		 PI(Instr::Halt)],
	    main.compile()?,
	    self.funs.iter().map(|f| f.compile()).collect::<Result<Vec<_>, _>>()?.concat()
	].concat())
    }
}

#[test]
fn test_1() -> Result<(), CompileError> {
    let mut new_heap: Vec<PInstr> = Vec::new();
    let mut my_value: Val = Val::Num(3);
        match my_value.compile(){
        Ok(y) => { 
            new_heap.push(PI(Instr::Push(Vi32(3))));
            assert_eq!(y,new_heap);
            Ok(())
        },
        Err(e) => Err(CompileError("error".into()))
        }
    
}

#[test]
fn test_2() -> Result<(), CompileError> {
    let mut new_heap: Vec<PInstr> = Vec::new();
    let mut new_hash = HashMap::new();
    let mut rho = Env::mk(new_hash);
    let mut my_value: Val = Val::Bool(true);
    let mut my_exp: Exp = Exp::Val(my_value);
        match my_exp.compile(&rho){
        Ok(y) => { 
            new_heap.push(PI(Instr::Push(Vbool(true))));
            assert_eq!(y,new_heap);
            Ok(())
        },
        Err(e) => Err(CompileError("error".into()))
        }
    
}

#[test]
fn test_3() -> Result<(), CompileError> {
    let mut new_heap: Vec<PInstr> = Vec::new();
    let mut new_hash = HashMap::new();
    let mut u = Unop::Neg;
    let mut rho = Env::mk(new_hash);
    let mut my_value: Val = Val::Bool(true);
    let mut my_exp: Exp = Exp::Val(my_value);
    let box_1 = Box::new(my_exp.clone());
    let mut my_exp2: Exp = Exp::Unary(u,box_1);
    
        match my_exp2.compile(&rho){
        Ok(y) => {
            new_heap.push(PI(Instr::Push(Vbool(true))));
            new_heap.push(PI(Instr::Unary(Neg)));
            assert_eq!(y,new_heap);
            Ok(())
        },
        Err(e) => Err(CompileError("error".into()))
        }
    
}

#[test]
fn test_4() -> Result<(), CompileError> {
    let mut new_heap: Vec<PInstr> = Vec::new();
    let mut new_hash = HashMap::new();
    let b: Binop= Binop::Add;
    let mut rho = Env::mk(new_hash);
    let mut my_value: Val = Val::Num(3);
    let mut my_exp: Exp = Exp::Val(my_value);
    let mut my_value2: Val = Val::Num(4);
    let mut my_exp2: Exp = Exp::Val(my_value2);
    let box_1 = Box::new(my_exp.clone());
    let box_2 = Box::new(my_exp2.clone());
    let mut my_exp3: Exp = Exp::Binary(b,box_1,box_2);
    
        match my_exp3.compile(&rho){
        Ok(y) => { 
            new_heap.push(PI(Instr::Push(Vi32(4))));
            new_heap.push(PI(Instr::Push(Vi32(3))));
            new_heap.push(PI(Instr::Binary(Add)));
            assert_eq!(y,new_heap);
            Ok(())

        },
        Err(e) => Err(CompileError("error".into()))
        }
    //Ok(())
}





