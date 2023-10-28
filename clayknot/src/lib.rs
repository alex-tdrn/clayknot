mod graph;
mod input;
mod node;
mod output;

use crate::graph::*;

pub fn foo() -> i64 {
    42
}

pub fn make_example_graph() -> Graph {
    let g = Graph::new();
    g
}
