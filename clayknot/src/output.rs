use std::any::{Any, TypeId};
use std::time::Instant;

pub trait Output {
    fn self_any(&self) -> &dyn Any;

    fn self_any_mut(&mut self) -> &mut dyn Any;

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
    fn self_any(&self) -> &dyn Any {
        self
    }

    fn self_any_mut(&mut self) -> &mut dyn Any {
        self
    }

    fn type_id(&self) -> TypeId {
        TypeId::of::<T>()
    }

    fn timestamp(&self) -> Instant {
        self.timestamp
    }
}
