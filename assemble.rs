//! Grumpy assembler.
//!
//! This module contains the assembler that translates
//! pseudo-instruction (assembly) programs into native
//! programs by resolving label addresses.

use std::collections::HashMap;
use crate::isa::{*, Instr::*, PInstr::*, Val::*};

/// Translate an assembly program to an equivalent native program.
pub fn assemble(pinstrs : Vec<PInstr>) -> Result<Vec<Instr>, String> {
    // Fill in your solution.
    let mut pc = 0;
    let mut d = 0;
    let mut instr_list = Vec::new();
    let mut label_map = HashMap::new();
    let mut found = false;

    for g in &pinstrs {
      match g{
        PInstr::PLabel(x) => {
          let y = x.clone();
          label_map.insert(x.clone(),pc);
          found = true;
        }
        PInstr::PPush(x) => {pc = pc + 1;},
        PInstr::PI(x) => {pc = pc + 1;}
      }
      }

      for temp_instruct in pinstrs {
        match temp_instruct{
          PInstr::PLabel(x) => {
          },
          PInstr::PPush(x) => {
            let d = label_map.get(&x).unwrap();
            instr_list.push(Push(Vloc(*d)));
          },
          PInstr::PI(x) =>instr_list.push(x)
        }

        }
Ok(instr_list)
}
