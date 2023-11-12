use std::any::Any;
use std::collections::HashMap;

pub use crate::input::*;
pub use crate::output::*;

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct NodeId(u64);

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct InputId(u64);

#[derive(Debug, Copy, Clone, Hash, Default, PartialEq, Eq)]
pub struct OutputId(u64);

struct Node {
    inputs: Vec<InputId>,
    outputs: Vec<OutputId>,
    function: Box<dyn Fn(&[&dyn Any], &mut [Box<dyn Output>])>,
}

pub struct Graph {
    inputs: HashMap<InputId, Box<dyn Input>>,
    outputs: HashMap<OutputId, Box<dyn Output>>,
    input_connections: HashMap<InputId, OutputId>,
    nodes: HashMap<NodeId, Node>,
    next_node_id: u64,
    next_input_id: u64,
    next_output_id: u64,
}

impl Graph {
    pub fn new() -> Self {
        Self {
            inputs: HashMap::new(),
            outputs: HashMap::new(),
            input_connections: HashMap::new(),
            nodes: HashMap::new(),
            next_node_id: 0,
            next_input_id: 0,
            next_output_id: 0,
        }
    }

    pub fn add_node(
        &mut self,
        inputs: Vec<Box<dyn Input>>,
        outputs: Vec<Box<dyn Output>>,
        function: Box<dyn Fn(&[&dyn Any], &mut [Box<dyn Output>])>,
    ) -> (NodeId, Vec<InputId>, Vec<OutputId>) {
        let mut input_ids = Vec::new();
        let mut output_ids = Vec::new();

        let node_id = self.get_next_node_id();

        for input in inputs {
            let input_id = self.get_next_input_id();
            input_ids.push(input_id);
            self.inputs.insert(input_id, input);
        }

        for output in outputs {
            let output_id = self.get_next_output_id();
            output_ids.push(output_id);
            self.outputs.insert(output_id, output);
        }

        self.nodes.insert(
            node_id,
            Node {
                inputs: input_ids.clone(),
                outputs: output_ids.clone(),
                function,
            },
        );

        (node_id, input_ids, output_ids)
    }

    pub fn connect(&mut self, input: &InputId, output: &OutputId) {
        self.input_connections.insert(*input, *output);
    }

    pub fn update(&mut self) {
        for node in self
            .calculate_node_update_order()
            .into_iter()
            .map(|node_id| &self.nodes[&node_id])
        {
            let mut outputs: Vec<_> = node
                .outputs
                .iter()
                .map(|output_id| self.outputs.remove(output_id).unwrap())
                .collect();

            let input_values: Vec<_> = node
                .inputs
                .iter()
                .map(|input_id| {
                    self.outputs
                        .get(self.input_connections.get(input_id).unwrap())
                        .unwrap()
                        .value_any()
                        .unwrap()
                })
                .collect();

            (node.function)(input_values.as_slice(), outputs.as_mut_slice());

            for (output_id, output) in std::iter::zip(node.outputs.iter(), outputs) {
                self.outputs.insert(*output_id, output);
            }
        }
    }

    pub fn get_input_value(&self, id: &InputId) -> Option<&dyn Any> {
        self.outputs[self.input_connections.get(id)?].value_any()
    }

    pub fn get_output_value(&self, id: &OutputId) -> Option<&dyn Any> {
        self.outputs.get(id)?.value_any()
    }

    fn calculate_node_update_order(&self) -> Vec<NodeId> {
        let mut ids: Vec<NodeId> = Vec::new();

        ids.extend(self.nodes.iter().filter_map(|(node_id, node)| {
            if node.inputs.is_empty() {
                Some(node_id)
            } else {
                None
            }
        }));

        ids.extend(self.nodes.iter().filter_map(|(node_id, node)| {
            if !node.inputs.is_empty() {
                Some(node_id)
            } else {
                None
            }
        }));

        ids
    }

    fn get_next_node_id(&mut self) -> NodeId {
        let node_id = NodeId(self.next_node_id);
        self.next_node_id += 1;
        node_id
    }

    fn get_next_input_id(&mut self) -> InputId {
        let input_id = InputId(self.next_input_id);
        self.next_input_id += 1;
        input_id
    }

    fn get_next_output_id(&mut self) -> OutputId {
        let output_id = OutputId(self.next_output_id);
        self.next_output_id += 1;
        output_id
    }
}
