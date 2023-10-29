#[test]
fn test_foo() {
    assert_eq!(42, clayknot::foo());
}

#[test]
fn test_graph() {
    let g = clayknot::Graph::new();
    assert_eq!(g.nodes.len(), 0);
}
