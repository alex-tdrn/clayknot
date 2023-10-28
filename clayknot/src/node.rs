use std::vec;

pub use crate::input::*;
pub use crate::output::*;

pub struct NodeId(u64);

pub struct Node {
    id: NodeId,
    inputs: Vec<Input>,
    outputs: Vec<Output>,
}
