//! GrumpyVM instruction set.
//!
//! This module contains the types of values and instructions
//! supported by GrumpyVM.

// Changes from PA3:
// * Display trait implementations for pseudo-instructions.
// * A few unit tests.
#![allow(warnings)]
use self::{Binop::*, Instr::*, PInstr::*, Unop::*, Val::*};
use crate::{ParseError, FromBytes, ToBytes};
use byteorder::{BigEndian, ByteOrder};
use std::fmt::{self, Display};
use std::str::FromStr;

/// Heap addresses.
pub type Address = usize;

/// GrumpyVM values.
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Val {
    // Value types that may appear in GrumpyVM programs:
    /// The unit value.
    Vunit,
    /// 32-bit signed integers.
    Vi32(i32),
    /// Booleans.
    Vbool(bool),
    /// Stack or instruction locations.
    Vloc(u32),
    /// The undefined value.
    Vundef,

    // Value types that are used internally by the language
    // implementation, and may not appear in GrumpyVM programs:
    /// Metadata for heap objects that span multiple values.
    Vsize(usize),
    /// Pointers to heap locations.
    Vaddr(Address),
}

/// Val methods.
impl Val {
    /// Try to extract an i32 from a Val.
    pub fn to_i32(&self) -> Option<i32> {
	match self {
	    Vi32(i) => Some(*i),
	    _ => None
	}
    }
    /// Try to extract a bool from a Val.
    pub fn to_bool(&self) -> Option<bool> {
	match self {
	    Vbool(b) => Some(*b),
	    _ => None
	}
    }
    /// Try to extract a loc (u32) from a Val.
    pub fn to_loc(&self) -> Option<u32> {
	match self {
	    Vloc(loc) => Some(*loc),
	    _ => None
	}
    }
    /// Try to extract an address (usize) from a Val.
    pub fn to_address(&self) -> Option<Address> {
	match self {
	    Vaddr(addr) => Some(*addr),
	    _ => None
	}
    }
}

/// GrumpyVM native instructions.
#[derive(Debug, Clone, PartialEq)]
pub enum Instr {
    /// Push(v): Push value v onto the stack.
    Push(Val),
    /// Pop a value from the stack, discarding it.
    Pop,
    /// Peek(i): Push onto the stack the ith value from the top.
    Peek(u32),
    /// Unary(u): Apply u to the top value on the stack.
    Unary(Unop),
    /// Binary(b): Apply b to the top two values on the stack,
    /// replacing them with the result.
    Binary(Binop),
    /// Swap the top two values.
    Swap,
    /// Allocate an array on the heap.
    Alloc,
    /// Write to a heap-allocated array.
    Set,
    /// Read from a heap-allocated array.
    Get,
    /// Var(i): Get the value at stack position fp+i.
    Var(u32),
    /// Store(i): Store a value at stack position fp+i.
    Store(u32),
    /// SetFrame(i): Set fp = s.stack.len() - i.
    SetFrame(u32),
    /// Function call.
    Call,
    /// Function return.
    Ret,
    /// Conditional jump.
    Branch,
    /// Halt the machine.
    Halt,
}

/// Program labels.
pub type Label = String;

/// Pseudo-instructions, extending native instructions with support
/// for labels. GrumpyVM cannot execute these directly -- they must
/// first be translated by the assembler to native instructions.
#[derive(Debug, Clone, PartialEq)]
pub enum PInstr {
    /// Label the next instruction.
    PLabel(Label),
    /// Push a label onto the stack.
    PPush(Label),
    /// Native machine instruction.
    PI(Instr),
}

/// Unary operators.
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Unop {
    /// Boolean negation.
    Neg,
}

/// Binary operators.
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Binop {
    /// i32 addition.
    Add,
    /// i32 multiplication.
    Mul,
    /// i32 subtraction.
    Sub,
    /// i32 division (raises an error on divide by zero).
    Div,
    /// Returns true if one i32 is less than another, otherwise false.
    Lt,
    /// Returns true if one i32 is equal another, otherwise false.
    Eq,
}

////////////////////////////////////////////////////////////////////////
// Display trait implementations
////////////////////////////////////////////////////////////////////////

impl Display for Unop {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Neg => write!(f, "neg")
        }
    }
}

impl Display for Binop {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Add => write!(f, "+"),
            Mul => write!(f, "*"),
            Sub => write!(f, "-"),
            Div => write!(f, "/"),
            Lt  => write!(f, "<"),
            Eq  => write!(f, "=="),
        }
    }
}

impl Display for Val {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Vunit    => write!(f, "tt"),
            Vi32(i)  => write!(f, "{}", i),
            Vbool(b) => write!(f, "{}", b),
            Vloc(u)  => write!(f, "{}", u),
            Vundef   => write!(f, "undef"),
            _ => Err(fmt::Error)
        }
    }
}

impl Display for Instr {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Push(v)     => write!(f, "push {}", v),
            Pop         => write!(f, "pop"),
            Peek(u)     => write!(f, "peek {}", u),
            Unary(u)    => write!(f, "unary {}", u),
            Binary(b)   => write!(f, "binary {}", b),
            Swap        => write!(f, "swap"),
            Alloc       => write!(f, "alloc"),
            Set         => write!(f, "set"),
            Get         => write!(f, "get"),
            Var(u)      => write!(f, "var {}", u),
            Store(u)    => write!(f, "store {}", u),
            SetFrame(u) => write!(f, "setframe {}", u),
            Call        => write!(f, "call"),
            Ret         => write!(f, "ret"),
            Branch      => write!(f, "branch"),
            Halt        => write!(f, "halt"),
        }
    }
}

impl Display for PInstr {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            PLabel(lbl) => write!(f, "{}:", lbl),
            PPush(lbl)  => write!(f, "push {}", lbl),
            PI(instr)   => write!(f, "{}", instr)
        }
    }
}

////////////////////////////////////////////////////////////////////////
// FromStr trait implementations
////////////////////////////////////////////////////////////////////////

impl FromStr for Unop {
    type Err = ParseError;
    
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "--" => Ok(Neg),
            tok => Ok(Neg)
          }
        }
}

impl FromStr for Binop {
    type Err = ParseError;
    
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "+" => {Ok(Add)},
            "*" => {Ok(Mul)},
            "-" => Ok(Sub),
            "/" => Ok(Div),
            "<" => Ok(Lt),
            "=" => Ok(Eq),
            tok => Ok(Eq)
          }
    }
}

impl FromStr for Val {
    type Err = ParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "tt" => Ok(Vunit),
            "true" => Ok(Vbool(true)),
            "false" => Ok(Vbool(false)),
            "undef" => Ok(Vundef),
            tok => {
              let n = tok.parse()?;
              Ok(Vi32(n))
            }
        }
    }
}

impl FromStr for Instr {
    type Err = ParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut count = 0;
            let mut str1 = "";
            let mut str2 = "";
            let mut label_instruct = s.split(" ");
            for r in label_instruct{
              if count == 0{
              //println!("{}",r);
              str1 = r;
              count = count + 1;
              }
              else{
              str2 = r;
              
              }
            }
    	     match str1 {
            "setframe" => {let mut y = u32::from_str(str2).unwrap();Ok(SetFrame(y))},
            "push" => {let mut value = Val::from_str(str2).unwrap();Ok(Push(value))},
            "pop" => {Ok(Pop)},
            "peek" => {let mut y = u32::from_str(str2).unwrap();Ok(Peek(y))},
            "unary" => {Ok(Unary(Neg))},
            "binary" => {let op=Binop::from_str(str2).unwrap();Ok(Binary(op))},
            "swap" => {Ok(Swap)},
            "alloc" => {Ok(Alloc)},
            "set" => {Ok(Set)},
            "get" => {Ok(Get)},
            "var" => {let mut value = u32::from_str(str2).unwrap();Ok(Var(value))},
            "store" => {let mut value = u32::from_str(str2).unwrap();Ok(Store(value))},
            "call" => {Ok(Call)},
            "ret" => {Ok(Ret)},
            "branch" => {Ok(Branch)},
            "halt" => {Ok(Halt)},
            tok => {Ok(Pop)}
        }
    }
}

fn parse_label(s: &str) -> Result<Label, ParseError> {
    unimplemented!()
}

impl FromStr for PInstr {
    type Err = ParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut h = s.to_string();
  let mut str1 = "";
  let mut str2 = "";
  let mut count = 0;
  let mut label_instruct = h.split(" ");
  for b in label_instruct{
    if count == 0{
    str1 = b;
    count = count + 1;
    }
    else{
    str2 = b;
    }
  }
  let mut res = str2.to_string();
  let mut res_1 = str1.to_string();
  if res_1.starts_with("L") || res_1.starts_with("_L") {
   
    let result = str::replace(s,":","");
    
    Ok(PLabel(result))
    }
    else if res.starts_with("L") || res.starts_with("_L") {
    
      res = res.trim().to_string();
      Ok(PPush(res))
    }
    else {
    let mut y = Instr::from_str(s).unwrap();
    Ok(PI(y))
    }
    }
}

/// Test to_string and from_string implementations (to_string comes
/// for free from Display).

#[test]
fn test_isa_parse() -> Result<(), ParseError> {
    assert_eq!(PLabel("Ltest".into()), PLabel("Ltest".into()).to_string().parse()?);
    assert_eq!(PPush("Ltest".into()), PPush("Ltest".into()).to_string().parse()?);
    let pinstrs: Vec<PInstr> = vec![Push(Vi32(123)), Pop, Peek(45), Unary(Neg),
				    Binary(Lt), Swap, Alloc, Set, Get, Var(65),
				    Store(5), Call, Ret, Branch, Halt]
	.into_iter().map(|x| PI(x)).collect();
    for pinstr in pinstrs {
	assert_eq!(pinstr, pinstr.to_string().parse()?);
    }
    Ok(())
}

////////////////////////////////////////////////////////////////////////
// ToBytes trait implementations
////////////////////////////////////////////////////////////////////////

// byteorder
// BigEndian::write_u32(..
// BigEndian::write_i32(..

impl ToBytes for u32 {
    fn to_bytes(&self) -> Vec<u8> {
        let mut v = vec![0x00; 4];
        BigEndian::write_u32(&mut v, *self);
        v
    }
}

impl ToBytes for i32 {
    fn to_bytes(&self) -> Vec<u8> {
        let mut v = vec![0x00; 4];
    BigEndian::write_i32(&mut v, *self);
    v
    }
}

impl ToBytes for Unop {
    fn to_bytes(&self) -> Vec<u8> {
        match self {
            Neg => vec![0x00]
          }
    }
}

impl ToBytes for Binop {
    fn to_bytes(&self) -> Vec<u8> {
        match self{
            Add => vec![0x00],
            Sub => vec![0x02],
            Mul => vec![0x01],
            Div => vec![0x03],
            Lt => vec![0x04],
            Eq => vec![0x05]
            }
    }
}

impl ToBytes for Val {
    fn to_bytes(&self) -> Vec<u8> {
        match self{
            Vunit => vec![0x00],
            Vi32(i) => {
              let mut v = vec![0x01];
              v.append(&mut i.to_bytes());
              v
            }
            Vbool(true) => {
              let mut v = vec![0x02];
              v
            }
            Vbool(false) => {
              let mut v = vec![0x03];
              v
            }
            Vundef => vec![0x05],
            Vloc(i) => {
            let mut v = vec![0x04];
            v.append(&mut i.to_bytes());
            v
            }
            _ => unimplemented!()
        }
    }
}

impl ToBytes for Instr {
    fn to_bytes(&self) -> Vec<u8> {
        match self{
            Push(i) => {
              let mut v = vec![0x00];
              v.append(&mut i.to_bytes());
              v
            }
            Pop => vec![0x01],
            Peek(i) => {
              let mut v = vec![0x02];
              v.append(&mut i.to_bytes());
              v
            }
            Unary(u) => {
              let mut v = vec![0x03];
              v.append(&mut u.to_bytes());
              v
            }
            Binary(b) => {
              let mut v = vec![0x04];
              v.append(&mut b.to_bytes());
              v
            }
            Swap => vec![0x05],
            Alloc => vec![0x06],
            Set => vec![0x07],
            Get => vec![0x08],
            Var(i) => {
              let mut v = vec![0x09];
              v.append(&mut i.to_bytes());
              v
            }
            Store(i) => {
            let mut v = vec![0xA];
            v.append(&mut i.to_bytes());
            v
            }
            SetFrame(i) => {
            let mut v = vec![0xB];
            v.append(&mut i.to_bytes());
            //BigEndian::write_u32(&mut i,*self);
            v
            }
            Call => vec![0xC],
            Ret => vec![0xD],
            Branch => vec![0xE],
            Halt => vec![0xF]
          }
    }
}

////////////////////////////////////////////////////////////////////////
// FromBytes trait implementations
////////////////////////////////////////////////////////////////////////

impl FromBytes for u32 {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<u32, ParseError> {
	let mut v: Vec<u8> = bytes.take(4).collect();
    //let mut num: Vec<u8> = Vec::new();
    //for x in 0..4{
        //num.push(bytes.next().unwrap());
    //}
    
    let mut num1: u32 = BigEndian::read_u32(&mut v);
    Ok(num1)
    }
}

impl FromBytes for i32 {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<i32, ParseError> {
        let mut v: Vec<u8> = bytes.take(4).collect();
        //let mut num: Vec<u8> = Vec::new();
        //for x in 0..4{
            //num.push(bytes.next().unwrap());
        //}
        
        let mut num1: i32 = BigEndian::read_i32(&mut v);
        Ok(num1)
    }
}

impl FromBytes for Unop {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<Unop, ParseError> {
        match bytes.next().ok_or(ParseError("not enough bytes".into()))? {
            0x00 => Ok(Neg),
            b => Err(ParseError(format!("Unknown val bytecode: {}",b)))  
        }
    }
}

impl FromBytes for Binop {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<Binop, ParseError> {
        match bytes.next().ok_or(ParseError("not enough bytes".into()))? {
            0x00 => Ok(Add),
            0x01 => Ok(Mul),
            0x02 => Ok(Sub),
            0x03 => Ok(Div),
            0x04 => Ok(Lt),
            0x05 => Ok(Eq),
            b => Err(ParseError(format!("Unknown val bytecode: {}",b)))  
        }
    }
}

impl FromBytes for Val {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<Val, ParseError> {
        match bytes.next().ok_or(ParseError("not enough bytes".into()))? {
            0x00 => Ok(Vunit),
            0x01 => Ok(Vi32(i32::from_bytes(bytes)?)),
            0x02 => Ok(Vbool(true)),
            0x03 => Ok(Vbool(false)),
            0x04 => Ok(Vloc(u32::from_bytes(bytes)?)),
            0x05 => Ok(Vundef),
            b => Err(ParseError(format!("Unknown val bytecode: {}",b)))
        }
    }
}

impl FromBytes for Instr {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<Instr, ParseError> {
        match bytes.next().ok_or(ParseError("not enough bytes".into()))? {
            0x00 => Ok(Push(Val::from_bytes(bytes)?)),
            0x01 => Ok(Pop),
            0x02 => Ok(Peek(u32::from_bytes(bytes)?)),
            0x03 => Ok(Unary(Unop::from_bytes(bytes)?)),
            0x04 => Ok(Binary(Binop::from_bytes(bytes)?)),
            0x05 => Ok(Swap),
            0x06 => Ok(Alloc),
            0x07 => Ok(Set),
            0x08 => Ok(Get),
            0x09 => Ok(Var(u32::from_bytes(bytes)?)),
            0xA => Ok(Store(u32::from_bytes(bytes)?)),
            0xB => Ok(SetFrame(u32::from_bytes(bytes)?)),
            0xC => Ok(Call),
            0xD => Ok(Ret),
            0xE => Ok(Branch),
            0xF => Ok(Halt),
            b => Err(ParseError(format!("Unknown val bytecode: {}",b)))
        }
    }
}

impl FromBytes for Vec<Instr> {
    type Err = ParseError;
    fn from_bytes<T: Iterator<Item=u8>>(bytes: &mut T) -> Result<Vec<Instr>, ParseError> {
        // Read the first 4 bytes to get the number of instructions n: u32.
        let mut y = (i32::from_bytes(bytes)?);
        let mut instruct: Vec<Instr> = Vec::new();
        for i in 0..y{
            instruct.push(Instr::from_bytes(bytes)?);
        }
        Ok(instruct)
    }
}

/// Test to_bytes and from_bytes implementations.

#[test]
fn test_isa_serialize() -> Result<(), ParseError> {
    let instrs: Vec<Instr> = vec![Push(Vi32(123)), Pop, Peek(45), Unary(Neg),
    				  Binary(Lt), Swap, Alloc, Set, Get, Var(65),
    				  Store(5), Call, Ret, Branch, Halt];
    for instr in instrs {
	assert_eq!(instr, Instr::from_bytes(&mut instr.to_bytes().into_iter())?);
    }
    Ok(())
}

