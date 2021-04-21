#![warn(clippy::all)]

use std::env;
use std::fs::OpenOptions;
use std::io::{self, Read};
use std::path::Path;

use grumpy::ir::Prog;

fn main() -> io::Result<()> {
    // Read input file (command line argument at index 1).
    let path_str = env::args().nth(1).expect("missing file argument");
    let path = Path::new(&path_str);
    let mut file = OpenOptions::new().read(true).open(path)?;

    // Load file contents into a string.
    let mut buf = String::new();
    file.read_to_string(&mut buf)?;

    // Parse program from the string.
    let prog: Prog = buf.parse()?;
    // Compile program to assembly (pseudo-instructions).
    let pinstrs = prog.compile()?;

    // Print compiled program.
    for pinstr in pinstrs {
        println!("{}", pinstr.to_string())
    }
    
    Ok(())
}
