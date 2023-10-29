use clayknot::*;

#[test]
fn test_foo() {
    assert_eq!(42, clayknot::foo());
}

#[test]
fn test_output() {
    let time_before_creation = std::time::Instant::now();
    let mut o: clayknot::OutputOf<u32> = clayknot::OutputOf::new(42);

    assert_eq!(*o.get(), 42);
    assert!(o.timestamp() > time_before_creation);

    let time_before_set = std::time::Instant::now();
    assert!(o.timestamp() < time_before_set);
    o.set(30);
    assert!(o.timestamp() > time_before_set);

    assert_eq!(*o.get(), 30);
}

#[test]
fn test_graph() {
    let g = clayknot::Graph::new();
    assert_eq!(g.nodes.len(), 0);
}
