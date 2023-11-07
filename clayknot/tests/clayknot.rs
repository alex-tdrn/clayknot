use clayknot::*;

use std::any::Any;

#[test]
fn test_foo() {
    assert_eq!(42, clayknot::foo());
}

// #[test]
// fn test_output() {
//     let time_before_creation = std::time::Instant::now();
//     let mut o: clayknot::OutputOf<u32> = clayknot::OutputOf::new(42);

//     assert_eq!(*o.get(), 42);
//     assert!(o.timestamp() > time_before_creation);

//     let time_before_set = std::time::Instant::now();
//     assert!(o.timestamp() < time_before_set);
//     o.set(30);
//     assert!(o.timestamp() > time_before_set);

//     assert_eq!(*o.get(), 30);
// }

#[test]
fn hmmmm() {
    let mut g = clayknot::Graph::new();

    let mut pampam = Box::new(clayknot::OutputOf::new(42));

    let (node_id, _, _) = g.add_node(
        vec![],
        vec![Box::new(clayknot::OutputOf::new(40)), pampam],
        Box::new(
            |inputs: &[&dyn clayknot::Input], outputs: &mut [Box<dyn clayknot::Output>]| {
                println!("hello from inside node! outputs:");
                for o in outputs {
                    let o_downcasted = o
                        .self_any_mut()
                        .downcast_mut::<clayknot::OutputOf<i32>>()
                        .unwrap();

                    println!(
                        "value: {}; timestamp: {:?}",
                        o_downcasted.get(),
                        o_downcasted.timestamp()
                    );

                    o_downcasted.set(o_downcasted.get() + 1);
                }
            },
        ),
    );

    g.run_node(&node_id);
    g.run_node(&node_id);
}
