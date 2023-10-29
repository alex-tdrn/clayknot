use std::collections::HashMap;

pub use crate::input::*;
pub use crate::output::*;

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct NodeId(u64);

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
struct InputId(u64);

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
struct OutputId(u64);

struct Node {
    inputs: Vec<InputId>,
    outputs: Vec<OutputId>,
    function: Box<dyn Fn(Vec<&dyn Input>, Vec<&mut dyn Output>)>,
}

pub struct Graph {
    inputs: HashMap<InputId, Box<dyn Input>>,
    outputs: HashMap<OutputId, Box<dyn Output>>,
    nodes: HashMap<NodeId, Node>,
}

impl Graph {
    pub fn run_node(&mut self, node: &NodeId) {
        let mut inputs: Vec<&dyn Input> = Vec::new();
        let mut outputs: Vec<&mut dyn Output> = Vec::new();

        (self.nodes.get(node).unwrap().function)(inputs, outputs);
    }
}
