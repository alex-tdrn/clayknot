use std::any::TypeId;
use std::time::Instant;

pub trait Output {
    fn type_id(&self) -> TypeId;

    fn timestamp(&self) -> Instant;
}

pub struct OutputOf<T: 'static> {
    value: T,
    timestamp: Instant,
}

impl<T> OutputOf<T> {
    pub fn new(value: T) -> Self {
        Self {
            value,
            timestamp: std::time::Instant::now(),
        }
    }

    pub fn set(&mut self, value: T) {
        self.value = value;
        self.timestamp = std::time::Instant::now();
    }

    pub fn get(&self) -> &T {
        &self.value
    }
}

impl<T> Output for OutputOf<T> {
    fn type_id(&self) -> TypeId {
        TypeId::of::<T>()
    }

    fn timestamp(&self) -> Instant {
        self.timestamp
    }
}
