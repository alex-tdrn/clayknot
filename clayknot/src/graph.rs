use std::collections::HashMap;
use std::vec;

pub use crate::node::*;

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
