use std::any::TypeId;
use std::marker::PhantomData;
use std::time::Instant;

pub trait Input {
    fn type_id(&self) -> TypeId;

    fn timestamp(&self) -> Instant;
}

pub struct InputOf<T: 'static> {
    name: String,
    phantom_value: PhantomData<T>,
    timestamp: Instant,
}

impl<T> InputOf<T> {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            phantom_value: PhantomData,
            timestamp: std::time::Instant::now(),
        }
    }
}

impl<T> Input for InputOf<T> {
    fn type_id(&self) -> TypeId {
        TypeId::of::<T>()
    }

    fn timestamp(&self) -> Instant {
        self.timestamp
    }
}
