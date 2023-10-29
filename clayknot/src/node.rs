pub use crate::input::*;
pub use crate::output::*;

pub struct Node {
    inputs: Vec<Box<dyn Input>>,
    outputs: Vec<Box<dyn Output>>,
}
