use std::vec;

pub use crate::input::*;
pub use crate::output::*;

pub struct Node {
    inputs: Vec<Input>,
    outputs: Vec<Box<dyn Output>>,
}
