pub use crate::port::*;

pub trait Output: Port {}

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

impl<T> Port for OutputOf<T> {
    fn type_id(&self) -> TypeId {
        TypeId::of::<T>()
    }

    fn timestamp(&self) -> Instant {
        self.timestamp
    }
}

impl<T> Output for OutputOf<T> {}
