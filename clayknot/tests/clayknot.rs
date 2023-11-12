use clayknot::*;

#[test]
fn source_node() {
    let mut g = Graph::new();

    let (_, _, outputs) = g.add_node(
        vec![],
        vec![Box::new(OutputOf::<u32>::new("Result"))],
        Box::new(|_: &[&dyn Input], outputs: &mut [Box<dyn Output>]| {
            outputs
                .first_mut()
                .unwrap()
                .self_any_mut()
                .downcast_mut::<OutputOf<u32>>()
                .unwrap()
                .set(42);
        }),
    );

    let output_id = outputs.first().unwrap();

    assert!(g.get_output_value(output_id).is_none());

    g.update();

    assert_eq!(
        *g.get_output_value(output_id)
            .unwrap()
            .downcast_ref::<u32>()
            .unwrap(),
        42
    );
}
