use clayknot::*;

#[test]
fn source_node() {
    let mut g = Graph::new();

    let output_id = {
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

        outputs.first().unwrap().clone()
    };

    assert!(g.get_output_value(&output_id).is_none());

    g.update();

    assert_eq!(
        *g.get_output_value(&output_id)
            .unwrap()
            .downcast_ref::<u32>()
            .unwrap(),
        42
    );
}

#[test]
fn two_connected_nodes() {
    let mut g = Graph::new();

    let output_id_a = {
        let (_, _, outputs_a) = g.add_node(
            vec![],
            vec![Box::new(OutputOf::<u32>::new("Output A"))],
            Box::new(|_: &[&dyn Input], outputs: &mut [Box<dyn Output>]| {
                outputs
                    .first_mut()
                    .unwrap()
                    .self_any_mut()
                    .downcast_mut::<OutputOf<u32>>()
                    .unwrap()
                    .set(1);
            }),
        );

        outputs_a.first().unwrap().clone()
    };

    let (input_id_b, output_id_b) = {
        let (_, inputs_b, outputs_b) = g.add_node(
            vec![Box::new(InputOf::<u32>::new("Input B"))],
            vec![Box::new(OutputOf::<u32>::new("Output B"))],
            Box::new(|_: &[&dyn Input], outputs: &mut [Box<dyn Output>]| {
                todo!();
                outputs
                    .first_mut()
                    .unwrap()
                    .self_any_mut()
                    .downcast_mut::<OutputOf<u32>>()
                    .unwrap()
                    .set(42);
            }),
        );

        (
            inputs_b.first().unwrap().clone(),
            outputs_b.first().unwrap().clone(),
        )
    };

    g.update();

    assert_eq!(
        *g.get_output_value(&output_id_a)
            .unwrap()
            .downcast_ref::<u32>()
            .unwrap(),
        1
    );

    assert_eq!(
        *g.get_input_value(&input_id_b)
            .unwrap()
            .downcast_ref::<u32>()
            .unwrap(),
        1
    );

    assert_eq!(
        *g.get_output_value(&output_id_b)
            .unwrap()
            .downcast_ref::<u32>()
            .unwrap(),
        2
    );
}
