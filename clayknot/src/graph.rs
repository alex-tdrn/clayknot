use std::collections::HashMap;
use std::vec;

pub use crate::node::*;

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct NodeId(u64);

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct InputId(u64);

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct OutputId(u64);

pub struct Graph {
    nodes: HashMap<NodeId, Node>,
    connections: Vec<(InputId, OutputId)>,
}

impl Graph {
    pub fn new() -> Self {
        Self {
            nodes: HashMap::new(),
            connections: Vec::new(),
        }
    }
}
